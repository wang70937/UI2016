#include "stdafx.h"
#include "attributebase.h"

AttributeBase::AttributeBase()
{
	/*m_pSerializer = NULL;*/
	m_pUIApplication = NULL;
	m_bData = false;
	m_bParentKeyIsNull = true;
	m_bReloadOnChanged = false;
}

void  AttributeBase::SetKey(LPCTSTR szKey)
{
	SETSTRING(m_strKey, szKey);
}
LPCTSTR  AttributeBase::GetKey()
{
	return m_strKey.c_str();
}
AttributeBase*  AttributeBase::SetCompatibleKey(LPCTSTR szKey)
{
	SETSTRING(m_strKeyCompatible, szKey);
	return this;
}
LPCTSTR  AttributeBase::GetCompatibleKey()
{
	return m_strKeyCompatible.c_str();
}
bool  AttributeBase::IsKey(LPCTSTR szKey)
{
	if (!szKey || !szKey[0])
		return false;
	if (m_strKey == szKey)
		return true;
	if (m_strKeyCompatible == szKey)
		return true;

	return false;
}

LPCTSTR  AttributeBase::GetDesc()
{
	return m_strDesc.c_str();
}

AttributeBase*  AttributeBase::AsData()
{
	m_bData = true;
	return this;
}
bool  AttributeBase::IsData()
{
	return m_bData;
}

AttributeBase*  AttributeBase::ReloadOnChanged()
{
	m_bReloadOnChanged = true;
	return this;
}
bool  AttributeBase::IsReloadOnChanged()
{
	return m_bReloadOnChanged;
}

AttributeBase*  AttributeBase::SetParentKey(LPCTSTR szParent)
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
LPCTSTR  AttributeBase::GetParentKey()
{
	if (m_bParentKeyIsNull)
		return NULL;
	return m_strParentKey.c_str();
}

void  AttributeBase::SetGroupName(LPCTSTR szGroupName)
{
	SETSTRING(m_strGroupName, szGroupName);
}
LPCTSTR  AttributeBase::GetGroupName()
{
	return m_strGroupName.c_str();
}
void  AttributeBase::SetUIApplication(UIApplication* p)
{
	m_pUIApplication = p;
}
void  AttributeBase::SetSkinRes(SkinRes* p)
{
	m_pSkinRes = p;
}
UIApplication*  AttributeBase::GetUIApplication()
{
	return m_pUIApplication;
}

void  AttributeBase::Load(SERIALIZEDATA* pData)
{
	bool bEraseAttr = pData->NeedErase();

	if (pData->IsReload())
	{
		if (IsData())
			return;

		LPCTSTR szKey = GetKey();
		LPCTSTR szText = pData->pMapAttrib->GetAttr(NULL, szKey, bEraseAttr);
		if (!szText)
		{
			LPCTSTR szCompatibleKey = GetCompatibleKey();
			if (szCompatibleKey && szCompatibleKey[0])
				szText = pData->pMapAttrib->GetAttr(NULL, szCompatibleKey, bEraseAttr);
		}

		if (szText)
		{
			Set(szText);
		}
		// û�����ø����ԣ����á�
		// ��Ҳ���õ�setter���ÿؼ�ȥ�ͷ�һЩ��Դ����renderbase��
		// �����Ͳ�����ҪUI_WM_RESETEATTRIBUTE
		else if (!IsDefaultValue())
		{
			Reset();
		}
	}
	else
	{
		LPCTSTR szKey = GetKey();
		LPCTSTR szText = pData->pMapAttrib->GetAttr(
			NULL, szKey, bEraseAttr);

		if (!szText)
		{
			LPCTSTR szCompatibleKey = GetCompatibleKey();
			if (szCompatibleKey && szCompatibleKey[0])
				szText = pData->pMapAttrib->GetAttr(NULL, szCompatibleKey, bEraseAttr);
		}

		if (szText)
		{
			Set(szText);
		}
		else
		{
			// ���Ӹô��룬�Է������ڹ��캯���еĸ�ֵ�������Ե�Ĭ��ֵ
			// �����ϲ�һ�������±������л�ʧ��
			Reset();
		}
	}
}

void  AttributeBase::Save(SERIALIZEDATA* pData)
{
	LPCTSTR szKey = GetKey();
	if (!szKey)
		return;

	LPCTSTR szValue = Get();
	if (szValue && !IsDefaultValue())
	{
		pData->pListAttrib->AddAttr(szKey, szValue);
	}
}

