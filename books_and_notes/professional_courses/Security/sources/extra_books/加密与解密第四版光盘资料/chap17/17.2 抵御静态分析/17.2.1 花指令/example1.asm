;*******************************************************
;第17章  软件保护技术
;《加密与解密（第四版）》
;(c)  看雪学院 www.kanxue.com 2000-2018
;********************************************************
.586P
.MODEL FLAT,STDCALL
OPTION CASEMAP:NONE

include \masm32\include\windows.inc
include \masm32\include\kernel32.inc
include \masm32\include\user32.inc
include \masm32\include\comctl32.inc

includelib \masm32\lib\kernel32.lib
includelib \masm32\lib\user32.lib
includelib \masm32\lib\comctl32.lib



.DATA

.CODE
start_:
		xor     eax,eax
		test    eax,eax
		jz		label1
		jnz		label1
                db		0E8h
label1:	xor     eax,3
	add     eax,4
        xor     eax,5
        ret

end start_