#pragma once

// VulnerAX_SEHPropPage.h : Declaration of the CVulnerAX_SEHPropPage property page class.


// CVulnerAX_SEHPropPage : See VulnerAX_SEHPropPage.cpp for implementation.

class CVulnerAX_SEHPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CVulnerAX_SEHPropPage)
	DECLARE_OLECREATE_EX(CVulnerAX_SEHPropPage)

// Constructor
public:
	CVulnerAX_SEHPropPage();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_VULNERAX_SEH };

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	DECLARE_MESSAGE_MAP()
};

