/*************************************************************************//**
 *****************************************************************************
 * @file   forkexit.c
 * @brief  
 * @author Forrest Y. Yu
 * @date   Tue May  6 00:37:15 2008
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
#include "keyboard.h"
#include "proto.h"


PRIVATE void cleanup(struct proc * proc);

/*****************************************************************************
 *                                do_fork
 *****************************************************************************/
/**
 * Perform the fork() syscall.
 * 
 * @return  Zero if success, otherwise -1.
 *****************************************************************************/
PUBLIC int do_fork()
{
	/* find a free slot in proc_table */
	struct proc* p = proc_table;
	int i;
	for (i = 0; i < NR_TASKS + NR_PROCS; i++,p++)
		if (p->p_flags == FREE_SLOT)
			break;

	int child_pid = i;
	assert(p == &proc_table[child_pid]);
	assert(child_pid >= NR_TASKS + NR_NATIVE_PROCS);
	if (i == NR_TASKS + NR_PROCS) /* no free slot */
		return -1;
	assert(i < NR_TASKS + NR_PROCS);

	/* duplicate the process table */
	int pid = mm_msg.source;
	u16 child_ldt_sel = p->ldt_sel;
	*p = proc_table[pid];
	p->ldt_sel = child_ldt_sel;
	p->p_parent = pid;
	sprintf(p->name, "%s_%d", proc_table[pid].name, child_pid);

	/* duplicate the process: T, D & S */
	struct descriptor * ppd;

	/* Text segment */
	ppd = &proc_table[pid].ldts[INDEX_LDT_C];
	/* base of T-seg, in bytes */
	int caller_T_base  = reassembly(ppd->base_high, 24,
					ppd->base_mid,  16,
					ppd->base_low);
	/* limit of T-seg, in 1 or 4096 bytes,
	   depending on the G bit of descriptor */
	int caller_T_limit = reassembly(0, 0,
					(ppd->limit_high_attr2 & 0xF), 16,
					ppd->limit_low);
	/* size of T-seg, in bytes */
	int caller_T_size  = ((caller_T_limit + 1) *
			      ((ppd->limit_high_attr2 & (DA_LIMIT_4K >> 8)) ?
			       4096 : 1));

	/* Data & Stack segments */
	ppd = &proc_table[pid].ldts[INDEX_LDT_RW];
	/* base of D&S-seg, in bytes */
	int caller_D_S_base  = reassembly(ppd->base_high, 24,
					  ppd->base_mid,  16,
					  ppd->base_low);
	/* limit of D&S-seg, in 1 or 4096 bytes,
	   depending on the G bit of descriptor */
	int caller_D_S_limit = reassembly((ppd->limit_high_attr2 & 0xF), 16,
					  0, 0,
					  ppd->limit_low);
	/* size of D&S-seg, in bytes */
	int caller_D_S_size  = ((caller_T_limit + 1) *
				((ppd->limit_high_attr2 & (DA_LIMIT_4K >> 8)) ?
				 4096 : 1));

	/* we don't separate T, D & S segments, so we have: */
	assert((caller_T_base  == caller_D_S_base ) &&
	       (caller_T_limit == caller_D_S_limit) &&
	       (caller_T_size  == caller_D_S_size ));

	/* base of child proc, T, D & S segments share the same space,
	   so we allocate memory just once */
	int child_base = alloc_mem(child_pid, caller_T_size);

	/* child is a copy of the parent */
	phys_copy((void*)child_base, (void*)caller_T_base, caller_T_size);

	/* child's LDT */
	init_desc(&p->ldts[INDEX_LDT_C],
		  child_base,
		  (PROC_IMAGE_SIZE_DEFAULT - 1) >> LIMIT_4K_SHIFT,
		  DA_LIMIT_4K | DA_32 | DA_C | PRIVILEGE_USER << 5);
	init_desc(&p->ldts[INDEX_LDT_RW],
		  child_base,
		  (PROC_IMAGE_SIZE_DEFAULT - 1) >> LIMIT_4K_SHIFT,
		  DA_LIMIT_4K | DA_32 | DA_DRW | PRIVILEGE_USER << 5);

	/* tell FS, see fs_fork() */
	MESSAGE msg2fs;
	msg2fs.type = FORK;
	msg2fs.PID = child_pid;
	send_recv(BOTH, TASK_FS, &msg2fs);

	/* child PID will be returned to the parent proc */
	mm_msg.PID = child_pid;

	/* birth of the child */
	MESSAGE m;
	m.type = SYSCALL_RET;
	m.RETVAL = 0;
	m.PID = 0;
	send_recv(SEND, child_pid, &m);

	return 0;
}

/*****************************************************************************
 *                                do_exit
 *****************************************************************************/
/**
 * Perform the exit() syscall.
 *
 * If proc A calls exit(), then MM will do the following in this routine:
 *     <1> inform FS so that the fd-related things will be cleaned up
 *     <2> free A's memory
 *     <3> set A.exit_status, which is for the parent
 *     <4> depends on parent's status. if parent (say P) is:
 *           (1) WAITING
 *                 - clean P's WAITING bit, and
 *                 - send P a message to unblock it
 *                 - release A's proc_table[] slot
 *           (2) not WAITING
 *                 - set A's HANGING bit
 *     <5> iterate proc_table[], if proc B is found as A's child, then:
 *           (1) make INIT the new parent of B, and
 *           (2) if INIT is WAITING and B is HANGING, then:
 *                 - clean INIT's WAITING bit, and
 *                 - send INIT a message to unblock it
 *                 - release B's proc_table[] slot
 *               else
 *                 if INIT is WAITING but B is not HANGING, then
 *                     - B will call exit()
 *                 if B is HANGING but INIT is not WAITING, then
 *                     - INIT will call wait()
 *
 * TERMs:
 *     - HANGING: everything except the proc_table entry has been cleaned up.
 *     - WAITING: a proc has at least one child, and it is waiting for the
 *                child(ren) to exit()
 *     - zombie: say P has a child A, A will become a zombie if
 *         - A exit(), and
 *         - P does not wait(), neither does it exit(). that is to say, P just
 *           keeps running without terminating itself or its child
 * 
 * @param status  Exiting status for parent.
 * 
 *****************************************************************************/
PUBLIC void do_exit(int status)
{
	int i;
	int pid = mm_msg.source; /* PID of caller */
	int parent_pid = proc_table[pid].p_parent;
	struct proc * p = &proc_table[pid];

	/* tell FS, see fs_exit() */
	MESSAGE msg2fs;
	msg2fs.type = EXIT;
	msg2fs.PID = pid;
	send_recv(BOTH, TASK_FS, &msg2fs);

	free_mem(pid);

	p->exit_status = status;

	if (proc_table[parent_pid].p_flags & WAITING) { /* parent is waiting */
		proc_table[parent_pid].p_flags &= ~WAITING;
		cleanup(&proc_table[pid]);
	}
	else { /* parent is not waiting */
		proc_table[pid].p_flags |= HANGING;
	}

	/* if the proc has any child, make INIT the new parent */
	for (i = 0; i < NR_TASKS + NR_PROCS; i++) {
		if (proc_table[i].p_parent == pid) { /* is a child */
			proc_table[i].p_parent = INIT;
			if ((proc_table[INIT].p_flags & WAITING) &&
			    (proc_table[i].p_flags & HANGING)) {
				proc_table[INIT].p_flags &= ~WAITING;
				cleanup(&proc_table[i]);
			}
		}
	}
}

/*****************************************************************************
 *                                cleanup
 *****************************************************************************/
/**
 * Do the last jobs to clean up a proc thoroughly:
 *     - Send proc's parent a message to unblock it, and
 *     - release proc's proc_table[] entry
 * 
 * @param proc  Process to clean up.
 *****************************************************************************/
PRIVATE void cleanup(struct proc * proc)
{
	MESSAGE msg2parent;
	msg2parent.type = SYSCALL_RET;
	msg2parent.PID = proc2pid(proc);
	msg2parent.STATUS = proc->exit_status;
	send_recv(SEND, proc->p_parent, &msg2parent);

	proc->p_flags = FREE_SLOT;
}

/*****************************************************************************
 *                                do_wait
 *****************************************************************************/
/**
 * Perform the wait() syscall.
 *
 * If proc P calls wait(), then MM will do the following in this routine:
 *     <1> iterate proc_table[],
 *         if proc A is found as P's child and it is HANGING
 *           - reply to P (cleanup() will send P a messageto unblock it)
 *           - release A's proc_table[] entry
 *           - return (MM will go on with the next message loop)
 *     <2> if no child of P is HANGING
 *           - set P's WAITING bit
 *     <3> if P has no child at all
 *           - reply to P with error
 *     <4> return (MM will go on with the next message loop)
 *
 *****************************************************************************/
PUBLIC void do_wait()
{
	int pid = mm_msg.source;

	int i;
	int children = 0;
	struct proc* p_proc = proc_table;
	for (i = 0; i < NR_TASKS + NR_PROCS; i++,p_proc++) {
		if (p_proc->p_parent == pid) {
			children++;
			if (p_proc->p_flags & HANGING) {
				cleanup(p_proc);
				return;
			}
		}
	}

	if (children) {
		/* has children, but no child is HANGING */
		proc_table[pid].p_flags |= WAITING;
	}
	else {
		/* no child at all */
		MESSAGE msg;
		msg.type = SYSCALL_RET;
		msg.PID = NO_TASK;
		send_recv(SEND, pid, &msg);
	}
}
