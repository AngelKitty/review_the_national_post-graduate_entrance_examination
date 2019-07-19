/*-----------------------------------------------------------------------
第19章  外壳编写基础
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

#pragma once

/*-------------------------------------------------------------*/
/* 定义子程序与全局变量、常量                                  */
/*-------------------------------------------------------------*/

#define				    MAXINPUTLEN 80

BOOL	CALLBACK MainDlg   (HWND, UINT, WPARAM, LPARAM) ;

HINSTANCE				hInst;
HWND					hProgress;

TCHAR					szFilePath[MAX_PATH];
TCHAR					*pMessageBuffer;		// 消息框缓冲
TCHAR					 IniFileName[] = TEXT ("pediy.ini");// 属性配置记录文件

PCHAR					m_pInfoData= NULL;      // 用于记录压缩过的区块信息
UINT					m_pInfoSize;
const UINT				SIZE_OF_PACKINFO = 0x2000;// 记录压缩过的区块信息


UINT					m_nImageSize = 0;		// 映象大小
PIMAGE_NT_HEADERS 		m_pntHeaders = 0;		// PE结构指针
PIMAGE_SECTION_HEADER 	m_psecHeader = 0;		// 第一个SECTION结构指针
PCHAR					m_pImageBase = 0;		// 映象基址


PCHAR					m_pWorkSpace=NULL;		// 压缩申请的临时内存
PCHAR					m_pPackData=NULL;		// 压缩后的地址
UINT					m_nPackSize;			// 压缩后数据大小

HANDLE				    hPackFile ;				// 创建压缩文件的句柄

BOOL					ISWORKING=0;              // 是否正在压缩标志

//外壳shell.asm中的变量
extern "C" DWORD ShellStart0;
extern "C" DWORD ShellEnd0;
extern "C" DWORD ShellStart;
extern "C" DWORD ShellEnd;
extern "C" DWORD ImportTableBegin;
extern "C" DWORD ImportTableEnd;
extern "C" DWORD SHELL_DATA_0;
extern "C" DWORD SHELL_DATA_1;
extern "C" BYTE  TlsTable;
extern "C" BYTE  RelocBaseBegin;

//属性对话框变量
BOOL IsFileAlignment200 = FALSE;
BOOL IsProtImpTable     = FALSE;//外壳是否输入表
BOOL isPackRes          = FALSE;//是否压缩资源
BOOL IsSaveSData        = FALSE;//是否保留额外数据
BOOL IsMergeSection     = FALSE;//是否合并区块
BOOL IsClsSecName       = FALSE;//清空区块名
BOOL IsNoRelocation     = FALSE;//清空区块名
BOOL IsCreateBak        = FALSE;//备份文件
BOOL IsDelShare         = FALSE;//忽略匹块共享

/*-------------------------------------------------------------*/
/* RVAToPtr － RVA转当前偏移地址函数						   */
/*-------------------------------------------------------------*/
PCHAR RVAToPtr(DWORD dwRVA)
{
	if ((UINT)dwRVA < m_nImageSize)
	{
		return (PCHAR)((DWORD)m_pImageBase + dwRVA);
	}
	else
	{
		return NULL;
	}
}




/*-------------------------------------------------------------*/
/* AddLine － 在消息框中增加一行消息输出                       */
/*-------------------------------------------------------------*/


void AddLine(HWND hDlg,TCHAR *szMsg)
{
	if(GetDlgItemText(hDlg,IDC_MESSAGEBOX_EDIT,pMessageBuffer,MAX_PATH)!=0){
		strcat(pMessageBuffer, "\r\n"); 
	}
	strcat(pMessageBuffer, szMsg); 

	SendDlgItemMessage(hDlg,IDC_MESSAGEBOX_EDIT,WM_SETTEXT,0,(LPARAM)pMessageBuffer);
	SendDlgItemMessage(hDlg, IDC_MESSAGEBOX_EDIT, EM_LINESCROLL, 0, SendDlgItemMessage(hDlg, IDC_MESSAGEBOX_EDIT, EM_GETLINECOUNT, 0, 0));
}

/*-------------------------------------------------------------*/
/* AlignSize － 取整对齐函数								   */
/*-------------------------------------------------------------*/
UINT AlignSize(UINT nSize, UINT nAlign)
{
	return ((nSize + nAlign - 1) / nAlign * nAlign);
}
