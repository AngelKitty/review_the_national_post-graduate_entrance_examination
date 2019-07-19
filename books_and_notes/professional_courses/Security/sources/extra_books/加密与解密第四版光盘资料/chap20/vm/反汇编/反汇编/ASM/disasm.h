// Free Disassembler and Assembler -- Header file
//
// Copyright (C) 2001 Oleh Yuschuk
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#pragma once

#ifndef MAINPROG
#define odunique extern
#else
#define odunique
#endif

#pragma   warning(disable:4309)//关闭截断常数值警告

// If you prefere Borland, this will force necessary setting (but, as a side
// effect, may cause plenty of warnings if other include files will be compiled
// with different options):

#ifdef __BORLANDC__
#pragma option -K                      // Unsigned char
#endif
//
//#if (char)0xFF != 255
//#error Please set default char type to unsigned
//#endif


#define NEGLIMIT       (-16384)        // Limit to display constans as signed
#define PSEUDOOP       128             // Base for pseudooperands
#define TEXTLEN        256             // Maximal length of text string

// Special command features.
#define WW             0x01            // Bit W (size of operand)
#define SS             0x02            // Bit S (sign extention of immediate)
#define WS             0x03            // Bits W and S
#define W3             0x08            // Bit W at position 3
#define CC             0x10            // Conditional jump
#define FF             0x20            // Forced 16-bit size
#define LL             0x40            // Conditional loop
#define PR             0x80            // Protected command
#define WP             0x81            // I/O command with bit W

// All possible types of operands in 80x86. A bit more than you expected, he?
#define NNN            0               // No operand \\无操作
#define REG            1               // Integer register in Reg field             \\积存器结构中的寄存器
#define RCM            2               // Integer register in command byte          \\命令字节中的寄存器
#define RG4            3               // Integer 4-byte register in Reg field      \\用不到PMOVMSKB指令
#define RAC            4               // Accumulator (AL/AX/EAX, implicit)         \\暗示累加器?
#define RAX            5               // AX (2-byte, implicit)						\\暗示AX
#define RDX            6               // DX (16-bit implicit port address)
#define RCL            7               // Implicit CL register (for shifts)         \\暗示CL寄存器
#define RS0            8               // Top of FPU stack (ST(0), implicit)
#define RST            9               // FPU register (ST(i)) in command byte
#define RMX            10              // MMX register MMx
#define R3D            11              // 3DNow! register MMx
#define MRG            12              // Memory/register in ModRM byte
#define MR1            13              // 1-byte memory/register in ModRM byte
#define MR2            14              // 2-byte memory/register in ModRM byte
#define MR4            15              // 4-byte memory/register in ModRM byte
#define RR4            16              // 4-byte memory/register (register only)
#define MR8            17              // 8-byte memory/MMX register in ModRM
#define RR8            18              // 8-byte MMX register only in ModRM
#define MRD            19              // 8-byte memory/3DNow! register in ModRM
#define RRD            20              // 8-byte memory/3DNow! (register only)
#define MRJ            21              // Memory/reg in ModRM as JUMP target
#define MMA            22              // Memory address in ModRM byte for LEA
#define MML            23              // Memory in ModRM byte (for LES)
#define MMS            24              // Memory in ModRM byte (as SEG:OFFS)
#define MM6            25              // Memory in ModRm (6-byte descriptor)
#define MMB            26              // Two adjacent memory locations (BOUND)
#define MD2            27              // Memory in ModRM (16-bit integer)
#define MB2            28              // Memory in ModRM (16-bit binary)
#define MD4            29              // Memory in ModRM byte (32-bit integer)
#define MD8            30              // Memory in ModRM byte (64-bit integer)
#define MDA            31              // Memory in ModRM byte (80-bit BCD)
#define MF4            32              // Memory in ModRM byte (32-bit float)
#define MF8            33              // Memory in ModRM byte (64-bit float)
#define MFA            34              // Memory in ModRM byte (80-bit float)
#define MFE            35              // Memory in ModRM byte (FPU environment)
#define MFS            36              // Memory in ModRM byte (FPU state)
#define MFX            37              // Memory in ModRM byte (ext. FPU state)
#define MSO            38              // Source in string op's ([ESI])
#define MDE            39              // Destination in string op's ([EDI])
#define MXL            40              // XLAT operand ([EBX+AL])
#define IMM            41              // Immediate data (8 or 16/32)
#define IMU            42              // Immediate unsigned data (8 or 16/32)
#define VXD            43              // VxD service
#define IMX            44              // Immediate sign-extendable byte
#define C01            45              // Implicit constant 1 (for shifts)
#define IMS            46              // Immediate byte (for shifts)
#define IM1            47              // Immediate byte
#define IM2            48              // Immediate word (ENTER/RET)
#define IMA            49              // Immediate absolute near data address
#define JOB            50              // Immediate byte offset (for jumps)
#define JOW            51              // Immediate full offset (for jumps)
#define JMF            52              // Immediate absolute far jump/call addr
#define SGM            53              // Segment register in ModRM byte
#define SCM            54              // 段寄存器      Segment register in command byte
#define CRX            55              // CRx控制寄存器 Control register CRx
#define DRX            56              // DRx调试寄存器 Debug register DRx
// Pseudooperands (implicit operands, never appear in assembler commands). Must
// have index equal to or exceeding PSEUDOOP.
#define PRN            (PSEUDOOP+0)    // Near return address
#define PRF            (PSEUDOOP+1)    // Far return address
#define PAC            (PSEUDOOP+2)    // Accumulator (AL/AX/EAX)
#define PAH            (PSEUDOOP+3)    // AH (in LAHF/SAHF commands)
#define PFL            (PSEUDOOP+4)    // Lower byte of flags (in LAHF/SAHF)
#define PS0            (PSEUDOOP+5)    // Top of FPU stack (ST(0))
#define PS1            (PSEUDOOP+6)    // ST(1)
#define PCX            (PSEUDOOP+7)    // CX/ECX
#define PDI            (PSEUDOOP+8)    // EDI (in MMX extentions)

// Errors detected during command disassembling.
#define DAE_NOERR      0               // No error
#define DAE_BADCMD     1               // Unrecognized command
#define DAE_CROSS      2               // Command crosses end of memory block
#define DAE_BADSEG     3               // Undefined segment register
#define DAE_MEMORY     4               // Register where only memory allowed
#define DAE_REGISTER   5               // Memory where only register allowed
#define DAE_INTERN     6               // Internal error

typedef unsigned char  uchar;          // Unsigned character (byte)
typedef unsigned short ushort;         // Unsigned short
typedef unsigned int   uint;           // Unsigned integer
typedef unsigned long  ulong;          // Unsigned long

typedef struct t_addrdec
{
	int            defseg;
	char           *descr;
} t_addrdec;
//命令数据
typedef struct t_cmddata
{
	ulong          mask;                 // Mask for first 4 bytes of the command
	ulong          code;                 // Compare masked bytes with this
	char           len;                  // Length of the main command code
	char           bits;                 // Special bits within the command
	char           arg1,arg2,arg3;       // Types of possible arguments
	char           type;                 // C_xxx + additional information
	char           *name;                // Symbolic name for this command
} t_cmddata;

// Initialized constant data structures used by all programs from assembler
// package. Contain names of register, register combinations or commands and
// their properties.
extern const char      *regname[3][9];
extern const char      *segname[8];
extern const char      *sizename[11];
extern const t_addrdec addr16[8];
extern const t_addrdec addr32[8];
extern const char      *fpuname[9];
extern const char      *mmxname[9];
extern const char      *crname[9];
extern const char      *drname[9];
extern const char      *condition[16];
extern const t_cmddata cmddata[];//可用结构数组
extern const t_cmddata vxdcmd;
extern const t_cmddata dangerous[];

////////////////////////////////////////////////////////////////////////////////
//////////////////// ASSEMBLER, DISASSEMBLER AND EXPRESSIONS ///////////////////

#define MAXCMDSIZE     16              // Maximal length of 80x86 command
#define MAXCALSIZE     8               // Max length of CALL without prefixes
#define NMODELS        8               // Number of assembler search models

#define INT3           0xCC            // Code of 1-byte breakpoint
#define NOP            0x90            // Code of 1-byte NOP command
#define TRAPFLAG       0x00000100      // Trap flag in CPU flag register

#define REG_EAX        0               // Indexes of general-purpose registers
#define REG_ECX        1               // in t_reg.
#define REG_EDX        2
#define REG_EBX        3
#define REG_ESP        4
#define REG_EBP        5
#define REG_ESI        6
#define REG_EDI        7

#define SEG_UNDEF     -1			   //没有段寄存器
#define SEG_ES         0               // Indexes of segment/selector registers
#define SEG_CS         1
#define SEG_SS         2
#define SEG_DS         3
#define SEG_FS         4
#define SEG_GS         5

#define C_TYPEMASK     0xF0            // Mask for command type
#define   C_CMD        0x00            // Ordinary instruction
#define   C_PSH        0x10            // 1-word PUSH instruction
#define   C_POP        0x20            // 1-word POP instruction
#define   C_MMX        0x30            // MMX instruction
#define   C_FLT        0x40            // FPU instruction
#define   C_JMP        0x50            // JUMP instruction
#define   C_JMC        0x60            // Conditional JUMP instruction
#define   C_CAL        0x70            // CALL instruction
#define   C_RET        0x80            // RET instruction
#define   C_FLG        0x90            // Changes system flags
#define   C_RTF        0xA0            // C_JMP and C_FLG simultaneously
#define   C_REP        0xB0            // Instruction with REPxx prefix
#define   C_PRI        0xC0            // Privileged instruction
#define   C_DAT        0xD0            // Data (address) doubleword
#define   C_NOW        0xE0            // 3DNow! instruction
#define   C_BAD        0xF0            // Unrecognized command
#define C_RARE         0x08            // Rare command, seldom used in programs
#define C_SIZEMASK     0x07            // MMX data size or special flag
#define   C_EXPL       0x01            // (non-MMX) Specify explicit memory size

#define C_DANGER95     0x01            // Command is dangerous under Win95/98
#define C_DANGER       0x03            // Command is dangerous everywhere
#define C_DANGERLOCK   0x07            // Dangerous with LOCK prefix

#define DEC_TYPEMASK   0x1F            // Type of memory byte
#define   DEC_UNKNOWN  0x00            // Unknown type
#define   DEC_BYTE     0x01            // Accessed as byte
#define   DEC_WORD     0x02            // Accessed as short
#define   DEC_NEXTDATA 0x03            // Subsequent byte of code or data
#define   DEC_DWORD    0x04            // Accessed as long
#define   DEC_FLOAT4   0x05            // Accessed as float
#define   DEC_FWORD    0x06            // Accessed as descriptor/long pointer
#define   DEC_FLOAT8   0x07            // Accessed as double
#define   DEC_QWORD    0x08            // Accessed as 8-byte integer
#define   DEC_FLOAT10  0x09            // Accessed as long double
#define   DEC_TBYTE    0x0A            // Accessed as 10-byte integer
#define   DEC_STRING   0x0B            // Zero-terminated ASCII string
#define   DEC_UNICODE  0x0C            // Zero-terminated UNICODE string
#define   DEC_3DNOW    0x0D            // Accessed as 3Dnow operand
#define   DEC_BYTESW   0x11            // Accessed as byte index to switch
#define   DEC_NEXTCODE 0x13            // Subsequent byte of command
#define   DEC_COMMAND  0x1D            // First byte of command
#define   DEC_JMPDEST  0x1E            // Jump destination
#define   DEC_CALLDEST 0x1F            // Call (and maybe jump) destination
#define DEC_PROCMASK   0x60            // Procedure analysis
#define   DEC_PROC     0x20            // Start of procedure
#define   DEC_PBODY    0x40            // Body of procedure
#define   DEC_PEND     0x60            // End of procedure
#define DEC_CHECKED    0x80            // Byte was analysed

#define DECR_TYPEMASK  0x3F            // Type of register or memory
#define   DECR_BYTE    0x21            // Byte register
#define   DECR_WORD    0x22            // Short integer register
#define   DECR_DWORD   0x24            // Long integer register
#define   DECR_QWORD   0x28            // MMX register
#define   DECR_FLOAT10 0x29            // Floating-point register
#define   DECR_SEG     0x2A            // Segment register
#define   DECR_3DNOW   0x2D            // 3Dnow! register
#define DECR_ISREG     0x20            // Mask to check that operand is register

#define DISASM_SIZE    0               // Determine command size only
#define DISASM_DATA    1               // Determine size and analysis data
#define DISASM_FILE    3               // Disassembly, no symbols
#define DISASM_CODE    4               // Full disassembly

// Warnings issued by Disasm():
#define DAW_FARADDR    0x0001          // Command is a far jump, call or return
#define DAW_SEGMENT    0x0002          // Command loads segment register
#define DAW_PRIV       0x0004          // Privileged command
#define DAW_IO         0x0008          // I/O command
#define DAW_SHIFT      0x0010          // Shift constant out of range 1..31
#define DAW_PREFIX     0x0020          // Superfluous prefix
#define DAW_LOCK       0x0040          // Command has LOCK prefix
#define DAW_STACK      0x0080          // Unaligned stack operation
#define DAW_DANGER95   0x1000          // May mess up Win95 if executed
#define DAW_DANGEROUS  0x3000          // May mess up any OS if executed
//bughoho 操作数类型
enum Optype
{
	Imm,
	Reg,
	Mem,
	Seg,
};
typedef struct t_disasm
{              // Results of disassembling
	ulong          ip;                   // Instrucion pointer
	char           dump[TEXTLEN];        // Hexadecimal dump of the command
	char           result[TEXTLEN];      // Disassembled command
	char           comment[TEXTLEN];     // Brief comment
	int            cmdtype;              // One of C_xxx
	int            memtype;              // Type of addressed variable in memory
	int            nprefix;              // Number of prefixes
	int            indexed;              // Address contains register(s)
	ulong          jmpconst;             // Constant jump address
	ulong          jmptable;             // Possible address of switch table
	ulong          adrconst;             // [edx+100] '100' 地址部分的常量 Constant part of address
	ulong          immconst;             // Immediate constant
	int            zeroconst;            // Whether contains zero constant
	int            fixupoffset;          // Possible offset of 32-bit fixups
	int            fixupsize;            // Possible total size of fixups or 0
	int            error;                // Error while disassembling command
	int            warnings;             // Combination of DAW_xxx

	//bughoho new
	BYTE		   hexcode[TEXTLEN];	 //2进制码
	int			   codelen;				 //长度

	int			   optype[3];				 // 操作数类型
	char		   vm_name[TEXTLEN];	 // 声称VM对应的Handler名称
	int			   is3dnow;				 // 3dnow函数
	int			   segment;				 // 段前缀
	int			   reg[3];				 // 3寄存器(假设操作数为寄存器)
	int			   segreg;				 // 段寄存器(假设操作数为段寄存器)
	int			   addrreg1;			 // 内存地址部分的第1个不带比例的寄存器
	int			   addrreg2;			 // 内存地址部分的第2个带比例的寄存器
	int			   regsscale;			 // 比例:1,2,4,8
	//还有一个adrconst已经定义,根据正负号来定义加减
	int			   memsize[3];			 // 操作数长度
	bool		   highbit[3];			 // 当是8位指令时并且为高位时(ah bh ch dh)为1
} t_disasm;

//汇编Code命令结构
struct CodeNode
{
	t_disasm	disasm;
	BOOL		IsJmcBeSideType;//跳到外部的指令
	BOOL		IsJmcUndefineType;//跳到代码内不明确的地址
	BOOL		IsJmcDynamicType;//动态跳转类型
	BOOL		IsJmcType;//跳转
	BOOL		IsJmcFromType;//从其他地方跳转过来的代码
	BOOL		IsJmcNextType;//JMC和CALL之后的代码
	BOOL		IsCallType;//调用子函数
	CodeNode()
	{
		memset(this,0,sizeof(CodeNode));
	}
};

typedef struct t_asmmodel
{            // Model to search for assembler command
	char           code[MAXCMDSIZE];     // Binary code
	char           mask[MAXCMDSIZE];     // Mask for binary code (0: bit ignored)
	int            length;               // Length of code, bytes (0: empty)
	int            jmpsize;              // Offset size if relative jump
	int            jmpoffset;            // Offset relative to IP
	int            jmppos;               // Position of jump offset in command
} t_asmmodel;

odunique int       ideal;                // Force IDEAL decoding mode
odunique int       lowercase;            // Force lowercase display
odunique int       tabarguments;         // Tab between mnemonic and arguments
odunique int       extraspace;           // Extra space between arguments
odunique int       putdefseg;            // Display default segments in listing
odunique int       showmemsize;          // Always show memory size
odunique int       shownear;             // Show NEAR modifiers
odunique int       shortstringcmds;      // Use short form of string commands
odunique int       sizesens;             // How to decode size-sensitive mnemonics
odunique int       symbolic;             // Show symbolic addresses in disasm
odunique int       farcalls;             // Accept far calls, returns & addresses
odunique int       decodevxd;            // Decode VxD calls (Win95/98)
odunique int       privileged;           // Accept privileged commands
odunique int       iocommand;            // Accept I/O commands
odunique int       badshift;             // Accept shift out of range 1..31
odunique int       extraprefix;          // Accept superfluous prefixes
odunique int       lockedbus;            // Accept LOCK prefixes
odunique int       stackalign;           // Accept unaligned stack operations
odunique int       iswindowsnt;          // When checking for dangers, assume NT
//bughoho
odunique int	   stoperand;			 //当前的循环

int    Assemble(char *cmd,ulong ip,t_asmmodel *model,int attempt,
				int constsize,char *errtext);
int    Checkcondition(int code,ulong flags);
int    Decodeaddress(ulong addr,char *symb,int nsymb,char *comment);
ulong  Disasm(char *src,ulong srcsize,ulong srcip,
			  t_disasm *disasm,int disasmmode);
ulong  Disassembleback(char *block,ulong base,ulong size,ulong ip,int n);
ulong  Disassembleforward(char *block,ulong base,ulong size,ulong ip,int n);
int    Isfilling(ulong addr,char *data,ulong size,ulong align);
int    Print3dnow(char *s,char *f);
int    Printfloat10(char *s,long double ext);
int    Printfloat4(char *s,float f);
int    Printfloat8(char *s,double d);
  