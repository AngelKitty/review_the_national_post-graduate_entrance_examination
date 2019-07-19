/*-----------------------------------------------------------------------
第23章  代码的二次开发
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

#include <windows.h>
#include "MenuLib.h"
#include "commdlg.h"

int WINAPI DllMain (HINSTANCE hInstance, DWORD fdwReason, PVOID pvReserved)
{
     return TRUE ;
}



EXPORT BOOL CALLBACK MenuOpen(HWND hWnd)  // 打开文件功能
{
	
	HANDLE hFile;	
	DWORD szTemp;
	long FileSize;
	static OPENFILENAME ofn ;
	TCHAR  *pBuffer = new TCHAR [0x10000];     // 申请64K内存
	TCHAR szFileName[MAX_PATH ], szTitleName[MAX_PATH ] ;
	static TCHAR szFilter[] =TEXT ("TXT Files (*.TXT)\0*.txt\0") \
				TEXT ("All Files (*.*)\0*.*\0\0") ;

	szFileName[0] = '\0';                         
	ZeroMemory(&ofn, sizeof(ofn));                             // 初始化OPENFILENAME结构
	ofn.lStructSize       = sizeof (OPENFILENAME) ;
	ofn.hwndOwner         = hWnd ;
	ofn.lpstrFilter       = szFilter ;
	ofn.lpstrFile         = szFileName ;         
	ofn.nMaxFile          = MAX_PATH ;
	ofn.lpstrFileTitle    = szTitleName ;          
	ofn.nMaxFileTitle     = MAX_PATH ;
	ofn.Flags             = OFN_HIDEREADONLY | OFN_CREATEPROMPT ;           


	if(GetOpenFileName (&ofn))
	{
		HWND hwndEdit = GetDlgItem(hWnd, 1);
		if (hwndEdit )
		{
		     hFile = CreateFile( szFileName,	
					 GENERIC_READ ,
					 NULL,
					 NULL,
					 OPEN_EXISTING,
					 FILE_ATTRIBUTE_NORMAL ,
					 NULL);
		     if( hFile != INVALID_HANDLE_VALUE )
		     {
			     FileSize=GetFileSize(hFile,&szTemp);  // 获取文件大小
			     if(FileSize<0x10000)                  // 如文件小于64K则读取
			     {
				     if(ReadFile(hFile, pBuffer, FileSize, &szTemp, NULL)!=NULL) // 读取数据
				     {

					     SetWindowText( hWnd,szFileName);       // 在窗口标题栏显示打开的文件名
					     SetWindowText(hwndEdit,pBuffer);       // 将内存中的数据显示在文本编辑框中
					     CloseHandle(hFile);
					     delete pBuffer;  // 释放内存
					     return TRUE;   // 退出函数
				     }
			     }
			     CloseHandle(hFile);
			    	
		     }	 
		}
		MessageBoxA (NULL, TEXT ("可能是文件大于64K等其它原因!"), TEXT ("打开文件失败！"), 0) ;
	}


	delete pBuffer; 
	return FALSE;

}

EXPORT BOOL CALLBACK MenuSave(HWND hWnd)  // 保存功能
{

	TCHAR szFileName[MAX_PATH ], szTitleName[MAX_PATH ] ;
	
	HANDLE hFile;
	TCHAR  *pBuffer = new TCHAR [0x10000];     // 申请64内存
	DWORD dwLen;
	DWORD NumberOfBytesRW;
	static OPENFILENAME ofn ;
	static TCHAR szFilter[] =TEXT ("TXT Files (*.TXT)\0*.txt\0") \
		                 TEXT ("All Files (*.*)\0*.*\0\0") ;

	szFileName[0] = '\0';                         
	ZeroMemory(&ofn, sizeof(ofn));                             // 初始化OPENFILENAME结构
	ofn.lStructSize       = sizeof (OPENFILENAME) ;
	ofn.hwndOwner         = hWnd ;
	ofn.lpstrFilter       = szFilter ;
	ofn.lpstrFile         = szFileName ; 
	ofn.nMaxFile          = MAX_PATH ;
	ofn.lpstrFileTitle    = szTitleName ;          
	ofn.nMaxFileTitle     = MAX_PATH ;
	ofn.Flags             = OFN_HIDEREADONLY | OFN_CREATEPROMPT ;           
	ofn.lpstrDefExt       = TEXT("txt");                        // 保存的默认扩展名


	if(GetSaveFileName (&ofn))
	{
		HWND hwndEdit = GetDlgItem(hWnd, 1);
		if (hwndEdit)
		{
			dwLen = GetWindowText(hwndEdit, pBuffer, 0x1000); 
			if (dwLen<0x10000 && dwLen>0 )
			{
				hFile = CreateFile( szFileName,	
						GENERIC_WRITE,
						NULL,
						NULL,
						CREATE_ALWAYS,
						FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
						NULL);
				if( hFile != INVALID_HANDLE_VALUE )
				{
					if(WriteFile (hFile,pBuffer,dwLen,&NumberOfBytesRW, NULL))
					{
						SetWindowText( hWnd,szFileName);  
						CloseHandle(hFile);
						delete pBuffer;  
						return TRUE;
					}
					
					CloseHandle(hFile);
				}
			}
		}
		MessageBoxA (NULL, TEXT ("可能是文件太大或文件无内容!"), TEXT ("保存文件失败！"), 0) ;
	}
	delete pBuffer; 
	return FALSE;

}
