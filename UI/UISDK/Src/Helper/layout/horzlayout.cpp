#include "stdafx.h"
#include "horzlayout.h"
#include "Src\Base\Object\object.h"
#include "Src\Base\Attribute\attribute.h"
#include "Src\Base\Attribute\enum_attribute.h"
#include "Src\Base\Attribute\flags_attribute.h"
#include "Src\Util\DPI\dpihelper.h"

namespace UI
{

HorzLayout::HorzLayout()
{
    m_nSpace = 0;
//	this->m_eDirection = LAYOUT_Horz_LEFTTORIGHT;   // Ĭ�϶�ջ�Ǵ�����
}
HorzLayout::~HorzLayout()
{
}
void  HorzLayout::Serialize(SERIALIZEDATA* pData)
{
//     AttributeSerializer s(pData, TEXT("HorzLayout"));
//
//     s.AddLong(XML_LAYOUT_HORZ_GAP, this,
//         memfun_cast<pfnLongSetter>(&HorzLayout::LoadGap),
//         memfun_cast<pfnLongGetter>(&HorzLayout::SaveGap));
// 
}

void  HorzLayout::LoadGap(long n)
{
    m_nSpace = ScaleByDpi_if_gt0(n);
}
long  HorzLayout::SaveGap()
{
    return RestoreByDpi_if_gt0(m_nSpace);
}

void  HorzLayout::SetSpace(int n)
{
    m_nSpace = n;
}

SIZE  HorzLayout::Measure()
{
	SIZE s = { 0 };

	// ���ؼ��ڼ��
	RECT rcPadding = { 0 };
	m_pPanel->GetPaddingRegion(&rcPadding);
	s.cx += rcPadding.left + rcPadding.right;
	s.cy += rcPadding.top + rcPadding.bottom;

	int nMaxHeight = 0;

	Object* pChild = NULL;
	while (pChild = m_pPanel->EnumChildObject(pChild))
	{
		// ����IsSelfCollapsed֮ǰ��editor��Ҳ��Ҫ�������ض���Ĳ�������
		HorzLayoutParam* pParam = s_GetObjectLayoutParam(pChild);
		if (!pParam)
			continue;

		if (pChild->IsSelfCollapsed())
			continue;

        if (pParam->m_eWidthType == WH_AUTO)
		{
			s.cx += pChild->GetDesiredSize().cx;
		}
        else
        {
            s.cx += pParam->m_nConfigWidth;
        }

		RECT rMargin = { 0 };
		pChild->GetMarginRegion(&rMargin);

		s.cx += rMargin.left + rMargin.right;

		int nHeight = rMargin.top + rMargin.bottom;

        if (pParam->m_eHeightType == WH_AUTO)
		{
			nHeight += pChild->GetDesiredSize().cy;
		}
        else
        {
            nHeight += pParam->m_nConfigHeight;
        }

        nMaxHeight = max(nMaxHeight, nHeight);
	}
	s.cy = nMaxHeight;

	return s;
}

// ����˳��
// 1. �����������֪�������Ĵ�С
// 2. �����ʣ����
// 3. ���������˳��

struct ObjLayoutInfo
{
	Object*  pObj;
	int width;  // ������margin
	int height; // ������margin
};

void  HorzLayout::DoArrage(IObject* pIObjToArrage)
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

	// ����ÿ���ӿؼ���Ҫ�Ŀ�ȼ��䲼��˳��
	vector<ObjLayoutInfo>  vecInfo(nChildCount);
	memset(&vecInfo[0], 0, sizeof(ObjLayoutInfo)*nChildCount);

	int nLeftCursor = 0;  // ����ָ������ؼ��Ĳ���˳�򣬾��Ҷ���ĵ÷�������
	int nRightCursor = nChildCount-1;

	int nNeedWidth = 0;
	int nAvgCount = 0;
	
	Object* pChild = NULL;
	while (pChild = m_pPanel->EnumChildObject(pChild))
	{
		// ����IsSelfCollapsed֮ǰ��editor��Ҳ��Ҫ�������ض���Ĳ�������
		HorzLayoutParam* pParam = s_GetObjectLayoutParam(pChild);
		if (!pParam)
			continue;

		if (pChild->IsSelfCollapsed())
			continue;

		//  ���㲼��˳��
		int nIndex = nLeftCursor;
		if (pParam->m_nConfigLayoutFlags & LAYOUT_ITEM_ALIGN_RIGHT)
		{
			nIndex = nRightCursor;
			nRightCursor--;
		}
		else
		{
			nIndex = nLeftCursor;
			nLeftCursor++;
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
        
        if (pParam->m_eWidthType == WH_AVG)
		{
			// ����ѭ���������ټ���
            // ƽ��ֵ�У�������margin
			nAvgCount++;
			nObjWidth = 0;
		}
        else if (pParam->m_eWidthType == WH_PERCENT)
        {
            
#if 0 // �����а���margin
            nObjWidth = rcParent.Width() * pParam->m_nConfigWidth / 100;
            nObjWidth -= pChild->GetMarginW();

#else // �����в�����margin
            nObjWidth = rcParent.Width() * pParam->m_nConfigWidth / 100;
#endif
        }
        else if (pParam->m_eWidthType != WH_AUTO)
        {
            nObjWidth = pParam->m_nConfigWidth;
        }

        if (pParam->m_eHeightType == WH_SET)
		{
			nObjHeight = pParam->m_nConfigHeight;
		}
        else if (pParam->m_eHeightType == WH_PERCENT)
        {
            nObjHeight = rcParent.Height() * pParam->m_nConfigHeight / 100;
        }

		vecInfo[nIndex].pObj = pChild;
		vecInfo[nIndex].width = nObjWidth;
		vecInfo[nIndex].height = nObjHeight;

		nNeedWidth += nObjWidth + pChild->GetMarginW();
	}

	// ����ƽ�����
	int nAvgWidth = 0;
	int nAvgDiff = 0;  // ����
	if (nAvgCount > 0)
	{
		int nTotal = (rcParent.Width() - nNeedWidth);
		nAvgWidth = (nTotal / nAvgCount);
		nAvgDiff = nTotal - (nAvgWidth * nAvgCount);
	}

	// ��ʼ����
	int nLeftConsume = 0;
	int nRightConsume = 0;
	for (int i = 0; i < nChildCount; i++)
	{
		ObjLayoutInfo& info = vecInfo[i];
		pChild = info.pObj;
		if (!pChild)
			continue;

		// ����IsSelfCollapsed֮ǰ��editor��Ҳ��Ҫ�������ض���Ĳ�������
		HorzLayoutParam* pParam = s_GetObjectLayoutParam(pChild);
		if (!pParam)
			continue;

		if (pChild->IsSelfCollapsed())
			continue;

		RECT rMargin = { 0 };
		pChild->GetMarginRegion(&rMargin);

		CRect rcObj;

		// �����ƽ����ȣ�Ϊ���ȸ�ֵ
        if (pParam->m_eWidthType == WH_AVG)
		{
			info.width = nAvgWidth + nAvgDiff;
			nAvgDiff = 0;
		}

		// ����x����
		
		if (pParam->m_nConfigLayoutFlags & LAYOUT_ITEM_ALIGN_RIGHT)
		{
			nRightConsume += rMargin.right;

			rcObj.right = rcParent.right - nRightConsume;
			rcObj.left = rcObj.right - info.width;

			nRightConsume += info.width + rMargin.left;
		}
		else // if (pParam->m_nConfigLayoutFlags & LAYOUT_ITEM_ALIGN_LEFT)
		{
			nLeftConsume += rMargin.left;

			rcObj.left = rcParent.left + nLeftConsume;
			rcObj.right = rcObj.left + info.width;

			nLeftConsume += info.width + rMargin.right;
		}

		// ����y����
		if ((pParam->m_nConfigLayoutFlags & LAYOUT_ITEM_ALIGN_TOP) &&
			(pParam->m_nConfigLayoutFlags & LAYOUT_ITEM_ALIGN_BOTTOM))
		{
			rcObj.top = rcParent.top + rMargin.top;
			rcObj.bottom = rcParent.bottom - rMargin.bottom;
		}
		else if (pParam->m_nConfigLayoutFlags & LAYOUT_ITEM_ALIGN_BOTTOM)
		{
			rcObj.bottom = rcParent.bottom - rMargin.bottom;
			rcObj.top = rcObj.bottom - info.height;
		}
		else if (pParam->m_nConfigLayoutFlags & LAYOUT_ITEM_ALIGN_VCENTER)
		{
			rcObj.top = rcParent.top + (rcParent.Height() - info.height) / 2;
			rcObj.top = rcObj.top + rMargin.top - rMargin.bottom;
			rcObj.bottom = rcObj.top + info.height;
		}
		else // LAYOUT_ITEM_ALIGN_TOP:
		{
			rcObj.top = rcParent.top + rMargin.top;
			rcObj.bottom = rcObj.top + info.height;
		}

		pChild->SetObjectPos(
			rcObj,
			SWP_NOREDRAW | SWP_NOUPDATELAYOUTPOS | SWP_FORCESENDSIZEMSG);
	}
}

void  HorzLayout::ChildObjectVisibleChanged(IObject* pObj)
{
    UIASSERT (pObj);
	UIASSERT(pObj->GetParentObject());
	UIASSERT(pObj->GetParentObject()->GetImpl() == m_pPanel);

    SetDirty(true);
    m_pPanel->Invalidate();
}

void  HorzLayout::ChildObjectContentSizeChanged(IObject* pObj)
{
    UIASSERT(pObj);
    UIASSERT(pObj->GetParentObject());
    UIASSERT(pObj->GetParentObject()->GetImpl() == m_pPanel);

    SetDirty(true);
    m_pPanel->Invalidate();
}

HorzLayoutParam::HorzLayoutParam()
{
    m_nConfigWidth = AUTO;
    m_nConfigHeight = AUTO;
    m_nConfigLayoutFlags = 0;
}

HorzLayoutParam::~HorzLayoutParam()
{

}

void  HorzLayoutParam::UpdateByRect()
{
    CRect  rcParent;
    m_pObj->GetParentRect(&rcParent);

    if (m_eWidthType == WH_SET)
    {
        m_nConfigWidth = rcParent.Width();
    }
    if (m_eHeightType == WH_SET)
    {
        m_nConfigHeight = rcParent.Height();
    }
}
void  HorzLayoutParam::Serialize(SERIALIZEDATA* pData)
{
    AttributeSerializer s(pData, TEXT("HorzLayoutParam"));
    
    s.AddString(
            XML_WIDTH,  this,
            memfun_cast<pfnStringSetter>(&HorzLayoutParam::LoadConfigWidth), 
            memfun_cast<pfnStringGetter>(&HorzLayoutParam::SaveConfigWidth)
        )
		->SetCompatibleKey(XML_LAYOUT_PREFIX XML_WIDTH);

    s.AddString(
            XML_HEIGHT,  this,
            memfun_cast<pfnStringSetter>(&HorzLayoutParam::LoadConfigHeight),
            memfun_cast<pfnStringGetter>(&HorzLayoutParam::SaveConfigHeight)
        )
		->SetCompatibleKey(XML_LAYOUT_PREFIX XML_HEIGHT);;

    s.AddFlags(XML_LAYOUT_ITEM_ALIGN, m_nConfigLayoutFlags)
        ->AddFlag(LAYOUT_ITEM_ALIGN_LEFT,        XML_LAYOUT_ITEM_ALIGN_LEFT)
        ->AddFlag(LAYOUT_ITEM_ALIGN_RIGHT,       XML_LAYOUT_ITEM_ALIGN_RIGHT)
        ->AddFlag(LAYOUT_ITEM_ALIGN_TOP,         XML_LAYOUT_ITEM_ALIGN_TOP)
        ->AddFlag(LAYOUT_ITEM_ALIGN_BOTTOM,      XML_LAYOUT_ITEM_ALIGN_BOTTOM)
        ->AddFlag(LAYOUT_ITEM_ALIGN_CENTER,      XML_LAYOUT_ITEM_ALIGN_CENTER)
        ->AddFlag(LAYOUT_ITEM_ALIGN_VCENTER,     XML_LAYOUT_ITEM_ALIGN_VCENTER)
        ->AddFlag(LAYOUT_ITEM_ALIGN_FILL,        XML_LAYOUT_ITEM_ALIGN_FILL);
}

long  HorzLayoutParam::GetConfigWidth()
{
    return m_nConfigWidth;
}
void  HorzLayoutParam::SetConfigWidth(long n)
{
    m_nConfigWidth = n;
}

void  HorzLayoutParam::LoadConfigWidth(LPCTSTR szText)
{
    LoadConfigWH(szText, m_nConfigWidth, m_eWidthType);
}

LPCTSTR HorzLayoutParam::SaveConfigWidth()
{
    return SaveConfigWH(m_nConfigWidth, m_eWidthType);
}

long  HorzLayoutParam::GetConfigHeight()
{
    return m_nConfigHeight;
}
void  HorzLayoutParam::SetConfigHeight(long n)
{
    m_nConfigHeight = n;
}
void  HorzLayoutParam::LoadConfigHeight(LPCTSTR szText)
{
    LoadConfigWH(szText, m_nConfigHeight, m_eHeightType);
}
LPCTSTR  HorzLayoutParam::SaveConfigHeight()
{
    return SaveConfigWH(m_nConfigHeight, m_eHeightType);
}

void  HorzLayoutParam::SetConfigLayoutFlags(long n)
{
    m_nConfigLayoutFlags = n;
}
long  HorzLayoutParam::GetConfigLayoutFlags()
{
    return m_nConfigLayoutFlags;
}

SIZE  HorzLayoutParam::CalcDesiredSize()
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

bool  HorzLayoutParam::IsSizedByContent()
{
    if (m_eWidthType != AUTO && m_eHeightType != AUTO)
        return false;

    return true;
}
}