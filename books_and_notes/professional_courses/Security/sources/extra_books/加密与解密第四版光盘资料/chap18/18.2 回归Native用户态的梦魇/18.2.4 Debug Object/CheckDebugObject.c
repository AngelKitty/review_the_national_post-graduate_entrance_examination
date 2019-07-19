//code by Peter[Pan]


#define _WIN32_WINNT 0x0501
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <WinIoCtl.h>

#ifndef STATUS_INFO_LENGTH_MISMATCH
#define STATUS_INFO_LENGTH_MISMATCH	((UINT32)0xC0000004L)
#endif

typedef enum _POOL_TYPE {
  NonPagedPool,
  PagedPool,
  NonPagedPoolMustSucceed,
  DontUseThisType,
  NonPagedPoolCacheAligned,
  PagedPoolCacheAligned,
  NonPagedPoolCacheAlignedMustS
} POOL_TYPE;

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR Buffer;
} UNICODE_STRING;
typedef UNICODE_STRING *PUNICODE_STRING;
typedef const UNICODE_STRING *PCUNICODE_STRING;

typedef enum _OBJECT_INFORMATION_CLASS
{
	ObjectBasicInformation,			// Result is OBJECT_BASIC_INFORMATION structure
	ObjectNameInformation,			// Result is OBJECT_NAME_INFORMATION structure
	ObjectTypeInformation,			// Result is OBJECT_TYPE_INFORMATION structure
	ObjectAllTypesInformation,			// Result is OBJECT_ALL_INFORMATION structure
	ObjectDataInformation			// Result is OBJECT_DATA_INFORMATION structure
	
} OBJECT_INFORMATION_CLASS, *POBJECT_INFORMATION_CLASS;

typedef struct _OBJECT_TYPE_INFORMATION {

UNICODE_STRING TypeName; ULONG TotalNumberOfHandles; ULONG TotalNumberOfObjects; WCHAR Unused1[8]; ULONG HighWaterNumberOfHandles; ULONG HighWaterNumberOfObjects; WCHAR Unused2[8]; ACCESS_MASK InvalidAttributes; GENERIC_MAPPING GenericMapping; ACCESS_MASK ValidAttributes; BOOLEAN SecurityRequired; BOOLEAN MaintainHandleCount; USHORT MaintainTypeList; POOL_TYPE PoolType; ULONG DefaultPagedPoolCharge; ULONG DefaultNonPagedPoolCharge;

} OBJECT_TYPE_INFORMATION, *POBJECT_TYPE_INFORMATION;

typedef struct _OBJECT_ALL_INFORMATION {

ULONG NumberOfObjectsTypes; OBJECT_TYPE_INFORMATION ObjectTypeInformation[1];

} OBJECT_ALL_INFORMATION, *POBJECT_ALL_INFORMATION;

typedef struct _OBJECT_ALL_TYPES_INFORMATION {
    ULONG NumberOfTypes;
    OBJECT_TYPE_INFORMATION TypeInformation[1];
} OBJECT_ALL_TYPES_INFORMATION, *POBJECT_ALL_TYPES_INFORMATION;

typedef UINT32 (__stdcall  *ZwQueryObject_t) ( IN HANDLE ObjectHandle, IN OBJECT_INFORMATION_CLASS ObjectInformationClass, OUT PVOID ObjectInformation, IN ULONG Length, OUT PULONG ResultLength );
ZwQueryObject_t ZwQueryObject;

void main()
{

	int i;
	DWORD dwSize;
	HMODULE hNtDLL;
	UCHAR  KeyType=0;

	hNtDLL = GetModuleHandle("ntdll.dll");
	if(!hNtDLL)
		return;

	OBJECT_ALL_TYPES_INFORMATION	*Types;
	OBJECT_TYPE_INFORMATION		*t;

	ZwQueryObject = (ZwQueryObject_t)GetProcAddress(hNtDLL, "ZwQueryObject");
	if(!ZwQueryObject)
		return;

	
	UINT32 iResult = ZwQueryObject(NULL, ObjectAllTypesInformation, NULL, NULL, &dwSize);
	if(iResult== STATUS_INFO_LENGTH_MISMATCH)
	{   
		Types = (OBJECT_ALL_TYPES_INFORMATION*)VirtualAlloc(NULL,dwSize,MEM_COMMIT,PAGE_READWRITE);
		if (Types == NULL) 
			return;

	    if (iResult=ZwQueryObject(NULL,ObjectAllTypesInformation, Types, dwSize, &dwSize))
	    {	
			return;
	    }
	    for (t=Types->TypeInformation,i=0;i<Types->NumberOfTypes;i++)
	    {   
			if ( !_wcsicmp(t->TypeName.Buffer,L"DebugObject"))
			{   
				if(t->TotalNumberOfHandles > 0 || t->TotalNumberOfObjects > 0)
				{
					MessageBox(GetDesktopWindow(), "Turn the Debugger Off", "Caught!", MB_OK);
					VirtualFree (Types,0,MEM_RELEASE);
					return;
				}
				break; // Found Anyways

				//printf("TypeName: %S\r\n", t->TypeName.Buffer);  
				//printf("DefaultNonPagedPoolCharge: %X\r\n", t->DefaultNonPagedPoolCharge); 
				//printf("DefaultPagedPoolCharge: %X\r\n", t->DefaultPagedPoolCharge); 
				//printf("GenericMapping: %X\r\n", t->GenericMapping);
				//printf("HighWaterNumberOfHandles: %X\r\n", t->HighWaterNumberOfHandles);
				//printf("HighWaterNumberOfObjects: %X\r\n", t->HighWaterNumberOfObjects);
				//printf("InvalidAttributes: %X\r\n", t->InvalidAttributes);
				//printf("MaintainHandleCount: %X\r\n", t->MaintainHandleCount);
				//printf("PoolType: %X\r\n", t->PoolType);
				//printf("SecurityRequired: %X\r\n", t->SecurityRequired);
				//printf("TotalNumberOfHandles: %X\r\n", t->TotalNumberOfHandles);
				//printf("TotalNumberOfObjects: %X\r\n", t->TotalNumberOfObjects);
				//printf("TypeName: %X\r\n", t->TypeName);
				//printf("Unused1: %X\r\n", t->Unused1[0]);
				//printf("Unused1: %X\r\n", t->Unused1[1]);
				//printf("Unused1: %X\r\n", t->Unused1[2]);
				//printf("Unused1: %X\r\n", t->Unused1[3]);
				//printf("Unused1: %X\r\n", t->Unused1[4]);
				//printf("Unused1: %X\r\n", t->Unused1[5]);
				//printf("Unused1: %X\r\n", t->Unused1[6]);
				//printf("Unused1: %X\r\n", t->Unused1[7]);
				//printf("Unused2: %X\r\n", t->Unused2[0]);
				//printf("Unused2: %X\r\n", t->Unused2[1]);
				//printf("Unused2: %X\r\n", t->Unused2[2]);
				//printf("Unused2: %X\r\n", t->Unused2[3]);
				//printf("Unused2: %X\r\n", t->Unused2[4]);
				//printf("Unused2: %X\r\n", t->Unused2[5]);
				//printf("Unused2: %X\r\n", t->Unused2[6]);
				//printf("Unused2: %X\r\n", t->Unused2[7]);
				//getch();
			}
			t=(OBJECT_TYPE_INFORMATION *)((char *)t->TypeName.Buffer+((t->TypeName.MaximumLength+3)&~3));
	    }
	}
	MessageBox(GetDesktopWindow(), "Looks Good to me!", "Safe!", MB_OK);
	VirtualFree (Types,0,MEM_RELEASE);
	return;
}
