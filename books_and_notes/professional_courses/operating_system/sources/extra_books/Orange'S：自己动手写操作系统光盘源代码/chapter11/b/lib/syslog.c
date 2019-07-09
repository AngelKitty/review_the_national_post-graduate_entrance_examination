/*************************************************************************//**
 *****************************************************************************
 * @file   syslog.c
 * @brief  
 * @author Forrest Y. Yu
 * @date   Thu Nov 20 17:02:42 2008
 *****************************************************************************
 *****************************************************************************/

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
#include "proto.h"


/*****************************************************************************
 *                                syslog
 *****************************************************************************/
/**
 * Write log directly to the disk by sending message to FS.
 * 
 * @param fmt The format string.
 * 
 * @return How many chars have been printed.
 *****************************************************************************/
PUBLIC int syslog(const char *fmt, ...)
{
	int i;
	char buf[STR_DEFAULT_LEN];

	va_list arg = (va_list)((char*)(&fmt) + 4); /**
						     * 4: size of `fmt' in
						     *    the stack
						     */
	i = vsprintf(buf, fmt, arg);
	assert(strlen(buf) == i);

	return disklog(buf);
}

