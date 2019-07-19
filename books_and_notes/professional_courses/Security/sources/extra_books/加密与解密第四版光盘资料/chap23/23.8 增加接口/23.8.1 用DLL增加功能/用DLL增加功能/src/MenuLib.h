/*----------------------
   PRINTLIB.H header file
  ----------------------*/

#ifdef __cplusplus
#define EXPORT extern "C" __declspec (dllexport)
#else
#define EXPORT __declspec (dllexport)
#endif

EXPORT BOOL CALLBACK  MenuOpen(HWND hWnd);
EXPORT BOOL CALLBACK  MenuSave(HWND hWnd);
