#pragma once

/*
 *  2015.7.19 
 *
 *  1. ����������TTPlayer��ʽ�Ķ�Ƥ�����ܣ�ͬ��ά�����е�Ƥ���ɱ�̫��
 *     ����������ĸ��£����õķ�ʽ��һ��Ƥ������������ļ���
 *
 *  2. ��չ֧�ֲ��ģʽ�Ķ�Ƥ������ÿ�����������Լ���Ƥ������������Ҳ
 *     �����м̳й�ϵ��
 */
namespace UI
{
	interface ISkinManager;
class UIApplication;
class IUISkinDescParse;
enum  SKIN_PACKET_TYPE;
class SkinRes;

class SkinManager
{
public:
	SkinManager(); 
	~SkinManager(); 
    void  Destroy();

public:
	ISkinManager*  GetISkinManager();
	UIApplication*  GetUIApplication();
	void  SetUIApplication(UIApplication* pUIApp);

	SkinRes*  LoadSkinRes(LPCTSTR szPath);
	SkinRes*  GetDefaultSkinRes();
	SkinRes*  GetSkinResByName(LPCTSTR szName);
	uint  GetSkinResCount();
	SkinRes*  GetSkinResByIndex(uint i);

	bool  Save(SkinRes* pSkinRes=NULL);
    void  ChangeSkinHLS( short h, short l, short s, int nFlag );

	LPCTSTR  GetCurrentLanguage();

private:
    ISkinManager*  m_pISkinManager;

	// ���ݳ־ò�
//  String  m_strSkinDir;
	bool  m_bDirty;

	// �־������Զ���
	// SkinBuilderRes    m_SkinBuilderRes;

	// ����ʱ����
	vector<SkinRes*>  m_vSkinRes;           // ���е�Ƥ���б�

	// TODO: ���й���������л�����
	String  m_strLanguage;   // ��ǰʹ�õ�����

	UIApplication*    m_pUIApplication;
};
}

