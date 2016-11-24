#pragma  once
#include "Inc\Interface\ilabel.h"

namespace UI
{
//
//  ���ֿؼ�
//
class Label : public MessageProxy, public UI::IBindSourceChangedListener
{
public:
	Label(ILabel* p);
	~Label();

	UI_BEGIN_MSG_MAP()
		UIMSG_PAINT(OnPaint)
        //MSG_WM_LBUTTONUP(OnLButtonUp)
        UIMSG_GETDESIREDSIZE(GetDesiredSize)
        UIMSG_QUERYINTERFACE(Label)
        UIMSG_SERIALIZE(OnSerialize)
        UIMSG_FINALCONSTRUCT(FinalConstruct)
    UIALT_MSG_MAP(UIALT_CALLLESS)
        UIMSG_CREATEBYEDITOR(OnCreateByEditor)
	UI_END_MSG_MAP_CHAIN_PARENT_Ixxx(Label, IControl)

public:
    LPCTSTR  GetText();
    void  SetText(LPCTSTR pszText);
	void  SetTextAndInvalidate(LPCTSTR pszText);

	IBindSourceChangedListener*  GetListener() {
		return static_cast<IBindSourceChangedListener*>(this);
	}

	// ��Ϣ����
protected:
	void  OnPaint( IRenderTarget* hDC );
	void  OnSerialize(SERIALIZEDATA*);
	void  GetDesiredSize(SIZE* pSize);
    //void  OnLButtonUp(UINT nFlags, POINT point);
    HRESULT  FinalConstruct(ISkinRes* p);
    void  OnCreateByEditor(CREATEBYEDITORDATA* pData);

    //void  OnClicked(POINT* pt);

	virtual void OnBindSourceChanged(long key, bindable* src, void* context) override;

	// ����
protected:
    ILabel*  m_pILabel;
	String  m_strText;
};


//
//	��̬ͼƬ�ؼ�(֧��ico��... )
//
class PictureCtrl : public MessageProxy
{
public:
	PictureCtrl(IPictureCtrl* p);
	~PictureCtrl();

	UI_BEGIN_MSG_MAP()
		UIMSG_PAINT(OnPaint)
		UIMSG_GETDESIREDSIZE(GetDesiredSize)
		UIMSG_QUERYINTERFACE(PictureCtrl)
		UIMSG_FINALCONSTRUCT(FinalConstruct)
    UIALT_MSG_MAP(UIALT_CALLLESS)
        UIMSG_CREATEBYEDITOR(OnCreateByEditor)
    UI_END_MSG_MAP_CHAIN_PARENT_Ixxx(PictureCtrl, IControl)

public:
    bool SetImageByPath(LPCTSTR szPath);
    bool SetImageById(LPCTSTR szId);

private:
	void  GetDesiredSize(SIZE* pSize);
	void  OnPaint(IRenderTarget* pRenderTarget);
	HRESULT  FinalConstruct(ISkinRes* p);
    void  OnCreateByEditor(CREATEBYEDITORDATA* pData);

protected:
    IPictureCtrl*  m_pIPictureCtrl;
};


struct IGifImageRender;
//
//	����ͼƬ
//	TODO: ����PNG���ж�������
//
class GifCtrl : public MessageProxy
{
public:
	GifCtrl(IGifCtrl* p);
	~GifCtrl();

	UI_BEGIN_MSG_MAP()
		UIMSG_PAINT(OnPaint)
        UIMSG_GETDESIREDSIZE(OnGetDesiredSize)
		UIMSG_WM_GIFFRAME_TICK(OnGifFrameTick)
        UIMSG_VISIBLE_CHANGED(OnVisibleChanged)
        UIMSG_SERIALIZE(OnSerialize)
        UIMSG_QUERYINTERFACE(GifCtrl)
        UIMSG_FINALCONSTRUCT(FinalConstruct)
	UI_END_MSG_MAP_CHAIN_PARENT_Ixxx(GifCtrl, IControl)

    IGifCtrl*  GetIGifCtrl() { return m_pIGifCtrl; }

public:
    bool  Start();
    bool  Pause();
    bool  Stop();

public:
	// virtual ����
	void  OnGetDesiredSize(SIZE* pSize);
    void  OnSerialize(SERIALIZEDATA* pData);
	HRESULT  FinalConstruct(ISkinRes* p);

	void  OnPaint(IRenderTarget* pRenderTarget);
	void  OnGifFrameTick(WPARAM wParam, LPARAM lParam);
    void  OnVisibleChanged(BOOL bVisible, IObject* pObjChanged);

private:
    IGifCtrl*  m_pIGifCtrl;
	IGifImageRender*  m_pGifRender;
};
}