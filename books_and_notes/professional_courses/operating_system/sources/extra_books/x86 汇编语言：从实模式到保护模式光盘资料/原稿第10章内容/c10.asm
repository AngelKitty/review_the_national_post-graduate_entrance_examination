;=============================================================================== 
SECTION header vstart=0                     ;定义用户程序头部段 
    program_length  dd program_end          ;程序总长度[0x00]
    
    ;用户程序入口点
    code_entry      dw start                ;偏移地址[0x04]
                    dd section.code.start   ;段地址[0x06] 
    
    realloc_tbl_len dw (header_end-realloc_begin)/4
                                            ;段重定位表项个数[0x0a]
    
    realloc_begin:
    ;段重定位表           
    code_segment    dd section.code.start   
    data_segment    dd section.data.start  
    stack_segment   dd section.stack.start 
    
header_end:                

;===============================================================================
SECTION code align=16 vstart=0         ;定义代码段（16字节对齐）
put_string:                            ;显示字符串（0结尾） 
                                       ;输入：DS:BX=串地址
      push ax
      push bx
      push si
      
      mov ah,0x0e                      ;INT 0x10第0x0e号功能 
      mov si,bx                        ;字符串起始偏移地址 
      mov bl,0x07                      ;显示属性 
      
 .gchr:      
      mov al,[si]                      ;逐个取要显示的字符 
      or al,al                         ;如果AL内容为零，则 
      jz .rett                         ;跳转到过程返回指令 
      int 0x10                         ;BIOS字符显示功能调用 
      inc si                           ;下一个字符 
      jmp .gchr
      
 .rett:
      pop si
      pop bx
      pop ax
            
      ret

;------------------------------------------------------------------------------- 
write_dsp:
      push dx
      push ax
      
      mov dx,022ch
 .@22c:
      in al,dx                        
      and al,1000_0000b	               ;监视22c端口的第7位，直到它变为0
      jnz .@22c

      pop ax
      out dx,al
      pop dx
      
      ret

;-------------------------------------------------------------------------------
read_dsp:
      push dx
      
      mov dx,22eh
 .@22e:
      in al,dx                          
      and al,1000_0000b                ;监视22e端口的位7，直到它变成1
      jz .@22e
      mov dx,22ah
      in al,dx                         ;此时可以从22a端口读取数据
      
      pop dx
      
      ret

;-------------------------------------------------------------------------------
start:
      mov ax,[stack_segment]
      mov ss,ax                        ;修改SS时，处理器将在执行下一指
      mov sp,ss_pointer                ;令前禁止中断
      
      mov ax,[data_segment]
      mov ds,ax

      mov bx,init_msg
      call put_string
               
      ;以下复位DSP芯片
      mov dx,0x226 
      mov al,1                         ;第一步，先写“1”到复位端口 
      out dx,al
    
      xor ax,ax
  .@1:
      dec ax
      jnz .@1                          ;一个硬件要求的延时(至少3ms) 
    
      out dx,al                        ;第二步，写“0”到复位端口 

      call read_dsp
      cmp al,0xaa                      ;状态值0xaa表示初始化完成 
      jz .@4
    
      mov bx,err_msg                   ;显示出错信息 
      call put_string
      jmp .idle                        ;直接停机 
      
  .@4:
      mov bx,done_msg
      call put_string
      
      ;以下安装中断处理过程
      mov bx,intr_msg
      call put_string
      
      mov al,0x0d                      ;IR5引脚的中断号 
      mov bl,4                         ;每个中断向量占4个字节。本段等效于： 
      mul bl                           ;mov bx,0x0d
      mov bx,ax                        ;shl bx,2
      
      cli                              ;禁止在修改IVT期间发生中断 
      
      push es                          ;临时使用ES 
      xor ax,ax
      mov es,ax                        ;指向内存最低处的中断向量表 
      mov word [es:bx],dsp_interrupt
                                       ;偏移地址 
      inc bx
      inc bx
      mov word [es:bx],cs              ;当前代码段 
      pop es
      
      sti

      ;允许IRQ5
      in al,0x21                       ;8259主片的IMR 
      and al,1101_1111B                ;开放IR5 
      out 0x21,al
      
      mov bx,done_msg
      call put_string

      mov bx,dma_msg
      call put_string
                                       
      ;对DMA控制器编程，设置其工作模式、缓冲区地址和传输长度
      mov dx,0x0a	               ;DMAC1的屏蔽寄存器
      mov al,00000_1_01B	       ;关闭主DMAC的1号通道 
      out dx,al                      
		
      mov ax,ds                        ;计算缓冲区物理地址
      mov bx,16
      mul bx
      add ax,voice_data
      adc dx,0
      mov bx,dx                        ;bx:ax=缓冲区20位地址 

      xor al,al
      out 0x0c,al                      ;DMAC1高低触发器清零

      mov dx,0x02                      ;写通道1基址与当前地址寄存器
      out dx,al                        ;低8位DMA地址 
      mov al,ah
      out dx,al                        ;高8位DMA地址 

      mov dx,0x83                      ;写DMA通道 1 的页面寄存器
      mov al,bl
      out dx,al

      mov dx,0x03                      ;写通道1的基字计数与当前字计数器
      mov ax,init_msg-voice_data       ;数据块（当缓冲区用）的大小 
      dec ax                           ;DMA要求实际大小减一
      out dx,al                        ;缓冲区长度低8位
      mov al,ah
      out dx,al                        ;缓冲区长度高8位
       
      mov al,0101_1001b                ;设置DMAC1通道1工作方式：单字节传送/
      out 0x0b,al                      ;地址递增/自动预置/读传送/通道1

      mov dx,0x0a                      ;DMAC1屏蔽寄存器
      mov al,1                         ;允许通道1接受请求
      out dx,al

      mov al,0x40                      ;设置DSP采样率（播放）
      call write_dsp
      mov ax,65536-(256000000/(1*8000))
      xchg ah,al                       ;只使用结果的高8位 
      call write_dsp

      ;显示信息
      mov bx,done_msg
      call put_string

      ;编程设置DSP的DMA传输模式和数据长度，以启动音频播放
      mov al,0x48
      call write_dsp
      mov ax,init_msg-voice_data       ;数据块（当缓冲区用）的大小
      shr ax,1                         ;长度设为DMA的一半
      dec ax 
      call write_dsp                   ;写低字节 
      xchg ah,al                      
      call write_dsp                   ;写高字节
   
      ;打开喇叭输出
      mov al,0xd1
      call write_dsp

      ;启动DSP的传输的播放
      mov al,0x1c
      call write_dsp

      mov bx,play_msg
      call put_string
      
  .idle:
      hlt
      jmp .idle
       
;-------------------------------------------------------------------------------  
dsp_interrupt:                         ;中断处理过程
      push ax
      push bx
      push dx
      
      ;退出自动初始化模式
      mov al,0xda
      call write_dsp
      
      ;关闭扬声器
      mov al,0xd3
      call write_dsp
       
      mov bx,done_msg
      call put_string
      
      mov bx,okay_msg
      call put_string

      mov dx,0x22f                     ;DSP中断应答
      in al,dx

      ;发送EOI命令到中断控制器(主片)
      mov al,0x20                      ;中断结束命令EOI
      out 0x20,al                      ;发给主片 

      pop dx
      pop bx
      pop ax
      
      iret
      
;-------------------------------------------------------------------------------
SECTION data align=16 vstart=0

  voice_data   incbin "baby.wav",44
    
    init_msg       db 'Initializing sound blaster card...',0
    
    intr_msg       db 'Installing interrupt vector...',0
                   
     dma_msg       db 'Setup DMA ...',0
                   
    done_msg       db 'Done.',0x0d,0x0a,0
                   
    play_msg       db 'Voice is playing now...',0
    
    okay_msg       db 'Finished,stop.',0
    
     err_msg       db 'Sound card init failed.',0
                   
;===============================================================================
SECTION stack align=16 vstart=0
           
                 resb 256
ss_pointer:

;===============================================================================  
SECTION program_trail
program_end: