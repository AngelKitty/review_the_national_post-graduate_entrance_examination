/*-----------------------------------------------------------------------
第11章  PE文件格式
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

#include "stdafx.h"
#include "resource.h"
#include "ImportDlg.h"
#include "PEFuncs.h"

#define GETTHUNK(pImportDesc) ((DWORD)                          \
         ((PIMAGE_IMPORT_DESCRIPTOR)pImportDesc->OriginalFirstThunk ?                      \
         (PIMAGE_IMPORT_DESCRIPTOR)pImportDesc->OriginalFirstThunk:(PIMAGE_IMPORT_DESCRIPTOR)pImportDesc->FirstThunk \
          ))
extern MAP_FILE_STRUCT stMapFile;

LRESULT CALLBACK  ImportTableDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	int wmId;
	LPNMHDR pnmh;
	LPNMLISTVIEW lpnmlv;

	
	switch (message)
	{
	case WM_INITDIALOG:
		InitImportListCtrl( hDlg);
		InitImportFunctionsListCtrl(hDlg);
		ShowImportDescInfo(hDlg);
		return TRUE;
		
	case WM_NOTIFY:
            wmId    = LOWORD(wParam); 		 
			pnmh = (LPNMHDR) lParam; 
	   if (wmId==IDC_IMPORT_LIST)
	   {
		   switch (pnmh->code)
		   {
		   case LVN_ITEMCHANGED:
                lpnmlv=(LPNMLISTVIEW) lParam; 
		  		 if(lpnmlv->uNewState)
					ShowImportFuncsByDllIndex(hDlg,lpnmlv->iItem);
				 

		   }

	   }
		return TRUE;
	case WM_CLOSE:
		EndDialog(hDlg,0);
		return TRUE;;
	}
    return FALSE;
}
void InitImportListCtrl(HWND hDlg)
{
    LVCOLUMN     lvColumn;
	memset(&lvColumn, 0, sizeof(lvColumn));
	
	lvColumn.mask      = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx        = 98;
	lvColumn.pszText   = "DLL Name";
	SendDlgItemMessage(hDlg, IDC_IMPORT_LIST, LVM_INSERTCOLUMN, 0, (LPARAM)&lvColumn);
	
	lvColumn.mask      = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx        = 98;
	lvColumn.pszText   = "OriginalFirstThunk";
	SendDlgItemMessage(hDlg, IDC_IMPORT_LIST, LVM_INSERTCOLUMN, 1, (LPARAM)&lvColumn);
	
    lvColumn.mask      = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx        = 98;
	lvColumn.pszText   = "TimeDateStamp";
	SendDlgItemMessage(hDlg, IDC_IMPORT_LIST, LVM_INSERTCOLUMN, 2, (LPARAM)&lvColumn);
	
    lvColumn.mask      = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx        = 98;
	lvColumn.pszText   = "ForwarderChin";
	SendDlgItemMessage(hDlg, IDC_IMPORT_LIST, LVM_INSERTCOLUMN, 3, (LPARAM)&lvColumn);
	
	
    lvColumn.mask      = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx        = 98;
	lvColumn.pszText   = "Name";
	SendDlgItemMessage(hDlg, IDC_IMPORT_LIST, LVM_INSERTCOLUMN, 4, (LPARAM)&lvColumn);
	
	lvColumn.mask      = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx        = 98;
	lvColumn.pszText   = "FirstThunk";
	SendDlgItemMessage(hDlg, IDC_IMPORT_LIST, LVM_INSERTCOLUMN, 5, (LPARAM)&lvColumn);
		

}
void InitImportFunctionsListCtrl(HWND hDlg)
{
LVCOLUMN     lvColumn;
	memset(&lvColumn, 0, sizeof(lvColumn));
	
	lvColumn.mask      = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx        = 80;
	lvColumn.pszText   = "ThunkRVA";
	SendDlgItemMessage(hDlg, IDC_IMPORTFUNCTIONS_LIST, LVM_INSERTCOLUMN, 0, (LPARAM)&lvColumn);
	
    lvColumn.mask      = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx        = 80;
	lvColumn.pszText   = "ThunkValue";
	SendDlgItemMessage(hDlg, IDC_IMPORTFUNCTIONS_LIST, LVM_INSERTCOLUMN, 1, (LPARAM)&lvColumn);
	
    lvColumn.mask      = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx        = 50;
	lvColumn.pszText   = "Hint";
	SendDlgItemMessage(hDlg, IDC_IMPORTFUNCTIONS_LIST, LVM_INSERTCOLUMN, 2, (LPARAM)&lvColumn);
	
	
    lvColumn.mask      = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx        = 280;
	lvColumn.pszText   = "Function Name";
	SendDlgItemMessage(hDlg, IDC_IMPORTFUNCTIONS_LIST, LVM_INSERTCOLUMN, 3, (LPARAM)&lvColumn);
	

}
void  ShowImportDescInfo(HWND hDlg)
{
	HWND         hList;
	LVITEM       lvItem;
	char         cBuff[10], * szDllName; 
	
    PIMAGE_NT_HEADERS       pNtH=NULL;
	PIMAGE_IMPORT_DESCRIPTOR  pImportDesc=NULL;

	memset(&lvItem, 0, sizeof(lvItem));
	
	hList=GetDlgItem(hDlg,IDC_IMPORT_LIST);
	SendMessage(hList,LVM_SETEXTENDEDLISTVIEWSTYLE,0,(LPARAM)LVS_EX_FULLROWSELECT);
 
	pNtH=GetNtHeaders(stMapFile.ImageBase);
	pImportDesc=GetFirstImportDesc(stMapFile.ImageBase);
    if(!pImportDesc)
	{
		MessageBox(hDlg,"Can't get ImportDesc:(","PEInfo_Example",MB_OK);
		return;
	}
	
	int i=0;
    while(pImportDesc->FirstThunk)
	{
		
		memset(&lvItem, 0, sizeof(lvItem));
		lvItem.mask    = LVIF_TEXT;
		lvItem.iItem   = i;
       
		szDllName=(char*)RvaToPtr(pNtH,stMapFile.ImageBase,pImportDesc->Name);
		
		lvItem.pszText = szDllName;
		SendDlgItemMessage(hDlg,IDC_IMPORT_LIST,LVM_INSERTITEM,0,(LPARAM)&lvItem);
	
        lvItem.pszText  = cBuff;
		wsprintf(cBuff, "%08lX", pImportDesc->OriginalFirstThunk);
		lvItem.iSubItem = 1;
		SendDlgItemMessage(hDlg,IDC_IMPORT_LIST, LVM_SETITEM, 0, (LPARAM)&lvItem);
   
	    lvItem.pszText  = cBuff;
		wsprintf(cBuff, "%08lX", pImportDesc->TimeDateStamp);
		lvItem.iSubItem = 2;
		SendDlgItemMessage(hDlg,IDC_IMPORT_LIST, LVM_SETITEM, 0, (LPARAM)&lvItem);
   
		lvItem.pszText  = cBuff;
		wsprintf(cBuff, "%08lX", pImportDesc->ForwarderChain);
		lvItem.iSubItem = 3;
		SendDlgItemMessage(hDlg,IDC_IMPORT_LIST, LVM_SETITEM, 0, (LPARAM)&lvItem);
   
		lvItem.pszText  = cBuff;
		wsprintf(cBuff, "%08lX", pImportDesc->Name);
		lvItem.iSubItem = 4;
		SendDlgItemMessage(hDlg,IDC_IMPORT_LIST, LVM_SETITEM, 0, (LPARAM)&lvItem);
   
		lvItem.pszText  = cBuff;
		wsprintf(cBuff, "%08lX", pImportDesc->FirstThunk);
		lvItem.iSubItem = 5;
		SendDlgItemMessage(hDlg,IDC_IMPORT_LIST, LVM_SETITEM, 0, (LPARAM)&lvItem);
   
	   ++i;
	   ++pImportDesc;
	}

}
 void ShowImportFuncsByDllIndex(HWND hDlg,int index)
 {
    HWND         hFuncList;
	LVITEM       lvItem;
	char         cBuff[30],cOrd[30],cMemAddr[30], * szFuncName;
    DWORD        dwThunk, *pdwThunk=NULL, *pdwRVA=NULL;
    int i=0;
		
	PIMAGE_NT_HEADERS         pNtH=NULL;
	PIMAGE_IMPORT_DESCRIPTOR  pFistImportDesc=NULL,pCurrentImportDesc=NULL;
    PIMAGE_IMPORT_BY_NAME     pByName=NULL;
	memset(&lvItem, 0, sizeof(lvItem));
	
	hFuncList=GetDlgItem(hDlg,IDC_IMPORTFUNCTIONS_LIST);
	SendMessage(hFuncList,LVM_SETEXTENDEDLISTVIEWSTYLE,0,(LPARAM)LVS_EX_FULLROWSELECT);
    SendMessage(hFuncList,LVM_DELETEALLITEMS ,0,0);
 
	pNtH=GetNtHeaders(stMapFile.ImageBase);
	pFistImportDesc=GetFirstImportDesc(stMapFile.ImageBase);
    pCurrentImportDesc=&pFistImportDesc[index];
	dwThunk=GETTHUNK(pCurrentImportDesc);

	pdwRVA=(DWORD *)dwThunk;
	pdwThunk=(DWORD*)RvaToPtr(pNtH,stMapFile.ImageBase,dwThunk);
   	if(!pdwThunk)
		    return;

	while(*pdwThunk)
    {
		memset(&lvItem, 0, sizeof(lvItem));
		lvItem.mask    = LVIF_TEXT;
		lvItem.iItem   = i;

		lvItem.pszText = cBuff;
		wsprintf(cBuff, "%08lX",(DWORD)pdwRVA);
		SendDlgItemMessage(hDlg,IDC_IMPORTFUNCTIONS_LIST,LVM_INSERTITEM,0,(LPARAM)&lvItem);
		
		lvItem.pszText  = cBuff;
		wsprintf(cBuff, "%08lX", (DWORD)(*pdwThunk));
		lvItem.iSubItem = 1;
		SendDlgItemMessage(hDlg,IDC_IMPORTFUNCTIONS_LIST, LVM_SETITEM, 0, (LPARAM)&lvItem);
   
		if (HIWORD(*pdwThunk)==0x8000)
		{			
			strcpy(cBuff,"-");
			wsprintf(cOrd, "Ord:%08lX",IMAGE_ORDINAL32(*pdwThunk));
			szFuncName=cOrd;
		}
		else 
		{
			pByName =(PIMAGE_IMPORT_BY_NAME)RvaToPtr(pNtH,stMapFile.ImageBase,(DWORD)(*pdwThunk));
			if(pByName)
			{
				wsprintf(cBuff,"%04lX",pByName->Hint);
				szFuncName=(char *)pByName->Name;
			}
			else
			{
				strcpy(cBuff,"-");
				wsprintf(cMemAddr, "MemAddr:%08lX",(DWORD)(*pdwThunk));
				szFuncName=cMemAddr;
			}
		}
   
		lvItem.pszText  = cBuff;		
		lvItem.iSubItem = 2;
		SendDlgItemMessage(hDlg,IDC_IMPORTFUNCTIONS_LIST, LVM_SETITEM, 0, (LPARAM)&lvItem);

    
		lvItem.pszText = szFuncName;
		lvItem.iSubItem = 3;
		SendDlgItemMessage(hDlg,IDC_IMPORTFUNCTIONS_LIST, LVM_SETITEM, 0, (LPARAM)&lvItem);    
	
		
		++i;
		++pdwRVA;
		++pdwThunk;
	
	}
 
 
 
 
 
 }
 
