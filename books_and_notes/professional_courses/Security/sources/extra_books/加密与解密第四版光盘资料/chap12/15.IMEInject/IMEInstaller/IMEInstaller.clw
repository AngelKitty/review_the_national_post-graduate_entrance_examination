; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CIMEInstallerDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "IMEInstaller.h"

ClassCount=4
Class1=CIMEInstallerApp
Class2=CIMEInstallerDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_IMEINSTALLER_DIALOG

[CLS:CIMEInstallerApp]
Type=0
HeaderFile=IMEInstaller.h
ImplementationFile=IMEInstaller.cpp
Filter=N

[CLS:CIMEInstallerDlg]
Type=0
HeaderFile=IMEInstallerDlg.h
ImplementationFile=IMEInstallerDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=IMEInstallerDlg.h
ImplementationFile=IMEInstallerDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_IMEINSTALLER_DIALOG]
Type=1
Class=CIMEInstallerDlg
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_BUTTON_INSTALL,button,1342242816
Control4=IDC_BUTTON_UNINSTALL,button,1342242816

