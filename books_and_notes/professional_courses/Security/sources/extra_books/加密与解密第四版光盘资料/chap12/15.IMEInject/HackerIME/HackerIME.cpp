/*-----------------------------------------------------------------------
第12章  注入技术
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

// HackerIME.cpp : Defines the entry point for the DLL application.
// 本源码来自网上收集，作者未知
// 编写DLL时需要注意,当作IME文件的Dll需要有版本信息,Version资源中FILETYPE为VFT_DRV, FILESUBTYPE为VFT2_DRV_INPUTMETHOD,否则调用ImmInstallIME安装时会失败
#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "imm.h"
#include <tchar.h>
#include "HackerIME.h"

#pragma comment(lib,"imm32.lib")

//窗口类名
#define CLSNAME_UI			_T("DLLISUI")		//UI
#define CS_INPUTSTAR			(CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS)


#pragma data_seg("mysechx")
DWORD CallBackData1=0;
DWORD CallBackData2=0;
DWORD CallBackData3=0;
DWORD OnloadDllWhenExit=0;    // 当输入法退出时是否卸载客户DLL  0-是，1-否
DWORD LoadNextWhenActive=0;    // 当本输入法激活时，是否自动打开下一个输入法 0-否，1-是
#ifdef _WIN64
char g_IMEDLLString[802]="C:\\MsgDll64.dll";
#else
char g_IMEDLLString[802]="C:\\MsgDll.dll";
#endif
#pragma data_seg()

typedef DWORD (CALLBACK * RUNDLLHOSTCALLBACK)(DWORD calldata1, DWORD calldata2,DWORD calldata3);

HMODULE CilentDLL=NULL;
RUNDLLHOSTCALLBACK RunDllCallBackX=NULL;

// 先定义好各种函数
BOOL ImeClass_Register(HINSTANCE hInstance);
void ImeClass_Unregister(HINSTANCE hInstance);
LRESULT WINAPI UIWndProc(HWND hUIWnd,UINT message,WPARAM wParam,LPARAM lParam);
BOOL MyGenerateMessage(HIMC hIMC, UINT msg, WPARAM wParam, LPARAM lParam);

//VFT2_DRV_INPUTMETHOD
//VFT2_DRV_INSTALLABLE
DWORD WINAPI MyLoadCilentDLLThread(LPVOID lpParameters)
{
	LoadLibrary(g_IMEDLLString);   // 在输入法加载时同时加载客户DLL
	return 0 ;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved)
{
   switch(fdwReason)
    {
      case DLL_PROCESS_ATTACH:
		  if(!ImeClass_Register(hinstDLL)) return FALSE;   // DLL加载时注册必须的UI基本窗口类
		  CreateThread(NULL,0,MyLoadCilentDLLThread,NULL,0,NULL);
		  break;
	  case DLL_THREAD_ATTACH:
		 break;
	  case DLL_THREAD_DETACH:
		 break;
      case DLL_PROCESS_DETACH:
		  ImeClass_Unregister(hinstDLL);  // DLL退出时注销注册的窗口类
		  if (CilentDLL!=NULL && OnloadDllWhenExit==0)
		  {
			  FreeLibrary(CilentDLL);    // 输入法退出时卸载客户DLL
		  }
        break;
      default:
        break;
    }
	return true;
}

int WINAPI IMESetPubString(LPCTSTR tmpStr,DWORD UnloadDLL,DWORD loadNextIme,DWORD DllData1,DWORD DllData2,DWORD DllData3)
{
	CallBackData1=DllData1;
	CallBackData2=DllData2;
	CallBackData3=DllData3;
	OnloadDllWhenExit=UnloadDLL;
	LoadNextWhenActive=loadNextIme;

	memset(g_IMEDLLString,0,802);
	if (lstrlen(tmpStr)>800)
	{
		lstrcpyn(g_IMEDLLString,tmpStr,800);
	}
	else
	{
		lstrcpy(g_IMEDLLString,tmpStr);
	}
	return 1;
}

int WINAPI IMEClearPubString()
{
	CallBackData1=0;
	CallBackData2=0;
	CallBackData3=0;
	OnloadDllWhenExit=0;
	LoadNextWhenActive=0;

	memset(g_IMEDLLString,0,802);
	return 1;
}



//************************************************************
//	基本输入法窗口UI类注册
//************************************************************
BOOL ImeClass_Register(HINSTANCE hInstance)
{
    WNDCLASSEX wc;
	
    //
    // register class of UI window.
    //
    wc.cbSize         = sizeof(WNDCLASSEX);
    wc.style          = CS_INPUTSTAR | CS_IME;
    wc.lpfnWndProc    = UIWndProc;
    wc.cbClsExtra     = 0;
    wc.cbWndExtra     = 2 * sizeof(LONG);
    wc.hInstance      = hInstance;
    wc.hCursor        = LoadCursor( NULL, IDC_ARROW );
    wc.hIcon          = NULL;
    wc.lpszMenuName   = (LPTSTR)NULL;
    wc.lpszClassName  = CLSNAME_UI;
    wc.hbrBackground  = NULL;
    wc.hIconSm        = NULL;
	
    if( !RegisterClassEx( (LPWNDCLASSEX)&wc ) )
        return FALSE;
	
	return TRUE;
}

//**************************************************************
//	注销注册的窗口类
//**************************************************************
void ImeClass_Unregister(HINSTANCE hInstance)
{
	UnregisterClass(CLSNAME_UI,hInstance);
}


// ------------------------------------

DWORD WINAPI ImeConversionList(HIMC hIMC,LPCTSTR lpSource,LPCANDIDATELIST lpCandList,DWORD dwBufLen,UINT uFlag)
{
    return 0;
}

BOOL WINAPI ImeConfigure(HKL hKL,HWND hWnd, DWORD dwMode, LPVOID lpData)
{
    switch (dwMode) {
    case IME_CONFIG_GENERAL:
        MessageBox(NULL,"Windows标准输入法扩展服务 V1.0  ","关于输入法扩展",48);
        break;
    default:
        return (FALSE);
        break;
    }
    return (TRUE);
}

BOOL WINAPI ImeDestroy(UINT uForce)
{
    if (uForce) {
        return (FALSE);
    }

    return (TRUE);
}

LRESULT WINAPI ImeEscape(HIMC hIMC,UINT uSubFunc,LPVOID lpData)
{
	return FALSE;
}


BOOL WINAPI ImeInquire(LPIMEINFO lpIMEInfo,LPTSTR lpszUIClass,LPCTSTR lpszOption)
{
	// 输入法初始化过程
    lpIMEInfo->dwPrivateDataSize = 0; //系统根据它为INPUTCONTEXT.hPrivate分配空间

    lpIMEInfo->fdwProperty = IME_PROP_KBD_CHAR_FIRST | 
                             IME_PROP_IGNORE_UPKEYS |
							 IME_PROP_END_UNLOAD; 

    lpIMEInfo->fdwConversionCaps = IME_CMODE_FULLSHAPE |
								IME_CMODE_NATIVE;

    lpIMEInfo->fdwSentenceCaps = IME_SMODE_NONE;
    lpIMEInfo->fdwUICaps = UI_CAP_2700;

	lpIMEInfo->fdwSCSCaps = 0;

    lpIMEInfo->fdwSelectCaps = SELECT_CAP_CONVERSION;

    _tcscpy(lpszUIClass,CLSNAME_UI);  // 注意该输入法基本窗口类必须注册，否则输入法不能正常运行

    return TRUE;
}

/*
系统调用这个接口来判断IME是否处理当前键盘输入
HIMC hIMC:输入上下文
UINT uKey:键值
LPARAM lKeyData: unknown
CONST LPBYTE lpbKeyState:键盘状态,包含256键的状态
return : TRUE-IME处理,FALSE-系统处理
系统则调用ImeToAsciiEx，否则直接将键盘消息发到应用程序
*/
BOOL WINAPI ImeProcessKey(HIMC hIMC,UINT uKey,LPARAM lKeyData,CONST LPBYTE lpbKeyState)
{
	return FALSE;
}

/**********************************************************************/
/* ImeSelect()                                                        */
/* Return Value:                                                      */
/*      TRUE - successful, FALSE - failure                            */
/**********************************************************************/
BOOL WINAPI ImeSelect(HIMC hIMC,BOOL fSelect)
{
	MyLoadCilentDLLThread(NULL);   // 在切换输入法时判断是否需要加载客户DLL

    if (!hIMC) {
        return (FALSE);
    }
	if (fSelect==TRUE && LoadNextWhenActive!=0)
	{
		//ActivateKeyboardLayout((HKL)HKL_NEXT,0);  // 不要在该接口中使用此函数切换到下一个输入法，否则函数返回时输入法又会切换回去
		MyGenerateMessage(hIMC,WM_INPUTLANGCHANGEREQUEST,1,(LPARAM)HKL_NEXT);
	}
    return TRUE;
}


/*
使一个输入上下文激活或者失活，并通知输入法最新的输入上下文，可以在此做一些初始化工作
HIMC hIMC :输入上下文
BOOL fFlag : TRUE if activated, FALSE if deactivated. 
Returns TRUE if successful, FALSE otherwise. 
*/
BOOL WINAPI ImeSetActiveContext(HIMC hIMC,BOOL fFlag)
{
	//通过IME消息来实现窗口状态变化
    return TRUE;
}

/*
Causes the IME to arrange the composition string structure with the given data.
This function causes the IME to send the WM_IME_COMPOSITION message. 
Returns TRUE if successful, FALSE otherwise.
*/
BOOL WINAPI ImeSetCompositionString(HIMC hIMC, DWORD dwIndex, LPCVOID lpComp, DWORD dwComp, LPCVOID lpRead, DWORD dwRead)
{
    return FALSE;
}


/*
应用程序调用这个接口来进行输入上下文的转换，输入法程序在这个接口中转换用户的输入
UINT uVKey:键值,如果在ImeInquire接口中为fdwProperty设置了属性IME_PROP_KBD_CHAR_FIRST,则高字节是输入键值
UINT uScanCode:按键的扫描码，有时两个键有同样的键值，这时需要使用uScanCode来区分
CONST LPBYTE lpbKeyState:键盘状态,包含256键的状态
LPDWORD lpdwTransKey:消息缓冲区，用来保存IME要发给应用程序的消息，第一个双字是缓冲区可以容纳的最大消息条数
UINT fuState:Active menu flag(come from msdn)
HIMC hIMC:输入上下文
return : 返回保存在消息缓冲区lpdwTransKey中的消息个数
*/
UINT WINAPI ImeToAsciiEx (UINT uVKey,UINT uScanCode,CONST LPBYTE lpbKeyState,LPDWORD lpdwTransKey,UINT fuState,HIMC hIMC)
{
    return 0;
}


//由应用程序发给输入法的消息，输入法可以在此响应用程序的请求
//return : TRUE-正确响应了请求,FALSE-无响应
BOOL WINAPI NotifyIME(HIMC hIMC,DWORD dwAction,DWORD dwIndex,DWORD dwValue)
{
    BOOL bRet = FALSE;
    switch(dwAction)
    {
	case NI_OPENCANDIDATE:
		break;
	case NI_CLOSECANDIDATE:
		break;
	case NI_SELECTCANDIDATESTR:
		break;
	case NI_CHANGECANDIDATELIST:
		break;
	case NI_SETCANDIDATE_PAGESTART:
		break;
	case NI_SETCANDIDATE_PAGESIZE:
		break;
	case NI_CONTEXTUPDATED:
		switch (dwValue)
		{
		case IMC_SETCONVERSIONMODE:
			break;
		case IMC_SETSENTENCEMODE:
			break;
		case IMC_SETCANDIDATEPOS:
			break;
		case IMC_SETCOMPOSITIONFONT:
			break;
		case IMC_SETCOMPOSITIONWINDOW:
			break;
		case IMC_SETOPENSTATUS:
			break;
		default:
			break;
		}
		break;
		
	case NI_COMPOSITIONSTR:
		switch (dwIndex)
		{
		case CPS_COMPLETE:
			break;
		case CPS_CONVERT:
			break;
		case CPS_REVERT:
			break;
		case CPS_CANCEL:
			break;
		default:
			break;
		}
		break;
			
	default:
		break;
    }
    return bRet;
}


/**********************************************************************/
/* ImeRegsisterWord                                                   */
/* Return Value:                                                      */
/*      TRUE - successful, FALSE - failure                            */
/**********************************************************************/
BOOL WINAPI ImeRegisterWord(
    LPCTSTR lpszReading,
    DWORD   dwStyle,
    LPCTSTR lpszString)
{
    return (FALSE);
}

/**********************************************************************/
/* ImeUnregsisterWord                                                 */
/* Return Value:                                                      */
/*      TRUE - successful, FALSE - failure                            */
/**********************************************************************/
BOOL WINAPI ImeUnregisterWord(
    LPCTSTR lpszReading,
    DWORD   dwStyle,
    LPCTSTR lpszString)
{
    return (FALSE);
}

/**********************************************************************/
/* ImeGetRegsisterWordStyle                                           */
/* Return Value:                                                      */
/*      number of styles copied/required                              */
/**********************************************************************/
UINT WINAPI ImeGetRegisterWordStyle(
    UINT       nItem,
    LPSTYLEBUF lpStyleBuf)
{
    return (FALSE);
}

/**********************************************************************/
/* ImeEnumRegisterWord                                                */
/* Return Value:                                                      */
/*      the last value return by the callback function                */
/**********************************************************************/
UINT WINAPI ImeEnumRegisterWord(
    REGISTERWORDENUMPROC lpfnRegisterWordEnumProc,
    LPCTSTR              lpszReading,
    DWORD                dwStyle,
    LPCTSTR              lpszString,
    LPVOID               lpData)
{
    return (FALSE);
}


/**********************************************************************/
/*                                                                    */
/* UIWndProc()                                                        */
/*                                                                    */
/* 输入法界面窗口的窗口处理过程                                       */
/*                                                                    */
/**********************************************************************/
LRESULT WINAPI UIWndProc(HWND hUIWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
    return 0;
}

LRESULT WINAPI StatusWndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	// 输入法状态条的窗口处理过程
	return 0;
}

LRESULT WINAPI CompWndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	// 输入法显示候选字的窗口的的窗口处理过程
	return 0;
}

LRESULT WINAPI CandWndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	// 输入法编码窗口的窗口处理过程
	return 0;
}



//*****************************************************************
//	向HIMC发送消息
//	利用此函数直接向输入法所属窗口发送消息
//*****************************************************************
BOOL MyGenerateMessage(HIMC hIMC, UINT msg, WPARAM wParam, LPARAM lParam)
{
	BOOL bRet=FALSE;
    LPINPUTCONTEXT lpIMC= ImmLockIMC(hIMC);
	if(lpIMC == NULL)
		return FALSE;    

    if (IsWindow(lpIMC->hWnd))
    {
		PostMessage(lpIMC->hWnd,msg,wParam,lParam);
		bRet=TRUE;
    }

	ImmUnlockIMC(hIMC); 
	return bRet;
}