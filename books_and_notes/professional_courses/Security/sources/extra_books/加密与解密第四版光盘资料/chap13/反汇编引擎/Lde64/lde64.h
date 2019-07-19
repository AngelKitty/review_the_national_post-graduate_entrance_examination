#pragma once

#ifdef __cplusplus
#define EXTCFUN extern "C" 
#else
#define EXTCFUN
#endif

EXTCFUN int __stdcall LDE(void* , int); // 第二个参数 64
#pragma comment(lib, "LDE64.lib")
