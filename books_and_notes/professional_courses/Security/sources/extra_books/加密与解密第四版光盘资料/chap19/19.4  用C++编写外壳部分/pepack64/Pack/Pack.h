// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 PACK_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// PACK_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef PACK_EXPORTS
#define PACK_API __declspec(dllexport)
#else
#define PACK_API __declspec(dllimport)
#endif

// 此类是从 Pack.dll 导出的
class PACK_API CPack {
public:
	CPack(void);
	// TODO:  在此添加您的方法。
};

extern PACK_API int nPack;

PACK_API int fnPack(void);

extern "C" PACK_API BOOL Pack(CString strPath, BYTE byXor);