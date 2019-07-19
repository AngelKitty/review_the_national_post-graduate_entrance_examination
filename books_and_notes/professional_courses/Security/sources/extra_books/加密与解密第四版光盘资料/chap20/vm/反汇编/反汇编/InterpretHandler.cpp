#include "StdAfx.h"
#include "InterpretHandler.h"
#include "asm/disasm.h"

CInterpretHandler::CInterpretHandler(void)
{
}

CInterpretHandler::~CInterpretHandler(void)
{
}
// 获得寄存器的偏移
// 16和32位和8位低字节均使用32位enum，8位高字节使用20以上的数
int CInterpretHandler::GetRegisterOffset(int RegType)
{
	if( RegType < 0 )
		return RegType;
	int offset = m_RegisterIdx[RegType]*4;
	if ( RegType >= 20 )
		offset++;//8位高字节的地方
	return offset;
}
//初始化
BOOL CInterpretHandler::Init()
{
	RandListIdx(m_RegisterIdx,REGCOUNT);	//打乱寄存器偏移的索引

	return TRUE;
}

//设置参数
void CInterpretHandler::SetArg(VMTable* table,char* asmtext,int len)
{
	for(int i = 0; i < 3; i++)
	{
		if( i >= table->OperandNum )
			break;
		sprintf_s(asmtext,len,"%smov %s,[esp+%02x]\n",asmtext,ArgReg[i],i*4);//将堆栈中的数据交给参数寄存器
	}
}
//恢复参数
void CInterpretHandler::RestoreArg(VMTable* table,char* asmtext,int len)
{
	//将结果保存回堆栈
	for(int i = 0; i < 3; i++)
	{
		if( i >= table->OperandNum )
			break;
		sprintf_s(asmtext,len,"%smov [esp+%02x],%s\n",asmtext,i*4,ArgReg[i]);
	}
}
//恢复标志
void CInterpretHandler::RestoreFlag(char* asmtext,int len)
{
	sprintf_s(asmtext,len,"%sPush [edi+0x%02x]\nPopfd\n",asmtext,GetRegisterOffset(RT_EFlag));//恢复标志
}
//保存标志
void CInterpretHandler::SaveFlag(char* asmtext,int len)
{
	sprintf_s(asmtext,len,"%sPushfd\nPop [edi+0x%02x]\n",asmtext,GetRegisterOffset(RT_EFlag));//保存标志
}
//根据结构声称ASM字符串
BOOL CInterpretHandler::InterpretASMStr(VMTable* table,char* asmtext,int len)
{
	if( strcmp(table->VMInstrName,"") == 0 || asmtext == NULL ) return FALSE;

	memset(asmtext,0,len);

	SetArg(table,asmtext,len);//设置参数

	//恢复需要的寄存器
	for(int i = 0; i < 4; i++)
	{
		if( table->NeedReg[i] != NONE && table->NeedReg[i] < 14 )
		{
			sprintf_s(asmtext,len,"%smov %s,[edi+0x%02x]\n",asmtext,
				vregname[2][table->NeedReg[i]],GetRegisterOffset(table->NeedReg[i]));
		}
	}

	BOOL After = FALSE;
	if( _stricmp(table->strInstruction,"vBegin") == 0 )
	{
		After = InterpretvBegin(table,asmtext,len);
	}
	else if( _stricmp(table->strInstruction,"vtoReal") == 0 )
	{
		After = InterpretvtoReal(table,asmtext,len);
	}
///////////////////////////////////////////////////////////////////////////////
	else if( _stricmp(table->strInstruction,"push") == 0 )
	{
		After = InterpretPush(table,asmtext,len);
	}
	else if( _stricmp(table->strInstruction,"pop") == 0 )
	{
		After = InterpretPop(table,asmtext,len);
	}
	else if( _stricmp(table->strInstruction,"pushfd") == 0 )
	{
		After = InterpretPushfd(table,asmtext,len);
	}
	else if( _stricmp(table->strInstruction,"popfd") == 0 )
	{
		After = InterpretPopfd(table,asmtext,len);
	}
	else if( _stricmp(table->strInstruction,"pushad") == 0 )
	{
		After = InterpretPushad(table,asmtext,len);
	}
	else if( _stricmp(table->strInstruction,"popad") == 0 )
	{
		After = InterpretPopad(table,asmtext,len);
	}
	else if( _stricmp(table->strInstruction,"enter") == 0 )
	{
		After = InterpretEnter(table,asmtext,len);
	}
	else if( _stricmp(table->strInstruction,"leave") == 0 )
	{
		After = InterpretLeave(table,asmtext,len);
	}
/////////////////////////////////////////////////////////////////////////////////
	else if( _stricmp(table->strInstruction,"jcxz") == 0 )
	{
		RestoreFlag(asmtext,len);
		After = InterpretJCXZ(table,asmtext,len);
	}
	else if( _stricmp(table->strInstruction,"jmp") == 0 )
	{
		After = InterpretJMP(table,asmtext,len);
	}
	else if( table->strInstruction[0] == 'J' || table->strInstruction[0] == 'j' )//不是jcxz和jmp,就是条件跳转了
	{
		RestoreFlag(asmtext,len);
		After = InterpretJCC(table,asmtext,len);
	}
	else if( _stricmp(table->strInstruction,"loope") == 0 )
	{
		RestoreFlag(asmtext,len);
		After = InterpretLoope(table,asmtext,len);
		SaveFlag(asmtext,len);
	}
	else if( _stricmp(table->strInstruction,"retn") == 0 )
	{
		After = InterpretRetn(table,asmtext,len);
	}
	else if( _stricmp(table->strInstruction,"call") == 0 )
	{
		After = InterpretCall(table,asmtext,len);
	}
	else if( _stricmp(table->strInstruction,"SaveEsp") == 0 )
	{
		After = InterpretSaveEsp(table,asmtext,len);
	}
	else if( _stricmp(table->strInstruction,"RestoreEsp") == 0 )
	{
		After = InterpretRestoreEsp(table,asmtext,len);
	}
	else
	{
		After = CommonInstruction(table,asmtext,len);
	}

	//sprintf_s(asmtext,len,"%smov [edi+%02x],ebp\n",asmtext,GetRegisterOffset(RT_Esp));//将ebp(即真正esp)保存到esp的地址
	//保存寄存器的值
	for(int i = 0; i < 3; i++)
	{
		if( table->SaveReg[i] != NONE )
		{
			sprintf_s(asmtext,len,"%smov [edi+0x%02x],%s\n",asmtext,
				GetRegisterOffset(table->NeedReg[i]),vregname[2][table->NeedReg[i]]);
		}
	}

	if( After )
	{
		RestoreArg(table,asmtext,len);
	}

	_strupr_s(asmtext,ASMTEXTLEN);
	return TRUE;
}
int GetScalestr(int bit,OUT char* scalestr)
{
	int sizeidx = 0;
	if( bit == 8 )
	{
		sizeidx = 0;
		strcpy_s(scalestr,6,"byte");
	}
	else if( bit == 16 )
	{
		sizeidx = 1;
		strcpy_s(scalestr,6,"word");
	}
	else if( bit == 32 )
	{
		sizeidx = 2;
		strcpy_s(scalestr,6,"dword");
	}
	return sizeidx;
}

BOOL CInterpretHandler::CommonInstruction(VMTable* table,char* asmtext,int len)
{
	char scalestr[6] = {0};
	int sizeidx = 0;

	char stroperand[1024] = {0};
	for(int i = 0; i < 3; i++)
	{
		if( i >= table->OperandNum )
			break;
		sizeidx = GetScalestr(table->bitnum[i],scalestr);
		if( table->optype[i] == MEMTYPE )//内存数
		{	//操作数顺序eax,ecx,edx
			sprintf_s(stroperand,1024,"%s%s ptr %s[%s],",stroperand,scalestr,GetSegStr(table->Segment),vregname[2][i]);//得到参数
		}
		else//立即数和寄存器
		{
			sprintf_s(stroperand,1024,"%s%s,",stroperand,vregname[sizeidx][i]);//得到参数
		}
	}
	if( table->OperandNum > 0)
		stroperand[strlen(stroperand)-1] = '\0';//去掉逗号

	RestoreFlag(asmtext,len);
	sprintf_s(asmtext,len,"%s%s %s\n",asmtext,table->strInstruction,stroperand);//真正执行的指令
	SaveFlag(asmtext,len);
	return TRUE;
}

// 获得段前缀
char* CInterpretHandler::GetSegStr(int Segment)
{
	static char segstr[10] = "";
	memset(segstr,0,10);
	if( Segment == SEG_FS )
		strcpy_s(segstr,10,"fs:");
	else if( Segment == SEG_GS )
		strcpy_s(segstr,10,"gs:");
	return segstr;
}
// 首先执行的指令
BOOL CInterpretHandler::InterpretvBegin(VMTable* table,char* asmtext,int len)
{
	//popfd
	//pop ebp
	//pop edi
	//pop esi
	//pop edx
	//pop ecx
	//pop eax
	int s_reg[8] = {RT_EFlag,RT_Ebp,RT_Edi,RT_Esi,RT_Edx,RT_Ecx,RT_Ebx,RT_Eax};

	// 弹出寄存器
	for(int i = 0; i < 8;i++)
	{
		sprintf_s(asmtext,len,"%smov eax,dword ptr [ebp]\n",asmtext);
		sprintf_s(asmtext,len,"%smov [edi+%02X],eax\n",asmtext,GetRegisterOffset(s_reg[i]));
		
		sprintf_s(asmtext,len,"%sadd ebp,4\n",asmtext);
	}
	// 释放伪地址堆栈
	sprintf_s(asmtext,len,"%sadd ebp,4\n",asmtext);
	sprintf_s(asmtext,len,"%smov [edi+%02x],ebp\n",asmtext,GetRegisterOffset(RT_Esp));//将ebp(即真正esp)保存到esp的地址

	return FALSE;
}

// 跳转到真实指令
BOOL CInterpretHandler::InterpretvtoReal(VMTable* table,char* asmtext,int len)
{
	int s_reg[9] = {RT_Esp,RT_EFlag,RT_Ebp,RT_Edi,RT_Esi,RT_Edx,RT_Ecx,RT_Ebx,RT_Eax};
	//之前有一个pushimm32 xxxx地址

	//把这个值放如真实堆栈
	sprintf_s(asmtext,len,"%smov eax,dword ptr [esi]\n",asmtext);//释放4字节堆栈
	sprintf_s(asmtext,len,"%sadd esi,4\n",asmtext);
	sprintf_s(asmtext,len,"%ssub ebp,4\n",asmtext);
	sprintf_s(asmtext,len,"%smov dword ptr [ebp],eax\n",asmtext);//eax,第1个参数

	sprintf_s(asmtext,len,"%smov [edi+%02x],ebp\n",asmtext,GetRegisterOffset(RT_Esp));//将ebp(即真正esp)保存到esp的地址

	// 弹出寄存器
	for(int i = 0; i < 9;i++)
	{
		sprintf_s(asmtext,len,"%spush [edi+%02X]\n",asmtext,GetRegisterOffset(s_reg[i]));
	}
	sprintf_s(asmtext,len,"%spop eax\n",asmtext);
	sprintf_s(asmtext,len,"%spop ebx\n",asmtext);
	sprintf_s(asmtext,len,"%spop ecx\n",asmtext);
	sprintf_s(asmtext,len,"%spop edx\n",asmtext);
	sprintf_s(asmtext,len,"%spop esi\n",asmtext);
	sprintf_s(asmtext,len,"%spop edi\n",asmtext);
	sprintf_s(asmtext,len,"%spop ebp\n",asmtext);
	sprintf_s(asmtext,len,"%spopfd\n",asmtext);
	sprintf_s(asmtext,len,"%spop esp\n",asmtext);
	//返回
	sprintf_s(asmtext,len,"%sretn\n",asmtext);
	return FALSE;
}
//		mov		eax,dword ptr [esp]
//		sub		ebp,4
//		mov		eax,[eax]
//		mov		word ptr [ebp],ax
//解释push
BOOL CInterpretHandler::InterpretPush(VMTable* table,char* asmtext,int len)
{
	char scalestr[6] = {0};
	int sizeidx = 0;
	if( table->bitnum[0] == 8 )
	{
		sizeidx = 0;
		strcpy_s(scalestr,6,"byte");
	}
	else if( table->bitnum[0] == 16 )
	{
		sizeidx = 1;
		strcpy_s(scalestr,6,"word");
	}
	else if( table->bitnum[0] == 32 )
	{
		sizeidx = 2;
		strcpy_s(scalestr,6,"dword");
	}

	sprintf_s(asmtext,len,"%ssub ebp,%d\n",asmtext,table->bitnum[0] / 8);//从堆栈中腾出空间
	if( table->optype[0] == MEMTYPE )
	{
		sprintf_s(asmtext,len,"%smov %s,%s ptr %s[eax]\n",asmtext,vregname[sizeidx][RT_Eax],scalestr,GetSegStr(table->Segment));//得到内存数的值
	}
	sprintf_s(asmtext,len,"%smov %s ptr [ebp],%s\n",asmtext,scalestr,vregname[sizeidx][RT_Eax]);
	return TRUE;
}
//		mov		eax,dword ptr [ebp]
//		add		ebp,4
//		mov		eax,[eax]
//解释pop
BOOL CInterpretHandler::InterpretPop(VMTable* table,char* asmtext,int len)
{
	char scalestr[6] = {0};
	int sizeidx = 0;
	if( table->bitnum[0] == 8 )
	{
		sizeidx = 0;
		strcpy_s(scalestr,6,"byte");
	}
	else if( table->bitnum[0] == 16 )
	{
		sizeidx = 1;
		strcpy_s(scalestr,6,"word");
	}
	else if( table->bitnum[0] == 32 )
	{
		sizeidx = 2;
		strcpy_s(scalestr,6,"dword");
	}
	sprintf_s(asmtext,len,"%smov %s,%s ptr [ebp]\n",asmtext,vregname[sizeidx][RT_Ecx],scalestr);//得到堆栈的值
	sprintf_s(asmtext,len,"%sadd ebp,%d\n",asmtext,table->bitnum[0] / 8);//释放4字节堆栈
	if( table->optype[0] == MEMTYPE )
	{
		sprintf_s(asmtext,len,"%smov %s,%s ptr %s[eax]\n",asmtext,vregname[sizeidx][RT_Eax],scalestr,GetSegStr(table->Segment));//得到内存数的值
	}
	sprintf_s(asmtext,len,"%smov eax,ecx\n",asmtext);//将值给mem(只有mem，因为无论是reg还是mem在vm中都是内存地址)
	return TRUE;
}

//解释pushfd
BOOL CInterpretHandler::InterpretPushfd(VMTable* table,char* asmtext,int len)
{
	sprintf_s(asmtext,len,"%ssub ebp,4\n",asmtext);//从堆栈中取得参数
	sprintf_s(asmtext,len,"%smov eax,[edi+%02x]\n",asmtext,GetRegisterOffset(RT_EFlag));//得到EFL
	sprintf_s(asmtext,len,"%smov dword ptr [ebp],eax\n",asmtext);//赋值EFL寄存器
	return TRUE;
}
//解释popfd
BOOL CInterpretHandler::InterpretPopfd(VMTable* table,char* asmtext,int len)
{
	sprintf_s(asmtext,len,"%smov eax,dword ptr [ebp]\n",asmtext);//赋值EFL寄存器
	sprintf_s(asmtext,len,"%sadd ebp,4\n",asmtext);//释放堆栈
	sprintf_s(asmtext,len,"%smov [edi+%02x],eax\n",asmtext,GetRegisterOffset(RT_EFlag));//设置EFL
	return TRUE;
}


// 解释pushad
BOOL CInterpretHandler::InterpretPushad(VMTable* table,char* asmtext,int len)
{
	for(int i = 0 ; i < 8;i++)
	{
		sprintf_s(asmtext,len,"%ssub ebp,4\n",asmtext);//从堆栈中取得参数
		sprintf_s(asmtext,len,"%smov eax,[edi+%02x]\n",asmtext,GetRegisterOffset(i));//i = enum RegType
		sprintf_s(asmtext,len,"%smov dword ptr [ebp],eax\n",asmtext);//push
	}
	return TRUE;
}

// 解释popad
BOOL CInterpretHandler::InterpretPopad(VMTable* table,char* asmtext,int len)
{
	for(int i = 8 ; i > 0;i--)
	{
		sprintf_s(asmtext,len,"%smov eax,dword ptr [ebp]\n",asmtext);//
		sprintf_s(asmtext,len,"%sadd ebp,4\n",asmtext);//释放堆栈
		sprintf_s(asmtext,len,"%smov [edi+%02x],eax\n",asmtext,GetRegisterOffset(i));//i = enum RegType
	}
	return TRUE;
}

// 解释enter
//push ebp
//mov ebp,esp
//sub esp,(L - 1) * 4 ; L > 0才有这步操作，用来存储嵌套的L - 1个子过程的栈框架指针（注意是指针）
//push ebp ; 当前过程的栈框架指针
//sub esp,N
BOOL CInterpretHandler::InterpretEnter(VMTable* table,char* asmtext,int len)
{
	//push ebp
	sprintf_s(asmtext,len,"%smov edx,[edi+%02x]\n",asmtext,GetRegisterOffset(RT_Ebp));//
	sprintf_s(asmtext,len,"%ssub ebp,4\n",asmtext);//
	sprintf_s(asmtext,len,"%smov dword ptr [ebp],edx\n",asmtext);//push ebp

	//mov ebp,esp
	sprintf_s(asmtext,len,"%smov edx,ebp\n",asmtext);//
	sprintf_s(asmtext,len,"%smov [edi+%02x],edx\n",asmtext,GetRegisterOffset(RT_Esp));//
	
	//sub esp,(L - 1) * 4 ; L > 0才有这步操作
	sprintf_s(asmtext,len,"%smov edx,ebp\n",asmtext);//
	sprintf_s(asmtext,len,"%slea ecx,[ecx*4]\n",asmtext);//ecx = 第2个参数
	sprintf_s(asmtext,len,"%ssub edx,ecx\n",asmtext);//不管是不是0都减去
	sprintf_s(asmtext,len,"%ssub edx,4\n",asmtext);//再减1个4
	sprintf_s(asmtext,len,"%stest ecx,ecx\n",asmtext);//测试是否为0
	sprintf_s(asmtext,len,"%scmovne ebp,edx\n",asmtext);//如果不为0就赋值减去后的结果

	//push ebp
	sprintf_s(asmtext,len,"%smov edx,[edi+%02x]\n",asmtext,GetRegisterOffset(RT_Ebp));//
	sprintf_s(asmtext,len,"%ssub ebp,4\n",asmtext);//
	sprintf_s(asmtext,len,"%smov dword ptr [ebp],edx\n",asmtext);//push ebp

	//sub esp,N
	sprintf_s(asmtext,len,"%ssub ebp,eax\n",asmtext);//eax,第1个参数
	return TRUE;
}

// 解释leave
// mov ebp,esp
// pop ebp
BOOL CInterpretHandler::InterpretLeave(VMTable* table,char* asmtext,int len)
{
	//mov ebp,esp
	sprintf_s(asmtext,len,"%smov edx,ebp\n",asmtext);//
	sprintf_s(asmtext,len,"%smov [edi+%02x],edx\n",asmtext,GetRegisterOffset(RT_Esp));//
	//pop ebp
	sprintf_s(asmtext,len,"%smov eax,[ebp]\n",asmtext);//
	sprintf_s(asmtext,len,"%sadd ebp,4\n",asmtext);//释放堆栈
	sprintf_s(asmtext,len,"%smov [edi+%02x],eax\n",asmtext,GetRegisterOffset(RT_Ebp));//
	return TRUE;
}
///////////////////////////////////////////////////
// 解释jmp
BOOL CInterpretHandler::InterpretJMP(VMTable* table,char* asmtext,int len)
{
	sprintf_s(asmtext,len,"%smov esi,eax\n",asmtext);//
	sprintf_s(asmtext,len,"%sadd esp,4\n",asmtext);//
	return FALSE;
}
// 解释jcxz\jecxz
BOOL CInterpretHandler::InterpretJCXZ(VMTable* table,char* asmtext,int len)
{
	sprintf_s(asmtext,len,"%stest ecx,ecx\n",asmtext);//
	sprintf_s(asmtext,len,"%sCMOVZ esi,eax\n",asmtext);//eax为第1个参数
	sprintf_s(asmtext,len,"%sadd esp,4\n",asmtext);//
	return FALSE;
}	
// 解释jcc
BOOL CInterpretHandler::InterpretJCC(VMTable* table,char* asmtext,int len)
{
	char strPostfix[16] = {0};//条件后缀
	strcpy_s(strPostfix,16,&table->strInstruction[1]);
	sprintf_s(asmtext,len,"%scmov%s esi,[esp]\n",asmtext,strPostfix);//
	sprintf_s(asmtext,len,"%sadd esp,4\n",asmtext);//
	return FALSE;
}
// 解释loope
BOOL CInterpretHandler::InterpretLoope(VMTable* table,char* asmtext,int len)
{
	sprintf_s(asmtext,len,"%spushfd\n",asmtext);//
	sprintf_s(asmtext,len,"%stest ecx,ecx\n",asmtext);//
	sprintf_s(asmtext,len,"%scmovne edx,eax\n",asmtext);//eax为第1个参数
	sprintf_s(asmtext,len,"%spopfd\n",asmtext);//
	sprintf_s(asmtext,len,"%scmovne edx, esi\n",asmtext);//
	sprintf_s(asmtext,len,"%scmove edx,eax\n",asmtext);//eax为第1个参数
	sprintf_s(asmtext,len,"%sadd ebp,4\n",asmtext);//释放堆栈
	return FALSE;
}


// 解释返回
BOOL CInterpretHandler::InterpretRetn(VMTable* table,char* asmtext,int len)
{
	int s_reg[9] = {RT_Esp,RT_EFlag,RT_Ebp,RT_Edi,RT_Esi,RT_Edx,RT_Ecx,RT_Ebx,RT_Eax};

	if( table->OperandNum == 1 )//retn xxx
	{
		sprintf_s(asmtext,len,"%smov edx,ebp\n",asmtext);
		sprintf_s(asmtext,len,"%sadd ebp,eax\n",asmtext);//retn xx,先放上去
		sprintf_s(asmtext,len,"%smov [ebp],edx\n",asmtext);//再PUSH进retn地址
		sprintf_s(asmtext,len,"%smov [edi+%02x],ebp\n",asmtext,GetRegisterOffset(RT_Esp));//将ebp(即真正esp)保存到esp的地址
	}
	// 弹出寄存器
	for(int i = 0; i < 9;i++)
	{
		sprintf_s(asmtext,len,"%spush [edi+%02X]\n",asmtext,GetRegisterOffset(s_reg[i]));
	}
	sprintf_s(asmtext,len,"%spop eax\n",asmtext);
	sprintf_s(asmtext,len,"%spop ebx\n",asmtext);
	sprintf_s(asmtext,len,"%spop ecx\n",asmtext);
	sprintf_s(asmtext,len,"%spop edx\n",asmtext);
	sprintf_s(asmtext,len,"%spop esi\n",asmtext);
	sprintf_s(asmtext,len,"%spop edi\n",asmtext);
	sprintf_s(asmtext,len,"%spop ebp\n",asmtext);
	sprintf_s(asmtext,len,"%spopfd\n",asmtext);
	sprintf_s(asmtext,len,"%spop esp\n",asmtext);

	//返回
	sprintf_s(asmtext,len,"%sretn\n",asmtext);
	return FALSE;
}
// 解释子调用
BOOL CInterpretHandler::InterpretCall(VMTable* table,char* asmtext,int len)
{
	int s_reg[9] = {RT_Esp,RT_EFlag,RT_Ebp,RT_Edi,RT_Esi,RT_Edx,RT_Ecx,RT_Ebx,RT_Eax};
	
	sprintf_s(asmtext,len,"%smov edx,[esi]\n",asmtext);
	sprintf_s(asmtext,len,"%ssub ebp,4\n",asmtext);//4字节空间
	sprintf_s(asmtext,len,"%smov [ebp],edx\n",asmtext);//返回地址
	sprintf_s(asmtext,len,"%ssub ebp,4\n",asmtext);//4字节空间
	if( table->optype[0] == MEMTYPE )
	{
		sprintf_s(asmtext,len,"%smov eax,dword ptr [eax]\n",asmtext);//得到内存数的值
	}
	sprintf_s(asmtext,len,"%smov [ebp],eax\n",asmtext);//CALL的地址 eax = 第一个操作数

	sprintf_s(asmtext,len,"%smov [edi+%02x],ebp\n",asmtext,GetRegisterOffset(RT_Esp));//将ebp(即真正esp)保存到esp的地址

	// 弹出寄存器
	for(int i = 0; i < 9;i++)
	{
		sprintf_s(asmtext,len,"%spush [edi+%02X]\n",asmtext,GetRegisterOffset(s_reg[i]));
	}
	sprintf_s(asmtext,len,"%spop eax\n",asmtext);
	sprintf_s(asmtext,len,"%spop ebx\n",asmtext);
	sprintf_s(asmtext,len,"%spop ecx\n",asmtext);
	sprintf_s(asmtext,len,"%spop edx\n",asmtext);
	sprintf_s(asmtext,len,"%spop esi\n",asmtext);
	sprintf_s(asmtext,len,"%spop edi\n",asmtext);
	sprintf_s(asmtext,len,"%spop ebp\n",asmtext);
	sprintf_s(asmtext,len,"%spopfd\n",asmtext);
	sprintf_s(asmtext,len,"%spop esp\n",asmtext);

	//返回
	sprintf_s(asmtext,len,"%sretn\n",asmtext);

	return FALSE;//不恢复了
}
// 解释保护堆栈Handler
BOOL CInterpretHandler::InterpretSaveEsp(VMTable* table,char* asmtext,int len)
{
	sprintf_s(asmtext,len,"%smov [edi+%02x],ebp\n",asmtext,GetRegisterOffset(RT_Esp));//将ebp(即真正esp)保存到esp的地址
	return FALSE;
}
// 解释恢复堆栈Handler
BOOL CInterpretHandler::InterpretRestoreEsp(VMTable* table,char* asmtext,int len)
{
	sprintf_s(asmtext,len,"%smov ebp,[edi+%02x]\n",asmtext,GetRegisterOffset(RT_Esp));//将vmesp的值恢复到ebp(即真正esp)
	return FALSE;
}  