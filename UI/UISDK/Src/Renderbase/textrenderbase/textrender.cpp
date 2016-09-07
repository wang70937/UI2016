#include "stdafx.h"
#include "textrender.h"

#include "Src\Renderbase\renderbase\renderbase.h"
#include "Src\Base\Object\object.h"
#include "Inc\Interface\imapattr.h"
#include "Src\UIObject\Window\windowbase.h"
#include "Inc\Interface\iuires.h"
#include "Src\Atl\image.h"
#include "Src\Base\Attribute\attribute.h"
#include "Src\Base\Attribute\flags_attribute.h"
#include "Src\Base\Attribute\enum_attribute.h"
#include "Src\Base\Attribute\string_attribute.h"
#include "Src\Resource\skinres.h"
#include "Src\Layer\windowrender.h"
#include "Src\Base\Application\uiapplication.h"
#include "Src\Resource\colorres.h"

//////////////////////////////////////////////////////////////////////////////////////////
//                                                                                      //
//                                  TextRender                                          //
//                                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////

#define DEFAULT_DRAWTEXT_FLAG (DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_NOPREFIX)
TextRenderBase::TextRenderBase(ITextRenderBase* p) : Message(p)
{
    m_pITextRenderBase = p;
    m_pUIApplication = NULL;
	m_pObject = NULL;
	m_nTextRenderType = TEXTRENDER_TYPE_NULL;
	m_nDrawTextFlag = DEFAULT_DRAWTEXT_FLAG;
    m_eDrawTextEffect = TEXT_EFFECT_NONE;
    m_pColorTextBkgnd = NULL;
    m_wparamDrawText = m_lparamDrawText = 0;
}
TextRenderBase::~TextRenderBase()
{
    SAFE_RELEASE(m_pColorTextBkgnd);
}
void TextRenderBase::SetTextAlignment(int nDrawFlag)
{
	m_nDrawTextFlag = nDrawFlag;
}



void TextRenderBase::Serialize(AttributeSerializer* ps)
{
	ps->AddFlags(XML_TEXTRENDER_ALIGN, m_nDrawTextFlag)
// 		->AddFlag(DT_TOP, XML_TEXTRENDER_ALIGN_TOP)   // 0��������������
// 		->AddFlag(DT_LEFT, XML_TEXTRENDER_ALIGN_LEFT)
		->AddFlag(DT_RIGHT, XML_TEXTRENDER_ALIGN_RIGHT)
		->AddFlag(DT_CENTER, XML_TEXTRENDER_ALIGN_CENTER)
		->AddFlag(DT_BOTTOM, XML_TEXTRENDER_ALIGN_BOTTOM)
		->AddFlag(DT_VCENTER, XML_TEXTRENDER_ALIGN_VCENTER)
		->AddFlag(DT_SINGLELINE, XML_TEXTRENDER_ALIGN_SINGLELINE)
		->AddFlag(DT_WORDBREAK, XML_TEXTRENDER_ALIGN_MULTILINE)
		->AddFlag(DT_END_ELLIPSIS, XML_TEXTRENDER_ALIGN_END_ELLIPSIS)
		->AddFlag(DT_NOPREFIX, XML_TEXTRENDER_ALIGN_NO_PREFIX)
        ->SetDefault(DEFAULT_DRAWTEXT_FLAG);

	ps->AddEnum(XML_TEXTRENDER_EFFECT, *(long*)&m_eDrawTextEffect)
		->AddOption(TEXT_EFFECT_NONE, XML_TEXTRENDER_EFFECT_NONE)
		->AddOption(TEXT_EFFECT_HALO, XML_TEXTRENDER_EFFECT_HALO)
        ->AddOption(TEXT_EFFECT_ENDALPHAMASK, XML_TEXTRENDER_EFFECT_ENDALPHAMASK);

	ps->AddString(XML_TEXTRENDER_EFFECT_HALO_SHADOW_COLOR, this,
		memfun_cast<pfnStringSetter>(&TextRenderBase::LoadHaloColor),
		memfun_cast<pfnStringGetter>(&TextRenderBase::GetHaloColorId));
}

void  TextRenderBase::LoadHaloColor(LPCTSTR szColorId)
{
    _LoadColor(szColorId, m_pColorTextBkgnd); 
    m_wparamDrawText = 3; // Ĭ��ģ���뾶
}
LPCTSTR  TextRenderBase::GetHaloColorId()
{
    return _SaveColor(m_pColorTextBkgnd);
}

void  TextRenderBase::_LoadFont(LPCTSTR szFontId, IRenderFont*& pRenderFont)
{
    SAFE_RELEASE(pRenderFont);
    IFontRes*  pFontRes = GetSkinFontRes();
    if (!szFontId || !pFontRes)
        return;

	GRAPHICS_RENDER_LIBRARY_TYPE eType = GRAPHICS_RENDER_LIBRARY_TYPE_GDI;
	WindowBase* pWnd = m_pObject->GetWindowObject();
	if (pWnd)
	{
		WindowRender* pRender = pWnd->GetWindowRender();
		if (pRender)
			eType = pRender->GetGraphicsRenderType();
	}

	pFontRes->GetFont(szFontId, eType, &pRenderFont);
}

LPCTSTR  TextRenderBase::_SaveFont(IRenderFont*& pRenderFont)
{
    if (!pRenderFont)
        return NULL;

    IFontRes* pFontRes = GetSkinFontRes();
    if (pFontRes)
    {
        LPCTSTR szId = pFontRes->GetRenderFontId(pRenderFont);
        if (szId)
            return szId;
    }
    return NULL;
}

void  TextRenderBase::_LoadDefalutFont(IRenderFont** ppRenderFont)
{
	WindowBase* pWnd = m_pObject->GetWindowObject();
	if (!pWnd)
		return;

	IRenderFont* pFont = pWnd->GetWindowDefaultRenderFont();
    if (pFont)
    {
		if (ppRenderFont)
		{
			*ppRenderFont = pFont;
			(*ppRenderFont)->AddRef();
		}
    }
    else
    {
        IFontRes* pFontRes = GetSkinFontRes();

        // ������û�д��ڶ��󣬱�����һ�� popup listbox����menu�����ڻ�û�д�������ȡĬ������
		if (pFontRes && m_pObject)
		{
			GRAPHICS_RENDER_LIBRARY_TYPE eType = GRAPHICS_RENDER_LIBRARY_TYPE_GDI;
			WindowBase* pWnd = m_pObject->GetWindowObject();
			if (pWnd)
			{
				WindowRender* pRender = pWnd->GetWindowRender();
				if (pRender)
					eType = pRender->GetGraphicsRenderType();
			}

			pFontRes->GetDefaultFont(eType, ppRenderFont);
		}
    }    
}

void  TextRenderBase::_LoadColor(LPCTSTR szColorId, Color*& pColor)
{
    SAFE_RELEASE(pColor);
    if (!szColorId)
        return;

    IColorRes* pColorRes = GetSkinColorRes();
    if (!pColorRes)
        return;

    pColorRes->GetColor(szColorId, &pColor);
}
LPCTSTR  TextRenderBase::_SaveColor(Color*& pColor)
{
    if (!pColor)
        return NULL;

    IColorRes* pColorRes = GetSkinColorRes();
    if (pColorRes)
    {
        LPCTSTR szId = pColorRes->GetColorId(pColor);
        if (szId)
            return szId;
    }

    TCHAR* szBuffer = GetTempBuffer();
    pColor->ToHexString(szBuffer);
    return szBuffer;
}

void  TextRenderBase::OnGetDesiredSize(SIZE* pSize, GETDESIREDSIZEINFO* pInfo)
{
    *pSize = GetDesiredSize(pInfo->szText, pInfo->nLimitWidth);
}

SIZE TextRenderBase::GetDesiredSize(LPCTSTR szText, int nLimitWidth)
{
    SIZE s = {0,0};
    IRenderFont* pFont = m_pITextRenderBase->GetRenderFont();
    if (pFont)
        s = pFont->MeasureString(szText, nLimitWidth);

    return s;
}

IColorRes*  TextRenderBase::GetSkinColorRes()
{
	if (m_pObject)
	{
		SkinRes* pSkinRes = m_pObject->GetSkinRes();
		if (pSkinRes)
			return pSkinRes->GetColorRes().GetIColorRes();
	}
	else
	{
		if (m_pUIApplication)
			return m_pUIApplication->GetActiveSkinColorRes()->GetIColorRes();
	}

	return NULL;
}

IFontRes*  TextRenderBase::GetSkinFontRes()
{
	if (m_pObject)
	{
		SkinRes* pSkinRes = m_pObject->GetSkinRes();
		if (pSkinRes)
			return pSkinRes->GetFontRes().GetIFontRes();
	}
	else
	{
		if (m_pUIApplication)
			return m_pUIApplication->GetActiveSkinFontRes()->GetIFontRes();
	}

	return NULL;
}

bool  TextRenderBase::IsThemeRender() 
{ 
    return  m_nTextRenderType > TEXTRENDER_TYPE_THEME_FIRST && 
            m_nTextRenderType < TEXTRENDER_TYPE_THEME_LAST; 
}

void  TextRenderBase::CheckSkinTextureChanged()
{
    if (m_nTextRenderType > TEXTRENDER_TYPE_CONTRAST_FIRST && m_nTextRenderType < TEXTRENDER_TYPE_CONTRAST_LAST)
    {
        UISendMessage(static_cast<IMessage*>(m_pITextRenderBase), UI_MSG_SKINTEXTURECHANGED);
    }
}


SimpleTextRender::SimpleTextRender(ISimpleTextRender* p):TextRenderBase(p)
{
    m_pISimpleTextRender = p;

	m_pColorText = NULL;
	m_pRenderFont = NULL;
}
SimpleTextRender::~SimpleTextRender()
{
	SAFE_RELEASE(m_pColorText);
	SAFE_RELEASE(m_pRenderFont);
}

// �������ָ��Ϊ�գ���ȡobject������������塣
void SimpleTextRender::OnSerialize(SERIALIZEDATA* pData)
{
	{
		AttributeSerializer s(pData, TEXT("SimpleTextRender"));
        __super::Serialize(&s);

		s.AddString(XML_TEXTRENDER_FONT, this,
			memfun_cast<pfnStringSetter>(&SimpleTextRender::LoadFont),
			memfun_cast<pfnStringGetter>(&SimpleTextRender::GetFontId));
		s.AddString(XML_TEXTRENDER_COLOR, this,
			memfun_cast<pfnStringSetter>(&SimpleTextRender::LoadColor),
			memfun_cast<pfnStringGetter>(&SimpleTextRender::GetColorId));
	}

	if (!m_pRenderFont && pData->IsLoad())
	{
		_LoadDefalutFont(&m_pRenderFont);
	}
}

void  SimpleTextRender::LoadFont(LPCTSTR szFontId)
{
    _LoadFont(szFontId, m_pRenderFont);
}
LPCTSTR  SimpleTextRender::GetFontId()
{
    return _SaveFont(m_pRenderFont);
}

void  SimpleTextRender::LoadColor(LPCTSTR szColorId)
{
    _LoadColor(szColorId, m_pColorText);
}
LPCTSTR  SimpleTextRender::GetColorId()
{
    return _SaveColor(m_pColorText);
}

void SimpleTextRender::SetRenderFont(IRenderFont* pFont)
{
	SAFE_RELEASE(m_pRenderFont);

	m_pRenderFont = pFont;

	if (pFont)
		pFont->AddRef();
}

void SimpleTextRender::SetColor(Color*  pColText)
{
	SAFE_RELEASE(m_pColorText);
    if (pColText)
        m_pColorText = Color::CreateInstance(pColText->m_col);
}

void SimpleTextRender::DrawState(TEXTRENDERBASE_DRAWSTATE* pDrawStruct)
{
    IRenderTarget* pRenderTarget = pDrawStruct->ds_renderbase.pRenderTarget;
	if (NULL == pRenderTarget || NULL == pDrawStruct->szText)
		return ;

	if (m_pRenderFont && _tcslen(pDrawStruct->szText) > 0)
	{
        DRAWTEXTPARAM  param;

        if (m_pColorText)
			param.color = m_pColorText->m_col;
        param.nFormatFlag = pDrawStruct->nDrawTextFlag==-1 ? m_nDrawTextFlag:pDrawStruct->nDrawTextFlag;
        param.prc = &pDrawStruct->ds_renderbase.rc;
        param.szText = pDrawStruct->szText;

        param.nEffectFlag = m_eDrawTextEffect;
        if (m_pColorTextBkgnd)
            param.bkcolor = *m_pColorTextBkgnd;
        param.wParam = m_wparamDrawText;
        param.lParam = m_lparamDrawText;
		pRenderTarget->DrawString(m_pRenderFont, &param);
	}
}


//////////////////////////////////////////////////////////////////////////


ContrastColorTextRender::ContrastColorTextRender(IContrastColorTextRender* p):TextRenderBase(p)
{
    m_pIContrastColorTextRender = p;

    m_pColorText = NULL;
    m_pRenderFont = NULL;
    m_bNeedCalcColor = false;
}
ContrastColorTextRender::~ContrastColorTextRender()
{
    SAFE_RELEASE(m_pRenderFont);
    SAFE_RELEASE(m_pColorText);
}

// �������ָ��Ϊ�գ���ȡobject������������塣
void ContrastColorTextRender::OnSerialize(SERIALIZEDATA* pData)
{
    {
        AttributeSerializer s(pData, TEXT("ContrastColorTextRender"));
        __super::Serialize(&s);

        s.AddString(XML_TEXTRENDER_FONT, this,
            memfun_cast<pfnStringSetter>(&ContrastColorTextRender::LoadFont),
            memfun_cast<pfnStringGetter>(&SimpleTextRender::GetFontId));
        s.AddString(XML_TEXTRENDER_COLOR, this,
            memfun_cast<pfnStringSetter>(&ContrastColorTextRender::LoadColor),
            memfun_cast<pfnStringGetter>(&ContrastColorTextRender::GetColorId));
    }

    if (pData->IsLoad())
    {
        if (!m_pRenderFont)
        {
            _LoadDefalutFont(&m_pRenderFont);
        }

        // ���û�����ñ���ɫ����Ĭ������һ��
        if (m_eDrawTextEffect == TEXT_EFFECT_HALO)
        {
            if (m_pColorText && !m_pColorTextBkgnd)
            {
                SAFE_RELEASE(m_pColorTextBkgnd);
                if (m_pColorText->m_col == 0xFFFFFFFF)
                {
                    m_pColorTextBkgnd = Color::CreateInstance(0xFF000000);
                }
                else
                {
                    m_pColorTextBkgnd = Color::CreateInstance(0xFFFFFFFF);
                }
            }
            m_wparamDrawText = 3; // Ĭ��ģ���뾶
        }
    }
}

void ContrastColorTextRender::SetRenderFont(IRenderFont* pFont)
{
    SAFE_RELEASE(m_pRenderFont);

    m_pRenderFont = pFont;

    if (pFont)
        pFont->AddRef();
}

void ContrastColorTextRender::DrawState(TEXTRENDERBASE_DRAWSTATE* pDrawStruct)
{
	UIASSERT(0);
#if 0
    if (NULL == m_pObject)
        return;

    if (NULL == m_pColorText)
    {
        DWORD dwColor = m_pObject->CalcContrastTextColor();
        m_pColorText = Color::CreateInstance(dwColor);

        if (m_eDrawTextEffect == TEXT_EFFECT_HALO)
        {
            SAFE_RELEASE(m_pColorTextBkgnd);
            if (dwColor == 0xFFFFFFFF)
            {
                m_pColorTextBkgnd = Color::CreateInstance(0xFF000000);
            }
            else
            {
                m_pColorTextBkgnd = Color::CreateInstance(0xFFFFFFFF);
            }
        }
    }

    IRenderTarget* pRenderTarget = pDrawStruct->ds_renderbase.pRenderTarget;
    if (NULL == pRenderTarget || NULL == pDrawStruct->szText)
        return ;

    if (m_pRenderFont && _tcslen(pDrawStruct->szText) > 0)
    {
        DRAWTEXTPARAM  param;
        param.szText = pDrawStruct->szText;
        param.prc = &pDrawStruct->ds_renderbase.rc;
        param.color = m_pColorText->m_col;
        param.nFormatFlag = pDrawStruct->nDrawTextFlag==-1 ? m_nDrawTextFlag:pDrawStruct->nDrawTextFlag;

        param.nEffectFlag = m_eDrawTextEffect;
        if (m_pColorTextBkgnd)
            param.bkcolor = *m_pColorTextBkgnd;
        param.wParam = m_wparamDrawText;
        param.lParam = m_lparamDrawText;
        pRenderTarget->DrawString(m_pRenderFont, &param);
    }
#endif
}

LRESULT  ContrastColorTextRender::OnSkinTextureChanged(UINT, WPARAM, LPARAM)
{
    SAFE_RELEASE(m_pColorText);
    return 0;
}

//////////////////////////////////////////////////////////////////////////

ContrastColorListTextRender::ContrastColorListTextRender(
		IContrastColorListTextRender* p):TextRenderBase(p)
{
    m_nCount = 0;
    m_pRenderFont = NULL;
    m_pIContrastColorListTextRender = p;
}
ContrastColorListTextRender::~ContrastColorListTextRender()
{
    this->Clear();
    SAFE_RELEASE(m_pRenderFont);
}
void  ContrastColorListTextRender::Clear()
{
    for (int i = 0; i < m_nCount; i++)
    {
        SAFE_RELEASE(m_vTextColor[i]);
    }

    m_vTextColor.clear();
    m_nCount = 0;
}


LRESULT  ContrastColorListTextRender::OnSkinTextureChanged(UINT, WPARAM, LPARAM)
{
    for (int i = 0; i < m_nCount; i++)
    {
        SAFE_RELEASE(m_vTextColor[i]);
        m_vTextColor[i] = NULL;
    }

    return 0;
}
void ContrastColorListTextRender::OnSerialize(SERIALIZEDATA* pData)
{
    {
        AttributeSerializer s(pData, TEXT("ContrastColorListTextRender"));
        __super::Serialize(&s);

        s.AddLong(XML_TEXTRENDER_COLORLIST_COUNT, this,
            memfun_cast<pfnLongSetter>(&ContrastColorListTextRender::SetCount),
            memfun_cast<pfnLongGetter>(&ContrastColorListTextRender::GetCount));

        s.AddString(XML_TEXTRENDER_FONT, this,
            memfun_cast<pfnStringSetter>(&ContrastColorListTextRender::LoadFont),
            memfun_cast<pfnStringGetter>(&ContrastColorListTextRender::GetFontId));
    }

    if (pData->IsLoad() && !m_pRenderFont)
    {
        _LoadDefalutFont(&m_pRenderFont);
    }
}

void ContrastColorListTextRender::DrawState(TEXTRENDERBASE_DRAWSTATE* pDrawStruct)
{
	UIASSERT(0);
#if 0
    if (0 == m_nCount || NULL == m_pObject)
        return;

    int nRealState = LOWORD(pDrawStruct->ds_renderbase.nState);
    if (nRealState >= m_nCount)
        nRealState = 0;


    if (NULL  == m_vTextColor[nRealState])
        m_vTextColor[nRealState] = Color::CreateInstance(m_pObject->CalcContrastTextColor());

    DRAWTEXTPARAM  param;

    param.prc = &pDrawStruct->ds_renderbase.rc;
    param.nFormatFlag = pDrawStruct->nDrawTextFlag==-1 ? m_nDrawTextFlag:pDrawStruct->nDrawTextFlag;
    if (m_vTextColor[nRealState])
        param.color = m_vTextColor[nRealState]->m_col;
    param.szText = pDrawStruct->szText;
    pDrawStruct->ds_renderbase.pRenderTarget->DrawString(m_pRenderFont, &param);
#endif
}

IRenderFont* ContrastColorListTextRender::GetRenderFont()
{
    return m_pRenderFont;
}

void ContrastColorListTextRender::SetRenderFont(IRenderFont* pFont)
{
    SAFE_RELEASE(m_pRenderFont);
    m_pRenderFont = pFont;

    if (pFont)
        pFont->AddRef();
}

void ContrastColorListTextRender::SetCount(long nCount)
{
    this->Clear();
    m_nCount = nCount;

    for (int i = 0; i < m_nCount; i++)
    {
        m_vTextColor.push_back(NULL);
    }
}
long  ContrastColorListTextRender::GetCount()
{
    return (long)m_vTextColor.size();
}

//////////////////////////////////////////////////////////////////////////

ColorListTextRender::ColorListTextRender(IColorListTextRender* p):TextRenderBase(p)
{
	m_nCount = 0;
	m_pRenderFont = NULL;
    m_pIColorListTextRender = p;
}
ColorListTextRender::~ColorListTextRender()
{
	this->Clear();
	SAFE_RELEASE(m_pRenderFont);
}
void ColorListTextRender::Clear()
{
	for (int i = 0; i < m_nCount; i++)
	{
		SAFE_RELEASE(m_vTextColor[i]);
	}
	m_vTextColor.clear();
	m_nCount = 0;
}


void ColorListTextRender::OnSerialize(SERIALIZEDATA* pData)
{
    {
        AttributeSerializer s(pData, TEXT("ColorListTextRender"));
        __super::Serialize(&s);

        s.AddLong(XML_TEXTRENDER_COLORLIST_COUNT, this,
            memfun_cast<pfnLongSetter>(&ColorListTextRender::SetCount),
            memfun_cast<pfnLongGetter>(&ColorListTextRender::GetCount));

        s.AddString(XML_TEXTRENDER_COLOR, this,
            memfun_cast<pfnStringSetter>(&ColorListTextRender::LoadColor),
            memfun_cast<pfnStringGetter>(&ColorListTextRender::GetColor));

        s.AddString(XML_TEXTRENDER_FONT, this,
            memfun_cast<pfnStringSetter>(&ColorListTextRender::LoadFont),
            memfun_cast<pfnStringGetter>(&ColorListTextRender::GetFontId));
    }

	if (pData->IsLoad() && !m_pRenderFont)
	{
		_LoadDefalutFont(&m_pRenderFont);
	}
}

void  ColorListTextRender::LoadColor(LPCTSTR szText)
{
    if (!szText)
        return ;
    
    IColorRes* pColorRes = GetSkinColorRes();

    vector<String> vColors;
    UI_Split(szText, XML_MULTI_SEPARATOR, vColors);
    int nCount = (int)vColors.size();

    if (0 == m_nCount)
    {
        this->SetCount(nCount); //  ���δ��ʾָ��count�����Զ�ȡ����Ĵ�С
    }

    for(int i = 0; i < m_nCount && i < nCount; i++ )
    {
        if (!vColors[i].empty())
        {
            SAFE_RELEASE(m_vTextColor[i]);
            pColorRes->GetColor((BSTR)vColors[i].c_str(), &m_vTextColor[i]);
        }
    }
}
LPCTSTR  ColorListTextRender::GetColor()
{
    String&  strBuffer = GetTempBufferString();
    for (int i = 0; i < m_nCount; i++)
    {
        if (i > 0)
            strBuffer.push_back(XML_MULTI_SEPARATOR);

        LPCTSTR szTemp = _SaveColor(m_vTextColor[i]);
        if (szTemp)
            strBuffer.append(szTemp);
    }

    return strBuffer.c_str();
}

void ColorListTextRender::DrawState(TEXTRENDERBASE_DRAWSTATE* pDrawStruct)
{
    if (0 == m_nCount)
        return;

	int nRealState = LOWORD(pDrawStruct->ds_renderbase.nState);
	if (nRealState >= m_nCount)
		nRealState = 0;

    DRAWTEXTPARAM  param;
	if (m_vTextColor[nRealState])
		param.color = m_vTextColor[nRealState]->m_col;
	param.prc = &pDrawStruct->ds_renderbase.rc;
	param.nFormatFlag = pDrawStruct->nDrawTextFlag==-1 ? m_nDrawTextFlag:pDrawStruct->nDrawTextFlag;
    param.szText = pDrawStruct->szText;
	pDrawStruct->ds_renderbase.pRenderTarget->DrawString(m_pRenderFont, &param);
}
IRenderFont* ColorListTextRender::GetRenderFont()
{
	return m_pRenderFont;
}

void ColorListTextRender::SetRenderFont(IRenderFont* pFont)
{
	SAFE_RELEASE(m_pRenderFont);
	m_pRenderFont = pFont;

	if (pFont)
		pFont->AddRef();
}

void ColorListTextRender::SetCount(long nCount)
{
	this->Clear();
	m_nCount = nCount;

	for (int i = 0; i < m_nCount; i++)
	{
		m_vTextColor.push_back(NULL);
	}
}
long  ColorListTextRender::GetCount()
{
    return m_vTextColor.size();
}

void ColorListTextRender::SetColor(int nIndex, COLORREF col)
{
	nIndex = LOWORD(nIndex);
	if (nIndex >= m_nCount)
		return;

	if (m_vTextColor[nIndex])
	{
		SAFE_RELEASE(m_vTextColor[nIndex]);
	}
    m_vTextColor[nIndex] = Color::CreateInstance(col);
}

//////////////////////////////////////////////////////////////////////////
FontColorListTextRender::FontColorListTextRender(
		IFontColorListTextRender* p):TextRenderBase(p)
{
	m_nCount = 0;
    m_pIFontColorListTextRender = p;
}
FontColorListTextRender::~FontColorListTextRender()
{
	this->Clear();
}
void FontColorListTextRender::Clear()
{
	for (int i = 0; i < m_nCount; i++)
	{
		SAFE_RELEASE(m_vTextColor[i]);
		SAFE_RELEASE(m_vTextFont[i]);
	}
	m_vTextFont.clear();
	m_vTextColor.clear();
	m_nCount = 0;
}

void FontColorListTextRender::OnSerialize(SERIALIZEDATA* pData)
{
    {
        AttributeSerializer s(pData, TEXT("FontColorListTextRender"));
        __super::Serialize(&s);

        s.AddLong(XML_TEXTRENDER_FONTCOLORLIST_COUNT, this,
            memfun_cast<pfnLongSetter>(&FontColorListTextRender::SetCount),
            memfun_cast<pfnLongGetter>(&FontColorListTextRender::GetCount));

        s.AddString(XML_TEXTRENDER_COLOR, this,
            memfun_cast<pfnStringSetter>(&FontColorListTextRender::LoadColor),
            memfun_cast<pfnStringGetter>(&FontColorListTextRender::GetColor));

        s.AddString(XML_TEXTRENDER_FONT, this,
            memfun_cast<pfnStringSetter>(&FontColorListTextRender::LoadFont),
            memfun_cast<pfnStringGetter>(&FontColorListTextRender::GetFont));
    }
}

void  FontColorListTextRender::LoadColor(LPCTSTR szText)
{
    if (!szText)
        return;

    vector<String> vColors;
    UI_Split(szText, XML_MULTI_SEPARATOR, vColors);
    int nCount = (int)vColors.size();

    if (0 == m_nCount)
        this->SetCount(nCount); //  ���δ��ʾָ��count�����Զ�ȡ����Ĵ�С

    IColorRes* pColorRes = GetSkinColorRes();
    if (!pColorRes)
        return;

    for(int i = 0; i < m_nCount && i < nCount; i++ )
    {
        if(! vColors[i].empty() )
        {
            pColorRes->GetColor(vColors[i].c_str(), &m_vTextColor[i]);
        }
    }
}
LPCTSTR  FontColorListTextRender::GetColor()
{
    String&  strBuffer = GetTempBufferString();
    for (int i = 0; i < m_nCount; i++)
    {
        if (i > 0)
            strBuffer.push_back(XML_MULTI_SEPARATOR);

        LPCTSTR szTemp = _SaveColor(m_vTextColor[i]);
        if (szTemp)
            strBuffer.append(szTemp);
    }

    return strBuffer.c_str();
}

void  FontColorListTextRender::LoadFont(LPCTSTR szText)
{
    IFontRes* pFontRes = GetSkinFontRes();

    if (szText && m_pObject)
    {
        vector<String> vFont;
        UI_Split(szText, XML_MULTI_SEPARATOR, vFont);
        int nCount = (int)vFont.size();

        if (0 == m_nCount)
            this->SetCount(nCount); //  ���δ��ʾָ��count�����Զ�ȡ����Ĵ�С


		GRAPHICS_RENDER_LIBRARY_TYPE eType = GRAPHICS_RENDER_LIBRARY_TYPE_GDI;
		WindowBase* pWnd = m_pObject->GetWindowObject();
		if (pWnd)
		{
			WindowRender* pRender = pWnd->GetWindowRender();
			if (pRender)
				eType = pRender->GetGraphicsRenderType();
		}

        for(int i = 0; i < m_nCount && i < nCount; i++ )
        {
            if(! vFont[i].empty() )
            {
				pFontRes->GetFont((BSTR)vFont[i].c_str(), eType, &m_vTextFont[i]);
            }
        }
    }

	WindowBase* pWnd = m_pObject->GetWindowObject();

    for(int i = 0; i < m_nCount; i++ )
    {
        if(NULL == m_vTextFont[i])
        {
			if (pWnd)
				m_vTextFont[i] = pWnd->GetWindowDefaultRenderFont();
            if (m_vTextFont[i])
                m_vTextFont[i]->AddRef();
        }
    }
}

LPCTSTR  FontColorListTextRender::GetFont()
{
    String&  strBuffer = GetTempBufferString();
    for (int i = 0; i < m_nCount; i++)
    {
        if (i > 0)
            strBuffer.push_back(XML_MULTI_SEPARATOR);

        LPCTSTR szTemp = _SaveFont(m_vTextFont[i]);
        if (szTemp)
            strBuffer.append(szTemp);
    }

    return strBuffer.c_str();
}

void FontColorListTextRender::DrawState(TEXTRENDERBASE_DRAWSTATE* pDrawStruct)
{
    if (0 == m_nCount)
        return;

	int nRealState = LOWORD(pDrawStruct->ds_renderbase.nState);
	if (nRealState >= m_nCount)
		nRealState = 0;

    DRAWTEXTPARAM  param;
	if (m_vTextColor[nRealState])
		param.color = *m_vTextColor[nRealState];

	param.nFormatFlag = pDrawStruct->nDrawTextFlag==-1 ? m_nDrawTextFlag:pDrawStruct->nDrawTextFlag;
    param.szText = pDrawStruct->szText;
    param.prc = &(pDrawStruct->ds_renderbase.rc);
	pDrawStruct->ds_renderbase.pRenderTarget->DrawString(m_vTextFont[nRealState], &param);
}
IRenderFont* FontColorListTextRender::GetRenderFont()
{
	if (m_vTextFont.size() > 0)
		return m_vTextFont[0];
	
	return NULL;
}

void FontColorListTextRender::SetRenderFont(IRenderFont* pRenderFont)
{
	if (m_vTextFont.size() > 0)
	{
		SAFE_RELEASE(m_vTextFont[0]);
		m_vTextFont[0] = pRenderFont;

		if (pRenderFont)
			pRenderFont->AddRef();
	}
}

void FontColorListTextRender::SetCount(long nCount)
{
	this->Clear();
	m_nCount = nCount;

	for (int i = 0; i < m_nCount; i++)
	{
		m_vTextColor.push_back(NULL);
		m_vTextFont.push_back(NULL);
	}
}

long  FontColorListTextRender::GetCount()
{
    return m_nCount;
}

void  FontColorListTextRender::SetColor(int nIndex, DWORD col)
{
	nIndex = LOWORD(nIndex);
	if (nIndex >= m_nCount)
		return;

	if (m_vTextColor[nIndex])
	{
		SAFE_RELEASE(m_vTextColor[nIndex]);
	}
    m_vTextColor[nIndex] = Color::CreateInstance(col);
}
void FontColorListTextRender::SetFont(int nIndex, IRenderFont* pFont)
{
	if (nIndex >= m_nCount)
		return;

	SAFE_RELEASE(m_vTextFont[nIndex]);

	m_vTextFont[nIndex] = pFont;

	if (pFont)
		pFont->AddRef();
}
//////////////////////////////////////////////////////////////////////////
#if 0
ThemeTextRenderBase::ThemeTextRenderBase(IThemeTextRenderBase* p):TextRenderBase(p)
{
    m_pIThemeTextRenderBase = p;
	m_hTheme = NULL;
    m_bNoTheme = false;
}
ThemeTextRenderBase::~ThemeTextRenderBase()
{
	if (m_hTheme)
	{
		::CloseThemeData( m_hTheme );
		m_hTheme = NULL;
	}
}
void ThemeTextRenderBase::OnInit()
{
	this->CreateTheme();
}
void ThemeTextRenderBase::OnThemeChanged()
{
	this->CreateTheme();
}
void ThemeTextRenderBase::CreateTheme()
{
    if (m_hTheme)
    {
        ::CloseThemeData(m_hTheme);
        m_hTheme = NULL;
    }
    if (!m_bNoTheme)
    {
        LPCTSTR pThemeName = m_pIThemeTextRenderBase->GetThemeName();
        if (pThemeName)
        {
            m_hTheme = ::OpenThemeData(m_pObject->GetHWND(), pThemeName);
        }
    }
}

void  ThemeTextRenderBase::Serialize(AttributeSerializerWrap* ps)
{
	AttributeSerializer* psImpl = ps->GetImpl();
	__super::Serialize(psImpl);
	
	psImpl->AddBool(XML_RENDER_THEME_DISABLE, this,
		memfun_cast<pfnBoolSetter>(&ThemeTextRenderBase::SetNoTheme),
		memfun_cast<pfnBoolGetter>(&ThemeTextRenderBase::GetNoTheme));
}

#endif