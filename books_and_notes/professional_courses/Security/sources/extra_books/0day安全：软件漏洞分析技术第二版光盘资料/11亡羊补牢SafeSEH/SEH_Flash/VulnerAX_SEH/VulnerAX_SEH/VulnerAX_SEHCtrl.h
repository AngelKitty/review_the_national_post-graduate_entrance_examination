#pragma once

// VulnerAX_SEHCtrl.h : Declaration of the CVulnerAX_SEHCtrl ActiveX Control class.


// CVulnerAX_SEHCtrl : See VulnerAX_SEHCtrl.cpp for implementation.

class CVulnerAX_SEHCtrl : public COleControl
{
	DECLARE_DYNCREATE(CVulnerAX_SEHCtrl)

// Constructor
public:
	CVulnerAX_SEHCtrl();

// Overrides
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// Implementation
protected:
	~CVulnerAX_SEHCtrl();

	DECLARE_OLECREATE_EX(CVulnerAX_SEHCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CVulnerAX_SEHCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CVulnerAX_SEHCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CVulnerAX_SEHCtrl)		// Type name and misc status

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

