/*************************************************************************//**
 *****************************************************************************
 * @file   disklog.c
 * @brief  
 * @author Forrest Y. Yu
 * @date   Thu Nov 20 16:22:45 2008
 *****************************************************************************
 *****************************************************************************/

#include "type.h"
#include "config.h"
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
#include "hd.h"
#include "fs.h"

#define DISKLOG_RD_SECT(dev,sect_nr) rw_sector(DEV_READ, \
					       dev,			\
					       (sect_nr) * SECTOR_SIZE,	\
					       SECTOR_SIZE, /* read one sector */ \
					       getpid(),		\
					       logdiskbuf);
#define DISKLOG_WR_SECT(dev,sect_nr) rw_sector(DEV_WRITE, \
					       dev,			\
					       (sect_nr) * SECTOR_SIZE,	\
					       SECTOR_SIZE, /* write one sector */ \
					       getpid(),		\
					       logdiskbuf);


/* /\***************************************************************************** */
/*  *                                do_disklog */
/*  *****************************************************************************\/ */
/* /\** */
/*  * Perform syslog() system call . */
/*  * */
/*  * @return  */
/*  *****************************************************************************\/ */
/* PUBLIC int do_disklog() */
/* { */
/* 	char buf[STR_DEFAULT_LEN]; */

/* 	/\* get parameters from the message *\/ */
/* 	int str_len = fs_msg.CNT;	/\* length of filename *\/ */
/* 	int src = fs_msg.source;	/\* caller proc nr. *\/ */
/* 	assert(str_len < STR_DEFAULT_LEN); */
/* 	phys_copy((void*)va2la(TASK_FS, buf),    /\* to   *\/ */
/* 		  (void*)va2la(src, fs_msg.BUF), /\* from *\/ */
/* 		  str_len); */
/* 	buf[str_len] = 0;	/\* terminate the string *\/ */

/* 	return disklog(buf); */
/* } */

/*****************************************************************************
 *                                disklog
 *****************************************************************************/
/**
 * <Ring 1> Write log string directly into disk.
 * 
 * @param p  Ptr to the MESSAGE.
 *****************************************************************************/
PUBLIC int disklog(char * logstr)
{
	int device = root_inode->i_dev;
	struct super_block * sb = get_super_block(device);
	int nr_log_blk0_nr = sb->nr_sects - NR_SECTS_FOR_LOG; /* 0x9D41-0x800=0x9541 */

	static int pos = 0;
	if (!pos) { /* first time invoking this routine */

#ifdef SET_LOG_SECT_SMAP_AT_STARTUP
		/*
		 * set sector-map so that other files cannot use the log sectors
		 */

		int bits_per_sect = SECTOR_SIZE * 8; /* 4096 */

		int smap_blk0_nr = 1 + 1 + sb->nr_imap_sects; /* 3 */
		int sect_nr  = smap_blk0_nr + nr_log_blk0_nr / bits_per_sect; /* 3+9=12 */
		int byte_off = (nr_log_blk0_nr % bits_per_sect) / 8; /* 168 */
		int bit_off  = (nr_log_blk0_nr % bits_per_sect) % 8; /* 1 */
		int sect_cnt = NR_SECTS_FOR_LOG / bits_per_sect + 2; /* 1 */
		int bits_left= NR_SECTS_FOR_LOG; /* 2048 */

		int i;
		for (i = 0; i < sect_cnt; i++) {
			DISKLOG_RD_SECT(device, sect_nr + i); /* DISKLOG_RD_SECT(?, 12) */

			for (; byte_off < SECTOR_SIZE && bits_left > 0; byte_off++) {
				for (; bit_off < 8; bit_off++) { /* repeat till enough bits are set */
					assert(((logdiskbuf[byte_off] >> bit_off) & 1) == 0);
					logdiskbuf[byte_off] |= (1 << bit_off);
					if (--bits_left  == 0)
						break;
				}
				bit_off = 0;
			}
			byte_off = 0;
			bit_off = 0;

			DISKLOG_WR_SECT(device, sect_nr + i);

			if (bits_left == 0)
				break;
		}
		assert(bits_left == 0);
#endif /* SET_LOG_SECT_SMAP_AT_STARTUP */

		pos = 0x40;

#ifdef MEMSET_LOG_SECTS
		/* write padding stuff to log sectors */
		int chunk = min(MAX_IO_BYTES, LOGDISKBUF_SIZE >> SECTOR_SIZE_SHIFT);
		assert(chunk == 256);
		int sects_left = NR_SECTS_FOR_LOG;
		for (i = nr_log_blk0_nr;
		     i < nr_log_blk0_nr + NR_SECTS_FOR_LOG;
		     i += chunk) {
			memset(logdiskbuf, 0x20, chunk*SECTOR_SIZE);
			rw_sector(DEV_WRITE,
				  device,
				  i * SECTOR_SIZE,
				  chunk * SECTOR_SIZE,
				  getpid(),
				  logdiskbuf);
			sects_left -= chunk;
		}
		if (sects_left != 0)
			panic("sects_left should be 0, current: %d.", sects_left);
#endif /* MEMSET_LOG_SECTS */
	}

	char * p = logstr;
	int bytes_left = strlen(logstr);

	int sect_nr = nr_log_blk0_nr + (pos >> SECTOR_SIZE_SHIFT);

	while (bytes_left) {
		DISKLOG_RD_SECT(device, sect_nr);

		int off = pos % SECTOR_SIZE;
		int bytes = min(bytes_left, SECTOR_SIZE - off);

		memcpy(&logdiskbuf[off], p, bytes);
		off += bytes;
		bytes_left -= bytes;

		DISKLOG_WR_SECT(device, sect_nr);
		sect_nr++;
		pos += bytes;
		p += bytes;
	}

	struct time t;
	MESSAGE msg;
	msg.type = GET_RTC_TIME;
	msg.BUF= &t;
	send_recv(BOTH, TASK_SYS, &msg);

	/* write `pos' and time into the log file header */
	DISKLOG_RD_SECT(device, nr_log_blk0_nr);

	sprintf((char*)logdiskbuf, "%8d\n", pos);
	memset(logdiskbuf+9, ' ', 22);
	logdiskbuf[31] = '\n';

	sprintf((char*)logdiskbuf+32, "<%d-%02d-%02d %02d:%02d:%02d>\n",
		t.year,
		t.month,
		t.day,
		t.hour,
		t.minute,
		t.second);
	memset(logdiskbuf+32+22, ' ', 9);
	logdiskbuf[63] = '\n';

	DISKLOG_WR_SECT(device, nr_log_blk0_nr);
	memset(logdiskbuf+64, logdiskbuf[32+19], 512-64);
	DISKLOG_WR_SECT(device, nr_log_blk0_nr + NR_SECTS_FOR_LOG - 1);

	return pos;
}

/* /\***************************************************************************** */
/*  *                                inode2filename */
/*  *****************************************************************************\/ */
/* /\** */
/*  * Get filename via i-node */
/*  *  */
/*  * @param inode  */
/*  * @param filename  */
/*  *  */
/*  * @return  */
/*  *****************************************************************************\/ */
/* PRIVATE char * inode2filename(int inode, char * filename) */
/* { */
/* 	int i, j; */

/* 	struct inode * dir_inode = root_inode; */

/* 	/\** */
/* 	 * Search the dir for the file. */
/* 	 *\/ */
/* 	int dir_blk0_nr = dir_inode->i_start_sect; */
/* 	int nr_dir_blks = (dir_inode->i_size + SECTOR_SIZE - 1) / SECTOR_SIZE; */
/* 	int nr_dir_entries = */
/* 	  dir_inode->i_size / DIR_ENTRY_SIZE; /\** */
/* 					       * including unused slots */
/* 					       * (the file has been deleted */
/* 					       * but the slot is still there) */
/* 					       *\/ */
/* 	int m = 0; */
/* 	struct dir_entry * pde; */
/* 	for (i = 0; i < nr_dir_blks; i++) { */
/* 		DISKLOG_RD_SECT(dir_inode->i_dev, dir_blk0_nr + i); */
/* 		pde = (struct dir_entry *)logdiskbuf; */
/* 		for (j = 0; j < SECTOR_SIZE / DIR_ENTRY_SIZE; j++,pde++) { */
/* 			if (pde->inode_nr == inode) { */
/* 				memcpy(filename, pde->name, MAX_FILENAME_LEN); */
/* 				filename[MAX_FILENAME_LEN] = 0; */

/* 				if (filename[0] == '.') */
/* 					filename[0] = '/'; */

/* 				return filename; */
/* 			} */
/* 			if (++m > nr_dir_entries) */
/* 				break; */
/* 		} */
/* 		if (m > nr_dir_entries) /\* all entries have been iterated *\/ */
/* 			break; */
/* 	} */

/* 	/\* file not found *\/ */
/* 	return 0; */

/* } */

#define LOG_PROCS			1 /* YES */

#define LOG_FD_TABLE			1
#define LOG_INODE_TABLE			1
#define LOG_SMAP			1
#define LOG_IMAP			1
#define LOG_INODE_ARRAY			1
#define LOG_ROOT_DIR			1

#define LOG_MSG_SRC2DST			1 /* YES */
#define LOG_ARROW_PARENT_CHILD		1 /* YES */

#define LOG_ARROW_PROC_FD		1
#define LOG_ARROW_FD_INODE		1
#define LOG_ARROW_INODE_INODEARRAY	1

#if (LOG_SMAP == 1 || LOG_IMAP == 1 || LOG_INODE_ARRAY || LOG_ROOT_DIR == 1)
static char _buf[SECTOR_SIZE];
#endif



/*****************************************************************************
 *                                dump_fd_graph
 *****************************************************************************/
/**
 * Output a dot graph file.
 * 
 *****************************************************************************/
PUBLIC void dump_fd_graph(const char * fmt, ...)
{
	int i;
	int  logbufpos = 0;
	char title[STR_DEFAULT_LEN];

	va_list arg = (va_list)((char*)(&fmt) + 4); /**
						     * 4: size of `fmt' in
						     *    the stack
						     */
	i = vsprintf(title, fmt, arg);
	assert(strlen(title) == i);

	printl("dump_fd_graph: %s\n", title);

	struct proc* p_proc;

	int callerpid = getpid();

	/* assert(getpid() == TASK_MM); */

	printl("<|");

	disable_int();

	int tcks[NR_TASKS + NR_PROCS];
	int prio[NR_TASKS + NR_PROCS];
	p_proc = proc_table;
	for (i = 0; i < NR_TASKS + NR_PROCS; i++,p_proc++) {
		if (p_proc->p_flags == FREE_SLOT)
			continue;
		if ((i == TASK_TTY) ||
		    (i == TASK_SYS) ||
		    (i == TASK_HD)  ||
		    /* (i == TASK_FS)  || */
		    (i == callerpid))
			continue;

		tcks[i] = p_proc->ticks;
		prio[i] = p_proc->priority;
		p_proc->ticks = 0;
		p_proc->priority = 0;
	}

	static int graph_idx = 0;

#if (LOG_ROOT_DIR == 1)
	char filename[MAX_FILENAME_LEN+1];
#endif

	char * proc_flags[32];
	for (i = 0; i < sizeof(proc_flags) / sizeof(proc_flags[0]); i++) {
		proc_flags[i] = "__NOT_DEFINED__";
	}
	proc_flags[0]         = "RUNNING";
	proc_flags[SENDING]   = "SENDING";
	proc_flags[RECEIVING] = "RECEIVING";
	proc_flags[WAITING]   = "WAITING";
	proc_flags[HANGING]   = "HANGING";
	proc_flags[FREE_SLOT] = "FREE_SLOT";

	proc_flags[SENDING+WAITING]   = "SENDING, WAITING";
	proc_flags[RECEIVING+WAITING] = "RECEIVING, WAITING";
	proc_flags[RECEIVING+HANGING] = "RECEIVING, HANGING";

	/* int inode_list[64]; */
	/* int il_idx = 0; */

#if (LOG_PROCS == 1 || LOG_ARROW_PARENT_CHILD == 1)
	struct proc_parent_map {
		int pid;
		int ppid;
	} ppm[256];
	int ppm_idx = 0;
#endif

#if (LOG_PROCS == 1 || LOG_ARROW_PROC_FD == 1)
	struct proc_fdesc_map {
		int pid;	/* PID */
		int filp;	/* idx of proc_table[pid].filp[] */
		int desc;	/* idx of f_desc_table[] */
	} pfm[256];
	int pfm_idx = 0;
#endif

#if (LOG_FD_TABLE == 1 || LOG_ARROW_FD_INODE == 1)
	struct fdesc_inode_map {
		int desc;	/* idx of f_desc_table[] */
		int inode;	/* idx of inode_table[] */
	} fim[256];
	int fim_idx = 0;
#endif

	struct msg_src_dst {
		int src;
		int dst;
		int dir;
	} msd[256];
	int msd_idx = 0;
	printl("|_|");

	/* head */
	logbufpos += sprintf(logbuf + logbufpos,
			     "digraph filedesc%02d {\n", graph_idx++);
	logbufpos += sprintf(logbuf + logbufpos, "\tgraph [\n");
	logbufpos += sprintf(logbuf + logbufpos, "		rankdir = \"LR\"\n");
	logbufpos += sprintf(logbuf + logbufpos, "	];\n");
	logbufpos += sprintf(logbuf + logbufpos, "	node [\n");
	logbufpos += sprintf(logbuf + logbufpos, "		fontsize = \"16\"\n");
	logbufpos += sprintf(logbuf + logbufpos, "		shape = \"ellipse\"\n");
	logbufpos += sprintf(logbuf + logbufpos, "	];\n");
	logbufpos += sprintf(logbuf + logbufpos, "	edge [\n");
	logbufpos += sprintf(logbuf + logbufpos, "	];\n");

#if (LOG_PROCS == 1)
	int k;
	p_proc = proc_table;
	logbufpos += sprintf(logbuf + logbufpos, "\n\tsubgraph cluster_0 {\n");
	for (i = 0; i < NR_TASKS + NR_PROCS; i++,p_proc++) {
		/* skip unused proc_table entries */
		if (p_proc->p_flags == FREE_SLOT)
			continue;

		/* /\* skip procs which open no files *\/ */
		/* for (k = 0; k < NR_FILES; k++) { */
		/* 	if (p_proc->filp[k] != 0) */
		/* 		break; */
		/* } */
		/* if (k == NR_FILES) */
		/* 	continue; */

		if (p_proc->p_parent != NO_TASK) {
			ppm[ppm_idx].pid = i;
			ppm[ppm_idx].ppid = p_proc->p_parent;
			ppm_idx++;
		}

		if (p_proc->p_flags & RECEIVING) {
			msd[msd_idx].src = p_proc->p_recvfrom;
			msd[msd_idx].dst = proc2pid(p_proc);
			msd[msd_idx].dir = RECEIVING;
			msd_idx++;
		}
		if (p_proc->p_flags & SENDING) {
			msd[msd_idx].src = proc2pid(p_proc);
			msd[msd_idx].dst = p_proc->p_sendto;
			msd[msd_idx].dir = SENDING;
			msd_idx++;
		}

		logbufpos += sprintf(logbuf + logbufpos,
				     "\t\t\"proc%d\" [\n"
				     "\t\t\tlabel = \"<f0>%s (%d) "
				     "|<f1> p_flags:%d(%s)"
				     "|<f2> p_parent:%d%s"
				     "|<f3> eip:0x%x",
				     i,
				     p_proc->name,
				     i,
				     p_proc->p_flags,
				     proc_flags[p_proc->p_flags],
				     p_proc->p_parent,
				     p_proc->p_parent == NO_TASK ? "(NO_TASK)" : "",
				     p_proc->regs.eip);

		int fnr = 3;
		for (k = 0; k < NR_FILES; k++) {
			if (p_proc->filp[k] == 0)
				continue;

			int fdesc_tbl_idx = p_proc->filp[k] - f_desc_table;
			logbufpos += sprintf(logbuf + logbufpos, "\t|<f%d> filp[%d]: %d",
					     fnr,
					     k,
					     fdesc_tbl_idx);
			pfm[pfm_idx].pid = i;
			pfm[pfm_idx].filp = fnr;
			pfm[pfm_idx].desc = fdesc_tbl_idx;
			fnr++;
			pfm_idx++;
		}

		logbufpos += sprintf(logbuf + logbufpos, "\t\"\n");
		logbufpos += sprintf(logbuf + logbufpos, "\t\t\tshape = \"record\"\n");
		logbufpos += sprintf(logbuf + logbufpos, "\t\t];\n");
	}
	i = ANY;
	logbufpos += sprintf(logbuf + logbufpos, "\t\t\"proc%d\" [\n", i);
	logbufpos += sprintf(logbuf + logbufpos, "\t\t\tlabel = \"<f0>ANY |<f1> ");
	logbufpos += sprintf(logbuf + logbufpos, "\t\"\n");
	logbufpos += sprintf(logbuf + logbufpos, "\t\t\tshape = \"record\"\n");
	logbufpos += sprintf(logbuf + logbufpos, "\t\t];\n");

	logbufpos += sprintf(logbuf + logbufpos, "\t\tlabel = \"procs\";\n");
	logbufpos += sprintf(logbuf + logbufpos, "\t}\n");
#endif

	printl("0");

#if (LOG_FD_TABLE == 1)
	logbufpos += sprintf(logbuf + logbufpos, "\n\tsubgraph cluster_1 {\n");
	for (i = 0; i < NR_FILE_DESC; i++) {
		if (f_desc_table[i].fd_inode == 0)
			continue;

		int inode_tbl_idx = f_desc_table[i].fd_inode - inode_table;
		logbufpos += sprintf(logbuf + logbufpos, "\t\t\"filedesc%d\" [\n", i);
		logbufpos += sprintf(logbuf + logbufpos, "\t\t\tlabel = \"<f0>filedesc %d"
				     "|<f1> fd_mode:%d"
				     "|<f2> fd_pos:%d"
				     "|<f3> fd_cnt:%d"
				     "|<f4> fd_inode:%d",
				     i,
				     f_desc_table[i].fd_mode,
				     f_desc_table[i].fd_pos,
				     f_desc_table[i].fd_cnt,
				     inode_tbl_idx);
		fim[fim_idx].desc = i;
		fim[fim_idx].inode = inode_tbl_idx;
		fim_idx++;

		logbufpos += sprintf(logbuf + logbufpos, "\t\"\n");
		logbufpos += sprintf(logbuf + logbufpos, "\t\t\tshape = \"record\"\n");
		logbufpos += sprintf(logbuf + logbufpos, "\t\t];\n");
	}
	logbufpos += sprintf(logbuf + logbufpos, "\t\tlabel = \"filedescs\";\n");
	logbufpos += sprintf(logbuf + logbufpos, "\t}\n");
#endif

	printl("1");

#if (LOG_INODE_TABLE == 1)
	logbufpos += sprintf(logbuf + logbufpos, "\n\tsubgraph cluster_2 {\n");
	for (i = 0; i < NR_INODE; i++) {
		if (inode_table[i].i_cnt == 0)
			continue;

		logbufpos += sprintf(logbuf + logbufpos, "\t\t\"inode%d\" [\n", i);
		logbufpos += sprintf(logbuf + logbufpos, "\t\t\tlabel = \"<f0>inode %d"
				     "|<f1> i_mode:0x%x"
				     "|<f2> i_size:0x%x"
				     "|<f3> i_start_sect:0x%x"
				     "|<f4> i_nr_sects:0x%x"
				     "|<f5> i_dev:0x%x"
				     "|<f6> i_cnt:%d"
				     "|<f7> i_num:%d",
				     i,
				     inode_table[i].i_mode,
				     inode_table[i].i_size,
				     inode_table[i].i_start_sect,
				     inode_table[i].i_nr_sects,
				     inode_table[i].i_dev,
				     inode_table[i].i_cnt,
				     inode_table[i].i_num);

		logbufpos += sprintf(logbuf + logbufpos, "\t\"\n");
		logbufpos += sprintf(logbuf + logbufpos, "\t\t\tshape = \"record\"\n");
		logbufpos += sprintf(logbuf + logbufpos, "\t\t];\n");
	}
	logbufpos += sprintf(logbuf + logbufpos, "\t\tlabel = \"inodes\";\n");
	logbufpos += sprintf(logbuf + logbufpos, "\t}\n");
#endif

	printl("2");

	enable_int();

#if (LOG_SMAP == 1)
	logbufpos += sprintf(logbuf + logbufpos, "\n\tsubgraph cluster_3 {\n");
	logbufpos += sprintf(logbuf + logbufpos, "\n\t\tstyle=filled;\n");
	logbufpos += sprintf(logbuf + logbufpos, "\n\t\tcolor=lightgrey;\n");
	int smap_flag = 0;
	int bit_start = 0;
	/* i:     sector index */
	int j; /* byte index */
	/* k:     bit index */
	struct super_block * sb = get_super_block(root_inode->i_dev);
	int smap_blk0_nr = 1 + 1 + sb->nr_imap_sects;
	for (i = 0; i < sb->nr_smap_sects; i++) { /* smap_blk0_nr + i : current sect nr. */
		DISKLOG_RD_SECT(root_inode->i_dev, smap_blk0_nr + i);
		memcpy(_buf, logdiskbuf, SECTOR_SIZE);
		for (j = 0; j < SECTOR_SIZE; j++) {
			for (k = 0; k < 8; k++) {
				if (!smap_flag) {
					if ((_buf[j] >> k ) & 1) {
						smap_flag = 1;
						bit_start = (i * SECTOR_SIZE + j) * 8 + k;
					}
					else {
						continue;
					}
				}
				else {
					if ((_buf[j] >> k ) & 1) {
						continue;
					}
					else {
						smap_flag = 0;
						int bit_end = (i * SECTOR_SIZE + j) * 8 + k - 1;
						logbufpos += sprintf(logbuf + logbufpos, "\t\t\"sector %xh\" [\n", bit_start);
						logbufpos += sprintf(logbuf + logbufpos, "\t\t\tlabel = \"<f0>sect %xh-%xh",
								     bit_start,
								     bit_end);
						logbufpos += sprintf(logbuf + logbufpos, "\t\"\n");
						logbufpos += sprintf(logbuf + logbufpos, "\t\t\tshape = \"record\"\n");
						logbufpos += sprintf(logbuf + logbufpos, "\t\t];\n");
					}
				}
			}
		}
	}
	logbufpos += sprintf(logbuf + logbufpos, "\t\tlabel = \"sector map (dev size: %xh)\";\n", sb->nr_sects);
	logbufpos += sprintf(logbuf + logbufpos, "\t}\n");
#endif

	printl("3");

#if (LOG_IMAP == 1)
	logbufpos += sprintf(logbuf + logbufpos, "\n\tsubgraph cluster_4 {\n");
	logbufpos += sprintf(logbuf + logbufpos, "\n\t\tstyle=filled;\n");
	logbufpos += sprintf(logbuf + logbufpos, "\n\t\tcolor=lightgrey;\n");
	logbufpos += sprintf(logbuf + logbufpos, "\t\t\"imap\" [\n");
	logbufpos += sprintf(logbuf + logbufpos, "\t\t\tlabel = \"<f0>bits");
	/* i:     sector index */
	/* j:     byte index */
	/* k:     bit index */
	int imap_blk0_nr = 1 + 1;
	for (i = 0; i < sb->nr_imap_sects; i++) { /* smap_blk0_nr + i : current sect nr. */
		DISKLOG_RD_SECT(root_inode->i_dev, imap_blk0_nr + i);
		memcpy(_buf, logdiskbuf, SECTOR_SIZE);
		for (j = 0; j < SECTOR_SIZE; j++) {
			for (k = 0; k < 8; k++) {
				if ((_buf[j] >> k ) & 1) {
					int bit_nr = (i * SECTOR_SIZE + j) * 8 + k;
					logbufpos += sprintf(logbuf + logbufpos, "| %xh ", bit_nr);
				}
			}
		}
	}
	logbufpos += sprintf(logbuf + logbufpos, "\t\"\n");
	logbufpos += sprintf(logbuf + logbufpos, "\t\t\tshape = \"record\"\n");
	logbufpos += sprintf(logbuf + logbufpos, "\t\t];\n");
	logbufpos += sprintf(logbuf + logbufpos, "\t\tlabel = \"inode map\";\n");
	logbufpos += sprintf(logbuf + logbufpos, "\t}\n");
#endif

	printl("4");

#if (LOG_INODE_ARRAY == 1)
	logbufpos += sprintf(logbuf + logbufpos, "\n\tsubgraph cluster_5 {\n");
	logbufpos += sprintf(logbuf + logbufpos, "\n\t\tstyle=filled;\n");
	logbufpos += sprintf(logbuf + logbufpos, "\n\t\tcolor=lightgrey;\n");
	sb = get_super_block(root_inode->i_dev);
	int blk_nr = 1 + 1 + sb->nr_imap_sects + sb->nr_smap_sects;
	DISKLOG_RD_SECT(root_inode->i_dev, blk_nr);
	memcpy(_buf, logdiskbuf, SECTOR_SIZE);

	char * p = _buf;
	for (i = 0; i < SECTOR_SIZE / sizeof(struct inode); i++,p+=INODE_SIZE) {
		struct inode * pinode = (struct inode*)p;
		if (pinode->i_start_sect == 0)
			continue;
		int start_sect;
		int end_sect;
		if (pinode->i_mode) {
			if (pinode->i_start_sect < sb->n_1st_sect) {
				panic("should not happen: %x < %x.",
				      pinode->i_start_sect,
				      sb->n_1st_sect);
			}
			start_sect =  pinode->i_start_sect - sb->n_1st_sect + 1;
			end_sect = start_sect + pinode->i_nr_sects - 1;
			logbufpos += sprintf(logbuf + logbufpos, "\t\t\"inodearray%d\" [\n", i+1);
			logbufpos += sprintf(logbuf + logbufpos, "\t\t\tlabel = \"<f0> %d"
					     "|<f2> i_size:0x%x"
					     "|<f3> sect: %xh-%xh",
					     i+1,
					     pinode->i_size,
					     start_sect,
					     end_sect);

			logbufpos += sprintf(logbuf + logbufpos, "\t\"\n");
			logbufpos += sprintf(logbuf + logbufpos, "\t\t\tshape = \"record\"\n");
			logbufpos += sprintf(logbuf + logbufpos, "\t\t];\n");
		}
		else {
			start_sect = MAJOR(pinode->i_start_sect);
			end_sect = MINOR(pinode->i_start_sect);
			logbufpos += sprintf(logbuf + logbufpos, "\t\t\"inodearray%d\" [\n", i+1);
			logbufpos += sprintf(logbuf + logbufpos, "\t\t\tlabel = \"<f0> %d"
					     "|<f2> i_size:0x%x"
					     "|<f3> dev nr: (%xh,%xh)",
					     i+1,
					     pinode->i_size,
					     start_sect,
					     end_sect);

			logbufpos += sprintf(logbuf + logbufpos, "\t\"\n");
			logbufpos += sprintf(logbuf + logbufpos, "\t\t\tshape = \"record\"\n");
			logbufpos += sprintf(logbuf + logbufpos, "\t\t];\n");
		}
	}
	logbufpos += sprintf(logbuf + logbufpos, "\t\tlabel = \"inode array\";\n");
	logbufpos += sprintf(logbuf + logbufpos, "\t}\n");
#endif

	printl("5");

#if (LOG_ROOT_DIR == 1)
	logbufpos += sprintf(logbuf + logbufpos, "\n\tsubgraph cluster_6 {\n");
	logbufpos += sprintf(logbuf + logbufpos, "\n\t\tstyle=filled;\n");
	logbufpos += sprintf(logbuf + logbufpos, "\n\t\tcolor=lightgrey;\n");
	sb = get_super_block(root_inode->i_dev);
	int dir_blk0_nr = root_inode->i_start_sect;
	int nr_dir_blks = (root_inode->i_size + SECTOR_SIZE - 1) / SECTOR_SIZE;
	int nr_dir_entries =
	  root_inode->i_size / DIR_ENTRY_SIZE; /**
					       * including unused slots
					       * (the file has been deleted
					       * but the slot is still there)
					       */
	int m = 0;
	struct dir_entry * pde;
	for (i = 0; i < nr_dir_blks; i++) {
		DISKLOG_RD_SECT(root_inode->i_dev, dir_blk0_nr + i);
		memcpy(_buf, logdiskbuf, SECTOR_SIZE);
		pde = (struct dir_entry *)_buf;
		for (j = 0; j < SECTOR_SIZE / DIR_ENTRY_SIZE; j++,pde++) {
			if (pde->inode_nr) {
				memcpy(filename, pde->name, MAX_FILENAME_LEN);
				if (filename[0] == '.')
					filename[0] = '/';
				logbufpos += sprintf(logbuf + logbufpos, "\t\t\"rootdirent%d\" [\n", pde->inode_nr);
				logbufpos += sprintf(logbuf + logbufpos, "\t\t\tlabel = \"<f0> %d"
						     "|<f2> %s",
						     pde->inode_nr,
						     filename);
				logbufpos += sprintf(logbuf + logbufpos, "\t\"\n");
				logbufpos += sprintf(logbuf + logbufpos, "\t\t\tshape = \"record\"\n");
				logbufpos += sprintf(logbuf + logbufpos, "\t\t];\n");

				logbufpos += sprintf(logbuf + logbufpos, "\t"
						     "\"inodearray%d\":f0"
						     " -> "
						     "\"rootdirent%d\":f0"
						     ";\n",
						     pde->inode_nr, pde->inode_nr);
			}
		}
		if (m > nr_dir_entries) /* all entries have been iterated */
			break;
	}

	logbufpos += sprintf(logbuf + logbufpos, "\t\tlabel = \"root dir\";\n");
	logbufpos += sprintf(logbuf + logbufpos, "\t}\n");
#endif

	printl("6");

#if (LOG_MSG_SRC2DST == 1)
	for (i = 0; i < msd_idx; i++) {
		if (msd[i].dir == RECEIVING)
			logbufpos += sprintf(logbuf + logbufpos, "\t\"proc%d\":f0 -> \"proc%d\":f0 "
					     "[arrowhead=\"crow\", color=\"green\", label=\"%d\"];\n",
					     msd[i].dst,
					     msd[i].src,
					     i);
		else if (msd[i].dir == SENDING)
			logbufpos += sprintf(logbuf + logbufpos, "\t\"proc%d\":f0 -> \"proc%d\":f0 "
					     "[arrowhead=\"vee\", color=\"blue\", label=\"%d\"];\n",
					     msd[i].src,
					     msd[i].dst,
					     i);
		else
			assert(0);
	}
#endif

#if (LOG_ARROW_PARENT_CHILD == 1)
	for (i = 0; i < ppm_idx; i++) {
		logbufpos += sprintf(logbuf + logbufpos, "\t\"proc%d\":f0 -> \"proc%d\":f0 "
				     "[arrowhead=\"dot\", color=\"ivory3\"];\n",
				     ppm[i].ppid,
				     ppm[i].pid);
	}
#endif

#if (LOG_ARROW_PROC_FD == 1)
	for (i = 0; i < pfm_idx; i++) {
		logbufpos += sprintf(logbuf + logbufpos, "\t\"proc%d\":f%d -> \"filedesc%d\":f3;\n",
				     pfm[i].pid,
				     pfm[i].filp,
				     pfm[i].desc);
	}
#endif

#if (LOG_ARROW_FD_INODE == 1)
	for (i = 0; i < fim_idx; i++) {
		logbufpos += sprintf(logbuf + logbufpos, "\t\"filedesc%d\":f4 -> \"inode%d\":f6;\n",
				     fim[i].desc,
				     fim[i].inode);
	}
#endif

#if (LOG_ARROW_INODE_INODEARRAY == 1)
	for (i = 0; i < NR_INODE; i++) {
		if (inode_table[i].i_cnt != 0)
			logbufpos += sprintf(logbuf + logbufpos, "\t\"inode%d\":f7 -> \"inodearray%d\":f0;\n",
					     i,
					     inode_table[i].i_num);
	}
#endif
	/* for (i = 0; i < il_idx; i++) { */
	/* 	logbufpos += sprintf(logbuf + logbufpos, "\t\"inode%d\":f7 -> \"inodearray%d\":f0;\n", */
	/* 	       inode_list[i], inode_list[i]); */
	/* } */

	/* tail */
	logbufpos += sprintf(logbuf + logbufpos, "\tlabel = \"%s\";\n", title);
	logbufpos += sprintf(logbuf + logbufpos, "}\n");

	/* separator */
	logbufpos += sprintf(logbuf + logbufpos, "--separator--\n");

	assert(logbufpos < LOGBUF_SIZE);

	logbuf[logbufpos] = 0;
	char tmp[STR_DEFAULT_LEN/2];
	int bytes_left = logbufpos;
	int pos = 0;
	while (bytes_left) {
		int bytes = min(bytes_left, STR_DEFAULT_LEN/2 - 1);
		memcpy(tmp, logbuf + pos, bytes);
		tmp[bytes] = 0;
		disklog(tmp);
		pos += bytes;
		bytes_left -= bytes;
	}

	disable_int();
	p_proc = proc_table;
	for (i = 0; i < NR_TASKS + NR_PROCS; i++,p_proc++) {
		if (p_proc->p_flags == FREE_SLOT)
			continue;
		if ((i == TASK_TTY) ||
		    (i == TASK_SYS) ||
		    (i == TASK_HD)  ||
		    /* (i == TASK_FS)  || */
		    (i == getpid()))
			continue;
		p_proc->ticks = tcks[i];
		p_proc->priority = prio[i];
	}
	enable_int();

	printl("|>");

	/* int pos = logbufpos += sprintf(logbuf + logbufpos, "--separator--\n"); */
	/* printl("dump_fd_graph(%s)::logbufpos:%d\n", title, logbufpos); */
}

