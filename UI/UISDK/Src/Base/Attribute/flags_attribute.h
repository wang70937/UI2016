#pragma once
#include "long_attribute.h"

//
// ���� layout.align="left|top"
//
namespace UI
{

class FlagsAttribute : public LongAttribute
{
public:
    FlagsAttribute();
    ~FlagsAttribute();

    virtual LPCTSTR  Get() override; 
    virtual void  Set(LPCTSTR) override;
    virtual void  Editor(AttributeEditorProxy*, EditorAttributeFlag e) override;

public:
    FlagsAttribute*  AddFlag(long, LPCTSTR);
    IFlagsAttribute*  GetIFlagsAttribute();

private:
    IFlagsAttribute*  m_pIFlagsAttribute;
};

AttributeBase*  CreateFlagsAttribute();

}