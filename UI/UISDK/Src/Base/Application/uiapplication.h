#pragma once

#include "Src\Helper\topwindow\topwindowmanager.h"
#include "Src\Helper\tooltip\tooltipmanager.h"
#include "Src\Helper\msg\msghelper.h"
#include "Src\Resource\skinmanager.h"
#include "Src\Helper\MouseKeyboard\mousemanager.h"
#include "Src\Helper\layout\layout_factory.h"
#include "Src\Renderbase\renderbase\renderbase_factory.h"
#include "Src\Renderbase\textrenderbase\textrender_factory.h"
#include "Src\Animate\wrap\animate.h"


namespace UI
{
class GifTimerManager;
interface IObjectDescription;
class ImageManager;
class ImageRes;
class CursorRes;
class GifRes;
class FontManager;
class FontRes;
class ColorManager;
class ColorRes;
class StyleManager;
class StyleRes;
class LayoutManager;

//
//  PS: 20130110 ���ǵ���UIEditor�л��������UIApplicationʵ������˲����ٽ�UIApplication��Ϊȫ�ֶ�����ʵ���ˡ�
//  PS: 20130317 ���ǵ��Ժ����չ�������������ӿ����ɴ��麯�������ǲ���Pimplʵ��
//
class UIApplication
{
public:
	UIApplication(IUIApplication* p);
	~UIApplication(void);
    void  x_Init();  // �ڲ���ʼ���������ڹ��캯���е���̫�ණ��

	void  SetEditorMode(bool b) { m_bEditorMode = b; }
	bool  IsEditorMode() { return m_bEditorMode; }
	void  SetUIEditorPtr(IUIEditor* p) { m_pUIEditor = p; }
	IUIEditor*  GetUIEditorPtr() { return m_pUIEditor; }
	
	SkinManager&  GetSkinMgr();
	ITopWindowManager*  GetITopWindowMgr();
	TopWindowManager*   GetTopWindowMgr() { return &m_TopWindowMgr; }
	UIA::IAnimateManager*   GetAnimateMgr();
	GifTimerManager*    GetGifTimerMgr() { return m_pGifTimerMgr; }
	IWaitForHandlesMgr*  GetWaitForHandlesMgr() { return &m_WaitForHandlesMgr; }
	IMessageFilterMgr*  GetMessageFilterMgr() { return &m_MsgFilterMgr; }
	HMODULE  GetUID2DModule();
	HMODULE  GetUID3DModule();

	RenderBaseFactory&  GetRenderBaseFactory() { return m_renderBaseFactory; }
	TextRenderFactory&  GetTextRenderFactroy() { return m_textRenderFactroy; }
	LayoutFactory&  GetLayoutFactory() { return m_layoutFactory; }

	LPCTSTR  GetRenderBaseName(int nType);
	bool  CreateRenderBaseByName(
				LPCTSTR szName, 
                IObject* pObject, 
                IRenderBase** ppOut);

	SkinRes*  GetDefaultSkinRes();
	
	bool  ShowToolTip(TOOLTIPITEM* pItem);
	void  HideToolTip();

    void  RestoreRegisterUIObject();
    bool  RegisterControlTagParseFunc(LPCTSTR szTag, pfnParseControlTag func);
    bool  GetSkinTagParseFunc(LPCTSTR szTag, pfnParseSkinTag* pFunc);
    bool  GetControlTagParseFunc(LPCTSTR szTag, pfnParseControlTag* pFunc);

    bool  RegisterUIObject(IObjectDescription*);
	IObject*  CreateUIObjectByName(LPCTSTR szXmlName, ISkinRes*);
    IObject*  CreateUIObjectByClsid(REFCLSID clsid, ISkinRes*);
    void  LoadUIObjectListToToolBox();

	BOOL  IsDialogMessage(MSG* pMsg);
	void  MsgHandleLoop(bool* pbQuitLoopRef);
	void  RedrawTopWindows() { m_TopWindowMgr.UpdateAllWindow(); }
	bool  IsUnderXpOS();
	bool  IsVistaOrWin7etc();
	HWND  GetForwardPostMessageWnd();

    // gpu 
    bool  IsGpuCompositeEnable();
    bool  EnableGpuComposite();

public:
    IUIApplication*  GetIUIApplication() { return m_pUIApplication; }

private:
    void  RegisterWndClass();
    void  RegisterDefaultUIObject();
    void  ClearRegisterUIObject();

public:

private:
    typedef vector<IObjectDescription*>       UIOBJ_CREATE_DATA;
    typedef map<String, pfnParseSkinTag>      UISKINTAGPARSE_DATA;
    typedef map<String, pfnParseControlTag>   UICONTROLTAGPARSE_DATA;

    IUIApplication*     m_pUIApplication;              // �����ṩ�Ľӿ�
    OSVERSIONINFOEX     m_osvi;                        // ����ϵͳ�汾
	IUIEditor*          m_pUIEditor;                   // �ⲿ�ı༭��ָ�룬������Ϣ֪ͨ�����ݻ�ȡ

    // gpu�ϳ�
    bool  m_bGpuEnable;
    // �Ƿ������ģʽ
    bool  m_bEditorMode;

	UIOBJ_CREATE_DATA         m_vecUIObjectDesc;       // ����UI�����XML�ַ��������ڴ��ַ�������UI����
    UISKINTAGPARSE_DATA       m_mapSkinTagParseData;
    UICONTROLTAGPARSE_DATA    m_mapControlTagParseData;

	RenderBaseFactory  m_renderBaseFactory;
	TextRenderFactory  m_textRenderFactroy;
	LayoutFactory  m_layoutFactory;

    TopWindowManager    m_TopWindowMgr; 
    SkinManager         m_skinMgr;       // ��Ҫ��ǰ�ͷţ�����ָ��ģʽ
	GifTimerManager*    m_pGifTimerMgr;

    WaitForHandlesMgr   m_WaitForHandlesMgr;
    ForwardPostMessageWindow  m_WndForwardPostMsg; 
    MessageFilterMgr    m_MsgFilterMgr;
    ToolTipManager      m_ToolTipMgr;
    AnimateHelper       m_animate; 
};

}

