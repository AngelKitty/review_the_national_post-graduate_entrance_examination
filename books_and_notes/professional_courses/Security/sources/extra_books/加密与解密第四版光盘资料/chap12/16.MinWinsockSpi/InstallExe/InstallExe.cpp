// InstallExe.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\common\xinstall.h"

void GetPath(OUT TCHAR *sPath) 
{
	TCHAR sFilename[MAX_PATH];
	TCHAR sDrive[_MAX_DRIVE];
	TCHAR sDir[_MAX_DIR];
	TCHAR sFname[_MAX_FNAME];
	TCHAR sExt[_MAX_EXT];

	GetModuleFileName(NULL, sFilename, _MAX_PATH);
	
	_tsplitpath(sFilename, sDrive, sDir, sFname, sExt);

	_tcscpy(sPath, sDrive);
	_tcscat(sPath, sDir);

	if(sPath[_tcslen(sPath) - 1] != _T('\\'))
		_tcscat(sPath, _T("\\"));
}  

void PrintReturnCode()
{
	printf("\n");
	printf("	0	Success.\n");
	printf("	-801	The TcpIpDog service provider not installed.\n");
	printf("	-802	The TcpIpDog service provider already installed.\n");
	printf("	-803	Failed to open registry.\n");
	printf("	-804	Failed to save the path of TcpIpDog service provider.\n");
	printf("	-805	Failed to read the registry value.\n");
	printf("	-806	Failed to create the key in the registry.\n");
	printf("	-807	Failed to set the value of the key in the registry.\n");
	printf("	-808	Failed to delete the key of registry.\n");
}

int main(int argc, char* argv[])
{
	__try
	{
		if(argc <= 1 || argc >= 3)
		{
			printf("Install [-install | -remove]\n");
			printf("  -install : used to install tcpipdog service provider.\n");
			printf("  -remove  : used to remove the tcpipdog service provider.\n");
			return 0;
		}

		CXInstall	m_Install;

		if(_tcscmp(argv[1], _T("-install")) == 0)
		{
			TCHAR sProvider[MAX_PATH];

			GetPath(sProvider);

			if(sProvider[0] == '\0')
			{
				printf("Can't find the application path.\n");
				return 0;
			}

			_tcscat(sProvider, XFILTER_SERVICE_DLL_NAME);

			if(_taccess(sProvider, 0) == -1)
			{
				printf("Can't find %s", sProvider);
				return 0;
			}

			printf("Install The TcpIpDog Service Provider...\n\n");
			printf("Return Code: %d\n", m_Install.InstallProvider(sProvider));
			PrintReturnCode();
			return 0;
		}
		else if(_tcscmp(argv[1], _T("-remove")) == 0)
		{
			printf("Remove the TcpIpDog service provider...\n\n");
			printf("Return Code: %d\n", m_Install.RemoveProvider());
			PrintReturnCode();
			return 0;
		}
		
		printf("Invaid parameter.\n");

		return 0;
	}
	__finally
	{
		printf("\nCopyRight (c) 2001 Xstudio Technology.\nAll Right Reserved.\n\n");
	}
	
	return 0;
}

