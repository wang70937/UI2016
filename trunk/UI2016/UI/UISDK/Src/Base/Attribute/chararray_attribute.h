#pragma once
#include "attributebase.h"

namespace UI
{

class CharArrayAttribute : public AttributeBase
{
public:
    CharArrayAttribute();
    ~CharArrayAttribute();
    ICharArrayAttribute*  GetICharArrayAttribute();

    virtual LPCTSTR  Get() override; 
    virtual void  Set(LPCTSTR) override;
	virtual void  Reset() override;
    virtual bool  IsDefaultValue() override;
    virtual void  SetBindValue(void*) override;
    virtual void  SetBindFuction(void* _this, void* _setter, void* _getter) override;
    virtual void  Editor(UI::AttributeEditorProxy *,UI::EditorAttributeFlag) override;
    void  SetBindValueSize(long lSize);
    
public:
	CharArrayAttribute*  SetDefault(LPCTSTR);
	LPCTSTR  GetDefault();

protected:
    ICharArrayAttribute*  m_pICharArrayAttribute;

    wchar_t*  m_pBindValue;
    long  m_lBindValueSize;

    String   m_strDefault;
	bool  m_bDefaultIsNULL;   // Ĭ����NULL��������""

    void*  _this;
    pfnStringSetter  _setter;
    pfnStringGetter  _getter;
};

AttributeBase*  CreateCharArrayAttribute();

}