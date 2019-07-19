@ECHO OFF
ECHO --- Building aPLib Pelles C example ---
ECHO.

cc /Ot /I..\..\lib\mscoff appack.c ..\..\lib\mscoff\aplib.lib
