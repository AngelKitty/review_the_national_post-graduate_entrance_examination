// Link.h: interface for the CLink class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINK_H__C2CCD87E_3E0B_44EC_8DDD_92F8E52C37A7__INCLUDED_)
#define AFX_LINK_H__C2CCD87E_3E0B_44EC_8DDD_92F8E52C37A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "GameBase.h"

typedef struct T_Link
{
	void *data;
	struct T_Link *next;
}T_Link;

class CLink  
{
public:
	T_Link* GetLink();
	T_Link* GetNext();
	void Remove();
	void RemoveAll();
	T_Link* GetCurPos();
	T_Link* GetHead();
	T_Link* GetLast();
	void AddTail(void *data);
	CLink();
	virtual ~CLink();

private:
	T_Link* m_prepos;
	T_Link* m_curpos;
	T_Link* m_last;
	T_Link* m_head;
};

#endif // !defined(AFX_LINK_H__C2CCD87E_3E0B_44EC_8DDD_92F8E52C37A7__INCLUDED_)
