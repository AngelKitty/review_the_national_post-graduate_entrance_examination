/*-----------------------------------------------------------------------
第7章  Windows内核基础
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

#include <ntddk.h>

VOID DriverUnload(PDRIVER_OBJECT pDriverObject)
{
	DbgPrint("Goodbye world!\n");
}


NTSTATUS DriverEntry(
IN PDRIVER_OBJECT pDriverObject,
IN PUNICODE_STRING pRegistryPath)
{
	DbgPrint("Hello, world\n");
	pDriverObject->DriverUnload = DriverUnload;
	return STATUS_SUCCESS;
}