#include "stdafx.h"
#include "message.h"


namespace UI
{
//
//	return
//		����pMsg->lRet��Ϊ����ֵ��
//			�¼���Ӧ�����Ľ������ֵ��ΪpMsg->lRet;
//			����¼���Ӧ����û�з���ֵ��pMsg->lRetĬ��Ϊ0
//	remark
//		��Ҫ֪�������Ϣ��û�б������ɵ���IsMsgHandled()
//
//  ע������ϵͳ��SendMessage������nMsg�з�Χ���Ƶģ�
//     if(Msg&0xFFFE0000)// Msgs (>0xFFFF) are reserved by the system   
//     {  
//         User32._UserSetLaseError(0x57); //0x57 The parameter is incorrect.    
//         return 0;  
//     }  
long UISendMessage(UIMSG* pMsg, int nMsgMapID, BOOL* pbHandled)
{
	UIASSERT (pMsg != NULL);
	if (NULL == pMsg)
	{
		UI_LOG_ERROR(_T("UISendMessage, pMsg==NULL! ") );
		return -1;
	}
	
	UIASSERT(pMsg->pMsgTo != NULL);
//	UIASSERT(pMsg->message != 0);   // �п�����WM_NULL

	if (NULL == pMsg->pMsgTo)
	{
		UI_LOG_ERROR(_T("UISendMessage, pMessageTo==NULL!  message=%d"), pMsg->message);
		return -1;
	}

	BOOL bRet = pMsg->pMsgTo->ProcessMessage(pMsg, nMsgMapID, true);
	if (pbHandled) 
	{
		*pbHandled = bRet;
	}

	return pMsg->lRet;
}

long  UISendMessage(IMessage* pMsgTo,  UINT message, 
					WPARAM wParam, LPARAM lParam, 
					UINT nCode, IMessage* pMsgFrom,
					int nMsgMapID,BOOL* pbHandled)
{
	if (NULL == pMsgTo)
	{
		UI_LOG_FATAL(_T("pObjMsgTo == NULL"));
		return 0;
	}

    UIMSG msg;
	msg.pMsgFrom = pMsgFrom;
	msg.pMsgTo   = pMsgTo;

	msg.message = message;
	msg.nCode   = nCode;
	msg.wParam  = wParam;
	msg.lParam  = lParam;

	BOOL bRet = pMsgTo->ProcessMessage(&msg, nMsgMapID, true);
	if (pbHandled) 
	{
		*pbHandled = bRet;
	}

	return msg.lRet;
}

long UISendMessage(Message* pMsgTo, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (NULL == pMsgTo)
		return 0;

	return UISendMessage(pMsgTo->GetIMessage(), message, wParam, lParam, 0, NULL, 0, NULL);
}


// ��֧�ֿ��̵߳���
long  UIPostMessage(HWND hForwardMsgWnd, UIMSG* pMsg, int nMsgMapID)
{
	if (!pMsg->pMsgTo)
		return 0;

	UIMSG* pCloneMsg = new UIMSG;
	memcpy(pCloneMsg, pMsg, sizeof(UIMSG));

	pMsg->pMsgTo->AddDelayRef((void**)&pCloneMsg->pMsgTo);

	// -->ForwardPostMessageWindow::ProcessWindowMessage
	::PostMessage(hForwardMsgWnd, UI_MSG_POSTMESSAGE, (WPARAM)pCloneMsg, (LPARAM)nMsgMapID);


	return 0;
}

// ����pMsg.pObjTo��һ��Message���ͣ��޷���ȡpUIAppָ�룬ֻ��������һ������
long  UIPostMessage(IUIApplication* pUIApp, UIMSG* pMsg, int nMsgMapID)
{
    if (NULL == pUIApp)
        return 0;

    HWND hDestWnd = pUIApp->GetForwardPostMessageWnd();
    if (NULL == hDestWnd)
        return 0;

    return UIPostMessage(hDestWnd, pMsg, nMsgMapID);
}


}

Message::Message(IMessage* pIMessage)
{
	m_pCurMsg = NULL;
    m_pIMessage = pIMessage;
    m_bCreateIMessage = FALSE;
}
Message::~Message()
{
	m_pCurMsg = NULL;

	this->ClearNotify();
	this->ClearHook();
    this->ResetDelayRef();

    if (m_bCreateIMessage)
    {
		m_pIMessage->m_pImpl = NULL;
        SAFE_DELETE(m_pIMessage);
    }
}

// ���Լ�û�д�IMessage����ʱ��Ϊ��֧��IMessage���Լ�����һ����
IMessage*  Message::GetIMessage()   
{ 
//     if (NULL == m_pIMessage)
//     {
//         m_pIMessage = new IMessageInnerProxy;
//         m_pIMessage->SetMessageImpl(this);
//         m_bCreateIMessage = TRUE;
//     }
    return m_pIMessage; 
}


// ��ȡ��ǰ��Ϣ�Ƿ��ѱ��������
// remark
//	  �ú���ֻ����ProcessMessage��Χ�ڵ��ã���Ϊm_pCurMsgֻ����η�Χ����Ч
BOOL Message::IsMsgHandled() const                         
{                  
	UIASSERT(m_pCurMsg);
    if (!m_pCurMsg)
        return FALSE;

	return this->m_pCurMsg->bHandled;                         
}   

// ���õ�ǰ��Ϣ�ѱ��������
void Message::SetMsgHandled(BOOL bHandled)
{
    UIASSERT(m_pCurMsg);	
    if (!m_pCurMsg)
        return;

    this->m_pCurMsg->bHandled = bHandled; 
}

void Message::SetNotify(IMessage* pObj, int nMsgMapID)
{
	m_objNotify.pObj = pObj;
	m_objNotify.nMsgMapIDToNotify = nMsgMapID;
}

void Message::ClearNotify()
{
	m_objNotify.pObj = NULL;
	m_objNotify.nMsgMapIDToNotify = 0;
}

//
//  e.g. �˵������ڵ���ʱ����Ҫ����һ�θ����ڵ�notify����
//
void Message::CopyNotifyTo(IMessage* pObjCopyTo)
{
	if (NULL == pObjCopyTo)
		return;

	pObjCopyTo->SetNotify(m_objNotify.pObj, m_objNotify.nMsgMapIDToNotify);
}

// ����Notify��Ϣ��ע���Notify�Ķ���.(�޸�Ϊֻ֪ͨ��һ����.֪ͨ�����̫����)
// ��bPostΪtrueʱ������ҪpUIApp����
long Message::DoNotify(UIMSG* pMsg/*, bool bPost, IUIApplication* pUIApp*/)
{
//	IMessage* pNotifyObj = pMsg->pMsgTo;
	int    nMsgMapID = 0;
	long   lRet = 0;     // �����UIMSG��ָ����pObjMsgTo�������ȷ��͸��ö��󣬲���nMsgMapID = 0;

	if (NULL == pMsg->pMsgTo)
	{
		pMsg->pMsgTo = m_objNotify.pObj/*->GetIMessage()*/;
		nMsgMapID = m_objNotify.nMsgMapIDToNotify;
	}
	if (NULL == pMsg->pMsgTo)
	{
		return 0;
	}

// 	if (bPost && NULL != pUIApp)
// 	{
// 		UIPostMessage(pUIApp, pMsg, nMsgMapID);
// 		lRet = 0;
// 	}
// 	else
	{
		lRet = UISendMessage(pMsg, nMsgMapID, NULL);
	}
	return lRet;
}



// 
//	����Ϣ����HOOK����
//
//	return
//		�����е�HOOK�б��У�ֻҪ��һ��HOOK��������󷵻�TRUE��������
//		HOOK���̽�����
//	remark
//		��HOOK�����У����� pMsg->pObjMsgTo���޸�
//
BOOL Message::DoHook( UIMSG* pMsg, int nMsgMapID )
{
	BOOL  bRet = FALSE;

	if (0 == m_lHookMsgMap.size())
		return bRet;

	list<MsgHook*>::iterator  iter = m_lHookMsgMap.begin();
	for (; iter!=m_lHookMsgMap.end(); )
	{
		MsgHook* pHook = *iter;
		iter++;  // ��++���������Ϊ�˽����ProccessMessage�е���RemoveHook->erase(iter)�ı�������

		if (nMsgMapID == pHook->nMsgMapIDToHook)
		{
			if (TRUE == pHook->pObj->ProcessMessage(pMsg, (int)pHook->nMsgMapIDToNotify))
				return TRUE;
		}
	}

	return FALSE;
}

void Message::AddHook(IMessage* pObj, int nMsgMapIDToHook, int nMsgMapIDToNotify )
{
	list< MsgHook* >::iterator  iter    = m_lHookMsgMap.begin();
	list< MsgHook* >::iterator  iterEnd = m_lHookMsgMap.end();
	for( ; iter!=iterEnd; iter++ )
	{
		MsgHook* pHook = *iter;

		if( pObj == pHook->pObj &&
			nMsgMapIDToHook == pHook->nMsgMapIDToHook &&
			nMsgMapIDToNotify == pHook->nMsgMapIDToNotify )
			return;
	}

	MsgHook*  pHook = new MsgHook;
	pHook->pObj = pObj;
	pHook->nMsgMapIDToHook = nMsgMapIDToHook;
	pHook->nMsgMapIDToNotify = nMsgMapIDToNotify;

	this->m_lHookMsgMap.push_back( pHook );
}

void Message::RemoveHook(IMessage* pObj, int nMsgMapIDToHook, int nMsgMapIDToNotify )
{
	list< MsgHook* >::iterator  iter    = m_lHookMsgMap.begin();
	list< MsgHook* >::iterator  iterEnd = m_lHookMsgMap.end();
	for (; iter!=iterEnd; iter++)
	{
		MsgHook* pHook = *iter;

		if (pObj == pHook->pObj &&
			nMsgMapIDToHook == pHook->nMsgMapIDToHook &&
			nMsgMapIDToNotify == pHook->nMsgMapIDToNotify)
		{
			
			delete    pHook;
			this->m_lHookMsgMap.erase( iter );

			return;
		}
	}
}
void Message::RemoveHook(IMessage* pObj)
{
	list< MsgHook* >::iterator  iter    = m_lHookMsgMap.begin();
	list< MsgHook* >::iterator  iterEnd = m_lHookMsgMap.end();
	for (; iter!=iterEnd;)
	{
		MsgHook*  pHook = *iter;
		if (pObj == pHook->pObj)
		{
			delete    pHook;
			iter = this->m_lHookMsgMap.erase( iter );
			continue;
		}
		iter++;
	}
}

void Message::ClearHook( )
{
	list< MsgHook* >::iterator  iter    = m_lHookMsgMap.begin();
	list< MsgHook* >::iterator  iterEnd = m_lHookMsgMap.end();
	for (; iter!=iterEnd; iter++)
	{
		MsgHook*  pHook = *iter;
		delete    pHook;
	}
	this->m_lHookMsgMap.clear();
}

void  Message::AddDelayRef(void** pp)
{
    list<void**>::iterator  iter = std::find(m_lDelayRefs.begin(), m_lDelayRefs.end(), pp);
    if (iter == m_lDelayRefs.end())
    {
        m_lDelayRefs.push_back(pp);
    }
}
void  Message::RemoveDelayRef(void** pp)
{
    list<void**>::iterator  iter = std::find(m_lDelayRefs.begin(), m_lDelayRefs.end(), pp);
    if (iter != m_lDelayRefs.end())
    {
        m_lDelayRefs.erase(iter);
    }
}
void  Message::ResetDelayRef()
{
    list<void**>::iterator  iter = m_lDelayRefs.begin();
    for (; iter != m_lDelayRefs.end(); iter++)
    {
        void** pp = *iter;
        *pp = NULL;
    }
    m_lDelayRefs.clear();
}

 BOOL Message::ProcessMessage(
        UIMSG* pMsg, int nMsgMapID, bool bDoHook)
 {
     UI::UIMSG*  pOldMsg  = this->GetCurMsg(); 
     BOOL bRet = this->virtualProcessMessage(pMsg, nMsgMapID, bDoHook);
     this->SetCurMsg(pOldMsg);    
     return bRet;
 }
 BOOL Message::virtualProcessMessage(UIMSG* pMsg, int nMsgMapID, bool bDoHook)
 {
     return FALSE; 
 }
