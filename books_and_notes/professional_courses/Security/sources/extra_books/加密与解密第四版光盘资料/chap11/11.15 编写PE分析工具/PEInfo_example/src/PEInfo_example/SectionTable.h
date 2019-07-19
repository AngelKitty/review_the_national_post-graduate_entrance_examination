#ifndef __SECTIONTABLE_H_
#define __SECTIONTABLE_H_


#include <commctrl.h>

LRESULT CALLBACK	SectionTableDlgProc(HWND, UINT, WPARAM, LPARAM);
void InitSectionListCtrl(HWND); 
void ShowSectionHeaderInfo(HWND);
# endif