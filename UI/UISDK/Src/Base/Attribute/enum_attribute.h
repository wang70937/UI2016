#pragma once
#include "long_attribute.h"

//
// ���� layout="stack" <--> 2
//
namespace UI
{

class EnumAttribute : public LongAttribute
{
public:
    EnumAttribute();
    ~EnumAttribute();
    IEnumAttribute*  GetIEnumAttribute();

    virtual LPCTSTR  Get() override; 
    virtual void  Set(LPCTSTR) override;
	virtual void  Editor(AttributeEditorProxy* p, EditorAttributeFlag e) override;

public:
    EnumAttribute*  AddOption(long, LPCTSTR);
    EnumAttribute*  FillLayoutTypeData();
	EnumAttribute*  FillGraphicsRenderLibraryData();

private:
    IEnumAttribute*  m_pIEnumAttribute;
};

AttributeBase*  CreateEnumAttribute();

}