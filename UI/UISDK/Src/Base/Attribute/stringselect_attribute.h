#pragma once
#include "string_attribute.h"

//
// ���� layout="stack"
//
namespace UI
{

class StringEnumAttribute : public StringAttribute
{
public:
    StringEnumAttribute();
    ~StringEnumAttribute();

    virtual LPCTSTR  Get() override; 
    virtual void  Set(LPCTSTR) override;
	virtual void  Editor(SERIALIZEDATA* pData, AttributeEditorProxy* p, EditorAttributeFlag e) override;

public:
    StringEnumAttribute*  Add(LPCTSTR);
    bool  IsExist(LPCTSTR);
    void  EnumString(pfnEnumStringEnumCallback, WPARAM, LPARAM);
    
    StringEnumAttribute*  FillRenderBaseTypeData();
    StringEnumAttribute*  FillTextRenderBaseTypeData();

    IStringEnumAttribute*  GetIStringEnumAttribute();

private:
    IStringEnumAttribute*  m_pIStringEnumAttribute;

    bool  m_bEditable;  // ���˴��б���ѡ��һ��������Լ��༭һ��ֵ
    list<String>  m_list;
};

AttributeBase*  CreateStringEnumAttribute();

}