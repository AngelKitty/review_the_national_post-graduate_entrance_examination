
#if !defined(AFX_PEINFO_EXAMPLE_H__68854692_1615_42EF_822C_266E81F24982__INCLUDED_)
#define AFX_PEINFO_EXAMPLE_H__68854692_1615_42EF_822C_266E81F24982__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"



LRESULT CALLBACK	MainDlgProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

BOOL    OpenFileDlg(HWND);  
void    ShowFileHeaderInfo(HWND);
void    ShowOptionHeaderInfo(HWND);
void    EnableEditCtrl(HWND,BOOL);        

#endif // !defined(AFX_PEINFO_EXAMPLE_H__68854692_1615_42EF_822C_266E81F24982__INCLUDED_)
