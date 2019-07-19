// VulnerAXPropPage.cpp : Implementation of the CVulnerAXPropPage property page class.

#include "stdafx.h"
#include "VulnerAX.h"
#include "VulnerAXPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CVulnerAXPropPage, COlePropertyPage)



// Message map

BEGIN_MESSAGE_MAP(CVulnerAXPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CVulnerAXPropPage, "VULNERAX.VulnerAXPropPage.1",
	0xc2b9aade, 0xd02f, 0x4523, 0xba, 0xb1, 0x2c, 0x9f, 0x68, 0x16, 0x36, 0x17)



// CVulnerAXPropPage::CVulnerAXPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CVulnerAXPropPage

BOOL CVulnerAXPropPage::CVulnerAXPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_VULNERAX_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CVulnerAXPropPage::CVulnerAXPropPage - Constructor

CVulnerAXPropPage::CVulnerAXPropPage() :
	COlePropertyPage(IDD, IDS_VULNERAX_PPG_CAPTION)
{
}



// CVulnerAXPropPage::DoDataExchange - Moves data between page and properties

void CVulnerAXPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CVulnerAXPropPage message handlers
