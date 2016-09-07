#pragma once
#include "attributebase.h"

namespace UI
{

class RectAttribute : public AttributeBase
{
public:
    RectAttribute();
    ~RectAttribute();

    virtual LPCTSTR  Get() override; 
    virtual void  Set(LPCTSTR) override;
	virtual void  Reset() override;
    virtual void  Editor(AttributeEditorProxy* p, EditorAttributeFlag e) override;
    virtual bool  IsDefaultValue() override;
    virtual void  SetBindValue(void*) override;
    virtual void  SetBindFuction(void* _this, void* _setter, void* _getter) override;

public:
	RectAttribute*  SetDefault(LPRECT);
    IRectAttribute*  GetIRectAttribute();

private:
    void  get(LPRECT);
    void  set(LPRECT);

private:
    IRectAttribute*  m_pIRectAttribute;

    RECT*  m_pBindValue;
    RECT   m_rcDefault;

    void*   _this;
    pfnRectSetter  _setter;
    pfnRectGetter  _getter;
};

AttributeBase*  CreateRectAttribute();

}