#pragma once

// VulnerAXCtrl.h : Declaration of the CVulnerAXCtrl ActiveX Control class.


// CVulnerAXCtrl : See VulnerAXCtrl.cpp for implementation.

class CVulnerAXCtrl : public COleControl
{
	DECLARE_DYNCREATE(CVulnerAXCtrl)

// Constructor
public:
	CVulnerAXCtrl();

// Overrides
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// Implementation
protected:
	~CVulnerAXCtrl();

	DECLARE_OLECREATE_EX(CVulnerAXCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CVulnerAXCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CVulnerAXCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CVulnerAXCtrl)		// Type name and misc status

// Message maps
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
		dispidtest = 1L
	};
protected:
	void test(LPCTSTR str);
};

