// EmuLoop.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

struct MsgBlob
{
    MsgBlob*        pNext;
    ULONG           ulBlobID;
    // ... other fields
    BYTE            bPayload[1];
};

struct MsgQueue
{
     MsgBlob * m_pCurMsgBlob;    
};


void FatalError(ULONG ulErrCode)
{
    MessageBox(NULL, "Couldn't move to next","EmuLoop", MB_OK);
}

void * GetPayloadAndMoveToNext(MsgQueue * pMsgQueue)
{
    MsgBlob * pCurrent=pMsgQueue->m_pCurMsgBlob;

    if(pCurrent==NULL)
    {
        FatalError(E_FAIL); // never return
    }

    pMsgQueue->m_pCurMsgBlob=pCurrent->pNext;

    return &(pCurrent->bPayload);
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
    MsgBlob   blob={NULL, 1, 'a'};
    MsgQueue queue;

    queue.m_pCurMsgBlob=&blob;

 	// TODO: Place code here.
    GetPayloadAndMoveToNext(&queue);
    
	return 0;
}



