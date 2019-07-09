/*************************************************************************//**
 *****************************************************************************
 * @file   lseek.c
 * @brief  seek()
 * @author Forrest Y. Yu
 * @date   Mon Apr 21 11:28:59 2008
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
 *                                lseek
 *****************************************************************************/
/**
 * Reposition r/w file offset.
 * 
 * @param fd      File descriptor.
 * @param offset  The offset according to `whence'.
 * @param whence  SEEK_SET, SEEK_CUR or SEEK_END.
 * 
 * @return  The resulting offset location as measured in bytes from the
 *          beginning of the file.
 *****************************************************************************/
PUBLIC int lseek(int fd, int offset, int whence)
{
	MESSAGE msg;
	msg.type   = LSEEK;
	msg.FD     = fd;
	msg.OFFSET = offset;
	msg.WHENCE = whence;

	send_recv(BOTH, TASK_FS, &msg);

	return msg.OFFSET;
}
