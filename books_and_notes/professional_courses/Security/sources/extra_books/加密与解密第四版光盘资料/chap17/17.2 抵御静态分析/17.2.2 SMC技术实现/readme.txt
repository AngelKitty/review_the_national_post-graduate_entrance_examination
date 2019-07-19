解释一下 SMC.EXE 中的所使用的两种反跟踪代码的作用。
即第一段
      pushf
      or    byte ptr [esp+1],01h
      popf
      nop

第二段
      pushad
      mov    edi,offset sContext
      mov    ecx,sizeof CONTEXT
      xor    eax,eax
      cld
      rep    stosb
      invoke  GetCurrentThread
      mov    edi,eax
      mov    [sContext].ContextFlags, CONTEXT_ALL
      invoke  GetThreadContext,edi,addr sContext
      mov    [sContext].ContextFlags, CONTEXT_DEBUG_REGISTERS
      mov    [sContext].iDr7,101h ;LOCAL_EXACT_BPM_ENABLED + DR0_ENABLED
      invoke  SetThreadContext,edi,addr sContext
      popad

本意上写 SMC 这一节时希望能够把原理讲清楚，代码设计不是太复杂，对初学者而言能够更容易看懂。所以反跟踪的代码并没有在书中详细介绍其作用。
完整的代码也只放在随书光盘中。

在实际运用中通常仅仅只靠SMC来做安全保护，往往显得比较单薄，对于熟练的调试者来说很容易跳过。他的缺陷在哪儿呢？

这里介绍的SMC用作安全保护的原理是利用SMC来依次分段解密后面要运行的代码，边运行边解密。解密代码的密钥则是通过计算之前运行的代码字节与长度而得。即
  KEY = CalcKEY(INIT_KEY, PrevCode, PrevCodeLen)
  NextCode = Decrypt(NextCode, KEY)
如果之前运行的代码字节被修改，则计算的密钥错误从而无法正确解密出后面要运行的代码。

这种设计可以防范利用 INT 3 断点进行跟踪的方式，也就是说当调试者试图在当前运行的代码的某处下 INT 3 断点时，会导致代码字节被修改。这样就影响了
程序解密其后的代码的密钥，而密钥错误时解密代码得到的是一堆乱七八糟的字节，自然无法继续对其后代码的跟踪。

但是我们知道除了利用 INT 3 断点进行跟踪之外，还可以使用单步跟踪和硬件调试器断点进行跟踪。而以上的设计并不能防范这两种方式的调试跟踪。

于是针对这种设计的缺陷，需要在代码中加入相关的反跟踪代码与SMC结合，这样才能更好的防范调试者。书中主要考虑以介绍SMC的基本运用为主，就没有为此详细展开说明，这里就来多说几句。

前面加入的第一段反跟踪代码，针对的主要是单步跟踪。当程序没有被调试者跟踪时，执行到 popf 代码时会产生单步异常。而程序中之前已经通过以下代码
Block1:
      call  loc_next
      ....
loc_next:  push  fs:[0]
      mov    fs:[0],esp

建立了自己的异常处理函数。异常处理函数首地址即为 call loc_next 的下一条指令。
      ;-=-=这里是异常处理函数的起始地址=-=-
      mov    esi,[esp+4]
      assume  esi:ptr EXCEPTION_RECORD
      mov    edi,[esp+0Ch]
      assume  edi:ptr CONTEXT
      cmp    [esi].ExceptionCode,EXCEPTION_SINGLE_STEP
      jz    @F
      mov    eax,ExceptionContinueSearch
      ret
@@:      mov    eax,[edi].regEax
      xchg  eax,[edi].regEdx
      mov    [edi].regEax,eax
      xor    eax,eax
      mov    [edi].iDr0,eax
      and    [edi].iDr1,eax
      and    [edi].iDr2,eax
      and    [edi].iDr3,eax
      and    [edi].iDr6,0FFFF0FF0h
      and    [edi].iDr7,eax
      mov    [edi].ContextFlags,CONTEXT_ALL
      mov    eax,ExceptionContinueExecution
      ret

当程序产生单步异常时，异常处理函数中的处理主要是交换 EAX 和 EDX 寄存器的值，然后程序返回到异常点继续执行。
这里交换寄存器的作用是什么呢？我们来看程序中计算密钥的一段代码：

@@:      lodsd
      xor    eax,edx
      xor    eax,ecx
      ;-=-=把反跟踪与计算相结合=-=-
      pushf
      or    byte ptr [esp+1],01h
      popf
      nop
      loop  @B

以上代码中，EDX 初始值为 INIT_KEY ，ECX 为代码长度，EAX 为加密的代码字节。可以看到如果没有异常处理函数交换 EAX 和 EDX 寄存器，那么每一轮循环
时计算得到的值都保存在 EAX 中，在下一轮循环时由于 lodsd 指令，会丢失之前计算的 EAX 的值。而利用 pushf / popf 产生异常，在异常处理函数中交换
寄存器值，即功能等同于以下的代码：

@@:      lodsd
      xor    eax,edx
      xor    eax,ecx
      xchg  eax,edx
      loop  @B

这样最后在 EDX 中保存的即是计算得到的密钥值。

如果试图单步跟踪这段代码，误入陷阱的话，在这里就是单步跟踪到 popf 时，则不会再产生单步调试异常，直接运行到其后的 nop 指令处，由于没有运行异常处理函数
因而 EAX 和 EDX 寄存器值也就没有交换，最后计算的密钥也就是错误的。

如果你知道了这个原理，作为调试者时当然不会再傻到单步跟踪 pushf / popf 的地步。但同样如果明白了原理，也可以结合其他的反单步跟踪的代码与SMC方法结合来反跟踪。

现在再来介绍第二段反跟踪代码。其实这段代码也是后来才加入到整个程序之中，所以在代码开始和结束时用了 pushad / popad 对所有寄存器进行保存和恢复。你也可以注释掉
这段代码后重新编译下，来调试跟踪对比一下。

这段代码也是加在计算密钥的流程之中，如果误入陷阱，同样会影响到密钥的计算，最后导致无法跟踪到下一段利用SMC解密得到的代码。
这段代码通过使能DR0调试寄存器，使得调试者不能在跟踪时随意使用硬件寄存器断点或BPM断点，一旦设下断点，在使用 OllyDBG 调试时运行到 SetThreadContext 时可能会触发断点，从而改变了程序的流程，最终将影响程序的继续运行。