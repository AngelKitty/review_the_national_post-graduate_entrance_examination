/*******************************************************
/*《加密与解密》第四版配套实例
/*第19章 外壳编写基础
/*Microsoft Visual C++ 6.0
/*Code by Hying 2001.1
/*Modified by kanxue  2005.3
/*Thanks ljtt
/*Hying原来的外壳主程序是asm，kanxue用VC改写，改写过程，参考了ljtt的外壳源码
/*(c)  看雪学院 www.kanxue.com 2000-2018
********************************************************/
/********************************************************************************/
/*  VC 6.0工程直接融合了MASM32汇编编译的方法							        */
/* 1、将shell.asm填加到VC工程的Source files中；					                */
/* 2、将Source files中的shell.obj删除；							                */
/* 3、在Source files中的shell.asm上：右键->Setting->选中Custom Build页	        */
/*   在Commands中输入：													        */
/*    如果是DEBUG模式，则输入：											        */   
/*    c:\masm32\bin\ml /c /coff /Zi /Fo$(IntDir)\$(InputName).obj $(InputPath)  */
/*																		        */
/*    如果是RELEASE模式，则输入： 							                    */
/*    c:\masm32\bin\ml /c /coff  /Fo$(IntDir)\$(InputName).obj $(InputPath)     */
/*																		        */
/*    在Outputs中输入：													      	*/
/* $(IntDir)\$(InputName).obj                                                   */
/*    如果没有把masm安装在c盘，则要作相应的修改。                               */
/********************************************************************************/

#include "res\resource.h"
#include <windows.h> 
#include "aplib\aplib.h"
#include <commctrl.h>
#include "pediy.h"
#include "ImportTable.h"
#include "ResourceDir.h"
#include "IsPEFile.h"
#include "Relocation.h"
#include "Section.h"
#include "PackFile.h"
#include "DisposeShell.h"
#include "ProtTheFile.h"
#include <process.h> 


#pragma comment(lib, "comctl32.lib")



/*-------------------------------------------------------------*/
/* WinMain － 基于WIN32的程序的入口                            */
/*-------------------------------------------------------------*/

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
	hInst = hInstance;
	DialogBoxParam (hInstance, MAKEINTRESOURCE(IDD_MAINDLG), NULL, MainDlg, NULL);
	return 0;	
}

/*-------------------------------------------------------------*/
/* AboutDlgProc － 关于窗口                                    */
/*-------------------------------------------------------------*/

BOOL CALLBACK AboutDlgProc (HWND hDlg, UINT message, 
                            WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
   case  WM_LBUTTONDOWN:
		PostMessage(hDlg, WM_NCLBUTTONDOWN, HTCAPTION, 0);
		return TRUE ;
    
	case WM_COMMAND :
		switch (LOWORD (wParam))  		            
		{
		case IDOK :
		case IDCANCEL :
			EndDialog (hDlg, 0) ;
			return TRUE ;
		 }
		  break ;
	 }
     return FALSE ;
}



/*-------------------------------------------------------------*/
/* OpenFileDlg － 打开对话框函数                               */
/*-------------------------------------------------------------*/
BOOL  OpenFileDlg(TCHAR *szFilePath,HWND hwnd)   
{
	OPENFILENAME ofn;
    memset(szFilePath,0,MAX_PATH);
	memset(&ofn, 0, sizeof(ofn));

	ofn.lStructSize      =sizeof(ofn);
	ofn.hwndOwner        =hwnd;
	ofn.hInstance        =hInst;
	ofn.nMaxFile         =MAX_PATH;
	ofn.lpstrInitialDir  =NULL;
	ofn.lpstrFile        =szFilePath;
	ofn.lpstrTitle       ="Open ...";
	ofn.Flags            = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_LONGNAMES|OFN_EXPLORER | OFN_HIDEREADONLY;
	ofn.lpstrFilter      =TEXT ("PE Files (*.exe;*.dll)\0*.EXE;*.DLL\0")\
						  TEXT ("All Files (*.*)\0*.*\0\0") ;
	if(!GetOpenFileName(&ofn))
		return FALSE;
	
	SendDlgItemMessage(hwnd,IDC_MESSAGEBOX_EDIT,WM_SETTEXT,0,0);//清空消息框中的提示
	return TRUE;

}



/*-------------------------------------------------------------*/
/* properinitDlgProc－ 选项对话框初始化                            */
/*-------------------------------------------------------------*/
void properinitDlgProc(HWND hDlg)
{

	//强制文件按0x200对齐
	if(GetPrivateProfileInt("ProtSet","IsFileAlignment200",0,IniFileName)==1) 
	{
		IsFileAlignment200=TRUE;
		CheckDlgButton(hDlg,IDC_ALIGN_200,BST_CHECKED);
	}

	//是否处理输入表
	if(GetPrivateProfileInt("ProtSet","IsProtImpTable",0,IniFileName)==1) 
	{
		IsProtImpTable=TRUE;
		CheckDlgButton(hDlg,IDC_APIPROT,BST_CHECKED);
	}

	//是否压缩资源
   	if(GetPrivateProfileInt("ProtSet","isPackRes",0,IniFileName)==1) 
	{
		isPackRes =TRUE;
		CheckDlgButton(hDlg,IDC_PACKRES,BST_CHECKED);
	}

	//是否保留额外数据
   	if(GetPrivateProfileInt("ProtSet","IsSaveSData",0,IniFileName)==1) 
	{
		isPackRes =TRUE;
		CheckDlgButton(hDlg,IDC_SDATA,BST_CHECKED);
	}

	//是否合并区块
   	if(GetPrivateProfileInt("ProtSet","IsMergeSection",0,IniFileName)==1) 
	{
		IsMergeSection =TRUE;
		CheckDlgButton(hDlg,IDC_MERGESECTION,BST_CHECKED);
	}

	//清空区块名
   	if(GetPrivateProfileInt("ProtSet","IsClsSecName",0,IniFileName)==1) 
	{
		IsClsSecName =TRUE;
		CheckDlgButton(hDlg,IDC_CLSSECNAME,BST_CHECKED);
	}

	//清除重定位数据
   	if(GetPrivateProfileInt("ProtSet","IsNoRelocation",0,IniFileName)==1) 
	{
		IsNoRelocation =TRUE;
		CheckDlgButton(hDlg,IDC_NOREL0,BST_CHECKED);
	}

	//备份文件
   	if(GetPrivateProfileInt("ProtSet","IsCreateBak",0,IniFileName)==1) 
	{
		IsCreateBak =TRUE;
		CheckDlgButton(hDlg,IDC_CREATEBAK,BST_CHECKED);
	}

	//忽略区块共享
   	if(GetPrivateProfileInt("ProtSet","IsDelShare",0,IniFileName)==1) 
	{
		IsDelShare =TRUE;
		CheckDlgButton(hDlg,IDC_DELSHSRE,BST_CHECKED);
	}

}

//*****************取得设置**********************************
void GetOption(HWND hDlg)
{
	TCHAR string[32];

	//强制文件对齐为200吗?
	if(SendDlgItemMessage(hDlg, IDC_ALIGN_200, BM_GETCHECK, 0, 0)==BST_CHECKED)
		IsFileAlignment200=TRUE;
	else
		IsFileAlignment200=FALSE;
	wsprintf(string,"%d",IsFileAlignment200); 
	WritePrivateProfileString("ProtSet","IsFileAlignment200",string,IniFileName);
	
	//是否处理输入表
	if(SendDlgItemMessage(hDlg, IDC_APIPROT, BM_GETCHECK, 0, 0)==BST_CHECKED)
		IsProtImpTable=TRUE;
	else
		IsProtImpTable=FALSE;
	wsprintf(string,"%d",IsProtImpTable); 
	WritePrivateProfileString("ProtSet","IsProtImpTable",string,IniFileName);
	
	//是否压缩资源
	if(SendDlgItemMessage(hDlg, IDC_PACKRES, BM_GETCHECK, 0, 0)==BST_CHECKED)
		isPackRes=TRUE;
	else
		isPackRes=FALSE;
	wsprintf(string,"%d",isPackRes); 
	WritePrivateProfileString("ProtSet","isPackRes",string,IniFileName);
	
	//是否保留额外数据
	if(SendDlgItemMessage(hDlg, IDC_SDATA, BM_GETCHECK, 0, 0)==BST_CHECKED)
		IsSaveSData=TRUE;
	else
		IsSaveSData=FALSE;
	wsprintf(string,"%d",IsSaveSData); 
	WritePrivateProfileString("ProtSet","IsSaveSData",string,IniFileName);
	
	//是否合并区块
	if(SendDlgItemMessage(hDlg, IDC_MERGESECTION, BM_GETCHECK, 0, 0)==BST_CHECKED)
		IsMergeSection=TRUE;
	else
		IsMergeSection=FALSE;
	wsprintf(string,"%d",IsMergeSection); 
	WritePrivateProfileString("ProtSet","IsMergeSection",string,IniFileName);
	
	//清空区块名
	if(SendDlgItemMessage(hDlg, IDC_CLSSECNAME, BM_GETCHECK, 0, 0)==BST_CHECKED)
		IsClsSecName=TRUE;
	else
		IsClsSecName=FALSE;
	wsprintf(string,"%d",IsClsSecName); 
	WritePrivateProfileString("ProtSet","IsClsSecName",string,IniFileName);
	
	//清除重定位数据
	if(SendDlgItemMessage(hDlg, IDC_NOREL0, BM_GETCHECK, 0, 0)==BST_CHECKED)
		IsNoRelocation=TRUE;
	else
		IsNoRelocation=FALSE;
	wsprintf(string,"%d",IsNoRelocation); 
	WritePrivateProfileString("ProtSet","IsNoRelocation",string,IniFileName);
	
	//备份文件
	if(SendDlgItemMessage(hDlg, IDC_CREATEBAK , BM_GETCHECK, 0, 0)==BST_CHECKED)
		IsCreateBak=TRUE;
	else
		IsCreateBak=FALSE;
	wsprintf(string,"%d",IsCreateBak); 
	WritePrivateProfileString("ProtSet","IsCreateBak",string,IniFileName);
	
	//忽略区块共享
	if(SendDlgItemMessage(hDlg, IDC_DELSHSRE , BM_GETCHECK, 0, 0)==BST_CHECKED)
		IsDelShare=TRUE;
	else
		IsDelShare=FALSE;
	wsprintf(string,"%d",IsDelShare); 
	WritePrivateProfileString("ProtSet","IsDelShare",string,IniFileName);
	
	
}



/*-------------------------------------------------------------*/
/* PropertDlgProc － 属性设置窗口                            */
/*-------------------------------------------------------------*/

BOOL CALLBACK PropertDlgProc (HWND hDlg, UINT message, 
                            WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{

	case WM_INITDIALOG:
		//配置对话框初始化
		properinitDlgProc(hDlg);
		break;
	 }
     return FALSE ;
}


/*-------------------------------------------------------------*/
/*  IDD_STATUS － 加密窗口                                      */
/*-------------------------------------------------------------*/

BOOL CALLBACK StatusDlgProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)

{	
	HANDLE					hThread ;
	//DWORD                 ProtFileThreadID;
	UINT                    ProtFileThreadID;



	switch (message) 
	{
	case WM_INITDIALOG:

		//创建消息框缓冲
		pMessageBuffer = new TCHAR [0x10000];  
		ZeroMemory(pMessageBuffer, 0x10000); //将消息缓冲区数据清零

		hProgress = GetDlgItem(hDlg, IDC_PROGRESS) ;

		DragAcceptFiles(hDlg,TRUE);



		break; 

	case WM_DROPFILES://支持文件拖放
		
		if(FALSE==ISWORKING){
			
			ZeroMemory(pMessageBuffer, 0x10000); //将消息缓冲区数据清零
			ZeroMemory(szFilePath,MAX_PATH);//清空文件名缓冲
			DragQueryFile((HDROP)wParam, 0, szFilePath, sizeof(szFilePath));
			DragFinish((HDROP)wParam);

			SendDlgItemMessage(hDlg,IDC_MESSAGEBOX_EDIT,WM_SETTEXT,0,0);//清空消息框中的提示
			SendDlgItemMessage(hDlg,IDC_FILEPATH_EDIT,WM_SETTEXT,MAX_PATH,(LPARAM)szFilePath);
			AddLine(hDlg,szFilePath);
			if(!IsPEFile(szFilePath,hDlg)) 
				EnableWindow(GetDlgItem(hDlg,IDC_PROT_BUTTON), FALSE);
			else
				EnableWindow(GetDlgItem(hDlg,IDC_PROT_BUTTON), TRUE);
			SendMessage(hProgress,PBM_SETPOS, 0, 0);					
			}		
		break;

	case WM_COMMAND: 
		switch (LOWORD (wParam))
		{
		case IDC__EXIT_BUTTON:
			SendMessage(GetParent(GetParent(hDlg)), WM_CLOSE, 0, 0);

			break;
			
		//置顶
		case IDC_ONTOP:	
			
			if(SendDlgItemMessage(hDlg, IDC_ONTOP, BM_GETCHECK, 0, 0)==BST_CHECKED)
				SetWindowPos(GetParent(GetParent(hDlg)),HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			else
				SetWindowPos(GetParent(GetParent(hDlg)),HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		
			break;

        //保护			
		case IDC_PROT_BUTTON:
			EnableWindow(GetDlgItem(hDlg,IDC_PROT_BUTTON), FALSE);
			EnableWindow(GetDlgItem(hDlg,IDC_OPEN_BUTTON), FALSE);//压缩过程中打开按钮不可用
			//ProtTheFile(hDlg);
			//创建一个进程来压缩数据
		//	hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ProtTheFile, (VOID *)hDlg, NORMAL_PRIORITY_CLASS, &ProtFileThreadID);
			hThread = (HANDLE)_beginthreadex( NULL, 0, (unsigned int (_stdcall*)(void*))&ProtTheFile, (LPVOID)hDlg, 0, &ProtFileThreadID );
			CloseHandle(hThread);
	
			break;
        
	    //打开预处理
		case IDC_OPEN_BUTTON:
			if(!OpenFileDlg(szFilePath,hDlg))
				break;
			SendDlgItemMessage(hDlg,IDC_FILEPATH_EDIT,WM_SETTEXT,MAX_PATH,(LPARAM)szFilePath);
			AddLine(hDlg,szFilePath);
			if(!IsPEFile(szFilePath,hDlg)) 
				EnableWindow(GetDlgItem(hDlg,IDC_PROT_BUTTON), FALSE);
			else
				EnableWindow(GetDlgItem(hDlg,IDC_PROT_BUTTON), TRUE);
			SendMessage(hProgress,PBM_SETPOS, 0, 0);
			break;
		}

		return TRUE;
		break;
	}
	return FALSE;
}





//****************主对话框消息循环****************
int CALLBACK MainDlg (HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static int     i;
	static HWND    hwndTab;           //TAB控件句柄
	static HWND    Child1hWnd;        //3个子对话框句柄
	static HWND    Child2hWnd;
	static HWND    Child3hWnd;
	TC_ITEM ItemStruct;



	switch (uMsg)
	{
	case WM_CLOSE:
		if(FALSE==ISWORKING){
			delete pMessageBuffer;  // 释放内存
			DestroyWindow(hDlg); 		                       
		}
		else
		    MessageBox(NULL, TEXT ("正在压缩，不可退出！"), TEXT ("警告"), MB_OK) ;


		break;

	case WM_COMMAND: 
		switch (LOWORD (wParam))
		{
			case IDM_HELP_ABOUT :
				DialogBox (hInst, MAKEINTRESOURCE (IDD_ABOUT), hDlg, AboutDlgProc) ;
				break;

			case ID_MENU_OPEN:
				SendMessage(Child1hWnd, WM_COMMAND, (WPARAM)IDC_OPEN_BUTTON, 0);

				break;

			case ID_MENU_EXIT:
				SendMessage(hDlg, WM_CLOSE, 0, 0);
				break;


		}
		break;

	case WM_INITDIALOG:

		properinitDlgProc(hDlg);//配置对话框初始化

		SendMessage(hDlg,WM_SETICON,ICON_BIG,LPARAM(LoadIcon(hInst,MAKEINTRESOURCE(IDI_ICON)))); //设置图标
		EnableWindow(GetDlgItem(hDlg,IDC_PROT_BUTTON), FALSE);


		InitCommonControls();
		hwndTab = GetDlgItem(hDlg, IDC_TAB1);
		ItemStruct.mask        = TCIF_TEXT;
  		ItemStruct.iImage      = 0;
 		ItemStruct.lParam      = 0;
		ItemStruct.pszText     = "处理";
		ItemStruct.cchTextMax  = 4;
		SendMessage(hwndTab, TCM_INSERTITEM, 0, (LPARAM)&ItemStruct);

		ItemStruct.pszText     = "选项";
		ItemStruct.cchTextMax  = 4;
		SendMessage(hwndTab, TCM_INSERTITEM, 1, (LPARAM)&ItemStruct);
		
		Child1hWnd = CreateDialogParam(hInst, MAKEINTRESOURCE(IDD_CHILD1), hwndTab, StatusDlgProc, 0);
		Child2hWnd = CreateDialogParam(hInst, MAKEINTRESOURCE(IDD_CHILD2), hwndTab, PropertDlgProc , 0);

		ShowWindow(Child1hWnd, SW_SHOWDEFAULT);
		break;

	case WM_NOTIFY:      
	//2个子对话框间的切换
		if ( *(LPDWORD)((LPBYTE)lParam+8)==TCN_SELCHANGE )
		{
		//先隐藏所有子对话框
			ShowWindow(Child1hWnd, SW_HIDE);  
			ShowWindow(Child2hWnd, SW_HIDE);

			i = SendMessage(hwndTab, TCM_GETCURSEL, 0, 0);
			if (i == 0)
			{
				GetOption(Child2hWnd);//取得设置并保存到配置文件
				ShowWindow(Child1hWnd, SW_SHOWDEFAULT);
			}
			else if (i == 1)
			{
				ShowWindow(Child2hWnd, SW_SHOWDEFAULT);
			}
	
		}
		break;

	default:
		break;
	}

	return 0;
}
