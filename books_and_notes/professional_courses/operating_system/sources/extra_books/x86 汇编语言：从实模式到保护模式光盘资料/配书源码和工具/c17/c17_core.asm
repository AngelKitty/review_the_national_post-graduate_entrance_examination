         ;代码清单17-2
         ;文件名：c17_core.asm
         ;文件说明：保护模式微型核心程序 
         ;创建日期：2012-07-12 23:15
;-------------------------------------------------------------------------------
         ;以下定义常量
         flat_4gb_code_seg_sel  equ  0x0008      ;平坦模型下的4GB代码段选择子 
         flat_4gb_data_seg_sel  equ  0x0018      ;平坦模型下的4GB数据段选择子 
         idt_linear_address     equ  0x8001f000  ;中断描述符表的线性基地址 
;-------------------------------------------------------------------------------          
         ;以下定义宏
         %macro alloc_core_linear 0              ;在内核空间中分配虚拟内存 
               mov ebx,[core_tcb+0x06]
               add dword [core_tcb+0x06],0x1000
               call flat_4gb_code_seg_sel:alloc_inst_a_page
         %endmacro 
;-------------------------------------------------------------------------------
         %macro alloc_user_linear 0              ;在任务空间中分配虚拟内存 
               mov ebx,[esi+0x06]
               add dword [esi+0x06],0x1000
               call flat_4gb_code_seg_sel:alloc_inst_a_page
         %endmacro
         
;===============================================================================
SECTION  core  vstart=0x80040000

         ;以下是系统核心的头部，用于加载核心程序 
         core_length      dd core_end       ;核心程序总长度#00

         core_entry       dd start          ;核心代码段入口点#04

;-------------------------------------------------------------------------------
         [bits 32]
;-------------------------------------------------------------------------------
         ;字符串显示例程（适用于平坦内存模型） 
put_string:                                 ;显示0终止的字符串并移动光标 
                                            ;输入：EBX=字符串的线性地址

         push ebx
         push ecx

         cli                                ;硬件操作期间，关中断

  .getc:
         mov cl,[ebx]
         or cl,cl                           ;检测串结束标志（0） 
         jz .exit                           ;显示完毕，返回 
         call put_char
         inc ebx
         jmp .getc

  .exit:

         sti                                ;硬件操作完毕，开放中断

         pop ecx
         pop ebx

         retf                               ;段间返回

;-------------------------------------------------------------------------------
put_char:                                   ;在当前光标处显示一个字符,并推进
                                            ;光标。仅用于段内调用 
                                            ;输入：CL=字符ASCII码 
         pushad

         ;以下取当前光标位置
         mov dx,0x3d4
         mov al,0x0e
         out dx,al
         inc dx                             ;0x3d5
         in al,dx                           ;高字
         mov ah,al

         dec dx                             ;0x3d4
         mov al,0x0f
         out dx,al
         inc dx                             ;0x3d5
         in al,dx                           ;低字
         mov bx,ax                          ;BX=代表光标位置的16位数
         and ebx,0x0000ffff                 ;准备使用32位寻址方式访问显存 
         
         cmp cl,0x0d                        ;回车符？
         jnz .put_0a                         
         
         mov ax,bx                          ;以下按回车符处理 
         mov bl,80
         div bl
         mul bl
         mov bx,ax
         jmp .set_cursor

  .put_0a:
         cmp cl,0x0a                        ;换行符？
         jnz .put_other
         add bx,80                          ;增加一行 
         jmp .roll_screen

  .put_other:                               ;正常显示字符
         shl bx,1
         mov [0x800b8000+ebx],cl            ;在光标位置处显示字符 

         ;以下将光标位置推进一个字符
         shr bx,1
         inc bx

  .roll_screen:
         cmp bx,2000                        ;光标超出屏幕？滚屏
         jl .set_cursor

         cld
         mov esi,0x800b80a0                 ;小心！32位模式下movsb/w/d 
         mov edi,0x800b8000                 ;使用的是esi/edi/ecx 
         mov ecx,1920
         rep movsd
         mov bx,3840                        ;清除屏幕最底一行
         mov ecx,80                         ;32位程序应该使用ECX
  .cls:
         mov word [0x800b8000+ebx],0x0720
         add bx,2
         loop .cls

         mov bx,1920

  .set_cursor:
         mov dx,0x3d4
         mov al,0x0e
         out dx,al
         inc dx                             ;0x3d5
         mov al,bh
         out dx,al
         dec dx                             ;0x3d4
         mov al,0x0f
         out dx,al
         inc dx                             ;0x3d5
         mov al,bl
         out dx,al
         
         popad
         
         ret                              

;-------------------------------------------------------------------------------
read_hard_disk_0:                           ;从硬盘读取一个逻辑扇区（平坦模型） 
                                            ;EAX=逻辑扇区号
                                            ;EBX=目标缓冲区线性地址
                                            ;返回：EBX=EBX+512
         cli
         
         push eax 
         push ecx
         push edx
      
         push eax
         
         mov dx,0x1f2
         mov al,1
         out dx,al                          ;读取的扇区数

         inc dx                             ;0x1f3
         pop eax
         out dx,al                          ;LBA地址7~0

         inc dx                             ;0x1f4
         mov cl,8
         shr eax,cl
         out dx,al                          ;LBA地址15~8

         inc dx                             ;0x1f5
         shr eax,cl
         out dx,al                          ;LBA地址23~16

         inc dx                             ;0x1f6
         shr eax,cl
         or al,0xe0                         ;第一硬盘  LBA地址27~24
         out dx,al

         inc dx                             ;0x1f7
         mov al,0x20                        ;读命令
         out dx,al

  .waits:
         in al,dx
         and al,0x88
         cmp al,0x08
         jnz .waits                         ;不忙，且硬盘已准备好数据传输 

         mov ecx,256                        ;总共要读取的字数
         mov dx,0x1f0
  .readw:
         in ax,dx
         mov [ebx],ax
         add ebx,2
         loop .readw

         pop edx
         pop ecx
         pop eax
      
         sti
      
         retf                               ;远返回 

;-------------------------------------------------------------------------------
;汇编语言程序是极难一次成功，而且调试非常困难。这个例程可以提供帮助 
put_hex_dword:                              ;在当前光标处以十六进制形式显示
                                            ;一个双字并推进光标 
                                            ;输入：EDX=要转换并显示的数字
                                            ;输出：无
         pushad

         mov ebx,bin_hex                    ;指向核心地址空间内的转换表
         mov ecx,8
  .xlt:    
         rol edx,4
         mov eax,edx
         and eax,0x0000000f
         xlat
      
         push ecx
         mov cl,al                           
         call put_char
         pop ecx
       
         loop .xlt
      
         popad
         retf
      
;-------------------------------------------------------------------------------
set_up_gdt_descriptor:                      ;在GDT内安装一个新的描述符
                                            ;输入：EDX:EAX=描述符 
                                            ;输出：CX=描述符的选择子
         push eax
         push ebx
         push edx

         sgdt [pgdt]                        ;取得GDTR的界限和线性地址 

         movzx ebx,word [pgdt]              ;GDT界限
         inc bx                             ;GDT总字节数，也是下一个描述符偏移
         add ebx,[pgdt+2]                   ;下一个描述符的线性地址

         mov [ebx],eax
         mov [ebx+4],edx

         add word [pgdt],8                  ;增加一个描述符的大小

         lgdt [pgdt]                        ;对GDT的更改生效

         mov ax,[pgdt]                      ;得到GDT界限值
         xor dx,dx
         mov bx,8
         div bx                             ;除以8，去掉余数
         mov cx,ax
         shl cx,3                           ;将索引号移到正确位置

         pop edx
         pop ebx
         pop eax

         retf
;-------------------------------------------------------------------------------
make_seg_descriptor:                        ;构造存储器和系统的段描述符
                                            ;输入：EAX=线性基地址
                                            ;      EBX=段界限
                                            ;      ECX=属性。各属性位都在原始
                                            ;          位置，无关的位清零 
                                            ;返回：EDX:EAX=描述符
         mov edx,eax
         shl eax,16
         or ax,bx                           ;描述符前32位(EAX)构造完毕

         and edx,0xffff0000                 ;清除基地址中无关的位
         rol edx,8
         bswap edx                          ;装配基址的31~24和23~16  (80486+)

         xor bx,bx
         or edx,ebx                         ;装配段界限的高4位

         or edx,ecx                         ;装配属性

         retf

;-------------------------------------------------------------------------------
make_gate_descriptor:                       ;构造门的描述符（调用门等）
                                            ;输入：EAX=门代码在段内偏移地址
                                            ;       BX=门代码所在段的选择子 
                                            ;       CX=段类型及属性等（各属
                                            ;          性位都在原始位置）
                                            ;返回：EDX:EAX=完整的描述符
         push ebx
         push ecx
      
         mov edx,eax
         and edx,0xffff0000                 ;得到偏移地址高16位 
         or dx,cx                           ;组装属性部分到EDX
       
         and eax,0x0000ffff                 ;得到偏移地址低16位 
         shl ebx,16                          
         or eax,ebx                         ;组装段选择子部分
      
         pop ecx
         pop ebx
      
         retf                                   
                             
;-------------------------------------------------------------------------------
allocate_a_4k_page:                         ;分配一个4KB的页
                                            ;输入：无
                                            ;输出：EAX=页的物理地址
         push ebx
         push ecx
         push edx

         xor eax,eax
  .b1:
         bts [page_bit_map],eax
         jnc .b2
         inc eax
         cmp eax,page_map_len*8
         jl .b1
         
         mov ebx,message_3
         call flat_4gb_code_seg_sel:put_string
         hlt                                ;没有可以分配的页，停机 
         
  .b2:
         shl eax,12                         ;乘以4096（0x1000） 
         
         pop edx
         pop ecx
         pop ebx
         
         ret
         
;-------------------------------------------------------------------------------
alloc_inst_a_page:                          ;分配一个页，并安装在当前活动的
                                            ;层级分页结构中
                                            ;输入：EBX=页的线性地址
         push eax
         push ebx
         push esi
         
         ;检查该线性地址所对应的页表是否存在
         mov esi,ebx
         and esi,0xffc00000
         shr esi,20                         ;得到页目录索引，并乘以4 
         or esi,0xfffff000                  ;页目录自身的线性地址+表内偏移 

         test dword [esi],0x00000001        ;P位是否为“1”。检查该线性地址是 
         jnz .b1                            ;否已经有对应的页表
          
         ;创建该线性地址所对应的页表 
         call allocate_a_4k_page            ;分配一个页做为页表 
         or eax,0x00000007
         mov [esi],eax                      ;在页目录中登记该页表
          
  .b1:
         ;开始访问该线性地址所对应的页表 
         mov esi,ebx
         shr esi,10
         and esi,0x003ff000                 ;或者0xfffff000，因高10位是零 
         or esi,0xffc00000                  ;得到该页表的线性地址
         
         ;得到该线性地址在页表内的对应条目（页表项） 
         and ebx,0x003ff000
         shr ebx,10                         ;相当于右移12位，再乘以4
         or esi,ebx                         ;页表项的线性地址 
         call allocate_a_4k_page            ;分配一个页，这才是要安装的页
         or eax,0x00000007
         mov [esi],eax 
          
         pop esi
         pop ebx
         pop eax
         
         retf  

;-------------------------------------------------------------------------------
create_copy_cur_pdir:                       ;创建新页目录，并复制当前页目录内容
                                            ;输入：无
                                            ;输出：EAX=新页目录的物理地址 
         push esi
         push edi
         push ebx
         push ecx
         
         call allocate_a_4k_page            
         mov ebx,eax
         or ebx,0x00000007
         mov [0xfffffff8],ebx

         invlpg [0xfffffff8]

         mov esi,0xfffff000                 ;ESI->当前页目录的线性地址
         mov edi,0xffffe000                 ;EDI->新页目录的线性地址
         mov ecx,1024                       ;ECX=要复制的目录项数
         cld
         repe movsd 
         
         pop ecx
         pop ebx
         pop edi
         pop esi
         
         retf
         
;-------------------------------------------------------------------------------
general_interrupt_handler:                  ;通用的中断处理过程
         push eax
          
         mov al,0x20                        ;中断结束命令EOI 
         out 0xa0,al                        ;向从片发送 
         out 0x20,al                        ;向主片发送
         
         pop eax
          
         iretd

;-------------------------------------------------------------------------------
general_exception_handler:                  ;通用的异常处理过程
         mov ebx,excep_msg
         call flat_4gb_code_seg_sel:put_string
         
         hlt

;-------------------------------------------------------------------------------
rtm_0x70_interrupt_handle:                  ;实时时钟中断处理过程

         pushad

         mov al,0x20                        ;中断结束命令EOI
         out 0xa0,al                        ;向8259A从片发送
         out 0x20,al                        ;向8259A主片发送

         mov al,0x0c                        ;寄存器C的索引。且开放NMI
         out 0x70,al
         in al,0x71                         ;读一下RTC的寄存器C，否则只发生一次中断
                                            ;此处不考虑闹钟和周期性中断的情况
         ;找当前任务（状态为忙的任务）在链表中的位置
         mov eax,tcb_chain                  
  .b0:                                      ;EAX=链表头或当前TCB线性地址
         mov ebx,[eax]                      ;EBX=下一个TCB线性地址
         or ebx,ebx
         jz .irtn                           ;链表为空，或已到末尾，从中断返回
         cmp word [ebx+0x04],0xffff         ;是忙任务（当前任务）？
         je .b1
         mov eax,ebx                        ;定位到下一个TCB（的线性地址）
         jmp .b0         

         ;将当前为忙的任务移到链尾
  .b1:
         mov ecx,[ebx]                      ;下游TCB的线性地址
         mov [eax],ecx                      ;将当前任务从链中拆除

  .b2:                                      ;此时，EBX=当前任务的线性地址
         mov edx,[eax]
         or edx,edx                         ;已到链表尾端？
         jz .b3
         mov eax,edx
         jmp .b2

  .b3:
         mov [eax],ebx                      ;将忙任务的TCB挂在链表尾端
         mov dword [ebx],0x00000000         ;将忙任务的TCB标记为链尾

         ;从链首搜索第一个空闲任务
         mov eax,tcb_chain
  .b4:
         mov eax,[eax]
         or eax,eax                         ;已到链尾（未发现空闲任务）
         jz .irtn                           ;未发现空闲任务，从中断返回
         cmp word [eax+0x04],0x0000         ;是空闲任务？
         jnz .b4

         ;将空闲任务和当前任务的状态都取反
         not word [eax+0x04]                ;设置空闲任务的状态为忙
         not word [ebx+0x04]                ;设置当前任务（忙）的状态为空闲
         jmp far [eax+0x14]                 ;任务转换

  .irtn:
         popad

         iretd

;-------------------------------------------------------------------------------
terminate_current_task:                     ;终止当前任务
                                            ;注意，执行此例程时，当前任务仍在
                                            ;运行中。此例程其实也是当前任务的
                                            ;一部分 
         ;找当前任务（状态为忙的任务）在链表中的位置
         mov eax,tcb_chain
  .b0:                                      ;EAX=链表头或当前TCB线性地址
         mov ebx,[eax]                      ;EBX=下一个TCB线性地址
         cmp word [ebx+0x04],0xffff         ;是忙任务（当前任务）？
         je .b1
         mov eax,ebx                        ;定位到下一个TCB（的线性地址）
         jmp .b0
         
  .b1:
         mov word [ebx+0x04],0x3333         ;修改当前任务的状态为“退出”
         
  .b2:
         hlt                                ;停机，等待程序管理器恢复运行时，
                                            ;将其回收 
         jmp .b2 

;------------------------------------------------------------------------------- 
         pgdt             dw  0             ;用于设置和修改GDT 
                          dd  0

         pidt             dw  0
                          dd  0
                          
         ;任务控制块链
         tcb_chain        dd  0 

         core_tcb   times  32  db 0         ;内核（程序管理器）的TCB

         page_bit_map     db  0xff,0xff,0xff,0xff,0xff,0xff,0x55,0x55
                          db  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
                          db  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
                          db  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
                          db  0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55
                          db  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
                          db  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
                          db  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
         page_map_len     equ $-page_bit_map
                          
         ;符号地址检索表
         salt:
         salt_1           db  '@PrintString'
                     times 256-($-salt_1) db 0
                          dd  put_string
                          dw  flat_4gb_code_seg_sel

         salt_2           db  '@ReadDiskData'
                     times 256-($-salt_2) db 0
                          dd  read_hard_disk_0
                          dw  flat_4gb_code_seg_sel

         salt_3           db  '@PrintDwordAsHexString'
                     times 256-($-salt_3) db 0
                          dd  put_hex_dword
                          dw  flat_4gb_code_seg_sel

         salt_4           db  '@TerminateProgram'
                     times 256-($-salt_4) db 0
                          dd  terminate_current_task
                          dw  flat_4gb_code_seg_sel

         salt_item_len   equ $-salt_4
         salt_items      equ ($-salt)/salt_item_len

         excep_msg        db  '********Exception encounted********',0

         message_0        db  '  Working in system core with protection '
                          db  'and paging are all enabled.System core is mapped '
                          db  'to address 0x80000000.',0x0d,0x0a,0

         message_1        db  '  System wide CALL-GATE mounted.',0x0d,0x0a,0
         
         message_3        db  '********No more pages********',0
         
         core_msg0        db  '  System core task running!',0x0d,0x0a,0
         
         bin_hex          db '0123456789ABCDEF'
                                            ;put_hex_dword子过程用的查找表 

         core_buf   times 512 db 0          ;内核用的缓冲区

         cpu_brnd0        db 0x0d,0x0a,'  ',0
         cpu_brand  times 52 db 0
         cpu_brnd1        db 0x0d,0x0a,0x0d,0x0a,0

;-------------------------------------------------------------------------------
fill_descriptor_in_ldt:                     ;在LDT内安装一个新的描述符
                                            ;输入：EDX:EAX=描述符
                                            ;          EBX=TCB基地址
                                            ;输出：CX=描述符的选择子
         push eax
         push edx
         push edi

         mov edi,[ebx+0x0c]                 ;获得LDT基地址
         
         xor ecx,ecx
         mov cx,[ebx+0x0a]                  ;获得LDT界限
         inc cx                             ;LDT的总字节数，即新描述符偏移地址
         
         mov [edi+ecx+0x00],eax
         mov [edi+ecx+0x04],edx             ;安装描述符

         add cx,8                           
         dec cx                             ;得到新的LDT界限值 

         mov [ebx+0x0a],cx                  ;更新LDT界限值到TCB

         mov ax,cx
         xor dx,dx
         mov cx,8
         div cx
         
         mov cx,ax
         shl cx,3                           ;左移3位，并且
         or cx,0000_0000_0000_0100B         ;使TI位=1，指向LDT，最后使RPL=00 

         pop edi
         pop edx
         pop eax
     
         ret
      
;-------------------------------------------------------------------------------
load_relocate_program:                      ;加载并重定位用户程序
                                            ;输入: PUSH 逻辑扇区号
                                            ;      PUSH 任务控制块基地址
                                            ;输出：无 
         pushad
      
         mov ebp,esp                        ;为访问通过堆栈传递的参数做准备
      
         ;清空当前页目录的前半部分（对应低2GB的局部地址空间） 
         mov ebx,0xfffff000
         xor esi,esi
  .b1:
         mov dword [ebx+esi*4],0x00000000
         inc esi
         cmp esi,512
         jl .b1

         mov eax,cr3
         mov cr3,eax                        ;刷新TLB 
         
         ;以下开始分配内存并加载用户程序
         mov eax,[ebp+40]                   ;从堆栈中取出用户程序起始扇区号
         mov ebx,core_buf                   ;读取程序头部数据
         call flat_4gb_code_seg_sel:read_hard_disk_0

         ;以下判断整个程序有多大
         mov eax,[core_buf]                 ;程序尺寸
         mov ebx,eax
         and ebx,0xfffff000                 ;使之4KB对齐 
         add ebx,0x1000                        
         test eax,0x00000fff                ;程序的大小正好是4KB的倍数吗? 
         cmovnz eax,ebx                     ;不是。使用凑整的结果

         mov ecx,eax
         shr ecx,12                         ;程序占用的总4KB页数 
         
         mov eax,[ebp+40]                   ;起始扇区号
         mov esi,[ebp+36]                   ;从堆栈中取得TCB的基地址
  .b2:
         alloc_user_linear                  ;宏：在用户任务地址空间上分配内存 
         
         push ecx
         mov ecx,8
  .b3:
         call flat_4gb_code_seg_sel:read_hard_disk_0               
         inc eax
         loop .b3

         pop ecx
         loop .b2

         ;在内核地址空间内创建用户任务的TSS
         alloc_core_linear                  ;宏：在内核的地址空间上分配内存
                                            ;用户任务的TSS必须在全局空间上分配 
         
         mov [esi+0x14],ebx                 ;在TCB中填写TSS的线性地址 
         mov word [esi+0x12],103            ;在TCB中填写TSS的界限值 
          
         ;在用户任务的局部地址空间内创建LDT 
         alloc_user_linear                  ;宏：在用户任务地址空间上分配内存

         mov [esi+0x0c],ebx                 ;填写LDT线性地址到TCB中 

         ;建立程序代码段描述符
         mov eax,0x00000000
         mov ebx,0x000fffff                 
         mov ecx,0x00c0f800                 ;4KB粒度的代码段描述符，特权级3
         call flat_4gb_code_seg_sel:make_seg_descriptor
         mov ebx,esi                        ;TCB的基地址
         call fill_descriptor_in_ldt
         or cx,0000_0000_0000_0011B         ;设置选择子的特权级为3
         
         mov ebx,[esi+0x14]                 ;从TCB中获取TSS的线性地址
         mov [ebx+76],cx                    ;填写TSS的CS域 

         ;建立程序数据段描述符
         mov eax,0x00000000
         mov ebx,0x000fffff                 
         mov ecx,0x00c0f200                 ;4KB粒度的数据段描述符，特权级3
         call flat_4gb_code_seg_sel:make_seg_descriptor
         mov ebx,esi                        ;TCB的基地址
         call fill_descriptor_in_ldt
         or cx,0000_0000_0000_0011B         ;设置选择子的特权级为3
         
         mov ebx,[esi+0x14]                 ;从TCB中获取TSS的线性地址
         mov [ebx+84],cx                    ;填写TSS的DS域 
         mov [ebx+72],cx                    ;填写TSS的ES域
         mov [ebx+88],cx                    ;填写TSS的FS域
         mov [ebx+92],cx                    ;填写TSS的GS域
         
         ;将数据段作为用户任务的3特权级固有堆栈 
         alloc_user_linear                  ;宏：在用户任务地址空间上分配内存
         
         mov ebx,[esi+0x14]                 ;从TCB中获取TSS的线性地址
         mov [ebx+80],cx                    ;填写TSS的SS域
         mov edx,[esi+0x06]                 ;堆栈的高端线性地址 
         mov [ebx+56],edx                   ;填写TSS的ESP域 

         ;在用户任务的局部地址空间内创建0特权级堆栈
         alloc_user_linear                  ;宏：在用户任务地址空间上分配内存

         mov eax,0x00000000
         mov ebx,0x000fffff
         mov ecx,0x00c09200                 ;4KB粒度的堆栈段描述符，特权级0
         call flat_4gb_code_seg_sel:make_seg_descriptor
         mov ebx,esi                        ;TCB的基地址
         call fill_descriptor_in_ldt
         or cx,0000_0000_0000_0000B         ;设置选择子的特权级为0

         mov ebx,[esi+0x14]                 ;从TCB中获取TSS的线性地址
         mov [ebx+8],cx                     ;填写TSS的SS0域
         mov edx,[esi+0x06]                 ;堆栈的高端线性地址
         mov [ebx+4],edx                    ;填写TSS的ESP0域 

         ;在用户任务的局部地址空间内创建1特权级堆栈
         alloc_user_linear                  ;宏：在用户任务地址空间上分配内存

         mov eax,0x00000000
         mov ebx,0x000fffff
         mov ecx,0x00c0b200                 ;4KB粒度的堆栈段描述符，特权级1
         call flat_4gb_code_seg_sel:make_seg_descriptor
         mov ebx,esi                        ;TCB的基地址
         call fill_descriptor_in_ldt
         or cx,0000_0000_0000_0001B         ;设置选择子的特权级为1

         mov ebx,[esi+0x14]                 ;从TCB中获取TSS的线性地址
         mov [ebx+16],cx                    ;填写TSS的SS1域
         mov edx,[esi+0x06]                 ;堆栈的高端线性地址
         mov [ebx+12],edx                   ;填写TSS的ESP1域 

         ;在用户任务的局部地址空间内创建2特权级堆栈
         alloc_user_linear                  ;宏：在用户任务地址空间上分配内存

         mov eax,0x00000000
         mov ebx,0x000fffff
         mov ecx,0x00c0d200                 ;4KB粒度的堆栈段描述符，特权级2
         call flat_4gb_code_seg_sel:make_seg_descriptor
         mov ebx,esi                        ;TCB的基地址
         call fill_descriptor_in_ldt
         or cx,0000_0000_0000_0010B         ;设置选择子的特权级为2

         mov ebx,[esi+0x14]                 ;从TCB中获取TSS的线性地址
         mov [ebx+24],cx                    ;填写TSS的SS2域
         mov edx,[esi+0x06]                 ;堆栈的高端线性地址
         mov [ebx+20],edx                   ;填写TSS的ESP2域 

         ;重定位U-SALT 
         cld

         mov ecx,[0x0c]                     ;U-SALT条目数 
         mov edi,[0x08]                     ;U-SALT在4GB空间内的偏移 
  .b4:
         push ecx
         push edi
      
         mov ecx,salt_items
         mov esi,salt
  .b5:
         push edi
         push esi
         push ecx

         mov ecx,64                         ;检索表中，每条目的比较次数 
         repe cmpsd                         ;每次比较4字节 
         jnz .b6
         mov eax,[esi]                      ;若匹配，则esi恰好指向其后的地址
         mov [edi-256],eax                  ;将字符串改写成偏移地址 
         mov ax,[esi+4]
         or ax,0000000000000011B            ;以用户程序自己的特权级使用调用门
                                            ;故RPL=3 
         mov [edi-252],ax                   ;回填调用门选择子 
  .b6:
      
         pop ecx
         pop esi
         add esi,salt_item_len
         pop edi                            ;从头比较 
         loop .b5
      
         pop edi
         add edi,256
         pop ecx
         loop .b4

         ;在GDT中登记LDT描述符
         mov esi,[ebp+36]                   ;从堆栈中取得TCB的基地址
         mov eax,[esi+0x0c]                 ;LDT的起始线性地址
         movzx ebx,word [esi+0x0a]          ;LDT段界限
         mov ecx,0x00408200                 ;LDT描述符，特权级0
         call flat_4gb_code_seg_sel:make_seg_descriptor
         call flat_4gb_code_seg_sel:set_up_gdt_descriptor
         mov [esi+0x10],cx                  ;登记LDT选择子到TCB中

         mov ebx,[esi+0x14]                 ;从TCB中获取TSS的线性地址
         mov [ebx+96],cx                    ;填写TSS的LDT域 

         mov word [ebx+0],0                 ;反向链=0
      
         mov dx,[esi+0x12]                  ;段长度（界限）
         mov [ebx+102],dx                   ;填写TSS的I/O位图偏移域 
      
         mov word [ebx+100],0               ;T=0
      
         mov eax,[0x04]                     ;从任务的4GB地址空间获取入口点 
         mov [ebx+32],eax                   ;填写TSS的EIP域 

         pushfd
         pop edx
         mov [ebx+36],edx                   ;填写TSS的EFLAGS域 

         ;在GDT中登记TSS描述符
         mov eax,[esi+0x14]                 ;从TCB中获取TSS的起始线性地址
         movzx ebx,word [esi+0x12]          ;段长度（界限）
         mov ecx,0x00408900                 ;TSS描述符，特权级0
         call flat_4gb_code_seg_sel:make_seg_descriptor
         call flat_4gb_code_seg_sel:set_up_gdt_descriptor
         mov [esi+0x18],cx                  ;登记TSS选择子到TCB

         ;创建用户任务的页目录
         ;注意！页的分配和使用是由页位图决定的，可以不占用线性地址空间 
         call flat_4gb_code_seg_sel:create_copy_cur_pdir
         mov ebx,[esi+0x14]                 ;从TCB中获取TSS的线性地址
         mov dword [ebx+28],eax             ;填写TSS的CR3(PDBR)域
                   
         popad
      
         ret 8                              ;丢弃调用本过程前压入的参数 
      
;-------------------------------------------------------------------------------
append_to_tcb_link:                         ;在TCB链上追加任务控制块
                                            ;输入：ECX=TCB线性基地址
         cli
         
         push eax
         push ebx

         mov eax,tcb_chain
  .b0:                                      ;EAX=链表头或当前TCB线性地址
         mov ebx,[eax]                      ;EBX=下一个TCB线性地址
         or ebx,ebx
         jz .b1                             ;链表为空，或已到末尾
         mov eax,ebx                        ;定位到下一个TCB（的线性地址）
         jmp .b0

  .b1:
         mov [eax],ecx
         mov dword [ecx],0x00000000         ;当前TCB指针域清零，以指示这是最
                                            ;后一个TCB
         pop ebx
         pop eax
         
         sti
         
         ret
         
;-------------------------------------------------------------------------------
start:
         ;创建中断描述符表IDT
         ;在此之前，禁止调用put_string过程，以及任何含有sti指令的过程。
          
         ;前20个向量是处理器异常使用的
         mov eax,general_exception_handler  ;门代码在段内偏移地址
         mov bx,flat_4gb_code_seg_sel       ;门代码所在段的选择子
         mov cx,0x8e00                      ;32位中断门，0特权级
         call flat_4gb_code_seg_sel:make_gate_descriptor

         mov ebx,idt_linear_address         ;中断描述符表的线性地址
         xor esi,esi
  .idt0:
         mov [ebx+esi*8],eax
         mov [ebx+esi*8+4],edx
         inc esi
         cmp esi,19                         ;安装前20个异常中断处理过程
         jle .idt0

         ;其余为保留或硬件使用的中断向量
         mov eax,general_interrupt_handler  ;门代码在段内偏移地址
         mov bx,flat_4gb_code_seg_sel       ;门代码所在段的选择子
         mov cx,0x8e00                      ;32位中断门，0特权级
         call flat_4gb_code_seg_sel:make_gate_descriptor

         mov ebx,idt_linear_address         ;中断描述符表的线性地址
  .idt1:
         mov [ebx+esi*8],eax
         mov [ebx+esi*8+4],edx
         inc esi
         cmp esi,255                        ;安装普通的中断处理过程
         jle .idt1

         ;设置实时时钟中断处理过程
         mov eax,rtm_0x70_interrupt_handle  ;门代码在段内偏移地址
         mov bx,flat_4gb_code_seg_sel       ;门代码所在段的选择子
         mov cx,0x8e00                      ;32位中断门，0特权级
         call flat_4gb_code_seg_sel:make_gate_descriptor

         mov ebx,idt_linear_address         ;中断描述符表的线性地址
         mov [ebx+0x70*8],eax
         mov [ebx+0x70*8+4],edx

         ;准备开放中断
         mov word [pidt],256*8-1            ;IDT的界限
         mov dword [pidt+2],idt_linear_address
         lidt [pidt]                        ;加载中断描述符表寄存器IDTR

         ;设置8259A中断控制器
         mov al,0x11
         out 0x20,al                        ;ICW1：边沿触发/级联方式
         mov al,0x20
         out 0x21,al                        ;ICW2:起始中断向量
         mov al,0x04
         out 0x21,al                        ;ICW3:从片级联到IR2
         mov al,0x01
         out 0x21,al                        ;ICW4:非总线缓冲，全嵌套，正常EOI

         mov al,0x11
         out 0xa0,al                        ;ICW1：边沿触发/级联方式
         mov al,0x70
         out 0xa1,al                        ;ICW2:起始中断向量
         mov al,0x04
         out 0xa1,al                        ;ICW3:从片级联到IR2
         mov al,0x01
         out 0xa1,al                        ;ICW4:非总线缓冲，全嵌套，正常EOI

         ;设置和时钟中断相关的硬件 
         mov al,0x0b                        ;RTC寄存器B
         or al,0x80                         ;阻断NMI
         out 0x70,al
         mov al,0x12                        ;设置寄存器B，禁止周期性中断，开放更
         out 0x71,al                        ;新结束后中断，BCD码，24小时制

         in al,0xa1                         ;读8259从片的IMR寄存器
         and al,0xfe                        ;清除bit 0(此位连接RTC)
         out 0xa1,al                        ;写回此寄存器

         mov al,0x0c
         out 0x70,al
         in al,0x71                         ;读RTC寄存器C，复位未决的中断状态

         sti                                ;开放硬件中断

         mov ebx,message_0
         call flat_4gb_code_seg_sel:put_string

         ;显示处理器品牌信息 
         mov eax,0x80000002
         cpuid
         mov [cpu_brand + 0x00],eax
         mov [cpu_brand + 0x04],ebx
         mov [cpu_brand + 0x08],ecx
         mov [cpu_brand + 0x0c],edx
      
         mov eax,0x80000003
         cpuid
         mov [cpu_brand + 0x10],eax
         mov [cpu_brand + 0x14],ebx
         mov [cpu_brand + 0x18],ecx
         mov [cpu_brand + 0x1c],edx

         mov eax,0x80000004
         cpuid
         mov [cpu_brand + 0x20],eax
         mov [cpu_brand + 0x24],ebx
         mov [cpu_brand + 0x28],ecx
         mov [cpu_brand + 0x2c],edx

         mov ebx,cpu_brnd0                  ;显示处理器品牌信息 
         call flat_4gb_code_seg_sel:put_string
         mov ebx,cpu_brand
         call flat_4gb_code_seg_sel:put_string
         mov ebx,cpu_brnd1
         call flat_4gb_code_seg_sel:put_string

         ;以下开始安装为整个系统服务的调用门。特权级之间的控制转移必须使用门
         mov edi,salt                       ;C-SALT表的起始位置 
         mov ecx,salt_items                 ;C-SALT表的条目数量 
  .b4:
         push ecx   
         mov eax,[edi+256]                  ;该条目入口点的32位偏移地址 
         mov bx,[edi+260]                   ;该条目入口点的段选择子 
         mov cx,1_11_0_1100_000_00000B      ;特权级3的调用门(3以上的特权级才
                                            ;允许访问)，0个参数(因为用寄存器
                                            ;传递参数，而没有用栈) 
         call flat_4gb_code_seg_sel:make_gate_descriptor
         call flat_4gb_code_seg_sel:set_up_gdt_descriptor
         mov [edi+260],cx                   ;将返回的门描述符选择子回填
         add edi,salt_item_len              ;指向下一个C-SALT条目 
         pop ecx
         loop .b4

         ;对门进行测试 
         mov ebx,message_1
         call far [salt_1+256]              ;通过门显示信息(偏移量将被忽略) 

         ;初始化创建程序管理器任务的任务控制块TCB
         mov word [core_tcb+0x04],0xffff    ;任务状态：忙碌
         mov dword [core_tcb+0x06],0x80100000    
                                            ;内核虚拟空间的分配从这里开始。
         mov word [core_tcb+0x0a],0xffff    ;登记LDT初始的界限到TCB中（未使用）
         mov ecx,core_tcb
         call append_to_tcb_link            ;将此TCB添加到TCB链中

         ;为程序管理器的TSS分配内存空间
         alloc_core_linear                  ;宏：在内核的虚拟地址空间分配内存

         ;在程序管理器的TSS中设置必要的项目 
         mov word [ebx+0],0                 ;反向链=0
         mov eax,cr3
         mov dword [ebx+28],eax             ;登记CR3(PDBR)
         mov word [ebx+96],0                ;没有LDT。处理器允许没有LDT的任务。
         mov word [ebx+100],0               ;T=0
         mov word [ebx+102],103             ;没有I/O位图。0特权级事实上不需要。
         
         ;创建程序管理器的TSS描述符，并安装到GDT中 
         mov eax,ebx                        ;TSS的起始线性地址
         mov ebx,103                        ;段长度（界限）
         mov ecx,0x00408900                 ;TSS描述符，特权级0
         call flat_4gb_code_seg_sel:make_seg_descriptor
         call flat_4gb_code_seg_sel:set_up_gdt_descriptor
         mov [core_tcb+0x18],cx             ;登记内核任务的TSS选择子到其TCB

         ;任务寄存器TR中的内容是任务存在的标志，该内容也决定了当前任务是谁。
         ;下面的指令为当前正在执行的0特权级任务“程序管理器”后补手续（TSS）。
         ltr cx

         ;现在可认为“程序管理器”任务正执行中

         ;创建用户任务的任务控制块 
         alloc_core_linear                  ;宏：在内核的虚拟地址空间分配内存
         
         mov word [ebx+0x04],0              ;任务状态：空闲 
         mov dword [ebx+0x06],0             ;用户任务局部空间的分配从0开始。
         mov word [ebx+0x0a],0xffff         ;登记LDT初始的界限到TCB中
      
         push dword 50                      ;用户程序位于逻辑50扇区
         push ebx                           ;压入任务控制块起始线性地址 
         call load_relocate_program
         mov ecx,ebx         
         call append_to_tcb_link            ;将此TCB添加到TCB链中

         ;创建用户任务的任务控制块
         alloc_core_linear                  ;宏：在内核的虚拟地址空间分配内存

         mov word [ebx+0x04],0              ;任务状态：空闲
         mov dword [ebx+0x06],0             ;用户任务局部空间的分配从0开始。
         mov word [ebx+0x0a],0xffff         ;登记LDT初始的界限到TCB中

         push dword 100                     ;用户程序位于逻辑100扇区
         push ebx                           ;压入任务控制块起始线性地址
         call load_relocate_program
         mov ecx,ebx
         call append_to_tcb_link            ;将此TCB添加到TCB链中

  .core:
         mov ebx,core_msg0
         call flat_4gb_code_seg_sel:put_string
         
         ;这里可以编写回收已终止任务内存的代码
          
         jmp .core
            
core_code_end:

;-------------------------------------------------------------------------------
SECTION core_trail
;-------------------------------------------------------------------------------
core_end: