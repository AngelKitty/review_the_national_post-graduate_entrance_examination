
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            global.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/* EXTERN is defined as extern except in global.c */
#ifdef	GLOBAL_VARIABLES_HERE
#undef	EXTERN
#define	EXTERN
#endif

EXTERN	int	ticks;

EXTERN	int	disp_pos;

EXTERN	u8			gdt_ptr[6];	/* 0~15:Limit  16~47:Base */
EXTERN	struct descriptor	gdt[GDT_SIZE];
EXTERN	u8			idt_ptr[6];	/* 0~15:Limit  16~47:Base */
EXTERN	struct gate		idt[IDT_SIZE];

EXTERN	u32	k_reenter;
EXTERN	int	nr_current_console;

EXTERN	struct tss	tss;
EXTERN	struct proc*	p_proc_ready;

extern	char		task_stack[];
extern	struct proc	proc_table[];
extern  struct task	task_table[];
extern  struct task	user_proc_table[];
extern	irq_handler	irq_table[];
extern	TTY		tty_table[];
extern  CONSOLE		console_table[];

