#include "stdafx.h"
#include "asm/disasm.h"
#include "vmdisasm.h"

// 为一个索引数组随机排序.
void RandListIdx(int* idx,int cout)
{
	BOOL* IsUsed = new BOOL[cout];
	memset(IsUsed,FALSE,sizeof(BOOL)*cout);
	srand((unsigned)time(NULL));
	while(TRUE)
	{
		BOOL NotComplete = FALSE;
		for(int i = 0; i < cout; i++ )
		{
			if( !IsUsed[i] )
			{
				NotComplete = TRUE;
				break;
			}
		}
		if( !NotComplete )//全部完成
			return;
		int nrand = rand() % cout;
		//if( nrand == 0)//6.9.不能为0,0留给一个NULL的空间
		//	break;
		BOOL NoSame = FALSE;//默认为没有相同的
		for( int i = 0; i < cout; i++ )
		{
			if( IsUsed[i] && nrand == idx[i] )//如果分配了并且生成的数相等
			{
				NoSame = TRUE;
				break;
			}
		}
		if( NoSame )//如果有相同的
			continue;
		for( int i = 0; i < cout; i++ )
		{
			if( !IsUsed[i]  )//如果没有分配
			{
				idx[i] = nrand;
				IsUsed[i] = TRUE;//设为已分配
				break;
			}
		}
	}
	delete[] IsUsed;IsUsed = NULL;
}
