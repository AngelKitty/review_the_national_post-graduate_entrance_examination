@echo off
echo ----------------------------------------------------
echo Press any key to delete all files with ending:
echo  *.aps *.idb *.ncp *.obj *.pch *.tmp *.sbr
echo Visual c++/.Net junk 
echo ----------------------------------------------------
pause

del /F /Q /S *.aps vc*.pdb *.ncp *.log *.obj *.pch *.sbr *.tmp *.bsc *.ilk *.res *.ncb *.suo *.dep *.sdf *.ipch *.tlog *.lastbuildstate *.manifest mllink*


pause


