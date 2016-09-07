#include "stdafx.h"
#include "Inc\Interface\iuiapplication.h"
#include "Src\Base\Application\uiapplication.h"
#include "Inc\Interface\iobject.h"
#include "Src\Atl\image.h"
#include "Src\Helper\timer\timermanager.h"
#include "Src\Util\dwm\dwmhelper.h"
#include "Src\UIEffect\CacheBitmap\cachebitmap.h"
#include "Src\UIEffect\blur\webkit\shadowblur.h"
#include "Src\Resource\skinres.h"
#include "Src\Helper\layout\layout_factory.h"
#include "Src\Renderbase\renderbase\renderbase_factory.h"

using namespace UI;

IUIApplication::IUIApplication()
{
    m_pImpl = new UIApplication(this);
}
IUIApplication::~IUIApplication()
{
    SAFE_DELETE(m_pImpl);
}

UIApplication*  IUIApplication::GetImpl() 
{ 
	return m_pImpl;
}

void  IUIApplication::Release()
{
	delete this;
}

ISkinRes*  IUIApplication::LoadSkinRes(LPCTSTR szSkinResPath)
{
    SkinRes* p = m_pImpl->GetSkinMgr().LoadSkinRes(szSkinResPath); 
	if (p)
		return p->GetISkinRes();
	return NULL;
}

void  IUIApplication::SetDesignMode(bool b)
{
	m_pImpl->SetDesignMode(b); 
}
void  IUIApplication::SetUIEditorPtr(IUIEditor* p)
{
	m_pImpl->SetUIEditorPtr(p); 
}

bool  IUIApplication::IsDesignMode()
{ 
	return m_pImpl->IsDesignMode();
}
IUIEditor*  IUIApplication::GetUIEditorPtr()
{
    return m_pImpl->GetUIEditorPtr(); 
}

ISkinManager*  IUIApplication::GetSkinMgr()    
{ 
    return m_pImpl->GetSkinMgr().GetISkinManager();
}
ITopWindowManager*  IUIApplication::GetTopWindowMgr()
{
    return m_pImpl->GetITopWindowMgr(); 
}
UIA::IAnimateManager*  IUIApplication::GetAnimateMgr()  
{ 
    return m_pImpl->GetAnimateMgr();
}
IMessageFilterMgr*  IUIApplication::GetMessageFilterMgr()
{
	return m_pImpl->GetMessageFilterMgr();
}
ISkinRes*  IUIApplication::GetDefaultSkinRes()
{
    SkinRes* p = m_pImpl->GetDefaultSkinRes(); 
	if (p)
		return p->GetISkinRes();
	return NULL;
}
IImageManager*  IUIApplication::GetActiveSkinImageMgr()
{
    ImageManager* p = m_pImpl->GetActiveSkinImageMgr();
	if (p)
		return p->GetIImageManager();
	return NULL;
}
IImageRes*  IUIApplication::GetActiveSkinImageRes()
{
    ImageRes* p = m_pImpl->GetActiveSkinImageRes(); 
	if (p)
		return p->GetIImageRes();
	return NULL;
}
ICursorRes*  IUIApplication::GetActiveSkinCursorRes()
{
    CursorRes* p = m_pImpl->GetActiveSkinCursorRes();
	if (p)
		return p->GetICursorRes();
	return NULL;
}
IGifRes*  IUIApplication::GetActiveSkinGifRes()
{
    GifRes* p = m_pImpl->GetActiveSkinGifRes();
	if (p)
		return p->GetIGifRes();
	return NULL;
}
IFontManager* IUIApplication::GetActiveSkinFontMgr()
{
    FontManager* p = m_pImpl->GetActiveSkinFontMgr();
	if (p)
		return p->GetIFontManager();
	return NULL;
}
IFontRes*  IUIApplication::GetActiveSkinFontRes()
{ 
    FontRes* p = m_pImpl->GetActiveSkinFontRes();
	if (p)
		return p->GetIFontRes();
	return NULL;
}
IColorManager*  IUIApplication::GetActiveSkinColorMgr() 
{ 
    ColorManager* p = m_pImpl->GetActiveSkinColorMgr(); 
	if (p)
		return p->GetIColorManager();
	return NULL;
}
IColorRes*  IUIApplication::GetActiveSkinColorRes() 
{
    ColorRes* p = m_pImpl->GetActiveSkinColorRes(); 
	if (p)
		return p->GetIColorRes();
	return NULL;
}
IStyleManager*  IUIApplication::GetActiveSkinStyleMgr() 
{ 
    StyleManager* p = m_pImpl->GetActiveSkinStyleMgr();
	if (p)
		return p->GetIStyleManager();
	return NULL;
}
IStyleRes*  IUIApplication::GetActiveSkinStyleRes() 
{
    StyleRes* p = m_pImpl->GetActiveSkinStyleRes();
	if (p)
		return p->GetIStyleRes();
	return NULL;
}
ILayoutManager* IUIApplication::GetActiveSkinLayoutMgr() 
{
    LayoutManager* p = m_pImpl->GetActiveSkinLayoutMgr(); 
	if (p)
		return p->GetILayoutManager();
	return NULL;
}

void  IUIApplication::RestoreRegisterUIObject()      
{
    m_pImpl->RestoreRegisterUIObject(); 
}
bool  IUIApplication::RegisterControlTagParseFunc(LPCTSTR szTag, pfnParseControlTag func)
{ 
    return m_pImpl->RegisterControlTagParseFunc(szTag, func); 
}
bool  IUIApplication::GetSkinTagParseFunc(LPCTSTR szTag, pfnParseSkinTag* pFunc)
{ 
    return m_pImpl->GetSkinTagParseFunc(szTag, pFunc);
}
bool  IUIApplication::GetControlTagParseFunc(LPCTSTR szTag, pfnParseControlTag* pFunc) 
{ 
    return m_pImpl->GetControlTagParseFunc(szTag, pFunc);
}

IObject*  IUIApplication::CreateUIObjectByName(LPCTSTR szName, ISkinRes* pISkinRes)
{ 
    return m_pImpl->CreateUIObjectByName(szName, pISkinRes); 
}
IObject*  IUIApplication::CreateUIObjectByClsid(REFCLSID clsid, ISkinRes* pISkinRes) 
{ 
    return m_pImpl->CreateUIObjectByClsid(clsid, pISkinRes);
}
bool  IUIApplication::RegisterUIObject(IObjectDescription* p)
{
    return m_pImpl->RegisterUIObject(p); 
}
void     IUIApplication::LoadUIObjectListToToolBox()         
{
    m_pImpl->LoadUIObjectListToToolBox(); 
}

bool  IUIApplication::RegisterUIRenderBaseCreateData(
	LPCTSTR szName, int nType, pfnUICreateRenderBasePtr pfunc) 
{
    return m_pImpl->GetRenderBaseFactory().RegisterUIRenderBaseCreateData(szName, nType, pfunc); 
}
bool  IUIApplication::CreateRenderBaseByName(LPCTSTR szName, IObject* pObject, IRenderBase** ppOut)
{
    ISkinRes* pSkinRes = NULL;
    if (pObject)
        pSkinRes = pObject->GetSkinRes();
    else
        pSkinRes = GetDefaultSkinRes();
    return m_pImpl->GetRenderBaseFactory().CreateRenderBaseByName(pSkinRes, szName, pObject, ppOut); 
}
bool  IUIApplication::CreateRenderBase(int nType, IObject* pObject, IRenderBase** ppOut) 
{ 
    ISkinRes* pSkinRes = NULL;
    if (pObject)
        pSkinRes = pObject->GetSkinRes();
    else
        pSkinRes = GetDefaultSkinRes();
    return m_pImpl->GetRenderBaseFactory().CreateRenderBase(pSkinRes, nType, pObject, ppOut);
}
LPCTSTR  IUIApplication::GetRenderBaseName(int nType)
{
    return m_pImpl->GetRenderBaseFactory().GetRenderBaseName(nType);
}
void  IUIApplication::EnumRenderBaseName(pfnEnumRenderBaseNameCallback callback, WPARAM wParam, LPARAM lParam)
{
    m_pImpl->GetRenderBaseFactory().EnumRenderBaseName(callback, wParam, lParam);
}

bool  IUIApplication::RegisterUITextRenderBaseCreateData(
	LPCTSTR szName, int nType,
    pfnUICreateTextRenderBasePtr pfunc) 
{ 
    return m_pImpl->GetTextRenderFactroy().RegisterUITextRenderBaseCreateData(szName, nType, pfunc); 
}
bool  IUIApplication::CreateTextRenderBaseByName(LPCTSTR szName, IObject* pObject, ITextRenderBase** ppOut) 
{
    ISkinRes* pSkinRes = NULL;
    if (pObject)
        pSkinRes = pObject->GetSkinRes();
    else
        pSkinRes = GetDefaultSkinRes();
    return m_pImpl->GetTextRenderFactroy().CreateTextRenderBaseByName(pSkinRes, szName, pObject, ppOut);
}
bool  IUIApplication::CreateTextRenderBase(int nType, IObject* pObject, ITextRenderBase** ppOut) 
{ 
    ISkinRes* pSkinRes = NULL;
    if (pObject)
        pSkinRes = pObject->GetSkinRes();
    else
        pSkinRes = GetDefaultSkinRes();
    return m_pImpl->GetTextRenderFactroy().CreateTextRender(pSkinRes, nType, pObject, ppOut); 
}
void  IUIApplication::EnumTextRenderBaseName(pfnEnumTextRenderBaseNameCallback callback, WPARAM wParam, LPARAM lParam)
{
    m_pImpl->GetTextRenderFactroy().EnumTextRenderBaseName(callback, wParam, lParam);
}

bool  IUIApplication::CreateLayoutByName(LPCTSTR szName, IObject* pObject, bool bCreateDefault, ILayout** ppOut) 
{ 
    return m_pImpl->GetLayoutFactory().CreateByName(szName, pObject, bCreateDefault, ppOut); 
}
bool  IUIApplication::CreateLayout(int nType, IObject* pObject, ILayout** ppOut)
{
    return m_pImpl->GetLayoutFactory().Create(nType, pObject, ppOut);
}
void  IUIApplication::EnumLayoutType(pfnEnumLayoutTypeCallback callback, WPARAM wParam, LPARAM lParam)
{
    m_pImpl->GetLayoutFactory().EnumLayoutType(callback, wParam, lParam);
}

BOOL  IUIApplication::IsDialogMessage(MSG* pMsg) 
{
    return m_pImpl->IsDialogMessage(pMsg); 
}
void  IUIApplication::MsgHandleLoop(bool* pbQuitLoopRef)
{ 
    m_pImpl->MsgHandleLoop(pbQuitLoopRef);
}
void  IUIApplication::RedrawTopWindows() 
{ 
    m_pImpl->RedrawTopWindows(); 
}

bool  IUIApplication::ShowToolTip(TOOLTIPITEM* pItem)
{
    return m_pImpl->ShowToolTip(pItem); 
}
void  IUIApplication::HideToolTip() 
{ 
    m_pImpl->HideToolTip(); 
}

UINT_PTR  IUIApplication::SetTimer(int nElapse, IMessage* pNotify)
{
    return TimerHelper::GetInstance()->SetTimer(nElapse, pNotify);
}
UINT_PTR  IUIApplication::SetTimer(int nElapse, TimerItem* pTimerItem)
{
    return TimerHelper::GetInstance()->SetTimer(nElapse, pTimerItem);
}
UINT_PTR  IUIApplication::SetTimerById(int nElapse, int nId, IMessage* pNotify)
{
    return  TimerHelper::GetInstance()->SetTimerById(nElapse, nId, pNotify);
}
void  IUIApplication::KillTimer(UINT_PTR& nTimerID)
{
    TimerHelper::GetInstance()->KillTimer(nTimerID);
}
void  IUIApplication::KillTimerById(int nId, IMessage* pNotify)
{
    TimerHelper::GetInstance()->KillTimerById(nId, pNotify);
}
void  IUIApplication::KillTimerByNotify(IMessage* pNotify)
{
    TimerHelper::GetInstance()->KillTimerByNotify(pNotify);
}

HDC  IUIApplication::GetCacheDC()
{
    return Image::GetCacheDC();
}
void  IUIApplication::ReleaseCacheDC(HDC hDC)
{
    return Image::ReleaseCacheDC(hDC);
}  
HBITMAP  IUIApplication::GetCacheBitmap(int nWidth, int nHeight)
{
    return CacheBitmap::GetInstance()->Create(nWidth, nHeight);
}
void   IUIApplication::ShadowBlur(HBITMAP hBitmap, COLORREF colorShadow, RECT* prcBlur, int nRadius)
{
    ::ShadowBlur(hBitmap, colorShadow, prcBlur, nRadius);
}

bool  IUIApplication::IsUnderXpOS() 
{ 
    return m_pImpl->IsUnderXpOS();
}
bool  IUIApplication::IsVistaOrWin7etc() 
{ 
    return m_pImpl->IsVistaOrWin7etc();
}
bool  IUIApplication::IsAeroEnable()
{
    return DwmHelper::GetInstance()->IsEnable();
}

HWND  IUIApplication::GetForwardPostMessageWnd() 
{ 
    return m_pImpl->GetForwardPostMessageWnd();
}

bool  IUIApplication::IsGpuCompositeEnable()
{
    return m_pImpl->IsGpuCompositeEnable();
}
bool  IUIApplication::EnableGpuComposite()
{
    return m_pImpl->EnableGpuComposite();
}

IWindowBase*  IUIApplication::GetWindowBaseFromHWND(HWND hWnd)
{
    return m_pImpl->GetTopWindowMgr()->GetWindowBase(hWnd);
}
