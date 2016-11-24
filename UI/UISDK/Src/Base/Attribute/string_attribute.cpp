#include "stdafx.h"
#include "string_attribute.h"
#include "attribute.h"
#include "Src\Resource\i18nres.h"
#include "Src\Resource\skinres.h"
#include "..\Application\uiapplication.h"

using namespace UI;

namespace UI
{
AttributeBase*  CreateStringAttribute()
{
    return new StringAttribute();
}

// TODO: 
// ע��Ϊ��֧��i18n�ֶεı༭���ܣ����ò���������һ���洢�ռ������string id
//    ��Ϊ�˾��������ƻ��Ǳ༭ģʽ�µĴ����߼���Ŀǰ�������ǽ�set�����޸�Ϊ
//    set(LPCTSTR text, int size)
//    �ڱ༭ģʽ�£���text��Ϊ���Σ�ǰһ�α�ʾת��֮������ݣ���һ�����ݱ�ʾ
//    string id���м���\0�ָ���
//    ���ַ�ʽǿ���ⲿ��Ҫ����һ���µĺ��������Ǻܺã���û�뵽���õİ취 

AttributeBase*  CreateI18nStringAttribute()
{
    return new I18nStringAttribute();
}

}

StringAttribute::StringAttribute()
{
    m_pIStringAttribute = NULL;
    m_pBindValue = NULL;
    _this = NULL;
    _getter = NULL;
    _setter = NULL;
}

StringAttribute::~StringAttribute()
{
    SAFE_DELETE(m_pIStringAttribute);
}

IStringAttribute*  StringAttribute::GetIStringAttribute()
{
    if (!m_pIStringAttribute)
        m_pIStringAttribute = new IStringAttribute(this);

    return m_pIStringAttribute;
}

void  StringAttribute::BindReference(String& s)
{
    m_pBindValue = &s;
}

// ��������£�Ĭ��ֵΪNULL
void  StringAttribute::BindFunction(
        void* _this, pfnStringSetter s, pfnStringGetter g)
{
    this->_this = _this;
    this->_setter = s;
    this->_getter = g;
}

void  StringAttribute::BindFunction(
        const function<void(LPCTSTR)>& s, 
        const function<LPCTSTR()>& g)
{
    _func_setter = s;
    _func_getter = g;
}

LPCTSTR  StringAttribute::Get()
{
	LPCTSTR szValue = NULL;
    if (m_pBindValue)
    {
		szValue = m_pBindValue->c_str();
    }
    else if (_this && _getter)
    {
        LPCTSTR  szReturn = NULL;
		long  localVarThis = (long)_this;
		long  localVarGetter = (long)_getter;

		__asm
		{
			mov  ecx, localVarThis;  // ����this
			call localVarGetter;    // ����
			mov  szReturn, eax;        // ��ȡ����ֵ
		}

		szValue = szReturn;
    }
    else if (!_func_getter._Empty())
    {
        szValue = _func_getter();
    }

	return szValue;
}

void  StringAttribute::Set(LPCTSTR szValue)
{
    if (m_pBindValue)
    {
        if (szValue)
            m_pBindValue->assign(szValue);
        else
            m_pBindValue->clear();
    }
    else if (_this && _setter)
    {
		long  localVarThis = (long)_this;
		long  localVarSetter = (long)_setter;

		__asm
		{
			mov eax, dword ptr [szValue] // ѹ�����
			push eax;

			mov  ecx, localVarThis;   // ����this
			call localVarSetter;      // ����
		}
    }
    else if (!_func_setter._Empty())
    {
        _func_setter(szValue);
    }
}


void  StringAttribute::Reset()
{
	Set(GetDefault());
}

bool  StringAttribute::IsDefaultValue()
{
    LPCTSTR szValue = Get();
	LPCTSTR szDefault = GetDefault();

    if (m_strDefault.empty())
    {
        if (!szValue || !szValue[0])
            return true;
    }

	if (szValue && m_strDefault == szValue)
		return true;

    return false;
}

StringAttribute*  StringAttribute::SetDefault(LPCTSTR szValue)
{
	if (szValue)
	{
		m_strDefault = szValue;
	}
	else
	{
		m_strDefault.clear();
	}

	return this;
}

LPCTSTR  StringAttribute::GetDefault()
{
	return m_strDefault.c_str();
}

void StringAttribute::Editor(SERIALIZEDATA* pData, AttributeEditorProxy* p, EditorAttributeFlag e)
{
    p->String2Editor(this, e);
}

// void  StringAttribute::Internationalization()
// {
// 	m_bI18n = true;
// }



LPCTSTR  I18nStringAttribute::Get()
{
    LPCTSTR szValue = StringAttribute::Get();

    if (szValue)
    {
        if (GetUIApplication()->IsEditorMode())
        {
            // ȡ��ԭʼ�ַ���id
            LPCTSTR szBuffer = szValue;
            int nLength1 = _tcslen(szValue);
            if (nLength1)
                szValue = szBuffer + (nLength1 + 1);
        }
    }

    return szValue;
}

void  I18nStringAttribute::Set(LPCTSTR szValue)
{
    int nSize = 0;
    if (szValue)
    {
        int nLength1 = _tcslen(szValue);
        int nLength2 = 0;

        LPCTSTR szValue2 = m_pSkinRes->GetI18nRes().MapConfigValue(szValue);
        if (szValue2)
            nLength2 = _tcslen(szValue2);

        int length1 = (nLength1);
        int length2 = (nLength2);

        // �ڱ༭ģʽ���棬���ܽ�ԭʼ��id��������˱��浽ת�����ַ������棬��Getʱ����
        // ȡ������
        if (GetUIApplication()->IsEditorMode())
        {
            LPTSTR szBuffer = GetTempBuffer(nLength1 + nLength2 + 2);
            char* p = (char*)szBuffer;

            int nSize1 = sizeof(TCHAR)*(length1 + 1);
            int nSize2 = sizeof(TCHAR)*(length2 + 1);
            memcpy(p, szValue2, nSize2);
            memcpy(p + nSize2, szValue, nSize1);
            szValue = szBuffer;

            nSize = length1 + length1 + 2;
        }
        else
        {
            szValue = szValue2;
            nSize = length1;
        }
    }

    if (m_pBindValue)
    {
        if (szValue)
            m_pBindValue->assign(szValue, nSize);
        else
            m_pBindValue->clear();
    }
    else if (_this && _setter_ex)
    {
        long  localVarThis = (long)_this;
        long  localVarSetter = (long)_setter_ex;

        __asm
        {
            mov eax, nSize;
            push eax;
            mov eax, dword ptr[szValue]; // ѹ�����
            push eax;

            mov  ecx, localVarThis;   // ����this
            call localVarSetter;      // ����
        }
    }
    else if (!_func_setter_ex._Empty())
    {
        _func_setter_ex(szValue, nSize);
    }
}

// ��������£�Ĭ��ֵΪNULL
void  I18nStringAttribute::BindFunctionEx(
        void* _this, pfnStringExSetter s, pfnStringGetter g)
{
    this->_this = _this;
    this->_setter_ex = s;
    this->_getter = g;
}

void  I18nStringAttribute::BindFunctionEx(
    const function<void(LPCTSTR, int)>& s,
    const function<LPCTSTR()>& g)
{
    _func_setter_ex = s;
    _func_getter = g;
}