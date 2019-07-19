// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 STUB_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// STUB_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef STUB_EXPORTS
#define STUB_API __declspec(dllexport)
#else
#define STUB_API __declspec(dllimport)
#endif

extern "C"
{
	typedef struct _GLOBAL_PARAM
	{
		DWORD       dwStart;               // Stub中的OEP
		BOOL        bShowMessage;          // 是否显示解密信息
		ULONGLONG   dwImageBase;           // 映像基址
		DWORD       dwOEP;		           // 程序入口点
		PBYTE       lpStartVA;	           // 起始虚拟地址（被异或加密区）
		DWORD       dwCodeSize;	           // 被加密大小
		BYTE        byXor;                 // 加密异或的值
	}GLOBAL_PARAM, *PGLOBAL_PARAM;

	typedef ULONGLONG(WINAPI *fnGetProcAddress)(_In_ HMODULE hModule, _In_ LPCSTR lpProcName);
	typedef HMODULE(WINAPI *fnLoadLibraryA)(_In_ LPCSTR lpLibFileName);
	typedef int(WINAPI *fnMessageBox)(HWND hWnd, LPSTR lpText, LPSTR lpCaption, UINT uType);
	typedef HMODULE(WINAPI *fnGetModuleHandleA)(_In_opt_ LPCSTR lpModuleName);
	typedef BOOL(WINAPI *fnVirtualProtect)(_In_ LPVOID lpAddress, _In_ SIZE_T dwSize, _In_ DWORD flNewProtect, _Out_ PDWORD lpflOldProtect);
	typedef void(WINAPI *fnExitProcess)(_In_ UINT uExitCode);

	void Start();
}




