/*************************************************************************//**
 *****************************************************************************
 * @file   getpid.c
 * @brief  getpid()
 * @author Forrest Y. Yu
 * @date   Mon Nov 10 19:04:07 2008
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
 *                                getpid
 *****************************************************************************/
/**
 * Get the PID.
 * 
 * @return The PID.
 *****************************************************************************/
PUBLIC int getpid()
{
	MESSAGE msg;
	msg.type	= GET_PID;

	send_recv(BOTH, TASK_SYS, &msg);
	assert(msg.type == SYSCALL_RET);

	return msg.PID;
}
