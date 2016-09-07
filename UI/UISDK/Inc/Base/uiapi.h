#ifndef _UI_API_H_
#define _UI_API_H_

namespace UI
{

	// ����ʱ�汾����
#define UISDK_VERSION_MAJOR   1    // ���汾
#define UISDK_VERSION_MINOR   0    // �ΰ汾
#define UISDK_VERSION_PATCH   0    // �����汾

	interface IUIElement;
	interface ISkinRes;
	interface IObject;
	interface ILayout;
	interface IUIApplication;

	UISDKAPI bool  CreateUIApplication(IUIApplication** pp);

	// UI���󴴽�����
	//typedef  HRESULT(*pfnUICreateRenderBasePtr)(ISkinRes* pSkinRes, void** ppOut);
	//typedef  HRESULT(*pfnUICreateTextRenderBasePtr)(ISkinRes* pSkinRes, void** ppOut);
	typedef  HRESULT(*pfnParseSkinTag)(IUIElement*, ISkinRes* pSkinRes);
	//typedef  HRESULT(*pfnUICreateLayoutPtr)(IObject* pObject, ILayout**  ppLayout);

	// UI���󴴽�����
	typedef  HRESULT(*pfnUICreateRenderBasePtr)(ISkinRes* pSkinRes, void** ppOut);
	typedef  HRESULT(*pfnUICreateTextRenderBasePtr)(ISkinRes* pSkinRes, void** ppOut);
	typedef  HRESULT(*pfnParseSkinTag)(IUIElement*, ISkinRes* pSkinRes);
	typedef  HRESULT(*pfnUICreateLayoutPtr)(IObject* pObject, ILayout**  ppLayout);

	// uiapplication�е�ö�ٻص�
	typedef  bool(*pfnEnumLayoutTypeCallback)(long, LPCTSTR, WPARAM, LPARAM);
	typedef  void(*pfnEnumRenderBaseNameCallback)(LPCTSTR, WPARAM, LPARAM);
	typedef  void(*pfnEnumTextRenderBaseNameCallback)(LPCTSTR, WPARAM, LPARAM);


	// ����ֵ:
	enum  PARSE_CONTROL_RETURN
	{
		ParseControl_Failed,  // ��������
		ParseControl_CreateObject, // �������������˸ö��󣬵�û�м�������
		ParseControl_LoadObject,   // �������󲢼��������ԣ�����û�����ӽ��
		ParseControl_LoadDescendants,  // �����Լ��������ӽ�������
	};
	typedef  PARSE_CONTROL_RETURN(*pfnParseControlTag)(IUIElement*, ISkinRes*, IObject* pObjParent, IObject**);

	// �ⲿUI����ע����ڣ����ⲿDLLʵ�ָõ�������
#define FUN_RegisterUIObject_NAME  "RegisterUIObject"
	typedef  HRESULT(*funRegisterUIObjectPtr)(IUIApplication* p);


	struct UIMSG;
	interface IMessage;
 	UISDKAPI long  UIPostMessage(IUIApplication* pUIApp, UIMSG* pMsg, int nMsgMapID = 0);
	UISDKAPI long  UISendMessage(UIMSG* pMsg, int nMsgMapID = 0, BOOL* pbHandled = NULL);
	UISDKAPI long  UISendMessage(IMessage* pMsgTo, UINT message, WPARAM wParam = 0, LPARAM lParam = 0,
						UINT nCode = 0, IMessage* pMsgFrom = NULL, int nMsgMapID = 0, BOOL* pbHandled = NULL);

	interface IMapAttribute;
	interface IListAttribute;
	UISDKAPI HRESULT  UICreateIMapAttribute(IMapAttribute** ppOut);
	UISDKAPI HRESULT  UICreateIListAttribute(IListAttribute** ppOut);

	enum GRAPHICS_RENDER_LIBRARY_TYPE;
	enum IMAGE_ITEM_TYPE;
	interface IRenderBitmap;
	UISDKAPI void  UICreateRenderBitmap(IUIApplication*  pUIApp, GRAPHICS_RENDER_LIBRARY_TYPE eGraphicsRenderType, IMAGE_ITEM_TYPE eType, IRenderBitmap** ppOut);

}
#endif