#pragma  once
#include "Src\Base\Message\message.h"
#include "Inc\Interface\irenderbase.h"

namespace UI
{
	interface IRenderBase;
	interface IRenderBitmap;
	class UIApplication;
    class ColorRes;
    class ImageRes;

const UINT  WINDOW_BKGND_RENDER_STATE_ACTIVE  = 0;   // ���ڼ�����ʽ
const UINT  WINDOW_BKGND_RENDER_STATE_INACTIVE = 1;   // ���ڷǼ�����ʽ


// ���ڱ������ƻ��࣬����ʵ�ָ�����ʽ���ڱ����Ļ��ƣ��磺ƽ�̣����У����죬9����
class RenderBase : public Message
{
public:
	RenderBase(IRenderBase* p);
	virtual ~RenderBase(){};
    
	UI_BEGIN_MSG_MAP()
    UI_END_MSG_MAP()

	//
	//	����Render���Ա����ںܶ�ط����米����ͼ��ȵȡ�Ϊ���������õ����ĸ����ֵ�Render���ԣ�
	//	����Render����ǰ�����һ��ǰ׺����bkgnd.render.type  icon.render.type
	//	��SetAttribute�У���Ҫͬʱ����ǰ׺+�����������в���
	//
	//	����IRender����Ҫ�ṩRetAttribute��������ΪObject��Reset��ʱ��һ����delete irender
	//	�������Ҳû����� bool bReload ����
	//
    bool  IsThemeRender() {return m_nRenderType > RENDER_TYPE_THEME_FIRST && m_nRenderType < RENDER_TYPE_THEME_LAST; }
    void  CheckThemeChanged();

	void     SetObject(UIApplication* pUIApp, Object* pObject) { m_pUIApplication = pUIApp; m_pObject = pObject; }
    Object*  GetObject() { return m_pObject; }

	void  SetRenderType( const RENDER_TYPE& nType ){ m_nRenderType = nType ; }
	RENDER_TYPE  GetRenderType() { return m_nRenderType; }

    SkinRes*  GetSkinRes();
	ColorRes*  GetSkinColorRes();
	ImageRes*  GetSkinImageRes();
	
	void  _LoadColor(LPCTSTR szColorId, Color*& pColorRef);
	void  _LoadBitmap(LPCTSTR szBitmapId, IRenderBitmap*& pBitmapRef);
	LPCTSTR  _GetColorId(Color*& pColorRef);
	LPCTSTR  _GetBitmapId(IRenderBitmap*& pBitmapRef);

protected:
    UIApplication*  m_pUIApplication;
    IRenderBase*     m_pIRenderBase;

    Object*  m_pObject;  // �󶨵Ķ���Ҫ����˭�ı���
	RENDER_TYPE   m_nRenderType;  // �Լ�������
};

// ��bkgnd.render.type=null��ʾ��ʹ��Ĭ�ϱ���
class NullRender : public RenderBase
{
public:
	NullRender(INullRender* p) : RenderBase(p)
	{
		m_pINullRender = p; 
	}

    UI_DECLARE_RENDERBASE(NullRender, XML_RENDER_TYPE_NULL, RENDER_TYPE_NULL)

	UI_BEGIN_MSG_MAP()
        UIMSG_QUERYINTERFACE(NullRender)
    UI_END_MSG_MAP_CHAIN_PARENT(RenderBase)


    INullRender*  m_pINullRender;
};

}
