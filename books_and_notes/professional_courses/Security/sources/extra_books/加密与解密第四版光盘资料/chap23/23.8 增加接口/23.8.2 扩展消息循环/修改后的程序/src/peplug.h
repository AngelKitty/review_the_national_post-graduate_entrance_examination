/*----------------------
   PRINTLIB.H header file
  ----------------------*/

#ifdef __cplusplus
#define EXPORT extern "C" __declspec (dllexport)
#else
#define EXPORT __declspec (dllexport)
#endif

EXPORT void _cdecl MyWndProc(const DWORD,HWND, UINT, WPARAM, LPARAM);

