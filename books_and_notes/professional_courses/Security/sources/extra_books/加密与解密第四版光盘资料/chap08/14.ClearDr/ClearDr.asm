;******************************************
;coded by Hume,2K+
;《加密与解密（第四版）》
;(c)  看雪学院 www.kanxue.com 2000-2018
;******************************************
;例子1.演示在SEH回调函数中清空Dr寄存器以达到反调试的目的
;******************************************
include ..\asminc.h
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	.DATA
Text           db "SEH程序没有运行!",0
TextSEH        db "Hello,SEH!",0
Caption        db "SEH",0
	.DATA?
	
.code
_start:
assume fs:nothing
;------------------------------------------------
; 在栈中构造一个 EXCEPTION_REGISTRATION 结构
        push	offset _except_handler
        push	fs:[0]       
        mov	fs:[0],esp  
;---------------------------------------------------
; 引发异常的指令          
	    mov	esi,0
	    mov	eax,[esi]		; 读0地址的内存异常
 WouldBeOmit:                                  
   	    invoke  MessageBox,0,addr Text,addr Caption,MB_OK  ; 这一句永远无法被执行
;---------------------------------------------------
; 异常处理完毕后，从这里开始执行
ExecuteHere:                           
   	    invoke   MessageBox,0,addr TextSEH,addr Caption,MB_OK
;--------------------------------------------------
; 恢复原来的 SEH 链
    	    pop     fs:[0]                      
    	    add     esp,4 
    	    invoke  ExitProcess,NULL        
;-------------------------------------------------
; 异常回调处理函数
_except_handler proc uses ebx pExcept:DWORD,pFrame:DWORD,pContext:DWORD,pDispatch:DWORD
        mov  	  eax,pContext 
        Assume  eax:ptr CONTEXT
        lea      ebx, ExecuteHere  	; 异常后准备从ExecuteHere后开始执行
        mov     [eax].regEip,ebx  	; 修改CONTEXT.EIP，准备改变代码运行路线
        xor      ebx,ebx
        mov     [eax].iDr0,ebx    	; 对Drx调试寄存器清零，使断点失效（反跟踪）
        mov     [eax].iDr1,ebx
        mov     [eax].iDr2,ebx
        mov     [eax].iDr3,ebx
        mov     [eax].iDr7,341     
        mov     eax,0            	; 返回值=ExceptionContinueExecution，表示已经修复
        ret                     
_except_handler endp
end _start
