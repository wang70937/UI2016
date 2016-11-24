#include "stdafx.h"
#include "Inc\Interface\iattribute.h"
#include "attribute.h"
#include "string_attribute.h"
#include "long_attribute.h"
#include "enum_attribute.h"
#include "bool_attribute.h"
#include "rect_attribute.h"
#include "stringselect_attribute.h"
#include "9region_attribute.h"
#include "flags_attribute.h"
#include "size_attribute.h"
#include "renderbase_attribute.h"
#include "textrenderbase_attribute.h"
#include "color_attribute.h"

using namespace UI;

//////////////////////////////////////////////////////////////////////////
ILongAttribute::ILongAttribute(LongAttribute* p)
{
    m_pImpl = p;
}
ILongAttribute*  ILongAttribute::SetDefault(long l)
{
    m_pImpl->SetDefault(l);
    return this;
}
ILongAttribute*  ILongAttribute::AddAlias(long l, LPCTSTR sz)
{
    m_pImpl->AddAlias(l, sz);
    return this;
}
ILongAttribute*  ILongAttribute::AsData()
{
    m_pImpl->AsData();
    return this;
}

LPCTSTR  ILongAttribute::GetKey()
{
    return m_pImpl->GetKey();
}
LPCTSTR  ILongAttribute::GetDesc()
{
    return EMPTYTEXT;
}
LPCTSTR  ILongAttribute::GetGroupName()
{
	return m_pImpl->GetGroupName();
}
LPCTSTR  ILongAttribute::GetParentKey()
{
    return m_pImpl->GetParentKey();
}
long  ILongAttribute::GetLong()
{
    return m_pImpl->GetLong();
}
LPCTSTR  ILongAttribute::Get()
{
    return m_pImpl->Get();
}
long  ILongAttribute::EnumAlias(pfnEnumAliasCallback c, WPARAM w, LPARAM l)
{
	return m_pImpl->EnumAlias(c, w, l);
}
uint  ILongAttribute::GetAliasCount()
{
	return m_pImpl->GetAliasCount();
}
//////////////////////////////////////////////////////////////////////////

IBoolAttribute::IBoolAttribute(BoolAttribute* p)
{
    m_pImpl = p;
}
IBoolAttribute*  IBoolAttribute::SetDefault(bool b)
{
    m_pImpl->SetDefault(b);
    return this;
}
IBoolAttribute*  IBoolAttribute::AsData()
{
    m_pImpl->AsData();
    return this;
}

LPCTSTR  IBoolAttribute::GetKey()
{
    return m_pImpl->GetKey();
}

LPCTSTR  IBoolAttribute::GetDesc()
{
    return EMPTYTEXT;
}
LPCTSTR  IBoolAttribute::GetGroupName()
{
	return m_pImpl->GetGroupName();
}
LPCTSTR  IBoolAttribute::GetParentKey()
{
    return m_pImpl->GetParentKey();
}
LPCTSTR  IBoolAttribute::Get()
{
    return m_pImpl->Get();
}
bool  IBoolAttribute::GetBool()
{
    return m_pImpl->GetBool();
}

bool  IBoolAttribute::GetDefaultBool()
{
    return m_pImpl->GetDefaultBool();
}

IBoolAttribute*  IBoolAttribute::ReloadOnChanged()
{
    m_pImpl->ReloadOnChanged();
    return this;
}

//////////////////////////////////////////////////////////////////////////
IStringAttribute::IStringAttribute(StringAttribute* p)
{
    m_pImpl = p;
}

IStringAttribute*  IStringAttribute::SetDefault(LPCTSTR sz)
{
    m_pImpl->SetDefault(sz);
    return this;
}
IStringAttribute*  IStringAttribute::AsData()
{
    m_pImpl->AsData();
    return this;
}
// IStringAttribute*  IStringAttribute::Internationalization()
// {
// 	m_pImpl->Internationalization();
// 	return this;
// }

LPCTSTR  IStringAttribute::GetKey()
{
    return m_pImpl->GetKey();
}
LPCTSTR  IStringAttribute::Get()
{
    return m_pImpl->Get();
}
LPCTSTR  IStringAttribute::GetDesc()
{
    return EMPTYTEXT;
}
LPCTSTR  IStringAttribute::GetGroupName()
{
	return m_pImpl->GetGroupName();
}
LPCTSTR  IStringAttribute::GetParentKey()
{
    return m_pImpl->GetParentKey();
}

//////////////////////////////////////////////////////////////////////////


ICharArrayAttribute::ICharArrayAttribute(CharArrayAttribute* p)
{
    m_pImpl = p;
}

//////////////////////////////////////////////////////////////////////////

IEnumAttribute::IEnumAttribute(EnumAttribute* p)
{
    m_pImpl = p;
}
IEnumAttribute*  IEnumAttribute::AddOption(long l, LPCTSTR sz)
{
    m_pImpl->AddOption(l ,sz);
    return this;
}
IEnumAttribute*  IEnumAttribute::SetDefault(long l)
{
    m_pImpl->SetDefault(l);
    return this;
}
IEnumAttribute*  IEnumAttribute::AsData()
{
    m_pImpl->AsData();
    return this;
}

LPCTSTR  IEnumAttribute::GetKey()
{
	return m_pImpl->GetKey();
}
LPCTSTR  IEnumAttribute::GetDesc()
{
	return EMPTYTEXT;
}
LPCTSTR  IEnumAttribute::GetGroupName()
{
	return  m_pImpl->GetGroupName();
}
LPCTSTR  IEnumAttribute::GetParentKey()
{
    return m_pImpl->GetParentKey();
}
LPCTSTR  IEnumAttribute::Get()
{
    return m_pImpl->Get();
}
long  IEnumAttribute::GetLong()
{
	return m_pImpl->GetLong();
}
long  IEnumAttribute::EnumAlias(pfnEnumAliasCallback c, WPARAM w, LPARAM l)
{
	return m_pImpl->EnumAlias(c, w, l);
}
uint  IEnumAttribute::GetAliasCount()
{
	return m_pImpl->GetAliasCount();
}

IEnumAttribute*  IEnumAttribute::ReloadOnChanged()
{
	m_pImpl->ReloadOnChanged();
	return this;
}

//////////////////////////////////////////////////////////////////////////

IFlagsAttribute::IFlagsAttribute(FlagsAttribute* p)
{
    m_pImpl = p;
}
IFlagsAttribute*  IFlagsAttribute::AddFlag(long l, LPCTSTR sz)
{
    m_pImpl->AddFlag(l ,sz);
    return this;
}
IFlagsAttribute*  IFlagsAttribute::SetDefault(long l)
{
    m_pImpl->SetDefault(l);
    return this;
}
IFlagsAttribute*  IFlagsAttribute::AsData()
{
    m_pImpl->AsData();
    return this;
}

LPCTSTR  IFlagsAttribute::GetKey()
{
    return m_pImpl->GetKey();
}
LPCTSTR  IFlagsAttribute::GetDesc()
{
    return EMPTYTEXT;
}
LPCTSTR  IFlagsAttribute::GetGroupName()
{
    return  m_pImpl->GetGroupName();
}
LPCTSTR  IFlagsAttribute::GetParentKey()
{
    return m_pImpl->GetParentKey();
}
LPCTSTR  IFlagsAttribute::Get()
{
    return m_pImpl->Get();
}
long  IFlagsAttribute::GetLong()
{
    return m_pImpl->GetLong();
}
long  IFlagsAttribute::EnumAlias(pfnEnumAliasCallback c, WPARAM w, LPARAM l)
{
    return m_pImpl->EnumAlias(c, w, l);
}
uint  IFlagsAttribute::GetAliasCount()
{
    return m_pImpl->GetAliasCount();
}

//////////////////////////////////////////////////////////////////////////

IRectAttribute::IRectAttribute(RectAttribute* p)
{
    m_pImpl = p;
}
IRectAttribute*  IRectAttribute::AsData()
{
    m_pImpl->AsData();
    return this;
}

LPCTSTR  IRectAttribute::GetKey()
{
    return m_pImpl->GetKey();
}
LPCTSTR  IRectAttribute::GetDesc()
{
    return m_pImpl->GetDesc();
}
LPCTSTR  IRectAttribute::GetGroupName()
{
	return m_pImpl->GetGroupName();
}
LPCTSTR  IRectAttribute::GetParentKey()
{
    return m_pImpl->GetParentKey();
}
LPCTSTR  IRectAttribute::Get()
{
    return m_pImpl->Get();
}

//////////////////////////////////////////////////////////////////////////


ISizeAttribute::ISizeAttribute(SizeAttribute* p)
{
	m_pImpl = p;
}
ISizeAttribute*  ISizeAttribute::AsData()
{
	m_pImpl->AsData();
	return this;
}

LPCTSTR  ISizeAttribute::GetKey()
{
	return m_pImpl->GetKey();
}
LPCTSTR  ISizeAttribute::GetDesc()
{
	return m_pImpl->GetDesc();
}
LPCTSTR  ISizeAttribute::GetGroupName()
{
	return m_pImpl->GetGroupName();
}
LPCTSTR  ISizeAttribute::GetParentKey()
{
	return m_pImpl->GetParentKey();
}
LPCTSTR  ISizeAttribute::Get()
{
	return m_pImpl->Get();
}

//////////////////////////////////////////////////////////////////////////

IRegion9Attribute::IRegion9Attribute(Region9Attribute* p)
{
	m_pImpl = p;
}

IRegion9Attribute*  IRegion9Attribute::AsData()
{
	m_pImpl->AsData();
	return this;
}

LPCTSTR  IRegion9Attribute::GetKey()
{
	return m_pImpl->GetKey();
}
LPCTSTR  IRegion9Attribute::GetDesc()
{
	return m_pImpl->GetDesc();
}
LPCTSTR  IRegion9Attribute::GetGroupName()
{
	return m_pImpl->GetGroupName();
}
LPCTSTR  IRegion9Attribute::GetParentKey()
{
    return m_pImpl->GetParentKey();
}
LPCTSTR  IRegion9Attribute::Get()
{
	return m_pImpl->Get();
}

//////////////////////////////////////////////////////////////////////////

IStringEnumAttribute::IStringEnumAttribute(StringEnumAttribute* p)
{
    m_pImpl = p;
}

LPCTSTR  IStringEnumAttribute::Get()
{
    return m_pImpl->Get();
}

void  IStringEnumAttribute::EnumString(pfnEnumStringEnumCallback callback, WPARAM w, LPARAM l)
{
    m_pImpl->EnumString(callback, w, l);
}

IStringEnumAttribute*  IStringEnumAttribute::FillRenderBaseTypeData()
{
	m_pImpl->FillRenderBaseTypeData();
	return this;
}
IStringEnumAttribute*  IStringEnumAttribute::ReloadOnChanged()
{
	m_pImpl->ReloadOnChanged();
	return this;
}
IStringEnumAttribute*  IStringEnumAttribute::FillTextRenderBaseTypeData()
{
	m_pImpl->FillTextRenderBaseTypeData();
	return this;
}

IStringEnumAttribute*  IStringEnumAttribute::AsData()
{
    m_pImpl->AsData();
    return this;
}

LPCTSTR  IStringEnumAttribute::GetKey()
{
    return m_pImpl->GetKey();
}
LPCTSTR  IStringEnumAttribute::GetDesc()
{
    return m_pImpl->GetDesc();
}
LPCTSTR  IStringEnumAttribute::GetGroupName()
{
	return m_pImpl->GetGroupName();
}
LPCTSTR  IStringEnumAttribute::GetParentKey()
{
    return m_pImpl->GetParentKey();
}
//////////////////////////////////////////////////////////////////////////

IColorAttribute::IColorAttribute(ColorAttribute* p)
{
	m_pImpl = p;
}

LPCTSTR  IColorAttribute::GetKey()
{
	return m_pImpl->GetKey();
}
LPCTSTR  IColorAttribute::GetDesc()
{
	return m_pImpl->GetDesc();
}
LPCTSTR  IColorAttribute::GetGroupName()
{
	return m_pImpl->GetGroupName();
}
LPCTSTR  IColorAttribute::GetParentKey()
{
	return m_pImpl->GetParentKey();
}
LPCTSTR  IColorAttribute::Get()
{
	return m_pImpl->Get();
}

//////////////////////////////////////////////////////////////////////////

IRenderBaseAttribute::IRenderBaseAttribute(RenderBaseAttribute* p)
{
    m_pImpl = p;
}
IRenderBaseAttribute*  IRenderBaseAttribute::AsData()
{
    m_pImpl->AsData();
    return this;
}

LPCTSTR  IRenderBaseAttribute::GetKey()
{
    return m_pImpl->GetKey();
}
LPCTSTR  IRenderBaseAttribute::GetDesc()
{
    return m_pImpl->GetDesc();
}
LPCTSTR  IRenderBaseAttribute::Get()
{
    return m_pImpl->Get();
}

LPCTSTR  IRenderBaseAttribute::GetGroupName()
{
	return m_pImpl->GetGroupName();
}
LPCTSTR  IRenderBaseAttribute::GetParentKey()
{
	return m_pImpl->GetParentKey();
}

//////////////////////////////////////////////////////////////////////////

ITextRenderBaseAttribute::ITextRenderBaseAttribute(TextRenderBaseAttribute* p)
{
    m_pImpl = p;
}
ITextRenderBaseAttribute*  ITextRenderBaseAttribute::AsData()
{
    m_pImpl->AsData();
    return this;
}

LPCTSTR  ITextRenderBaseAttribute::GetKey()
{
    return m_pImpl->GetKey();
}
LPCTSTR  ITextRenderBaseAttribute::GetDesc()
{
    return m_pImpl->GetDesc();
}
LPCTSTR  ITextRenderBaseAttribute::Get()
{
    return m_pImpl->Get();
}
LPCTSTR  ITextRenderBaseAttribute::GetGroupName()
{
	return m_pImpl->GetGroupName();
}
LPCTSTR  ITextRenderBaseAttribute::GetParentKey()
{
	return m_pImpl->GetParentKey();
}

//////////////////////////////////////////////////////////////////////////

AttributeSerializerWrap::AttributeSerializerWrap(SERIALIZEDATA* p, LPCTSTR szGroupName)
{
    m_pImpl = new AttributeSerializer(p, szGroupName);
}
AttributeSerializerWrap::~AttributeSerializerWrap()
{
    SAFE_DELETE(m_pImpl);
}
AttributeSerializer*  AttributeSerializerWrap::GetImpl()
{
	return m_pImpl;
}

IStringAttribute*  AttributeSerializerWrap::AddString(LPCTSTR szKey, void* _this, pfnStringSetter s, pfnStringGetter g)
{
    StringAttribute* p = m_pImpl->AddString(szKey, _this, s, g);
    if (!p)
        return NULL;

    return p->GetIStringAttribute();
}

IStringAttribute*  AttributeSerializerWrap::AddString(
    LPCTSTR szKey, 
    const std::function<void(LPCTSTR)>& s, 
    const std::function<LPCTSTR()>& g)
{
    StringAttribute* p = m_pImpl->AddString(szKey, s, g);
    if (!p)
        return NULL;

    return p->GetIStringAttribute();
}

IStringAttribute*  AttributeSerializerWrap::AddI18nString(LPCTSTR szKey, void* _this, pfnStringExSetter s, pfnStringGetter g)
{
    StringAttribute* p = m_pImpl->AddI18nString(szKey, _this, s, g);
    if (!p)
        return NULL;

    return p->GetIStringAttribute();
}

IStringAttribute*  AttributeSerializerWrap::AddI18nString(
    LPCTSTR szKey, 
    const std::function<void(LPCTSTR, int)>& s, 
    const std::function<LPCTSTR()>& g)
{
    StringAttribute* p = m_pImpl->AddI18nString(szKey, s, g);
    if (!p)
        return NULL;

    return p->GetIStringAttribute();
}

IStringEnumAttribute*  AttributeSerializerWrap::AddStringEnum(LPCTSTR szKey, void* _this, pfnStringSetter s, pfnStringGetter g)
{
	StringEnumAttribute* p = m_pImpl->AddStringEnum(szKey, _this, s, g);
	if (!p)
		return NULL;

	return p->GetIStringEnumAttribute();
}

IBoolAttribute*  AttributeSerializerWrap::AddBool(LPCTSTR szKey, bool& bBindValue)
{
    BoolAttribute* p = m_pImpl->AddBool(szKey, bBindValue);
    if (!p)
        return NULL;

    return p->GetIBoolAttribute();
}
IBoolAttribute*  AttributeSerializerWrap::AddBool(LPCTSTR szKey, void* _this, pfnBoolSetter s, pfnBoolGetter g)
{
    BoolAttribute* p = m_pImpl->AddBool(szKey, _this, s, g);
    if (!p)
        return NULL;

    return p->GetIBoolAttribute();
}

ILongAttribute*  AttributeSerializerWrap::AddLong(LPCTSTR szKey, long& lBindValue)
{
    LongAttribute* p = m_pImpl->AddLong(szKey, lBindValue);
    if (!p)
        return NULL;

    return p->GetILongAttribute();
}
ILongAttribute*  AttributeSerializerWrap::AddLong(LPCTSTR szKey, int& lBindValue)
{
	return AddLong(szKey, *(long*)&lBindValue);
}
ILongAttribute*  AttributeSerializerWrap::AddLong(LPCTSTR szKey, void* _this, pfnLongSetter s, pfnLongGetter g)
{
    LongAttribute* p = m_pImpl->AddLong(szKey, _this, s, g);
    if (!p)
        return NULL;

    return p->GetILongAttribute();
}

IFlagsAttribute*  AttributeSerializerWrap::AddFlags(LPCTSTR szKey, long& lBindValue)
{
	FlagsAttribute* p = m_pImpl->AddFlags(szKey, lBindValue);
	if (!p)
		return NULL;

	return p->GetIFlagsAttribute();
}

IEnumAttribute*  AttributeSerializerWrap::AddEnum(LPCTSTR szKey, long& lBindValue)
{
    EnumAttribute* p = m_pImpl->AddEnum(szKey, lBindValue);
    if (!p)
        return NULL;

    return p->GetIEnumAttribute();
}
IEnumAttribute*  AttributeSerializerWrap::AddEnum(LPCTSTR szKey, void* _this, pfnLongSetter s, pfnLongGetter g)
{
    EnumAttribute* p = m_pImpl->AddEnum(szKey, _this, s, g);
    if (!p)
        return NULL;

    return p->GetIEnumAttribute();
}

IRectAttribute*  AttributeSerializerWrap::AddRect(LPCTSTR szKey, RECT& rcBindValue)
{
	RectAttribute* p = m_pImpl->AddRect(szKey, rcBindValue);
	if (!p)
		return NULL;

	return p->GetIRectAttribute();
}
IRectAttribute*  AttributeSerializerWrap::AddRect(LPCTSTR szKey, void* _this, pfnRectSetter s, pfnRectGetter g)
{
	RectAttribute* p = m_pImpl->AddRect(szKey, _this, s, g);
	if (!p)
		return NULL;

	return p->GetIRectAttribute();
}

ISizeAttribute*  AttributeSerializerWrap::AddSize(LPCTSTR szKey, SIZE& sBindValue)
{
	SizeAttribute* p = m_pImpl->AddSize(szKey, sBindValue);
	if (!p)
		return NULL;

	return p->GetISizeAttribute();
}
ISizeAttribute*  AttributeSerializerWrap::AddSize(LPCTSTR szKey, void* _this, pfnSizeSetter s, pfnSizeGetter g)
{
	SizeAttribute* p = m_pImpl->AddSize(szKey, _this, s, g);
	if (!p)
		return NULL;

	return p->GetISizeAttribute();
}

IRegion9Attribute*  AttributeSerializerWrap::Add9Region(LPCTSTR szKey, C9Region& rBindValue)
{
	Region9Attribute* p = m_pImpl->Add9Region(szKey, rBindValue);
	if (!p)
		return NULL;

	return p->GetIRegion9Attribute();
}

IColorAttribute*  AttributeSerializerWrap::AddColor(LPCTSTR szKey, Color*& pBindValue)
{
	ColorAttribute* p = m_pImpl->AddColor(szKey, pBindValue);
	if (!p)
		return NULL;

	return p->GetIColorAttribute();
}

IRegion9Attribute*  AttributeSerializerWrap::Add9Region(LPCTSTR szKey, void* _this, pfnRectSetter s, pfnRectGetter g)
{
	Region9Attribute* p = m_pImpl->Add9Region(szKey, _this, s, g);
	if (!p)
		return NULL;

	return p->GetIRegion9Attribute();
}

IRenderBaseAttribute*  AttributeSerializerWrap::AddRenderBase(LPCTSTR szPrefix, IObject* pObj, IRenderBase*& pBindValue)
{
	RenderBaseAttribute* p = m_pImpl->AddRenderBase(
		szPrefix, pObj?pObj->GetImpl():nullptr, pBindValue);
	if (!p)
		return NULL;

	return p->GetIRenderBaseAttribute();
}
//////////////////////////////////////////////////////////////////////////

IAttributeEditorProxy::IAttributeEditorProxy(IUIEditor* p)
{
    m_pImpl = new AttributeEditorProxy(*this, p);
}
IAttributeEditorProxy::~IAttributeEditorProxy()
{
    SAFE_DELETE(m_pImpl);
}
AttributeEditorProxy*  IAttributeEditorProxy::GetImpl()
{
    return m_pImpl;
}

void  IAttributeEditorProxy::LoadAttribute2Editor(IObject* pObj)
{
    m_pImpl->LoadAttribute2Editor(pObj);
}
UpdateAttribute2EditorResult  IAttributeEditorProxy::UpdateAttribute2Editor(LPCTSTR szKey)
{
	 return m_pImpl->UpdateAttribute2Editor(szKey);
}
bool  IAttributeEditorProxy::SetAttribute(LPCTSTR szKey, LPCTSTR szValue)
{
    return m_pImpl->Set(szKey, szValue);
}

LPCTSTR  IAttributeEditorProxy::GetAttribute(LPCTSTR szKey)
{
    return m_pImpl->Get(szKey);
}