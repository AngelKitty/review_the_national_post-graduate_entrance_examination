#pragma once

// VulnerAXPropPage.h : Declaration of the CVulnerAXPropPage property page class.


// CVulnerAXPropPage : See VulnerAXPropPage.cpp for implementation.

class CVulnerAXPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CVulnerAXPropPage)
	DECLARE_OLECREATE_EX(CVulnerAXPropPage)

// Constructor
public:
	CVulnerAXPropPage();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_VULNERAX };

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	DECLARE_MESSAGE_MAP()
};

