// VulnerAXCtrl.cpp : Implementation of the CVulnerAXCtrl ActiveX Control class.

#include "stdafx.h"
#include "VulnerAX.h"
#include "VulnerAXCtrl.h"
#include "VulnerAXPropPage.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CVulnerAXCtrl, COleControl)



// Message map

BEGIN_MESSAGE_MAP(CVulnerAXCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()



// Dispatch map

BEGIN_DISPATCH_MAP(CVulnerAXCtrl, COleControl)
	DISP_FUNCTION_ID(CVulnerAXCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CVulnerAXCtrl, "test", dispidtest, test, VT_EMPTY, VTS_BSTR)
END_DISPATCH_MAP()



// Event map

BEGIN_EVENT_MAP(CVulnerAXCtrl, COleControl)
END_EVENT_MAP()



// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CVulnerAXCtrl, 1)
	PROPPAGEID(CVulnerAXPropPage::guid)
END_PROPPAGEIDS(CVulnerAXCtrl)



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CVulnerAXCtrl, "VULNERAX.VulnerAXCtrl.1",
	0x39f64d5b, 0x74e8, 0x482f, 0x95, 0xf4, 0x91, 0x8e, 0x54, 0xb1, 0xb2, 0xc8)



// Type library ID and version

IMPLEMENT_OLETYPELIB(CVulnerAXCtrl, _tlid, _wVerMajor, _wVerMinor)



// Interface IDs

const IID BASED_CODE IID_DVulnerAX =
		{ 0x25C13936, 0x195C, 0x405C, { 0xA7, 0x97, 0xCE, 0x7F, 0x4B, 0xE3, 0xC2, 0x60 } };
const IID BASED_CODE IID_DVulnerAXEvents =
		{ 0x859AB449, 0x2E1E, 0x49D5, { 0x8A, 0xAD, 0x62, 0xB2, 0xA, 0x7A, 0x3D, 0x55 } };



// Control type information

static const DWORD BASED_CODE _dwVulnerAXOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CVulnerAXCtrl, IDS_VULNERAX, _dwVulnerAXOleMisc)



// CVulnerAXCtrl::CVulnerAXCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CVulnerAXCtrl

BOOL CVulnerAXCtrl::CVulnerAXCtrlFactory::UpdateRegistry(BOOL bRegister)
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
			IDS_VULNERAX,
			IDB_VULNERAX,
			afxRegApartmentThreading,
			_dwVulnerAXOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}



// CVulnerAXCtrl::CVulnerAXCtrl - Constructor

CVulnerAXCtrl::CVulnerAXCtrl()
{
	InitializeIIDs(&IID_DVulnerAX, &IID_DVulnerAXEvents);
	// TODO: Initialize your control's instance data here.
}



// CVulnerAXCtrl::~CVulnerAXCtrl - Destructor

CVulnerAXCtrl::~CVulnerAXCtrl()
{
	// TODO: Cleanup your control's instance data here.
}



// CVulnerAXCtrl::OnDraw - Drawing function

void CVulnerAXCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: Replace the following code with your own drawing code.
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}



// CVulnerAXCtrl::DoPropExchange - Persistence support

void CVulnerAXCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.
}



// CVulnerAXCtrl::OnResetState - Reset control to default state

void CVulnerAXCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}



// CVulnerAXCtrl::AboutBox - Display an "About" box to the user

void CVulnerAXCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_VULNERAX);
	dlgAbout.DoModal();
}



// CVulnerAXCtrl message handlers

void CVulnerAXCtrl::test(LPCTSTR str)
{
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// TODO: Add your dispatch handler code here
	printf("aaaa");//定位该函数的标记
    char dest[100];
	sprintf(dest,"%s",str);	
}

