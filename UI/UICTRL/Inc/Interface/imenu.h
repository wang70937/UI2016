#ifndef _UI_IMENU_H_
#define _UI_IMENU_H_
#include "..\..\..\UISDK\Inc\Interface\ilistitembase.h"
#include "..\..\..\UISDK\Inc\Interface\ilistctrlbase.h"

namespace UI
{

    //	����˵���
//		message: UI_WM_NOTIFY
//		code:    UI_MENU_CLICK 
//      wparam:  
//      lparam: 
//
#define UI_MENU_CLICK 132331849
    
// void OnMenuClick(UINT nMenuID);
#define UIMSG_MENU_CLICK(func)                        \
    if (uMsg == UI_MSG_NOTIFY  &&                     \
        code == UI_MENU_CLICK )                       \
    {                                                 \
        SetMsgHandled(TRUE);                          \
        func((UINT)wParam);                           \
        if (IsMsgHandled())                           \
            return TRUE;                              \
    }

#define MENU_ITEM_SEPARATOR_ID  (-1)

// ��ȡ�˵�ͼ������Ŀ��
// Return: long lWidth
#define UI_WM_MENU_GETICONGUTTERWIDTH     136111802

// ��ȡ�˵��Ҳ൯����ͷ�Ŀ��
// Return: long lWidth
#define UI_WM_MENU_GETPOPUPTRIANGLEWIDTH  136111803

#define UI_LISTITEM_TYPE_MENU_STRING      136111859
#define UI_LISTITEM_TYPE_MENU_SEPARATOR   136111900
#define UI_LISTITEM_TYPE_MENU_POPUP       136111901

enum TRACK_POPUP_MENU_FLAG
{
    UI_TPM_RETURNCMD = TPM_RETURNCMD,
    UI_TPM_CENTERALIGN = TPM_CENTERALIGN,
    UI_TPM_RIGHTALIGN = TPM_RIGHTALIGN,
    UI_TPM_VCENTERALIGN = TPM_VCENTERALIGN,
    UI_TPM_BOTTOMALIGN = TPM_BOTTOMALIGN,

    UI_TPM_SHOWACTIVATE = 0x80000000,  // �����㣬����֧�ֿ���̵�CEF�һ������˵� 
}; 

struct MENUSTYLE
{
	// ����������ʱ���Ƿ��Զ��ͷ��Ӳ˵��ڴ档��Ҫ���ڴ�xml��load�Ĳ˵�
	// ��Ϊ���ڲ��������Ӳ˵����ڲ����������Ӳ˵������ⲿ�Լ�create����
	bool  auto_release_submenu : 1;  
};

class MenuItemBase;
class MenuItem;
class Menu;
interface IMenu;
interface IWindow;

//////////////////////////////////////////////////////////////////////////
class MenuSeparatorItemShareData;
interface IMenuSeparatorItemShareData : public IListItemShareData
{
    UI_DECLARE_INTERFACE_ACROSSMODULE(MenuSeparatorItemShareData);
};
class MenuSeparatorItem;
interface IMenuSeparatorItem : public IListItemBase
{
    UI_DECLARE_INTERFACE_ACROSSMODULE(MenuSeparatorItem);
};

//////////////////////////////////////////////////////////////////////////
class MenuStringItemShareData;
interface IMenuStringItemShareData : public IListItemShareData
{
    UI_DECLARE_INTERFACE_ACROSSMODULE(MenuStringItemShareData);
};

class MenuStringItem;
interface IMenuStringItem : public IListItemBase
{
    UI_DECLARE_INTERFACE_ACROSSMODULE(MenuStringItem);
};
//////////////////////////////////////////////////////////////////////////
class MenuPopupItemShareData;
interface IMenuPopupItemShareData : public IListItemShareData
{
    UI_DECLARE_INTERFACE_ACROSSMODULE(MenuPopupItemShareData);
};
class MenuPopupItem;
interface IMenuPopupItem : public IListItemBase
{
    UI_DECLARE_INTERFACE_ACROSSMODULE(MenuPopupItem);
};
//////////////////////////////////////////////////////////////////////////

interface __declspec(uuid("DE6225F7-9DB1-4D13-A712-6553EF3B7556"))
UICTRLAPI IMenu : public IListCtrlBase
{
    void  OnNewChildElement(IUIElement* pUIElement);
    int   TrackPopupMenu(UINT nFlag, int x, int y, IMessage* pNotifyObj, HWND hWndClickFrom = NULL, RECT* prcClickFrom = NULL);
    IWindow*  CreateMenuWindow();
 
    IMenuStringItem*  AppendString(LPCTSTR szText, UINT nId);
    IMenuSeparatorItem*  AppendSeparator(UINT nId);
    IMenuPopupItem*  AppendPopup(LPCTSTR szText, UINT nId, IMenu* pSubMenu);

    IMenu*  GetSubMenuByPos(int nPos);
    IMenu*  GetSubMenuById(long lId);

	IWindow*  GetPopupWindow();
	int   GetIconGutterWidth();

	UI_DECLARE_INTERFACE_ACROSSMODULE(Menu);
};


struct LoadMenuData
{
	ISkinRes*  pSkinRes;
	/*__opt*/ LPCTSTR  szWndId;
	/*__opt*/ LPCTSTR  szMenuId;	
    HWND  hWndParent;
};
struct DefaultMenuData
{
	TCHAR  szDefaultWndId[32];
	TCHAR  szDefaultMenuId[32];
};


// �Զ�����delete_this,�ͷŲ˵�
class CMenuPtr
{
public:
	CMenuPtr(IMenu* pData = 0) : m_pData(pData)
	{
	}
	virtual ~CMenuPtr()
	{
		if (m_pData)
			m_pData->Release();
	}
	CMenuPtr& operator =(IMenu* p)
	{
		if (m_pData)
			m_pData->Release();
		m_pData = p;

		return *this;
	}
	IMenu* operator->() {
		return m_pData;
	}
	bool operator!() {
		return (m_pData == NULL);
	}
	operator IMenu*() {
		return m_pData;
	}

private:
	IMenu*  m_pData;
};

}

extern "C" UICTRLAPI void  UISetDefaultMenuData(UI::DefaultMenuData* pData);
extern "C" UICTRLAPI UI::IMenu*  UILoadMenu(UI::LoadMenuData* pData);
extern "C" UICTRLAPI bool  UIDestroyMenu(UI::IMenu* pMenu);

#endif // IMENU_H_31481C5E_BC0B_4872_9C54_FF8D06736CFB