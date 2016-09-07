#include "stdafx.h"
#include "colorrender.h"
#include "Src\Base\Object\object.h"
#include "Src\Base\Attribute\attribute.h"
#include "Src\Base\Attribute\string_attribute.h"
#include "Src\Base\Attribute\rect_attribute.h"
#include "Src\Base\Attribute\enum_attribute.h"
#include "Inc\Interface\iuires.h"
#include "Inc\Interface\renderlibrary.h"

//////////////////////////////////////////////////////////////////////////////////////////
//                                                                                      //
//                               ColorRender                                            //
//                                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////


void  ColorBorderRender_DrawState(RENDERBASE_DRAWSTATE* pDrawStruct, Object* pObj, COLORREF color, const CRegion4& rBorder)
{
    IRenderTarget* pRenderTarget = pDrawStruct->pRenderTarget;
    if (NULL == pRenderTarget)
        return ;

    // ���Ʊ߿�����ֱ�ӵ���Rectangle����ָ����ȵ�PEN����л��ƣ��ᵼ��PEN��
    // һ������λ�ڿؼ������������档�����������FillRect��ʵ�ֱ߿�Ļ��ơ�
    // ͬʱҲ֧���˵������߿��С��һ��ʱ�Ļ���

#if 0
    if (NULL == pObj)
        return;

    CRegion4 rBorder;
    pObj->GetBorderRegion(&rBorder);
#ifdef _DEBUG
    if (rBorder.left==0 && rBorder.right==0 && rBorder.top==0 && rBorder.bottom==0)
    {
        UI_LOG_WARN(_T("%s ָ����border color����û������border"), FUNC_NAME);
    }
#endif
#endif

    Color c(color);
    c.a = 255;
    if (0 != rBorder.left)
    {
        CRect rcLeft = pDrawStruct->rc;
        rcLeft.right = rcLeft.left+rBorder.left;
        
        pRenderTarget->DrawRect(&rcLeft, &c);
    }
    if (0 != rBorder.top)
    {
        CRect rcTop = pDrawStruct->rc;
        rcTop.bottom = rcTop.top+rBorder.top;
        pRenderTarget->DrawRect(&rcTop, &c);
    }
    if (0 != rBorder.right)
    {
        CRect rcRight = pDrawStruct->rc;
        rcRight.left = rcRight.right-rBorder.right;
        pRenderTarget->DrawRect(&rcRight, &c);
    }
    if (0 != rBorder.bottom)
    {
        CRect rcBottom = pDrawStruct->rc;
        rcBottom.top = rcBottom.bottom - rBorder.bottom;
        pRenderTarget->DrawRect(&rcBottom, &c);
    }
}

//////////////////////////////////////////////////////////////////////////

ColorRender::ColorRender(IColorRender* p) : RenderBase(p)
{
    m_pIColorRender = p;
	m_pBkColor = NULL;
    m_pBorderColor = NULL;
    m_rcBorder.SetRectEmpty();
}
ColorRender::~ColorRender()
{
	SAFE_RELEASE(m_pBkColor);
    SAFE_RELEASE(m_pBorderColor);
}

void ColorRender::SetBkColor( COLORREF col )
{
	SAFE_RELEASE(m_pBkColor);
    m_pBkColor = Color::CreateInstance(col);
}
void ColorRender::SetBorderColor(COLORREF col)
{
    SAFE_RELEASE(m_pBorderColor);
    m_pBorderColor = Color::CreateInstance(col);
}
void  ColorRender::SetBorderRegion(const CRegion4* prc)
{
    if (NULL == prc)
    {
        m_rcBorder.SetRectEmpty();
    }
    else
    {
        m_rcBorder.CopyRect(prc);
    }
}

void ColorRender::OnSerialize(SERIALIZEDATA* pData)
{
	AttributeSerializer s(pData, TEXT("ColorRender"));
	s.AddString(XML_RENDER_COLOR, this, 
		memfun_cast<pfnStringSetter>(&ColorRender::LoadBkColor),
		memfun_cast<pfnStringGetter>(&ColorRender::GetBkColorId));
	s.AddString(XML_RENDER_BORDERCOLOR, this, 
		memfun_cast<pfnStringSetter>(&ColorRender::LoadBorderColor),
		memfun_cast<pfnStringGetter>(&ColorRender::GetBorderColorId));
	s.AddRect(XML_RENDER_BORDER, m_rcBorder);
} 

void ColorRender::DrawState(RENDERBASE_DRAWSTATE* pDrawStruct)
{
    IRenderTarget* pRenderTarget = pDrawStruct->pRenderTarget;
	if (NULL == pRenderTarget)
		return ;

	if (m_pBkColor)
    {
 		pRenderTarget->DrawRect(&pDrawStruct->rc, m_pBkColor);
    }

    if (m_pBorderColor)
    {
        ColorBorderRender_DrawState(pDrawStruct, m_pObject, m_pBorderColor->m_col, &m_rcBorder);
    }
}



//////////////////////////////////////////////////////////////////////////////////////////
//                                                                                      //
//                               SysColorRender                                         //
//                                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////

SysColorRender::SysColorRender(ISysColorRender* p):RenderBase(p)
{
    m_pISysColorRender = p;

	m_nBkColorIndex = -1;
	m_nBorderColorIndex = -1;
    ::SetRectEmpty(&m_rcBorder);
}

SysColorRender::~SysColorRender()
{

}

void SysColorRender::SetBkColor(int nColorIndex)
{
	m_nBkColorIndex = nColorIndex;
	// 	if (m_nBkColorIndex != -1)
	// 	{
	// 		m_bkColor = ::GetSysColor(m_nBkColorIndex);
	// 	}
}
void SysColorRender::SetBorderColor(int nColorIndex)
{
	m_nBorderColorIndex = nColorIndex;
	// 	if (m_nBorderColorIndex != -1)
	// 	{
	// 		m_borderColor = ::GetSysColor(m_nBorderColorIndex);
	// 	}
}

// void SysColorRender::OnThemeChanged()
// {
// 	UI_LOG_DEBUG(_T("%s,1. %d"), FUNC_NAME, m_bkColor);
// 	if (m_nBkColorIndex != -1)
// 	{
// 		UI_LOG_DEBUG(_T("XXXX"));
// 		Sleep(100); // 1250, OK, 100, 50 10 BAD
// 		m_bkColor = ::GetSysColor(m_nBkColorIndex);
// 	}
// 	if (m_nBorderColorIndex != -1)
// 	{
// 		UI_LOG_DEBUG(_T("DDDDD"));
// 		m_borderColor = ::GetSysColor(m_nBorderColorIndex);
// 	}
// 	UI_LOG_DEBUG(_T("%s,2. %d"), FUNC_NAME, m_bkColor);
// }

void SysColorRender::OnSerialize(SERIALIZEDATA* pData)
{
    AttributeSerializer s(pData, TEXT("SysColorRender"));
    s.AddLong(XML_RENDER_COLOR, m_nBkColorIndex)->SetDefault(-1);
    s.AddLong(XML_RENDER_BORDERCOLOR, m_nBorderColorIndex)->SetDefault(-1);
    s.AddRect(XML_RENDER_BORDER, m_rcBorder);
}

void SysColorRender::DrawState(RENDERBASE_DRAWSTATE* pDrawStruct)
{
    IRenderTarget* pRenderTarget = pDrawStruct->pRenderTarget;
	if (NULL == pRenderTarget)
		return ;

	if (-1 != m_nBkColorIndex)
    {
        Color c(GetSysColor(m_nBkColorIndex));
        c.a = 255;

		pRenderTarget->DrawRect(&pDrawStruct->rc, &c);
    }

	// ���Ʊ߿�����ֱ�ӵ���Rectangle����ָ����ȵ�PEN����л��ƣ��ᵼ��PEN��
	// һ������λ�ڿؼ������������档�����������FillRect��ʵ�ֱ߿�Ļ��ơ�
	// ͬʱҲ֧���˵������߿��С��һ��ʱ�Ļ���

	if (-1 == m_nBorderColorIndex)
		return;

	COLORREF colorborder = GetSysColor(m_nBorderColorIndex)|0xff000000;
    ColorBorderRender_DrawState(pDrawStruct, m_pObject, colorborder, &m_rcBorder);

// 	CRegion4 rBorder;
// 	m_pObject->GetBorderRegion(&rBorder);
// 	if (0 != rBorder.left)
// 	{
// 		CRect rcLeft = *prc;
// 		rcLeft.right = rBorder.left;
// 		pRenderTarget->FillRect(&rcLeft, colorborder);
// 	}
// 	if (0 != rBorder.top)
// 	{
// 		CRect rcTop = *prc;
// 		rcTop.bottom = rBorder.top;
// 		pRenderTarget->FillRect(&rcTop, colorborder);
// 	}
// 	if (0 != rBorder.right)
// 	{
// 		CRect rcRight = *prc;
// 		rcRight.left = rcRight.right-rBorder.right;
// 		pRenderTarget->FillRect(&rcRight, colorborder);
// 	}
// 	if (0 != rBorder.bottom)
// 	{
// 		CRect rcBottom = *prc;
// 		rcBottom.top = rcBottom.bottom - rBorder.bottom;
// 		pRenderTarget->FillRect(&rcBottom, colorborder);
// 	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//                                                                                      //
//                               GradientRender                                  //
//                                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////


GradientRender::GradientRender(IGradientRender* p):RenderBase(p)
{
    m_pIGradientRender = p;
    m_pBorderColor = NULL;
    SetRectEmpty(&m_rcBorder);

	m_pColorFrom = NULL;
	m_pColorTo = NULL;
    m_nGradientDir = RENDER_COLOR_GRADIENT_DIR_H;
}
GradientRender::~GradientRender()
{
	SAFE_RELEASE(m_pColorFrom);
	SAFE_RELEASE(m_pColorTo);
    SAFE_RELEASE(m_pBorderColor);
}

void GradientRender::OnSerialize(SERIALIZEDATA* pData)
{
    AttributeSerializer s(pData, TEXT("GradientRender"));
    s.AddString(XML_RENDER_COLORFROM, this, 
        memfun_cast<pfnStringSetter>(&GradientRender::LoadFromColor),
        memfun_cast<pfnStringGetter>(&GradientRender::SaveFromColor));
    s.AddString(XML_RENDER_COLORTO, this, 
        memfun_cast<pfnStringSetter>(&GradientRender::LoadToColor),
        memfun_cast<pfnStringGetter>(&GradientRender::SaveToColor));
    s.AddString(XML_RENDER_BORDERCOLOR, this, 
        memfun_cast<pfnStringSetter>(&GradientRender::LoadBorderColor),
        memfun_cast<pfnStringGetter>(&GradientRender::SaveBorderColor));
    s.AddRect(XML_RENDER_BORDER, m_rcBorder);
    s.AddEnum(XML_RENDER_COLOR_GRADIENT_DIR, *(long*)&m_nGradientDir)
        ->AddOption(RENDER_COLOR_GRADIENT_DIR_H, XML_RENDER_COLOR_GRADIENT_DIR_H)
        ->AddOption(RENDER_COLOR_GRADIENT_DIR_V, XML_RENDER_COLOR_GRADIENT_DIR_V);
} 

void GradientRender::DrawState(RENDERBASE_DRAWSTATE* pDrawStruct)
{
    IRenderTarget* pRenderTarget = pDrawStruct->pRenderTarget;
	if(NULL == pRenderTarget || NULL == m_pColorFrom || NULL == m_pColorTo)
		return;

	CRect rc(0,0, this->m_pObject->GetWidth(), this->m_pObject->GetHeight());

	if (m_nGradientDir == RENDER_COLOR_GRADIENT_DIR_H)
	{
		pRenderTarget->GradientFillH(&pDrawStruct->rc, m_pColorFrom->m_col, m_pColorTo->m_col);
	}
	else if (m_nGradientDir == RENDER_COLOR_GRADIENT_DIR_V)
	{
		pRenderTarget->GradientFillV(&pDrawStruct->rc, m_pColorFrom->m_col, m_pColorTo->m_col);
	}

    if (m_pBorderColor)
    {
        ColorBorderRender_DrawState(pDrawStruct, m_pObject, m_pBorderColor->m_col, &m_rcBorder);
    }
}



//////////////////////////////////////////////////////////////////////////////////////////
//                                                                                      //
//                               ColorListRender                                        //
//                                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////

ColorListRender::ColorListRender(IColorListRender* p) : RenderBase(p)
{
    m_pIColorListRender = p;
	m_nCount = 0;
}

ColorListRender::~ColorListRender()
{
	this->Clear();
}
void  ColorListRender::Clear()
{
	for (int i = 0; i < m_nCount; i++)
	{
		SAFE_RELEASE(m_vBkColor[i]);
		SAFE_RELEASE(m_vBorderColor[i]);
	}
	m_vBkColor.clear();
	m_vBorderColor.clear();
}

void ColorListRender::SetStateColor(int nState, COLORREF colorBk, bool bSetBk, COLORREF colBorder, bool bSetBorder)
{
	nState = LOWORD(nState);
	if (m_nCount <= nState)
	{
		return;
	}

	if (bSetBk)
	{
		SAFE_RELEASE(m_vBkColor[nState]);
        m_vBkColor[nState] = Color::CreateInstance(colorBk);
	}
	if (bSetBorder)
	{
		SAFE_RELEASE(m_vBorderColor[nState]);
        m_vBorderColor[nState] = Color::CreateInstance(colBorder);
	}
}

void  ColorListRender::SetCount(long n)
{
	if (n < 0)
		return;

	this->Clear();

	for(int i = 0; i < n; i++ )
	{
		m_vBkColor.push_back((Color*)NULL);
		m_vBorderColor.push_back((Color*)NULL);
	}
	m_nCount = n;
}
long  ColorListRender::GetCount()
{
    return m_nCount;
}

void  ColorListRender::LoadBkColor(LPCTSTR szText)
{
    if (!szText)
        return;

    IColorRes* pColorRes = GetSkinColorRes();
    if (NULL == pColorRes)
        return;

    vector<String> vColors;
    UI_Split(szText, XML_MULTI_SEPARATOR, vColors);
    int nCount = (int)vColors.size();

    if (0 == m_nCount)
        this->SetCount(nCount); //  ���δ��ʾָ��count�����Զ�ȡ����Ĵ�С

    for (int i = 0; i < m_nCount && i < nCount; i++ )
    {
        if (!vColors[i].empty())
        {
            pColorRes->GetColor(vColors[i].c_str(), &m_vBkColor[i]);
        }
    }
}
LPCTSTR  ColorListRender::SaveBkColor()
{
    String&  strBuffer = GetTempBufferString();
    for (int i = 0; i < m_nCount; i++)
    {
        if (i > 0)
            strBuffer.push_back(XML_MULTI_SEPARATOR);

        LPCTSTR szTemp = _GetColorId(m_vBkColor[i]);
        if (szTemp)
            strBuffer.append(szTemp);
    }

    return strBuffer.c_str();
}
void  ColorListRender::LoadBorderColor(LPCTSTR szText)
{
    IColorRes* pColorRes = GetSkinColorRes();
    if (NULL == pColorRes)
        return;

    if (!szText)
        return;
    
    vector<String> vColors;
    UI_Split(szText, XML_MULTI_SEPARATOR, vColors);
    int nCount = (int)vColors.size();

    if (0 == m_nCount)
        this->SetCount(nCount); //  ���δ��ʾָ��count�����Զ�ȡ����Ĵ�С

    for (int i = 0; i < m_nCount && i < nCount; i++ )
    {
        if (!vColors[i].empty())
        {
            pColorRes->GetColor(vColors[i].c_str(), &m_vBorderColor[i]);
        }
    }
}

LPCTSTR  ColorListRender::SaveBorderColor()
{
    String&  strBuffer = GetTempBufferString();
    for (int i = 0; i < m_nCount; i++)
    {
        if (i > 0)
            strBuffer.push_back(XML_MULTI_SEPARATOR);

        LPCTSTR szTemp = _GetColorId(m_vBorderColor[i]);
        if (szTemp)
            strBuffer.append(szTemp);
    }

    return strBuffer.c_str();
}

void ColorListRender::OnSerialize(SERIALIZEDATA* pData)
{
    AttributeSerializer s(pData, TEXT("ColorListRender"));

    s.AddLong(XML_RENDER_COLORLIST_COUNT, this, 
        memfun_cast<pfnLongSetter>(&ColorListRender::SetCount),
        memfun_cast<pfnLongGetter>(&ColorListRender::GetCount));
    s.AddString(XML_RENDER_COLOR, this, 
        memfun_cast<pfnStringSetter>(&ColorListRender::LoadBkColor),
        memfun_cast<pfnStringGetter>(&ColorListRender::SaveBkColor));
    s.AddString(XML_RENDER_BORDERCOLOR, this, 
        memfun_cast<pfnStringSetter>(&ColorListRender::LoadBorderColor),
        memfun_cast<pfnStringGetter>(&ColorListRender::SaveBorderColor));
}

void ColorListRender::DrawState(RENDERBASE_DRAWSTATE* pDrawStruct)
{
    IRenderTarget* pRenderTarget = pDrawStruct->pRenderTarget;
	if (NULL == pRenderTarget)
		return;
    
    if (0 == m_nCount)
        return;

	int nRealState = LOWORD(pDrawStruct->nState);
	if (nRealState >= m_nCount)
		nRealState = 0;

	if (NULL == m_vBorderColor[nRealState])   // �����Ʊ߿�
	{
		if (m_vBkColor[nRealState])
		{
			pRenderTarget->DrawRect(&pDrawStruct->rc, m_vBkColor[nRealState]);
		}
	}
	else                           // ���Ʊ߿�
	{
		if (m_vBkColor[nRealState])
			pRenderTarget->Rectangle(&pDrawStruct->rc, m_vBorderColor[nRealState], m_vBkColor[nRealState], 1, false);
		else
			pRenderTarget->Rectangle(&pDrawStruct->rc, m_vBorderColor[nRealState], 0,1,true);
	}
}
