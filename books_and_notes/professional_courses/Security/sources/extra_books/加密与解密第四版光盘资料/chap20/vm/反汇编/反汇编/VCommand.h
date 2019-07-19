//"VMEND"结束标记
#define		VM_END								__asm		push    esi \
												__asm		dec     ebp \
												__asm		inc     ebp \
												__asm		dec     esi \
												__asm		inc     esp

#define		INTCC								__asm		_emit	0xCC
// 进入虚拟机
void VStartVM();
//检测堆栈是否覆盖
void DCheckESP();

//带D的为内部Handler
//内部push pop只做32位操作,段寄存器也当寄存器处理
void DPushReg32();
void DPushImm32();
void DPushMem32();

void DPopReg32();//弹回寄存器
void DFree();//释放堆栈
