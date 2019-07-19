

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Thu Apr 29 21:33:05 2010
 */
/* Compiler settings for .\VulnerAX.idl:
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

MIDL_DEFINE_GUID(IID, LIBID_VulnerAXLib,0x92B6E994,0x9F43,0x413C,0x88,0x8A,0x94,0xBB,0x52,0xEB,0x57,0x9C);


MIDL_DEFINE_GUID(IID, DIID__DVulnerAX,0x25C13936,0x195C,0x405C,0xA7,0x97,0xCE,0x7F,0x4B,0xE3,0xC2,0x60);


MIDL_DEFINE_GUID(IID, DIID__DVulnerAXEvents,0x859AB449,0x2E1E,0x49D5,0x8A,0xAD,0x62,0xB2,0x0A,0x7A,0x3D,0x55);


MIDL_DEFINE_GUID(CLSID, CLSID_VulnerAX,0x39F64D5B,0x74E8,0x482F,0x95,0xF4,0x91,0x8E,0x54,0xB1,0xB2,0xC8);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



