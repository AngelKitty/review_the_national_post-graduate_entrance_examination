/*-----------------------------------------------------------------------
第22章  补丁技术
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

#include "resource.h"
#include "peFile.c"
#include <windows.h>


//自定义程序入口，如果要优化，推荐使用这个 
//如果使用 VS.NET，也可以这样设置：在“解决方案管理器”里选定项目，右击，选择“属性”->“链接器”->“高级”->“入口点” 
//VC 6里面也有类似的设置，但具体不记得了 
//项目属性的其它设置可以仔细看看，其它优化也都在这里可以设置 
//#pragma comment(linker, "/ENTRY:EntryPoint") 

//下面是调整段对齐，默认是 4K，在 Win98 下能更快地装入 PE 文件，但会增加 PE 文件的大小 
//下面这一行在这里好像会说是“无效的指令”什么的，故在这里也注释掉了，也许是段太小的原因吧。 
//直接在工程选项里面设置：项目“属性”->“链接器”->“优化”->“Windows98 优化”->“否 (/OPT:NOWIN98)” 
//#pragma comment(linker, "/OPT:NOWIN98") 


//////////////////////////////////////////////////////////////////////////////////////////////////// 

//////////////////////////////////////////////////////////////////////////////////////////////////// 
//入口函数 
//使用我们自己的入口函数，而不用连接器默认提供的一大堆程序初始化操作的代码 
//为了在一个普通的 Win32SDK 程序里能使用这种方法，下面的函数将调用 WinMain() 函数，并给出相应的参数 

/*
void EntryPoint() 
{ 
      ExitProcess(WinMain(GetModuleHandle(NULL), NULL, NULL, SW_SHOWNORMAL)); 
}
*/
//////////////////////////////////////////////////////////////////////////////////////////////////// 


HINSTANCE hInst ;
TCHAR szFileName[30], szTitleName[30] ;
HINSTANCE dllhander;

BOOL CALLBACK MainDlg(HWND, UINT, WPARAM, LPARAM);

BOOL Patch();


int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
	hInst = hInstance;
	DialogBoxParam (hInstance, MAKEINTRESOURCE(IDD_MAINDLG), NULL, MainDlg, NULL);
	return 0;	
}

BOOL CALLBACK MainDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
	    case WM_INITDIALOG:
			PopFileInitialize (hDlg) ;	 
			SendMessage(hDlg,WM_SETICON,ICON_BIG,LPARAM(LoadIcon(hInst,MAKEINTRESOURCE(IDI_ICON)))); //设置图标
			break; 
		
        //close the dialog
        case WM_CLOSE:
            EndDialog(hDlg, NULL); //end modal dialog with EndDialog
		                           //and it has not WM_DESTORY message 
		    break;
			

		case WM_COMMAND:
			switch (LOWORD (wParam))
			{
			case IDOK:
				PopFileOpenDlg (hDlg, szFileName, szTitleName);
				EnableWindow(GetDlgItem(hDlg,IDOK2),TRUE);
				SendMessage(GetDlgItem(hDlg,IDC_ts), WM_SETTEXT, 0, (LPARAM)szFileName);
				break;
			case IDPATCH:				
				if(Patch())
					MessageBox(NULL,"补丁完成!","Patch",MB_ICONINFORMATION);
				else
					MessageBox(NULL,"补丁失败!","ERROR",0);

				break;

			case IDCANCEL:
				EndDialog(hDlg,NULL);
				break;
			}
			break;

		default:
			return FALSE;
	}
	
	return true;
}     


BOOL Patch()
{
	HANDLE hFile;
	DWORD szTemp;
	DWORD FileSize;
	DWORD lFileSize=40960;                //文件大小
	DWORD lFileOffset=0x11E3;                //需要修改的文件偏移地址
	DWORD lChanges=2;                  //需要修补的字节数
	BYTE BytesToWrite[]={0x6A,00};        //写的数据
	
	
	hFile = CreateFile( szFileName,	
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	
	if( hFile != INVALID_HANDLE_VALUE )
	{
		FileSize=GetFileSize(hFile,&szTemp);  // 获取文件大小
		if (FileSize == 0xFFFFFFFF) {
			CloseHandle(hFile);
			return FALSE;
		}
		if(FileSize!=lFileSize){
			CloseHandle(hFile);
			return FALSE;
		}
		
		SetFilePointer(hFile,lFileOffset,NULL,FILE_BEGIN);//设置文件指针
		if(!WriteFile(hFile,&BytesToWrite,lChanges,&szTemp,NULL))
		{
			CloseHandle(hFile);
			return FALSE;
		}
				
		CloseHandle(hFile);		
		return TRUE;
	}
	return FALSE;
	
}