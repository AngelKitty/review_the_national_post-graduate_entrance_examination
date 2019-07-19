

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Thu May 06 14:45:04 2010
 */
/* Compiler settings for .\VulnerAX_SEH.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_VulnerAX_SEHLib,0x2102398C,0x45B6,0x4276,0x98,0x59,0xCE,0x35,0xB3,0x0F,0x52,0x5A);


MIDL_DEFINE_GUID(IID, DIID__DVulnerAX_SEH,0x6CEE025A,0x3C2D,0x49CE,0x81,0x4E,0x8B,0xE9,0x90,0xF6,0x03,0x77);


MIDL_DEFINE_GUID(IID, DIID__DVulnerAX_SEHEvents,0x38A0AC24,0xC620,0x4839,0xBB,0xF5,0x66,0x64,0x4C,0x60,0x4A,0x93);


MIDL_DEFINE_GUID(CLSID, CLSID_VulnerAX_SEH,0x1F0837F2,0x15E5,0x4115,0xA2,0x35,0x81,0xDA,0x2F,0x73,0xC2,0x04);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



