//Exception Flags
#define EXCEPTION_NONCONTINUABLE 0x1    // Noncontinuable exception
#define EXCEPTION_UNWINDING 0x2         // Unwind is in progress
#define EXCEPTION_EXIT_UNWIND 0x4       // Exit unwind is in progress
#define EXCEPTION_STACK_INVALID 0x8     // Stack out of limits or unaligned
#define EXCEPTION_NESTED_CALL 0x10      // Nested exception handler call
#define EXCEPTION_TARGET_UNWIND 0x20    // Target unwind in progress
#define EXCEPTION_COLLIDED_UNWIND 0x40  // Collided exception handler call

//MmExecutionFlags on Win7
#define MEM_EXECUTE_OPTION_DISABLE 0x1 
#define MEM_EXECUTE_OPTION_ENABLE 0x2
#define MEM_EXECUTE_OPTION_DISABLE_THUNK_EMULATION 0x4
#define MEM_EXECUTE_OPTION_PERMANENT 0x8
#define MEM_EXECUTE_OPTION_EXECUTE_DISPATCH_ENABLE 0x10
#define MEM_EXECUTE_OPTION_IMAGE_DISPATCH_ENABLE 0x20
#define MEM_EXECUTE_OPTION_DISABLE_EXCEPTIONCHAIN_VALIDATION 0x40
#define MEM_EXECUTE_OPTION_VALID_FLAGS 0x7f

//NtGlobalFlag
#define FLG_ENABLE_CLOSE_EXCEPTIONS     0x00400000      // kernel mode only
#define FLG_ENABLE_EXCEPTION_LOGGING    0x00800000      // kernel mode only

//Part of ProcessInformationClass
#define ProcessExecuteFlags	34

typedef struct _DISPATCHER_CONTEXT {
	PEXCEPTION_REGISTRATION_RECORD RegistrationPointer;
} DISPATCHER_CONTEXT;

//
// Execute handler for exception function prototype.
//

EXCEPTION_DISPOSITION
	RtlpExecuteHandlerForException (
	IN PEXCEPTION_RECORD ExceptionRecord,
	IN PVOID EstablisherFrame,
	IN OUT PCONTEXT ContextRecord,
	IN OUT PVOID DispatcherContext,
	IN PEXCEPTION_ROUTINE ExceptionRoutine
	);

VOID
	RtlpGetStackLimits (
	OUT PULONG LowLimit,
	OUT PULONG HighLimit
	);

EXCEPTION_DISPOSITION
	RtlCallVectoredExceptionHandlers (
	IN PEXCEPTION_RECORD ExceptionRecord,
	IN OUT PCONTEXT ContextRecord
	);

EXCEPTION_DISPOSITION
	RtlCallVectoredContinueHandlers (
	IN PEXCEPTION_RECORD ExceptionRecord,
	IN OUT PCONTEXT ContextRecord
	);

PEXCEPTION_REGISTRATION_RECORD
	RtlpGetRegistrationHead (
	VOID
	);

BOOLEAN
	RtlIsValidHandler (
	IN PEXCEPTION_ROUTINE Handler,
	IN ULONG ProcessExecuteFlag
	);

BOOLEAN __stdcall RtlDispatchException(PEXCEPTION_RECORD pExcptRec, CONTEXT *pContext)
{
	BOOLEAN Completion; 
	PEXCEPTION_RECORD pExcptRec;
	EXCEPTION_REGISTRATION_RECORD *RegistrationPointerForCheck;
	EXCEPTION_REGISTRATION_RECORD *RegistrationPointer;
	EXCEPTION_REGISTRATION_RECORD *NestedRegistration;
	EXCEPTION_DISPOSITION Disposition; 
	EXCEPTION_RECORD ExceptionRecord1;
	DISPATCHER_CONTEXT DispatcherContext;
	ULONG ProcessExecuteOption;
	ULONG StackBase,StackLimit; 
	BOOLEAN IsSEHOPEnable;
	NTSTATUS status;

	Completion = FALSE;

	// 首先调用VEH异常处理例程，其返回值包括EXCEPTION_CONTINUE_EXECUTION (0xffffffff)和EXCEPTION_CONTINUE_SEARCH (0x0)两种情况
	// 这是从Windows XP开始加入的新的异常处理方式
	// 返回值不是EXCEPTION_CONTINUE_SEARCH，那么就结束异常分发过程
	if (RtlCallVectoredExceptionHandlers(pExcptRec, pContext)  !=  EXCEPTION_CONTINUE_SEARCH )
	{
		Completion = TRUE;
	}
	else
	{
		// 获取栈的内存范围
		RtlpGetStackLimits(&StackLimit, &StackBase);
		ProcessExecuteOption = 0;

		// 从fs:[0]获取SEH链的头节点
		RegistrationPointerForCheck = RtlpGetRegistrationHead();

		// 默认假设SEHOP机制已经启用，这是一种对SEH链的安全性进行增强验证的机制
		IsSEHOPEnable = TRUE; 

		// 查询进程的ProcessExecuteFlags标志，决定是否进行SEHOP验证
		status = ZwQueryInformationProcess(NtCurrentProcess(), ProcessExecuteFlags, &ProcessExecuteOption, sizeof(ULONG), NULL) ;

		// 在查询失败，或者没有设置标志位时，进行SEHOP增强验证
		// 也就是说，只有在明确查询到禁用了SEHOP时才不会进行增强验证
		if ( NT_SUCCESS(status) 
			&& (ProcessExecuteOption & MEM_EXECUTE_OPTION_DISABLE_EXCEPTIONCHAIN_VALIDATION) )
		{
			// 若确实未开启SEHOP增强校验机制，设置此标志
			IsSEHOPEnable = FALSE; 
		}
		else
		{
			// 否则，进行开始SEHOP验证
			if ( RegistrationPointerForCheck == -1 )
				break;

			//验证SEH链中各个结点的有效性并遍历至最后一个结点
			do
			{
				// 若发生以下情况，认为栈无效，此时不再执行基于栈的SEH处理
				    // 1.SEH节点不在栈中
				if ( (ULONG)RegistrationPointerForCheck < StackLimit 
					|| (ULONG)RegistrationPointerForCheck + 8 > StackBase
					// 2.SEH节点的位置没有按ULONG对齐
					|| (ULONG)RegistrationPointerForCheck & 3 
					// 3.Handler在栈中
					|| ((ULONG)RegistrationPointerForCheck->Handler < StackLimit || (ULONG)RegistrationPointerForCheck->Handler >= StackBase) )
				{
					pExcptRec->ExceptionFlags |= EXCEPTION_STACK_INVALID;
					goto DispatchExit;
				}
				// 取SEH链的下一个结点
				RegistrationPointerForCheck = RegistrationPointerForCheck->Next;
			}
			while ( RegistrationPointerForCheck != -1 );

			// 此时RegistrationPointerForCheck指向最后一个节点
			// 如果TEB->SameTebFlags中的RtlExceptionAttached位(第9位)被设置，但最后一个结点的Handler却不是预设的安全SEH，那么SEHOP校验不通过，不再执行任何SEHHandler
			if ((NtCurrentTeb()->SameTebFlags & 0x200) && RegistrationPointerForCheck->Handler != FinalExceptionHandler)
			{
				goto DispatchExit;
			}
		}
		
		// 从fs:[0]获取SEH链的头节点
		RegistrationPointer = RtlpGetRegistrationHead();
		NestedRegistration = NULL;

		// 遍历SEH链表执行Handler
		while ( TRUE )
		{
			if ( RegistrationPointer == -1 ) //-1表示SEH链的结束
				goto DispatchExit;

			// 若SEHOP机制未开启，则这里必须进行校验，反之则不需要，因为SEHOP机制已经验证过了
			if ( !IsSEHOPEnable )
			{
				if ( (ULONG)RegistrationPointer < StackLimit 
					|| (ULONG)RegistrationPointer + 8 > StackBase 
					|| (ULONG)RegistrationPointer & 3 
					|| ((ULONG)RegistrationPointer->Handler < StackLimit || (ULONG)RegistrationPointer->Handler >= StackBase) )
				{
					pExcptRec->ExceptionFlags |= EXCEPTION_STACK_INVALID;
					goto DispatchExit;
				}
			}

			// 调用RtlIsValidHandler对Handler进行增强验证，也就是SafeSEH机制
			if (!RtlIsValidHandler(RegistrationPointer->Handler, ProcessExecuteOption))
			{
				pExcptRec->ExceptionFlags |= EXCEPTION_STACK_INVALID;
				goto DispatchExit;
			}

			// 执行SEHHandler
			Disposition = RtlpExecuteHandlerForException(pExcptRec, RegistrationPointer, pContext, &DispatcherContext, RegistrationPointer->Handler);
			if ( NestedRegistration == RegistrationPointer )
			{
				pExcptRec->ExceptionFlags &=  (~EXCEPTION_NESTED_CALL);
				NestedRegistration = NULL;
			}

			// 检查SEHHandler的执行结果
			switch(Disposition)
			{
			case ExceptionContinueExecution :
				if ((ExceptionRecord->ExceptionFlags &
					EXCEPTION_NONCONTINUABLE) != 0) {
						ExceptionRecord1.ExceptionCode = STATUS_NONCONTINUABLE_EXCEPTION;
						ExceptionRecord1.ExceptionFlags = EXCEPTION_NONCONTINUABLE;
						ExceptionRecord1.ExceptionRecord = ExceptionRecord;
						ExceptionRecord1.NumberParameters = 0;
						RtlRaiseException(&ExceptionRecord1);

				} else {
					Completion = TRUE;
					goto DispatchExit;
				}

			case ExceptionContinueSearch :
				if (ExceptionRecord->ExceptionFlags & EXCEPTION_STACK_INVALID)
					goto DispatchExit;

				break;

			case ExceptionNestedException :
				ExceptionRecord->ExceptionFlags |= EXCEPTION_NESTED_CALL;
				if (DispatcherContext.RegistrationPointer > NestedRegistration) {
					NestedRegistration = DispatcherContext.RegistrationPointer;
				}

				break;

			default :
				ExceptionRecord1.ExceptionCode = STATUS_INVALID_DISPOSITION;
				ExceptionRecord1.ExceptionFlags = EXCEPTION_NONCONTINUABLE;
				ExceptionRecord1.ExceptionRecord = ExceptionRecord;
				ExceptionRecord1.NumberParameters = 0;
				RtlRaiseException(&ExceptionRecord1);
				break;
			}

			// 取SEH链的下一个结点
			RegistrationPointer = RegistrationPointer->Next;           // Next
		}
	}

DispatchExit:

	// 调用VEH的ContinueHandler
	// 只要RtlDispatchException函数正常返回，那么ContinueHandler总会在SEH执行完毕后被调用
	RtlCallVectoredContinueHandlers(pExcptRec, pContext);
	return Completion;
}