@ECHO OFF
ECHO --- Building aPLib Visual C/C++ example ---
ECHO.

cl /nologo /O2 /I..\..\lib\mscoff appack.c ..\..\lib\mscoff\aplib.lib
