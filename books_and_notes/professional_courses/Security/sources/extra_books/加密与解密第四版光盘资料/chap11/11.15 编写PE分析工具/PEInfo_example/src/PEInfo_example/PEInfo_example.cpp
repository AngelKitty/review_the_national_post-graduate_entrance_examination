/*-----------------------------------------------------------------------
第11章  PE文件格式
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

// PEInfo_example.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include  <commdlg.h>
#include <commctrl.h>
#include "PEFuncs.h"
#include "PEInfo_example.h"

#include  "SectionTable.h"
#include  "DataDirDlg.h"

char szFilePath[MAX_PATH];
HINSTANCE hInst;
HICON     hIcon;			
MAP_FILE_STRUCT stMapFile={0,0,0};


int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	hInst = hInstance;
	DialogBox(hInstance, (LPCTSTR)IDD_MAIN_DLG, NULL, (DLGPROC)MainDlgProc);
	return(FALSE);
}

LRESULT CALLBACK MainDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	switch (message) 
	{
		case WM_INITDIALOG:
			  hIcon=LoadIcon(hInst,MAKEINTRESOURCE(IDI_SMALL));
			  SendMessage(hWnd,WM_SETICON,ICON_SMALL,(LPARAM)hIcon);
			  InitCommonControls();
			  EnableEditCtrl(hWnd,FALSE);
				return TRUE;

		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
		
			switch (wmId)
			{
				case IDC_ABOUTBTN:
				   DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, (DLGPROC)About);
				   return TRUE;
				
				case IDC_EXIT:
					SendMessage(hWnd, WM_CLOSE,0,0);
				  return TRUE;
		
				case IDC_OPEN_BTN:
					if(!OpenFileDlg(hWnd))
						return FALSE;
   					SendDlgItemMessage(hWnd,IDC_FILEPATH_EDIT,WM_SETTEXT,MAX_PATH,(LPARAM)szFilePath);
					
					UnLoadFile(&stMapFile);
					if(!LoadFileR(szFilePath,&stMapFile))
					{
						MessageBox(hWnd,"Load file faild! :(","PEInfo_Example",MB_OK);
						EnableEditCtrl(hWnd,FALSE);
						return FALSE;
					}
					if(!IsPEFile(stMapFile.ImageBase))
					{
						MessageBox(hWnd,"Invalid PE Format file!:(","PEInfo_Example",MB_OK);
						UnLoadFile(&stMapFile);
							EnableEditCtrl(hWnd,FALSE);
						return FALSE;
					}
					EnableEditCtrl(hWnd,TRUE);			
                    ShowFileHeaderInfo(hWnd);
					ShowOptionHeaderInfo(hWnd);
				
					return TRUE;
				
				case IDC_SHOWSHT:  // show sectiontable dlg;				    			
						DialogBox(hInst, MAKEINTRESOURCE(IDD_SHT_DLG), hWnd, (DLGPROC)SectionTableDlgProc);
				   return TRUE;
			
		    	case IDC_SHOWDATADIR:  // show DataDirectory dlg;
				    DialogBox(hInst, MAKEINTRESOURCE(IDD_DATADIR_DLG), hWnd, (DLGPROC)DataDirDlgProc);
				   return TRUE;
			
			}
			break;
	
		case WM_CLOSE:
			UnLoadFile(&stMapFile);
			EndDialog(hWnd, 0);
			break;
		
   }
   return 0;
}

LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;
		
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK) 
		{
			SendMessage(hDlg,WM_CLOSE,0,0);
			return TRUE;
		}
		break;
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		break;
	}
    return FALSE;
}
BOOL  OpenFileDlg(HWND hwnd)   
{
	OPENFILENAME ofn;
    memset(szFilePath,0,MAX_PATH);
	memset(&ofn, 0, sizeof(ofn));

	ofn.lStructSize      =sizeof(ofn);
	ofn.hwndOwner        =hwnd;
	ofn.hInstance        =GetModuleHandle(NULL);
	ofn.nMaxFile         =MAX_PATH;
	ofn.lpstrInitialDir  =".";
	ofn.lpstrFile        =szFilePath;
	ofn.lpstrTitle       ="Open ...[PEInfo_example] by A.Yong(阿勇)";
	ofn.Flags            =OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrFilter      ="*.*\0*.*\0";
	if(!GetOpenFileName(&ofn))
		return FALSE;
	return TRUE;

}
void    ShowFileHeaderInfo(HWND hWnd)
{    
	char   cBuff[10];
     PIMAGE_FILE_HEADER pFH=NULL;
     
	 pFH=GetFileHeader(stMapFile.ImageBase);
     if(!pFH)
	 {
		 MessageBox(hWnd,"Can't get File Header ! :(","PEInfo_Example",MB_OK);
	     return;
	 }
	 wsprintf(cBuff, "%04lX", pFH->Machine);
	 SetDlgItemText(hWnd,IDC_EDIT_FH_MACHINE,cBuff);
	 
	 wsprintf(cBuff, "%04lX", pFH->NumberOfSections);
	 SetDlgItemText(hWnd,IDC_EDIT_FH_NUMOFSECTIONS,cBuff);
	 
	 wsprintf(cBuff, "%08lX", pFH->TimeDateStamp);
	 SetDlgItemText(hWnd,IDC_EDIT_FH_TDS,cBuff);
	 
	 wsprintf(cBuff, "%08lX", pFH->PointerToSymbolTable);
	 SetDlgItemText(hWnd,IDC_EDIT_FH_PTSYMBOL,cBuff);
	 
	 wsprintf(cBuff, "%08lX", pFH->NumberOfSymbols);
	 SetDlgItemText(hWnd,IDC_EDIT_FH_NUMOFSYM,cBuff);
	 
	 wsprintf(cBuff, "%04lX", pFH->SizeOfOptionalHeader);
	 SetDlgItemText(hWnd,IDC_EDIT_FH_SIZEOFOH,cBuff);
	 
	 wsprintf(cBuff, "%04lX", pFH->Characteristics);
	 SetDlgItemText(hWnd,IDC_EDIT_FH_CHARACTERISTICS,cBuff);
	 

}
void    ShowOptionHeaderInfo(HWND hWnd)       
{
	char   cBuff[10];
	PIMAGE_OPTIONAL_HEADER pOH=NULL;
    pOH=GetOptionalHeader(stMapFile.ImageBase);
	if(!pOH)
	{
	 MessageBox(hWnd,"Can't get Optional Header !:(","PEInfo_Example",MB_OK);
	     return;
	}
     wsprintf(cBuff, "%08lX", pOH->AddressOfEntryPoint);
	 SetDlgItemText(hWnd,IDC_EDIT_OH_EP,cBuff);

     wsprintf(cBuff, "%08lX", pOH->ImageBase);
	 SetDlgItemText(hWnd,IDC_EDIT_OH_IMAGEBASE,cBuff);

	 wsprintf(cBuff, "%08lX", pOH->BaseOfCode);
	 SetDlgItemText(hWnd,IDC_EDIT_OH_CODEBASE,cBuff);

	 wsprintf(cBuff, "%08lX", pOH->BaseOfData);
	 SetDlgItemText(hWnd,IDC_EDIT_OH_DATABASE,cBuff);

	 wsprintf(cBuff, "%08lX", pOH->SizeOfImage);
	 SetDlgItemText(hWnd,IDC_EDIT_OH_IMAGESIZE,cBuff);

     wsprintf(cBuff, "%08lX", pOH->SizeOfHeaders);
     SetDlgItemText(hWnd,IDC_EDIT_OH_HEADERSIZE,cBuff);

	 wsprintf(cBuff, "%08lX", pOH->SectionAlignment);
	 SetDlgItemText(hWnd,IDC_EDIT_OH_SECTIONALIGN,cBuff);

	 wsprintf(cBuff, "%08lX", pOH->FileAlignment);
	 SetDlgItemText(hWnd,IDC_EDIT_OH_FILEALIGN,cBuff);

	 wsprintf(cBuff, "%08lX", pOH->Subsystem);
     SetDlgItemText(hWnd,IDC_EDIT_OH_SUBSYSTEM,cBuff);

	 wsprintf(cBuff, "%04lX", pOH->CheckSum);
	 SetDlgItemText(hWnd,IDC_EDIT_OH_CHECKSUM,cBuff);

	 wsprintf(cBuff, "%04lX", pOH->DllCharacteristics);
     SetDlgItemText(hWnd,IDC_EDIT_OH_DLLFLAGS,cBuff);
}
void    EnableEditCtrl(HWND hWnd,BOOL bLoad)
{
	if(!bLoad)
	{   
		
		SendDlgItemMessage(hWnd,IDC_EDIT_OH_EP,WM_SETTEXT,MAX_PATH,NULL);
		SendDlgItemMessage(hWnd,IDC_EDIT_OH_IMAGEBASE,WM_SETTEXT,MAX_PATH,(LPARAM)NULL);
		SendDlgItemMessage(hWnd,IDC_EDIT_OH_CODEBASE,WM_SETTEXT,MAX_PATH,(LPARAM)NULL);
		SendDlgItemMessage(hWnd,IDC_EDIT_OH_DATABASE,WM_SETTEXT,MAX_PATH,(LPARAM)NULL);
		SendDlgItemMessage(hWnd,IDC_EDIT_OH_IMAGESIZE,WM_SETTEXT,MAX_PATH,(LPARAM)NULL);
		SendDlgItemMessage(hWnd,IDC_EDIT_OH_HEADERSIZE,WM_SETTEXT,MAX_PATH,(LPARAM)NULL);		
		SendDlgItemMessage(hWnd,IDC_EDIT_OH_SECTIONALIGN,WM_SETTEXT,MAX_PATH,NULL);
		SendDlgItemMessage(hWnd,IDC_EDIT_OH_FILEALIGN,WM_SETTEXT,MAX_PATH,NULL);
		SendDlgItemMessage(hWnd,IDC_EDIT_OH_SUBSYSTEM,WM_SETTEXT,MAX_PATH,NULL);
		SendDlgItemMessage(hWnd,IDC_EDIT_OH_CHECKSUM,WM_SETTEXT,MAX_PATH,NULL);
		SendDlgItemMessage(hWnd,IDC_EDIT_OH_DLLFLAGS,WM_SETTEXT,MAX_PATH,NULL);
		
		SendDlgItemMessage(hWnd,IDC_EDIT_FH_MACHINE,WM_SETTEXT,MAX_PATH,(LPARAM)NULL);
		SendDlgItemMessage(hWnd,IDC_EDIT_FH_NUMOFSECTIONS,WM_SETTEXT,MAX_PATH,NULL);
		SendDlgItemMessage(hWnd,IDC_EDIT_FH_TDS,WM_SETTEXT,MAX_PATH,NULL);
		SendDlgItemMessage(hWnd,IDC_EDIT_FH_PTSYMBOL,WM_SETTEXT,MAX_PATH,NULL);
		SendDlgItemMessage(hWnd,IDC_EDIT_FH_NUMOFSYM,WM_SETTEXT,MAX_PATH,NULL);
		SendDlgItemMessage(hWnd,IDC_EDIT_FH_SIZEOFOH,WM_SETTEXT,MAX_PATH,NULL);
		SendDlgItemMessage(hWnd,IDC_EDIT_FH_CHARACTERISTICS,WM_SETTEXT,MAX_PATH,NULL);
	

	}	
		EnableWindow(GetDlgItem(hWnd,IDC_SHOWSHT),bLoad);
		EnableWindow(GetDlgItem(hWnd,IDC_SHOWDATADIR),bLoad);
	

}