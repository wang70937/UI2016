#ifndef _UI_IOBJECT_H_
#define _UI_IOBJECT_H_

namespace UI
{
	interface IWindowBase;
	interface ISkinRes;
	interface IObjectDescription;
	interface ITextRenderBase;
	interface IRenderFont;
	interface IRenderBase;
	interface ILayer;
	interface IMapAttribute;

	typedef struct tagObjStyle
	{
		bool  transparent : 1;
		bool  float_on_parent_content : 1;   // ��ˢ�¸ÿؼ�ʱ����������Ҫ�ػ游���󱳾�(wm_erasebkgbkgnd)����Ҫ�ػ游��������(wm_paint)
		bool  post_paint : 1;
		bool  hscroll : 1;
		bool  vscroll : 1;
		bool  ncobject : 1;
		bool  reject_all_mouse_msg : 1;
		bool  reject_self_mouse_msg : 1;
		bool  receive_dragdrop_event : 1;
		bool  enable_ime : 1;
		bool  zindex_overlap : 1;
		bool  noclip : 1;
		bool  clip_client : 1;   // ���������ı������ٻ��ƶ����client����ʱ���Ƿ���Ҫ��client���������á���һ��Ķ������������ڻ������listctrl����Ҫ���ã�������������ݻᳬ�������Ǳ���
		bool  tabstop : 1;
		bool  layer;  // �����ֲ�

		// Ĭ��ֵ�ֶΡ����������า�Ǹ����Ĭ����Ϊ
		// ���Կ���ר��ΪdefaultҲ����һ��ͬ����tagObjStyle�����ڶ�Ӧÿһ����ʽ��Ĭ��ֵ
		bool  default_ncobject : 1;  // scrollbarĬ��Ϊncobj
		bool  default_reject_all_mouse_msg : 1; // Ĭ�ϸÿؼ��Ͳ����������Ϣ����panel/label
		bool  default_reject_self_mouse_msg : 1;
		bool  default_transparent : 1;  // �ÿؼ�Ĭ����͸���ġ�Ĭ�����еĿؼ���͸����������������Ч��
		bool  default_tabstop : 1;  // Ĭ�ϸÿؼ��ܷ�tabstop

	}OBJSTYLE;
	typedef struct tagObjState
	{
		char  visibility_ : 2;  // 0:collapsed 1:visible 2:hidden 
		bool  disable : 1;
		bool  press : 1;
		bool  hover : 1;
		bool  force_press : 1;
		bool  force_hover : 1;
		bool  focus : 1;
		bool  default_ : 1;
		bool  selected : 1;
		bool  checked : 1;
		bool  radio_checked : 1; // ??
		bool  readonly : 1;
		bool  activate : 1;   // ������ʹ��
	}OBJSTATE;

	class Object;
	interface __declspec(uuid("B2E563E6-46E7-45F1-B977-5884F217ACD2"))
	UISDKAPI IObject : public IMessage
	{
		IUIApplication* GetUIApplication();
		ISkinRes*  GetSkinRes();
		IWindowBase*  GetWindowObject();
		void  SetDescription(IObjectDescription* p);
		IObjectDescription*  GetDescription();
		LPCTSTR  GetId();
		HWND  GetHWND();
		void  GetMapAttribute(IMapAttribute** ppMapAttribute);
		
		IObject*  GetParentObject();
		IObject*  FindChildObject(LPCTSTR szObjId);
		IObject*  FindNcChildObject(LPCTSTR szObjId);
		IObject*  GetChildObject();
		IObject*  GetNcChildObject();
		IObject*  GetNextObject();
		IObject*  GetPrevObject();

		void  AddChild(IObject* p);
		void  AddNcChild(IObject*p);

		void  Invalidate();
		void  Invalidate(RECT* prcObj);
		void  Invalidate(RECT* prcObj, int nCount);
		void  SetObjectPos(int x, int y, int cx, int cy, int nFlag);
		void  SetObjectPos(RECT* prc, int nFlag);

		bool  IsVisible();
		bool  IsCollapsed();
		bool  IsSelfVisible();
		void  SetVisible(bool b);
		bool  IsHover();
		bool  IsPress();
		bool  IsForcePress();
		bool  IsForceHover();
		bool  IsDefault();
		bool  IsEnable();
		bool  IsFocus();
		bool  CanTabstop();
		void  SetEnable(bool b);
		void  SetForceHover(bool b, bool bNotify);
		void  SetForcePress(bool b, bool bNotify);
		bool  SetMouseCapture(int nNotifyMsgId);
		bool  ReleaseMouseCapture();
		bool  SetFocusInWindow();

		void  ModifyObjectStyle(OBJSTYLE* add, OBJSTYLE* remove);
		bool  TestObjectStyle(const OBJSTYLE& test);
		void  RemoveMeInTheTree();


		void  GetParentRect(RECT* prc);
		void  GetWindowRect(RECT* prc);
		SIZE  GetDesiredSize();
		int   GetWidth();
		int   GetHeight();
		void  SetPaddingRegion(REGION4* prc);
		void  GetPaddingRegion(REGION4* prc);
		void  SetMarginRegion(REGION4* prc);
		void  GetMarginRegion(REGION4* prc);
		void  GetBorderRegion(REGION4* prc);
		void  GetNonClientRegion(REGION4* prc);
		void  SetExtNonClientRegion(REGION4* prc);
		void  GetExtNonClientRegion(REGION4* prc);
		void  GetClientRectInObject(RECT* prc);
		void  GetObjectClientRect(RECT* prc);
		void  ClientRect2ObjectRect(const RECT* rcClient, RECT* rcObj);
		void  WindowRect2ObjectRect(const RECT* rcWindow, RECT* rcObj);
		void  WindowPoint2ObjectPoint(const POINT* ptWindow, POINT* ptObj, bool bCalcTransform);
		void  WindowPoint2ObjectClientPoint(const POINT* ptWindow, POINT* ptObj, bool bCalcTransform);

		ITextRenderBase*  GetTextRender();
		IRenderFont*  GetRenderFont();
		IRenderBase*  GetBkRender();
		IRenderBase*  GetForeRender();
		ILayer*  GetLayer();

		UI_DECLARE_INTERFACE(Object);
	};
}

#endif // _UI_IOBJECT_H_