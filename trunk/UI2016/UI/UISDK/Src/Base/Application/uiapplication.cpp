#include "stdafx.h"
#include "uiapplication.h"

#include "Inc\Interface\iuires.h"
#include "Src\Atl\image.h"
#include "Src\UIObject\Control\control.h"
#include "Src\Resource\skinres.h"
#include "Src\Resource\imagemanager.h"
#include "Src\Resource\colormanager.h"
#include "Src\Resource\fontmanager.h"
#include "Src\Resource\stylemanager.h"
#include "Src\Resource\layoutmanager.h"
#include "Inc\Interface\iuieditor.h"
#include "Src\UIObject\Window\window_desc.h"
#include "Src\UIObject\HwndHost\hwndhost_desc.h"
#include "Src\UIObject\Panel\panel_desc.h"
#include "Src\UIObject\Panel\ScrollPanel\scrollpanel_desc.h"
#include "Src\UIObject\Window\windowbase.h"
#include "Src\Util\DPI\dpihelper.h"
#include "Src\SkinParse\skinparseengine.h"
#include "Src\Util\Gif\giftimermanager.h"



UIApplication::UIApplication(IUIApplication* p) : 
    m_pUIApplication(p),
    m_WndForwardPostMsg(this), 
    m_TopWindowMgr(this),
	m_renderBaseFactory(*this),
	m_textRenderFactroy(*this)
{
}

void  UIApplication::x_Init()
{
    //	::CoInitialize(0);
    HRESULT  hr = OleInitialize(0);  // ��Ҫע��richedit��drag drop�������ole��ʼ��
	(hr);

    m_bDesignMode = false;
    m_pUIEditor = NULL;

    m_skinMgr.SetUIApplication(this);
    m_animate.Init(&m_WaitForHandlesMgr);

	m_pGifTimerMgr = NULL;
	m_pGifTimerMgr = new GifTimerManager();
	m_pGifTimerMgr->Init(this);

    m_ToolTipMgr.Init(this);

    // ��ȡ����ϵͳ�汾��Ϣ
    ZeroMemory(&m_osvi, sizeof(OSVERSIONINFOEX));
    m_osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    GetVersionEx((OSVERSIONINFO*) &m_osvi);

    // ���õ�ǰ���ԡ���Ҫ������ strcoll ����ƴ������(�磺combobox����)(TODO:��һ���ǲ�����Ҫ����һ�������)
    _wsetlocale( LC_ALL, _T("chs") );

    // ��ʼ��Gdiplus
    // ע��gdiplus�ᴴ��һ�������̣߳�GdiPlus.dll!BackgroundThreadProc()  + 0x59 �ֽ�	
    Image::InitGDIPlus();

    /* INITIALIZE COMMON CONTROLS, tooltip support */
    INITCOMMONCONTROLSEX iccex; 
    iccex.dwICC = ICC_WIN95_CLASSES;
    iccex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    InitCommonControlsEx(&iccex);

    // ����һ������ת����Ϣ�Ĵ��ڣ�ʵ��post ui message
    m_WndForwardPostMsg.Create(HWND_MESSAGE);

    // ���layer window��ֹ����Ӧʱ���ڱ��
    //DisableProcessWindowsGhosting();

    m_bGpuEnable = false;

    // �ȳ�ʼ��DPI���ã�Ҫ��Ȼ������ģ���ڳ�ʼ��ʱ��ֱ�ӵ���GetDCȡ����DPI��������ֵ96��
    GetDpi();

    RegisterDefaultUIObject(); 
    RegisterWndClass();
}

UIApplication::~UIApplication(void)
{
	// Ӧ�ó����˳���־
	UI_LOG_INFO( _T("\n\n------------  UI Quit ----------------\n"));

	this->m_ToolTipMgr.Release();  // ��֤���㴰�ڼ���Ϊ0

#ifdef _DEBUG
	int nCount = this->m_TopWindowMgr.GetTopWindowCount();
	if (0 != nCount)   // <-- �÷����п��ܻ��ǲ�׼���п��ܴ��ڱ������ˣ������ڶ���û������
	{
		UI_LOG_WARN(_T("UI_Exit TopWindowCount=%d"),nCount);  
	}
#endif

    ClearRegisterUIObject();

	if (m_WndForwardPostMsg.IsWindow())
	{
		m_WndForwardPostMsg.DestroyWindow();
	}
	m_skinMgr.Destroy();

	m_pUIEditor = NULL;

    m_animate.UnInit();

	SAFE_DELETE(m_pGifTimerMgr);

    Image::ReleaseGDIPlus();

//     if (m_bGpuEnable)
//     {
//         ShutdownGpuCompositor();
//     }

    //	::CoUninitialize(); // do not call CoInitialize, CoInitializeEx, or CoUninitialize from the DllMain function. 
	OleUninitialize();
}

SkinManager& UIApplication::GetSkinMgr()
{
	return m_skinMgr;
}

ITopWindowManager* UIApplication::GetITopWindowMgr()
{
	return m_TopWindowMgr.GetITopWindowManager();
}

UIA::IAnimateManager* UIApplication::GetAnimateMgr()
{
    return m_animate.GetAnimateManager();
}

SkinRes* UIApplication::GetDefaultSkinRes()
{
	return m_skinMgr.GetDefaultSkinRes();
}

ImageManager*  UIApplication::GetActiveSkinImageMgr()
{
	SkinRes* pSkinRes = GetDefaultSkinRes();
	if (NULL == pSkinRes)
		return NULL;

	return &pSkinRes->GetImageManager();
}
ImageRes*  UIApplication::GetActiveSkinImageRes()
{
	ImageManager* pImageMgr = GetActiveSkinImageMgr();
	if (NULL == pImageMgr)
		return NULL;

	return &pImageMgr->GetImageRes();
}
CursorRes*  UIApplication::GetActiveSkinCursorRes()
{
	ImageManager* pImageMgr =GetActiveSkinImageMgr();
	if (NULL == pImageMgr)
		return NULL;

	return &pImageMgr->GetCursorRes();
}
GifRes*  UIApplication::GetActiveSkinGifRes()
{
	ImageManager* pImageMgr = GetActiveSkinImageMgr();
	if (NULL == pImageMgr)
		return NULL;

	return &pImageMgr->GetGifRes();
}
FontManager*  UIApplication::GetActiveSkinFontMgr()
{
	SkinRes* pSkinRes = GetDefaultSkinRes();
	if (NULL == pSkinRes)
		return NULL;

	return &pSkinRes->GetFontManager();
}
FontRes*  UIApplication::GetActiveSkinFontRes()
{
	FontManager* pFontMgr = GetActiveSkinFontMgr();
	if (NULL == pFontMgr)
		return NULL;

	return &pFontMgr->GetFontRes();
}
ColorManager*  UIApplication::GetActiveSkinColorMgr()
{
	SkinRes* pSkinRes = GetDefaultSkinRes();
	if (NULL == pSkinRes)
		return NULL;

	return &pSkinRes->GetColorManager();
}

ColorRes*  UIApplication::GetActiveSkinColorRes()
{
	ColorManager* pColorMgr = GetActiveSkinColorMgr();
	if (NULL == pColorMgr)
		return NULL;

	return &pColorMgr->GetColorRes();
}
StyleManager*  UIApplication::GetActiveSkinStyleMgr()
{
	SkinRes* pSkinRes = GetDefaultSkinRes();
	if (NULL == pSkinRes)
		return NULL;

	return &pSkinRes->GetStyleManager();
}

StyleRes*  UIApplication::GetActiveSkinStyleRes()
{
	StyleManager* pStyleMgr = GetActiveSkinStyleMgr();
	if (NULL == pStyleMgr)
		return NULL;

	return &pStyleMgr->GetStyleRes();
}

LayoutManager*  UIApplication::GetActiveSkinLayoutMgr()
{
	SkinRes* pSkinRes = GetDefaultSkinRes();
	if (NULL == pSkinRes)
		return NULL;

	return &pSkinRes->GetLayoutManager();
}

//
//	һ���յĴ��ڹ��̣���ΪUI���������Ĵ��ڹ�������Ҫ���޸ĳ�Ϊһ����ĳ�Ա������
//  �������Ĵ��ڹ���ֻ���������WNDCLASS������
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return ::DefWindowProc( hWnd, message, wParam, lParam );
}

/*
**	[private] void  RegisterWndClass()
**
**	Parameters
**
**	Return
**		NA
**
**	Remark
**		Window����Ĵ���ʹ�õĴ��������������ע��ġ�
**
**	See Also
*/
void UIApplication::RegisterWndClass()
{
	WNDCLASSEX wcex;
	
	// ע��UI��ͨ������
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= /*CS_HREDRAW | CS_VREDRAW |*/CS_DBLCLKS;  // <-- �ֲ㴰�����ʱ�ղ���redraw��Ϣ�����ֱ����OnSize��ˢ��
	wcex.lpfnWndProc	= WindowBase::StartWindowProc;//WndProc;   ����windowsĬ�ϵĴ��ڹ���
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= 0;//::AfxGetInstanceHandle();;
	wcex.hIcon			= 0;//LoadIcon(0, MAKEINTRESOURCE(IDR_MAINFRAME));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1); // 2014.5.21 ��������ΪNULL�����aeroĥɰ���ڴ���С����ԭʱ��������ʾ��ɫ��������ˢ�����ݵ���˸���⣨������WM_NCCALCSIZE�ڲ����ƻ���COPY������
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= WND_CLASS_NAME;
	wcex.hIconSm		= 0;//LoadIcon(0, MAKEINTRESOURCE(IDR_MAINFRAME));
	RegisterClassEx(&wcex);

	// ע������ʵ�ֶ����Ĵ�����
	wcex.lpszClassName  = WND_ANIMATE_CLASS_NAME;
	wcex.lpfnWndProc    = ::DefWindowProc;
	RegisterClassEx(&wcex);


	// ע������ʵ�ֲ˵�������ʽ�б��Ĵ���(����Ӱ/������Ӱ)
	wcex.lpszClassName  = WND_POPUP_CONTROL_SHADOW_NAME;
	wcex.style          = CS_DROPSHADOW;
	wcex.lpfnWndProc    = WindowBase::StartWindowProc;;
	RegisterClassEx(&wcex);

	wcex.lpszClassName  = WND_POPUP_CONTROL_NAME;
	wcex.style          = 0;
	wcex.lpfnWndProc    = WindowBase::StartWindowProc;;
	RegisterClassEx(&wcex);

	// ��קʱ��ʾ��ͼƬ����
	wcex.lpszClassName  = WND_DRAGBITMAPWND_CLASS_NAME;
	wcex.style          = 0;
	wcex.lpfnWndProc    = DefWindowProc;
	RegisterClassEx(&wcex);

}

bool UIApplication::IsUnderXpOS()
{
	bool bUnderXpOs = true;;
	if (VER_PLATFORM_WIN32_NT == m_osvi.dwPlatformId)
	{
		if (m_osvi.dwMajorVersion >= 6)
		{
			bUnderXpOs = false;
		}
	}
	else
	{
		bUnderXpOs = false;
	}
	return bUnderXpOs;
}

bool UIApplication::IsVistaOrWin7etc()
{
	bool bHighThanVista = true;;
	if (VER_PLATFORM_WIN32_NT == m_osvi.dwPlatformId)
	{
		if (m_osvi.dwMajorVersion < 6)
		{
			bHighThanVista = false;
		}
	}
	else
	{
		bHighThanVista = false;
	}
	return bHighThanVista;
}

bool  UIApplication::GetSkinTagParseFunc(LPCTSTR szTag, pfnParseSkinTag* pFunc)
{
    if (NULL == szTag || NULL == pFunc)
        return false;

    UISKINTAGPARSE_DATA::iterator iter = m_mapSkinTagParseData.find(szTag);
    if (iter == m_mapSkinTagParseData.end())
        return false;

    *pFunc = iter->second;
    return true;
}

bool  UIApplication::RegisterControlTagParseFunc(LPCTSTR szTag, pfnParseControlTag func)
{
    if (NULL == szTag || NULL == func)
        return false;

    m_mapControlTagParseData[szTag] = func;
    return true;
}

bool  UIApplication::GetControlTagParseFunc(LPCTSTR szTag, pfnParseControlTag* pFunc)
{
    if (NULL == szTag || NULL == pFunc)
        return false;

    UICONTROLTAGPARSE_DATA::iterator iter = m_mapControlTagParseData.find(szTag);
    if (iter == m_mapControlTagParseData.end())
        return false;

    *pFunc = iter->second;
    return true;
}

//
//	Ϊ��ʵ��UI����Ĵ��������ַ������������Ӧ���ࣩ����app���б���������UI����Ĵ�����Ϣ��
//
//	ע: �������������UICreateObject�����ӳ���б��޷����������ʵ��һ��UI������������Щ
//      ���뽫��ӳ���б���Ϊ��̬���顣��������ʵ����һ��UI��ʱ����app��ע���䴴����Ϣ��
//

bool UIApplication::RegisterUIObject(IObjectDescription* pObjDesc)
{
	if (!pObjDesc)
		return false;
	
	int nSize = (int)m_vecUIObjectDesc.size();
	for (int i = 0; i < nSize; i++)
	{
		m_vecUIObjectDesc[i];
		if (m_vecUIObjectDesc[i] == pObjDesc)
		{
			UI_LOG_WARN(_T("register duplicate. name=%s"), 
                pObjDesc->GetTagName());
			return false;
		}
	}

	m_vecUIObjectDesc.push_back(pObjDesc);
	return true;
}


void  UIApplication::ClearRegisterUIObject()
{
#define vec_clear(type, var)                  \
    {                                         \
        type::iterator iter = var.begin();    \
        for ( ; iter != var.end(); iter++ )   \
            SAFE_DELETE(*iter);               \
        var.clear();                          \
    }

    m_vecUIObjectDesc.clear();

	m_renderBaseFactory.Clear();
	m_textRenderFactroy.Clear();
	m_layoutFactory.Clear();
}

void UIApplication::RegisterDefaultUIObject()
{
    RegisterUIObject(WindowDescription::Get());
    RegisterUIObject(CustomWindowDescription::Get());
    RegisterUIObject(HwndHostDescription::Get());
	RegisterUIObject(PanelDescription::Get());
//    RegisterUIObject(ScrollPanelDescription::Get());


    m_mapSkinTagParseData[XML_IMG] = ImageManager::UIParseImageTagCallback;
    m_mapSkinTagParseData[XML_COLOR] = ColorManager::UIParseColorTagCallback;
    m_mapSkinTagParseData[XML_FONT] = FontManager::UIParseFontTagCallback;
    m_mapSkinTagParseData[XML_STYLE] = StyleManager::UIParseStyleTagCallback;
    m_mapSkinTagParseData[XML_LAYOUT] = LayoutManager::UIParseLayoutTagCallback;
	m_mapSkinTagParseData[XML_LAYOUTCONFIG] = LayoutManager::UIParseLayoutConfigTagCallback;
    m_mapSkinTagParseData[XML_INCLUDE] = SkinParseEngine::UIParseIncludeTagCallback;
	m_mapSkinTagParseData[XML_I18N] = I18nManager::UIParseI18nTagCallback;

	m_renderBaseFactory.Init();
	m_textRenderFactroy.Init();
	m_layoutFactory.Init();
}

// ���ڱ༭���е����ؼ��������
void  UIApplication::RestoreRegisterUIObject()
{
    ClearRegisterUIObject();
    RegisterDefaultUIObject();
}

IObject* UIApplication::CreateUIObjectByName(LPCTSTR szXmlName, ISkinRes* pSkinRes)
{
	if (!szXmlName)
		return NULL;

    int nSize = (int)m_vecUIObjectDesc.size();
    for (int i = 0; i < nSize; i++)
    {
        if (0 == _tcscmp(szXmlName, m_vecUIObjectDesc[i]->GetTagName()))
        {
            IObject* p = NULL;
            m_vecUIObjectDesc[i]->CreateInstance(pSkinRes, (void**)&p);
            return p;
        }
    }

    UI_LOG_ERROR(_T("GetUICreateInstanceFuncPtr Failed. name=%s"), szXmlName);
	return NULL;
}

IObject* UIApplication::CreateUIObjectByClsid(REFCLSID clsid, ISkinRes* pSkinRes)
{
    int nSize = (int)m_vecUIObjectDesc.size();
    for (int i = 0; i < nSize; i++)
    {
        if (::IsEqualIID(clsid, m_vecUIObjectDesc[i]->GetGUID()))
        {
            IObject* p = NULL;
            m_vecUIObjectDesc[i]->CreateInstance(pSkinRes, (void**)&p);
            return p;
        }
    }

    UI_LOG_ERROR(_T("GetUICreateInstanceFuncPtr Failed."));
    return NULL;
}

BOOL UIApplication::IsDialogMessage(MSG* pMsg)
{
	if (NULL == pMsg)
		return FALSE;

	if((pMsg->message < WM_KEYFIRST || pMsg->message > WM_KEYLAST) &&
		(pMsg->message < (WM_MOUSEFIRST+1) || pMsg->message > WM_MOUSELAST))
		return FALSE;

//	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_TAB )
	{
		// ��ȡ�����Ϣ�Ĵ������ڵĶ��㴰�ڣ���Ϊ������������������㴰�ڽ��е�
        // 2013.6.20 ���ε���Щ���롣�����Ӵ�����UI���ڣ������㴰������ͨ���ڵ����
 		HWND hWndTop = pMsg->hwnd;
// 		while (1)
// 		{
// 			if (NULL == hWndTop)
// 				return FALSE;
// 
// 			LONG lStyle = ::GetWindowLong(hWndTop, GWL_STYLE);
// 			if (lStyle & WS_CHILD)
// 			{
// 				hWndTop = ::GetParent(hWndTop);
// 			}
// 			else
// 			{
// 				break;
// 			}
// 		}

		// �ж���������Ƿ�����UI�����һ�����㴰��
		IWindowBase* pIWindow = m_TopWindowMgr.GetWindowBase(hWndTop);
        if (NULL == pIWindow)
            return FALSE;

        WindowBase* pWindow = pIWindow->GetImpl();
        if (!pWindow)
            return FALSE;

        return pWindow->GetMouseMgr()->IsDialogMessage(pMsg);
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
//
// http://dsdm.bokee.com/6033955.html  �����ȷʹ��PeekMessage
// ����Ҫע����Ƕ���ģ̬�����⣬��α�֤�����˳�
//
//////////////////////////////////////////////////////////////////////////
// 2014.2.27 libo
// ע�⣬��Ҫʹ��MsgWaitForMultipleObjects -- QS_ALLEVENTS��־�������ϵͳ�����������簲װ�˵ײ���̹���ʱ��һ����Ϳ���
//
// HHOOK  g_hKeyBoardHook = NULL;
// LRESULT CALLBACK  LLKeyboardProc(int code, WPARAM wParam, LPARAM lParam)
// {
//     return 0;
//     return CallNextHookEx(g_hKeyBoardHook, code, wParam, lParam);
// }
// hKeyBoardHook = ::SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboardProc, g_hInstance, NULL);
// 
//////////////////////////////////////////////////////////////////////////


//
//  WaitMessage ʹ����֪��
//  A pretty hard rule for WaitMessage() is that the message queue should be empty before you call it. 
//  If it isn't empty then any messages left in the queue are marked as "seen" and WaitMessage() ignores them.
// 
//  ���û���µ���Ϣ��WaitMessage���᷵�ء�PeekMessage/GetMessage�Ⱥ����Ὣ����
//  ��Ϣ�����е���Ϣ��ʶΪ����Ϣ�����
//  PostMessage(xxx);
//  PeekMessage(PM_NOREMOVE);
//  WaitMessage();  ���ﲢ�����������أ�xxx��Ϣ�ѱ���ʶΪ����Ϣ��������һ���µ�
//  ��Ϣ��������ʹWaitMessage���ء�
//
void  UIApplication::MsgHandleLoop(bool* pbQuitLoopRef)
{
	DWORD    dwRet = 0;
    DWORD&   nCount = m_WaitForHandlesMgr.m_nHandleCount;
    HANDLE*& pHandles = m_WaitForHandlesMgr.m_pHandles;
	MSG      msg;

    // �ᴫ��pbQuitLoopRef�����ģ��п�����Modal���͵Ĳ˵��������������Ҫ����������ж�
    // ��˵�����һ����֧���Ż�
    if (pbQuitLoopRef)
    {
        bool   bExit = false;
        bool&  bQuitRef = (pbQuitLoopRef==NULL ? bExit : *pbQuitLoopRef);
	    while (false == bQuitRef)
	    {
		    dwRet = ::MsgWaitForMultipleObjects(nCount, pHandles, FALSE, INFINITE, QS_ALLINPUT) - WAIT_OBJECT_0;
		    if (nCount > 0 && dwRet < nCount)
		    {
			    m_WaitForHandlesMgr.Do(pHandles[dwRet]);
		    }
		    else
		    {
			    while (::PeekMessage(&msg, NULL, 0,0, PM_NOREMOVE))
			    {
				    if (WM_QUIT == msg.message)  // һֱ���ݸ������Loop���������ڲ�Loop�н�����Ϣ�Ե���
					    return;

                    if (m_MsgFilterMgr.PreTranslateMessage(&msg))  // �п�����PreTranslateMessage���޸���pbQuitLoopRef
                    {
                        PeekMessage(&msg, NULL, 0,0, PM_REMOVE);   // ����Ϣ�ѱ�������ˣ��Ƴ�
                        continue;
                    }

                    if (bQuitRef)     // ���ܵĳ����ǵ���ģ̬�˵���Ȼ���������ڵĹرհ�ť�ϵ���һ�£�Ȼ��˵���PopupMenuWindow::PreTranslateMessage��Ҫ���˳���
                        return;  // �����ʱ�����������ͻᵼ�������ڱ����٣���ǰ���ڵ����˵��Ķ�ջ����Ҳ��������

                    PeekMessage(&msg, NULL, 0,0, PM_REMOVE);
                    if (FALSE == this->IsDialogMessage(&msg))
                    {
                        ::TranslateMessage(&msg);
                        ::DispatchMessage(&msg);
                    }

                    if (bQuitRef)
                        return;
			    }
		    }
	    }
    }
    else
    {
        while (1)
        {
            dwRet = ::MsgWaitForMultipleObjects(nCount, pHandles, FALSE, INFINITE, QS_ALLINPUT) - WAIT_OBJECT_0;
            if (nCount > 0 && dwRet < nCount)
            {
                m_WaitForHandlesMgr.Do(pHandles[dwRet]);
            }
            else
            {
                while (::PeekMessage(&msg, NULL, 0,0, PM_REMOVE))
                {
                    if (WM_QUIT == msg.message)  
                    {
                        ::PostMessage(NULL, WM_QUIT, 0, 0);  // һֱ���ݸ������Loop���������ڲ�Loop�н�����Ϣ�Ե���
                        return;
                    }

                    if (FALSE == m_MsgFilterMgr.PreTranslateMessage(&msg) && 
                        FALSE == this->IsDialogMessage(&msg)) 
                    {
                        ::TranslateMessage(&msg);                                      
                        ::DispatchMessage(&msg);
                    }
                }
            }
        }
    }
	return;
}


bool  UIApplication::ShowToolTip(TOOLTIPITEM* pItem)
{
	bool bRet = m_ToolTipMgr.Show(pItem);
    return bRet;
}
void  UIApplication::HideToolTip()
{
	m_ToolTipMgr.Hide();
}

HWND  UIApplication::GetForwardPostMessageWnd()
{
	return m_WndForwardPostMsg.m_hWnd;
}

void UIApplication::LoadUIObjectListToToolBox()
{
    if (!m_pUIEditor)
        return;

    UIOBJ_CREATE_DATA::iterator iter = m_vecUIObjectDesc.begin();
    for (; iter != m_vecUIObjectDesc.end(); iter++)
    {
        m_pUIEditor->OnToolBox_AddObject(
            (*iter)->GetTagName(), 
            (*iter)->GetCategory(), 
            (*iter)->GetMajorType());
    }
}

// ����UI3D.dll
HMODULE  UIApplication::GetUID2DModule()
{
#if 0
    if (s_hUID2D)
        return s_hUID2D;

    TCHAR  szPath[MAX_PATH] = {0};
    Util::GetAppPath_(g_hInstance, szPath);
    _tcscat(szPath, _T("UI3D.dll"));

    s_hUID2D = LoadLibrary(szPath);

    if (!s_hUID2D)
    {
        UI_LOG_INFO(_T("%s Load UI3D.dll Failed. Error code = %d"), FUNC_NAME, GetLastError());
    }
    else
    {
        funcUI3D_InitD2D pFunc = (funcUI3D_InitD2D)GetProcAddress(s_hUID2D, NAME_UI3D_InitD2D);
        if (pFunc)
            pFunc(m_pUIApplication);
        else
            UIASSERT(0);
    }

	return s_hUID2D;
#endif
	return 0;
}

HMODULE  UIApplication::GetUID3DModule()
{
#if 0
	if (s_hUID3D)
		return s_hUID3D;

	TCHAR  szPath[MAX_PATH] = {0};
	Util::GetAppPath_(g_hInstance, szPath);
	_tcscat(szPath, _T("UI3D.dll"));

	s_hUID3D = LoadLibrary(szPath);

	if (!s_hUID3D)
	{
		UI_LOG_INFO(_T("%s Load UI3D.dll Failed. Error code = %d"), FUNC_NAME, GetLastError());
	}
	else
	{
		funcUI3D_InitD3D pFunc = (funcUI3D_InitD3D)GetProcAddress(s_hUID3D, NAME_UI3D_InitD3D);
		if (pFunc)
			pFunc(m_pUIApplication);
		else
			UIASSERT(0);
	}

	return s_hUID3D;
#endif
	return 0;
}

bool  UIApplication::IsGpuCompositeEnable() 
{ 
    return m_bGpuEnable;    
}
bool  UIApplication::EnableGpuComposite()
{
	UIASSERT(0);
#if 0
    if (m_bGpuEnable)
        return true;

    if (IsVistaOrWin7etc() && 0 == StartupGpuCompositor())
    {
        UI_LOG_INFO(TEXT("GpuCompositor Enable."));

        m_bGpuEnable = true;
        return true;
    }
    else
    {
        UI_LOG_INFO(TEXT("GpuCompositor Startup Failed."));
    }
#endif

    return false;
}