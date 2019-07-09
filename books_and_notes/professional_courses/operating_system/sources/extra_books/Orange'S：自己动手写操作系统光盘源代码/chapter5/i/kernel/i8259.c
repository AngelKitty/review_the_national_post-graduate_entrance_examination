
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            i8259.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


#include "type.h"
#include "const.h"
#include "protect.h"
#include "proto.h"


/*======================================================================*
                            init_8259A
 *======================================================================*/
PUBLIC void init_8259A()
{
	/* Master 8259, ICW1. */
	out_byte(INT_M_CTL,	0x11);

	/* Slave  8259, ICW1. */
	out_byte(INT_S_CTL,	0x11);

	/* Master 8259, ICW2. 设置 '主8259' 的中断入口地址为 0x20. */
	out_byte(INT_M_CTLMASK,	INT_VECTOR_IRQ0);

	/* Slave  8259, ICW2. 设置 '从8259' 的中断入口地址为 0x28 */
	out_byte(INT_S_CTLMASK,	INT_VECTOR_IRQ8);

	/* Master 8259, ICW3. IR2 对应 '从8259'. */
	out_byte(INT_M_CTLMASK,	0x4);

	/* Slave  8259, ICW3. 对应 '主8259' 的 IR2. */
	out_byte(INT_S_CTLMASK,	0x2);

	/* Master 8259, ICW4. */
	out_byte(INT_M_CTLMASK,	0x1);

	/* Slave  8259, ICW4. */
	out_byte(INT_S_CTLMASK,	0x1);

	/* Master 8259, OCW1.  */
	out_byte(INT_M_CTLMASK,	0xFD);

	/* Slave  8259, OCW1.  */
	out_byte(INT_S_CTLMASK,	0xFF);
}

/*======================================================================*
                           spurious_irq
 *======================================================================*/
PUBLIC void spurious_irq(int irq)
{
        disp_str("spurious_irq: ");
        disp_int(irq);
        disp_str("\n");
}
