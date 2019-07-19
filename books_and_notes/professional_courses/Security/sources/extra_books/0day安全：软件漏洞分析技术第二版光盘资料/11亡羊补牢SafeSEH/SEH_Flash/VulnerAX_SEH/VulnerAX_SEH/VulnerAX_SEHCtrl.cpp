// VulnerAX_SEHCtrl.cpp : Implementation of the CVulnerAX_SEHCtrl ActiveX Control class.

#include "stdafx.h"
#include "VulnerAX_SEH.h"
#include "VulnerAX_SEHCtrl.h"
#include "VulnerAX_SEHPropPage.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CVulnerAX_SEHCtrl, COleControl)



// Message map

BEGIN_MESSAGE_MAP(CVulnerAX_SEHCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()



// Dispatch map

BEGIN_DISPATCH_MAP(CVulnerAX_SEHCtrl, COleControl)
	DISP_FUNCTION_ID(CVulnerAX_SEHCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CVulnerAX_SEHCtrl, "test", dispidtest, test, VT_EMPTY, VTS_BSTR)
END_DISPATCH_MAP()



// Event map

BEGIN_EVENT_MAP(CVulnerAX_SEHCtrl, COleControl)
END_EVENT_MAP()



// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CVulnerAX_SEHCtrl, 1)
	PROPPAGEID(CVulnerAX_SEHPropPage::guid)
END_PROPPAGEIDS(CVulnerAX_SEHCtrl)



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CVulnerAX_SEHCtrl, "VULNERAX_SEH.VulnerAX_SEHCtrl.1",
	0x1f0837f2, 0x15e5, 0x4115, 0xa2, 0x35, 0x81, 0xda, 0x2f, 0x73, 0xc2, 0x4)



// Type library ID and version

IMPLEMENT_OLETYPELIB(CVulnerAX_SEHCtrl, _tlid, _wVerMajor, _wVerMinor)



// Interface IDs

const IID BASED_CODE IID_DVulnerAX_SEH =
		{ 0x6CEE025A, 0x3C2D, 0x49CE, { 0x81, 0x4E, 0x8B, 0xE9, 0x90, 0xF6, 0x3, 0x77 } };
const IID BASED_CODE IID_DVulnerAX_SEHEvents =
		{ 0x38A0AC24, 0xC620, 0x4839, { 0xBB, 0xF5, 0x66, 0x64, 0x4C, 0x60, 0x4A, 0x93 } };



// Control type information

static const DWORD BASED_CODE _dwVulnerAX_SEHOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CVulnerAX_SEHCtrl, IDS_VULNERAX_SEH, _dwVulnerAX_SEHOleMisc)



// CVulnerAX_SEHCtrl::CVulnerAX_SEHCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CVulnerAX_SEHCtrl

BOOL CVulnerAX_SEHCtrl::CVulnerAX_SEHCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_VULNERAX_SEH,
			IDB_VULNERAX_SEH,
			afxRegApartmentThreading,
			_dwVulnerAX_SEHOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}



// CVulnerAX_SEHCtrl::CVulnerAX_SEHCtrl - Constructor

CVulnerAX_SEHCtrl::CVulnerAX_SEHCtrl()
{
	InitializeIIDs(&IID_DVulnerAX_SEH, &IID_DVulnerAX_SEHEvents);
	// TODO: Initialize your control's instance data here.
}



// CVulnerAX_SEHCtrl::~CVulnerAX_SEHCtrl - Destructor

CVulnerAX_SEHCtrl::~CVulnerAX_SEHCtrl()
{
	// TODO: Cleanup your control's instance data here.
}



// CVulnerAX_SEHCtrl::OnDraw - Drawing function

void CVulnerAX_SEHCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: Replace the following code with your own drawing code.
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}



// CVulnerAX_SEHCtrl::DoPropExchange - Persistence support

void CVulnerAX_SEHCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.
}



// CVulnerAX_SEHCtrl::OnResetState - Reset control to default state

void CVulnerAX_SEHCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}



// CVulnerAX_SEHCtrl::AboutBox - Display an "About" box to the user

void CVulnerAX_SEHCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_VULNERAX_SEH);
	dlgAbout.DoModal();
}



DWORD MyException(void)
{
	return 1;
}

// CVulnerAX_SEHCtrl message handlers
void CVulnerAX_SEHCtrl::test(LPCTSTR str)
{
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// TODO: Add your dispatch handler code here
	printf("aaaa");//定位该函数的标记
	char dest[100];
	sprintf(dest,"%s",str);	
	int zero=0;
	__try
	{
	    zero=1/zero;
	}
	__except(MyException())
	{
	}



}
