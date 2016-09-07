#include "stdafx.h"
#include "imagerender.h"
#include "Src\Base\Object\object.h"
#include "Src\Base\Attribute\attribute.h"
#include "Src\Base\Attribute\bool_attribute.h"
#include "Src\Base\Attribute\string_attribute.h"
#include "Src\Base\Attribute\long_attribute.h"
#include "Src\Base\Attribute\enum_attribute.h"
#include "Src\Base\Attribute\rect_attribute.h"
#include "Src\Base\Attribute\9region_attribute.h"
#include "Inc\Interface\renderlibrary.h"
#include "Inc\Util\struct.h"


ImageRender::ImageRender(IImageRender* p) : RenderBase(p)
{
    m_pIImageRender = p;

	m_pBitmap = NULL;
	m_pColorBk = NULL;
	m_nImageDrawType = DRAW_BITMAP_BITBLT;
    m_nAlpha = 255;
    SetRectEmpty(&m_rcSrc);
    m_eBkColorFillType = BKCOLOR_FILL_ALL;
}
ImageRender::~ImageRender()
{
	SAFE_RELEASE(m_pBitmap);
	SAFE_RELEASE(m_pColorBk);
}

void  ImageRender::SetAlpha(int nAlpha)
{
    m_nAlpha = nAlpha;
}
int   ImageRender::GetAlpha()
{
    return m_nAlpha;
}

Color  ImageRender::GetColor()
{
    if (NULL == m_pColorBk)
        return Color(0);
    else
        return *m_pColorBk;
}

void  ImageRender::SetImageDrawType(int n)
{
    m_nImageDrawType = n;
}
int   ImageRender::GetImageDrawType()
{
    return m_nImageDrawType;
}

void  ImageRender::OnSerialize(SERIALIZEDATA* pData)
{
	AttributeSerializer s(pData, TEXT("ImageRender"));
	s.AddString(XML_RENDER_IMAGE, this, 
		memfun_cast<pfnStringSetter>(&ImageRender::LoadBitmap),
		memfun_cast<pfnStringGetter>(&ImageRender::GetBitmapId));

	// ������ɫ
	s.AddString(XML_RENDER_COLOR, this, 
		memfun_cast<pfnStringSetter>(&ImageRender::LoadColor),
		memfun_cast<pfnStringGetter>(&ImageRender::GetColorId));

	// ��������
	s.Add9Region(XML_RENDER_IMAGE9REGION, m_Region);

	// ����͸����
	s.AddLong(XML_RENDER_IMAGE_ALPHA, m_nAlpha)->SetDefault(255);

	// Դ����
	s.AddRect(XML_RENDER_IMAGE_SRC_REGION, m_rcSrc);

	// ��������
	s.AddEnum(XML_RENDER_IMAGE_DRAWTYPE, m_nImageDrawType)
		->AddOption(DRAW_BITMAP_BITBLT, XML_RENDER_IMAGE_DRAWTYPE_BITBLT)
		->AddOption(DRAW_BITMAP_TILE, XML_RENDER_IMAGE_DRAWTYPE_TILE)
		->AddOption(DRAW_BITMAP_STRETCH, XML_RENDER_IMAGE_DRAWTYPE_STRETCH)
		->AddOption(DRAW_BITMAP_ADAPT, XML_RENDER_IMAGE_DRAWTYPE_ADAPT)
		->AddOption(DRAW_BITMAP_CENTER, XML_RENDER_IMAGE_DRAWTYPE_CENTER)
		->AddOption(DRAW_BITMAP_STRETCH_BORDER, XML_RENDER_IMAGE_DRAWTYPE_STRETCH_BORDER)
		->AddOption(DRAW_BITMAP_BITBLT_RIGHTTOP, XML_RENDER_IMAGE_DRAWTYPE_BITBLT_RIGHTTOP)
		->AddOption(DRAW_BITMAP_BITBLT_LEFTBOTTOM, XML_RENDER_IMAGE_DRAWTYPE_BITBLT_LEFTBOTTOM)
		->AddOption(DRAW_BITMAP_BITBLT_RIGHTBOTTOM, XML_RENDER_IMAGE_DRAWTYPE_BITBLT_RIGHTBOTTOM)
        ->SetDefault(DRAW_BITMAP_BITBLT);
} 

void  ImageRender::SetRenderBitmap(IRenderBitmap* pBitmap)
{
	SAFE_RELEASE(m_pBitmap);
	m_pBitmap = pBitmap;
	if (m_pBitmap)
		m_pBitmap->AddRef();
}

void  ImageRender::SetColor(Color c)
{
    SAFE_RELEASE(m_pColorBk);
    m_pColorBk = Color::CreateInstance(c);
}

void  ImageRender::DrawState(RENDERBASE_DRAWSTATE* pDrawStruct)
{
    RECT* prc = &pDrawStruct->rc;

	if (m_pColorBk && m_eBkColorFillType == BKCOLOR_FILL_ALL)
    {
        Color c = m_pColorBk->m_col;
        c.a = (byte)m_nAlpha;

        pDrawStruct->pRenderTarget->DrawRect(prc, &c);
    }

    CRect rcRealDraw(0, 0, 0, 0);
	if (m_pBitmap)
	{
		DRAWBITMAPPARAM param;
		param.nFlag = m_nImageDrawType;
		param.xDest = prc->left;
		param.yDest = prc->top;
		param.wDest = prc->right-prc->left;
		param.hDest = prc->bottom-prc->top;
        if (!IsRectEmpty(&m_rcSrc))
        {
		    param.xSrc  = m_rcSrc.left;
		    param.ySrc  = m_rcSrc.top;
			param.wSrc  = m_rcSrc.right-m_rcSrc.left;
		    param.hSrc  = m_rcSrc.bottom-m_rcSrc.top;
        }
        else if (m_pBitmap)
        {
            param.xSrc  = 0;
            param.ySrc  = 0;
            param.wSrc  = m_pBitmap->GetWidth();
            param.hSrc  = m_pBitmap->GetHeight();
        }
		if (!m_Region.IsAll_0())
			param.pRegion = &m_Region;
        param.nAlpha  = (byte)m_nAlpha;

		if (pDrawStruct->nState & RENDER_STATE_DISABLE)
		{
			param.nFlag |= DRAW_BITMAP_DISABLE;
		}

        if (m_pColorBk && m_eBkColorFillType == BKCOLOR_FILL_EMPTY)
        {
            param.prcRealDraw = &rcRealDraw;
        }
		pDrawStruct->pRenderTarget->DrawBitmap(m_pBitmap, &param);
	}

    if (m_pColorBk && m_eBkColorFillType == BKCOLOR_FILL_EMPTY)
    {
        Color c = m_pColorBk->m_col;
        c.a = (byte)m_nAlpha;

        // Top
        {
            CRect rc(prc->left, prc->top, prc->right, rcRealDraw.top);
            if (rc.Width() > 0 && rc.Height() > 0)
            {
                pDrawStruct->pRenderTarget->DrawRect(&rc, &c);
            }
        }
        // Left
        {
            CRect rc(prc->left, rcRealDraw.top, rcRealDraw.left, rcRealDraw.bottom);
            if (rc.Width() > 0 && rc.Height() > 0)
            {
                pDrawStruct->pRenderTarget->DrawRect(&rc, &c);
            }
        }
        // Right
        {
            CRect rc(rcRealDraw.right, rcRealDraw.top, prc->right, rcRealDraw.bottom);
            if (rc.Width() > 0 && rc.Height() > 0)
            {
                pDrawStruct->pRenderTarget->DrawRect(&rc, &c);
            }
        }
        // Bottom
        {
            CRect rc(prc->left, rcRealDraw.bottom, prc->right, prc->bottom);
            if (rc.Width() > 0 && rc.Height() > 0)
            {
                pDrawStruct->pRenderTarget->DrawRect(&rc, &c);
            }
        }
    }
}
void  ImageRender::GetDesiredSize(SIZE* pSize)
{
	pSize->cx = pSize->cy = 0;
	if (NULL == m_pBitmap)
		return;

	pSize->cx = m_pBitmap->GetWidth();
	pSize->cy = m_pBitmap->GetHeight();
}

//////////////////////////////////////////////////////////////////////////

ImageListItemRender::ImageListItemRender(IImageListItemRender* p) :ImageRender(p)
{
    m_pIImageListItemRender = p;
	m_nImagelistIndex = -1;
	m_pImageList = NULL;
}
ImageListItemRender::~ImageListItemRender()
{
	m_nImagelistIndex = 0;
	m_pImageList = NULL;
}

void  ImageListItemRender::OnSerialize(SERIALIZEDATA* pData)
{
	__super::OnSerialize(pData);

	{
		AttributeSerializer s(pData, TEXT("ImageListItemRender"));
		s.AddLong(XML_RENDER_IMAGELISTITEM_INDEX, m_nImagelistIndex);
	}

	if (pData->IsLoad())
	{
		if (m_pBitmap)
		{
			if (m_pBitmap->GetImageType() != IMAGE_ITEM_TYPE_IMAGE_LIST)
				SAFE_RELEASE(m_pBitmap);
		}
		if (m_pBitmap)
		{
			m_pImageList = static_cast<IImageListRenderBitmap*>(m_pBitmap);

			POINT pt = {0,0};
			m_pImageList->GetIndexPos(m_nImagelistIndex, &pt);
			m_rcSrc.left = pt.x;
			m_rcSrc.top = pt.y;

			SIZE s;
			this->GetDesiredSize(&s);
			m_rcSrc.right = m_rcSrc.left + s.cx;
			m_rcSrc.bottom = m_rcSrc.top + s.cy;
		}
	}
}

void  ImageListItemRender::GetDesiredSize(SIZE* pSize)
{
	pSize->cx = 0;
    pSize->cy = 0;
	if (NULL == m_pImageList )
		return;

	pSize->cx = m_pImageList->GetItemWidth();
	pSize->cy = m_pImageList->GetItemHeight();
}

void  ImageListItemRender::DrawState(RENDERBASE_DRAWSTATE* pDrawStruct)
{
	if (DRAW_BITMAP_TILE == m_nImageDrawType)
	{
		UI_LOG_WARN(_T("image list item donot support tile draw"));
		UIASSERT(0 && _T("TODO:"));
		return;
	}

    if (-1 == m_nImagelistIndex && m_pImageList)
    {
        POINT pt = {0, 0};
        SIZE  s = {0, 0};

        if (false == m_pImageList->GetIndexPos(pDrawStruct->nState, &pt))
            return;
        this->GetDesiredSize(&s);

		::SetRect(&m_rcSrc, pt.x, pt.y, pt.x+s.cx, pt.y+s.cy);
    }

	SetMsgHandled(FALSE);
}


//////////////////////////////////////////////////////////////////////////////////////////
//                                                                                      //
//                               ImageListRender                                        //
//                                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////

ImageListRender::ImageListRender(IImageListRender* p):RenderBase(p)
{
    m_pIImageListRender = p;

	m_pImageList = NULL;
	m_nImageDrawType = DRAW_BITMAP_BITBLT;

	m_nPrevState = RENDER_STATE_NORMAL;
	m_nCurrentAlpha = 255;
	m_bIsAnimate = false;
	m_bUseAlphaAnimate = false;
}
ImageListRender::~ImageListRender( )
{
	SAFE_RELEASE(m_pImageList);
	DestroyAnimate();
}

void  ImageListRender::OnSerialize(SERIALIZEDATA* pData)
{
	{
		AttributeSerializer s(pData, TEXT("ImageListItemRender"));
		s.AddBool(XML_RENDER_IMAGELIST_ALPHA_ANIMATE, m_bUseAlphaAnimate);

		s.Add9Region(XML_RENDER_IMAGE9REGION, m_9Region);

		s.AddEnum(XML_RENDER_IMAGE_DRAWTYPE, m_nImageDrawType)
			->AddOption(DRAW_BITMAP_BITBLT, XML_RENDER_IMAGE_DRAWTYPE_BITBLT)
			->AddOption(DRAW_BITMAP_TILE, XML_RENDER_IMAGE_DRAWTYPE_TILE)
			->AddOption(DRAW_BITMAP_STRETCH, XML_RENDER_IMAGE_DRAWTYPE_STRETCH)
			->AddOption(DRAW_BITMAP_ADAPT, XML_RENDER_IMAGE_DRAWTYPE_ADAPT)
			->AddOption(DRAW_BITMAP_CENTER, XML_RENDER_IMAGE_DRAWTYPE_CENTER)
			->AddOption(DRAW_BITMAP_STRETCH_BORDER, XML_RENDER_IMAGE_DRAWTYPE_STRETCH_BORDER)
            ->SetDefault(DRAW_BITMAP_BITBLT);

		s.AddString(XML_RENDER_LIST_STATEMAPINDEX, this, 
			memfun_cast<pfnStringSetter>(&ImageListRender::SetState2Index),
			memfun_cast<pfnStringGetter>(&ImageListRender::GetState2Index));

		s.AddString(XML_RENDER_IMAGE, this, 
			memfun_cast<pfnStringSetter>(&ImageListRender::LoadImageList),
			memfun_cast<pfnStringGetter>(&ImageListRender::GetImageListId));
	}
}

void  ImageListRender::LoadImageList(LPCTSTR szText)
{
	SAFE_RELEASE(m_pImageList);

	IRenderBitmap* pBitmap = NULL;
	_LoadBitmap(szText, pBitmap);
	if (NULL == pBitmap)
		return;

	if (pBitmap->GetImageType() != IMAGE_ITEM_TYPE_IMAGE_LIST)
	{
		UI_LOG_WARN(_T("ImageType != IMAGE_ITEM_TYPE_IMAGE_LIST. Bitmap: %s"), szText);
		SAFE_RELEASE(pBitmap);
		return;
	}
	m_pImageList = static_cast<IImageListRenderBitmap*>(pBitmap);

	// ���ͼƬû��alphaͨ��������֧��alpha����
	if (m_pImageList)
	{
		if (m_pImageList->GetBPP() != 32)
		{
			m_bUseAlphaAnimate = false;
		}
	}
}
LPCTSTR  ImageListRender::GetImageListId()
{
	UI::IRenderBitmap* p = static_cast<UI::IRenderBitmap*>(m_pImageList);
	return _GetBitmapId(p);
}

void  ImageListRender::SetState2Index(LPCTSTR szText)
{ 
	m_mapState2Index.clear();

	Util::ISplitStringEnum* pEnum = NULL;
	int nCount = Util::SplitString(szText, XML_SEPARATOR, &pEnum);
	for (int i = 0; i < nCount; i++)
	{
		LPCTSTR szStateIndex = pEnum->GetText(i);

		Util::ISplitStringEnum* pEnumInner = NULL;
		if (2 != Util::SplitString(szStateIndex, _T(':'), &pEnumInner))
		{
			UI_LOG_WARN(_T("invalid state index: %s"), szStateIndex);
			SAFE_RELEASE(pEnumInner);
			continue;
		}
		int nState = _ttoi(pEnumInner->GetText(0));
		int nIndex = _ttoi(pEnumInner->GetText(1));
		m_mapState2Index[nState] = nIndex;
		SAFE_RELEASE(pEnumInner);
	}
	SAFE_RELEASE(pEnum);
}
LPCTSTR  ImageListRender::GetState2Index()
{
    if (m_mapState2Index.empty())
        return NULL;

	String& strTempBuffer = GetTempBufferString();

	TCHAR szItem[16] = {0};
	map<int,int>::iterator iter = m_mapState2Index.begin();
	for (; iter != m_mapState2Index.end(); ++iter)
	{
		_stprintf(szItem, TEXT("%d%c%d"), iter->first, XML_KEYVALUE_SEPARATOR, iter->second);

		if (!strTempBuffer.empty())
			strTempBuffer.push_back(XML_SEPARATOR);
		strTempBuffer.append(szItem);
	}

	return strTempBuffer.c_str();
}

void  ImageListRender::SetIImageListRenderBitmap(IImageListRenderBitmap* pBitmap)
{
    SAFE_RELEASE(m_pImageList);
    m_pImageList = pBitmap;
    if (m_pImageList)
        m_pImageList->AddRef();
}

IRenderBitmap*  ImageListRender::GetRenderBitmap()
{
    return m_pImageList;
}
#if 0
UIA::E_ANIMATE_TICK_RESULT ImageListRender::OnAnimateTick(UIA::IStoryboard* pStoryboard)
{
    m_nCurrentAlpha = pStoryboard->FindTimeline(0)
		->GetCurrentIntValue();

    bool bFinish = pStoryboard->IsFinish();
    //m_pObject->UpdateObject();  // ע����listbox���ֿؼ��У���ʹ�õ�ǰforegnd render�����������е�item������Imagelistrender�еĶ��������ϵ�ɾ������ӣ�������
	m_pObject->Invalidate();

    if (bFinish)
        m_bIsAnimate = false;

    return UIA::ANIMATE_TICK_RESULT_CONTINUE;
}
#endif
void  ImageListRender::DrawState(RENDERBASE_DRAWSTATE* pDrawStruct)
{
	if (NULL == m_pImageList)
		return;

    IRenderTarget*  pRenderTarget = pDrawStruct->pRenderTarget;
    CRect rc(&pDrawStruct->rc);
    int&  nState = pDrawStruct->nState;

	int nRenderState = (pDrawStruct->nState) & RENDER_STATE_MASK;
	int nRealIndex = LOWORD(nState);
// 	if (nRealIndex >= m_pImageList->GetItemCount())  // ��GetStateIndex��ȡ
// 		nRealIndex = 0;

	if (false == m_bUseAlphaAnimate)
	{
		DrawIndexWidthAlpha(pRenderTarget, &rc, nRealIndex, 255);
		return;
	}

	// ��Normal->Hover����Hover->Normalʱ������������ʱ
	if ((m_nPrevState&(RENDER_STATE_NORMAL|RENDER_STATE_DEFAULT)) && (nRenderState&RENDER_STATE_HOVER))
	{
        m_nCurrentAlpha = 0;  // �����ڵ�һ��Tick��Ӧ֮ǰ���ؼ�ǿ��ˢ���ˣ������ʱ��m_nCurrentAlpha���Ǽ���õ���ֵ��
        CreateAnimate(0, 255);

		DrawIndexWidthAlpha(pRenderTarget, &rc, LOWORD(m_nPrevState), 255);
	}
	else if ((nRenderState&(RENDER_STATE_NORMAL|RENDER_STATE_DEFAULT)) && (m_nPrevState&RENDER_STATE_HOVER))
	{
        m_nCurrentAlpha = 255;
        CreateAnimate(255, 0);

		DrawIndexWidthAlpha(pRenderTarget, &rc, LOWORD(m_nPrevState), 255);
	}
	else
	{
		if (m_bIsAnimate)
		{
			if (0==(nRenderState & (RENDER_STATE_NORMAL|RENDER_STATE_DEFAULT|RENDER_STATE_HOVER)))
			{
				// �ڶ��������а����ˣ�Ӧ������ֹͣ����
				DestroyAnimate();
				DrawIndexWidthAlpha(pRenderTarget, &rc, nRealIndex, 255);
			}
			else
			{
				bool bSelected = (nRenderState & RENDER_STATE_SELECTED)?true:false;
                if (bSelected)  // checkbox
                {
					DrawIndexWidthAlpha(pRenderTarget, &rc, 4, (byte)(255 - m_nCurrentAlpha));
					DrawIndexWidthAlpha(pRenderTarget, &rc, 5, (byte)m_nCurrentAlpha);
                }
                else   // pushbutton/defaultbutton
                {
                    bool bDefault = (nRenderState & RENDER_STATE_DEFAULT)?true:false;

					DrawIndexWidthAlpha(pRenderTarget, &rc, bDefault ? 4 : 0, (byte)(255 - m_nCurrentAlpha));
					DrawIndexWidthAlpha(pRenderTarget, &rc, 1, (byte)m_nCurrentAlpha);
                }
			}
		}
		else
		{
			DrawIndexWidthAlpha(pRenderTarget, &rc, nRealIndex, 255);
		}
	}
	m_nPrevState = nState;
}

void  ImageListRender::DestroyAnimate()
{
#if 0
    if (m_bUseAlphaAnimate && m_bIsAnimate)
    {
        IUIApplication* pUIApp = m_pObject->GetIUIApplication();
        UIA::IAnimateManager* pAnimateMgr = pUIApp->GetAnimateMgr();
        pAnimateMgr->ClearStoryboardByNotify(
            static_cast<IAnimateEventCallback*>(this));
        m_bIsAnimate = false;
    }
#endif
}

void  ImageListRender::CreateAnimate(int nFrom, int nTo)
{
#if 0
    IUIApplication* pUIApp = m_pObject->GetIUIApplication();
    UIA::IAnimateManager* pAnimateMgr = pUIApp->GetAnimateMgr();
    pAnimateMgr->ClearStoryboardByNotify(
        static_cast<IAnimateEventCallback*>(this));

    UIA::IStoryboard*  pStoryboard = pAnimateMgr->
            CreateStoryboard(static_cast<IAnimateEventCallback*>(this));

    UIA::IFromToTimeline* pTimeline = pStoryboard->CreateTimeline(0);
    pTimeline->SetParam((float)nFrom, (float)nTo, 200)
            ->SetEaseType(UIA::linear);
    pStoryboard->Begin();

    m_bIsAnimate = true;
#endif
}

void  ImageListRender::DrawIndexWidthAlpha(IRenderTarget* pRenderTarget, const CRect* prc, int nIndex, byte bAlpha)
{
	if (NULL == m_pImageList)
		return;

//    UI_LOG_ERROR(_T("%s  nIndex=%d, nAlpha=%d"), FUNC_NAME, nIndex, bAlpha);
    int nRealIndex = GetStateIndex(nIndex);
    if (nRealIndex < 0)
        return;

	DRAWBITMAPPARAM param;
	param.nFlag = m_nImageDrawType;
	param.xDest = prc->left;
	param.yDest = prc->top;
	param.wDest = prc->Width();
	param.hDest = prc->Height();
	param.wSrc = m_pImageList->GetItemWidth();
	param.hSrc = m_pImageList->GetItemHeight();
	if (!m_9Region.IsAll_0())
		param.pRegion = &m_9Region;
	param.nAlpha = bAlpha;

	POINT pt = {0, 0};
	m_pImageList->GetIndexPos(nRealIndex, &pt);
	param.xSrc = pt.x;
	param.ySrc = pt.y;
// 	if (m_eImageLayout == IMAGELIST_LAYOUT_TYPE_H)
// 	{
// 		param.xSrc = nIndex*m_nItemWidth;
// 		param.ySrc = 0;
// 	}
// 	else
// 	{
// 		param.xSrc = 0;
// 		param.ySrc = nIndex*m_nItemHeight;
// 	}
	pRenderTarget->DrawBitmap(m_pImageList, &param);
}

void  ImageListRender::GetDesiredSize(SIZE* pSize)
{
	pSize->cx = pSize->cy = 0;
	if (NULL == m_pImageList)
		return;

	pSize->cx = m_pImageList->GetItemWidth();
	pSize->cy = m_pImageList->GetItemHeight();
}

int  ImageListRender::GetItemWidth()
{
	if (NULL == m_pImageList)
		return 0;

	return m_pImageList->GetItemWidth();
}
int  ImageListRender::GetItemHeight()
{
	if (NULL == m_pImageList)
		return 0;

	return m_pImageList->GetItemHeight();
}
int   ImageListRender::GetItemCount()
{
    if (NULL == m_pImageList)
        return 0;

    return m_pImageList->GetItemCount();
}

void  ImageListRender::SetImageStretch9Region(C9Region* p)
{
	if (p)
		m_9Region.Copy(*p);
	else
		m_9Region.Set(0);
}
//
//	��ȡָ��״̬��Ӧ��ͼƬ��
//
int  ImageListRender::GetStateIndex(int nState)
{
	if (NULL == m_pImageList)
		return -1;

	if (m_pImageList->GetItemCount() <= 0)
		return -1;

	map<int, int>::iterator iter = m_mapState2Index.end();
    if (m_mapState2Index.size() > 0)
        iter = m_mapState2Index.find(nState);

	if (m_mapState2Index.end() == iter)
	{
		// û�����ø�״̬����û������
		if (nState < m_pImageList->GetItemCount())  // ���ͼƬ�������ڵ�ǰ״̬��ֱ��ȡ��ǰ״ֵ̬��Ӧ��ͼƬ
		{
			return nState;
		}
		else                    // ״ֵ̬����ͼƬ������ȡĬ��ֵ0
		{
			return 0;
		}
	}
	else
	{
		int& nRet = iter->second;
		if (nRet < 0 || nRet >= m_pImageList->GetItemCount())
			return -1;

		return nRet;
	}
	return -1;
}

