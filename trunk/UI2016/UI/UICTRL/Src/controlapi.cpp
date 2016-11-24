#include "stdafx.h"
#include "Control\Button\button_desc.h"
#include "Control\Label\label_desc.h"
#include "Control\Menu\menu_desc.h"
#include "Control\Menu\menu.h"
#include "Control\Edit\edit_desc.h"
#include "Control\RadioButton\radiobutton_desc.h"
#include "Control\CheckButton\checkbutton_desc.h"
#include "Control\ListBox\listbox_desc.h"
#include "Control\ScrollBar\scrollbar_desc.h"
#include "Control\ComboBox\combobox_desc.h"
#include "Control\HyperLink\hyperlink_desc.h"
#include "Control\RichText\richtext_desc.h"
#include "Control\TreeView\treeview_desc.h"

namespace UI
{
    extern "C"
    __declspec(dllexport) bool RegisterUIObject(IUIApplication* p)
    {
		p->RegisterUIObject(ButtonDescription::Get());
        p->RegisterUIObject(SystemButtonDescription::Get());
		p->RegisterUIObject(LabelDescription::Get());
		p->RegisterUIObject(PictureCtrlDescription::Get());
		p->RegisterUIObject(GifCtrlDescription::Get());
		p->RegisterUIObject(EditDescription::Get());
		p->RegisterUIObject(InstantEditDescription::Get());
		p->RegisterUIObject(PasswordEditDescription::Get());
		p->RegisterUIObject(RadioButtonDescription::Get());
		p->RegisterUIObject(CheckButtonDescription::Get());
		p->RegisterUIObject(ListBoxDescription::Get());
		p->RegisterUIObject(SystemVScrollBarDescription::Get());
		p->RegisterUIObject(SystemHScrollBarDescription::Get());
		p->RegisterUIObject(ComboBoxDescription::Get());
        p->RegisterUIObject(HyperLinkDescription::Get());
        p->RegisterUIObject(RichTextDescription::Get());
        p->RegisterUIObject(TreeViewDescription::Get());
		
		// p->RegisterUIObject(MenuDescription::Get());
		p->RegisterControlTagParseFunc(_T("Menu"), Menu::UIParseMenuTag);

        return true;
    }
}

extern "C"
bool UICTRL_RegisterUIObject(IUIApplication* p)
{
	return RegisterUIObject(p);
}

// 
// extern "C"
// bool UICreateTrayIcon(IUIApplication* pUIApplication, ITrayIcon** ppOut)
// {
// 	if (NULL == ppOut)
// 		return false;
// 
// 	TrayIcon* p = new TrayIcon(pUIApplication);
// 	*ppOut = static_cast<ITrayIcon*>(p);
// 	p->AddRef();
// 
// 	return true;
// }

// ���һ�������Ƿ��£���CTRL,SHIFT,ALT
bool IsKeyDown(UINT vk)
{
	return ((GetKeyState(vk) & 0x8000) != 0);  // ���� ( GetKeyState( VK_CONTROL ) < 0 ) ??
}


// ����֧��Getʱ����һ��LPCTSTR��ʱ����
#define GlobalTempBufferSize  256
TCHAR   g_szGlobalTempBuffer[GlobalTempBufferSize];
String  g_strGlobalTempBuffer;

TCHAR*  GetTempBuffer(int nMaxSize)
{
    UIASSERT(nMaxSize < GlobalTempBufferSize);
    memset(g_szGlobalTempBuffer, 0, sizeof(g_szGlobalTempBuffer));
    return g_szGlobalTempBuffer;
}
String&  GetTempBufferString()
{
    g_strGlobalTempBuffer.clear();
    return g_strGlobalTempBuffer;
}
