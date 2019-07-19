/*-----------------------------------------------------------------------
第13章  Hook技术
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/


#include <windows.h>
#include <stdio.h>

#define STATUS_FAILED (0)

LONG WINAPI TargetFun(
	IN LONG ParameterIn,//传入的参数
	OUT PVOID ParameterOut
	);

typedef LONG 
(WINAPI *PFN_TargetFun)(
	IN LONG ParameterIn,//传入的参数
	OUT PVOID ParameterOut
	);

BOOL CheckParameters(LONG ParameterIn);
BOOL CheckResult(PVOID ParameterOut);
VOID CleanupResult(PVOID ParameterOut);

PFN_TargetFun pOriginalFun = NULL ;

int main(int argc, char* argv[])
{
	return 0 ;
}

LONG WINAPI TargetFun(
	IN LONG ParameterIn,//传入的参数
	OUT PVOID ParameterOut
	)
{
	return 0 ;
}

BOOL CheckParameters(LONG ParameterIn)
{
	return TRUE;
}

BOOL CheckResult(PVOID ParameterOut)
{
	return TRUE;
}

VOID CleanupResult(PVOID ParameterOut)
{
	
}

LONG WINAPI DetourFun(
	IN LONG ParameterIn,	//传入的参数
	OUT PVOID ParameterOut	//传出的返回结果
	)
{
	LONG result = STATUS_FAILED ;
	if (!CheckParameters(ParameterIn))
	{
		//参数检查不通过，拒绝此次调用，直接返回失败
		return STATUS_FAILED;
	}
	else
	{
		//检查通过，调用原函数
		result = pOriginalFun(ParameterIn,ParameterOut);
		if (result == STATUS_FAILED)
		{
			//调用不成功，不再理会，直接返回原结果
			return result;
		}
		else
		{
			//调用成功，对结果进行检查
			if (!CheckResult(ParameterOut))
			{
				//结果不通过，清理调用带来的影响，并返回失败
				//清理工作主要是释放内存、关闭句柄等
				CleanupResult(ParameterOut);
				return STATUS_FAILED;
			}
			else
			{
				//结果检查通过，放行并返回原结果
				return result;
			}
		}
	}
}