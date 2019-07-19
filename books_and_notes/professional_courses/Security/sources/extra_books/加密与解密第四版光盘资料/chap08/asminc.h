.586p
.model flat,stdcall
option casemap :none

include D:\masm32\include\windows.inc
include D:\masm32\include\kernel32.inc
include D:\masm32\include\user32.inc
include D:\masm32\include\masm32.inc

includelib D:\masm32\lib\user32.lib
includelib D:\masm32\lib\kernel32.lib
includelib D:\masm32\lib\masm32.lib

CTEXT macro Text
    local szText
    .data
    szText byte Text, 0
    .code
    exitm <offset szText>   
endm