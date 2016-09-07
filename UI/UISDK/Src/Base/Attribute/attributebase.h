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
        AttributeBase()
        {
            /*m_pSerializer = NULL;*/
            m_pUIApplication = NULL;
            m_bData = false;
            m_bParentKeyIsNull = true;
			m_bReloadOnChanged = false;
        }

        virtual ~AttributeBase() {};
        virtual void  Release() { delete this; };

        virtual LPCTSTR  Get() PURE; 
        virtual void  Set(LPCTSTR) PURE;
        // reloadʱ�������xml��û����ʽ���ø����ԣ������Reset��������Set
		virtual void  Reset() PURE;   
        virtual void  Editor(AttributeEditorProxy* p, EditorAttributeFlag e) PURE;
        virtual bool  IsDefaultValue() PURE;

        virtual void  SetBindValue(void*) PURE;
        virtual void  SetBindFuction(void* _this, void* _setter, void* _getter) PURE;

    public:
        void  SetKey(LPCTSTR szKey)
        {
            SETSTRING(m_strKey, szKey);
        }
        LPCTSTR  GetKey()
        {
            return m_strKey.c_str();
        }
		AttributeBase*  SetCompatibleKey(LPCTSTR szKey)
		{
			SETSTRING(m_strKeyCompatible, szKey);
			return this;
		}
		LPCTSTR  GetCompatibleKey()
		{
			return m_strKeyCompatible.c_str();
		}
		bool  IsKey(LPCTSTR szKey)
		{
			if (!szKey || !szKey[0])
				return false;
			if (m_strKey == szKey)
				return true;
			if (m_strKeyCompatible == szKey)
				return true;

			return false;
		}

        LPCTSTR  GetDesc()
        {
            return m_strDesc.c_str();
        }

        AttributeBase*  AsData()
        {
            m_bData = true; 
			return this;
        }
        bool  IsData() 
        {
            return m_bData; 
        }

        AttributeBase*  ReloadOnChanged()
        {
            m_bReloadOnChanged = true;
            return this;
        }
        bool  IsReloadOnChanged()
        {
            return m_bReloadOnChanged;
        }

		AttributeBase*  SetParentKey(LPCTSTR szParent)
		{
            if (szParent)
            {
                m_strParentKey = szParent;
                m_bParentKeyIsNull = false;
            }
            else
            {
                m_strParentKey.clear();
                m_bParentKeyIsNull = true;
            }
			return this;
		}
        LPCTSTR  GetParentKey()
        {
            if (m_bParentKeyIsNull)
                return NULL;
            return m_strParentKey.c_str();
        }

		void  SetGroupName(LPCTSTR szGroupName)
		{
			SETSTRING(m_strGroupName, szGroupName);
		}
		LPCTSTR  GetGroupName()
		{
			return m_strGroupName.c_str();
		}
        void  SetUIApplication(UIApplication* p)
        {
            m_pUIApplication = p;
        }
		void  SetSkinRes(SkinRes* p)
		{
			m_pSkinRes = p;
		}
        UIApplication*  GetUIApplication()
        {
            return m_pUIApplication;
        }

    protected:
        LPCTSTR  ConstructTempLPCTSTR(long lValue);

    private:
        String  m_strKey;
		String  m_strKeyCompatible; // ����key�����磺width="100"��Ҳ����д�� layout.width="100"
		String  m_strDesc;
        String  m_strGroupName;  // ���������Ķ�����������"Object" "ImageRender"
        String  m_strParentKey;  // �����fullkey������bkg.render.type
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
