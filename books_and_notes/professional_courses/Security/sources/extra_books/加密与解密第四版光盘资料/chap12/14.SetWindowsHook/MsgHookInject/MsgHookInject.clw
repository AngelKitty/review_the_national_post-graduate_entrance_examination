; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMsgHookInjectDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "MsgHookInject.h"

ClassCount=4
Class1=CMsgHookInjectApp
Class2=CMsgHookInjectDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_MSGHOOKINJECT_DIALOG

[CLS:CMsgHookInjectApp]
Type=0
HeaderFile=MsgHookInject.h
ImplementationFile=MsgHookInject.cpp
Filter=N

[CLS:CMsgHookInjectDlg]
Type=0
HeaderFile=MsgHookInjectDlg.h
ImplementationFile=MsgHookInjectDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=MsgHookInjectDlg.h
ImplementationFile=MsgHookInjectDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_MSGHOOKINJECT_DIALOG]
Type=1
Class=CMsgHookInjectDlg
ControlCount=3
Control1=IDCANCEL,button,1342242816
Control2=IDC_BUTTON_INSTALL,button,1342242816
Control3=IDC_BUTTON_UNINSTALL,button,1342242816

