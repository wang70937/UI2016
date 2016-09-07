#pragma once 
#include "Src\Base\Message\message.h"
#include "Inc\Interface\iimagerender.h"
#include "Src\Renderbase\renderbase\renderbase.h"
//#include "UISDK\Animate\Inc\animate_base_inc.h"

namespace UI
{

class ImageRender : public RenderBase
{
public:
	ImageRender(IImageRender* p);
	~ImageRender();

    UI_DECLARE_RENDERBASE(ImageRender, XML_RENDER_TYPE_IMAGE, RENDER_TYPE_IMAGE)

	UI_BEGIN_MSG_MAP()
        UIMSG_RENDERBASE_DRAWSTATE(DrawState)
        UIMSG_GETDESIREDSIZE(GetDesiredSize)
        UIMSG_SERIALIZE(OnSerialize)
        UIMSG_QUERYINTERFACE(ImageRender)
    UI_END_MSG_MAP_CHAIN_PARENT(RenderBase)


public:
    void  SetAlpha(int nAlpha);
    int   GetAlpha();
    void  SetColor(Color c);
    Color  GetColor();
    void  SetImageDrawType(int n);
    int   GetImageDrawType();

    BKCOLOR_FILL_TYPE  GetBkColorFillType() { return m_eBkColorFillType; }
    void  SetBkColorFillType(BKCOLOR_FILL_TYPE eType) { m_eBkColorFillType = eType; }

    IRenderBitmap*  GetRenderBitmap() { return m_pBitmap; }
    void  SetRenderBitmap(IRenderBitmap* pBitmap);

protected:
    void  OnSerialize(SERIALIZEDATA* pData);
    void  DrawState(RENDERBASE_DRAWSTATE* pDrawStruct);
	void  GetDesiredSize(SIZE* pSize);

private:
	void  LoadBitmap(LPCTSTR szBitmapId)
	{
		_LoadBitmap(szBitmapId, m_pBitmap);
	}
	LPCTSTR  GetBitmapId()
	{
		return _GetBitmapId(m_pBitmap);
	}
	void  LoadColor(LPCTSTR szColorId)
	{
		_LoadColor(szColorId, m_pColorBk); 
	}
	LPCTSTR  GetColorId()
	{
		return _GetColorId(m_pColorBk);
	}

protected:
    IImageRender*     m_pIImageRender;

	IRenderBitmap*    m_pBitmap;
	Color*    m_pColorBk;
	C9Region  m_Region;   // �������ʱ����
    RECT  m_rcSrc;     // ��������(Ϊ�ձ�ʾδ����)���п���ֻ�ǻ���ͼƬ��һ����.�������ͨ���޸ĸ�ֵʵ��ĳЩ����
    BKCOLOR_FILL_TYPE  m_eBkColorFillType;
	long   m_nImageDrawType;
    long  m_nAlpha;

};

// ����ͼƬ�б���ָ��������һ��
class ImageListItemRender : public ImageRender 
{
public:
	ImageListItemRender(IImageListItemRender* p);
	~ImageListItemRender();

    UI_DECLARE_RENDERBASE(ImageListItemRender, XML_RENDER_TYPE_IMAGELISTITEM, RENDER_TYPE_IMAGELISTITEM)

	UI_BEGIN_MSG_MAP()
        UIMSG_RENDERBASE_DRAWSTATE(DrawState)
        UIMSG_GETDESIREDSIZE(GetDesiredSize)
        UIMSG_SERIALIZE(OnSerialize)
        UIMSG_QUERYINTERFACE(ImageListItemRender)
    UI_END_MSG_MAP_CHAIN_PARENT(ImageRender)


	void  OnSerialize(SERIALIZEDATA* pData);
	void  GetDesiredSize(SIZE* pSize);
	void  DrawState(RENDERBASE_DRAWSTATE* pDrawStruct);

protected:
    IImageListItemRender*  m_pIImageListItemRender;

	IImageListRenderBitmap*  m_pImageList;
	long  m_nImagelistIndex;
};


//
//	ͼƬ������ť
//
interface IStoryboard;
class ImageListRender :
            public RenderBase
//			, public UIA::IAnimateEventCallback
{
public:
	ImageListRender(IImageListRender* p);
	~ImageListRender( );

    UI_DECLARE_RENDERBASE(ImageListRender, XML_RENDER_TYPE_IMAGELIST, RENDER_TYPE_IMAGELIST)

	UI_BEGIN_MSG_MAP()
        UIMSG_RENDERBASE_DRAWSTATE(DrawState)
        UIMSG_QUERYINTERFACE(ImageListRender)
        UIMSG_GETDESIREDSIZE(GetDesiredSize)
        UIMSG_SERIALIZE(OnSerialize)
    UI_END_MSG_MAP_CHAIN_PARENT(RenderBase)

    void  SetIImageListRenderBitmap(IImageListRenderBitmap* pBitmap);
    IRenderBitmap*  GetRenderBitmap();
	void  LoadImageList(LPCTSTR);
	LPCTSTR  GetImageListId();

    void  SetImageDrawType(long n) { m_nImageDrawType = n; }
    long   GetImageDrawType() { return m_nImageDrawType; }
    void  SetImageStretch9Region(C9Region* p);
	void  SetState2Index(LPCTSTR);
	LPCTSTR  GetState2Index();
	
	void  OnSerialize(SERIALIZEDATA* pData);
	void  DrawState(RENDERBASE_DRAWSTATE* pDrawStruct);
	void  GetDesiredSize(SIZE* pSize);
    //virtual UIA::E_ANIMATE_TICK_RESULT OnAnimateTick(UIA::IStoryboard*) override;

	int   GetItemWidth();
	int   GetItemHeight();
    int   GetItemCount();

	int   GetStateIndex(int nState);
protected:
	void  DrawIndexWidthAlpha(IRenderTarget*, const CRect* prc, int nIndex, byte bAlpha);
    void  CreateAnimate(int nFrom, int nTo);
    void  DestroyAnimate();

protected:
    IImageListRender*  m_pIImageListRender;

	IImageListRenderBitmap*   m_pImageList;
	long  m_nImageDrawType; 
	C9Region  m_9Region;

	int  m_nPrevState;        // �����жϵ�ǰ״̬�仯�Ƿ���Ҫ���н���
	int  m_nCurrentAlpha;     // ���������е�alphaֵ
	bool m_bIsAnimate;        // ��ǰ�Ƿ����ڶ���������
	bool m_bUseAlphaAnimate;  // �Ƿ�����alpha���䶯��
	
	map<int,int>   m_mapState2Index;    // Ҫ���Ƶ�״̬��ӦͼƬ���ĸ�item
};

}