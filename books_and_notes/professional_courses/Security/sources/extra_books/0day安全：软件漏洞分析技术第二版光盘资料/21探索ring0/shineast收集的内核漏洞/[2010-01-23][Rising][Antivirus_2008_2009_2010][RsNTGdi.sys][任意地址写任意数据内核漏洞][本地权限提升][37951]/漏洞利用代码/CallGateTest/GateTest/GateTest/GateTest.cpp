///////////////////////////////////////////////////////////////////////////////
///
/// Copyright (c) 2010 - <company name here>
///
/// Original filename: GateTest.cpp
/// Project          : GateTest
/// Date of creation : 2010-02-01
/// Author(s)        : 
///
/// Purpose          : <description>
///
/// Revisions:
///  0000 [2010-02-01] Initial revision.
///
///////////////////////////////////////////////////////////////////////////////

// $Id$

#ifdef __cplusplus
extern "C" {
#endif
#include <ntddk.h>
#include <string.h>
#ifdef __cplusplus
}; // extern "C"
#endif

#include "GateTest.h"

#ifdef __cplusplus
namespace { // anonymous namespace to limit the scope of this global variable!
#endif
PDRIVER_OBJECT pdoGlobalDrvObj = 0;
#ifdef __cplusplus
}; // anonymous namespace
#endif

NTSTATUS GATETEST_DispatchCreateClose(
	IN PDEVICE_OBJECT		DeviceObject,
	IN PIRP					Irp
	)
{
	NTSTATUS status = STATUS_SUCCESS;
	Irp->IoStatus.Status = status;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return status;
}

NTSTATUS GATETEST_DispatchDeviceControl(
	IN PDEVICE_OBJECT		DeviceObject,
	IN PIRP					Irp
	)
{
	NTSTATUS status = STATUS_SUCCESS;
	PIO_STACK_LOCATION irpSp = IoGetCurrentIrpStackLocation(Irp);
    ULONG inputLen = irpSp->Parameters.DeviceIoControl.InputBufferLength;
    ULONG outputLen = irpSp->Parameters.DeviceIoControl.OutputBufferLength;
    ULONG* inputBuf = (ULONG*)irpSp->Parameters.DeviceIoControl.Type3InputBuffer;
    ULONG* outputBuf = (ULONG*)Irp->UserBuffer;

	switch(irpSp->Parameters.DeviceIoControl.IoControlCode)
	{
	case IOCTL_GATETEST_OPERATION:
		if(inputLen != sizeof(ULONG) ||
            outputLen != sizeof(ULONG))
        {
            KdPrint(("invalid param.\n"));
            Irp->IoStatus.Status = STATUS_INVALID_PARAMETER;
            break;
        }
        KdPrint(("write ok~~~.\n"));
        *outputBuf = *inputBuf;
        Irp->IoStatus.Information = sizeof(ULONG);
        Irp->IoStatus.Status = STATUS_SUCCESS;
		break;
	default:
		Irp->IoStatus.Status = STATUS_INVALID_DEVICE_REQUEST;
		Irp->IoStatus.Information = 0;
		break;
	}

	status = Irp->IoStatus.Status;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return status;
}

VOID GATETEST_DriverUnload(
	IN PDRIVER_OBJECT		DriverObject
	)
{
	PDEVICE_OBJECT pdoNextDeviceObj = pdoGlobalDrvObj->DeviceObject;
	IoDeleteSymbolicLink(&usSymlinkName);

	// Delete all the device objects
	while(pdoNextDeviceObj)
	{
		PDEVICE_OBJECT pdoThisDeviceObj = pdoNextDeviceObj;
		pdoNextDeviceObj = pdoThisDeviceObj->NextDevice;
		IoDeleteDevice(pdoThisDeviceObj);
	}
}

#ifdef __cplusplus
extern "C" {
#endif
NTSTATUS DriverEntry(
	IN OUT PDRIVER_OBJECT   DriverObject,
	IN PUNICODE_STRING      RegistryPath
	)
{
	PDEVICE_OBJECT pdoDeviceObj = 0;
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	pdoGlobalDrvObj = DriverObject;

	// Create the device object.
	if(!NT_SUCCESS(status = IoCreateDevice(
		DriverObject,
		0,
		&usDeviceName,
		FILE_DEVICE_UNKNOWN,
		FILE_DEVICE_SECURE_OPEN,
		FALSE,
		&pdoDeviceObj
		)))
	{
		// Bail out (implicitly forces the driver to unload).
		return status;
	};

	// Now create the respective symbolic link object
	if(!NT_SUCCESS(status = IoCreateSymbolicLink(
		&usSymlinkName,
		&usDeviceName
		)))
	{
		IoDeleteDevice(pdoDeviceObj);
		return status;
	}

	// NOTE: You need not provide your own implementation for any major function that
	//       you do not want to handle. I have seen code using DDKWizard that left the
	//       *empty* dispatch routines intact. This is not necessary at all!
	DriverObject->MajorFunction[IRP_MJ_CREATE] =
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = GATETEST_DispatchCreateClose;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = GATETEST_DispatchDeviceControl;
	DriverObject->DriverUnload = GATETEST_DriverUnload;

	return STATUS_SUCCESS;
}
#ifdef __cplusplus
}; // extern "C"
#endif
