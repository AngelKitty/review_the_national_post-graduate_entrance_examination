;******************************************
;coded by Hume,2K+
;《加密与解密（第四版）》
;(c)  看雪学院 www.kanxue.com 2000-2018
;******************************************
;例子3.演示线程异常处理的嵌套
;******************************************
include ../asminc.h
;~~~~~~~~~~~~~~~~MACROs
;注册回调函数
InstSEHframe    MACRO CallbackFucAddr
        push    offset CallbackFucAddr
        push    fs:[0]
        mov     fs:[0],esp
ENDM
;卸载回调函数
UnInstSEHframe    MACRO
        pop     fs:[0]
        add     esp,4
ENDM
;用宏简化重复代码，对应于handler中判断部分
SEHhandlerProcessOrNot MACRO ExceptType,Exit2SearchAddr
        Assume esi:ptr EXCEPTION_RECORD
        Assume edi:ptr CONTEXT

        
        mov     esi,[pExcept]
        mov     edi,pContext
        test    [esi].ExceptionFlags,7
        jnz     Exit2SearchAddr
        cmp     [esi].ExceptionCode,ExceptType
        jnz     Exit2SearchAddr
        ;;below should follow the real processing codes
ENDM 
;~~~~~~~~~~~~~~~~~~~~~~~
	.DATA
szTit           db "SEH例子-Per_Thread嵌套，Hume,2k+",0
FixDivSuc       db "Fix Div0 Error Suc!",0
FixWriSuc       db "Fix Write Acess Error Suc!",0
FixInt3Suc      db "Fix Int3 BreakPoint Suc!",0
DATABUF         dd 0
;;-----------------------------------------
	.CODE
;除0错异常处理函数
Div_handler0    proc  C  uses ebx esi edi pExcept,pFrame,pContext,pDispatch
        PUSHAD
        SEHhandlerProcessOrNot  STATUS_INTEGER_DIVIDE_BY_ZERO,@ContiSearch      ;是否是整数除0错
        mov     [edi].regEcx,10                 ;修正被除数

        POPAD
        mov     eax,ExceptionContinueExecution  ;返回继续执行       
        ret
@ContiSearch:
        POPAD
        mov     eax,ExceptionContinueSearch         
        ret
Div_handler0    endp    

;读写冲突内存异常处理函数
Wri_handler1    proc  C  uses ebx esi edi pExcept,pFrame,pContext,pDispatch
        PUSHAD
        SEHhandlerProcessOrNot  STATUS_ACCESS_VIOLATION,@ContiSearch      ;是否是读写内存冲突

        mov     [edi].regEip,offset safePlace1  ;改变返回后指令的执行地址
        ;mov     [edi].regEdx,offset DATABUF    ;将写地址转换为有效值
        POPAD
        mov     eax,ExceptionContinueExecution
        ret
@ContiSearch:
        POPAD
        mov     eax,ExceptionContinueSearch 
        ret        

Wri_handler1    endp    

;断点中断异常处理函数
Int3_handler2   proc C   uses ebx esi edi pExcept,pFrame,pContext,pDispatch
        PUSHAD
        SEHhandlerProcessOrNot  STATUS_BREAKPOINT,@ContiSearch      ;是否是断点
        INC     [edi].regEip                            ;调整返回后指令的执行地址，越过断点继续执行  
                                                        ;注意在9X下INT3异常发生后指令地址为
        POPAD
        mov     eax,ExceptionContinueExecution
        ret
@ContiSearch:
        POPAD
        mov     eax,ExceptionContinueSearch 
        ret       
Int3_handler2   endp    
;mesAddr应含有指向欲显示消息的地址
MsgBox          proc   mesAddr
        invoke	MessageBox,0,mesAddr,offset szTit,MB_ICONINFORMATION
        ret
MsgBox          endp
;-----------------------------------------

_StArT:
        Assume fs:nothing
        invoke	SetErrorMode,0
        InstSEHframe    Div_handler0 
        InstSEHframe    Wri_handler1
        InstSEHframe    Int3_handler2
        
        mov     eax,100
        cdq                     ;eax=100 edx=0
        xor     ecx,ecx         ;ecx＝0
        div     ecx             ;除0异常！
        invoke	MsgBox,offset FixDivSuc       ;如果处理除0错成功

        xor     edx,edx
        mov     [edx],eax       ;向地址0处写入，发生写异常！

safePlace1:
        invoke	MsgBox,offset FixWriSuc       ;如果处理写保护内存成功

        int     3
        nop
        invoke	MsgBox,offset FixInt3Suc      ;如果处理断点int 3成功
        
       
        invoke	MessageBox,0,CTEXT("Test Illegal INSTR without Handler or Not(Y/N)?"),offset szTit,MB_YESNO
        cmp     eax,IDYES
        jnz     no_test
        db      0Fh,17h      ;为非法指令测试

        invoke	MsgBox,CTEXT("here,will Exit")
no_test:
        UnInstSEHframe          ;卸载所有的回调函数
        UnInstSEHframe
        UnInstSEHframe
	invoke ExitProcess,0
END	_StArT