/*-----------------------------------------------------------------------
第11章  PE文件格式
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

#include "stdafx.h"
#include "resource.h"
#include "DataDirDlg.h"

#include "ExportDlg.h"
#include "ImportDlg.h"
#include "PEFuncs.h"

extern HINSTANCE hInst;
extern MAP_FILE_STRUCT stMapFile;

typedef struct
{
    UINT   ID_RVA;
    UINT   ID_SIZE;
} DataDir_EditID;

DataDir_EditID EditID_Array[]=
{
	{IDC_EDIT_DD_RVA_EXPORT,     IDC_EDIT_DD_SIZE_EXPORT},
    {IDC_EDIT_DD_RVA_IMPORT,     IDC_EDIT_DD_SIZE_IMPORT},
    {IDC_EDIT_DD_RVA_RES,        IDC_EDIT_DD_SZIE_RES},
    {IDC_EDIT_DD_RVA_EXCEPTION,  IDC_EDIT_DD_SZIE_EXCEPTION},
	{IDC_EDIT_DD_RVA_SECURITY,	 IDC_EDIT_DD_SIZE_SECURITY},
    {IDC_EDIT_DD_RVA_RELOC,		 IDC_EDIT_DD_SIZE_RELOC},
    {IDC_EDIT_DD_RVA_DEBUG,		 IDC_EDIT_DD_SIZE_DEBUG},
	{IDC_EDIT_DD_RVA_COPYRIGHT,	 IDC_EDIT_DD_SIZE_COPYRIGHT},
	{IDC_EDIT_DD_RVA_GP,		 IDC_EDIT_DD_SIZE_GP},
    {IDC_EDIT_DD_RVA_TLS,        IDC_EDIT_DD_SIZE_TLS},
	{IDC_EDIT_DD_RVA_LOADCONFIG, IDC_EDIT_DD_SIZE_LOADCONFIG},
	{IDC_EDIT_DD_RVA_IAT,		 IDC_EDIT_DD_SIZE_IAT},
	{IDC_EDIT_DD_RVA_BOUND,		 IDC_EDIT_DD_SIZE_BOUND},
	{IDC_EDIT_DD_RVA_COM,		 IDC_EDIT_DD_SIZE_COM},
	{IDC_EDIT_DD_RVA_DELAYIMPORT,IDC_EDIT_DD_SIZE_DELAYIMPORT},
	{IDC_EDIT_DD_RVA_NOUSE,		 IDC_EDIT_DD_SIZE_NOUSE}
 };

LRESULT CALLBACK  DataDirDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
   int wmId, wmEvent;
	switch (message)
	{
	case WM_INITDIALOG:
		ShowDataDirInfo(hDlg);
			return TRUE;
		
	case WM_COMMAND:
		wmId    = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 
		switch (wmId)
		{
			
		case IDC_OK:  
			SendMessage(hDlg, WM_CLOSE,0,0);
			return TRUE;
			
		case IDC_SHOWEXPORT_BTN: 
			if(IsDataDirPresent(stMapFile.ImageBase,IMAGE_DIRECTORY_ENTRY_EXPORT))
			   DialogBox(hInst, MAKEINTRESOURCE(IDD_EXPORT_DLG), hDlg, (DLGPROC)ExportTableDlgProc);
			else
				MessageBox(hDlg,"No Export Table present","PEInfo_example",MB_OK);
			return TRUE;
			
		case IDC_SHOWIMPORT_BTN: 
			if(IsDataDirPresent(stMapFile.ImageBase,IMAGE_DIRECTORY_ENTRY_IMPORT))
				DialogBox(hInst, MAKEINTRESOURCE(IDD_IMPORT_DLG), hDlg, (DLGPROC)ImportTableDlgProc);
			else
				MessageBox(hDlg,"No Import Table present","PEInfo_example",MB_OK);
			return TRUE;
		}
		break;
		case WM_CLOSE:
			EndDialog(hDlg, 0);
			return  TRUE;
	}
   
	
	return FALSE;
	
}
void ShowDataDirInfo(HWND hDlg)
{
    char   cBuff[9];
    PIMAGE_OPTIONAL_HEADER pOH=NULL;
    pOH=GetOptionalHeader(stMapFile.ImageBase);
   if(!pOH)
        return;

	for(int i=0;i<16;i++)
   {
    wsprintf(cBuff, "%08lX", pOH->DataDirectory[i].VirtualAddress);
    SetDlgItemText(hDlg,EditID_Array[i].ID_RVA,cBuff);
   
	 wsprintf(cBuff, "%08lX", pOH->DataDirectory[i].Size);
     SetDlgItemText(hDlg,EditID_Array[i].ID_SIZE,cBuff);
	}
   
}