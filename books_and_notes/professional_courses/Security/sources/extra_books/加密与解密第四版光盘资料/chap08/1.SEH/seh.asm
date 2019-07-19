;******************************************
;coded by Hume,2K+
;******************************************
;例子1.演示SEH的安装和卸载
;******************************************
include ..\asminc.h
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	.DATA
szTit           db "OK",0
messuc          db "SEH Succeed",0
mesfail         db "SEH Fail",0
	.DATA?
;;-----------------------------------------
	.CODE

;异常处理回调函数
myHandler proc C uses ebx esi edi pExcept,pFrame,pContext,pDispatch

      invoke	MessageBox,0,addr messuc,addr szTit,MB_APPLMODAL or MB_OK
      invoke  ExitProcess,0  
myHandler endp

;程序入口点
_Start:
assume fs:nothing
      push    ebp
      mov     ebp,esp
      push     offset myHandler
      push     fs:[0]
      mov      fs:[0],esp
      xor     esi,esi
      mov     eax, dword ptr [esi]
      invoke	MessageBox,0,addr mesfail,addr szTit,MB_APPLMODAL or MB_OK
      mov     esp,dword ptr fs:[0]
      pop     dword ptr fs:[0]
      mov     esp,ebp
      pop     ebp
      retn
END	_Start

