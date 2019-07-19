/********************************************************************
	created:	2010/12/06
	filename: 	D:\0day\HelloWorld\helloworld.c
	author:		shineast
	purpose:	Hello world driver demo 
*********************************************************************/
#include <ntddk.h>
#define DEVICE_NAME L"\\Device\\HelloWorld"
#define DEVICE_LINK L"\\DosDevices\\HelloWorld"
//创建的设备对象指针
PDEVICE_OBJECT g_DeviceObject;
/**********************************************************************
 驱动卸载函数
	输入：驱动对象的指针
	输出：无
**********************************************************************/
VOID DriverUnload( IN PDRIVER_OBJECT  driverObject )
{
	//什么都不做，只是打印一句话
	KdPrint(("DriverUnload: 88!\n"));
} 
/**********************************************************************
 驱动派遣例程函数
	输入：驱动对象的指针,Irp指针
	输出：NTSTATUS类型的结果
**********************************************************************/
NTSTATUS DrvDispatch(IN PDEVICE_OBJECT driverObject,IN PIRP pIrp)
{ 
	KdPrint(("Enter DrvDispatch\n"));
	//设置IRP状态
	pIrp->IoStatus.Status=STATUS_SUCCESS;
	//设置IRP操作字节数
	pIrp->IoStatus.Information=0;
	//完成IRP的处理
	IoCompleteRequest(pIrp,IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}
/*********************************************************************
 驱动入口函数（相当于main函数）
	输入：驱动对象的指针，服务程序对应的注册表路径
	输出：NTSTATUS类型的结果
**********************************************************************/
NTSTATUS DriverEntry( IN PDRIVER_OBJECT  driverObject, IN PUNICODE_STRING  registryPath )
{ 
	NTSTATUS       ntStatus;
	UNICODE_STRING devName;
	UNICODE_STRING symLinkName;
	int i=0; 
	//打印一句调试信息
	KdPrint(("DriverEntry: Hello world driver demo!\n"));
	//设置该驱动对象的卸载函数
	//driverObject->DriverUnload = DriverUnload; 
	//创建设备 
	RtlInitUnicodeString(&devName,DEVICE_NAME);
	ntStatus = IoCreateDevice( driverObject,
		0,
		&devName,
		FILE_DEVICE_UNKNOWN,
		0, TRUE,
		&g_DeviceObject );
	if (!NT_SUCCESS(ntStatus))
	{
		return ntStatus;  
	}
	//创建符号链接  
	RtlInitUnicodeString(&symLinkName,DEVICE_LINK);
	ntStatus = IoCreateSymbolicLink( &symLinkName,&devName );
	if (!NT_SUCCESS(ntStatus)) 
	{
		IoDeleteDevice( g_DeviceObject );
		return ntStatus;
	}
	//设置该驱动对象的派遣例程函数
	for (i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
	{
		driverObject->MajorFunction[i] = DrvDispatch;
	}
	//返回成功结果
	return STATUS_SUCCESS;
}