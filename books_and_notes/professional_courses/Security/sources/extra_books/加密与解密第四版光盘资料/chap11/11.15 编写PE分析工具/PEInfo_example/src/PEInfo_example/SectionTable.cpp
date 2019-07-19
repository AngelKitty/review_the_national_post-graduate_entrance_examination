/*-----------------------------------------------------------------------
第11章  PE文件格式
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

#include "stdafx.h"
#include "resource.h"
#include "SectionTable.h"
#include "PEFuncs.h"
extern MAP_FILE_STRUCT stMapFile;

LRESULT CALLBACK  SectionTableDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
		switch (message)
	{
	case WM_INITDIALOG:
		InitSectionListCtrl(hDlg);
		ShowSectionHeaderInfo(hDlg);
		return TRUE;
		
	case WM_COMMAND:
	
		break;
	case WM_CLOSE:
		EndDialog(hDlg,0);
		return TRUE;;
	}
    return FALSE;
	
}
void  InitSectionListCtrl(HWND hDlg)
{
	HWND         hList;
	LVCOLUMN     lvColumn;
	memset(&lvColumn, 0, sizeof(lvColumn));
	
	hList=GetDlgItem(hDlg,IDC_SECTIONLIST);
	SendMessage(hList,LVM_SETEXTENDEDLISTVIEWSTYLE,0,(LPARAM)LVS_EX_FULLROWSELECT);

	lvColumn.mask      = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx        = 80;
	lvColumn.pszText   = "Name";
	SendDlgItemMessage(hDlg, IDC_SECTIONLIST, LVM_INSERTCOLUMN, 0, (LPARAM)&lvColumn);
	
	lvColumn.mask      = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx        = 80;
	lvColumn.pszText   = "VAddress";
	SendDlgItemMessage(hDlg, IDC_SECTIONLIST, LVM_INSERTCOLUMN, 1, (LPARAM)&lvColumn);
	
    lvColumn.mask      = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx        = 80;
	lvColumn.pszText   = "VSize";
	SendDlgItemMessage(hDlg, IDC_SECTIONLIST, LVM_INSERTCOLUMN, 2, (LPARAM)&lvColumn);
	
    lvColumn.mask      = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx        = 80;
	lvColumn.pszText   = "ROffset";
	SendDlgItemMessage(hDlg, IDC_SECTIONLIST, LVM_INSERTCOLUMN, 3, (LPARAM)&lvColumn);
	
	
    lvColumn.mask      = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx        = 80;
	lvColumn.pszText   = "RSize";
	SendDlgItemMessage(hDlg, IDC_SECTIONLIST, LVM_INSERTCOLUMN, 4, (LPARAM)&lvColumn);
	
	lvColumn.mask      = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx        = 80;
	lvColumn.pszText   = "Flags";
	SendDlgItemMessage(hDlg, IDC_SECTIONLIST, LVM_INSERTCOLUMN, 5, (LPARAM)&lvColumn);
		
}
void ShowSectionHeaderInfo(HWND hDlg)
{
	LVITEM                  lvItem;
	char                    cBuff[9],cName[9]; 
	WORD                    i;
    PIMAGE_FILE_HEADER       pFH=NULL;
	PIMAGE_SECTION_HEADER   pSH=NULL;


	pFH=GetFileHeader(stMapFile.ImageBase);
	if(!pFH)
        return;
	
	pSH=GetFirstSectionHeader(stMapFile.ImageBase);

	for( i=0;i<pFH->NumberOfSections;i++)
	{
		memset(&lvItem, 0, sizeof(lvItem));
		lvItem.mask    = LVIF_TEXT;
		lvItem.iItem   = i;

		memset(cName,0,sizeof(cName));
		memcpy(cName, pSH->Name, 8);
	
		lvItem.pszText = cName;
		SendDlgItemMessage(hDlg,IDC_SECTIONLIST,LVM_INSERTITEM,0,(LPARAM)&lvItem);
	
        lvItem.pszText  = cBuff;
		wsprintf(cBuff, "%08lX", pSH->VirtualAddress);
		lvItem.iSubItem = 1;
		SendDlgItemMessage(hDlg,IDC_SECTIONLIST, LVM_SETITEM, 0, (LPARAM)&lvItem);
       
		wsprintf(cBuff, "%08lX", pSH->Misc.VirtualSize);
		lvItem.iSubItem = 2;
		SendDlgItemMessage(hDlg,IDC_SECTIONLIST, LVM_SETITEM, 0, (LPARAM)&lvItem);

		wsprintf(cBuff, "%08lX", pSH->PointerToRawData);
		lvItem.iSubItem = 3;
		SendDlgItemMessage(hDlg,IDC_SECTIONLIST, LVM_SETITEM, 0, (LPARAM)&lvItem);
        
		wsprintf(cBuff, "%08lX", pSH->SizeOfRawData);
		lvItem.iSubItem = 4;
		SendDlgItemMessage(hDlg,IDC_SECTIONLIST, LVM_SETITEM, 0, (LPARAM)&lvItem);
		
		wsprintf(cBuff, "%08lX", pSH->Characteristics);
		lvItem.iSubItem = 5;
		SendDlgItemMessage(hDlg,IDC_SECTIONLIST, LVM_SETITEM, 0, (LPARAM)&lvItem);

		++pSH;
	}


}