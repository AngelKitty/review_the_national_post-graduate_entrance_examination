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

static char _buf[SECTOR_SIZE];

/*****************************************************************************
 *                                do_disklog
 *****************************************************************************/
/**
 * Perform syslog() system call .
 *
 * @return 
 *****************************************************************************/
PUBLIC int do_disklog()
{
	char buf[STR_DEFAULT_LEN];

	/* get parameters from the message */
	int str_len = fs_msg.CNT;	/* length of filename */
	int src = fs_msg.source;	/* caller proc nr. */
	assert(str_len < STR_DEFAULT_LEN);
	phys_copy((void*)va2la(TASK_FS, buf),    /* to   */
		  (void*)va2la(src, fs_msg.BUF), /* from */
		  str_len);
	buf[str_len] = 0;	/* terminate the string */

	return disklog(buf);
}

/*****************************************************************************
 *                                disklog
 *****************************************************************************/
/**
 * <Ring 1> This routine handles the DEV_LOG message.
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
			RD_SECT(device, sect_nr + i); /* RD_SECT(?, 12) */

			for (; byte_off < SECTOR_SIZE && bits_left > 0; byte_off++) {
				for (; bit_off < 8; bit_off++) { /* repeat till enough bits are set */
					assert(((fsbuf[byte_off] >> bit_off) & 1) == 0);
					fsbuf[byte_off] |= (1 << bit_off);
					if (--bits_left  == 0)
						break;
				}
				bit_off = 0;
			}
			byte_off = 0;
			bit_off = 0;

			WR_SECT(device, sect_nr + i);

			if (bits_left == 0)
				break;
		}
		assert(bits_left == 0);
#endif /* SET_LOG_SECT_SMAP_AT_STARTUP */

		pos = 0x40;

#ifdef MEMSET_LOG_SECTS
		/* write padding stuff to log sectors */
		int chunk = min(MAX_IO_BYTES, FSBUF_SIZE >> SECTOR_SIZE_SHIFT);
		assert(chunk == 256);
		int sects_left = NR_SECTS_FOR_LOG;
		for (i = nr_log_blk0_nr;
		     i < nr_log_blk0_nr + NR_SECTS_FOR_LOG;
		     i += chunk) {
			memset(fsbuf, 0x20, chunk*SECTOR_SIZE);
			rw_sector(DEV_WRITE,
				  device,
				  i * SECTOR_SIZE,
				  chunk * SECTOR_SIZE,
				  TASK_FS,
				  fsbuf);
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
		RD_SECT(device, sect_nr);

		int off = pos % SECTOR_SIZE;
		int bytes = min(bytes_left, SECTOR_SIZE - off);

		memcpy(&fsbuf[off], p, bytes);
		off += bytes;
		bytes_left -= bytes;

		WR_SECT(device, sect_nr);
		sect_nr++;
		pos += bytes;
		p += bytes;
	}

	/* write `pos' into the log file header */
	RD_SECT(device, nr_log_blk0_nr);

	sprintf((char*)fsbuf, "%8d\n", pos);
	memset(fsbuf+9, ' ', 22);
	fsbuf[31] = '\n';

	memset(fsbuf+32, ' ', 31);
	fsbuf[63] = '\n';

	WR_SECT(device, nr_log_blk0_nr);
	memset(fsbuf+64, fsbuf[32+19], 512-64);
	WR_SECT(device, nr_log_blk0_nr + NR_SECTS_FOR_LOG - 1);

	return pos;
}


/*****************************************************************************
 *                                dump_fd_graph
 *****************************************************************************/
/**
 * Output a dot graph.
 * 
 *****************************************************************************/
PUBLIC void dump_fd_graph(const char * fmt, ...)
{
	int i;
	char title[STR_DEFAULT_LEN];

	va_list arg = (va_list)((char*)(&fmt) + 4); /**
						     * 4: size of `fmt' in
						     *    the stack
						     */
	i = vsprintf(title, fmt, arg);
	assert(strlen(title) == i);

	static int graph_idx = 0;
	char filename[MAX_FILENAME_LEN+1];

	char * proc_flags[32];
	proc_flags[0x02] = "SENDING";
	proc_flags[0x04] = "RECEIVING";
	proc_flags[0x08] = "WAITING";
	proc_flags[0x10] = "FREE_SLOT";

	struct proc_fdesc_map {
		int pid;	/* PID */
		int filp;	/* idx of proc_table[pid].filp[] */
		int desc;	/* idx of f_desc_table[] */
	} pfm[256];
	int pfm_idx = 0;

	struct fdesc_inode_map {
		int desc;	/* idx of f_desc_table[] */
		int inode;	/* idx of inode_table[] */
	} fim[256];
	int fim_idx = 0;

	/* head */
	SYSLOG("digraph filedesc%02d {\n", graph_idx++);
	SYSLOG("\tgraph [\n");
	SYSLOG("		rankdir = \"LR\"\n");
	SYSLOG("	];\n");
	SYSLOG("	node [\n");
	SYSLOG("		fontsize = \"16\"\n");
	SYSLOG("		shape = \"ellipse\"\n");
	SYSLOG("	];\n");
	SYSLOG("	edge [\n");
	SYSLOG("	];\n");

	int k;
	struct proc* p_proc = proc_table;
	SYSLOG("\n\tsubgraph cluster_0 {\n");
	for (i = 0; i < NR_TASKS + NR_PROCS; i++,p_proc++) {
		/* skip procs which open no files */
		for (k = 0; k < NR_FILES; k++) {
			if (p_proc->filp[k] != 0)
				break;
		}
		if (k == NR_FILES)
			continue;

		SYSLOG("\t\t\"proc%d\" [\n", i);
		SYSLOG("\t\t\tlabel = \"<f0>%s (%d) "
		       "|<f1> p_flags:%d(%s)",
		       p_proc->name,
		       i,
		       p_proc->p_flags,
		       proc_flags[p_proc->p_flags]);
		int fnr = 3;
		for (k = 0; k < NR_FILES; k++) {
			if (p_proc->filp[k] == 0)
				continue;

			int fdesc_tbl_idx = p_proc->filp[k] - f_desc_table;
			SYSLOG("\t|<f%d> filp[%d]: %d",
			       fnr,
			       k,
			       fdesc_tbl_idx);
			pfm[pfm_idx].pid = i;
			pfm[pfm_idx].filp = fnr;
			pfm[pfm_idx].desc = fdesc_tbl_idx;
			fnr++;
			pfm_idx++;
		}

		SYSLOG("\t\"\n");
		SYSLOG("\t\t\tshape = \"record\"\n");
		SYSLOG("\t\t];\n");
	}
	SYSLOG("\t\tlabel = \"procs\";\n");
	SYSLOG("\t}\n");

	SYSLOG("\n\tsubgraph cluster_1 {\n");
	for (i = 0; i < NR_FILE_DESC; i++) {
		if (f_desc_table[i].fd_inode == 0)
			continue;

		int inode_tbl_idx = f_desc_table[i].fd_inode - inode_table;
		SYSLOG("\t\t\"filedesc%d\" [\n", i);
		SYSLOG("\t\t\tlabel = \"<f0>filedesc %d"
		       "|<f1> fd_mode:%d"
		       "|<f2> fd_pos:%d"
		       "|<f4> fd_inode:%d",
		       i,
		       f_desc_table[i].fd_mode,
		       f_desc_table[i].fd_pos,
		       inode_tbl_idx);
		fim[fim_idx].desc = i;
		fim[fim_idx].inode = inode_tbl_idx;
		fim_idx++;

		SYSLOG("\t\"\n");
		SYSLOG("\t\t\tshape = \"record\"\n");
		SYSLOG("\t\t];\n");
	}
	SYSLOG("\t\tlabel = \"filedescs\";\n");
	SYSLOG("\t}\n");

	SYSLOG("\n\tsubgraph cluster_2 {\n");
	for (i = 0; i < NR_INODE; i++) {
		if (inode_table[i].i_cnt == 0)
			continue;

		SYSLOG("\t\t\"inode%d\" [\n", i);
		SYSLOG("\t\t\tlabel = \"<f0>inode %d"
		       "|<f1> i_mode:0x%x"
		       "|<f2> i_size:0x%x"
		       "|<f3> i_start_sect:0x%x"
		       "|<f4> i_nr_sects:0x%x"
		       "|<f5> i_dev:0x%x"
		       "|<f6> i_cnt:%d"
		       "|<f7> i_num:%d",
		       inode_table[i].i_num,
		       inode_table[i].i_mode,
		       inode_table[i].i_size,
		       inode_table[i].i_start_sect,
		       inode_table[i].i_nr_sects,
		       inode_table[i].i_dev,
		       inode_table[i].i_cnt,
		       inode_table[i].i_num);
		assert(filename != 0);

		SYSLOG("\t\"\n");
		SYSLOG("\t\t\tshape = \"record\"\n");
		SYSLOG("\t\t];\n");
	}
	SYSLOG("\t\tlabel = \"inodes\";\n");
	SYSLOG("\t}\n");

	SYSLOG("\n\tsubgraph cluster_3 {\n");
	SYSLOG("\n\t\tstyle=filled;\n");
	SYSLOG("\n\t\tcolor=lightgrey;\n");
	int smap_flag = 0;
	int bit_start = 0;
	/* i:     sector index */
	int j; /* byte index */
	/* k:     bit index */
	struct super_block * sb = get_super_block(root_inode->i_dev);
	int smap_blk0_nr = 1 + 1 + sb->nr_imap_sects;
	for (i = 0; i < sb->nr_smap_sects; i++) { /* smap_blk0_nr + i : current sect nr. */
		RD_SECT(root_inode->i_dev, smap_blk0_nr + i);
		memcpy(_buf, fsbuf, SECTOR_SIZE);
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
						SYSLOG("\t\t\"sector %xh\" [\n", bit_start);
						SYSLOG("\t\t\tlabel = \"<f0>sect %xh-%xh",
						       bit_start,
						       bit_end);
						SYSLOG("\t\"\n");
						SYSLOG("\t\t\tshape = \"record\"\n");
						SYSLOG("\t\t];\n");
					}
				}
			}
		}
	}
	SYSLOG("\t\tlabel = \"sector map (dev size: %xh)\";\n", sb->nr_sects);
	SYSLOG("\t}\n");

	SYSLOG("\n\tsubgraph cluster_4 {\n");
	SYSLOG("\n\t\tstyle=filled;\n");
	SYSLOG("\n\t\tcolor=lightgrey;\n");
	SYSLOG("\t\t\"imap\" [\n");
	SYSLOG("\t\t\tlabel = \"<f0>bits");
	/* i:     sector index */
	/* j:     byte index */
	/* k:     bit index */
	int imap_blk0_nr = 1 + 1;
	for (i = 0; i < sb->nr_imap_sects; i++) { /* smap_blk0_nr + i : current sect nr. */
		RD_SECT(root_inode->i_dev, imap_blk0_nr + i);
		memcpy(_buf, fsbuf, SECTOR_SIZE);
		for (j = 0; j < SECTOR_SIZE; j++) {
			for (k = 0; k < 8; k++) {
				if ((_buf[j] >> k ) & 1) {
					int bit_nr = (i * SECTOR_SIZE + j) * 8 + k;
					SYSLOG("| %xh ", bit_nr);
				}
			}
		}
	}
	SYSLOG("\t\"\n");
	SYSLOG("\t\t\tshape = \"record\"\n");
	SYSLOG("\t\t];\n");
	SYSLOG("\t\tlabel = \"inode map\";\n");
	SYSLOG("\t}\n");

	SYSLOG("\n\tsubgraph cluster_5 {\n");
	SYSLOG("\n\t\tstyle=filled;\n");
	SYSLOG("\n\t\tcolor=lightgrey;\n");
	sb = get_super_block(root_inode->i_dev);
	int blk_nr = 1 + 1 + sb->nr_imap_sects + sb->nr_smap_sects;
	RD_SECT(root_inode->i_dev, blk_nr);
	memcpy(_buf, fsbuf, SECTOR_SIZE);

	char * p = _buf;
	for (i = 0; i < SECTOR_SIZE / sizeof(struct inode); i++,p+=INODE_SIZE) {
		struct inode * pinode = (struct inode*)p;
		if (pinode->i_start_sect == 0)
			continue;
		int start_sect;
		int end_sect;
		if (pinode->i_mode != I_CHAR_SPECIAL) {
			if (pinode->i_start_sect < sb->n_1st_sect) {
				panic("should not happen: %x < %x.",
				      pinode->i_start_sect,
				      sb->n_1st_sect);
			}
			start_sect =  pinode->i_start_sect - sb->n_1st_sect + 1;
			end_sect = start_sect + pinode->i_nr_sects - 1;
			SYSLOG("\t\t\"inodearray%d\" [\n", i+1);
			SYSLOG("\t\t\tlabel = \"<f0> %d"
			       "|<f2> i_size:0x%x"
			       "|<f3> sect: %xh-%xh",
			       i+1,
			       pinode->i_size,
			       start_sect,
			       end_sect);

			SYSLOG("\t\"\n");
			SYSLOG("\t\t\tshape = \"record\"\n");
			SYSLOG("\t\t];\n");
		}
		else {
			start_sect = MAJOR(pinode->i_start_sect);
			end_sect = MINOR(pinode->i_start_sect);
			SYSLOG("\t\t\"inodearray%d\" [\n", i+1);
			SYSLOG("\t\t\tlabel = \"<f0> %d"
			       "|<f2> i_size:0x%x"
			       "|<f3> dev nr: (%xh,%xh)",
			       i+1,
			       pinode->i_size,
			       start_sect,
			       end_sect);

			SYSLOG("\t\"\n");
			SYSLOG("\t\t\tshape = \"record\"\n");
			SYSLOG("\t\t];\n");
		}
	}
	SYSLOG("\t\tlabel = \"inode array\";\n");
	SYSLOG("\t}\n");

	SYSLOG("\n\tsubgraph cluster_6 {\n");
	SYSLOG("\n\t\tstyle=filled;\n");
	SYSLOG("\n\t\tcolor=lightgrey;\n");
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
		RD_SECT(root_inode->i_dev, dir_blk0_nr + i);
		memcpy(_buf, fsbuf, SECTOR_SIZE);
		pde = (struct dir_entry *)_buf;
		for (j = 0; j < SECTOR_SIZE / DIR_ENTRY_SIZE; j++,pde++) {
			if (pde->inode_nr) {
				memcpy(filename, pde->name, MAX_FILENAME_LEN);
				if (filename[0] == '.')
					filename[0] = '/';
				SYSLOG("\t\t\"rootdirent%d\" [\n", pde->inode_nr);
				SYSLOG("\t\t\tlabel = \"<f0> %d"
				       "|<f2> %s",
				       pde->inode_nr,
				       filename);
				SYSLOG("\t\"\n");
				SYSLOG("\t\t\tshape = \"record\"\n");
				SYSLOG("\t\t];\n");

				SYSLOG("\t"
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

	SYSLOG("\t\tlabel = \"root dir\";\n");
	SYSLOG("\t}\n");

	for (i = 0; i < pfm_idx; i++) {
		SYSLOG("\t\"proc%d\":f%d -> \"filedesc%d\":f0;\n",
		       pfm[i].pid,
		       pfm[i].filp,
		       pfm[i].desc);
	}

	for (i = 0; i < fim_idx; i++) {
		SYSLOG("\t\"filedesc%d\":f4 -> \"inode%d\":f6;\n",
		       fim[i].desc,
		       fim[i].inode);
	}

	for (i = 0; i < NR_INODE; i++) {
		if (inode_table[i].i_cnt != 0)
			SYSLOG("\t\"inode%d\":f7 -> \"inodearray%d\":f0;\n",
			       i,
			       inode_table[i].i_num);
	}

	/* tail */
	SYSLOG("\tlabel = \"%s\";\n", title);
	SYSLOG("}\n");

	/* separator */
	SYSLOG("--separator--\n");
	/* int pos = SYSLOG("--separator--\n"); */
	/* printl("dump_fd_graph(%s)::logpos:%d\n", title, pos); */
}

