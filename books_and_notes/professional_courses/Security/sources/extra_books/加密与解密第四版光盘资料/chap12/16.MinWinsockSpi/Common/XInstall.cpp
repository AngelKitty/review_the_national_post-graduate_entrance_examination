//=============================================================================================
/*
	文件：		XInstall.cpp

	说明:
	---------------------------------------------------
		用来安装 Xfilter 个人防火墙。
	---------------------------------------------------

	工程：		Xfilter 个人防火墙
	作者：		朱雁辉，朱雁冰
	创建日期：	2001/08/28
	网址：		http://www.xfilt.com
	电子邮件：	xstudio@xfilt.com
	版权所有 (c) 2001-2002 X 工作室

	警告:
	---------------------------------------------------
		本电脑程序受著作权法的保护。未经授权，不能使用
	和修改本软件全部或部分源代码。凡擅自复制、盗用或散
	布此程序或部分程序或者有其它任何越权行为，将遭到民
	事赔偿及刑事的处罚，并将依法以最高刑罚进行追诉。
	
		凡通过合法途径购买本软件源代码的用户被默认授权
	可以在自己的程序中使用本软件的部分代码，但作者不对
	代码产生的任何后果负责。
	
		使用了本软件代码的程序只能以可执行文件形式发布，
	未经特别许可，不能将含有本软件源代码的源程序以任何
	形式发布。
	---------------------------------------------------	
*/
//=============================================================================================

#include "stdafx.h"
#include "XInstall.h"

BOOL CXInstall::IsWinsock2()
{
	WORD	wVersionRequested	= MAKEWORD(2, 0);
	WSADATA wsaData;
	
	if(WSAStartup(wVersionRequested, &wsaData) != 0)
		return FALSE;
 
	if (LOBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		return FALSE; 
	}

	return TRUE;
}

BOOL CXInstall::IsInstalled(TCHAR *sPathName)
{
	TCHAR tsPathName[MAX_PATH];

	if( ReadReg(REG_INSTALL_PATH_ITEM, 
				(BYTE*)tsPathName, 
				MAX_PATH, 
				HKEY_LOCAL_MACHINE, 
				REG_INSTALL_KEY, REG_SZ
				)
		)
		{
			if(sPathName != NULL)
				_tcscpy(sPathName, tsPathName);
			return TRUE;
		}

	return FALSE;
}

int CXInstall::InstallProvider(TCHAR *sPathName)
{
	if(IsInstalled())
		return XERR_PROVIDER_ALREADY_INSTALL;

	_tcscpy(m_sPathName, sPathName);

	int iRet;
	if((iRet = EnumHookKey()) != XERR_SUCCESS)
		return iRet;

	if(!SaveReg(
			REG_INSTALL_PATH_ITEM, 
			(BYTE*)sPathName, 
			_tcslen(sPathName),
			HKEY_LOCAL_MACHINE, 
			REG_INSTALL_KEY, 
			REG_SZ
			)
		)
		return XERR_PROVIDER_SAVE_PATH_FAILED;

	return XERR_SUCCESS;
}

BOOL CXInstall::RemoveProvider()
{
	int iRet = XERR_SUCCESS;

	if(!IsInstalled())
		return XERR_PROVIDER_NOT_INSTALL;

	if(iRet = EnumHookKey(TRUE) != XERR_SUCCESS)
		return iRet;

	if(!DeleteReg())
		return XERR_PROVIDER_REG_DELETE_FAILED;

	return XERR_SUCCESS;
}

//=============================================================================================
// 私有安装函数
//

int CXInstall::EnumHookKey(BOOL IsRemove)
{
	HKEY hkey = NULL;

	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, REG_PROTOCOL_CATALOG_KEY, 0, KEY_READ, &hkey) != ERROR_SUCCESS)
		return XERR_PROVIDER_OPEN_REG_FAILED;

	__try
	{
		TCHAR sSubKey[MAX_PATH];
		DWORD dwIndex	= 0;
		int	  iRet		= 0;

		while(RegEnumKey(hkey, dwIndex, sSubKey, MAX_PATH) == ERROR_SUCCESS)
		{
			if((iRet = SaveHookKey(hkey, sSubKey, IsRemove)) != XERR_SUCCESS)
				return iRet;

			dwIndex ++;
		}
	}
	__finally
	{
		RegCloseKey(hkey);
	}

	return XERR_SUCCESS;
}

int CXInstall::SaveHookKey(HKEY hkey, LPCTSTR sSubKey, BOOL IsRemove)
{
	HKEY	hSubKey		= NULL;
	BYTE	ItemValue	  [MAX_PROTOCOL_CATALOG_LENTH];
	DWORD	ItemSize	= MAX_PROTOCOL_CATALOG_LENTH;

	if(RegOpenKeyEx(hkey, sSubKey, 0, KEY_ALL_ACCESS, &hSubKey) != ERROR_SUCCESS)
		return XERR_PROVIDER_OPEN_REG_FAILED;

	__try
	{
		if(RegQueryValueEx(hSubKey, REG_PROTOCOL_CATALOG_ITEM, 0, NULL, ItemValue, &ItemSize) != ERROR_SUCCESS
			|| (ItemSize != MAX_PROTOCOL_CATALOG_LENTH))
			return XERR_PROVIDER_READ_VALUE_FAILED;

		WSAPROTOCOL_INFOW *mProtocolInfo = (WSAPROTOCOL_INFOW*)(ItemValue + MAX_PATH);
		
		if(mProtocolInfo->ProtocolChain.ChainLen == 1)
		{
			TCHAR sItem[21];
			_stprintf(sItem, _T("%u"), mProtocolInfo->dwCatalogEntryId);
			if(!IsRemove)
			{
				if(!SaveReg(
						sItem, 
						ItemValue,
						_tcslen((TCHAR*)ItemValue), 
						HKEY_LOCAL_MACHINE, 
						REG_INSTALL_KEY, 
						REG_SZ
						)
					)
					return XERR_PROVIDER_CREATE_ITEM_FAILED;

				_tcscpy((TCHAR*)ItemValue, m_sPathName);

				if(RegSetValueEx(hSubKey, REG_PROTOCOL_CATALOG_ITEM, 0, REG_BINARY, ItemValue, ItemSize) != ERROR_SUCCESS)
					return XERR_PROVIDER_SET_VALUE_FAILED;
			}
			else
			{
				TCHAR sProvider[MAX_PATH];
				
				int iRet = ReadReg(
								sItem, 
								(BYTE*)sProvider, 
								MAX_PATH, 
								HKEY_LOCAL_MACHINE, 
								REG_INSTALL_KEY, REG_SZ
								);
				_tcscpy((TCHAR*)ItemValue, sProvider);
				iRet = RegSetValueEx(hSubKey, REG_PROTOCOL_CATALOG_ITEM, 0, REG_BINARY, ItemValue, ItemSize);
			}
		}
	}
	__finally
	{
		RegCloseKey(hSubKey);
	}

	return XERR_SUCCESS;
}

//=============================================================================================
// 注册表操作函数
//

BOOL CXInstall::ReadReg(
	TCHAR	*sKey, 
	BYTE	*pBuffer,	
	DWORD	dwBufSize,
	HKEY	hkey, 
	TCHAR	*sSubKey, 
	DWORD	ulType
)
{
	HKEY	hSubkey;

	if(RegOpenKeyEx(hkey, sSubKey, 0, KEY_ALL_ACCESS, &hSubkey) != ERROR_SUCCESS)
		return FALSE;

	__try
	{
		DWORD	dwType;

		if (RegQueryValueEx(hSubkey, sKey, 0, &dwType, pBuffer, &dwBufSize) == ERROR_SUCCESS
			&& dwType == ulType)
			return TRUE;
	}
	__finally
	{
		RegCloseKey(hSubkey);
	}

	return FALSE;
}

BOOL CXInstall::SaveReg(
	TCHAR	*sKey, 
	BYTE	*pBuffer,
	DWORD	dwBufSize,
	HKEY	hkey, 
	TCHAR	*sSubKey, 
	DWORD	ulType
)
{
	HKEY	hSubkey;
	DWORD	dwDisposition;

	if (RegCreateKeyEx(hkey, sSubKey, 0, NULL, REG_OPTION_NON_VOLATILE
		, KEY_ALL_ACCESS, NULL, &hSubkey, &dwDisposition) != ERROR_SUCCESS)
		return FALSE;

	if (RegSetValueEx(hSubkey, sKey, 0, ulType, pBuffer, dwBufSize) != ERROR_SUCCESS)
	{
		RegCloseKey(hSubkey);
		return FALSE;
	}

	RegCloseKey(hSubkey);

	return TRUE;
}

BOOL CXInstall::DeleteReg(
	HKEY	hkey,
	TCHAR	*sSubKey, 
	TCHAR	*sItem
)
{
	if(hkey == NULL || sSubKey == NULL)
		return FALSE;

	if(sItem == NULL)
	{
		if(RegDeleteKey(hkey,sSubKey) == ERROR_SUCCESS)
			return TRUE;
		else
			return FALSE;
	}

	HKEY	hSubKey;

	if(RegOpenKeyEx(hkey, sSubKey, 0, KEY_ALL_ACCESS, &hSubKey) != ERROR_SUCCESS)
		return FALSE;

	__try
	{
		if(RegDeleteValue(hSubKey, sItem) == ERROR_SUCCESS)
			return TRUE;
	}
	__finally
	{
		RegCloseKey(hSubKey);
	}

	return FALSE;
}


