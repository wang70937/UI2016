#pragma once
#include "objtree.h"

namespace UI
{
class SkinRes;
class UIApplication;
class WindowBase;
class Layer;
class ObjectLayer;
interface UIElement;
interface IRenderBase;
interface ITextRenderBase;
interface ILayoutParam;

class Object : public ObjTree
{
public:
    Object(IObject*);
    virtual ~Object();

	UI_BEGIN_MSG_MAP()
		UIMSG_ERASEBKGND(OnEraseBkgnd)
		UIMSG_HITTEST(OnHitTest)
		UIMSG_VISIBLE_CHANGED(OnVisibleChanged)
		UIMSG_SERIALIZE(OnSerialize)
		UIMSG_QUERYINTERFACE(Object)
		UIMSG_FINALCONSTRUCT(FinalConstruct)
		UIMSG_FINALRELEASE(FinalRelease)
	UI_END_MSG_MAP()

public:
	IObject* GetIObject();

	LPCTSTR  GetId();
	void  SetId(LPCTSTR szText);

	bool  CreateAccesible(IAccessible** pp);
	void  SetDescription(IObjectDescription*);
	IObjectDescription*  GetDescription();

	UIApplication*  GetUIApplication();
	IUIApplication*  GetIUIApplication();
	SkinRes*  GetSkinRes();
	ISkinRes*  GetISkinRes();
	WindowBase*  GetWindowObject();
	HWND  GetHWND();

	Layer*  GetLayer();
	ObjectLayer*  GetLayerEx();
	Layer*  FindNextLayer(Layer* pParentLayer);
	Object*  GetRenderLayerCreateObject();
	void  SetRenderLayer(bool b);
	bool  HasRenderLayer();

	void  Invalidate();
	void  Invalidate(RECT* prcObj);
	void  Invalidate(RECT* prcObjArray, int nCount);
	void  DrawToLayer__(IRenderTarget* pRenderTarget);
	void  DrawChildObject__(IRenderTarget* pRenderTarget, Object* pChildStart);
	
	Object*  FindChildObject(LPCTSTR szObjId);
	Object*  FindNcChildObject(LPCTSTR szobjId);
	void  ClearMyTreeRelationOnly();
	unsigned long  GetChildCount();
	Object*  GetChildObjectByIndex(unsigned long lIndex);
	unsigned long  GetChildObjectIndex(Object* pChild);

	bool  CanRedraw();
	bool  IsTransparent();
	void  SetTransparent(bool b);
	bool  IsNoClip();
	bool  NeedClip();
	void  SetNoClip(bool b);
	void  SetClipClient(bool b);
	bool  NeedClipClient();
	bool  IsFocus();
	void  SetFocus(bool b, bool bNoitfy=true);
	bool  SetFocusInWindow();
	bool  IsTabstop();
	bool  CanTabstop();
	void  SetTabstop(bool b);
	bool  IsSelfVisible();
	virtual bool  IsVisible();
	bool  IsCollapsed();
	bool  IsSelfCollapsed();
	virtual bool  IsEnable();
	void  LoadVisibleEx(long l);
	long  SaveVisibleEx();
	void  SetVisible(bool b);
	void  SetVisibleEx(VISIBILITY_TYPE eType);
	void  SetDisableDirect(bool b);
	bool  IsSelfDisable();
	void  SetEnable(bool b, bool bNoitfy = true);
	bool  IsDefault();
	bool  IsHover();
	bool  IsPress();
	bool  IsForceHover();
	bool  IsForcePress();
	bool  IsSelected();
	void  SetDefault(bool b, bool bNotify = true);
	void  SetSelected(bool b, bool bNotify = true);
	void  SetForceHover(bool b, bool bNotify = true);
	void  SetForcePress(bool b, bool bNotify = true);
	void  SetHover(bool b, bool bNotify = true);
	void  SetPress(bool b, bool bNotify=true);
	void  SetAsNcObject(bool b);
	bool  IsNcObject();
	bool  IsRejectMouseMsgAll();
	bool  IsRejectMouseMsgSelf();
	void  SetRejectMouseMsgAll(bool b);
	void  SetRejectMouseMsgSelf(bool b);
	int   GetZOrder();
	void  SetZorderDirect(int z);
	void  SortChildByZorder();

	void  ModifyObjectStyle(OBJSTYLE* add, OBJSTYLE* remove);
	bool  TestObjectStyle(const OBJSTYLE& test);

	void  ParseStyleAndLoadAttribute(IMapAttribute* pMatAttrib, bool bReload);
	void  LoadAttributeFromXml(UIElement* pElement, bool bReload);

	LPCTSTR  GetAttribute(LPCTSTR szKey, bool bErase);
	void  AddAttribute(LPCTSTR szKey, LPCTSTR  szValue);
	void  GetMapAttribute(IMapAttribute** ppMapAttribute);
	void  InitDefaultAttrib();

	void  SetOutRef(void** ppOutRef);
	Object*  GetObjectByPos(POINT* pt);
	bool  SetMouseCapture(int nNotifyMsgId);
	bool  ReleaseMouseCapture();
	bool  SetKeyboardCapture(int nNotifyMsgId);
	bool  ReleaseKeyboardCapture();

	
	void  SetBkgndRender(IRenderBase* p);
	void  SetForegndRender(IRenderBase* p);
	void Object::SetTextRender(ITextRenderBase* p);
	ITextRenderBase*  GetTextRender();
	IRenderBase*  GetBkRender();
	IRenderBase*  GetForeRender();
	IRenderFont*  GetRenderFont();

	void  LoadBkgndRender(LPCTSTR szName);
	void  LoadForegndRender(LPCTSTR szName);
	void  LoadTextRender(LPCTSTR szName);
	LPCTSTR  SaveBkgndRender();
	LPCTSTR  SaveForegndRender();
	LPCTSTR  SaveTextRender();

	SIZE  GetDesiredSize();
	void  UpdateLayout();
	//void  UpdateMyLayout();
	void  UpdateLayoutPos();
	void  UpdateObjectNonClientRegion();
	virtual void  SetObjectPos(int x, int y, int cx, int cy, int nFlag = 0);
	void  SetObjectPos(RECT* prc, int nFlag);

	void  GetParentRect(RECT* prc);
	POINT  GetWindowPoint();
	void  GetWindowRect(RECT* lprc);
	bool  GetRectInWindow(RECT* prc, bool bOnlyVisiblePart);
	void  GetClientRectInObject(RECT* prc);
	void  GetObjectClientRect(RECT* prc);
	bool  CalcRectInAncestor(Object*  pObjAncestor, LPCRECT prcObjPart, bool bCalcVisible, RECT* prcOut);
	bool  GetScrollOffset(int* pxOffset, int* pyOffset);
	bool  GetScrollRange(int* pxRange, int* pyRange);

	ILayoutParam*  GetLayoutParam();
	void  CreateLayoutParam();
	void  SetLayoutParam(ILayoutParam* p);

	// Canvas����ר�ýӿ�
	int   GetConfigWidth();
	int   GetConfigHeight();
	int   GetConfigLayoutFlags();
	int   GetConfigLeft();
	int   GetConfigRight();
	int   GetConfigTop();
	int   GetConfigBottom();
	void  SetConfigWidth(int n);
	void  SetConfigHeight(int n);
	void  SetConfigLayoutFlags(int n);
	void  SetConfigLeft(int n);
	void  SetConfigRight(int n);
	void  SetConfigTop(int n);
	void  SetConfigBottom(int n);

	int   GetParentRectL() { return m_rcParent.left; }
	int   GetParentRectT() { return m_rcParent.top; }
	int   GetParentRectR() { return m_rcParent.right; }
	int   GetParentRectB() { return m_rcParent.bottom; }

	void  LoadBorder(REGION4* prc);
	void  SaveBorder(REGION4* prc);
	int   GetBorderL() { return m_rcBorder.left; }
	int   GetBorderT() { return m_rcBorder.top; }
	int   GetBorderR() { return m_rcBorder.right; }
	int   GetBorderB() { return m_rcBorder.bottom; }
	void  SetBorderRegion(RECT* prc);
	void  GetBorderRegion(REGION4* prc) { ::CopyRect(prc, &m_rcBorder); }
	
	void  LoadPadding(REGION4* prc);
	void  SavePadding(REGION4* prc);
	int   GetPaddingL() { return m_rcPadding.left; }
	int   GetPaddingT() { return m_rcPadding.top; }
	int   GetPaddingR() { return m_rcPadding.right; }
	int   GetPaddingB() { return m_rcPadding.bottom; }
	int   GetPaddingW() { return m_rcPadding.left + m_rcPadding.right; }
	int   GetPaddingH() { return m_rcPadding.top + m_rcPadding.bottom; }
	void  GetPaddingRegion(REGION4* prc) { ::CopyRect(prc, &m_rcPadding); }
	void  SetPaddingRegion(REGION4* prc);

	void  LoadMargin(REGION4* prc);
	void  SaveMargin(REGION4* prc);
	void  SetMarginRegion(REGION4* prc) { m_rcMargin.CopyRect(prc); }
	void  GetMarginRegion(REGION4* prc) { ::CopyRect(prc, &m_rcMargin); }
	int   GetMarginL() { return m_rcMargin.left; }
	int   GetMarginT() { return m_rcMargin.top; }
	int   GetMarginR() { return m_rcMargin.right; }
	int   GetMarginB() { return m_rcMargin.bottom; }
	int   GetMarginW() { return m_rcMargin.left + m_rcMargin.right; }
	int   GetMarginH() { return m_rcMargin.top + m_rcMargin.bottom; }

	void  GetNonClientRegion(REGION4* prc);
	void  SetExtNonClientRegion(REGION4* prc) { m_rcExtNonClient.CopyRect(prc); }
	void  GetExtNonClientRegion(REGION4* prc) { ::CopyRect(prc, &m_rcExtNonClient); }

	void  GetClientRectInParent(RECT* prc);
	void  GetClientRectInWindow(RECT* prc);
	int   GetWidth();
	int   GetHeight();
	int   GetWidthWithMargins();
	int   GetHeightWithMargins();

	bool  IntersectWindowRect(LPCRECT prcWindow, RECT* prcIntersectWnd, RECT* prcIntersectObj);
	void  WindowPoint2ObjectPoint(const POINT* ptWindow, POINT* ptObj, bool bCalcTransform);
	void  WindowPoint2ObjectClientPoint(const POINT* ptWindow, POINT* ptClient, bool bCalcTransform);
	void  WindowRect2ObjectClientRect(LPCRECT rcWindow, RECT* rcObj);
	void  WindowRect2ObjectRect(LPCRECT rcWindow, RECT* rcObj);

	static void  ParentClientPoint2ChildPoint(Object* pObjChild, const POINT* pt, POINT* pOut);

	static void  ObjectPoint2ObjectClientPoint(Object* pObj, const POINT* pt, POINT* pOut);
	static void  ObjectPoint2ObjectNonClientPoint(Object* pObj, const POINT* pt, POINT* pOut);
	static void  ObjectRect2ObjectClientRect(Object* pObj, LPCRECT prc, RECT* pOut);
	static void  ObjectClientRect2WindowRect(Object* pObj, LPCRECT prcClient, LPRECT prcWnd);
	static void  ObjectRect2WindowRect(Object* pObj, LPCRECT prcObj, LPRECT prcWnd);

	static void  ParentClientPoint2ChildClientPoint(Object* pObjChild, const POINT* pt, POINT* pOut);
	static void  ParentClientRect2ChildClientRect(Object* pObjChild, LPCRECT prc, RECT* pOut);

	static void  ParentPoint2ChildPoint(Object* pObjChild, const POINT* pt, POINT* pOut);
	static void  ParentRect2ChildRect(Object* pObjChild, LPCRECT prc, RECT* pOut);

	static void  ChildPoint2ParentClientPoint(Object* pObjChild, const POINT* ptChild, POINT*  ptOut);
	static void  ChildRect2ParentClientRect(Object* pObjChild, LPCRECT prc, RECT*  pOut);

	static void  ObjectClientPoint2ObjectPoint(Object*  pObj, const POINT* ptChild, POINT*  ptOut);
	static void  ObjectClientRect2ObjectRect(Object*  pObj, LPCRECT prc, RECT*  pOut);

	static void  ChildPoint2ParentPoint(Object* pObjChild, const POINT* ptChild, POINT*  ptOut);
	static void  ChildRect2ParentRect(Object* pObjChild, LPCRECT prc, RECT*  pOut);

protected:
	HRESULT  FinalConstruct(ISkinRes* pSkinRes);
	void  FinalRelease();
	void*  QueryInterface(const IID* pIID);
	UINT  OnHitTest(POINT* ptInParent, __out POINT* ptInChild);
	void  OnVisibleChanged(BOOL bVisible, IObject* pObjChanged);
	void  OnSerialize(SERIALIZEDATA* pData);
	void  OnEraseBkgnd(IRenderTarget* pRenderTarget);

protected:
	Object*  find_child_object(LPCTSTR szobjId, bool bFindDecendant);
	void  load_renderbase(LPCTSTR szName, IRenderBase*& pRender);
	void  load_textrender(LPCTSTR szName, ITextRenderBase*& pTextRender);
	LPCTSTR  get_renderbase_name(IRenderBase*& pRender);
	LPCTSTR  get_textrender_name(ITextRenderBase*& pTextRender);

	void  load_layer_config(bool b);
	void  update_layer_ptr();

public:
	void  notify_WM_SIZE(UINT nType, UINT nWidth, UINT nHeight);
	void  notify_WM_MOVE(int x, int y);

	IMKMgr* GetIMKMgr();

protected: // virtual
	virtual  IMKMgr*  virtualGetIMKMgr() { return NULL; }
	virtual  void  virtualSetVisibleEx(VISIBILITY_TYPE eType);
	virtual  void  virtualSetEnable(bool b);
	virtual  void  virtualOnSize(UINT nType, UINT nWidth, UINT nHeight);
	virtual  void  virtualOnMove(int x, int y);
	virtual  void  virtualOnPostDrawObjectErasebkgnd() {};

public:
	virtual  void  virtualOnLoad();

public: // static
	void Object::ForwardMessageToChildObject(
			Object* pParent, UIMSG* pMsg);
	void Object::ForwardMessageToChildObject2(
			Object* pParent, UIMSG* pMsg, UIMSG* pMsg2);

protected:
	IObject*   m_pIObject;
	SkinRes*   m_pSkinRes;  // ����֧�ֶ�Ƥ�������棨���ģʽ��
	IObjectDescription*  m_pDescription; // �����һЩ��̬���ԣ�������ָ�룬ͨ������һ��static�����ַ��

	String  m_strId;                    // �ö�����XML�еı�ʶ

#pragma region //�����������
	CRect      m_rcParent;              // �ö���ķ�Χ�������parent��client����.����Window�����ǿͻ�����λ�ã������Ͻ�Ϊ0��0
	CRegion4   m_rcExtNonClient;        // ��չ�ķǿͻ�������border��padding��ͬ��Ϊ����ķǿͻ�����
	CRegion4   m_rcMargin;
	CRegion4   m_rcPadding;
	CRegion4   m_rcBorder;
	//HRGN     m_hRgn;                  // ��δʹ�ã�����ö�����һ�����������򣬱������ø�ֵ����ֵ��window���Ͷ�����Ч. rgn������ڴ������Ͻǵġ�
	ILayoutParam*  m_pLayoutParam;      // ���ֲ�������Object�����ͷ�
#pragma endregion

	OBJSTYLE   m_objStyle;
	OBJSTATE   m_objState;

	// byte  m_lCanRedrawRef;         // ���ڽ���ദ����SetRedraw(false)�󣬱������еط��ſ�SetRedraw(true)���ܽ��л���
	// UINT  m_nStyle2;               // �ؼ���ʽ����Ĺ�ϵ
	long  m_lzOrder;               // �ؼ�z������ʵ�ֿؼ��ص�ʱ��ˢ���ж�����

	IMapAttribute*    m_pIMapAttributeRemain;  // ������չ��δ����������
	IRenderBase*      m_pBkgndRender;          // ������Ⱦ
	IRenderBase*      m_pForegndRender;        // ǰ����Ⱦ
	ITextRenderBase*  m_pTextRender;           // ������Ⱦ����control�����ȡ������
	//IUICursor*        m_pCursor;               // ����������ʽ
	//RenderLayer*      m_pRenderLayer;          // �ö����Ƿ񴴽���һ��layer-- ���ڣ�����ʹ��m_pLayer
	IAccessible*      m_pAccessible;

	// �Ƿ񴴽�һ��layer��Ŀǰ����������������
	// 1. ָ����layer style
	// 2. z order > 0��(Ŀǰδʵ��z order < 0��layer)
	ObjectLayer*  m_pLayer;

	// void*      m_pUserData;             // �Զ������ݣ�Ŀǰ��������UIBuilder�����ݹ�����
	void**     m_ppOutRef;              // Ϊ�˽��һ�����Ա�����п��ܱ��Լ��ĸ�����ɾ���������ȴ��֪������ɾ���ö���ʱ������.

	friend class ObjectAccessible;
};


}


