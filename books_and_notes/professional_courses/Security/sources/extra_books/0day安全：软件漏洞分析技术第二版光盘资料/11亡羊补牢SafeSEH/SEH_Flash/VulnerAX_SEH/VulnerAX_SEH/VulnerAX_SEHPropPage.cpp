// VulnerAX_SEHPropPage.cpp : Implementation of the CVulnerAX_SEHPropPage property page class.

#include "stdafx.h"
#include "VulnerAX_SEH.h"
#include "VulnerAX_SEHPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CVulnerAX_SEHPropPage, COlePropertyPage)



// Message map

BEGIN_MESSAGE_MAP(CVulnerAX_SEHPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CVulnerAX_SEHPropPage, "VULNERAX_SEH.VulnerAX_SEHPropPage.1",
	0x88195456, 0x431f, 0x461c, 0x94, 0xcc, 0xe7, 0xbc, 0x7d, 0xaf, 0xea, 0xc2)



// CVulnerAX_SEHPropPage::CVulnerAX_SEHPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CVulnerAX_SEHPropPage

BOOL CVulnerAX_SEHPropPage::CVulnerAX_SEHPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_VULNERAX_SEH_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CVulnerAX_SEHPropPage::CVulnerAX_SEHPropPage - Constructor

CVulnerAX_SEHPropPage::CVulnerAX_SEHPropPage() :
	COlePropertyPage(IDD, IDS_VULNERAX_SEH_PPG_CAPTION)
{
}



// CVulnerAX_SEHPropPage::DoDataExchange - Moves data between page and properties

void CVulnerAX_SEHPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CVulnerAX_SEHPropPage message handlers
