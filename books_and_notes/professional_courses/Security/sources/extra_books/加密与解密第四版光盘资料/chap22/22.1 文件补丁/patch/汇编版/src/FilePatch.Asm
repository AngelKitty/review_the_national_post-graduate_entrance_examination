;*******************************************************
;第22章  补丁技术
;《加密与解密（第四版）》
;(c)  看雪学院 www.kanxue.com 2000-2018
;********************************************************
    .486
    .model flat, stdcall
    option casemap :none     

; #########################################################################

    include c:\masm32\include\windows.inc
    include c:\masm32\include\user32.inc
    include c:\masm32\include\kernel32.inc
    include c:\masm32\include\comdlg32.inc

    includelib c:\masm32\lib\user32.lib
    includelib c:\masm32\lib\kernel32.lib
    includelib c:\masm32\lib\comdlg32.lib



; #########################################################################

    .data
                   
szMsgTitle      db '文件补丁示例',0
szFileName      DB                      512 DUP (?)
ofName          OPENFILENAME            <>


lFileSize       dd 40960                ;文件大小
lFileOffset     dd 11E3h                ;需要修改的文件偏移地址
lChanges        dd 2h                   ;需要修补的字节数
BytesToWrite    db 06Ah,00h ;写的数据

; -------------------------------------------------------------------------

lBytesWriten    dd 0                    ;写进文件的字节数
hFile           dd 0                    ;文件句柄
szOK            db '补丁完成 :)',0
szErrCreate     db '不能打开文件',13,10
                db '检查文件路径和文件名再试一次！',13,10,0
szErrFileSize   db '文件大小不符',13,10,0
szErrFilePtr    db '不能设置文件指针',13,10,0
szErrWriteFile  db '文件不可写',13,10,0

; -------------------------------------------------------------------------
; 下面是打开对话框过滤器
szFilter        db      '*.exe', 0, '*.exe',  0, 0

; #########################################################################

    .code

start:
;---------------------------------------              
; 下面初始化ofName结构，并显示打开对话框
       MOV     ofName.lStructSize, SIZEOF OPENFILENAME
       MOV     ofName.lpstrFile, OFFSET szFileName
       MOV     ofName.nMaxFile, MAX_PATH
       MOV     ofName.Flags, OFN_FILEMUSTEXIST OR OFN_HIDEREADONLY
       MOV     ofName.lpstrFilter, OFFSET szFilter
       invoke  GetOpenFileName, ADDR ofName
       .IF     EAX == 0                  ; 如果用户在打开对话框中点击取消按钮
       INVOKE  ExitProcess, 1            ; 退出
       .ENDIF


;---------------------------------------
;打开文件

        invoke CreateFileA,offset szFileName,GENERIC_READ + GENERIC_WRITE, \
            0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0
        cmp eax,-1                      ;发现文件?
        mov ebx,offset szErrCreate
        jz error_create                  

        mov hFile,eax                   ;保存文件句柄

;----------------------------------------
; 检查文件尺寸

        invoke GetFileSize,eax,NULL
        cmp eax,lFileSize
        mov ebx,offset szErrFileSize
        jnz error                       

;----------------------------------------
; 在文件内设置指针偏移

        invoke SetFilePointer,hFile,lFileOffset,NULL,FILE_BEGIN
        cmp eax,-1
        mov ebx,offset szErrFilePtr
        jz error                       

;----------------------------------------
; 写数据到文件

        invoke WriteFile,hFile,offset BytesToWrite,lChanges, \
            offset lBytesWriten,NULL
        test eax,eax
        mov ebx,offset szErrWriteFile
        jz error                        

        mov ebx,offset szOK

error:
        invoke CloseHandle,hFile        ;关闭文件

error_create:

        invoke MessageBox,NULL,ebx,offset szMsgTitle,MB_OK+MB_ICONINFORMATION

        invoke ExitProcess,NULL             ;退出

End start
