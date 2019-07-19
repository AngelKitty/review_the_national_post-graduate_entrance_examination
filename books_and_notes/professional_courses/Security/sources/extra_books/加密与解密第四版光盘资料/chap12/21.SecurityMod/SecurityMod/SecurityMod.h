
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SECURITYMOD_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SECURITYMOD_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef SECURITYMOD_EXPORTS
#define SECURITYMOD_API __declspec(dllexport)
#else
#define SECURITYMOD_API __declspec(dllimport)
#endif

extern "C" SECURITYMOD_API BOOL RegisterThread(DWORD dwThreadId);