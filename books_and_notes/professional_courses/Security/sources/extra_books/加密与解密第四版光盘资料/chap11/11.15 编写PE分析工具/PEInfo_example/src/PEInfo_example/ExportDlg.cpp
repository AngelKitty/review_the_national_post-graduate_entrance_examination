/*-----------------------------------------------------------------------
第11章  PE文件格式
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

#include "stdafx.h"
#include "resource.h"

#include "ExportDlg.h"
#include "PEFuncs.h"

extern MAP_FILE_STRUCT stMapFile;

LRESULT CALLBACK  ExportTableDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		InitExportListCtrl(hDlg);
		ShowExportDirInfo(hDlg);
		ShowExportFuncsInfo(hDlg);
		return TRUE;
		
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_OK:  
			SendMessage(hDlg, WM_CLOSE,0,0);
			return TRUE;
		}
		break;
		case WM_CLOSE:
			EndDialog(hDlg,0);
			return TRUE;;
	}
    return FALSE;
}
void InitExportListCtrl(HWND hDlg) 
{
	LVCOLUMN     lvColumn;
	memset(&lvColumn, 0, sizeof(lvColumn));
	
	lvColumn.mask      = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx        = 80;
	lvColumn.pszText   = "Ordinal";
	SendDlgItemMessage(hDlg, IDC_EXPORT_LIST, LVM_INSERTCOLUMN, 0, (LPARAM)&lvColumn);
	
	lvColumn.mask      = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx        = 80;
	lvColumn.pszText   = "RVA";
	SendDlgItemMessage(hDlg, IDC_EXPORT_LIST, LVM_INSERTCOLUMN, 1, (LPARAM)&lvColumn);
	
    lvColumn.mask      = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx        = 280;
	lvColumn.pszText   = "Function Name";
	SendDlgItemMessage(hDlg, IDC_EXPORT_LIST, LVM_INSERTCOLUMN, 2, (LPARAM)&lvColumn);
	
}
void   ShowExportDirInfo(HWND hDlg)
{
     char  cBuff[10], *szName; 
     PIMAGE_NT_HEADERS       pNtH=NULL;
     PIMAGE_EXPORT_DIRECTORY pExportDir=NULL;
	 pNtH=GetNtHeaders(stMapFile.ImageBase);
     pExportDir=GetExportDirectory(stMapFile.ImageBase);
	 if(!pExportDir)
	 {
		MessageBox(hDlg,"Can't get Export Directory:(","PEInfo_Example",MB_OK);
		return;
	 }

     wsprintf(cBuff, "%08lX", pExportDir->Characteristics);
	 SetDlgItemText(hDlg,IDC_EDIT_EXPDIR_CHARACTERISTICS,cBuff);
	 
	 wsprintf(cBuff, "%08lX", pExportDir->TimeDateStamp);
	 SetDlgItemText(hDlg,IDC_EDIT_EXPDIR_TDS,cBuff);
	 
	 wsprintf(cBuff, "%08lX", pExportDir->Base);
	 SetDlgItemText(hDlg,IDC_EDIT_EXPDIR_BASE,cBuff);
	 
	 wsprintf(cBuff, "%08lX", pExportDir->Name);
	 SetDlgItemText(hDlg,IDC_EDIT_EXPDIR_NAME,cBuff);
	 
	 wsprintf(cBuff, "%08lX", pExportDir->NumberOfFunctions);
	 SetDlgItemText(hDlg,IDC_EDIT_EXPDIR_NUMOFFUNCS,cBuff);
	 
	 wsprintf(cBuff, "%08lX", pExportDir->NumberOfNames);
	 SetDlgItemText(hDlg,IDC_EDIT_EXPDIR_NUMOFNAMES,cBuff);
	 
	 wsprintf(cBuff, "%08lX", pExportDir->AddressOfFunctions);
	 SetDlgItemText(hDlg,IDC_EDIT_EXPDIR_ADDROFFUNCS,cBuff);
	 
	 wsprintf(cBuff, "%08lX", pExportDir->AddressOfNames);
	 SetDlgItemText(hDlg,IDC_EDIT_EXPDIR_ADDROFNAMES,cBuff);
	 
	 wsprintf(cBuff, "%08lX", pExportDir->AddressOfNameOrdinals);
	 SetDlgItemText(hDlg,IDC_EDIT_EXPDIR_ADDROFNAMEORD,cBuff);
	 
	 szName=(char *)RvaToPtr(pNtH,stMapFile.ImageBase,pExportDir->Name);
	 SetDlgItemText(hDlg,IDC_EDIT_EXPDIR_DLLNAME,szName);
	 


}
void   ShowExportFuncsInfo(HWND hDlg)
{
	HWND         hList;
	LVITEM       lvItem;
	char         cBuff[10], *szFuncName; 
	
	UINT                    iNumOfName=0;
	PDWORD                  pdwRvas, pdwNames;
	PWORD                   pwOrds;
	UINT                    i=0,j=0,k=0; 
	BOOL                    bIsByName=FALSE;;


	PIMAGE_NT_HEADERS       pNtH=NULL;
    PIMAGE_EXPORT_DIRECTORY pExportDir=NULL;

    pNtH=GetNtHeaders(stMapFile.ImageBase);
    if(!pNtH)
		return ;
	pExportDir= (PIMAGE_EXPORT_DIRECTORY)GetExportDirectory(stMapFile.ImageBase);
	if (!pExportDir)
    		return ; 


	pwOrds    = (PWORD)RvaToPtr(pNtH, stMapFile.ImageBase,pExportDir->AddressOfNameOrdinals);
	pdwRvas   = (PDWORD)RvaToPtr(pNtH, stMapFile.ImageBase,pExportDir->AddressOfFunctions);
	pdwNames  = (PDWORD)RvaToPtr(pNtH, stMapFile.ImageBase,pExportDir->AddressOfNames);

	if(!pdwRvas)
		return;
  
	hList=GetDlgItem(hDlg,IDC_EXPORT_LIST);
	SendMessage(hList,LVM_SETEXTENDEDLISTVIEWSTYLE,0,(LPARAM)LVS_EX_FULLROWSELECT);
		
	
	iNumOfName=pExportDir->NumberOfNames;

	for( i=0;i<pExportDir->NumberOfFunctions;i++)
	{
		if(*pdwRvas)
		{    
			for( j=0;j<iNumOfName;j++)
			{
				if(i==pwOrds[j])
				{  
					bIsByName=TRUE;
					szFuncName=(char*)RvaToPtr(pNtH,stMapFile.ImageBase,pdwNames[j]);
					break;
				}
				
				bIsByName=FALSE;
			}
	         
	        //show funcs to listctrl
	
		memset(&lvItem, 0, sizeof(lvItem));
		lvItem.mask    = LVIF_TEXT;
		lvItem.iItem   = k;
       		
		lvItem.pszText = cBuff;
		wsprintf(cBuff, "%04lX", (UINT)(pExportDir->Base+i));
		SendDlgItemMessage(hDlg,IDC_EXPORT_LIST,LVM_INSERTITEM,0,(LPARAM)&lvItem);
	
        lvItem.pszText  = cBuff;
		wsprintf(cBuff, "%08lX", (*pdwRvas));
		lvItem.iSubItem = 1;
		SendDlgItemMessage(hDlg,IDC_EXPORT_LIST, LVM_SETITEM, 0, (LPARAM)&lvItem);
   
		if(bIsByName)			
			lvItem.pszText=szFuncName;
		else
			lvItem.pszText  = "-";
	
		lvItem.iSubItem = 2;
		SendDlgItemMessage(hDlg,IDC_EXPORT_LIST, LVM_SETITEM, 0, (LPARAM)&lvItem);
   
		
		   //
		++k;
	
		}
	
			++pdwRvas;
		  
 }
  	
  
}

