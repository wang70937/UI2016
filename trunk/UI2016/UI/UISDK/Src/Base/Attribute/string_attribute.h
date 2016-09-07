#pragma once
#include "attributebase.h"

namespace UI
{

class StringAttribute : public AttributeBase
{
public:
    StringAttribute();
    ~StringAttribute();
    IStringAttribute*  GetIStringAttribute();

    virtual LPCTSTR  Get() override; 
    virtual void  Set(LPCTSTR) override;
	virtual void  Reset() override;
    virtual void  Editor(AttributeEditorProxy* p, EditorAttributeFlag e) override;
    virtual bool  IsDefaultValue() override;
    virtual void  SetBindValue(void*) override;
    virtual void  SetBindFuction(void* _this, void* _setter, void* _getter) override;

public:
	StringAttribute*  SetDefault(LPCTSTR);
	LPCTSTR  GetDefault();
	void  Internationalization();

protected:
    IStringAttribute*  m_pIStringAttribute;

    String*  m_pBindValue;

    String   m_strDefault;
	bool  m_bDefaultIsNULL;   // Ĭ����NULL��������""
	bool  m_bI18n;            // ��Ҫ���ʻ����ֶ�

    void*  _this;
    pfnStringSetter  _setter;
    pfnStringGetter  _getter;
};

AttributeBase*  CreateStringAttribute();

}