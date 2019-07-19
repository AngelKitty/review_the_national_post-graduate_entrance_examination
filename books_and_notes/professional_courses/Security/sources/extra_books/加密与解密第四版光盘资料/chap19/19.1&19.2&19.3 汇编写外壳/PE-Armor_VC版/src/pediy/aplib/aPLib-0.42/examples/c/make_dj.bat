@ECHO OFF
ECHO --- Building aPLib DJGPP example ---
ECHO.

gcc -O2 -s -DAP_UNIX_STYLE -DAP_HAS_CONIO -I..\..\lib\djgpp appack.c ..\..\lib\djgpp\aplib.a -o appack.exe
