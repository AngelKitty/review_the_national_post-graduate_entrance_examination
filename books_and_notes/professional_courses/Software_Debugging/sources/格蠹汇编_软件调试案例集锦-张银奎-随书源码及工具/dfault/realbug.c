//----------------------------------------------------------------------
//
// RealBug - simple driver to serve as debuggee for training purpose.
// 
// Copyright (C) 2006 Raymond Zhang
// Advanced Debugging - http://advdbg.org
//
//----------------------------------------------------------------------
#include "ntddk.h"
#include "..\Imbuggy\ioctlcmd.h"
#include "debug.h"

#define MAX_LOOP 1
VOID SehFilter()
{
	int i;
	LARGE_INTEGER li;
	li.QuadPart=-100000;
	for(i=0;i<MAX_LOOP;i++)
	{
		DBGOUT(("Execute SEH filter now %d",KeGetCurrentIrql()));
		KeDelayExecutionThread(KernelMode,TRUE, &li); 
	}
}
VOID SehHandler()
{
	DBGOUT(("Execute handler now"));
}
VOID SEH()
{
	__try
	{
		__try
		{
			*(int *)0=1;
		}
		__except(SehFilter(), EXCEPTION_CONTINUE_SEARCH)
		{
			DBGOUT(("Never should reach here"));
		}
	}
    __except(SehFilter(), EXCEPTION_EXECUTE_HANDLER)
	{
		SehHandler();
	}
}
VOID UncaughtException()
{
	*(int *)0=1;
}
VOID NullPointer()
{
	*(int *)0=1;
}
#pragma warning(disable : 4723)
VOID PropDivideZero()
{
	int n,m;
	n=1;
	m=0;
	__try
	{
		n=n/m;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DBGOUT(("Caught divide by zero safely."));
	}
}
#pragma warning(default : 4723)

VOID DivideZero()
{
	int n,m;
	PropDivideZero();

	n=1;
	m=0;
	n=n/m;
}

VOID StackBufferOverflow(PCHAR psz)
{
	int i=0;
    CHAR szVar[10]="aaaaa...";

	for(i=0; i< 1000;i++)
		szVar[i]=psz[i];

}

VOID StackOverflow(ULONG ulRecursive)
{
    if(--ulRecursive>0)
        StackOverflow(ulRecursive);
}

/*------------------------------------------------------------------
Let this thread wait in kernel forevel, this will become a 
unkillable thread in XP.
-------------------------------------------------------------------*/
VOID WaitForever(VOID)
{
	KEVENT evt;
	LARGE_INTEGER nInterval={1000,0};
	
	KeInitializeEvent(&evt, NotificationEvent, FALSE);
	while(1)
		KeWaitForSingleObject(&evt, Executive, KernelMode, TRUE, &nInterval);
}
VOID RoamAtIRQL(ULONG ulIRQL)
{
	KIRQL OldIrql;
	KeRaiseIrql((KIRQL)ulIRQL, &OldIrql);
	while(1) 
		_asm{hlt};

	KeLowerIrql(OldIrql);
}
VOID LeakPool(ULONG ulTemp)
{
    PCHAR  Ptr;

    Ptr = ExAllocatePoolWithTag( PagedPool, ulTemp,  'GBDA');
	// left it as leak, will bug check when unload the driver
	//
}

VOID AccessAnywhere()
{
	PKTHREAD Thread;
	Thread = KeGetCurrentThread();
	// Dog food for OT, I am crazy...
	memset(Thread, 0xCCCCCCCC, 0x1c0);
}
ULONG ReadMemory(ULONG ulAddr)
{
	ULONG ulResult=0;
	__try
	{
		ulResult=*(ULONG *)ulAddr;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("execption occurred when accessing 0x%x", ulAddr);
	}
	return ulResult;
}
void FileIoDispatch(IN PDEVICE_OBJECT DeviceObject, ULONG ulIrql)
{
    UNICODE_STRING              FileName;
    OBJECT_ATTRIBUTES           FileAttributes;
    IO_STATUS_BLOCK             IoStatus;
    HANDLE                      FileHandle;
    NTSTATUS                    status;
	KIRQL                       OldIrql;
    FILE_POSITION_INFORMATION   position;

    if(ulIrql>0)
        KeRaiseIrql((KIRQL)ulIrql, &OldIrql);

    //
    // Assume the directory is a temp directory under %windir%
    //
    RtlInitUnicodeString(&FileName, L"\\SystemRoot\\temp\\FileFromKernel.txt");

    InitializeObjectAttributes( &FileAttributes,
                                &FileName,
                                OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
                                NULL, // RootDirectory
                                NULL // SecurityDescriptor
                                );

    status = ZwCreateFile (
                    &FileHandle,
                    SYNCHRONIZE | GENERIC_WRITE | GENERIC_READ,
                    &FileAttributes,
                    &IoStatus,
                    NULL,// alloc size = none
                    FILE_ATTRIBUTE_NORMAL,
                    FILE_SHARE_READ,
                    FILE_OPEN_IF,
                    FILE_SYNCHRONOUS_IO_NONALERT |FILE_NON_DIRECTORY_FILE,
                    NULL,// eabuffer
                    0// ealength
                    );
    if (!NT_SUCCESS(status)) {
        DBGOUT(("ZwCreateFile failed with status 0x%x", 
                   status));
    }
    //
    // Set the file position to the beginning of the file.
    //
    position.CurrentByteOffset.QuadPart = 0;

    status = ZwSetInformationFile(FileHandle,
                         &IoStatus,
                         &position,
                         sizeof(FILE_POSITION_INFORMATION),
                         FilePositionInformation);

    status = ZwWriteFile(FileHandle,
                        NULL,//   Event,
                        NULL,// PIO_APC_ROUTINE  ApcRoutine
                        NULL,// PVOID  ApcContext
                        &IoStatus,
                        FileName.Buffer,
                        FileName.Length,
                        0, // ByteOffset
                        NULL // Key
                        );
    if (!NT_SUCCESS(status))
    {
        DBGOUT(("ZwWriteFile failed with status 0x%x", 
                   status));
    }
    status = ZwClose(FileHandle);

    if(ulIrql>0)
    	KeLowerIrql(OldIrql);
}
VOID
WaitAtHighIrqlThread(
    __in PVOID Context
    )
{
	KIRQL OldIrql = 0;
	KEVENT evt;
	LARGE_INTEGER nInterval={50000,0};
    PVOID    Object;
    PWAIT_IRQL_PARA pWaitIrqlPara=(PWAIT_IRQL_PARA)Context;
    NTSTATUS Status;
    
    Status = ObReferenceObjectByHandle(
        pWaitIrqlPara->hEvent,
        SYNCHRONIZE,
        *ExEventObjectType,
        KernelMode,
        &Object,
        NULL);
    if(Status != STATUS_SUCCESS)
    {
        DBGOUT(("bad handle %p met in WaitAtIrql\n", pWaitIrqlPara->hEvent));
        return;
    }
	
    KeRaiseIrql((KIRQL)pWaitIrqlPara->ulIRQL,&OldIrql);
	
    while(1)
    {
        DBGOUT(("going to wait at IRQL = %d ...\n", pWaitIrqlPara->ulIRQL));

	    KeWaitForSingleObject(Context,Executive,KernelMode,FALSE,&nInterval);
    }

    DBGOUT(("Escaped from wait at DISPATCH_LEVEL...\n"));

    KeLowerIrql(OldIrql);
}

void  WaitAtIrql(PWAIT_IRQL_PARA pWaitIrqlPara)
{
    NTSTATUS Status = STATUS_SUCCESS;
    HANDLE   hThreadHighIrql; 

    Status = PsCreateSystemThread(
        &hThreadHighIrql,
        0,
        NULL,
        NtCurrentProcess(),
        0,
        (PKSTART_ROUTINE)WaitAtHighIrqlThread,
        pWaitIrqlPara);

    DBGOUT(("PsCreateSystemThread returned with 0x%x\n", Status));
}
NTSTATUS  
RealBugDeviceControl( 
    IN PFILE_OBJECT FileObject, 
    IN BOOLEAN Wait,
    IN PVOID InputBuffer, 
    IN ULONG InputBufferLength, 
    OUT PVOID OutputBuffer, 
    IN ULONG OutputBufferLength, 
    IN ULONG IoControlCode, 
    OUT PIO_STATUS_BLOCK IoStatus, 
    IN PDEVICE_OBJECT DeviceObject 
    ) 
{
	ULONG ulTemp=0;
    IoStatus->Status = STATUS_SUCCESS;
	IoStatus->Information = 0;

	switch ( IoControlCode ) {
    case IOCTL_DIVIDEBYZERO:
        DivideZero();
        break;
    case IOCTL_NULLPOINTER:
        NullPointer();
        break;
    case IOCTL_SEH:
        SEH();
        break;
    case IOCTL_UNCAUGHT_EXCEPTION: 
        UncaughtException();
        break;
	case IOCTL_BUGCHECK:
		KeBugCheck(0x88888888);
		break;
	case IOCTL_STACK_BUFFER_OVERFLOW:
		StackBufferOverflow("zzzzzz...");
		break;
    case IOCTL_STACK_OVERFLOW:
		ulTemp=(InputBufferLength==sizeof(ULONG))?
			*(ULONG*)InputBuffer:888888;
        StackOverflow(ulTemp);
        break;
	case IOCTL_HANG_IN_KERNEL:
		WaitForever();
		break;
	case IOCTL_ROAM_AT_IRQL:
		ulTemp=(InputBufferLength==sizeof(ULONG))?
			*(ULONG*)InputBuffer:HIGH_LEVEL;
		RoamAtIRQL(ulTemp);
		break;
	case IOCTL_POOL_LEAK:
		ulTemp=(InputBufferLength==sizeof(ULONG))?
			*(ULONG*)InputBuffer:1024;
		LeakPool(ulTemp);
		break;
	case IOCTL_ACCESS_ANYWHERE:
		AccessAnywhere();
		break;
	case IOCTL_READ_MEMORY:
		ulTemp=(InputBufferLength==sizeof(ULONG))?
			*(ULONG*)InputBuffer:0xc0000000;
		*(ULONG*)OutputBuffer=ReadMemory(ulTemp);
		IoStatus->Information=sizeof(ULONG);
		break;
    case IOCTL_FILEIO_AT_IRQL:
		ulTemp=(InputBufferLength==sizeof(ULONG))?
			*(ULONG*)InputBuffer:DISPATCH_LEVEL;
        FileIoDispatch(DeviceObject, ulTemp); 
        break;
    case IOCTL_WAIT_AT_IRQL:
		if(InputBufferLength<sizeof(WAIT_IRQL_PARA))
        {
            IoStatus->Status = STATUS_INVALID_PARAMETER;
            DBGOUT(("bad size parameter %d\n", InputBufferLength));
            break;
        }
        WaitAtIrql((PWAIT_IRQL_PARA) InputBuffer ); 
        break;
    default: 
        IoStatus->Status = STATUS_NOT_SUPPORTED;
        break;
	}
	return IoStatus->Status;
}


NTSTATUS 
RealBugDispatch( 
    IN PDEVICE_OBJECT DeviceObject, 
    IN PIRP Irp 
    )
{
	PIO_STACK_LOCATION      iosp;
	PVOID                   inputBuffer;
	PVOID                   outputBuffer;
	ULONG                   inputBufferLength;
	ULONG                   outputBufferLength;
	ULONG                   ioControlCode;
	NTSTATUS                status;

    //
    // Switch on the request type
    //
	iosp = IoGetCurrentIrpStackLocation (Irp);
	switch (iosp->MajorFunction) {

    case IRP_MJ_CREATE:
    case IRP_MJ_CLOSE:

        status = STATUS_SUCCESS;
        break;

	case IRP_MJ_DEVICE_CONTROL:

        inputBuffer        = Irp->AssociatedIrp.SystemBuffer;
        inputBufferLength  = iosp->Parameters.DeviceIoControl.InputBufferLength;
        outputBuffer       = Irp->AssociatedIrp.SystemBuffer;
        outputBufferLength = iosp->Parameters.DeviceIoControl.OutputBufferLength;
        ioControlCode      = iosp->Parameters.DeviceIoControl.IoControlCode;
		status = RealBugDeviceControl( iosp->FileObject, TRUE,
                                       inputBuffer, inputBufferLength, 
                                       outputBuffer, outputBufferLength,
                                       ioControlCode, &Irp->IoStatus, 
                                       DeviceObject );
		break;

    default:

        status = STATUS_INVALID_DEVICE_REQUEST;
        break;        
	}

    //
    // Complete the request
    //
    Irp->IoStatus.Status = status;
	IoCompleteRequest( Irp, IO_NO_INCREMENT );
	return status;
}


VOID 
RealBugUnload( 
    IN PDRIVER_OBJECT DriverObject 
    )
{
	WCHAR                   deviceLinkBuffer[]  = L"\\Device\\RealBug";
	UNICODE_STRING          deviceLinkUnicodeString;

    //
	// Delete the symbolic link for our device
    //
	RtlInitUnicodeString( &deviceLinkUnicodeString, deviceLinkBuffer );
	IoDeleteSymbolicLink( &deviceLinkUnicodeString );

    //
	// Delete the device object
    //
	IoDeleteDevice( DriverObject->DeviceObject );
}
NTSTATUS RealBugPower(
    IN PDEVICE_OBJECT    DeviceObject,
    IN PIRP              Irp)
{
    PAGED_CODE();

	DBGOUT(("RealBugPower %x", DeviceObject));
    
    // mimic bad behavior to veto sleep
    PoStartNextPowerIrp(Irp);
    Irp->IoStatus.Status =  STATUS_UNSUCCESSFUL;
    IoCompleteRequest(Irp, IO_NO_INCREMENT );

    return STATUS_UNSUCCESSFUL;
}


NTSTATUS 
DriverEntry(
    IN PDRIVER_OBJECT DriverObject, 
    IN PUNICODE_STRING RegistryPath 
    )
{
    NTSTATUS                status;
    WCHAR                   deviceNameBuffer[]  = L"\\Device\\RealBug";
    UNICODE_STRING          deviceNameUnicodeString;
    WCHAR                   deviceLinkBuffer[]  = L"\\DosDevices\\RealBug";
    UNICODE_STRING          deviceLinkUnicodeString;  
    PDEVICE_OBJECT          interfaceDevice = NULL;


    //
    // Create a named device object
    //
    RtlInitUnicodeString (&deviceNameUnicodeString,
                          deviceNameBuffer );
    status = IoCreateDevice ( DriverObject,
                                0,
                                &deviceNameUnicodeString,
                                FILE_DEVICE_RealBug,
                                0,
                                TRUE,
                                &interfaceDevice );
    if (NT_SUCCESS(status)) {

	   //
	   // Create a symbolic link that the GUI can specify to gain access
	   // to this driver/device
	   //
	   RtlInitUnicodeString (&deviceLinkUnicodeString,
							 deviceLinkBuffer );
	   status = IoCreateSymbolicLink (&deviceLinkUnicodeString,
										&deviceNameUnicodeString );

	   //
	   // Create dispatch points for all routines that must be RealBugd
	   //
	   DriverObject->MajorFunction[IRP_MJ_CREATE]          =
       DriverObject->MajorFunction[IRP_MJ_CLOSE]           =
       DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL]  = RealBugDispatch;
	   DriverObject->DriverUnload                          = RealBugUnload;
	   DriverObject->MajorFunction[IRP_MJ_POWER] = RealBugPower;
    }
	
    if (!NT_SUCCESS(status)) {
      
	   //
	   // Something went wrong, so clean up 
	   //
	   if( interfaceDevice ) {

           IoDeleteDevice( interfaceDevice );
       }
    }

    return status;
}
    


