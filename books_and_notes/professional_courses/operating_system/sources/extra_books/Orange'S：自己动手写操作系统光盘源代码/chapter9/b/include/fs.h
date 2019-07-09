/*************************************************************************//**
 *****************************************************************************
 * @file   include/sys/fs.h
 * @brief  Header file for File System.
 * @author Forrest Yu
 * @date   2008
 *****************************************************************************
 *****************************************************************************/

#ifndef	_ORANGES_FS_H_
#define	_ORANGES_FS_H_

/**
 * @struct dev_drv_map fs.h "include/sys/fs.h"
 * @brief  The Device_nr.\ - Driver_nr.\ MAP.
 *
 *  \dot
 *  digraph DD_MAP {
 *    graph[rankdir=LR];
 *    node [shape=record, fontname=Helvetica];
 *    b [ label="Device Nr."];
 *    c [ label="Driver (the task)"];
 *    b -> c [ label="DD_MAP", fontcolor=blue, URL="\ref DD_MAP", arrowhead="open", style="dashed" ];
 *  }
 *  \enddot
 */
struct dev_drv_map {
	int driver_nr; /**< The proc nr.\ of the device driver. */
};

/**
 * @def   MAGIC_V1
 * @brief Magic number of FS v1.0
 */
#define	MAGIC_V1	0x111

/**
 * @struct super_block fs.h "include/fs.h"
 * @brief  The 2nd sector of the FS
 *
 * Remember to change SUPER_BLOCK_SIZE if the members are changed.
 *
 * @attention Remember to change boot/include/load.inc::SB_* if the members
 *            are changed.
 *
 * \dot
 * digraph super_block {
 *         node [shape=plaintext];
 * 
 *         sb [label=<<TABLE CELLPADDING="0" CELLSPACING="0" BORDER="0">
 *                 <TR>
 *                         <TD HEIGHT="20" WIDTH="70" ALIGN="right" VALIGN="top" BORDER="0">Sector #&nbsp;</TD>
 *                         <TD HEIGHT="20" WIDTH="90" ALIGN="left" VALIGN="top" BORDER="0">0</TD>
 *                         <TD HEIGHT="20" WIDTH="90" ALIGN="left" VALIGN="top" BORDER="0">1</TD>
 *                         <TD HEIGHT="20" WIDTH="290" ALIGN="center" VALIGN="top" BORDER="0">...</TD>
 *                         <TD HEIGHT="20" WIDTH="10" ALIGN="right" BORDER="0"></TD>
 *                 </TR>
 *                 <TR>
 *                         <TD HEIGHT="30" WIDTH="70" ALIGN="right" BORDER="0"></TD>
 *                         <TD HEIGHT="30" WIDTH="90" ALIGN="center" BORDER="1" BGCOLOR="white">Boot Sector</TD>
 *                         <TD HEIGHT="30" WIDTH="90" ALIGN="center" BORDER="1" BGCOLOR="grey">Super Block</TD>
 *                         <TD HEIGHT="30" WIDTH="290" ALIGN="center" BORDER="1" BGCOLOR="white">...</TD>
 *                         <TD HEIGHT="30" WIDTH="10" ALIGN="right" BORDER="0"></TD>
 *                 </TR>
 *         </TABLE>>];
 * }
 * \enddot
 */
struct super_block {
	u32	magic;		  /**< Magic number */
	u32	nr_inodes;	  /**< How many inodes */
	u32	nr_sects;	  /**< How many sectors (including bit maps) */
	u32	nr_imap_sects;	  /**< How many inode-map sectors */
	u32	nr_smap_sects;	  /**< How many sector-map sectors */
	u32	n_1st_sect;	  /**< Number of the 1st data sector */

	u32	nr_inode_sects;   /**< How many inode sectors */
	u32	root_inode;       /**< Inode nr of root directory */
	u32	inode_size;       /**< INODE_SIZE */
	u32	inode_isize_off;  /**< Offset of `struct inode::i_size' */
	u32	inode_start_off;  /**< Offset of `struct inode::i_start_sect' */
	u32	dir_ent_size;     /**< DIR_ENTRY_SIZE */
	u32	dir_ent_inode_off;/**< Offset of `struct dir_entry::inode_nr' */
	u32	dir_ent_fname_off;/**< Offset of `struct dir_entry::name' */

	/*
	 * the following item(s) are only present in memory
	 */
	int	sb_dev; 	/**< the super block's home device */
};

/**
 * @def   SUPER_BLK_MAGIC_V1
 * @brief Magic number of super block, version 1.
 * @attention It must correspond with boot/include/load.h::SB_MAGIC_V1
 */
#define	SUPER_BLK_MAGIC_V1		0x111

/**
 * @def   SUPER_BLOCK_SIZE
 * @brief The size of super block \b in \b the \b device.
 *
 * Note that this is the size of the struct in the device, \b NOT in memory.
 * The size in memory is larger because of some more members.
 */
#define	SUPER_BLOCK_SIZE	56

/**
 * @struct inode
 * @brief  i-node
 *
 * The file, currently, have tree valid attributes only:
 *   - size
 *   - start_sect
 *   - nr_sects
 *
 * The \c start_sect and\c nr_sects locate the file in the device,
 * and the size show how many bytes is used.
 * If <tt> size < (nr_sects * SECTOR_SIZE) </tt>, the rest bytes
 * are wasted and reserved for later writing.
 *
 * \b NOTE: Remember to change INODE_SIZE if the members are changed
 */
struct inode {
	u32	i_mode;		/**< Accsess mode. Unused currently */
	u32	i_size;		/**< File size */
	u32	i_start_sect;	/**< The first sector of the data */
	u32	i_nr_sects;	/**< How many sectors the file occupies */
	u8	_unused[16];	/**< Stuff for alignment */

	/* the following items are only present in memory */
	int	i_dev;
	int	i_cnt;		/**< How many procs share this inode  */
	int	i_num;		/**< inode nr.  */
};

/**
 * @def   INODE_SIZE
 * @brief The size of i-node stored \b in \b the \b device.
 *
 * Note that this is the size of the struct in the device, \b NOT in memory.
 * The size in memory is larger because of some more members.
 */
#define	INODE_SIZE	32

/**
 * @struct file_desc
 * @brief  File Descriptor
 */
struct file_desc {
	int		fd_mode;	/**< R or W */
	int		fd_pos;		/**< Current position for R/W. */
	struct inode*	fd_inode;	/**< Ptr to the i-node */
};


/**
 * Since all invocations of `rw_sector()' in FS look similar (most of the
 * params are the same), we use this macro to make code more readable.
 *
 * Before I wrote this macro, I found almost every rw_sector invocation
 * line matchs this emacs-style regex:
 * `rw_sector(\([-a-zA-Z0-9_>\ \*()+.]+,\)\{3\}\ *SECTOR_SIZE,\ *TASK_FS,\ *fsbuf)'
 */
#define RD_SECT(dev,sect_nr) rw_sector(DEV_READ, \
				       dev,				\
				       (sect_nr) * SECTOR_SIZE,		\
				       SECTOR_SIZE, /* read one sector */ \
				       TASK_FS,				\
				       fsbuf);
#define WR_SECT(dev,sect_nr) rw_sector(DEV_WRITE, \
				       dev,				\
				       (sect_nr) * SECTOR_SIZE,		\
				       SECTOR_SIZE, /* write one sector */ \
				       TASK_FS,				\
				       fsbuf);

	
#endif /* _ORANGES_FS_H_ */
