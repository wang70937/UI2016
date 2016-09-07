#include "stdafx.h"
#include "canvaslayout.h"
#include "Src\Base\Object\object.h"
#include "Src\Base\Attribute\attribute.h"
#include "Src\Base\Attribute\long_attribute.h"
#include "Src\Base\Attribute\flags_attribute.h"
#include "Src\Base\Application\uiapplication.h"
#include "Src\Util\DPI\dpihelper.h"

namespace UI
{

SIZE  CanvasLayout::Measure()
{
    SIZE size = {0,0};

    // ͨ���ӿؼ�����ȡ�Լ�����Ҫ�Ĵ�С
    Object*  pChild = NULL;
    while (pChild = this->m_pPanel->EnumChildObject(pChild))
    {
        CanvasLayoutParam* pParam = s_GetObjectLayoutParam(pChild);
        if (!pParam)
        {
            continue;
        }
        SIZE  s = pChild->GetDesiredSize();
        

        if (NDEF != pParam->GetConfigLeft())
        {
            s.cx += pParam->GetConfigLeft();
        }
        else if (NDEF != pParam->GetConfigRight())
        {
            s.cx += pParam->GetConfigRight();
        }
        if (NDEF != pParam->GetConfigTop())
        {
            s.cy += pParam->GetConfigTop();
        }
        else if (NDEF != pParam->GetConfigBottom())
        {
            s.cy += pParam->GetConfigBottom();
        }

        if (size.cx < s.cx)
        {
            size.cx = s.cx;
        }
        if (size.cy < s.cy)
        {
            size.cy = s.cy;
        }
    }
    return size;
}
void  CanvasLayout::DoArrage(IObject* pIObjToArrage)
{
    Object* pObjToArrage = NULL;
    if (pIObjToArrage)
        pObjToArrage = pIObjToArrage->GetImpl();

    // ���øú���ʱ���Լ��Ĵ�С�Ѿ����������
    CRect rcClient;
    m_pPanel->GetClientRectInObject(&rcClient);
    int  nWidth  = rcClient.Width();  //m_pPanel->GetWidth();
    int  nHeight = rcClient.Height(); //m_pPanel->GetHeight();

    Object* pChild = NULL;
    while (pChild = m_pPanel->EnumChildObject(pChild))
    {
        if (pObjToArrage && pObjToArrage != pChild)
            continue;

        if (pChild->IsSelfCollapsed())
		{
			// ���صĿؼ����ڱ༭����Ҳ��Ҫ���ز������ԡ�
			if (m_pPanel->GetUIApplication()->IsDesignMode())
				s_GetObjectLayoutParam(pChild);

			continue;
		}

        this->ArrangeObject(pChild, nWidth, nHeight);

        if (pObjToArrage && pObjToArrage == pChild)
            break;
    }
}

void  CanvasLayout::ArrangeObject(Object*  pChild, const int& nWidth, const int& nHeight)
{
    int  x = 0, y = 0;                                  // pChild������parent�е�����

    CanvasLayoutParam* pParam = s_GetObjectLayoutParam(pChild);
    if (!pParam)
    {
        return;
    }

    int left = pParam->GetConfigLeft();
    int top  = pParam->GetConfigTop();
    int right  = pParam->GetConfigRight();
    int bottom = pParam->GetConfigBottom();
    // int nConfigWidth = pParam->GetConfigWidth();
    // int nConfigHeight = pParam->GetConfigHeight();
    int nConfigFlag = pParam->GetConfigLayoutFlags();

    //////////////////////////////////////////////////////////////////////////
    // ����� pChild �� rectP�Ŀ�͸�
    SIZE s = pParam->CalcDesiredSize();

    // ���ͬʱָ����left/right,�����width����
    if (left != NDEF && right != NDEF)
    {
        s.cx = nWidth - left - right;
    }
    // ���ͬʱָ����top/bottom�������height����
    if (top != NDEF && bottom != NDEF)
    {
        s.cy = nHeight - top - bottom;
    }
	if (s.cx < 0)
		s.cx = 0;
	if (s.cy < 0)
		s.cy = 0;

    CRect rcChildObj ;
    int nChildW = s.cx - pChild->GetMarginW();
    int nChildH = s.cy - pChild->GetMarginH();
    rcChildObj.SetRect(0, 0, nChildW, nChildH );

    // ��������꣬��left/right,top/bottom����һ��δָ���ģ���ôȡ0������DesktopLayout����ȡ���У�
    if (left != NDEF)
    {
        x = left;
        x += pChild->GetMarginL();
    }
    else
    {
        if (right != NDEF)
        {
            x = nWidth - right - nChildW;
            x -= pChild->GetMarginR();
        }
        else
        {
            if (nConfigFlag & LAYOUT_ITEM_ALIGN_CENTER)
            {
                x = ( nWidth - s.cx ) / 2;  // ����
            }
            else
            {
                x = 0;
                x += pChild->GetMarginL();
            }
        }
    }
    if (top != NDEF)
    {
        y = top;
        y += pChild->GetMarginT();
    }
    else
    {
        if (bottom != NDEF)
        {
            y = nHeight - bottom - nChildH;
            y -= pChild->GetMarginB();
        }
        else
        {
            if (nConfigFlag & LAYOUT_ITEM_ALIGN_CENTER)
            {
                y = ( nHeight - s.cy ) / 2;  // ����
            }
            else
            {
                y = 0;
                y += pChild->GetMarginT();
            }
        }
    }

    rcChildObj.OffsetRect(x, y);
    pChild->SetObjectPos(&rcChildObj, SWP_NOREDRAW|SWP_NOUPDATELAYOUTPOS|SWP_FORCESENDSIZEMSG);
}

void  CanvasLayout::OnChildObjectVisibleChanged(IObject* pObj)
{
    UIASSERT (pObj);
	UIASSERT(pObj->GetParentObject());
	UIASSERT(pObj->GetParentObject()->GetImpl() == m_pPanel);

    // TODO: �Ż�Ϊֻ���ָ�pObj�����ҽ���collapsed���������Ҫ����
    //SetDirty(true);<-- ���ܽ����ú��������ܺ���û�д���invalidate
    DoArrage(pObj);

    pObj->Invalidate();
}

//////////////////////////////////////////////////////////////////////////

CanvasLayoutParam::CanvasLayoutParam()
{
    m_nConfigWidth = m_nConfigHeight = AUTO;
    m_nConfigLeft = m_nConfigRight = m_nConfigTop = m_nConfigBottom = NDEF;
    m_nConfigLayoutFlags = 0;

}
CanvasLayoutParam::~CanvasLayoutParam()
{

}

SIZE  CanvasLayoutParam::CalcDesiredSize()
{
    SIZE size = {0,0};

	if (IsSizedByContent())
	{
        // ��ȡ�Ӷ�������Ҫ�Ŀռ�
        UISendMessage(m_pObj, UI_MSG_GETDESIREDSIZE, (WPARAM)&size);

        // �����ָ��width��height������һ������ô��������һ���еõ���ֵ
        if (this->m_nConfigWidth != AUTO)
            size.cx = this->m_nConfigWidth;
        if (this->m_nConfigHeight!= AUTO)
            size.cy = this->m_nConfigHeight;
    }
	else
	{
		size.cx = this->m_nConfigWidth;
		size.cy = this->m_nConfigHeight;
	}

    // ���� margin �Ĵ�С
    size.cx += m_pObj->GetMarginW();
    size.cy += m_pObj->GetMarginH();

    return size;
}

void  CanvasLayoutParam::UpdateByRect()
{
    Object* pParent = m_pObj->GetParentObject();
    if (!pParent)
        return;

    CRect  rcParent;
    CRect  rcPanel;
    m_pObj->GetParentRect(&rcParent);
    pParent->GetClientRectInObject(&rcPanel);

    bool  bSetX = false;
    if (m_nConfigLeft >= 0)
    {
        bSetX = true;
        m_nConfigLeft = rcParent.left;
    }
    if (m_nConfigRight >= 0)
    {
        bSetX = true;
        m_nConfigRight = rcPanel.Width() - rcParent.right;
    }
    if (!bSetX)
    {
        m_nConfigLeft = rcParent.left;   // ��left/right��û��ָ��ʱ��ǿ������һ��
    }


    bool  bSetY = false;
    if (m_nConfigTop >= 0)
    {
        bSetY = true;
        m_nConfigTop = rcParent.top;
    }
    if (m_nConfigBottom >= 0)
    {
        bSetY = true;
        m_nConfigBottom = rcPanel.Height() - rcParent.bottom;
    }
    if (!bSetY)
    {
        m_nConfigTop = rcParent.top;   // ��top/bottom��û��ָ��ʱ��ǿ������һ��
    }

    if (m_nConfigWidth >= 0)
    {
        m_nConfigWidth = rcParent.Width();
    }
    if (m_nConfigHeight >= 0)
    {
        m_nConfigHeight = rcParent.Height();
    }
}
void  CanvasLayoutParam::Serialize(SERIALIZEDATA* pData)
{
    AttributeSerializer s(pData, TEXT("CanvasLayoutParam"));

	// width=""��Ҳ֧�� layout.width=""
    s.AddLong(
            XML_WIDTH,  this,
            memfun_cast<pfnLongSetter>(&CanvasLayoutParam::LoadConfigWidth), 
            memfun_cast<pfnLongGetter>(&CanvasLayoutParam::SaveConfigWidth)
        )
		->AddAlias(AUTO, XML_AUTO)
		->SetDefault(AUTO)
		->SetCompatibleKey(XML_LAYOUT_PREFIX XML_WIDTH);

    s.AddLong(
            XML_HEIGHT,  this,
            memfun_cast<pfnLongSetter>(&CanvasLayoutParam::LoadConfigHeight), 
            memfun_cast<pfnLongGetter>(&CanvasLayoutParam::SaveConfigHeight)
        )
		->AddAlias(AUTO, XML_AUTO)
		->SetDefault(AUTO)
		->SetCompatibleKey(XML_LAYOUT_PREFIX XML_HEIGHT);

    s.AddLong(
            XML_LAYOUT_ITEM_LEFT,  this,
            memfun_cast<pfnLongSetter>(&CanvasLayoutParam::LoadConfigLeft), 
            memfun_cast<pfnLongGetter>(&CanvasLayoutParam::SaveConfigLeft)
        )->SetDefault(NDEF);

    s.AddLong(
            XML_LAYOUT_ITEM_TOP,  this,
            memfun_cast<pfnLongSetter>(&CanvasLayoutParam::LoadConfigTop), 
            memfun_cast<pfnLongGetter>(&CanvasLayoutParam::SaveConfigTop)
        )->SetDefault(NDEF);

    s.AddLong(
            XML_LAYOUT_ITEM_RIGHT,  this,
            memfun_cast<pfnLongSetter>(&CanvasLayoutParam::LoadConfigRight), 
            memfun_cast<pfnLongGetter>(&CanvasLayoutParam::SaveConfigRight)
        )->SetDefault(NDEF);

    s.AddLong(
            XML_LAYOUT_ITEM_BOTTOM,  this,
            memfun_cast<pfnLongSetter>(&CanvasLayoutParam::LoadConfigBottom), 
            memfun_cast<pfnLongGetter>(&CanvasLayoutParam::SaveConfigBottom)
        )->SetDefault(NDEF);

    s.AddFlags(XML_LAYOUT_ITEM_ALIGN, m_nConfigLayoutFlags)
        ->AddFlag(LAYOUT_ITEM_ALIGN_LEFT,        XML_LAYOUT_ITEM_ALIGN_LEFT)
        ->AddFlag(LAYOUT_ITEM_ALIGN_RIGHT,       XML_LAYOUT_ITEM_ALIGN_RIGHT)
        ->AddFlag(LAYOUT_ITEM_ALIGN_TOP,         XML_LAYOUT_ITEM_ALIGN_TOP)
        ->AddFlag(LAYOUT_ITEM_ALIGN_BOTTOM,      XML_LAYOUT_ITEM_ALIGN_BOTTOM)
        ->AddFlag(LAYOUT_ITEM_ALIGN_CENTER,      XML_LAYOUT_ITEM_ALIGN_CENTER)
        ->AddFlag(LAYOUT_ITEM_ALIGN_VCENTER,     XML_LAYOUT_ITEM_ALIGN_VCENTER)
        ->AddFlag(LAYOUT_FLAG_FORCE_DESIREDSIZE, XML_LAYOUT_ALIGN_FORCE_DESIREDSIZE);
}

long  CanvasLayoutParam::GetConfigLeft()
{
    return m_nConfigLeft;
}
void  CanvasLayoutParam::LoadConfigLeft(long n)
{
    m_nConfigLeft = ScaleByDpi_if_gt0(n);
}
long  CanvasLayoutParam::SaveConfigLeft()
{
    return RestoreByDpi_if_gt0(m_nConfigLeft);
}
void  CanvasLayoutParam::SetConfigLeft(long n)
{
    m_nConfigLeft = n;
}


long  CanvasLayoutParam::GetConfigTop()
{
    return m_nConfigTop;
}
void  CanvasLayoutParam::LoadConfigTop(long n)
{
    m_nConfigTop = ScaleByDpi_if_gt0(n);
}
long  CanvasLayoutParam::SaveConfigTop()
{
    return RestoreByDpi_if_gt0(m_nConfigTop);
}
void  CanvasLayoutParam::SetConfigTop(long n)
{
    m_nConfigTop = n;
}

long  CanvasLayoutParam::GetConfigRight()
{
    return m_nConfigRight;
}
void  CanvasLayoutParam::LoadConfigRight(long n)
{
    m_nConfigRight = ScaleByDpi_if_gt0(n);
}
long  CanvasLayoutParam::SaveConfigRight()
{
    return RestoreByDpi_if_gt0(m_nConfigRight);
}
void  CanvasLayoutParam::SetConfigRight(long n)
{
    m_nConfigRight = n;
}

long  CanvasLayoutParam::GetConfigBottom()
{
    return m_nConfigBottom;
}
void  CanvasLayoutParam::LoadConfigBottom(long n)
{
    m_nConfigBottom = ScaleByDpi_if_gt0(n);
}
long  CanvasLayoutParam::SaveConfigBottom()
{
    return RestoreByDpi_if_gt0(m_nConfigBottom);
}
void  CanvasLayoutParam::SetConfigBottom(long n)
{
    m_nConfigBottom = n;
}

long  CanvasLayoutParam::GetConfigWidth()
{
    return m_nConfigWidth;
}
void  CanvasLayoutParam::SetConfigWidth(long n)
{
    m_nConfigWidth = n;
}
void  CanvasLayoutParam::LoadConfigWidth(long n)
{
    m_nConfigWidth = ScaleByDpi_if_gt0(n);
}
long  CanvasLayoutParam::SaveConfigWidth()
{
    return RestoreByDpi_if_gt0(m_nConfigWidth);
}

long  CanvasLayoutParam::GetConfigHeight()
{
    return m_nConfigHeight;
}
void  CanvasLayoutParam::SetConfigHeight(long n)
{
    m_nConfigHeight = n;
}
void  CanvasLayoutParam::LoadConfigHeight(long n)
{
    m_nConfigHeight = ScaleByDpi_if_gt0(n);
}
long  CanvasLayoutParam::SaveConfigHeight()
{
    return RestoreByDpi_if_gt0(m_nConfigHeight);
}

void  CanvasLayoutParam::SetConfigLayoutFlags(long n)
{
    m_nConfigLayoutFlags = n;
}
long  CanvasLayoutParam::GetConfigLayoutFlags()
{
    return m_nConfigLayoutFlags;
}

int  CanvasLayoutParam::ParseAlignAttr(LPCTSTR szAttr)
{
    Util::ISplitStringEnum*  pEnum = NULL;
    int nCount = Util::SplitString(szAttr, XML_FLAG_SEPARATOR, &pEnum);

    int nRet = 0;
    for (int i = 0; i < nCount; i++)
    {
        LPCTSTR  szFlag = pEnum->GetText(i);
        if (0 == _tcscmp(szFlag, XML_LAYOUT_ITEM_ALIGN_LEFT))
            nRet |= LAYOUT_ITEM_ALIGN_LEFT;
        else if (0 == _tcscmp(szFlag, XML_LAYOUT_ITEM_ALIGN_RIGHT))
            nRet |= LAYOUT_ITEM_ALIGN_RIGHT;
        else if (0 == _tcscmp(szFlag, XML_LAYOUT_ITEM_ALIGN_TOP))
            nRet |= LAYOUT_ITEM_ALIGN_TOP;
        else if (0 == _tcscmp(szFlag, XML_LAYOUT_ITEM_ALIGN_BOTTOM))
            nRet |= LAYOUT_ITEM_ALIGN_BOTTOM;
        else if (0 == _tcscmp(szFlag, XML_LAYOUT_ITEM_ALIGN_CENTER))
            nRet |= LAYOUT_ITEM_ALIGN_CENTER;
        else if (0 == _tcscmp(szFlag, XML_LAYOUT_ITEM_ALIGN_VCENTER))
            nRet |= LAYOUT_ITEM_ALIGN_VCENTER;
		else if (0 == _tcscmp(szFlag, XML_LAYOUT_ALIGN_FORCE_DESIREDSIZE))
			nRet |= LAYOUT_FLAG_FORCE_DESIREDSIZE;
    }
    SAFE_RELEASE(pEnum);

    return nRet;
}

bool  CanvasLayoutParam::IsSizedByContent()
{
    bool  bWidthNotConfiged = (m_nConfigLeft == NDEF || m_nConfigRight == NDEF) && m_nConfigWidth ==AUTO;
    bool  bHeightNotConfiged = (m_nConfigTop == NDEF || m_nConfigBottom == NDEF) && m_nConfigHeight == AUTO;

    if (bWidthNotConfiged || bHeightNotConfiged || 
        (m_nConfigLayoutFlags & LAYOUT_FLAG_FORCE_DESIREDSIZE))
        return true;

    return false;
}



}