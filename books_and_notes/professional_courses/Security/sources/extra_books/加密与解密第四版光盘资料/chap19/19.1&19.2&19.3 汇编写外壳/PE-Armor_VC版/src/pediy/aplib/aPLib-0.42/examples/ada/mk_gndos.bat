@ECHO OFF
ECHO --- Building aPLib Ada example for GNAT/DOS (ez2load) ---
ECHO.

gnatmake -g -i %1 -O2 -gnatp -aOACU_DOS apacdemo -largs ..\..\lib\djgpp\aplib.a

if exist b_*.* del b_*.*
