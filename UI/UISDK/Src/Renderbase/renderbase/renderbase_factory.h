#pragma once
namespace UI
{	
	class UIApplication;
	interface IRenderBase;
}

class RenderBaseFactory
{
public:
	RenderBaseFactory(UIApplication& app);
	~RenderBaseFactory();

	void  Init();
	void  Clear();

	bool  RegisterUIRenderBaseCreateData(
            LPCTSTR szName, 
            int nType,
            pfnUICreateRenderBasePtr pfunc);

	bool  CreateRenderBaseByName(
            ISkinRes* pSkinRes, 
            LPCTSTR szName, 
            IObject* pObject, 
            IRenderBase** ppOut);

	bool  CreateRenderBase(
            ISkinRes* pSkinRes, 
            int nType, 
            IObject* pObject, 
            IRenderBase** ppOut);

	LPCTSTR  GetRenderBaseName(int nType);

	void  EnumRenderBaseName(
            pfnEnumRenderBaseNameCallback callback, 
            WPARAM wParam, 
            LPARAM lParam);

private:
	UIApplication&  m_app;

	struct UIRENDERBASE_CREATE_INFO
	{
		String  m_strName;    // ���ںܶ���theme���ͣ�����Ӧ�Ŀؼ����Ͳ�һ��
		int     m_nRenderType;
		pfnUICreateRenderBasePtr m_func;
	};

	typedef vector<UIRENDERBASE_CREATE_INFO*>  UIRENDERBASE_CREATE_DATA;
	UIRENDERBASE_CREATE_DATA  m_vecUIRenderBaseCreateData;  // ���ڴ���RenderBase
};