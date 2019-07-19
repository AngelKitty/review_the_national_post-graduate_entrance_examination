# Microsoft Developer Studio Project File - Name="ProcessNotifyInject" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=ProcessNotifyInject - Win32 XP Checked 64
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ProcessNotifyInject.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ProcessNotifyInject.mak" CFG="ProcessNotifyInject - Win32 XP Checked 64"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ProcessNotifyInject - Win32 XP Free" (based on "Win32 (x86) External Target")
!MESSAGE "ProcessNotifyInject - Win32 XP Checked" (based on "Win32 (x86) External Target")
!MESSAGE "ProcessNotifyInject - Win32 XP Checked 64" (based on "Win32 (x86) External Target")
!MESSAGE "ProcessNotifyInject - Win32 Free 64" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "ProcessNotifyInject - Win32 XP Free"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir ""
# PROP Cmd_Line "ddkbuild -WNETXP free ."
# PROP Rebuild_Opt "-cZ"
# PROP Target_File "ProcessNotifyInject.sys"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "ProcessNotifyInject - Win32 XP Checked"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir ""
# PROP Cmd_Line "ddkbuild -WNETXP checked ."
# PROP Rebuild_Opt "-cZ"
# PROP Target_File "ProcessNotifyInject.sys"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "ProcessNotifyInject - Win32 XP Checked 64"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ProcessNotifyInject___Win32_XP_Checked_64"
# PROP BASE Intermediate_Dir "ProcessNotifyInject___Win32_XP_Checked_64"
# PROP BASE Cmd_Line "ddkbuild -WNETXP checked ."
# PROP BASE Rebuild_Opt "-cZ"
# PROP BASE Target_File "ProcessNotifyInject.sys"
# PROP BASE Bsc_Name ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "ProcessNotifyInject___Win32_XP_Checked_64"
# PROP Intermediate_Dir "ProcessNotifyInject___Win32_XP_Checked_64"
# PROP Cmd_Line "ddkbuild -WNETAMD64 checked ."
# PROP Rebuild_Opt "-cZ"
# PROP Target_File "ProcessNotifyInject.sys"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "ProcessNotifyInject - Win32 Free 64"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ProcessNotifyInject___Win32_Free_64"
# PROP BASE Intermediate_Dir "ProcessNotifyInject___Win32_Free_64"
# PROP BASE Cmd_Line "ddkbuild -WNETXP free ."
# PROP BASE Rebuild_Opt "-cZ"
# PROP BASE Target_File "ProcessNotifyInject.sys"
# PROP BASE Bsc_Name ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ProcessNotifyInject___Win32_Free_64"
# PROP Intermediate_Dir "ProcessNotifyInject___Win32_Free_64"
# PROP Cmd_Line "ddkbuild -WNETXP free ."
# PROP Rebuild_Opt "-cZ"
# PROP Target_File "ProcessNotifyInject.sys"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "ProcessNotifyInject - Win32 XP Free"
# Name "ProcessNotifyInject - Win32 XP Checked"
# Name "ProcessNotifyInject - Win32 XP Checked 64"
# Name "ProcessNotifyInject - Win32 Free 64"

!IF  "$(CFG)" == "ProcessNotifyInject - Win32 XP Free"

!ELSEIF  "$(CFG)" == "ProcessNotifyInject - Win32 XP Checked"

!ELSEIF  "$(CFG)" == "ProcessNotifyInject - Win32 XP Checked 64"

!ELSEIF  "$(CFG)" == "ProcessNotifyInject - Win32 Free 64"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Common.c
# End Source File
# Begin Source File

SOURCE=.\ProcessNotifyInject.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ProcessNotifyInject.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\sources
# End Source File
# End Target
# End Project
