/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            keyboard.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "type.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
#include "string.h"
#include "proc.h"
#include "global.h"
#include "keyboard.h"
#include "keymap.h"

PRIVATE KB_INPUT	kb_in;

/*======================================================================*
                            keyboard_handler
 *======================================================================*/
PUBLIC void keyboard_handler(int irq)
{
	u8 scan_code = in_byte(KB_DATA);

	if (kb_in.count < KB_IN_BYTES) {
		*(kb_in.p_head) = scan_code;
		kb_in.p_head++;
		if (kb_in.p_head == kb_in.buf + KB_IN_BYTES) {
			kb_in.p_head = kb_in.buf;
		}
		kb_in.count++;
	}
}


/*======================================================================*
                           init_keyboard
*======================================================================*/
PUBLIC void init_keyboard()
{
	kb_in.count = 0;
	kb_in.p_head = kb_in.p_tail = kb_in.buf;

        put_irq_handler(KEYBOARD_IRQ, keyboard_handler);/*设定键盘中断处理程序*/
        enable_irq(KEYBOARD_IRQ);                       /*开键盘中断*/
}


/*======================================================================*
                           keyboard_read
*======================================================================*/
PUBLIC void keyboard_read()
{
	u8	scan_code;
	char	output[2];
	int	make;	/* TRUE: make;  FALSE: break. */

	memset(output, 0, 2);

	if(kb_in.count > 0){
		disable_int();
		scan_code = *(kb_in.p_tail);
		kb_in.p_tail++;
		if (kb_in.p_tail == kb_in.buf + KB_IN_BYTES) {
			kb_in.p_tail = kb_in.buf;
		}
		kb_in.count--;
		enable_int();

		/* 下面开始解析扫描码 */
		if (scan_code == 0xE1) {
			/* 暂时不做任何操作 */
		}
		else if (scan_code == 0xE0) {
			/* 暂时不做任何操作 */
		}
		else {	/* 下面处理可打印字符 */

			/* 首先判断Make Code 还是 Break Code */
			make = (scan_code & FLAG_BREAK ? FALSE : TRUE);

			/* 如果是Make Code 就打印，是 Break Code 则不做处理 */
			if(make) {
				output[0] = keymap[(scan_code&0x7F)*MAP_COLS];
				disp_str(output);
			}
		}
		/* disp_int(scan_code); */
	}
}
