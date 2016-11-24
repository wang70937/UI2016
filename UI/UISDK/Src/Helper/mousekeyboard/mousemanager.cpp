#include "stdafx.h"
#include "mousemanager.h"
#include "mkmgr.h"
#include "Src\Helper\tooltip\tooltipmanager.h"
#include "Inc\Interface\iwindow.h"
#include "Src\Base\Object\object.h"
#include "Src\UIObject\Window\window.h"
#include "Src\UIObject\HwndHost\HwndHost.h"
#include "Src\Util\Gesture\gesturehelper.h"
#include "Src\Base\Application\uiapplication.h"

#pragma comment(lib, "Imm32.lib")

namespace UI
{

WindowMouseMgr::WindowMouseMgr(WindowBase& o):
m_oWindow(o)
{
    m_pUIApplication = NULL;
	m_pObjPress = NULL;
	m_pObjHover = NULL;
    m_pObjRPress = NULL;
    m_pObjMPress = NULL;
    m_pObjGesture = NULL;
    m_pObjDefault = NULL;
    m_pObjOriginDefault = NULL;
    m_pFocusObject = NULL;
    m_pFocusObject = NULL;
    m_pOldFocusObject = NULL;
    m_pObjKeyboardCapture  = NULL;
    m_pObjMouseCapture = NULL;
    m_bMouseTrack = TRUE;      // Ĭ����Ҫ����������
    m_nKeyboardCaptureNotifyMsgId = 0;
    m_nMouseCaptureNotifyMsgId = 0;
    this->m_bMouseMoveReady = FALSE;
    m_posPrevClick = -1;
}

WindowMouseMgr::~WindowMouseMgr()
{
	m_pObjPress = NULL;
	m_pObjHover = NULL;
    m_bMouseTrack = TRUE;
}

Object*  WindowMouseMgr::GetHoverObject()
{
	return m_pObjHover;
}
Object*  WindowMouseMgr::GetPressObject()
{
	return m_pObjPress;
}

void WindowMouseMgr::SetHoverObject(Object* pNewHoverObj)
{
    _SetHoverObject(pNewHoverObj, this);
}
void WindowMouseMgr::SetPressObject(Object* pNewPressObj)
{
	_SetPressObject(pNewPressObj, this);
}

// ������Direct���������ڷ�ֹ�����Ĳ��Ӧ���Ǽܹ����ŵ�����
// ��listctrl�е��ڲ��ӿؼ�����ʱ��û��֪ͨ��listctrl��mousemgr������ȥ֪ͨwindow��mousemgr
// ���������listctrl::removeall��SetHoverObject(NULL)�б���
void  WindowMouseMgr::SetHoverObjectDirectNULL()
{
    m_pObjHover = NULL;
}
void  WindowMouseMgr::SetPressObjectDirectNULL()
{
    m_pObjPress = NULL;
}
void WindowMouseMgr::OnObjectVisibleChangeInd(Object* pObj, bool bVisible)
{
    if (NULL == pObj)
        return;

    // ��ʹ����ʾһ������Ҳ������ֱ����ʾ���˵�ǰhover object֮�ϣ����������
    // hover���󣬻ᵼ���ٴε��ʱ���õ���Ӧ�����ϵ�hover object.
    // ������ʾ�����ض�Ҫ����m_bMouseMoveReady = FALSE
    m_bMouseMoveReady = FALSE;

    if (!bVisible)
    {
        if (m_pObjHover == pObj || pObj->IsMyChild(m_pObjHover, true))
        {
            SetHoverObject(NULL);
            m_bMouseMoveReady = FALSE;

            GetUIApplication()->HideToolTip();
        }
        if (m_pObjPress == pObj || pObj->IsMyChild(m_pObjPress, true))
        {
            SetPressObject(NULL);
            m_bMouseMoveReady = FALSE;
        }

        if (m_pObjDefault == pObj)
        {
            SetDefaultObject(NULL, true);
        }
        if (m_pObjOriginDefault == pObj)
        {
            m_pObjOriginDefault = NULL;
        }

        // �л��µĽ���
        if (m_pFocusObject && 
            (pObj == m_pFocusObject || pObj->IsMyChild(m_pFocusObject, true)))
        {
            m_pFocusObject->SetFocus(false);
            ::UISendMessage(m_pFocusObject, WM_KILLFOCUS, (WPARAM)pObj, NULL );
            m_pFocusObject = NULL;

            this->Tab_2_NextControl();
        }
    }
}

void WindowMouseMgr::OnObjectRemoveInd(Object* pObj)
{
	if (NULL == pObj)
		return;

	if (m_pObjHover == pObj || pObj->IsMyChild(m_pObjHover, true))
	{
		m_pObjHover = NULL;
        m_bMouseMoveReady = FALSE;
	}
	if (m_pObjPress == pObj || pObj->IsMyChild(m_pObjPress, true))
	{
		m_pObjPress = NULL;
        m_bMouseMoveReady = FALSE;
	}
    if (m_pObjRPress == pObj || pObj->IsMyChild(m_pObjRPress, true))
    {
        m_pObjRPress = NULL;
    }
    if (m_pObjMPress == pObj || pObj->IsMyChild(m_pObjMPress, true))
    {
        m_pObjMPress = NULL;
    }
    if (m_pObjGesture == pObj || pObj->IsMyChild(m_pObjGesture, true))
    {
        m_pObjGesture = NULL;
    }
    if (m_pObjDefault == pObj || pObj->IsMyChild(m_pObjDefault, true))
    {
        m_pObjDefault = NULL;
    }
    if (m_pObjOriginDefault == pObj || pObj->IsMyChild(m_pObjOriginDefault, true))
    {
        m_pObjOriginDefault = NULL;
    }

    if (pObj == m_pFocusObject || pObj->IsMyChild(m_pFocusObject, true))
    {
        m_pFocusObject = NULL;
    }
    if (pObj == m_pOldFocusObject)
    {
        m_pOldFocusObject = NULL;
    }

    if (pObj->GetIMessage() == m_pObjKeyboardCapture)
    {
        m_pObjKeyboardCapture = NULL;
        m_nKeyboardCaptureNotifyMsgId = 0;
    }
    if (pObj->GetIMessage() == m_pObjMouseCapture)
    {
        m_pObjMouseCapture = NULL;
        m_nMouseCaptureNotifyMsgId = 0;
    }
}


//
//	[�ݹ�+����] [public] [static] 
//
//	��ȡ��ǰ����µ���С��λ�ؼ�. �� ���ɼ��������ã������������Ϣ�Ŀؼ���Panel ��Щ���͵Ķ��󲻷��أ�
//
//	Parameter
//
//		pObjParent
//			[in]
//				�ݹ����ʱ��ʹ�õ�parent object.���øú���ʱ��ֵΪ���ڶ���(Window/)pObj���ɡ�
//		ptParent
//			[in]
//				�ڸ������е����λ�ã���ʼֵΪ�ڴ����е�λ��
// 
//      bUIEditorInvoke
//          [in]
//              ��UIBuilder��ֻ��Ҫһ��������CONTROL���粻����SliderCtrl�е�button��
//              ����UIBuilder����Ҫ���Ҳ����������Ϣ���͵Ŀؼ�����Label, Picture
//
//	Return
//		��λ���µĶ���û���򷵻ؿ�
//
Object* WindowMouseMgr::GetObjectByPos(
		Object* pObjParent, POINT* ptParent, __out POINT* ptOutInObj)
{
	GetObjectByPosExData  data = {0};
	data.pObjParent = pObjParent;
	data.ptParent = ptParent;
	data.ptOutInObj = ptOutInObj;
	data.bTestDecendant = true;
	data.bTestRejectMouseMsg = true;
	data.bTestDisable = false;
	return GetObjectByPosEx(&data);
}

Object*  WindowMouseMgr::GetObjectByPos_UIEditor(
		Object* pObjParent, POINT* ptParent, __out POINT* ptOutInObj)
{
	GetObjectByPosExData  data = {0};
	data.pObjParent = pObjParent;
	data.ptParent = ptParent;
	data.ptOutInObj = ptOutInObj;
	data.bTestDecendant = true;
	data.bTestRejectMouseMsg = false;
	data.bTestDisable = false;
	return GetObjectByPosEx(&data);
}

//  �����������GetObjectByPos�У�����Ӱ��GetObjectByPos��Ч��
Object*  WindowMouseMgr::GetObjectByPosEx(GetObjectByPosExData* pData)
{
	Object*  pObjParent = pData->pObjParent;
	POINT*  ptParent = pData->ptParent;
	POINT*  ptOutInObj = pData->ptOutInObj;

    Object*  pHitObj = NULL;
    POINT  ptHitTest = *ptParent;
    POINT  ptInChild = {0};

    Object* pChild = NULL;
    while (pChild = pObjParent->EnumNcChildObject(pChild))
    {
        if (!pChild->IsVisible())
            continue;

		if (pData->bTestDisable)
		{
			if (!pChild->IsEnable())  // ע��disable�ؼ�Ӧ������tooltips
				continue;
		}

		if (pData->bTestRejectMouseMsg)
		{
			if (pChild->IsRejectMouseMsgAll())
				continue;
		}

        UINT nHitTest = UISendMessage(pChild, UI_MSG_HITTEST, (WPARAM)&ptHitTest, (LPARAM)&ptInChild);
        if (HTNOWHERE != nHitTest)
        {
            pHitObj = pChild;
            break;
        }
    }

    if (NULL == pHitObj)  // ���������ͻ������Ӷ���
    {
        REGION4 rcNonClient = {0};
        pObjParent->GetNonClientRegion(&rcNonClient);

        ptHitTest.x -= rcNonClient.left;
        ptHitTest.y -= rcNonClient.top;

        POINT ptOffset = {0,0};
        if (pObjParent->GetScrollOffset((int*)&ptOffset.x, (int*)&ptOffset.y))
        {
            ptHitTest.x += ptOffset.x;
            ptHitTest.y += ptOffset.y;
        }

        pChild = NULL;
        while (pChild = pObjParent->REnumChildObject(pChild))  // ��������������ΪԽ����Ĳ���ʾ������
        {
            if (!pChild->IsVisible())
                continue;

			if (pData->bTestDisable)
			{
				if (!pChild->IsEnable())  // ע��disable�ؼ�Ӧ������tooltips
					continue;
			}

			if (pData->bTestRejectMouseMsg)
			{
				if (pChild->IsRejectMouseMsgAll())
					continue;
			}

            UINT nHitTest = UISendMessage(pChild, UI_MSG_HITTEST, (WPARAM)&ptHitTest, (LPARAM)&ptInChild);
            if (HTNOWHERE != nHitTest)
            {
                pHitObj = pChild;
                break;
            }
        }
    }

    if (NULL == pHitObj)
        return NULL;

    // ������ת��Ϊ�����pHitObj������
    ptHitTest = ptInChild;;
    POINT  ptInHitChild = {0};
    Object* pRetObj = NULL;
	if (pData->bTestDecendant)
	{
		GetObjectByPosExData data = {0};
		data.pObjParent = pHitObj;
		data.ptParent = &ptHitTest;
		data.ptOutInObj = &ptInHitChild;
		data.bTestDecendant = pData->bTestDecendant;
		data.bTestDisable = pData->bTestDisable;
		data.bTestRejectMouseMsg = pData->bTestRejectMouseMsg;
		pRetObj = GetObjectByPosEx(&data);
	}

	// Panel�����������Ϣ����Panel�е��Ӷ�����Ҫ���գ�
	if (pRetObj && pData->bTestRejectMouseMsg)
	{
		if (pRetObj->IsRejectMouseMsgSelf() || pRetObj->IsRejectMouseMsgAll())
			pRetObj = NULL;
	}

    // ���ӿؼ���û�д��ڸ�λ�õģ�ֱ�ӷ��ر�����
    if (pRetObj)
    {
        if (ptOutInObj)
            *ptOutInObj = ptInHitChild;
        return pRetObj;
    }

	if (pHitObj && pData->bTestRejectMouseMsg)
	{
		if (pHitObj->IsRejectMouseMsgSelf() || pHitObj->IsRejectMouseMsgAll())
			pHitObj = NULL;
	}

    if (pHitObj && ptOutInObj)
        *ptOutInObj = ptInChild;
    return pHitObj;
}


void  WindowMouseMgr::SetUIApplication(UIApplication* p)
{
    m_pUIApplication = p;
}


//////////////////////////////////////////////////////////////////////////

UIApplication*  WindowMouseMgr::GetUIApplication()
{
	if (NULL == m_oWindow)
		return NULL;

	return m_oWindow.GetUIApplication();
}

WindowBase* WindowMouseMgr::GetWindowObject() 
{ 
    return &m_oWindow; 
}


LRESULT  WindowMouseMgr::HandleMessage(UINT msg, WPARAM w, LPARAM l, BOOL* pbHandled)
{
    switch (msg)
    {
    case WM_KILLFOCUS:
        this->OnKillFocus((HWND)w);
        break;

    case WM_SETFOCUS:
        this->OnSetFocus();
        break;

    case WM_NCDESTROY:
        this->OnNcDestroy();
        break;

    case WM_SETCURSOR:
        return this->OnSetCursor(w, l);
        break;
    }
    return 0L;
}
//
//The low-order word specifies the x-coordinate of the cursor. The coordinate is relative to the upper-left corner of the client area. 
//The high-order word specifies the y-coordinate of the cursor. The coordinate is relative to the upper-left corner of the client area. 
//
LRESULT WindowMouseMgr::HandleMouseMessage(UINT msg, WPARAM w, LPARAM l, BOOL* pbHandled)
{
    if (this->m_pObjMouseCapture)
    {
        BOOL bHandled = FALSE;
        LRESULT lRet = UISendMessage(m_pObjMouseCapture, msg, w, l, 0,0, m_nMouseCaptureNotifyMsgId, &bHandled);
        if (bHandled)
        {
            if (pbHandled)
                *pbHandled = TRUE;

            return lRet;
        }
    }

	int vkFlag = (int)w;
	int xPos = GET_X_LPARAM ( l );
	int yPos = GET_Y_LPARAM ( l );

	switch( msg )
	{
	case WM_MOUSEMOVE:
		{
			if (m_bMouseTrack)    //������׷�٣���
			{
				TRACKMOUSEEVENT tme;
				tme.cbSize = sizeof( TRACKMOUSEEVENT );
				tme.dwFlags = TME_LEAVE;
				tme.dwHoverTime = HOVER_DEFAULT;
				tme.hwndTrack = this->m_oWindow.GetHWND();
				::TrackMouseEvent( &tme );
				m_bMouseTrack = FALSE;
			}

			Object* pOldHover = m_pObjHover;
			Object* pOldPress = m_pObjPress;
			LRESULT lRet = this->OnMouseMove( vkFlag, xPos, yPos );

			if (pOldPress==m_pObjPress && pOldHover!=m_pObjHover)  // ����µĶ������仯����Ҫ���������ʽ
            {
                HWND hWnd = m_oWindow.GetHWND();
				if (GetCapture() != hWnd)             // �����קʱ���䣬�����EDIT����ק����
			    {
				    ::PostMessage(hWnd, WM_SETCURSOR, (WPARAM)hWnd, MAKELPARAM(HTCLIENT,1));  // hiword 0��ʾ�����˵�
			    }
            }
			return lRet;
		}
		break;

	case WM_MOUSELEAVE:
		return this->OnMouseLeave( vkFlag, xPos, yPos );
		break;

	case WM_CANCELMODE:
		return this->OnCancelMode(w,l);

	case WM_LBUTTONDOWN:
		return this->OnLButtonDown(w, l, pbHandled);
		break;

	case WM_LBUTTONUP:
		return this->OnLButtonUp(w, l);
		break;

	case WM_RBUTTONDOWN:
		return this->OnRButtonDown(w,l);
		break;

	case WM_RBUTTONUP:
		return this->OnRButtonUp(w,l);
		break;

	case WM_LBUTTONDBLCLK:
		//this->OnLButtonDown( vkFlag, xPos, yPos );
		return this->OnLButtonDBClick(w,l, pbHandled);
		break;

    case WM_MBUTTONDBLCLK:
        return this->OnMButtonDBClick(w,l);
        break;
    case WM_MBUTTONDOWN:
        return this->OnMButtonDown(w, l);
        break;
    case WM_MBUTTONUP:
        return this->OnMButtonUp(w, l);
        break;

    case WM_MOUSEWHEEL:
        return this->OnMouseWheel(w,l);  // ����Ϣ������Ҫ���׸������ڴ���
	}
	return 0L;
}

LRESULT  WindowMouseMgr::HandleKeyboardMessage(UINT msg, WPARAM w, LPARAM l, BOOL* pbHandled)
{
    if (this->m_pObjKeyboardCapture)
    {
        BOOL bHandled = FALSE;
        LRESULT lRet = UISendMessage(m_pObjKeyboardCapture, msg, w, l, 0,0, m_nKeyboardCaptureNotifyMsgId, &bHandled);
        if (bHandled)
        {
            if (pbHandled)
                *pbHandled = TRUE;

            return lRet;
        }
    }

    switch( msg )
    {
    case WM_CHAR:
        return this->OnChar(w, l);
        break;

    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        return this->OnKeyDown(msg, w,l );
        break;

    case WM_KEYUP:
    case WM_SYSKEYUP:
        return this->OnKeyUp(w, l);
        break;

    case WM_IME_STARTCOMPOSITION:
    case WM_IME_ENDCOMPOSITION:
    case WM_IME_COMPOSITION:
    case WM_IME_SETCONTEXT:
    case WM_IME_NOTIFY:
    case WM_IME_CONTROL:
    case WM_IME_COMPOSITIONFULL:
    case WM_IME_SELECT:
    case WM_IME_CHAR:
    case WM_IME_REQUEST:
    case WM_IME_KEYDOWN:
    case WM_IME_KEYUP:
        {
            return this->OnImeMsg(msg, w, l, pbHandled);
        }
    }
    return 0L;
}

LRESULT  WindowMouseMgr::HandleTouchMessage(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    bHandled = FALSE;
	
    if (msg == WM_GESTURE)
    {
        // ::OutputDebugStringA("WindowMouseMgr::HandleTouchMessage  WM_GESTURE\r\n");
        bHandled = OnGesture(lParam);
        return 0;
    }
    else if (msg == WM_GESTURENOTIFY)
    {
        if (!GetGestureInstance()->IsValid())
            return 0;

        // Gives you a chance to set the gesture configuration. 
        // This message should always be bubbled up using the DefWindowProc function.
        bHandled = FALSE;
        //::OutputDebugStringA("WindowMouseMgr::HandleTouchMessage  WM_GESTURENOTIFY\r\n");

        UI::GESTURENOTIFYSTRUCT* pStruct = (UI::GESTURENOTIFYSTRUCT*)lParam;  pStruct;

		DWORD  panWant = GC_PAN |
						 GC_PAN_WITH_SINGLE_FINGER_VERTICALLY |   						 
						 GC_PAN_WITH_GUTTER |                     
						 GC_PAN_WITH_INERTIA;
		DWORD  panBlock = GC_PAN_WITH_SINGLE_FINGER_HORIZONTALLY;  // �ݲ�����������PAN������֧��ListItemDragDrop
        // TODO: ��չ�������ؼ�������Ҫ����pan

		UI::GESTURECONFIG  gc[] = //{0, GC_ALLGESTURES, 0};
		{
			{ GID_PAN, panWant, panBlock },
			{ GID_ZOOM, GC_ZOOM, 0 },
			{ GID_ROTATE, GC_ROTATE, 0 },
			{ GID_TWOFINGERTAP, GC_TWOFINGERTAP, 0 },
			{ GID_PRESSANDTAP, GC_PRESSANDTAP, 0}
		};
        BOOL bResult = GetGestureInstance()->pSetGestureConfig(m_oWindow.m_hWnd, 0, 5, gc, sizeof(UI::GESTURECONFIG));

        if (!bResult)
        {
            // an error
        }
        return 0;
    }
    return 0;
}

Object*  WindowMouseMgr::GetGestureTargetObject(POINT ptScreen, WPARAM wParam)
{
    MapWindowPoints(NULL, m_oWindow.GetHWND(), &ptScreen, 1);
    Object* p = GetObjectByPos(&m_oWindow, &ptScreen, NULL);
    while (p)
    {
        if (UISendMessage(p->GetIMessage(), UI_WM_GESTURE_BEGIN_REQ, wParam))
        {
            return p;
        }
        p = p->GetParentObject();
    }
    return NULL;
}

BOOL  WindowMouseMgr::OnGesture(LPARAM lParam)
{
	// ������Ϣ֪ͨ��������
	static POINT lastPoint = {0};
	// ��һ�����Ƶ�ĿǰΪֹ���ܹ�������
	static SIZE overpan = {0};
	static ULONGLONG lastArguments = 0;

	HWND hWnd = m_oWindow.GetHWND();
    UI::HGESTUREINFO  hGestureInfo = (UI::HGESTUREINFO)lParam;

    UI::GESTUREINFO gi = {0}; 
    gi.cbSize = sizeof(UI::GESTUREINFO);
    GetGestureInstance()->pGetGestureInfo(hGestureInfo, &gi);
	
    BOOL bHandled = FALSE;

    bool bFirstGesture = (gi.dwFlags&GF_BEGIN) ? true:false;
    bool bLastGesture = (gi.dwFlags&GF_END) ? true:false;
    switch (gi.dwID)
    {
    case GID_PAN:
	case GID_PRESSANDTAP:
        {
			UINT msg = UI_WM_GESTURE_PAN;
			if (gi.dwID == GID_PRESSANDTAP)				
				msg = UI_WM_GESTURE_PRESSANDTAP;	

			int xOffset = 0;
			int yOffset = 0;

            if (bFirstGesture)
            {
                POINT pt = {gi.ptsLocation.x, gi.ptsLocation.y};
                m_pObjGesture = GetGestureTargetObject(pt, (WPARAM)&gi);
            }
			else
			{
				xOffset = (gi.ptsLocation.x - lastPoint.x);
				yOffset = (gi.ptsLocation.y - lastPoint.y);
			}

			if (bFirstGesture)
			{
				GetGestureInstance()->pBeginPanningFeedback(hWnd);
				overpan.cx = overpan.cy = 0;
			}
			else if (bLastGesture)
			{
				GetGestureInstance()->pEndPanningFeedback(hWnd, TRUE);
				overpan.cx = overpan.cy = 0;
			}

			overpan.cx += xOffset;
			overpan.cy += yOffset;

			if (m_pObjGesture)
            {
                long lRet = UISendMessage(
					m_pObjGesture->GetIMessage(),
					msg,
					MAKEWPARAM((short)xOffset, (short)yOffset),
					(LPARAM)&gi);

				if (lRet > 0)
					bHandled = TRUE;

				if (lRet == GESTURE_RETURN_NEED_BOUNCE_EDGE)
				{
					GetGestureInstance()->pUpdatePanningFeedback(
						hWnd, 0, overpan.cy, gi.dwFlags & GF_INERTIA);
				}

                if (bLastGesture)
                    m_pObjGesture = NULL;       
            }
        }
        break;
    }

	//Remember last values for delta calculations
	lastPoint.x = gi.ptsLocation.x;
	lastPoint.y = gi.ptsLocation.y;
	lastArguments = gi.ullArguments;

    GetGestureInstance()->pCloseGestureInfoHandle(hGestureInfo);
    return bHandled;
}

//
//	Parameter
//		vkFlag
//			[in]
//
//		xPos, yPos
//			[in]	��ǰ���λ�ã��Դ������Ͻ�Ϊ(0,0)���
//
LRESULT WindowMouseMgr::OnMouseMove( int vkFlag, int xPos, int yPos )
{	
    this->m_bMouseMoveReady = TRUE;

	// 1. �жϵ�ǰ���λ��
	POINT pt = { xPos, yPos };
    POINT ptInObj = {0};

	Object* pObj = this->GetObjectByPos(&m_oWindow, &pt, &ptInObj);

	_OnMouseMove(pObj, vkFlag, MAKELPARAM(pt.x, pt.y), this);
	return 0L;
}


LRESULT WindowMouseMgr::OnMouseLeave( int vkFlag, int xPos, int yPos )
{
	_OnMouseLeave(this);

	this->m_bMouseTrack = TRUE;// ��������TRACKMOUSEEVENT
    this->m_bMouseMoveReady = FALSE;

	if (GetCapture() == m_oWindow.GetHWND())  // �п�������������ֱ�ӷ��͹���WM_MOUSELEAVE������COMBOBOX.button.onlbuttondown
	{
		::ReleaseCapture();
	}
	return TRUE;
}

LRESULT WindowMouseMgr::OnCancelMode(WPARAM w, LPARAM l)
{
	if (m_pObjPress != NULL)
	{
        UIMSG msg;
		msg.pMsgTo = m_pObjPress->GetIMessage();
		msg.message = WM_CANCELMODE;
		msg.wParam  = w;
		msg.lParam  = l;
		::UISendMessage(&msg);
	}

	return 0;
}


LRESULT WindowMouseMgr::OnLButtonDown( WPARAM w, LPARAM l, BOOL* pbHandled)
{
    // ���汾�ε��λ�ڣ����ڹ���˫���¼�
    m_posPrevClick = l;

    if (FALSE == this->m_bMouseMoveReady)
    {
        HandleMouseMessage(WM_MOUSEMOVE, 0, l, NULL);  // ע��Ҫȡ��wParam�е���갴�±��
    }

	if (m_pObjHover)
		::SetCapture(this->m_oWindow.GetHWND());

	_OnLButtonDown(w, l, pbHandled, this);
	return 0L;
}

LRESULT WindowMouseMgr::OnLButtonUp( WPARAM w, LPARAM l)
{
    // Ϊ�˷�ֹ�ڶ����ڴ���WM_LBUTTONUP��ϢʱMouseManager��״̬�����˸ı�,�ȱ���״̬
    Object*  pSaveObjPress = m_pObjPress;
    Object*  pSaveObjHover = m_pObjHover;

    // ���ͷ�capture��up����Ӧ�����У��簴ťclick�����ܵ���
    // �˵�����������ģʽ����captureȴһֱ���ǰ�ť�������档
    //
    // ���Ҳ��ܷ���if (m_pObjPress)�С�m_pObjPress�п�����;�����
    if (GetCapture() == m_oWindow.GetHWND()) 
        ::ReleaseCapture();

	if (m_pObjPress)
	{
        _OnLButtonUp(w, l, this);

        // ������ʱ������뿪�˿ؼ�������MOUSELEAVE��Ϣ. ���ʱ�����λ�ÿ����Ѿ����±仯�ˣ���Ҫ���»�ȡ
        POINT ptCursorNow;
        GetCursorPos(&ptCursorNow);
        ::MapWindowPoints(NULL, m_oWindow.GetHWND(), &ptCursorNow, 1);

        Object* pNowHover = this->GetObjectByPos(&m_oWindow, &ptCursorNow, NULL);
        this->SetHoverObject(pNowHover);
        if (pNowHover != pSaveObjPress && NULL != pSaveObjPress)
            ::UISendMessage(pSaveObjPress, WM_MOUSELEAVE);
        if (pNowHover != pSaveObjHover && NULL != pNowHover)
            ::UISendMessage(pNowHover, WM_MOUSEMOVE, w, l );
	}
	 
	return 0L;
}

LRESULT WindowMouseMgr::OnLButtonDBClick( WPARAM w,LPARAM l, BOOL* pbHandled )
{
    if (!AdjustDoubleClickMessage(l))
        return OnLButtonDown(w, l, pbHandled);

    if (FALSE == this->m_bMouseMoveReady)
    {
        HandleMouseMessage(WM_MOUSEMOVE, 0, l, NULL);  // ע��Ҫȡ��wParam�е���갴�±��
    }

	if (m_pObjPress || m_pObjHover)
	{
		::SetCapture(this->m_oWindow.GetHWND());
	}
	
    _OnLButtonDBClick(w, l, pbHandled, this);
	return 0;
}

LRESULT WindowMouseMgr::OnRButtonDown( WPARAM w,LPARAM l )
{
    if (FALSE == this->m_bMouseMoveReady)
    {
        HandleMouseMessage(WM_MOUSEMOVE, 0, l, NULL);
    }

    Object* p = m_pObjHover;
    if (NULL == p)
        return 0;

    m_pObjRPress = p;
    if (p && p->CanTabstop())
    {
        SetFocusObject(p);
    }

	if (p)
	{
		GetUIApplication()->HideToolTip();
		return ::UISendMessage(p, WM_RBUTTONDOWN, w, l);
	}
	
	return 0;
}
LRESULT WindowMouseMgr::OnRButtonUp( WPARAM w,LPARAM l )
{
	if (m_pObjRPress)
	{
		LRESULT lRet = ::UISendMessage(m_pObjRPress, WM_RBUTTONUP, w, l);
        m_pObjRPress = NULL;
        return lRet;
	}
// 	else if (m_pObjHover)  // ��hoverʱ������up��Ϣ����Ϊ�п���������ڱ�ĵط������Ȼ���Ƶ�hover���ͷ�
// 	{
// 		return ::UISendMessage(m_pObjHover, WM_RBUTTONUP, w, l);
// 	}
	return 0;
}

LRESULT  WindowMouseMgr::OnMButtonDown(WPARAM w,LPARAM l)
{
    if (FALSE == this->m_bMouseMoveReady)
        HandleMouseMessage(WM_MOUSEMOVE, 0, l, NULL);

    if (m_pObjHover)
    {
        m_pObjMPress = m_pObjHover;
        GetUIApplication()->HideToolTip();
        ::UISendMessage(m_pObjMPress, WM_MBUTTONDOWN, w, l);
    }

    return 0;
}
LRESULT  WindowMouseMgr::OnMButtonDBClick(WPARAM w,LPARAM l)
{
    if (FALSE == this->m_bMouseMoveReady)
        HandleMouseMessage(WM_MOUSEMOVE, 0, l, NULL);

    if (m_pObjHover)
    {
        m_pObjMPress = m_pObjHover;
        GetUIApplication()->HideToolTip();
        ::UISendMessage(m_pObjMPress, WM_MBUTTONDBLCLK, w, l);
    }
    return 0;
}
LRESULT  WindowMouseMgr::OnMButtonUp(WPARAM w,LPARAM l)
{
    if (m_pObjMPress)
    {
        ::UISendMessage(m_pObjMPress, WM_MBUTTONUP, w, l);
        m_pObjMPress = 0;
    }
    return 0;
}

// �ú���ͬʱ��֧����WM_CANCELMODE������
void WindowMouseMgr::OnKillFocus(HWND hFocusWnd)
{
	if (this->m_pObjHover)
	{
		::UISendMessage(m_pObjHover, WM_MOUSELEAVE);
	}
	if (this->m_pObjPress)
	{
        HWND hWndCapture = GetCapture();
        HWND hWnd = m_oWindow.GetHWND();
        if (hWndCapture == hWnd) 
        {
            ::ReleaseCapture();
        }

		if (this->m_pObjPress != this->m_pObjHover)  // ���⽫WM_MOUSELEAVE��������
			::UISendMessage(m_pObjPress, WM_MOUSELEAVE);
	}
    else
    {
        HWND hWndCapture = GetCapture();
        if (hWndCapture) 
        {
            // �Ӵ�����setcapture��Ȼ���ⲿ����EnableWindow(������,FALSE)
            // �������������յ�killfocus��Ϣ���Ӵ��ڵ�captureû�б�release
            HWND hWnd = m_oWindow.GetHWND();
            if (hWndCapture == hWnd || ::IsChild(hWnd, hWndCapture))
            {
                ::ReleaseCapture();
            }
        }
    }

	this->SetHoverObject(NULL);
	this->SetPressObject(NULL);
    this->m_bMouseMoveReady = FALSE;  // ʧ�����ֱ�Ӵ�����갴�»ᵼ��״̬����ȷ

    m_pObjKeyboardCapture = NULL;
    m_nKeyboardCaptureNotifyMsgId = 0;
    m_pObjMouseCapture = NULL;
    m_nMouseCaptureNotifyMsgId = 0;

    // TODO: KEYBOARD
    Object* p = this->GetOldFocusObject();
    if (NULL == p)
        p = m_pFocusObject;

    if (p)  // ��Focus�����ͽ�����Ϣ����������m_pFocusObject���������㵽�����ϵ��µ�ʧ��
    {
        p->SetFocus(false);
        ::UISendMessage(p, WM_KILLFOCUS, (WPARAM)NULL, (LPARAM)0);
    }

    // ������ǵ����˱����������һ��HOSTWND����Ӵ���ʱ������m_pFocusObject����������ٵ��m_pFocusObjectʱ��û��Ӧ
    if (m_oWindow)
    {
        HWND hWnd = m_oWindow.GetHWND();
        if (IsChild(hWnd, hFocusWnd))
            this->SetFocusObjectDirect(NULL);
    }
}

void WindowMouseMgr::OnSetFocus()
{
    if (m_pFocusObject && !m_pFocusObject->IsFocus())
    {
        m_pFocusObject->SetFocus(true);
        ::UISendMessage(m_pFocusObject, WM_SETFOCUS, (WPARAM)GetOldFocusObject(), (LPARAM)0);  // �����GetOldFocusObject��Ӧ��
    }
    else
    {
        this->Tab_2_NextControl();
    }
}

BOOL WindowMouseMgr::OnSetCursor( WPARAM w,LPARAM l )
{
    Object* pObj = m_pObjPress;
    if (NULL == pObj)
        pObj = m_pObjHover;

    BOOL bHandled = FALSE;
	if (pObj)
	{
		::UISendMessage(pObj->GetIMessage(), WM_SETCURSOR, w, l, 0, 0, 0, &bHandled);
	}
	return bHandled;
}

void WindowMouseMgr::OnNcDestroy()
{
	m_pObjHover = m_pObjPress = NULL;  // ������SetHoverObject/SetPressObject
    m_pFocusObject = NULL;	// ������SetFocusObject����Ϊ���漰ˢ����
    m_pOldFocusObject = NULL;
    m_pObjKeyboardCapture = NULL;
}

/*
Code                   Meaning
------------------------------------------------------------------------

DLGC_BUTTON            Control is a button (of any kind).
DLGC_DEFPUSHBUTTON     Control is a default push button.
DLGC_HASSETSEL         Windows will send an EM_SETSEL message to the control to select its contents.
DLGC_RADIOBUTTON       Control is an option (radio) button.
DLGC_STATIC            Control is a static control.
DLGC_UNDEFPUSHBUTTON   Control is a push button but not the default push button.(��һ����ť��������default)
DLGC_WANTALLKEYS       Control processes all keyboard input.
DLGC_WANTARROWS        Control processes arrow keys.
DLGC_WANTCHARS         Control processes WM_CHAR messages.
DLGC_WANTMESSAGE       Control processes the message in the MSG structure that lParam points to.
DLGC_WANTTAB           Control processes the TAB key.
*/

void WindowMouseMgr::CheckDefPushButton(Object* pNewObj)
{
    if (NULL == pNewObj)
        return;

    UINT codeNewFocus = UISendMessage(pNewObj->GetIObject(), WM_GETDLGCODE);

    if (pNewObj == m_pFocusObject)
    {
        if (codeNewFocus & DLGC_UNDEFPUSHBUTTON)
        {
            SetDefaultObject(pNewObj, true);
        }
        return;
    }

    if (codeNewFocus & DLGC_DEFPUSHBUTTON)
        return;

    Object* pLastDefaultObj = m_pObjDefault; // ���ڼ�������defbtn

    /*
     * If the focus is changing to or from a pushbutton, then remove the
     * default style from the current default button
     */
    if ((m_pFocusObject != NULL && (UISendMessage(m_pFocusObject, WM_GETDLGCODE) & (DLGC_DEFPUSHBUTTON | DLGC_UNDEFPUSHBUTTON))) ||
        (pNewObj != NULL && (codeNewFocus & (DLGC_DEFPUSHBUTTON | DLGC_UNDEFPUSHBUTTON))))
    {
        pLastDefaultObj = NULL;
    }

    /*
     * If moving to a button, make that button the default.
     */
    if (codeNewFocus & DLGC_UNDEFPUSHBUTTON)
    {
        pLastDefaultObj = pNewObj;
    }
    else
    {
        /*
         * Otherwise, make sure the original default button is default
         * and no others.
         */
        pLastDefaultObj = m_pObjOriginDefault;
    }

    SetDefaultObject(pLastDefaultObj, true);
}

BOOL WindowMouseMgr::IsDialogMessage(MSG* pMsg)
{
    if (pMsg->message == WM_LBUTTONDOWN)
    {
        CheckDefPushButton(m_pObjHover);
    }

    if (pMsg->message == WM_KEYDOWN)
    {
        if (VK_TAB == pMsg->wParam)
        {
            LRESULT lRet = SendMessage(m_oWindow.m_hWnd, WM_QUERYUISTATE, 0, 0);
            if (lRet & UISF_HIDEFOCUS)
            {
                lRet &= ~UISF_HIDEFOCUS;
                SendMessage(m_oWindow.m_hWnd, WM_UPDATEUISTATE, lRet|UIS_SET, 0);
            }
            Object* pFocusObj = GetFocusObject();
            if (pFocusObj)
            {
                LRESULT lr = UISendMessage(pFocusObj->GetIMessage(), WM_GETDLGCODE, pMsg->wParam, (LPARAM)pMsg);
                if (lr & DLGC_WANTTAB)
                    return FALSE;
            }

            // ����
            if (Util::IsKeyDown(VK_SHIFT))
            {
                Tab_2_PrevControl();
            }
            else
            {
                Tab_2_NextControl();
            }
            return TRUE;
        }
        else if (VK_LEFT == pMsg->wParam)
        {

        }
        else if (VK_RIGHT == pMsg->wParam)
        {

        }
        else if (VK_UP == pMsg->wParam)
        {

        }
        else if (VK_DOWN == pMsg->wParam)
        {

        }
        else if (VK_RETURN == pMsg->wParam || VK_EXECUTE == pMsg->wParam)
        {
            if (m_pFocusObject)
            {
                // �����ڶ��б༭���лس�
                UINT codeFocus = UISendMessage(m_pFocusObject, WM_GETDLGCODE, 0, (LPARAM)pMsg);
                if (codeFocus & (DLGC_WANTMESSAGE|DLGC_WANTALLKEYS))
                    return FALSE;
            }
            
            // Windows�������Ǹ����ڷ���һ��WM_COMMAND( id, hwnd )����Ϣ
            // ����default button�����͸�defpushbutton
            if (m_pObjDefault && 
                m_pObjDefault->IsEnable() && 
                m_pObjDefault->IsVisible())
            {
                UIMSG   msg;
                msg.message = UI_MSG_DEFAULTBUTTON_VKRETURN_EVENT;
                msg.pMsgTo = m_pObjDefault->GetIMessage();
                UISendMessage(&msg);
                return TRUE;
            }
        }
        else if (VK_ESCAPE == pMsg->wParam || VK_CANCEL == pMsg->wParam)
        {
            // ����ȡ����ť��������Ϣ
        }
    }
    else if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_MENU)
    {
        LRESULT lRet = SendMessage(m_oWindow.m_hWnd, WM_QUERYUISTATE, 0, 0);
        if (lRet & UISF_HIDEFOCUS)
        {
            lRet &= ~UISF_HIDEFOCUS;
            SendMessage(m_oWindow.m_hWnd, WM_UPDATEUISTATE, lRet|UIS_SET, 0);

            if (m_pFocusObject)
            {
                m_pFocusObject->Invalidate();
            }
        }
    }
    
    return FALSE;
}


void WindowMouseMgr::SetKeyboardCapture(IMessage* pObj, int nNotifyMsgId)
{
    m_pObjKeyboardCapture = pObj;
    m_nKeyboardCaptureNotifyMsgId = nNotifyMsgId;
}
void WindowMouseMgr::ReleaseKeyboardCapture(IMessage* pObj)
{
    if (pObj == m_pObjKeyboardCapture)
    {
        m_pObjKeyboardCapture = NULL;
        m_nKeyboardCaptureNotifyMsgId = 0;
    }
}
void  WindowMouseMgr::SetMouseCapture(IMessage* pObj, int nNotifyMsgId)
{
    m_pObjMouseCapture = pObj;
    m_nMouseCaptureNotifyMsgId = nNotifyMsgId;
}
void  WindowMouseMgr::ReleaseMouseCapture(IMessage* pObj)
{
    if (pObj == m_pObjMouseCapture)
    {
        m_pObjMouseCapture = NULL;
        m_nMouseCaptureNotifyMsgId = 0;
    }
}

void WindowMouseMgr::Tab_2_NextControl()
{
    Object* p = m_pFocusObject;
    if (!p)
        p = &m_oWindow;

    if (!p)
        return;

    p = p->GetNextTabObject();

    if (p)
    {
        CheckDefPushButton(p);
        SetFocusObject(p);
    }
}
void WindowMouseMgr::Tab_2_PrevControl()
{
    Object* p = m_pFocusObject;
    if (NULL == p)
    {
        p = &m_oWindow;
    }
    p = p->GetPrevTabObject();

    if (p)
    {
        CheckDefPushButton(p);
        SetFocusObject(p);
    }
}


Object*  WindowMouseMgr::GetOldFocusObject(bool bRemove)
{
    if (bRemove)
    {
        Object* p = m_pOldFocusObject;
        m_pOldFocusObject = NULL;
        return p;
    }
    else
    {
        return m_pOldFocusObject;
    }
}

void WindowMouseMgr::SetFocusObjectDirect(Object* pObj )
{
    m_pFocusObject = pObj;
}

Object*  WindowMouseMgr::GetFocusObject()
{
    return m_pFocusObject; 
}

void  WindowMouseMgr::SetDefaultObject(Object* pObj, bool bUpdate)
{
    if (pObj == m_pObjDefault)
        return;

    if (m_pObjDefault)
        m_pObjDefault->SetDefault(false, bUpdate);

    m_pObjDefault = pObj;

    if (m_pObjDefault)
        m_pObjDefault->SetDefault(true, bUpdate);
}


Object*  WindowMouseMgr::GetOriginDefaultObject()
{
    return m_pObjOriginDefault;
}
void  WindowMouseMgr::SetOriginDefaultObject(Object* pObj)
{
    m_pObjOriginDefault = pObj;
}
Object*  WindowMouseMgr::GetDefaultObject()
{
    return m_pObjDefault;
}


//
//	�ú���ֻ���������ط�����:
//		a. �����ѡ��һ��UI�ؼ�
//		b. ʹ��TAB�л��ؼ�
//
//	��˵������ѡ��һ��ϵͳ�ؼ�ʱ����Ҫ��HWNDHOST�е���һ��SetFocusObjectDirect
//
void WindowMouseMgr::SetFocusObject(Object* pObj)
{
    // TODO: ��������ᵼ�½����ϵͳ��combobox.edit�����ui combobox.editʱû��Ӧ
    //       ��������������ᵼ��UI COMBOBOX�е�EDIT�޷���קѡ�У���Ϊһֱ��KILLFOCUS/SETFOCUS��
    if (m_pFocusObject == pObj)  
        return;

    if (pObj)
    {

        if (pObj->GetDescription() && OBJ_HWNDHOST == pObj->GetDescription()->GetMajorType())
        {
            HWND hWndFocus = ::GetFocus();
            HWND hWndHost = ((HwndHost*)pObj)->m_hWnd;
            if (hWndFocus != hWndHost && FALSE == ::IsChild(hWndHost, hWndFocus))  // �п��ܵ����combobox�е�edit,edit����һ��HOSTWND������HOSTWND���Ӵ���
            {
                m_pOldFocusObject = m_pFocusObject;   // ��HwndHost::WndProc WM_SETFOCUS��ʹ��
                m_pFocusObject = pObj;

                if (GetWindowLong(hWndHost, GWL_EXSTYLE)&WS_EX_NOACTIVATE)
                {}
                else
                    ::SetFocus(hWndHost);  
                return;
            }
        }
        else
        {
            if (m_oWindow && ::GetFocus() != m_oWindow.GetHWND())
            {
                m_pOldFocusObject = m_pFocusObject;   // ��KeyboardManager::SetFocus��ʹ��
                m_pFocusObject = pObj;

                HWND hWnd = m_oWindow.GetHWND();
                if (!IsWindowVisible(hWnd) || GetWindowLong(hWnd, GWL_EXSTYLE)&WS_EX_NOACTIVATE)
                {

                }
                else
                {
                    ::SetFocus(hWnd);  
                }

                return;
            }
        }
    }

    if (m_pFocusObject)
    {
        m_pFocusObject->SetFocus(false);
        ::UISendMessage(m_pFocusObject, WM_KILLFOCUS, (WPARAM)pObj, NULL );
    }
    if (pObj)
    {
        pObj->SetFocus(true);
        ::UISendMessage(pObj, WM_SETFOCUS, (WPARAM)m_pFocusObject, NULL );
    }
    m_pFocusObject = pObj;

    // ���ݽ�������л����뷨״̬
    // ���ڵ����뷨����������һ���ؼ��Ƿ���Ҫ�������뷨��
    // ���һ���ؼ����������뷨����ʹ�л����������뷨��Ҳ������ʾ���뷨��״̬���ġ�
	OBJSTYLE s = {0};
	s.enable_ime = 1;
    if (m_pFocusObject && m_pFocusObject->TestObjectStyle(s))
    {
        //ImmAssociateContext(m_oWindow.m_hWnd, m_hIMC);
        ImmAssociateContextEx(m_oWindow.m_hWnd, NULL, IACE_DEFAULT);
    }
    else
    {
        ImmAssociateContextEx(m_oWindow.m_hWnd, NULL, IACE_IGNORENOCONTEXT);
        //ImmAssociateContext(m_oWindow.m_hWnd, NULL);
    }
}


BOOL WindowMouseMgr::OnChar(WPARAM w, LPARAM l)
{	
    if (this->m_pFocusObject)
    {
        UISendMessage(m_pFocusObject, WM_CHAR, w, l);
        return TRUE;
    }
    return FALSE;
}

BOOL WindowMouseMgr::OnKeyDown(UINT nMsg, WPARAM w,LPARAM l)
{
    if (this->m_pFocusObject)
    {
        UISendMessage(m_pFocusObject, WM_KEYDOWN, w, l );
        return TRUE;
    }

    return FALSE;
}
BOOL WindowMouseMgr::OnKeyUp(WPARAM w,LPARAM l)
{
    if (this->m_pFocusObject)
    {
        UISendMessage(m_pFocusObject, WM_KEYUP, w, l);
        return TRUE;
    }

    return FALSE;
}

long WindowMouseMgr::OnMouseWheel(WPARAM w, LPARAM l)
{
    // ��ѯ�ʵ�ǰ��press obj���ܷ����mouse wheel�ַ�
    if (m_pObjPress)
    {
        BOOL  bHandled = UISendMessage(m_pObjPress->GetIMessage(), UI_MSG_MOUSEWHEEL_REQ, w, l);
        if (bHandled)
            return 0;
    }

    // �Ƚ�������µ�hover������
    if (m_pObjHover)
    {
        BOOL  bHandled = UISendMessage(m_pObjHover->GetIMessage(), WM_MOUSEWHEEL, w, l);
        if (bHandled)
            return 0;
    }

    // ����������û�������׸��������ĸ�����
    // ����һ��λ��ScrollPanel�е�Edit����Edit
    // �н���MouseWheel�Ļ�����Ӧ�ù���ScrollPanel

    Object*  pObj = m_pFocusObject;
	if (!pObj)
	{
		// û��focusʱ��ֱ�Ӹ�hover�������ð��
		if (m_pObjHover)
			pObj = m_pObjHover->GetParentObject();
	}

    while (pObj)
    {
        BOOL  bHandled = UISendMessage(pObj->GetIMessage(), WM_MOUSEWHEEL, w, l);
        if (bHandled)
            break;

        pObj = pObj->GetParentObject();
    }
    return 0;
}

LRESULT  WindowMouseMgr::OnImeMsg(UINT uMsg, WPARAM w, LPARAM l, BOOL* pbHandled)
{
    if (this->m_pFocusObject)
    {
        return UISendMessage(m_pFocusObject->GetIMessage(), uMsg, w, l, 0, NULL, 0, pbHandled);
    }
    return 0;
}

// ϵͳ��������˫���¼��ǻ��ڴ��ڿͻ�����ġ�
// ����dui�У�ÿ���ؼ������޴��ڵģ��п����ڿؼ�A�ϵ������£�Ȼ������
// �ܵ��ؼ�B��˫���ˡ���˶�˫���¼���һ�����ˡ�
//
// return: true��һ��˫���¼�, false��Ϊ�����¼�����
bool  WindowMouseMgr::AdjustDoubleClickMessage(LPARAM l)
{
    if (m_posPrevClick == l)
        return true;

    return false;
}

}