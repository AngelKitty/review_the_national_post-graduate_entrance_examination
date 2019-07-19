//=============================================================================================
/*
	XInstall.h
	Install XFILTER Winscok 2 base service provider hook

	Project	: XFILTER 1.0 Personal Firewall
	Author	: Tony Zhu
	Create Date	: 2001/08/28
	Email	: xstudio@xfilt.com
	URL		: http://www.xfilt.com

	Copyright (c) 2001-2002 XStudio Technology.
	All Rights Reserved.

	WARNNING: 
*/
//=============================================================================================

#define XFILTER_SERVICE_DLL_NAME	_T("MinWinsockSpi.dll")
#define REG_INSTALL_KEY				_T("SYSTEM\\CurrentControlSet\\Services\\WinSock2\\Xstudio_Min_Winsock_Spi")
#define REG_INSTALL_PATH_ITEM		_T("PathName")
#define	REG_PROTOCOL_CATALOG_KEY	_T("SYSTEM\\CurrentControlSet\\Services\\WinSock2\\Parameters\\Protocol_Catalog9\\Catalog_Entries")
#define REG_PROTOCOL_CATALOG_ITEM	_T("PackedCatalogItem")

#define MAX_PROTOCOL_CATALOG_LENTH		sizeof(WSAPROTOCOL_INFOW) + MAX_PATH

#define	XERR_SUCCESS						0
#define XERR_PROVIDER_NOT_INSTALL			-801
#define XERR_PROVIDER_ALREADY_INSTALL		-802
#define XERR_PROVIDER_OPEN_REG_FAILED		-803
#define XERR_PROVIDER_SAVE_PATH_FAILED		-804
#define XERR_PROVIDER_READ_VALUE_FAILED		-805
#define XERR_PROVIDER_CREATE_ITEM_FAILED	-806
#define XERR_PROVIDER_SET_VALUE_FAILED		-807
#define XERR_PROVIDER_REG_DELETE_FAILED		-808

class CXInstall
{
private:
	int EnumHookKey(BOOL IsRemove = FALSE);
	int SaveHookKey(HKEY hkey, LPCTSTR sSubKey, BOOL IsRemove = FALSE);

public:
	BOOL	IsWinsock2();
	BOOL	IsInstalled(TCHAR *sPathName = NULL);
	int		InstallProvider(TCHAR *sPathName);
	int		RemoveProvider();

public:
	BOOL ReadReg(
		TCHAR	*sKey,
		BYTE	*pBuffer,
		DWORD	dwBufSize, 
		HKEY	hkey	= HKEY_LOCAL_MACHINE, 
		TCHAR	*sSubKey = REG_INSTALL_KEY,
		DWORD	ulType	= REG_BINARY
		);

	BOOL SaveReg(
		TCHAR	*sKey, 
		BYTE	*pBuffer, 
		DWORD	dwBufSize, 
		HKEY	hkey	= HKEY_LOCAL_MACHINE, 
		TCHAR	*sSubKey = REG_INSTALL_KEY,	
		DWORD	ulType	= REG_BINARY
		);

	BOOL DeleteReg(
		HKEY	hkey	= HKEY_LOCAL_MACHINE,
		TCHAR	*sSubKey = REG_INSTALL_KEY, 
		TCHAR	*sItem	= NULL
		);

public:
	TCHAR m_sPathName[MAX_PATH];
};