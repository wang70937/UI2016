#include "stdafx.h"
#include "object.h"
#include "Src\UIObject\Window\windowbase.h"
#include "Inc\Interface\ilayout.h"
#include "Src\Helper\layout\canvaslayout.h"
#include "Src\UIObject\HwndHost\HwndHost.h"
#include "Src\Util\DPI\dpihelper.h"
#include "Src\Layer\layer.h"
#include "object_layer.h"
#include "Inc\Interface\imapattr.h"

namespace UI
{
	

void  Object::ParentClientPoint2ChildPoint(Object* pObjChild, const POINT* pt, POINT* pOut)
{
	pOut->x = pt->x - pObjChild->m_rcParent.left;
	pOut->y = pt->y - pObjChild->m_rcParent.top;
}
void  Object::ObjectPoint2ObjectClientPoint(Object* pObj, const POINT* pt,  POINT* pOut)
{
    REGION4 rcNonClient = {0};
    pObj->GetNonClientRegion(&rcNonClient);

	pOut->x = pt->x - rcNonClient.left;
	pOut->y = pt->y - rcNonClient.top;

	int xOffset = 0, yOffset = 0;
	if (pObj->GetScrollOffset(&xOffset, &yOffset))
	{
		pOut->x += xOffset;
		pOut->y += yOffset;
	}
}
void  Object::ObjectPoint2ObjectNonClientPoint(Object* pObj, const POINT* pt, POINT* pOut)
{
    pOut->x = pt->x - pObj->m_rcBorder.left;
    pOut->y = pt->y - pObj->m_rcBorder.left;
}
void  Object::ObjectRect2ObjectClientRect(Object* pObj, LPCRECT prc, RECT* pOut)
{
	POINT pt = {prc->left, prc->top};
	ObjectPoint2ObjectClientPoint(pObj, &pt, &pt);

	SetRect(pOut, 
		pt.x, 
		pt.y, 
		pt.x+(prc->right-prc->left), 
		pt.y+(prc->bottom-prc->top));
}

void  Object::ObjectClientRect2WindowRect(Object* pObj, LPCRECT prcClient, LPRECT prcWnd)
{
	RECT  rcClientInWnd;
	pObj->GetClientRectInWindow(&rcClientInWnd);

	prcWnd->left = rcClientInWnd.left + prcClient->left;
	prcWnd->top = rcClientInWnd.top + prcClient->top;
	prcWnd->right = prcWnd->left + (prcClient->right-prcClient->left);
	prcWnd->bottom = prcWnd->top + (prcClient->bottom-prcClient->top);
}

void  Object::ObjectRect2WindowRect(Object* pObj, LPCRECT prcObj, LPRECT prcWnd)
{
	RECT rcInWindow;
	pObj->GetWindowRect(&rcInWindow);

	prcWnd->left = rcInWindow.left + prcObj->left;
	prcWnd->top = rcInWindow.top + prcObj->top;
	prcWnd->right = prcWnd->left + (prcObj->right-prcObj->left);
	prcWnd->bottom = prcWnd->top + (prcObj->bottom-prcObj->top);
}

void  Object::ParentClientPoint2ChildClientPoint(Object* pObjChild, const POINT* pt, POINT* pOut)
{
	ParentClientPoint2ChildPoint(pObjChild, pt, pOut);
	if (!pObjChild->IsNcObject())
	{
		ObjectPoint2ObjectClientPoint(pObjChild, pOut, pOut);
	}
}
void  Object::ParentClientRect2ChildClientRect(Object* pObjChild, LPCRECT prc, RECT* pOut)
{
	POINT pt = {prc->left, prc->top};
	ParentClientPoint2ChildClientPoint(pObjChild, &pt, &pt);

	SetRect(pOut, 
		pt.x, 
		pt.y, 
		pt.x+(prc->right-prc->left), 
		pt.y+(prc->bottom-prc->top));
}
void  Object::ParentPoint2ChildPoint(Object* pObjChild, const POINT* pt, POINT* pOut)
{
	POINT ptTemp;
    if (pObjChild->IsNcObject())
    {
        ObjectPoint2ObjectNonClientPoint(pObjChild->m_pParent, pt, &ptTemp);
    }
    else
    {
	    ObjectPoint2ObjectClientPoint(pObjChild->m_pParent, pt, &ptTemp);
    }
	ParentClientPoint2ChildPoint(pObjChild, &ptTemp, pOut);
}

void  Object::ParentRect2ChildRect(Object* pObjChild, LPCRECT prc, RECT* pOut)
{
	POINT pt = {prc->left, prc->top};
	ParentPoint2ChildPoint(pObjChild, &pt, &pt);

	SetRect(pOut, 
		pt.x, 
		pt.y, 
		pt.x+(prc->right-prc->left), 
		pt.y+(prc->bottom-prc->top));
}

void  Object::ChildPoint2ParentClientPoint(Object* pObjChild, const POINT* ptChild, POINT*  ptOut)
{
	ptOut->x = ptChild->x + pObjChild->m_rcParent.left;
	ptOut->y = ptChild->y + pObjChild->m_rcParent.top;
}

void  Object::ChildRect2ParentClientRect(Object* pObjChild, LPCRECT prc, RECT*  pOut)
{
	POINT pt = {prc->left, prc->top};
	ChildPoint2ParentClientPoint(pObjChild, &pt, &pt);

	SetRect(pOut, 
		pt.x, 
		pt.y, 
		pt.x+(prc->right-prc->left), 
		pt.y+(prc->bottom-prc->top));
}

void  Object::ObjectClientPoint2ObjectPoint(Object*  pObj, const POINT* ptChild, POINT*  ptOut)
{
    REGION4 rcNonClient = {0};
    pObj->GetNonClientRegion(&rcNonClient);

	ptOut->x = ptChild->x + rcNonClient.left;
	ptOut->y = ptChild->y + rcNonClient.top;

	int xScroll = 0, yScroll = 0;
	if (pObj->GetScrollOffset(&xScroll, &yScroll))
	{
		ptOut->x -= xScroll;
		ptOut->y -= yScroll;
	}
}

void  Object::ObjectClientRect2ObjectRect(Object*  pObj, LPCRECT prc, RECT*  pOut)
{
	POINT pt = {prc->left, prc->top};
	ObjectClientPoint2ObjectPoint(pObj, &pt, &pt);

	SetRect(pOut, 
		pt.x, 
		pt.y, 
		pt.x+(prc->right-prc->left), 
		pt.y+(prc->bottom-prc->top));
}

void  Object::ChildPoint2ParentPoint(Object* pObjChild, const POINT* ptChild, POINT*  ptOut)
{
	ChildPoint2ParentClientPoint(pObjChild, ptChild, ptOut);
	if (!pObjChild->IsNcObject())
	{
		ObjectClientPoint2ObjectPoint(pObjChild->m_pParent, ptOut, ptOut);
	}
}

void  Object::ChildRect2ParentRect(Object* pObjChild, LPCRECT prc, RECT*  pOut)
{
	POINT pt = {prc->left, prc->top};
	ChildPoint2ParentPoint(pObjChild, &pt, &pt);

	SetRect(pOut, 
		pt.x, 
		pt.y, 
		pt.x+(prc->right-prc->left), 
		pt.y+(prc->bottom-prc->top));
}


// Ҫ���Ƹö���֮ǰ����ȡ�ö����ڴ����е�ʵ��λ�ã���������ƫ�����Ͳü���
POINT Object::GetWindowPoint()
{
	POINT pt = {0, 0};

	Object* pObjParent = NULL;
	Object* pObjChild = this;
	while (pObjParent = this->EnumParentObject(pObjParent))
	{
		ChildPoint2ParentPoint(pObjChild, &pt, &pt);
		pObjChild = pObjParent;
	}

	return pt;
}

void Object::GetWindowRect(RECT* lprc)
{
	UIASSERT(lprc);

	POINT pt = this->GetWindowPoint();
	::SetRect(
		lprc,
		pt.x, pt.y, 
		pt.x+GetWidth(), 
		pt.y+GetHeight());
}


//
// ��ȡ�ö����ƫ����
//
// ReturnQ
//		����false��ʾ�ö����޹�������
//
bool Object::GetScrollOffset(int* pxOffset, int* pyOffset)
{
	if (NULL == pxOffset || NULL == pyOffset)
		return false;

	*pxOffset = 0; *pyOffset = 0;

	if (m_objStyle.hscroll || m_objStyle.vscroll)
	{
		::UISendMessage(this, UI_MSG_GETSCROLLOFFSET, (WPARAM)pxOffset, (LPARAM)pyOffset);
		return true;
	}

	return false;
}

bool Object::GetScrollRange(int* pxRange, int* pyRange)
{
	if (NULL == pxRange || NULL == pyRange)
		return false;

	*pxRange = 0; *pyRange = 0;

	if (m_objStyle.hscroll || m_objStyle.vscroll)
	{
		::UISendMessage(this, UI_MSG_GETSCROLLRANGE, (WPARAM)pxRange, (LPARAM)pyRange);
		return true;
	}

	return false;
}

// 2014.3.28����������ת���ɶ����ڲ����꣬������ת�任������£���Ҫ��������任
void Object::WindowPoint2ObjectPoint(const POINT* ptWindow, POINT* ptObj, bool bCalcTransform)
{
	if (NULL == ptObj || NULL == ptWindow)
		return;

    if (false == bCalcTransform)
    {
	    POINT pt = this->GetWindowPoint();
	    ptObj->x = ptWindow->x - pt.x;
	    ptObj->y = ptWindow->y - pt.y;
    }
    else
    {
        POINT pt = *ptWindow;

        Object* pObjParent = NULL;

        vector<Object*>  vAncestor;
        while (pObjParent = this->REnumParentObject(pObjParent))
        {
            vAncestor.push_back(pObjParent);
        }
        vAncestor.push_back(this);  // ���Ҫ���Լ������귶Χ��ת��һ��

        unsigned int nSize = vAncestor.size();
        for (unsigned int i = 0; i < nSize; i++)
        {
			Object* p = vAncestor[i];
			//assert (0 && "TODO:");
#if 0
            
            if (p->m_pLayer && p->m_pLayer->HasTransform())
            {
                p->m_pRenderLayer->GetTransformPtr()->mappoint_view_2_layer(&pt);
            }
#endif

            if (p != this)
            {
                ParentPoint2ChildPoint(vAncestor[i+1], &pt, &pt);
            }
        }

        *ptObj = pt;
    }
}

void Object::WindowPoint2ObjectClientPoint(const POINT* ptWindow, POINT* ptClient, bool bCalcTransform)
{
	if (NULL == ptClient || NULL == ptWindow)
		return;

	POINT pt = {0};
	WindowPoint2ObjectPoint(ptWindow, &pt, bCalcTransform);
	ObjectPoint2ObjectClientPoint(this, &pt, ptClient);
}


void  Object::WindowRect2ObjectClientRect(LPCRECT rcWindow, RECT* rcObj)
{
	POINT ptWindow = {rcWindow->left, rcWindow->top};
	POINT ptClient = {0};

	this->WindowPoint2ObjectClientPoint(&ptWindow, &ptClient, false);
	rcObj->left = ptClient.x;
	rcObj->top = ptClient.y;
	rcObj->right = rcObj->left + (rcWindow->right-rcWindow->left);
	rcObj->bottom = rcObj->top + (rcWindow->bottom-rcWindow->top);
}
void  Object::WindowRect2ObjectRect(LPCRECT rcWindow, RECT* rcObj)
{
	POINT ptWindow = {rcWindow->left, rcWindow->top};
	POINT ptObj = {0};

	this->WindowPoint2ObjectPoint(&ptWindow, &ptObj, false);
	rcObj->left = ptObj.x;
	rcObj->top = ptObj.y;
	rcObj->right = rcObj->left + (rcWindow->right-rcWindow->left);
	rcObj->bottom = rcObj->top + (rcWindow->bottom-rcWindow->top);
}

//////////////////////////////////////////////////////////////////////////


// ��ȡһ�������ڴ����ϵĿ��������������ڻ��Ƹö���ʱ�Ĳü�
bool  Object::GetRectInWindow(RECT* prc, bool bOnlyVisiblePart)
{
	return CalcRectInAncestor(GetWindowObject(), NULL, bOnlyVisiblePart, prc);
}

// ��������ڲ��е�λ�ã����ǲ㻺���е�λ�ã�����Ҳ������ƫ�ƣ�
/*
bool  Object::GetRectInLayer(RECT* prc, bool bOnlyVisiblePart)
{
	assert(0 && "TODO");
#if 0
	RenderLayer*  pRenderLayer = GetRenderLayer2();
	if (!pRenderLayer)
		return GetRectInWindow(prc, bOnlyVisiblePart);

	if (!CalcRectInAncestor(pRenderLayer->GetCreateObject(),
			NULL, bOnlyVisiblePart, prc))
	{
		return false;
	}
#endif
	return true;
}


bool  Object::GetVisibleClientRectInLayer(RECT* prc)
{
	assert (0 && TEXT("todo"));
#if 0
    RenderLayer*  pRenderLayer = GetRenderLayer2();
    if (!pRenderLayer)
        return GetRectInWindow(prc, true);

    CRect rcClient;
    this->GetClientRectInObject(&rcClient);
    if (!CalcRectInAncestor(
			pRenderLayer->GetCreateObject(), 
			&rcClient, true, prc))
	{
        return false;
	}
#endif

    return true;
}
*/
//
// prcObjPart
//   [in] Ĭ����CRect  rcClip(0, 0, GetWidth(), GetHeight());
//        ��Ϊ��֧��ֻ��Object�ϵ�ĳһ���������������еĿɼ��������Ӹò���
//     
bool  Object::CalcRectInAncestor(
				__in Object*  pObjAncestor,
				__in LPCRECT prcObjPart,
				bool bCalcVisible, 
				__out RECT* prcOut)
{
	// �������ϱ���
	if (!pObjAncestor)
		return false;

    // ��ʼ��rcObjPart���п��ܸ��Ĳ�����Χ�ͳ������Լ�������
    if (prcOut)
        SetRectEmpty(prcOut);

    RECT  rcObj = {0, 0, GetWidth(), GetHeight()};
    RECT  rcObjPart = {0};   // �淶���ĺ�������

    if (NULL == prcObjPart)
    {
        CopyRect(&rcObjPart, &rcObj);
    }
    else
    {
        if (!::IntersectRect(&rcObjPart, prcObjPart, &rcObj))
            return false;
    }
    
    // �Լ�
	if (pObjAncestor == this)
	{
		if (prcOut)
		    CopyRect(prcOut, &rcObjPart);
		return true;
	}

    //  ���ϱ���
	CRect    rcClip(&rcObjPart);
	Object*  pParent = NULL;
	Object*  pChild = this;
	while (pParent = this->EnumParentObject(pParent))
	{
        // ת�����ڸ��е�����
		Object::ChildRect2ParentRect(pChild, &rcClip, &rcClip);

		if (bCalcVisible)
		{
			// �븸���������
			CRect  rcParent(0, 0, pParent->GetWidth(), pParent->GetHeight());
			if (!::IntersectRect(rcClip, &rcClip, &rcParent))
				return false;
		}

		if (pParent == pObjAncestor)
			break;

		pChild = pParent;
	}
	
	::CopyRect(prcOut, &rcClip);
	return true;
}

// ���Դ����ϵ��������Լ��Ľ���
bool  Object::IntersectWindowRect(LPCRECT prcWindow, RECT* prcIntersectWnd, RECT* prcIntersectObj)
{
	UIASSERT(prcWindow);

	// wnd
	RECT  rcWndMe, rcIntersectWnd;
	GetRectInWindow(&rcWndMe, true);

	if (!::IntersectRect(&rcIntersectWnd, &rcWndMe, prcWindow))
		return false;

	if (prcIntersectWnd)
	{
		CopyRect(prcIntersectWnd, &rcIntersectWnd);
	}

	// obj
	if (prcIntersectObj)
	{
		//         RECT  rcObj = {0};
		//         WindowRect2ObjectRect(prcWindow, &rcObj);
		// 
		//         RECT  rcMy = {0, 0, GetWidth(), GetHeight()};
		//         RECT  rcIntersect = {0};
		//         IntersectRect(&rcIntersect, &rcMy, &rcObj);
		//         ::CopyRect(prcIntersectObj, &rcIntersect);

		RECT rcMe = {0, 0, GetWidth(), GetHeight()};
		rcMe.left += rcIntersectWnd.left - rcWndMe.left;
		rcMe.right -= rcWndMe.right - rcIntersectWnd.right;
		rcMe.top += rcIntersectWnd.top - rcWndMe.top;
		rcMe.bottom += rcWndMe.bottom - rcIntersectWnd.bottom;

		CopyRect(prcIntersectObj, &rcMe);

	}
	return true;
}

// �����clientrect ������0,0���ģ����������Object���ϽǶ���
void Object::GetClientRectInObject(RECT* prc)
{
	if (NULL == prc)
		return;

    RECT rcNonClient = {0};
    GetNonClientRegion(&rcNonClient);

	::SetRect(prc,
		rcNonClient.left, 
		rcNonClient.top, 
		this->GetWidth() - rcNonClient.right, 
		this->GetHeight()- rcNonClient.bottom); 
}

// �����clientrect ��0,0����
void Object::GetObjectClientRect(RECT* prc)
{
	if (NULL == prc)
		return;

    RECT rcNonClient = {0};
    GetNonClientRegion(&rcNonClient);
    
	::SetRect(prc,
		0, 
		0, 
		this->GetWidth() - rcNonClient.left - rcNonClient.right, 
		this->GetHeight()- rcNonClient.top - rcNonClient.bottom); 
}

void  Object::GetNonClientRegion(REGION4* prc) 
{
    SetRect(prc,
        m_rcExtNonClient.left   + m_rcPadding.left   + m_rcBorder.left,
        m_rcExtNonClient.top    + m_rcPadding.top    + m_rcBorder.top,
        m_rcExtNonClient.right  + m_rcPadding.right  + m_rcBorder.right,
        m_rcExtNonClient.bottom + m_rcPadding.bottom + m_rcBorder.bottom);
}

void  Object::GetClientRectInParent(RECT* prc)
{
	GetClientRectInObject(prc);
	OffsetRect(prc, m_rcParent.left, m_rcParent.top);
}

// clientrect�ڴ����е�����
void Object::GetClientRectInWindow( RECT* prc )
{
	if (NULL == prc)
		return;

	this->GetWindowRect(prc);

    REGION4 rcNonClient = {0};
    GetNonClientRegion(&rcNonClient);

	prc->left += rcNonClient.left;
	prc->top  += rcNonClient.top;
	prc->right  -= rcNonClient.right;
	prc->bottom -= rcNonClient.bottom;
}



// Panel/Object��Ҫ��ӦWM_SIZE�����п��ܿؼ�������WM_SIZE�����ڲ��޷�����
// ���������һ���麯����ר�����ڲ�����size�ı��¼�
void  Object::notify_WM_SIZE(UINT nType, UINT nWidth, UINT nHeight)
{
    this->virtualOnSize(nType, nWidth, nHeight);

    UISendMessage(m_pIObject, WM_SIZE, 0, MAKELPARAM(nWidth, nHeight));
}

void  Object::virtualOnSize(UINT nType, UINT nWidth, UINT nHeight)
{
    if (m_pLayer)
    {
        m_pLayer->OnSize(nWidth, nHeight);
    }
}
void  Object::notify_WM_MOVE(int x, int y)
{
    this->virtualOnMove(x, y);

    UISendMessage(this, WM_MOVE, 0, MAKELPARAM(m_rcParent.left,m_rcParent.top));
}
void  Object::virtualOnMove(int x, int y)
{
}

//
//	��������ʾ/���أ����ߴ�С�����ı�ʱ������ˢ���Լ�����layout�Ĳ���
//
//	1. ��ȡ�Լ�������Layout
//	2. ����Layout�Ĵ�С�Ƿ����ı䣬������Layout��С�ı䣬�ٻ�ȡ��һ��Layout
//
void Object::UpdateLayout()
{
	if (m_pDescription && m_pDescription->GetMajorType() == OBJ_WINDOW)
	{
		ILayout* pLayout = static_cast<WindowBase*>(this)->GetLayout();
		if (pLayout)
		{
			pLayout->Arrange(NULL);
            this->Invalidate();
		}
		return;
	}

    Object* pParent = this->GetParentObject();
    Object* pObjectToUpdate = this;

    while (pParent)
    {
		OBJ_TYPE nObjType = OBJ_UNKNOWN;
		if (pParent->GetDescription())
			nObjType = pParent->GetDescription()->GetMajorType();

        if (OBJ_PANEL != nObjType && OBJ_WINDOW != nObjType)  // ����listview�е�headerctrl�����ĸ�������panel
        {
            pParent->notify_WM_SIZE(0, pParent->GetWidth(), pParent->GetHeight());
            return;
        }

        ILayout* pLayout = ((Panel*)pParent)->GetLayout();
        if (NULL == pLayout)
        {
            UI_LOG_WARN(_T("GetLayout failed. id=%s"), pParent->m_strId.c_str());
            break;
        }

        ILayoutParam*  pLayoutParam = pParent->GetLayoutParam();
        if (pLayoutParam && !pLayoutParam->IsSizedByContent())
        {
            pLayout->Arrange(pObjectToUpdate->GetIObject());
            pParent->Invalidate();
            break;
        }

        SIZE sizeOld = {pParent->GetWidth(), pParent->GetHeight()};
        SIZE size = pParent->GetDesiredSize();

        if (sizeOld.cx == size.cx || sizeOld.cy == size.cy)
        {
            pLayout->Arrange(pObjectToUpdate->GetIObject());
            pParent->Invalidate();
            break;
        }
        // pParent�Ĵ�С�����˱仯����������
        if (pParent->GetParentObject())
        {
            pObjectToUpdate = pParent;
            pParent = pParent->GetParentObject();  // TODO: �е���... ���Ǵ��ڵ�ʱ��sizeΪwindow rect��sizeOldΪclient rect����˿϶���һ��
            continue;                              // �������������������window( parent==null )��������´���
        }
        else if (
			pParent->GetDescription() && 
			pParent->GetDescription()->GetMajorType() == OBJ_WINDOW)
        {
            HWND hWnd = GetHWND();

            CRect rcWndNow;
            ::GetWindowRect(hWnd, rcWndNow);
            if (rcWndNow.Width() == size.cx && rcWndNow.Height() == size.cy)  // ����Сû�иı�ʱ�����ᴥ��WM_SIZE��Ҳ�Ͳ��������
            {
                pLayout->Arrange(pObjectToUpdate->GetIObject());
                pParent->Invalidate();
            }
            else
            {
                
                UINT nFlag = SWP_NOZORDER|SWP_NOMOVE|SWP_NOACTIVATE;
//                 if (!bUpdate) 
//                 {
//                     WindowRender*  pRender = static_cast<WindowBase*>(pParent)->GetWindowRender();
//                     pRender->SetCanCommit(false);
//                     SetWindowPos(GetHWND(), 0, 0,0, size.cx, size.cy, nFlag);
//                     pRender->SetCanCommit(true);
//                 }
//                 else
                {
                    SetWindowPos(GetHWND(), 0, 0,0, size.cx, size.cy, nFlag);
                }
            }
            break;
        }
    }
}

// UpdateLayout�����Լ��ڴ����ϵĲ��֣�һ�㲻�޸����Լ����ڲ�����
// UpdateMyLayoutֻ�޸��Լ����ڲ�����
// void  Object::UpdateMyLayout()
// {
//     IPanel*  pPanel = (IPanel*)m_pIObject->QueryInterface(__uuidof(IPanel));
//     if (pPanel)
//     {
//         ILayout*  pLayout = pPanel->GetLayout();
//         if (pLayout)
//         {
//             pLayout->Arrange();
//             pPanel->Invalidate();
//         }
//     }
// }

//
//	��ȡ�����Լ������Ĵ�С
//
SIZE Object::GetDesiredSize()
{
    if (!m_pLayoutParam)
    {
        CreateLayoutParam();

        if (!m_pLayoutParam) // ���Ǵ������ɹ�������Combobox�е�Button����û�и�Panel-Layout
        {
            m_pLayoutParam = CanvasLayout::s_CreateLayoutParam(m_pIObject);
        }
    }

    return m_pLayoutParam->CalcDesiredSize();
}


void Object::SetObjectPos(int x, int y, int cx, int cy, int nFlag)
{
	if (cx < 0)
		cx = 0;
	if (cy < 0)
		cy = 0;

    bool bMove = (nFlag&SWP_NOMOVE)?false:true;
    bool bSize = (nFlag&SWP_NOSIZE)?false:true;

    //  TODO: ������һ�δ���ᵼ��SetVisible�еĲ��ֳ������⣬���pp -> p -> o��o�����仯
    //  ��pȥ���²��֣�Ȼ��p�ټ���Լ���desiredsize�仯�ˣ�����pp�������²��֣����pp�Ĵ�
    //  Сû�з����仯������p����ȥ���²��֣�oˢ��ʧ�ܡ�
    //     if (bMove && 
    //         x == m_rcParent.left &&
    //         y == m_rcParent.top && 
    //         bSize && 
    //         cx == m_rcParent.Width() && 
    //         cy == m_rcParent.Height())
    //         return;

    nFlag |= SWP_NOZORDER;   // �ú������ṩ�޸�ZORDER�Ĺ���
    WINDOWPOS wndpos =  {NULL, NULL, x, y, cx, cy, nFlag};
    UISendMessage(this, WM_WINDOWPOSCHANGING, 0, (LPARAM)&wndpos);
    x = wndpos.x;
    y = wndpos.y;
    cx = wndpos.cx;
    cy = wndpos.cy;
    nFlag = wndpos.flags;

    if (x == m_rcParent.left && y == m_rcParent.top)
        bMove = false;
    if (cx == m_rcParent.Width() && cy == m_rcParent.Height())
        bSize = false;

    if (bMove&&bSize)
    {

    }
    else if (bMove)
    {
        cx = this->GetWidth();
        cy = this->GetHeight();
    }
    else if (bSize)
    {
        x = m_rcParent.left;
        y = m_rcParent.top;
    }
    else
    {
        if (nFlag & SWP_FORCESENDSIZEMSG)
        {
            notify_WM_SIZE(0, m_rcParent.Width(),m_rcParent.Height());
        }
        if (nFlag & SWP_FORCEUPDATEOBJECT)
        {
            this->Invalidate();
        }
        return;  // DONOTHING
    }

	WindowBase* pWindow = this->GetWindowObject();
	bool bHardComposite = false;
	if (pWindow)
		bHardComposite = pWindow->IsGpuComposite();

    //RECT rcOldVisibleRect = {0};
    if (bMove || bSize) 
    {
        // ˢ���ƶ�ǰ������λ��
        if (!(nFlag & SWP_NOREDRAW))
        {
            //this->GetRectInWindow(&rcOldVisibleRect, true);  // ��ȡ�µ�ǰ��ˢ�µ�����Χ�����ں�������ύ
            this->Invalidate();
        }
    }

	::SetRect(&m_rcParent, x,y,x+cx,y+cy);
    if (!(nFlag&SWP_NOUPDATELAYOUTPOS))
    {
        UpdateLayoutPos();
    }

    // MSDN: MoveWindow sends the WM_WINDOWPOSCHANGING, WM_WINDOWPOSCHANGED, WM_MOVE, WM_SIZE, and WM_NCCALCSIZE messages to the window. 
    // ������������ʱֻ�ȷ���WM_MOVE/WM_SIZE��Ϣ
    if (!(nFlag & SWP_NOSENDCHANGING))
    {
        if (bMove)
        {
            notify_WM_MOVE(m_rcParent.left,m_rcParent.top);
        }
        if (bSize)
        {
            notify_WM_SIZE(0, m_rcParent.Width(),m_rcParent.Height());
        }

        WINDOWPOS wndpos2 =  {NULL, NULL, x, y, cx, cy, nFlag};
        UISendMessage(this, WM_WINDOWPOSCHANGED, NULL, (LPARAM)&wndpos2);
    }

    if (bMove || bSize)
    {
        // ˢ���ƶ��������λ��
        if (!(nFlag & SWP_NOREDRAW))
        {
			// 2015.9.22
			// �����Ӳ���ϳ�ģʽ�������մ����ϵĻ��沢����������
			// ������ݣ�ֱ��CommitDoubleBuffer�ᵼ�����ݶ�ʧ��

            this->Invalidate();
			/*if (bHardComposite)
			{
				pWindow->DirectComposite();
			}
			else
			{
                RECT rcArray[2];
                CopyRect(rcArray, &rcOldVisibleRect);
                GetRectInWindow(&rcArray[1], true);
                
				// ���ƶ�֮ǰ�����������ύ�������ϣ�������˸
				pWindow->CommitDoubleBuffet2Window(NULL, rcArray, 2);
			}
            */
        }
    }
}


void Object::SetObjectPos(RECT* prc, int nFlag)
{
    if (NULL == prc)
        return;

    this->SetObjectPos(prc->left, prc->top, RECTW(prc), RECTH(prc), nFlag);
}

// ����m_rcParent���� m_nConfigLeft/m_nConfigRight/m_nConfigTop/m_nConfigBottom/m_nConfigLayoutFlags
void Object::UpdateLayoutPos()
{
    if (m_pLayoutParam)
    {
        m_pLayoutParam->UpdateByRect();
    }
}

int Object::GetWidth()
{
	return m_rcParent.Width();
}


int Object::GetHeight()
{
	return m_rcParent.Height();
}


int Object::GetWidthWithMargins()
{
	return m_rcParent.Width() + m_rcMargin.left + m_rcMargin.right;
}


int Object::GetHeightWithMargins()
{
	return m_rcParent.Height() + m_rcMargin.top + m_rcMargin.bottom;
}


//
// �����Լ���nc object�������Լ���non client region
//
void Object::UpdateObjectNonClientRegion()
{
	CRegion4  rcNonClient(0,0,0,0);

    UIMSG msg;
	msg.message = UI_MSG_CALC_PARENT_NONCLIENTRECT;
	msg.wParam = (WPARAM)&rcNonClient;
	msg.pMsgFrom = this->GetIMessage();

	Object* pNcChild = NULL;
	while (pNcChild = this->EnumNcChildObject(pNcChild))
	{
		msg.pMsgTo = pNcChild->GetIMessage();
		UISendMessage(&msg);
	}

	this->SetExtNonClientRegion(&rcNonClient);
}

void Object::GetParentRect( RECT* prc )
{
	if (NULL == prc)
		return;

	::CopyRect(prc, &m_rcParent);
}

ILayoutParam*  Object::GetLayoutParam()
{
    return m_pLayoutParam;
}

// ���û�У��򴴽�һ��canvas����
ILayoutParam*  Object::GetSafeLayoutParam()
{
    if (m_pLayoutParam)
        return m_pLayoutParam;

    CreateLayoutParam();
    if (m_pLayoutParam)
        return m_pLayoutParam;

    CanvasLayout::s_GetObjectLayoutParam(this);

    SERIALIZEDATA data = { 0 };
    data.pMapAttrib = m_pIMapAttributeRemain;
    data.pSkinRes = GetISkinRes();
    data.pUIApplication = GetIUIApplication();
    data.nFlags = SERIALIZEFLAG_LOAD;
    m_pLayoutParam->Serialize(&data);

    return m_pLayoutParam;
}

void  Object::CreateLayoutParam()
{
    if (m_pLayoutParam)
        return;

    if (!m_pParent)
        return;

    ILayout* pLayout = (ILayout*)UISendMessage(m_pParent, UI_MSG_GETLAYOUT);
    if (!pLayout)
        return;

    m_pLayoutParam = pLayout->CreateLayoutParam(m_pIObject);
}

void  Object::DestroyLayoutParam()
{
    SAFE_RELEASE(m_pLayoutParam);
}

void  Object::SetLayoutParam(ILayoutParam* p)
{
    SAFE_RELEASE(m_pLayoutParam);
    m_pLayoutParam = p;
}

int   Object::GetConfigWidth()
{
    if (m_pLayoutParam)
    {
        if (m_pLayoutParam->GetLayoutType() != LAYOUT_TYPE_CANVAS)
            return NDEF;
        else
            return static_cast<CanvasLayoutParam*>(m_pLayoutParam)->GetConfigWidth();
    }
    else if (m_pIMapAttributeRemain)
    {
        int nRet = NDEF;
        m_pIMapAttributeRemain->GetAttr_int(XML_WIDTH, false, &nRet);
        return nRet;
    }

	return NDEF;
}

int   Object::GetConfigHeight()
{
    if (m_pLayoutParam)
    {
        if (m_pLayoutParam->GetLayoutType() != LAYOUT_TYPE_CANVAS)
            return NDEF;
        else
            return static_cast<CanvasLayoutParam*>(m_pLayoutParam)->GetConfigHeight();
    }
    else if (m_pIMapAttributeRemain)
    {
        int nRet = NDEF;
        m_pIMapAttributeRemain->GetAttr_int(XML_HEIGHT, false, &nRet);
        return nRet;
    }
	return NDEF;
}

int   Object::GetConfigLayoutFlags()
{
    if (m_pLayoutParam)
    {
        if (m_pLayoutParam->GetLayoutType() != LAYOUT_TYPE_CANVAS)
            return 0;
        else
            return static_cast<CanvasLayoutParam*>(m_pLayoutParam)->GetConfigLayoutFlags();
    }
    else
    {
        const TCHAR* szText = m_pIMapAttributeRemain->GetAttr(XML_LAYOUT_ITEM_ALIGN, false);
        if (!szText)
            return 0;

        return CanvasLayoutParam::ParseAlignAttr(szText);
    }
}
int   Object::GetConfigLeft()
{
    if (m_pLayoutParam)
    {
        if (m_pLayoutParam->GetLayoutType() != LAYOUT_TYPE_CANVAS)
            return NDEF;
        else
            return static_cast<CanvasLayoutParam*>(m_pLayoutParam)->GetConfigLeft();
    }
    else
    {
        int nRet = NDEF;
        m_pIMapAttributeRemain->GetAttr_int(XML_LAYOUT_ITEM_LEFT, false, &nRet);
        return nRet;
    }
}
int   Object::GetConfigRight()
{
    if (m_pLayoutParam)
    {
        if (m_pLayoutParam->GetLayoutType() != LAYOUT_TYPE_CANVAS)
            return NDEF;
        else
            return static_cast<CanvasLayoutParam*>(m_pLayoutParam)->GetConfigRight();
    }
    else
    {
        int nRet = NDEF;
        m_pIMapAttributeRemain->GetAttr_int(XML_LAYOUT_ITEM_RIGHT, false, &nRet);
        return nRet;
    }
}
int   Object::GetConfigTop()
{
    if (m_pLayoutParam)
    {
        if (m_pLayoutParam->GetLayoutType() != LAYOUT_TYPE_CANVAS)
            return NDEF;
        else
            return static_cast<CanvasLayoutParam*>(m_pLayoutParam)->GetConfigTop();
    }
    else
    {
        int nRet = NDEF;
        m_pIMapAttributeRemain->GetAttr_int(XML_LAYOUT_ITEM_TOP, false, &nRet);
        return nRet;
    }
}
int   Object::GetConfigBottom()
{
    if (m_pLayoutParam)
    {
        if (m_pLayoutParam->GetLayoutType() != LAYOUT_TYPE_CANVAS)
            return NDEF;
        else
            return static_cast<CanvasLayoutParam*>(m_pLayoutParam)->GetConfigBottom();
    }
    else
    {
        int nRet = NDEF;
        m_pIMapAttributeRemain->GetAttr_int(XML_LAYOUT_ITEM_BOTTOM, false, &nRet);
        return nRet;
    }
}

void  Object::SetConfigWidth(int n)
{
    if (m_pLayoutParam)
    {
        if (m_pLayoutParam->GetLayoutType() == LAYOUT_TYPE_CANVAS)
            static_cast<CanvasLayoutParam*>(m_pLayoutParam)->SetConfigWidth(n);
    }
    else
    {
        CanvasLayout::s_GetObjectLayoutParam(this)->SetConfigWidth(n);
    }
}
void  Object::SetConfigHeight(int n)
{
    if (m_pLayoutParam)
    {
        if (m_pLayoutParam->GetLayoutType() == LAYOUT_TYPE_CANVAS)
            static_cast<CanvasLayoutParam*>(m_pLayoutParam)->SetConfigHeight(n);
    }
    else
    {
        CanvasLayout::s_GetObjectLayoutParam(this)->SetConfigHeight(n);
    }
}
void  Object::SetConfigLayoutFlags(int n)
{
    if (m_pLayoutParam)
    {
        if (m_pLayoutParam->GetLayoutType() == LAYOUT_TYPE_CANVAS)
            static_cast<CanvasLayoutParam*>(m_pLayoutParam)->SetConfigLayoutFlags(n);
    }
    else
    {
        CanvasLayout::s_GetObjectLayoutParam(this)->SetConfigLayoutFlags(n);
    }
}
void  Object::SetConfigLeft(int n)
{
    if (m_pLayoutParam)
    {
        if (m_pLayoutParam->GetLayoutType() == LAYOUT_TYPE_CANVAS)
            static_cast<CanvasLayoutParam*>(m_pLayoutParam)->SetConfigLeft(n);
    }
    else
    {
        CanvasLayout::s_GetObjectLayoutParam(this)->SetConfigLeft(n);
    }
}
void  Object::SetConfigRight(int n)
{
    if (m_pLayoutParam)
    {
        if (m_pLayoutParam->GetLayoutType() == LAYOUT_TYPE_CANVAS)
            static_cast<CanvasLayoutParam*>(m_pLayoutParam)->SetConfigRight(n);
    }
    else
    {
        CanvasLayout::s_GetObjectLayoutParam(this)->SetConfigRight(n);
    }
}
void  Object::SetConfigTop(int n)
{
    if (m_pLayoutParam)
    {
        if (m_pLayoutParam->GetLayoutType() == LAYOUT_TYPE_CANVAS)
            static_cast<CanvasLayoutParam*>(m_pLayoutParam)->SetConfigTop(n);
    }
    else
    {
        CanvasLayout::s_GetObjectLayoutParam(this)->SetConfigTop(n);
    }
}
void  Object::SetConfigBottom(int n)
{
    if (m_pLayoutParam)
    {
        if (m_pLayoutParam->GetLayoutType() == LAYOUT_TYPE_CANVAS)
            static_cast<CanvasLayoutParam*>(m_pLayoutParam)->SetConfigBottom(n);
    }
    else
    {
        CanvasLayout::s_GetObjectLayoutParam(this)->SetConfigBottom(n);
    }
}

void  Object::LoadBorder(REGION4* prc)
{
    m_rcBorder.left = ScaleByDpi(prc->left);
    m_rcBorder.top = ScaleByDpi(prc->top);
    m_rcBorder.right = ScaleByDpi(prc->right);
    m_rcBorder.bottom = ScaleByDpi(prc->bottom);
}
void  Object::SaveBorder(REGION4* prc)
{
    prc->left = ScaleByDpi(m_rcBorder.left);
    prc->top = ScaleByDpi(m_rcBorder.top);
    prc->right = ScaleByDpi(m_rcBorder.right);
    prc->bottom = ScaleByDpi(m_rcBorder.bottom);
}

void  Object::SavePadding(REGION4* prc)
{
    prc->left = ScaleByDpi(m_rcPadding.left);
    prc->top = ScaleByDpi(m_rcPadding.top);
    prc->right = ScaleByDpi(m_rcPadding.right);
    prc->bottom = ScaleByDpi(m_rcPadding.bottom);
}

void  Object::LoadPadding(REGION4* prc)
{
    m_rcPadding.left = ScaleByDpi(prc->left);
    m_rcPadding.top = ScaleByDpi(prc->top);
    m_rcPadding.right = ScaleByDpi(prc->right);
    m_rcPadding.bottom = ScaleByDpi(prc->bottom);
}

void  Object::LoadMargin(REGION4* prc)
{
    m_rcMargin.left = ScaleByDpi(prc->left);
    m_rcMargin.top = ScaleByDpi(prc->top);
    m_rcMargin.right = ScaleByDpi(prc->right);
    m_rcMargin.bottom = ScaleByDpi(prc->bottom);
}

void  Object::SaveMargin(REGION4* prc)
{
    prc->left = ScaleByDpi(m_rcMargin.left);
    prc->top = ScaleByDpi(m_rcMargin.top);
    prc->right = ScaleByDpi(m_rcMargin.right);
    prc->bottom = ScaleByDpi(m_rcMargin.bottom);
}

}
