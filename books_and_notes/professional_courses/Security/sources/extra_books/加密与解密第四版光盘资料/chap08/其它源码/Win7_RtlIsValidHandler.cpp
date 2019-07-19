//MmExecutionFlags on Win7
#define MEM_EXECUTE_OPTION_DISABLE 0x1 
#define MEM_EXECUTE_OPTION_ENABLE 0x2
#define MEM_EXECUTE_OPTION_DISABLE_THUNK_EMULATION 0x4
#define MEM_EXECUTE_OPTION_PERMANENT 0x8
#define MEM_EXECUTE_OPTION_EXECUTE_DISPATCH_ENABLE 0x10
#define MEM_EXECUTE_OPTION_IMAGE_DISPATCH_ENABLE 0x20
#define MEM_EXECUTE_OPTION_DISABLE_EXCEPTIONCHAIN_VALIDATION 0x40
#define MEM_EXECUTE_OPTION_VALID_FLAGS 0x7f

// 该函数的功能是根据ControlPc查找它所在的模块的SafeSEHTable
PVOID
RtlLookupFunctionTable (
	IN PVOID ControlPc,
	OUT PVOID *ImageBase,
	OUT PULONG SizeOfTable
    );

VOID
RtlInvalidHandlerDetected(
	IN PVOID Handler, 
	IN PULONG FunctionTable,
	IN ULONG FunctionTableLength
    );

BOOLEAN __stdcall RtlIsValidHandler(PVOID Handler, int ProcessInformation)
{
  PVOID FunctionTable; 
  LONG High,Low,Middle; 
  ULONG HandlerVA;
  ULONG FunctionEntry;
  NTSTATUS status; 
  void *v10; 
  MEMORY_BASIC_INFORMATION MemoryInformation; 
  ULONG ReturnLength; 
  PVOID Base; 
  PVOID HandlerTable; 
  ULONG FunctionTableLength; 

  FunctionTable = RtlLookupFunctionTable(Handler, &Base, &FunctionTableLength);
  HandlerTable = FunctionTable;
  if ( FunctionTable )
  {
    High = FunctionTableLength;
    if ( FunctionTableLength )
    {
      if ( FunctionTable != (PVOID)-1 || FunctionTableLength != -1 )
      {
        HandlerVA = (ULONG)Handler - (ULONG)Base;
        Low = 0;
        if ( (FunctionTableLength & 0x80000000) == 0 )
        {
          do
          {
            Middle = (Low + High) /2;
            FunctionEntry = *((DWORD *)FunctionTable + Middle);
            if ( HandlerVA < FunctionEntry )
            {
              if ( !Middle )
                break;
              High = Middle - 1;
            }
            else
            {
              if ( HandlerVA <= FunctionEntry )
                return 1;
              Low = Middle + 1;
            }
          }
          while ( High >= Low );
        }
        RtlInvalidHandlerDetected(Handler, FunctionTable, FunctionTableLength);
      }
      return 0;
    }
  }
  if ( !ProcessInformation
    && ZwQueryInformationProcess(NtCurrentProcess(), ProcessExecuteFlags, &ProcessInformation, 4u, 0) < 0 )
  {
    ProcessInformation = 0;
  }
  if ( (ProcessInformation & 0x30) == 0x30 )
    return 1;
  status = NtQueryVirtualMemory(NtCurrentProcess(), Handler, 0, &MemoryInformation, sizeof(MEMORY_BASIC_INFORMATION), &ReturnLength);
  if ( !NT_SUCCESS(status) )
    return 1;
  if ( !(MemoryInformation.Protect & 0xF0) )
  {
    if ( !(ProcessInformation & MEM_EXECUTE_OPTION_EXECUTE_DISPATCH_ENABLE) )
    {
      RtlInvalidHandlerDetected(Handler, (PVOID)-2, -2);
      return 0;
    }
    return 1;
  }
  if ( MemoryInformation.Type == SEC_IMAGE )
  {
    RtlCaptureImageExceptionValues(v10, (PVOID *)MemoryInformation.AllocationBase, (PULONG)&HandlerTable);
    if ( HandlerTable && FunctionTableLength )
      return 0;
    return 1;
  }
  return ProcessInformation & MEM_EXECUTE_OPTION_IMAGE_DISPATCH_ENABLE;
}