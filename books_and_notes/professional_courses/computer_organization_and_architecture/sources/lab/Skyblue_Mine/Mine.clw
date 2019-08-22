; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMineWnd
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "mine.h"
LastPage=0

ClassCount=5
Class1=CDlgCustom
Class2=CDlgHero
Class3=CDlgNewRecord
Class4=CMineApp
Class5=CMineWnd

ResourceCount=4
Resource1=IDR_MENU
Resource2=IDD_DLG_HERO
Resource3=IDD_DLG_NEWRECORD
Resource4=IDD_DLG_CUSTOM

[CLS:CDlgCustom]
Type=0
BaseClass=CDialog
HeaderFile=DlgCustom.h
ImplementationFile=DlgCustom.cpp
Filter=D
LastObject=IDCANCEL

[CLS:CDlgHero]
Type=0
BaseClass=CDialog
HeaderFile=DlgHero.h
ImplementationFile=DlgHero.cpp
LastObject=CDlgHero

[CLS:CDlgNewRecord]
Type=0
BaseClass=CDialog
HeaderFile=DlgNewRecord.h
ImplementationFile=DlgNewRecord.cpp
LastObject=CDlgNewRecord

[CLS:CMineApp]
Type=0
BaseClass=CWinApp
HeaderFile=Mine.h
ImplementationFile=Mine.cpp
LastObject=CMineApp

[CLS:CMineWnd]
Type=0
BaseClass=CWnd
HeaderFile=MineWnd.h
ImplementationFile=MineWnd.cpp
LastObject=CMineWnd
Filter=W

[DLG:IDD_DLG_CUSTOM]
Type=1
Class=CDlgCustom
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308865
Control4=IDC_STATIC,static,1342308865
Control5=IDC_STATIC,static,1342308865
Control6=IDC_HEIGHT,edit,1350631552
Control7=IDC_WIDTH,edit,1350631552
Control8=IDC_NUMBER,edit,1350631552

[DLG:IDD_DLG_HERO]
Type=1
Class=CDlgHero
ControlCount=11
Control1=IDOK,button,1342242817
Control2=IDC_RESET,button,1342242816
Control3=IDC_STATIC,static,1342308353
Control4=IDC_B_R,static,1342308353
Control5=IDC_STATIC,static,1342308353
Control6=IDC_I_R,static,1342308353
Control7=IDC_STATIC,static,1342308353
Control8=IDC_E_R,static,1342308353
Control9=IDC_B_H,static,1342308353
Control10=IDC_I_H,static,1342308353
Control11=IDC_E_H,static,1342308353

[DLG:IDD_DLG_NEWRECORD]
Type=1
Class=CDlgNewRecord
ControlCount=3
Control1=IDC_EDIT_NAME,edit,1350631552
Control2=IDOK,button,1342242817
Control3=IDC_DESCRIBE,static,1342308353

[MNU:IDR_MENU]
Type=1
Class=?
Command1=IDM_START
Command2=IDM_PRIMARY
Command3=IDM_SECOND
Command4=IDM_ADVANCE
Command5=IDM_CUSTOM
Command6=IDM_MARK
Command7=IDM_COLOR
Command8=IDM_SOUND
Command9=IDM_HERO
Command10=IDM_EXIT
Command11=IDM_HELP_USE
Command12=IDM_ABOUT
CommandCount=12

