#ifndef __EXPORT_DLG_H_
#define __EXPORT_DLG_H_

#include  <commctrl.h>

LRESULT CALLBACK	ExportTableDlgProc(HWND, UINT, WPARAM, LPARAM);
void   InitExportListCtrl(HWND); 
void   ShowExportDirInfo(HWND);
void   ShowExportFuncsInfo(HWND);

#endif