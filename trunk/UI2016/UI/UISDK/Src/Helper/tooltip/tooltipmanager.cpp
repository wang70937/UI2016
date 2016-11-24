#include "stdafx.h"
#include "tooltipmanager.h"
#include "Src\Atl\image.h"
#include "Inc\Interface\iobject.h"
#include "Src\Base\Application\uiapplication.h"
#include "Inc\Interface\iuiapplication.h"

#pragma region //CSystemTooltip
class CSystemTooltip : public IToolTipUI
{
public:
	CSystemTooltip() 
	{
		m_hToolTip = NULL;
		memset(&m_toolinfo, 0, sizeof(TOOLINFO));

		m_pUIApp = NULL;
	}
	~CSystemTooltip()
	{
		m_pUIApp = NULL;  // 不维护uiapp的引用计数
	}

	virtual bool  Create()
	{
		UIASSERT( m_hToolTip == NULL );

		m_hToolTip = CreateWindowEx(WS_EX_TOPMOST|WS_EX_TRANSPARENT,
						TOOLTIPS_CLASS,
						NULL,
						WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,		
						CW_USEDEFAULT,
						CW_USEDEFAULT,
						CW_USEDEFAULT,
						CW_USEDEFAULT,
						NULL,
						NULL,
						NULL,
						NULL
						);

		/*
			如果#define _WIN32_WINNT 0x0500，那么tooltip一切正常
			当有一天#define _WIN32_WINNT 0x0501了，这时设置tooltip.cbSize = sizeof(TOOLINFO)的时候就不显示了。
			编译调试不显示任何错误，但是tooltip就是显示不出来了
			因为系统默认加载comctl 5.82，这个版本里面的tooltip的size根本没有sizeof(TOOLINFO),
			里面没有void *lpReserved;所以cbSize设大了，出错了。
			这时需要写成tooltip.cbSize = TTTOOLINFOA_V2_SIZE;
			或者在exe工程中强制指定comctl 6.0（在dll中指定无效）
			#pragma comment(linker, "\"/manifestdependency:type='Win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='X86' publicKeyToken='6595b64144ccf1df' language='*'\"")
			
			指定#define _WIN32_WINNT 0x0501将导致程序只能运行在xp及以上系统。
		*/
		m_toolinfo.cbSize = /*sizeof(TOOLINFO)*/ TTTOOLINFOA_V2_SIZE;
		m_toolinfo.uFlags = 0/* | TTF_TRACK | TTF_ABSOLUTE*/; // 注：加上TTF_TRACK|TTF_ABSOLUTE之后将导致提示条失去显示在屏幕范围之内的功能
		m_toolinfo.hwnd   = NULL;
		m_toolinfo.uId    = (UINT)0;
		m_toolinfo.hinst  = NULL;
		m_toolinfo.lpszText  = _T("leeihcy")/*LPSTR_TEXTCALLBACK*/;
		m_toolinfo.rect.left = m_toolinfo.rect.top = m_toolinfo.rect.bottom = m_toolinfo.rect.right = 0; 

		::SendMessage(m_hToolTip, TTM_ADDTOOL, 0, (LPARAM)&m_toolinfo);
		::SendMessage(m_hToolTip, TTM_SETMAXTIPWIDTH, 0, TOOLTIP_MAX_WIDTH);   // 备注：该属性如果不和6.0控件一起使用的话，在碰到一个很长的单词时，将无视max width，仅显示一行(仅win7下有效)。

		// Deleted. 该设置仅对默认情况下的宋体字体有效，其它字体没有作用。
		// 因此决定自己重新实现一个提示条。摆脱系统的限制！

		// 解决非6.0控件时，提示条内容不居中的问题。（测试发现在6.0控件下无视margin的值，但正好6.0就是居中的，可以不管）
		// 英文操作系统下面是OK的，不用修改。
		// win7和xp下面需要调整的参数不一致。
// 		if (936 == GetACP())
// 		{
// 			if (m_bUnderXpOs)
// 			{
// 				RECT rc = {1,3,0,0};
// 				::SendMessage(this->m_hToolTip, TTM_SETMARGIN, 0, (LPARAM)&rc);
// 			}
// 			else
// 			{
// 				RECT rc = {2,3,0,0};
// 				::SendMessage(this->m_hToolTip, TTM_SETMARGIN, 0, (LPARAM)&rc);
// 			}
// 		}

#if 0
		LPCTSTR szFontFace =  /*_T("宋体");*/_T("微软雅黑");
		// 设置字体为 微软雅黑
		if ( Util::IsFontExist(szFontFace) )
		{
			m_hFont = ::CreateFont(
				14,//18,                        // nHeight
				0,                         // nWidth
				0,                         // nEscapement
				0,                         // nOrientation
				FW_NORMAL,                 // nWeight
				FALSE,                     // bItalic
				FALSE,                     // bUnderline
				0,                         // cStrikeOut
				ANSI_CHARSET,              // nCharSet
				OUT_DEFAULT_PRECIS,        // nOutPrecision
				CLIP_DEFAULT_PRECIS,       // nClipPrecision
				DEFAULT_QUALITY,           // nQuality
				DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
				szFontFace);               // lpszFacename

			if (m_hFont)
			{
				::SendMessage(m_hToolTip, WM_SETFONT, (WPARAM)m_hFont, 0);

				RECT rcMargin;
				::SendMessage(m_hToolTip, TTM_GETMARGIN, 0, (LPARAM)(LPRECT)&rcMargin);
				int a= 0;
			}
		}
#endif
		return true;
	}
	virtual bool  Destroy()
	{
		if( NULL != m_hToolTip )
		{
			::DestroyWindow(m_hToolTip);
			m_hToolTip = NULL;
		}
		delete this;
		return true;
	}
	virtual bool  SetText(LPCTSTR szText)
	{
		if (NULL == m_hToolTip)
		{
			Create();
		}

		String strTooltipText;
		FixStringWordBreakUnderXP(szText, strTooltipText);

		m_toolinfo.lpszText = (TCHAR*)strTooltipText.c_str();
		::SendMessage(m_hToolTip, TTM_UPDATETIPTEXTW, 0, (LPARAM)&m_toolinfo );
		return true;
	}
	virtual bool  SetTitle(LPCTSTR szText) 
	{
		if (NULL == m_hToolTip)
		{
			Create(); 
		}
		return true;
	}
	virtual bool  Show(HWND hWndParent)
	{
		if (NULL == m_hToolTip)
		{
			Create();
		}

		POINT pt;
		::GetCursorPos(&pt);
		::SendMessage(m_hToolTip, TTM_TRACKPOSITION, 0, MAKELPARAM(pt.x, pt.y+22));
		::SendMessage(m_hToolTip, TTM_TRACKACTIVATE, (WPARAM)TRUE, (LPARAM)&m_toolinfo );
		return true;
	}
	virtual bool  Hide()
	{
        if (m_hToolTip && IsWindowVisible(m_hToolTip))
        {
		    ::SendMessage(this->m_hToolTip, TTM_TRACKACTIVATE, (WPARAM)FALSE, (LPARAM)&this->m_toolinfo );
        }

		// TODO: 清理其它信息，如标题、图标等，避免下次弹出来时，还显示本次的数据
		return true;
	}

	virtual void  OnSerialize(SERIALIZEDATA* pData)
	{

	}
	
	virtual bool  SetUIApplication(IUIApplication* p)
	{
        // 尽量少维护UIApp的引用计数
		m_pUIApp = p;
		return true;
	}
protected:
	//
	// 解决 tooltip一个长单词在xp下面不会按照max width自动分行的bug
	//
	void    FixStringWordBreakUnderXP(const String& src, String& strOut)
	{
		// 只在XP下处理该字符串
		if (m_pUIApp && false == m_pUIApp->IsUnderXpOS())
		{
			strOut = src;
			return ;
		}

		HFONT hFont = (HFONT)SendMessage(m_hToolTip, UI_MSG_GETRENDERFONT, 0,0);
		HDC   hDC   = Image::GetCacheDC();
		HFONT hOldFont = (HFONT)SelectObject(hDC, hFont);
		LPCTSTR szText = src.c_str();

		int   nStart  = 0;
		int   nLength = src.length(); 
		RECT  rcLimit = {0,0, TOOLTIP_MAX_WIDTH, 1};  // 将高度设置为1，保证DrawTextEx只计算第一行文本的字符数
		UINT  nDrawTextFlag = DT_EDITCONTROL|DT_WORDBREAK/*|DT_NOFULLWIDTHCHARBREAK*/;

		DRAWTEXTPARAMS  param;
		ZeroMemory(&param, sizeof(DRAWTEXTPARAMS));
		param.cbSize = sizeof(DRAWTEXTPARAMS);

		while (nStart < nLength)
		{
			// 计算一行文本中的字符数
			param.uiLengthDrawn = 0;
			DrawTextEx(hDC, (TCHAR*)(szText+nStart), nLength-nStart, &rcLimit, nDrawTextFlag, &param);

			strOut.append(szText+nStart, param.uiLengthDrawn);
			nStart += param.uiLengthDrawn;

			// 手动添加换行符
			TCHAR cLast = szText[nStart-1];
			if (cLast != _T('\r') && cLast != _T('\n') && nStart < nLength)
			{
				strOut.append(_T("\n"));
			}
		}

		SelectObject(hDC, hOldFont);
		Image::ReleaseCacheDC(hDC);
	}

protected:
	HWND       m_hToolTip;
	TOOLINFO   m_toolinfo;
	bool       m_bUnderXpOs;  // 是否是XP系统。区别于WIN7，两者对于tooltip的显示有些区别
	IUIApplication*  m_pUIApp;
};
#pragma endregion
//////////////////////////////////////////////////////////////////////////

ToolTipManager::ToolTipManager()
{
	m_pToolTipUI = NULL;
    m_pUIApplication = NULL;
}

ToolTipManager::~ToolTipManager()
{
	this->Release();
}

void ToolTipManager::Init(UIApplication* p, IToolTipUI* pTooltipUI)
{
    m_pUIApplication = p;

	if (m_pToolTipUI)
	{
		m_pToolTipUI->Destroy();
	}

	if (NULL == pTooltipUI)
		m_pToolTipUI = new CSystemTooltip;	
	else
		m_pToolTipUI = pTooltipUI;

    m_pToolTipUI->SetUIApplication(m_pUIApplication->GetIUIApplication());
    m_timeout.Init(this);
}

void ToolTipManager::Release()
{
	if (m_pToolTipUI)
	{
		m_pToolTipUI->Destroy();
		m_pToolTipUI = NULL;
	}
}

bool ToolTipManager::Show(TOOLTIPITEM* pItemInfo)
{
 	if (NULL == m_pToolTipUI || NULL == pItemInfo)
		return false;

	Hide();
	m_tooltipItem.Set(pItemInfo);

	switch (pItemInfo->nToolTipFlag)
	{
	case TOOLTIP_ACTION_FLAG_NORMAL:
		{
			m_timeout.SetParam(TIMEOUT_DELAY_SHOW);
            m_timeout.Start(TOOLTIP_DELAY_SHOW_TIME);

            if (m_tooltipItem.pNotifyObj)
                m_tooltipItem.pNotifyObj->AddDelayRef((void**)&m_tooltipItem.pNotifyObj);
		}
		break;

	}
	return true;
}

bool ToolTipManager::Hide()
{
    m_timeout.Cancel();

	if (m_pToolTipUI)
	{
		m_pToolTipUI->Hide();
	}
    
    if (m_tooltipItem.pNotifyObj)
        m_tooltipItem.pNotifyObj->RemoveDelayRef((void**)&m_tooltipItem.pNotifyObj);

    m_tooltipItem.Reset();
	return true;
}

void ToolTipManager::OnTimeout(long lId, WPARAM wParam, LPARAM lParam)
{
	if (NULL == m_pToolTipUI)
		return;

	if (lId == TIMEOUT_AUTO_HIDE)
	{
		Hide();
		return;
	}

    // 直接模式
    if (!m_tooltipItem.strContent.empty())
    {
        UIASSERT(NULL == m_tooltipItem.pNotifyObj);

        m_pToolTipUI->SetText(m_tooltipItem.strContent.c_str());
        m_pToolTipUI->Show(NULL);
        m_tooltipItem.Reset();

		m_timeout.SetParam(TIMEOUT_AUTO_HIDE);
		m_timeout.Start(TOOLTIP_AUTO_HIDE_TIME);
        return;
    }

    // 有可能对象被删除了
    if (!m_tooltipItem.pNotifyObj)
        return;

    m_tooltipItem.pNotifyObj->RemoveDelayRef((void**)&m_tooltipItem.pNotifyObj);    
	if (0 == UISendMessage(m_tooltipItem.pNotifyObj, UI_MSG_SHOW_TOOLTIP))
	{
        if (UISendMessage(
                m_tooltipItem.pNotifyObj, 
                UI_MSG_GET_TOOLTIPINFO, 
                (WPARAM)&m_tooltipItem,
                (LPARAM)m_pToolTipUI)
                )
        {
		    m_pToolTipUI->Show(NULL);

			m_timeout.SetParam(TIMEOUT_AUTO_HIDE);
			m_timeout.Start(TOOLTIP_AUTO_HIDE_TIME);
        }
	}
    m_tooltipItem.Reset();
}


ToolTipItem::ToolTipItem() 
{ 
    Reset();
}
void  ToolTipItem::Reset()
{
    pNotifyObj = NULL;
    pItemData = NULL;
    nToolTipFlag = 0;
    strContent.clear();
}
void  ToolTipItem::Set(TOOLTIPITEM* pItem)
{
    Reset();

    if (!pItem)
        return;

    if (pItem->szToolTipContent)
    {
        strContent = pItem->szToolTipContent;
    }
    else
    {
        pNotifyObj = pItem->pNotifyObj;
        pItemData = pItem->pItemData;
        nToolTipFlag = pItem->nToolTipFlag;
    }
}