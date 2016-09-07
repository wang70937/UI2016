#pragma once

namespace UI
{
class SkinRes;
class Object;

class OutCtrlDllItem
{
public:
	OutCtrlDllItem();
	~OutCtrlDllItem();

public:
	HMODULE hModule;
	String  strDllPath;
};
class SkinBuilderRes
{
public:
	SkinBuilderRes(){m_pUIApp = NULL;};
	~SkinBuilderRes() {Clear();}

	void  SetUIApplication(IUIApplication* pUIApp){ m_pUIApp = pUIApp; }
	void  AddOuterCtrlsDllPath(const String&  strDll);
	OutCtrlDllItem*  FindOuterCtrlsDll(const String& strDll);
	OutCtrlDllItem*  FindOuterCtrlsDll(HMODULE hModule);
	void  Clear();

	bool  Register(OutCtrlDllItem*);

private:
	IUIApplication*  m_pUIApp;
	vector<OutCtrlDllItem*>   m_vecOuterCtrlsDll;   // �ⲿ�ؼ����ڵ�DLL������ע���ⲿ��������
};

// ��ǰƤ����ɫ������
struct SKIN_HLS_INFO
{
	short  h;       // -120 ~ 120
	short  l;       // -100 ~ 100
	short  s;       // -100 ~ 100
	int   nFlag;
};


interface ISkinConfigRes : public IRootInterface
{
	virtual SKIN_HLS_INFO* GetSkinHlsInfo()= 0;
	virtual void SetHLS(char h, char l, char s, int nFlag) = 0;
};
//
//	Ƥ����Ϣ��������Ԫ��xmlλ�� 
//
class SkinConfigRes : public ISkinConfigRes
{
public:
	SkinConfigRes();

private:
	String   m_strImageXmlPath;       // ȫ·�������浽�ļ��еĲ������·�� 
	String   m_strFontXmlPath;
	String   m_strColorXmlPath;
	String   m_strStyleXmlPath;
	String   m_strLayoutXmlPath;
	String   m_strI18nXmlPath;

	SKIN_HLS_INFO  m_sHlsInfo;
public:

	DECLARE_STRING_SETGET( ImageXmlPath );
	DECLARE_STRING_SETGET( FontXmlPath );
	DECLARE_STRING_SETGET( ColorXmlPath );
	DECLARE_STRING_SETGET( StyleXmlPath );
	DECLARE_STRING_SETGET( LayoutXmlPath );
	DECLARE_STRING_SETGET( I18nXmlPath );

	void    SetHLS(char h, char l, char s, int nFlag);
	virtual SKIN_HLS_INFO* GetSkinHlsInfo() { return &m_sHlsInfo; }
};


}

