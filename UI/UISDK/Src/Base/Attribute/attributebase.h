#pragma once
#include "Inc\Interface\iattribute.h"
#include "Src\Resource\skinres.h"

namespace UI
{
    class AttributeSerializer;
    class AttributeEditorProxy;
	class UIApplication;

    // ���Ի���
    class AttributeBase
    {
    public:
		AttributeBase();

        virtual ~AttributeBase() {};
        virtual void  Release() { delete this; };

        virtual LPCTSTR  Get() PURE; 
        virtual void  Set(LPCTSTR) PURE;
        // reloadʱ�������xml��û����ʽ���ø����ԣ������Reset��������Set
		virtual void  Reset() PURE;   
        virtual bool  IsDefaultValue() PURE;

		// ���ء����桢�༭����ʾ
		virtual void  Load(SERIALIZEDATA*);
		virtual void  Save(SERIALIZEDATA*);
		virtual void  Editor(
						SERIALIZEDATA* pData, 
						AttributeEditorProxy* p,
						EditorAttributeFlag e) PURE;

        // �ѹ��ڣ���ɾ�����޸�Ϊ�������Լ��ķ��������á�����StringAttribute.
        virtual void  SetBindValue(void*) {};
        virtual void  SetBindFuction(void* _this, void* _setter, void* _getter) {};

    public:
		void  SetKey(LPCTSTR szKey);
		LPCTSTR  GetKey();
		AttributeBase*  SetCompatibleKey(LPCTSTR szKey);
		LPCTSTR  GetCompatibleKey();
		bool  IsKey(LPCTSTR szKey);

		LPCTSTR  GetDesc();
		AttributeBase*  AsData();
		bool  IsData();
		AttributeBase*  ReloadOnChanged();
		bool  IsReloadOnChanged();

		AttributeBase*  SetParentKey(LPCTSTR szParent);
		LPCTSTR  GetParentKey();

		void  SetGroupName(LPCTSTR szGroupName);
		LPCTSTR  GetGroupName();
		void  SetUIApplication(UIApplication* p);
		void  SetSkinRes(SkinRes* p);
		UIApplication*  GetUIApplication();

    protected:
        LPCTSTR  ConstructTempLPCTSTR(long lValue);

    private:
        String  m_strKey;
		String  m_strKeyCompatible; // ����key�����磺width="100"��Ҳ����д�� layout.width="100"
		String  m_strDesc;

		// ����back.render.image������groupname��ImageRender������
		// parentkey��back.render.type

		// ���������Ķ�����������"Object" "ImageRender"
        String  m_strGroupName;  
		// �����fullkey������bkg.render.type
        String  m_strParentKey;  
        bool  m_bParentKeyIsNull;

        // �������Ƿ���һ���ؼ�data��data�ڻ�����ʱ�����ټ���һ��
        bool  m_bData;  

        // ����bkg.render.type�����޸�ʱ����editor�����¼�������ؼ����������ԣ����л������render����
        // Ŀǰû�뵽һ���Ϻõİ취��ֻ����/ɾ����Ӧ�����ԡ�
        bool  m_bReloadOnChanged;  

    protected:
        UIApplication*  m_pUIApplication;
		SkinRes*  m_pSkinRes;
    };
}
