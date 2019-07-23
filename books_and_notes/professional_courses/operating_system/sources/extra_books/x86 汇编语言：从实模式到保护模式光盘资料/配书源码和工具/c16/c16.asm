         ;代码清单16-2
         ;文件名：c16.asm
         ;文件说明：用户程序 
         ;创建日期：2012-05-25 13:53   

         program_length   dd program_end          ;程序总长度#0x00
         entry_point      dd start                ;程序入口点#0x04
         salt_position    dd salt_begin           ;SALT表起始偏移量#0x08 
         salt_items       dd (salt_end-salt_begin)/256 ;SALT条目数#0x0C

;-------------------------------------------------------------------------------

         ;符号地址检索表
         salt_begin:                                     

         PrintString      db  '@PrintString'
                     times 256-($-PrintString) db 0
                     
         TerminateProgram db  '@TerminateProgram'
                     times 256-($-TerminateProgram) db 0
;-------------------------------------------------------------------------------

         reserved  times 256*500 db 0            ;保留一个空白区，以演示分页

;-------------------------------------------------------------------------------
         ReadDiskData     db  '@ReadDiskData'
                     times 256-($-ReadDiskData) db 0
         
         PrintDwordAsHex  db  '@PrintDwordAsHexString'
                     times 256-($-PrintDwordAsHex) db 0
         
         salt_end:

         message_0        db  0x0d,0x0a,
                          db  '  ............User task is running with '
                          db  'paging enabled!............',0x0d,0x0a,0

         space            db  0x20,0x20,0
         
;-------------------------------------------------------------------------------
      [bits 32]
;-------------------------------------------------------------------------------

start:
          
         mov ebx,message_0
         call far [PrintString]
         
         xor esi,esi
         mov ecx,88
  .b1:
         mov ebx,space
         call far [PrintString] 
         
         mov edx,[esi*4]
         call far [PrintDwordAsHex]
         
         inc esi
         loop .b1 
        
         call far [TerminateProgram]              ;退出，并将控制权返回到核心 
    
;-------------------------------------------------------------------------------
program_end: