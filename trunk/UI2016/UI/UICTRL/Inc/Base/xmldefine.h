#ifndef _UI_CTRL_XMLDEFINE_H_
#define _UI_CTRL_XMLDEFINE_H_

namespace UI
{

// UICtrl�е�xml���Զ���

#define XML_RENDER_TYPE_THEME_HSCROLLBAR_THUMB_BUTTON       TEXT("theme.hscrollbar.thumb")
#define XML_RENDER_TYPE_THEME_VSCROLLBAR_THUMB_BUTTON       TEXT("theme.vscrollbar.thumb")
#define XML_RENDER_TYPE_THEME_SCROLLBAR_LINE_RIGHT_BUTTON   TEXT("theme.hscrollbar.right")
#define XML_RENDER_TYPE_THEME_SCROLLBAR_LINE_LEFT_BUTTON    TEXT("theme.hscrollbar.left")
#define XML_RENDER_TYPE_THEME_SCROLLBAR_LINE_UP_BUTTON      TEXT("theme.vscrollbar.up")
#define XML_RENDER_TYPE_THEME_SCROLLBAR_LINE_DOWN_BUTTON    TEXT("theme.vscrollbar.down")
#define XML_RENDER_TYPE_THEME_VSCROLLBARBACKGND             TEXT("theme.vscrollbar.bkg")
#define XML_RENDER_TYPE_THEME_HSCROLLBARBACKGND             TEXT("theme.hscrollbar.bkg")
#define XML_RENDER_TYPE_THEME_SCROLLBAR_SIZEBOX             TEXT("theme.scrollbar.sizebox")

#define XML_RENDER_TYPE_THEME_BUTTON_BKGND                  TEXT("theme.button")
#define XML_RENDER_TYPE_THEME_RADIO_BUTTON                  TEXT("theme.radiobutton")
#define XML_RENDER_TYPE_THEME_CHECK_BUTTON                  TEXT("theme.checkbutton")
#define XML_RENDER_TYPE_THEME_EDIT_BKGND                    TEXT("theme.edit")
#define XML_RENDER_TYPE_THEME_GROUPBOX_BKGND                TEXT("theme.groupbox.bkg")
#define XML_RENDER_TYPE_THEME_SLIDER_BKG                    TEXT("theme.slider.bkg")
#define XML_RENDER_TYPE_THEME_SLIDER_BTN_BKG                TEXT("theme.slider.btn.bkg")
#define XML_RENDER_TYPE_THEME_PROGRESS_FOREGND              TEXT("theme.progress.foregnd")
#define XML_RENDER_TYPE_THEME_PROGRESS_BKGND                TEXT("theme.progress.bkgnd")
#define XML_RENDER_TYPE_THEME_COMBOBOX_BUTTON               TEXT("theme.combobox.btn")
#define XML_RENDER_TYPE_THEME_COMBOBOX_BKGND                TEXT("theme.combobox.bkg")
#define XML_RENDER_TYPE_THEME_LISTBOX_BKG                   TEXT("theme.listbox.bkg")
#define XML_RENDER_TYPE_THEME_TREEVIEW_EXPANDCOLLAPSE_ICON  TEXT("theme.treeview.expandcollpase.icon")
#define XML_RENDER_TYPE_THEME_LISTVIEW_ITEM                 TEXT("theme.listview.item")
#define XML_RENDER_TYPE_THEME_HEADER_ITEM                   TEXT("theme.header.item")
#define XML_RENDER_TYPE_THEME_HEADER_BKG                    TEXT("theme.header.bkg")
#define XML_RENDER_TYPE_THEME_TABCTRL_BUTTON_BKGND          TEXT("theme.tab.button")

#define XML_RENDER_TYPE_THEME_MENU_BKGND                    TEXT("theme.menu.bkg")
#define XML_RENDER_TYPE_THEME_MENUSEPERATOR                 TEXT("theme.menu.separator")
#define XML_RENDER_TYPE_THEME_MENUSTRINGITEM                TEXT("theme.menu.stringitem")
#define XML_RENDER_TYPE_THEME_MENUPOPUPTRIANGLE             TEXT("theme.menu.triangle")
#define XML_RENDER_TYPE_THEME_MENURADIOICON                 TEXT("theme.menu.radioicon")
#define XML_RENDER_TYPE_THEME_MENURADIOCHECKICONBK          TEXT("theme.menu.radiocheckicon.bkg")

enum THEME_RENDER_TYPE
{
	// RENDER_TYPE_THEME_FIRST = 1000,
	RENDER_TYPE_THEME_BUTTON_BKGND = 2001,
	RENDER_TYPE_THEME_SLIDER_TRACK_BUTTON,
	RENDER_TYPE_THEME_COMBOBOX_BUTTON,
	RENDER_TYPE_THEME_RADIO_BUTTON,
	RENDER_TYPE_THEME_CHECK_BUTTON,
	RENDER_TYPE_THEME_EDIT_BKGND,
	RENDER_TYPE_THEME_GROUPBOX_BKGND,
	RENDER_TYPE_THEME_COMBOBOX_BKGND,
	RENDER_TYPE_THEME_COMBOBOX_LISTBOX_BKGND,
	RENDER_TYPE_THEME_LISTBOX_BKGND,
	RENDER_TYPE_THEME_LISTVIEW_ITEM,
	RENDER_TYPE_THEME_LISTVIEW_FOCUS,
	RENDER_TYPE_THEME_TREEVIEW_EXPANDCOLLAPSE_ICON,

	RENDER_TYPE_THEME_MENU_BKGND,
	RENDER_TYPE_THEME_MENUSTRINGITEM,
	RENDER_TYPE_THEME_MENUSEPERATOR,
	RENDER_TYPE_THEME_MENUPOPUPTRIANGLE,
	RENDER_TYPE_THEME_MENUCHECKICON,
	RENDER_TYPE_THEME_MENURADIOICON,
	RENDER_TYPE_THEME_MENURADIOCHECKICONBK,

	RENDER_TYPE_THEME_HSCROLLBAR_THUMB_BUTTON,
	RENDER_TYPE_THEME_VSCROLLBAR_THUMB_BUTTON,
	RENDER_TYPE_THEME_SCROLLBAR_LINE_RIGHT_BUTTON,
	RENDER_TYPE_THEME_SCROLLBAR_LINE_LEFT_BUTTON,
	RENDER_TYPE_THEME_SCROLLBAR_LINE_UP_BUTTON,
	RENDER_TYPE_THEME_SCROLLBAR_LINE_DOWN_BUTTON,
	RENDER_TYPE_THEME_VSCROLLBARBACKGND,
	RENDER_TYPE_THEME_HSCROLLBARBACKGND,
	RENDER_TYPE_THEME_SCROLLBAR_SIZEBOX,

	RENDER_TYPE_THEME_PROGRESS_BKGND,
	RENDER_TYPE_THEME_PROGRESS_FOREGND,
	RENDER_TYPE_THEME_SLIDERCTRL_BKGND,
	HEADER_TYPE_THEME_HEADERCTRL_ITEM_BKGND,
	RENDER_TYPE_THEME_HEADERCTRL_RIGHT_BLANK,

	RENDER_TYPE_THEME_TABCTRL_BUTTON_BKGND,
};

#define XML_LISTBOX_TEXT_INDENT   _T("textindent")    // �ı�����
#pragma endregion

// Button

// ϵͳ��ť����
enum SYSTEM_BUTTON_TYPE
{
    SYSTEM_BUTTON_TYPE_UNKNOWN = 0,
    SYSTEM_BUTTON_TYPE_MINIMIZE = 1,
    SYSTEM_BUTTON_TYPE_MAXIMIZE = 2,
    SYSTEM_BUTTON_TYPE_RESTORE = 3,
    SYSTEM_BUTTON_TYPE_CLOSE = 4,
};
#define  XML_SYSTEM_BUTTON_TYPE_MINIMIZE  TEXT("minimize")
#define  XML_SYSTEM_BUTTON_TYPE_MAXIMIZE  TEXT("maximize")
#define  XML_SYSTEM_BUTTON_TYPE_RESTORE   TEXT("restore")
#define  XML_SYSTEM_BUTTON_TYPE_CLOSE     TEXT("close")

#define  XML_SYSTEM_BUTTON_TYPE           TEXT("type")

#pragma region  // SplitterBar���Զ���
#define XML_SPLITTERBAR_OBJECTS_POS  _T("type")
#define XML_SPLITTERBAR_LEFT_RIGHT   _T("left_right")    // ������ק�����ؼ����·ֲ�
#define XML_SPLITTERBAR_UP_DOWN      _T("up_down")

enum SPLITTED_OBJECTS_POS
{
    SPLITTERBAR_LEFT_RIGHT,
    SPLITTERBAR_TOP_BOTTOM
};

#define XML_SPLITTERBAR_ALIGN           _T("align")       // �����ڴ�С�ı�ʱ�����ĸ�������Ϊ��׼
#define XML_SPLITTERBAR_ALIGN_LEFT      _T("left")
#define XML_SPLITTERBAR_ALIGN_TOP       _T("top")
#define XML_SPLITTERBAR_ALIGN_RIGHT     _T("right")
#define XML_SPLITTERBAR_ALIGN_BOTTOM    _T("bottom")
#define XML_SPLITTERBAR_ALIGN_PERCENT   _T("percent")

enum SPLITTERBAR_ALIGN
{
    SPLITTERBAR_ALIGN_LEFT   = 0,
    SPLITTERBAR_ALIGN_TOP    = 0,
    SPLITTERBAR_ALIGN_RIGHT  = 1,
    SPLITTERBAR_ALIGN_BOTTOM = 1,
    SPLITTERBAR_ALIGN_PERCENT = 2,
};

#define XML_SPLITTERBAR_LEFT_PREFIX    _T("left.")
#define XML_SPLITTERBAR_RIGHT_PREFIX   _T("right.")
#define XML_SPLITTERBAR_TOP_PREFIX     _T("top.")
#define XML_SPLITTERBAR_BOTTOM_PREFIX  _T("bottom.")

#define XML_SPLITTERBAR_OBJ_ID       _T("id")       // �ָ������ݵĿؼ�Id
#define XML_SPLITTERBAR_OBJ_MINSIZE  _T("minsize")  // �ָ������ݵĿؼ���С����
#define XML_SPLITTERBAR_OBJ_MAXSIZE  _T("maxsize")  // �ָ������ݵĿؼ�������

// popup windwo
#define XML_POPUPWINDOW_PREFIX      _T("popupwnd.") // ����ʽ��������ǰ׺���˵���combobox�ؼ���ʹ��

// ie wrap
#define XML_IE_URL   _T("url")   // ��Ҫ�򿪵�url��ַ

// richedit
#define XML_RICHEDIT_MSG_BUBBLE_PREFIX        _T("msgbubble.")  // ��Ϣ���ݶ���
#define XML_RICHEDIT_MSG_BUBBLE_ENABLE        _T("enable")      // msgbubble.enable �Ƿ��������ݡ�ֻ�п����˸�������ڱ༭������������image
#define XML_RICHEDIT_MSG_BUBBLE_LEFT_PREFIX   _T("left.")       // ������ݶ���
#define XML_RICHEDIT_MSG_BUBBLE_RIGHT_PREFIX  _T("right.")      // �Ҳ����ݶ���
#define XML_RICHEDIT_MSG_BUBBLE_IMAGE         _T("image")       // ���ݱ���ͼƬ
#define XML_RICHEDIT_MSG_BUBBLE_IMAGE_9REGION _T("image.9region") // ����9������

#define XML_RICHEDIT_OLE_PREFIX               _T("reole.")  
#define XML_REOLE_PICTURE_PREFIX              _T("picture.")  
#define XML_REOLE_PICTURE_LOADING_IMAGE       _T("loading.image")
#define XML_REOLE_PICTURE_FAILURE_IMAGE       _T("failure.image")
#define XML_REOLE_MAGNIFIER_PREFIX            _T("magnifier.")
#define XML_REOLE_NORMAL                      _T("normal")
#define XML_REOLE_HOVER                       _T("hover")
#define XML_REOLE_PRESS                       _T("press")

// richtext
#define XML_RICHTEXT_EDITABLE                 _T("editable")      // �༭ģʽ
#define XML_RICHTEXT_EDITABLE_DISABLE         _T("disable")
#define XML_RICHTEXT_EDITABLE_READONLY        _T("readonly")
#define XML_RICHTEXT_EDITABLE_ENABLE          _T("enable")

#define XML_RICHTEXT_PAGE_ALIGN_HORZ          _T("page.halign")   // page���ݶ��뷽ʽ
#define XML_RICHTEXT_PAGE_ALIGN_VERT          _T("page.valign")


}
#endif //_UI_CTRL_XMLDEFINE_H_