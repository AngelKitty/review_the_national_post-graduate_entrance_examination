;******************************************
;coded by Hume,2K+
;******************************************
;例子2.演示线程异常处理
;******************************************
include ../asminc.h

	.DATA
szTit   db "SEH例子-线程异常处理，Hume,2k+",0
mesSUC  db "WE SUCEED IN FIX DIV0 ERROR.",0	
	.DATA?
hInstance	dd ?
;;-----------------------------------------
	.CODE
SEHandler      proc C uses ebx esi edi pExcept,pFrame,pContext,pDispatch

        Assume  esi:ptr EXCEPTION_RECORD
        Assume  edi:ptr CONTEXT

        mov     esi,pExcept
        mov     edi,pContext
        test    [esi].ExceptionFlags,3
        jne     _continue_search
        cmp     [esi].ExceptionCode,STATUS_INTEGER_DIVIDE_BY_ZERO         ;是除0错？
        jne     _continue_search

        mov     [edi].regEcx,10                         ;将被除数改为非0值继续返回执行
                                                        ;这次可以得到正确结果是10

        mov     eax,ExceptionContinueExecution          ;修复完毕，继续执行
        ret
_continue_search:
        mov     eax,ExceptionContinueSearch             ;其他异常，无法处理，继续遍历seh回调函数列表
        ret
SEHandler      endp
_StArT:
        assume fs:nothing
      
        push    offset SEHandler
        push    fs:[0]
        mov     fs:[0],esp                      ;建立EXCEPTION_REGISTRATION_RECORD结构并将
                                                ;TIB偏移0改为该结构地址
        
        xor     ecx,ecx                         ;ECX=0
        mov     eax,100                         ;EAX=100
        xor     edx,edx                         ;EDX=0

        div     ecx                             ;产生除0错！
        invoke	MessageBox,0,addr mesSUC,addr szTit,0

        pop     fs:[0]                          ;恢复原异常回调函数
        add     esp,4                           ;平衡堆栈

	invoke ExitProcess,0
END	_StArT