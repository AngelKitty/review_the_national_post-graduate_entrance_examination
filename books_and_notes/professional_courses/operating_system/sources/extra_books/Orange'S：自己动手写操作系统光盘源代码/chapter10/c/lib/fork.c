/*************************************************************************//**
 *****************************************************************************
 * @file   fork.c
 * @brief  
 * @author Forrest Y. Yu
 * @date   Tue May  6 14:22:13 2008
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
 *                                fork
 *****************************************************************************/
/**
 * Create a child process, which is actually a copy of the caller.
 * 
 * @return   On success, the PID of the child process is returned in the
 *         parent's thread of execution, and a 0 is returned in the child's
 *         thread of execution.
 *           On failure, a -1 will be returned in the parent's context, no
 *         child process will be created.
 *****************************************************************************/
PUBLIC int fork()
{
	MESSAGE msg;
	msg.type = FORK;

	send_recv(BOTH, TASK_MM, &msg);
	assert(msg.type == SYSCALL_RET);
	assert(msg.RETVAL == 0);

	return msg.PID;
}
