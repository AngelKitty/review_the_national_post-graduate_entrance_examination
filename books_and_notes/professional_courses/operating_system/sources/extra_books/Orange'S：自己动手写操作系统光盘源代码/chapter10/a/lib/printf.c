
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                              printf.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "type.h"
#include "stdio.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "fs.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "keyboard.h"
#include "proto.h"

/******************************************************************************************
                        可变参数函数调用原理（其中涉及的数字皆为举例）
===========================================================================================

i = 0x23;
j = 0x78;
char fmt[] = "%x%d";
printf(fmt, i, j);

        push    j
        push    i
        push    fmt
        call    printf
        add     esp, 3 * 4


                ┃        HIGH        ┃                        ┃        HIGH        ┃
                ┃        ...         ┃                        ┃        ...         ┃
                ┣━━━━━━━━━━┫                        ┣━━━━━━━━━━┫
                ┃                    ┃                 0x32010┃        '\0'        ┃
                ┣━━━━━━━━━━┫                        ┣━━━━━━━━━━┫
         0x3046C┃        0x78        ┃                 0x3200c┃         d          ┃
                ┣━━━━━━━━━━┫                        ┣━━━━━━━━━━┫
   arg = 0x30468┃        0x23        ┃                 0x32008┃         %          ┃
                ┣━━━━━━━━━━┫                        ┣━━━━━━━━━━┫
         0x30464┃      0x32000 ───╂────┐       0x32004┃         x          ┃
                ┣━━━━━━━━━━┫        │              ┣━━━━━━━━━━┫
                ┃                    ┃        └──→ 0x32000┃         %          ┃
                ┣━━━━━━━━━━┫                        ┣━━━━━━━━━━┫
                ┃        ...         ┃                        ┃        ...         ┃
                ┃        LOW         ┃                        ┃        LOW         ┃

实际上，调用 vsprintf 的情形是这样的：

        vsprintf(buf, 0x32000, 0x30468);

******************************************************************************************/

/*****************************************************************************
 *                                printf
 *****************************************************************************/
/**
 * The most famous one.
 *
 * @note do not call me in any TASK, call me in USER PROC.
 * 
 * @param fmt  The format string
 * 
 * @return  The number of chars printed.
 *****************************************************************************/
PUBLIC int printf(const char *fmt, ...)
{
	int i;
	char buf[STR_DEFAULT_LEN];

	va_list arg = (va_list)((char*)(&fmt) + 4);        /* 4 是参数 fmt 所占堆栈中的大小 */
	i = vsprintf(buf, fmt, arg);
	int c = write(1, buf, i);

	assert(c == i);

	return i;
}

/*****************************************************************************
 *                                printl
 *****************************************************************************/
/**
 * low level print
 * 
 * @param fmt  The format string
 * 
 * @return  The number of chars printed.
 *****************************************************************************/
PUBLIC int printl(const char *fmt, ...)
{
	int i;
	char buf[STR_DEFAULT_LEN];

	va_list arg = (va_list)((char*)(&fmt) + 4); /**
						     * 4: size of `fmt' in
						     *    the stack
						     */
	i = vsprintf(buf, fmt, arg);
	printx(buf);

	return i;
}

