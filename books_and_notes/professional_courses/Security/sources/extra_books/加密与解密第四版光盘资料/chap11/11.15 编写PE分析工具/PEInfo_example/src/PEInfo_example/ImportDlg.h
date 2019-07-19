#ifndef __IMPORT_DLG_H_
#define __IMPORT_DLG_H_

#include  <commctrl.h>

LRESULT CALLBACK	ImportTableDlgProc(HWND, UINT, WPARAM, LPARAM);
void  InitImportListCtrl(HWND ); 
void  InitImportFunctionsListCtrl(HWND ); 
void  ShowImportDescInfo(HWND);
void  ShowImportFuncsByDllIndex(HWND,int);
#endif