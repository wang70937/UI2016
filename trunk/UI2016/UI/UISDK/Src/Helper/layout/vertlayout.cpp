#include "stdafx.h"
#include "vertlayout.h"
#include "Src\Base\Object\object.h"
#include "Src\Base\Attribute\attribute.h"
#include "Src\Base\Attribute\enum_attribute.h"
#include "Src\Base\Attribute\flags_attribute.h"
#include "Src\Util\DPI\dpihelper.h"

namespace UI
{

VertLayout::VertLayout()
{
    m_nSpace = 0;
//	this->m_eDirection = LAYOUT_Vert_LEFTTORIGHT;   // Ĭ�϶�ջ�Ǵ�����
}
VertLayout::~VertLayout()
{
}
void  VertLayout::Serialize(SERIALIZEDATA* pData)
{
//     AttributeSerializer s(pData, TEXT("VertLayout"));
//
//     s.AddLong(XML_LAYOUT_VERT_GAP, this,
//         memfun_cast<pfnLongSetter>(&VertLayout::LoadGap),
//         memfun_cast<pfnLongGetter>(&VertLayout::SaveGap));
// 
}

void  VertLayout::LoadGap(long n)
{
    m_nSpace = ScaleByDpi_if_gt0(n);
}
long  VertLayout::SaveGap()
{
    return RestoreByDpi_if_gt0(m_nSpace);
}

void  VertLayout::SetSpace(int n)
{
    m_nSpace = n;
}

SIZE  VertLayout::Measure()
{
	SIZE s = { 0 };

	// ���ؼ��ڼ��
	RECT rcPadding = { 0 };
	m_pPanel->GetPaddingRegion(&rcPadding);
	s.cx += rcPadding.left + rcPadding.right;
	s.cy += rcPadding.top + rcPadding.bottom;

	int nMaxWidth = 0;

	Object* pChild = NULL;
	while (pChild = m_pPanel->EnumChildObject(pChild))
	{
		// ����IsSelfCollapsed֮ǰ��editor��Ҳ��Ҫ�������ض���Ĳ�������
		VertLayoutParam* pParam = s_GetObjectLayoutParam(pChild);
		if (!pParam)
			continue;

		if (pChild->IsSelfCollapsed())
			continue;

		if (pParam->m_eHeightType == AUTO)
		{
			s.cy += pChild->GetDesiredSize().cy;
		}
        else
		{
			s.cy += pParam->m_nConfigHeight;
		} 

		RECT rMargin = { 0 };
		pChild->GetMarginRegion(&rMargin);

		s.cy += rMargin.top + rMargin.bottom;

		int nWidth = rMargin.left + rMargin.right;
		
        if (pParam->m_eHeightType == AUTO)
		{
			nWidth += pChild->GetDesiredSize().cx;
		}
		else 
		{
			nWidth += pParam->m_nConfigWidth;
		}
        

        nMaxWidth = max(nMaxWidth, nWidth);
	}
	s.cx = nMaxWidth;

	return s;

}

struct ObjLayoutInfo
{
	Object*  pObj;
	int width;  // ������margin
	int height; // ������margin
};

void  VertLayout::DoArrage(IObject* /*pIObjToArrage*/)
{
	// ����ÿ���ӿؼ���Ҫ�Ŀ�ȼ��䲼��˳��
	int nChildCount = m_pPanel->GetChildCount();
	if (nChildCount <= 0)
		return;

	// ���ؼ��ڼ��
	RECT rcPadding = { 0 };
	m_pPanel->GetPaddingRegion(&rcPadding);

	CRect rcParent;
	m_pPanel->GetObjectClientRect(&rcParent);

	vector<ObjLayoutInfo>  vecInfo(nChildCount);
	memset(&vecInfo[0], 0, sizeof(ObjLayoutInfo)*nChildCount);

	int nTopCursor = 0;  // ����ָ������ؼ��Ĳ���˳�򣬾��Ҷ���ĵ÷�������
	int nBottomCursor = nChildCount-1;

	int nNeedHeight = 0;
	int nAvgCount = 0;

	Object* pChild = NULL;
	while (pChild = m_pPanel->EnumChildObject(pChild))
	{
		// ����IsSelfCollapsed֮ǰ��editor��Ҳ��Ҫ�������ض���Ĳ�������
		VertLayoutParam* pParam = s_GetObjectLayoutParam(pChild);
		if (!pParam)
			continue;

		if (pChild->IsSelfCollapsed())
			continue;

		//  ���㲼��˳��
		int nIndex = nTopCursor;
		if (pParam->m_nConfigLayoutFlags & LAYOUT_ITEM_ALIGN_BOTTOM)
		{
			nIndex = nBottomCursor;
			nBottomCursor--;
		}
		else
		{
			nIndex = nTopCursor;
			nTopCursor++;
		}

		// ��������Ҫ�Ŀ��
		int nObjWidth = 0;
		int nObjHeight = 0;

		if (pParam->IsSizedByContent())
		{
			SIZE s = pChild->GetDesiredSize();

			nObjWidth = s.cx - pChild->GetMarginW();
			nObjHeight = s.cy - pChild->GetMarginH();
		}

        if (pParam->m_eHeightType == WH_AVG)
		{
			// ����ѭ���������ټ���
			nAvgCount++;
			nObjHeight = 0;
		}
		else if (pParam->m_eHeightType == WH_PERCENT)
		{
#if 0 // �����а���margin
            nObjHeight = rcParent.Height() * pParam->m_nConfigHeight / 100;
            nObjHeight -= pChild->GetMarginH();

#else // �����в�����margin
            nObjHeight = rcParent.Height() * pParam->m_nConfigHeight / 100;
#endif
		}
		else if (pParam->m_eHeightType != WH_AUTO)
		{
			nObjHeight = pParam->m_nConfigHeight;
		}
         

        if (pParam->m_eWidthType == WH_SET)
		{
			nObjWidth = pParam->m_nConfigWidth;
		}
        else if (pParam->m_eWidthType == WH_PERCENT)
        {
            nObjWidth = rcParent.Width() * pParam->m_nConfigWidth / 100;
        }

		ObjLayoutInfo& info = vecInfo[nIndex];
		info.pObj = pChild;
		info.width = nObjWidth;
		info.height = nObjHeight;

		nNeedHeight += nObjHeight + pChild->GetMarginH();
	}

	// ����ƽ�����
	int nAvgHeight = 0;
	int nAvgDiff = 0;  // ����
	if (nAvgCount > 0)
	{
		int nTotal = (rcParent.Height()- nNeedHeight);
		nAvgHeight = ((rcParent.Height()-nNeedHeight) / nAvgCount);
		nAvgDiff = nTotal - (nAvgHeight * nAvgCount);
	}

	// ��ʼ����
	int nTopConsume = 0;
	int nBottomConsume = 0;

	for (int i = 0; i < nChildCount; i++)
	{
		ObjLayoutInfo& info = vecInfo[i];
		pChild = info.pObj;
		if (!pChild)
			continue;

		// ����IsSelfCollapsed֮ǰ��editor��Ҳ��Ҫ�������ض���Ĳ�������
		VertLayoutParam* pParam = s_GetObjectLayoutParam(pChild);
		if (!pParam)
			continue;

		if (pChild->IsSelfCollapsed())
			continue;

		RECT rMargin = { 0 };
		pChild->GetMarginRegion(&rMargin);

		CRect rcObj;

		// �����ƽ����ȣ�Ϊ���ȸ�ֵ
        if (pParam->m_eHeightType == WH_AVG)
		{
			info.height = nAvgHeight;
			info.height += nAvgDiff;
			nAvgDiff = 0;
		}

		// ����y����
		if (pParam->m_nConfigLayoutFlags & LAYOUT_ITEM_ALIGN_BOTTOM)
		{
			nBottomConsume += rMargin.bottom;

			rcObj.bottom = rcParent.bottom - nBottomConsume;
			rcObj.top = rcObj.bottom - info.height;

			nBottomConsume += info.height + rMargin.top;
		}
		else // if (pParam->m_nConfigLayoutFlags & LAYOUT_ITEM_ALIGN_TOP)
		{
			nTopConsume += rMargin.top;

			rcObj.top = rcParent.top + nTopConsume;
			rcObj.bottom = rcObj.top + info.height;

			nTopConsume += info.height + rMargin.bottom;
		}

		// ����x����
		if ((pParam->m_nConfigLayoutFlags & LAYOUT_ITEM_ALIGN_LEFT) &&
			(pParam->m_nConfigLayoutFlags & LAYOUT_ITEM_ALIGN_RIGHT))
		{
			rcObj.left = rcParent.left + rMargin.left;
			rcObj.right = rcParent.right - rMargin.right;
		}
		else if (pParam->m_nConfigLayoutFlags & LAYOUT_ITEM_ALIGN_RIGHT)
		{
			rcObj.right = rcParent.right - rMargin.right;
			rcObj.left = rcObj.right - info.width;
		}
		else if (pParam->m_nConfigLayoutFlags & LAYOUT_ITEM_ALIGN_CENTER)
		{
			rcObj.left = rcParent.left + (rcParent.Width() - info.width) / 2;
			rcObj.left = rcObj.left + rMargin.left - rMargin.right;
			rcObj.right = rcObj.left + info.width;
		}
		else // if (pParam->m_nConfigLayoutFlags & LAYOUT_ITEM_ALIGN_LEFT)
		{
			rcObj.left = rcParent.left + rMargin.left;
			rcObj.right = rcObj.left + info.width;
		}

		pChild->SetObjectPos(
			rcObj,
			SWP_NOREDRAW|SWP_NOUPDATELAYOUTPOS|SWP_FORCESENDSIZEMSG);
	}

}

void  VertLayout::ChildObjectVisibleChanged(IObject* pObj)
{
    UIASSERT (pObj);
	UIASSERT(pObj->GetParentObject());
	UIASSERT(pObj->GetParentObject()->GetImpl() == m_pPanel);

    SetDirty(true);
    m_pPanel->Invalidate();
}

void  VertLayout::ChildObjectContentSizeChanged(IObject* pObj)
{
    UIASSERT(pObj);
    UIASSERT(pObj->GetParentObject());
    UIASSERT(pObj->GetParentObject()->GetImpl() == m_pPanel);

    SetDirty(true);
    m_pPanel->Invalidate();
}

//////////////////////////////////////////////////////////////////////////

VertLayoutParam::VertLayoutParam()
{
    m_nConfigWidth = AUTO;
    m_nConfigHeight = AUTO;
    m_nConfigLayoutFlags = 0;
}
VertLayoutParam::~VertLayoutParam()
{

}

void  VertLayoutParam::UpdateByRect()
{
    CRect  rcParent;
    m_pObj->GetParentRect(&rcParent);

    if (m_nConfigWidth >= 0)
    {
        m_nConfigWidth = rcParent.Width();
    }
    if (m_nConfigHeight >= 0)
    {
        m_nConfigHeight = rcParent.Height();
    }
}

void  VertLayoutParam::Serialize(SERIALIZEDATA* pData)
{
    AttributeSerializer s(pData, TEXT("VertLayoutParam"));

    s.AddString(
            XML_WIDTH, this,
            memfun_cast<pfnStringSetter>(&VertLayoutParam::LoadConfigWidth),
            memfun_cast<pfnStringGetter>(&VertLayoutParam::SaveConfigWidth)
        )
        ->SetCompatibleKey(XML_LAYOUT_PREFIX XML_WIDTH);

    s.AddString(
            XML_HEIGHT, this,
            memfun_cast<pfnStringSetter>(&VertLayoutParam::LoadConfigHeight),
            memfun_cast<pfnStringGetter>(&VertLayoutParam::SaveConfigHeight)
        )
		->SetCompatibleKey(XML_LAYOUT_PREFIX XML_HEIGHT);;

    s.AddFlags(XML_LAYOUT_ITEM_ALIGN, m_nConfigLayoutFlags)
        ->AddFlag(LAYOUT_ITEM_ALIGN_LEFT, XML_LAYOUT_ITEM_ALIGN_LEFT)
        ->AddFlag(LAYOUT_ITEM_ALIGN_RIGHT, XML_LAYOUT_ITEM_ALIGN_RIGHT)
        ->AddFlag(LAYOUT_ITEM_ALIGN_TOP, XML_LAYOUT_ITEM_ALIGN_TOP)
        ->AddFlag(LAYOUT_ITEM_ALIGN_BOTTOM, XML_LAYOUT_ITEM_ALIGN_BOTTOM)
        ->AddFlag(LAYOUT_ITEM_ALIGN_CENTER, XML_LAYOUT_ITEM_ALIGN_CENTER)
        ->AddFlag(LAYOUT_ITEM_ALIGN_VCENTER, XML_LAYOUT_ITEM_ALIGN_VCENTER)
        ->AddFlag(LAYOUT_ITEM_ALIGN_FILL, XML_LAYOUT_ITEM_ALIGN_FILL);
}

long  VertLayoutParam::GetConfigWidth()
{
    return m_nConfigWidth;
}
void  VertLayoutParam::SetConfigWidth(long n)
{
    m_nConfigWidth = n;
}
void  VertLayoutParam::LoadConfigWidth(LPCTSTR szText)
{
    LoadConfigWH(szText, m_nConfigWidth, m_eWidthType);
}
LPCTSTR  VertLayoutParam::SaveConfigWidth()
{
    return SaveConfigWH(m_nConfigWidth, m_eWidthType);
}

long  VertLayoutParam::GetConfigHeight()
{
    return m_nConfigHeight;
}
void  VertLayoutParam::SetConfigHeight(long n)
{
    m_nConfigHeight = n;
}
void  VertLayoutParam::LoadConfigHeight(LPCTSTR szText)
{
    LoadConfigWH(szText, m_nConfigHeight, m_eHeightType);
}
LPCTSTR  VertLayoutParam::SaveConfigHeight()
{
    return SaveConfigWH(m_nConfigHeight, m_eHeightType);
}

void  VertLayoutParam::SetConfigLayoutFlags(long n)
{
    m_nConfigLayoutFlags = n;
}
long  VertLayoutParam::GetConfigLayoutFlags()
{
    return m_nConfigLayoutFlags;
}

SIZE  VertLayoutParam::CalcDesiredSize()
{
    SIZE size = {0,0};

    if (IsSizedByContent())
    {
        // ��ȡ�Ӷ�������Ҫ�Ŀռ�
        UISendMessage(m_pObj, UI_MSG_GETDESIREDSIZE, (WPARAM)&size);

        // �����ָ��width��height������һ������ô��������һ���еõ���ֵ
        if (this->m_eWidthType != AUTO)
            size.cx = this->m_nConfigWidth;
        if (this->m_eHeightType != AUTO)
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

bool  VertLayoutParam::IsSizedByContent()
{
    if (m_eWidthType != AUTO && m_eHeightType != AUTO)
        return false;

    return true;
}
}