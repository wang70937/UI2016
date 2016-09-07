#include "stdafx.h"
#include "attribute.h"
#include "stringselect_attribute.h"
#include "9region_attribute.h"
#include "Inc\interface\iobject.h"
#include "Inc\Interface\imapattr.h"
#include "Src/Base/Object/object.h"
#include "renderbase_attribute.h"
#include "textrenderbase_attribute.h"
#include "color_attribute.h"


using namespace UI;

//////////////////////////////////////////////////////////////////////////

AttributeClassFactory::AttributeClassFactory()
{
    Register(ATTRIBUTE_TYPE_STRING, CreateStringAttribute);
    Register(ATTRIBUTE_TYPE_BOOL,   CreateBoolAttribute);
	Register(ATTRIBUTE_TYPE_RECT,   CreateRectAttribute);
	Register(ATTRIBUTE_TYPE_9REGION,  CreateRegion9Attribute);
    Register(ATTRIBUTE_TYPE_LONG,   CreateLongAttribute);
    Register(ATTRIBUTE_TYPE_FLAGS,  CreateFlagsAttribute);
    Register(ATTRIBUTE_TYPE_STRINGENUM, CreateStringEnumAttribute);
    Register(ATTRIBUTE_TYPE_ENUM,   CreateEnumAttribute);
	Register(ATTRIBUTE_TYPE_UICOLOR, CreateColorAttribute);
    Register(ATTRIBUTE_TYPE_RENDERBASE, CreateRenderBaseAttribute);
	Register(ATTRIBUTE_TYPE_TEXTRENDERBASE, CreateTextRenderBaseAttribute);
}

AttributeClassFactory& GetAttributeClassFactory()
{
    static AttributeClassFactory _s;
    return _s;
}

AttributeClassFactory::~AttributeClassFactory()
{
    m_map.clear();
}

bool  AttributeClassFactory::Register(long lType, pfnCreateAttributeClass p)
{
    if (!p)
        return false;

    if (m_map.find(lType) != m_map.end())
        return false;

    m_map[lType] = p;
    return true;
}

AttributeBase*  AttributeClassFactory::CreateInstance(long lType)
{
    _ClassIter iter = m_map.find(lType);
    if (iter == m_map.end())
        return NULL;

    return iter->second();
}

//////////////////////////////////////////////////////////////////////////

AttributeSerializer::AttributeSerializer(SERIALIZEDATA* p, LPCTSTR szGroupName):
    m_pData(p)
{
    UIASSERT(p);
	SETSTRING(m_strGroupName, szGroupName);
}

AttributeSerializer::~AttributeSerializer()
{
    DoAction();

    _AttrIter iter = m_list.begin();
    for (; iter != m_list.end(); ++iter)
    {
        (*iter)->Release();
    }
    m_list.clear();
}

void  AttributeSerializer::DoAction()
{
    if (m_pData->nFlags & SERIALIZEFLAG_LOAD)
    {
        Load();
    }
    else if (m_pData->nFlags & SERIALIZEFLAG_SAVE)
    {
        Save();
    }
    else if (m_pData->nFlags & SERIALIZEFLAG_EDITOR)
    {
        Editor();
    }
}

SERIALIZEDATA*  AttributeSerializer::GetSerializeData()
{
    return m_pData;
}
IUIApplication*  AttributeSerializer::GetUIApplication()
{
    if (m_pData)
        return m_pData->pUIApplication;

    return NULL;
}

StringAttribute*  AttributeSerializer::AddString(LPCTSTR szKey, String& sBindValue)
{
    return static_cast<StringAttribute*>(Add(ATTRIBUTE_TYPE_STRING, szKey, &sBindValue));
}
StringAttribute*  AttributeSerializer::AddString(LPCTSTR szKey, void* _this, pfnStringSetter s, pfnStringGetter g)
{
	return static_cast<StringAttribute*>(Add(ATTRIBUTE_TYPE_STRING, szKey, _this, s, g));
}
StringEnumAttribute*  AttributeSerializer::AddStringEnum(LPCTSTR szKey, void* _this, pfnStringSetter s, pfnStringGetter g)
{
    return static_cast<StringEnumAttribute*>(Add(ATTRIBUTE_TYPE_STRINGENUM, szKey, _this, s, g));
}
BoolAttribute*  AttributeSerializer::AddBool(LPCTSTR szKey, bool& bBindValue)
{
    return static_cast<BoolAttribute*>(Add(ATTRIBUTE_TYPE_BOOL, szKey, &bBindValue));
}
BoolAttribute*  AttributeSerializer::AddBool(LPCTSTR szKey, void* _this, pfnBoolSetter s, pfnBoolGetter g)
{
    return static_cast<BoolAttribute*>(Add(ATTRIBUTE_TYPE_BOOL, szKey, _this, s, g));
}
LongAttribute*  AttributeSerializer::AddLong(LPCTSTR szKey, long& lBindValue)
{
    return static_cast<LongAttribute*>(Add(ATTRIBUTE_TYPE_LONG, szKey, &lBindValue));
}
LongAttribute*  AttributeSerializer::AddLong(LPCTSTR szKey, void* _this, pfnLongSetter s, pfnLongGetter g)
{
    return static_cast<LongAttribute*>(Add(ATTRIBUTE_TYPE_LONG, szKey, _this, s, g));
}
FlagsAttribute*  AttributeSerializer::AddFlags(LPCTSTR szKey, long& lBindValue)
{
    return static_cast<FlagsAttribute*>(Add(ATTRIBUTE_TYPE_FLAGS, szKey, &lBindValue));
}

EnumAttribute*  AttributeSerializer::AddEnum(LPCTSTR szKey, long& lBindValue)
{
    return static_cast<EnumAttribute*>(Add(ATTRIBUTE_TYPE_ENUM, szKey, &lBindValue));
}
EnumAttribute*  AttributeSerializer::AddEnum(LPCTSTR szKey, void* _this, pfnLongSetter s, pfnLongGetter g)
{
    return static_cast<EnumAttribute*>(Add(ATTRIBUTE_TYPE_ENUM, szKey, _this, s, g));
}

RectAttribute*  AttributeSerializer::AddRect(LPCTSTR szKey, RECT& rcBindValue)
{
	return static_cast<RectAttribute*>(Add(ATTRIBUTE_TYPE_RECT, szKey, &rcBindValue));
}
RectAttribute*  AttributeSerializer::AddRect(LPCTSTR szKey, void* _this, pfnRectSetter s, pfnRectGetter g)
{
	return static_cast<RectAttribute*>(Add(ATTRIBUTE_TYPE_RECT, szKey, _this, s, g));
}

SizeAttribute*  AttributeSerializer::AddSize(LPCTSTR szKey, SIZE& sBindValue)
{
	return static_cast<SizeAttribute*>(Add(ATTRIBUTE_TYPE_SIZE, szKey, &sBindValue));
}
SizeAttribute*  AttributeSerializer::AddSize(LPCTSTR szKey, void* _this, pfnSizeSetter s, pfnSizeGetter g)
{
	return static_cast<SizeAttribute*>(Add(ATTRIBUTE_TYPE_SIZE, szKey, _this, s, g));
}

Region9Attribute*  AttributeSerializer::Add9Region(LPCTSTR szKey, C9Region& rBindValue)
{
	return static_cast<Region9Attribute*>(Add(ATTRIBUTE_TYPE_9REGION, szKey, &rBindValue));
}
Region9Attribute*  AttributeSerializer::Add9Region(LPCTSTR szKey, void* _this, pfnRectSetter s, pfnRectGetter g)
{
	return static_cast<Region9Attribute*>(Add(ATTRIBUTE_TYPE_9REGION, szKey, _this, s, g));
}

ColorAttribute*  AttributeSerializer::AddColor(LPCTSTR szKey, Color*& pBindValue)
{
	return static_cast<ColorAttribute*>(Add(ATTRIBUTE_TYPE_UICOLOR, szKey, &pBindValue));
}

RenderBaseAttribute*  AttributeSerializer::AddRenderBase(LPCTSTR szPrefix, Object* pObj, IRenderBase*& pBindValue)
{
    String strKey;
    if (szPrefix)
        strKey = szPrefix;

	strKey.append(XML_RENDER_TYPE);

    RenderBaseAttribute* p = static_cast<RenderBaseAttribute*>
        (Add(ATTRIBUTE_TYPE_RENDERBASE, strKey.c_str(), &pBindValue));
    if (!p)
        return NULL;

    p->SetPrefix(szPrefix);
    p->SetObjectPtr(pObj);

	// ��editor�У���̬�޸�render type��Ҫ������ԡ�
	// 1. ���Կ��ܹ���һ��key����render.image=����ʹ����type������Ҳ���Թ���
	// 2. Ҫʵ��undo/redo�����ܶ�������
	if (m_pData->pUIApplication->IsDesignMode())
		m_pData->SetErase(false);

	//p->FillRenderBaseTypeData()
	p->ReloadOnChanged();
    return p;
}

TextRenderBaseAttribute*  AttributeSerializer::AddTextRenderBase(
        LPCTSTR szPrefix, Object* pObj, ITextRenderBase*& pBindValue)
{
    String strKey;
    if (szPrefix)
        strKey = szPrefix;

	strKey.append(XML_TEXTRENDER_TYPE);

    TextRenderBaseAttribute* p = static_cast<TextRenderBaseAttribute*>
        (Add(ATTRIBUTE_TYPE_TEXTRENDERBASE, strKey.c_str(), &pBindValue));
    if (!p)
        return NULL;

    p->SetPrefix(szPrefix);
    p->SetObjectPtr(pObj);
    return p;
}

// ���һ������
AttributeBase*  AttributeSerializer::Add(long eType, LPCTSTR szKey, void* pBindValue)
{
    AttributeBase* pAttribute = GetAttributeClassFactory().CreateInstance(eType);
    if (!pAttribute)
    {
        UI_LOG_ERROR(TEXT("Create Attribute Class Factory Failed. Type=%d"), eType);
        return NULL;
    }

	pAttribute->SetUIApplication(m_pData->pUIApplication ? m_pData->pUIApplication->GetImpl() : NULL);
	pAttribute->SetSkinRes(m_pData->pSkinRes ? m_pData->pSkinRes->GetImpl() : NULL);
	if (m_pData->szPrefix)
	{
		String str(m_pData->szPrefix);
		str.append(szKey);
		pAttribute->SetKey(str.c_str());
	}
	else
	{
		pAttribute->SetKey(szKey);
	}

    pAttribute->SetParentKey(m_pData->szParentKey);
    if (pBindValue)
        pAttribute->SetBindValue(pBindValue);

    m_list.push_back(pAttribute);
    return pAttribute;
}

AttributeBase*  AttributeSerializer::Add(long eType, LPCTSTR szKey, void* _this, void* _setter, void* _getter)
{
    AttributeBase* pAttribute = GetAttributeClassFactory().CreateInstance(eType);
    if (!pAttribute)
    {
        UI_LOG_ERROR(TEXT("Create Attribute Class Factory Failed. Type=%d"), eType);
        return NULL;
    }

	pAttribute->SetUIApplication(m_pData->pUIApplication ? m_pData->pUIApplication->GetImpl() : NULL);
	pAttribute->SetSkinRes(m_pData->pSkinRes ? m_pData->pSkinRes->GetImpl() : NULL);
	if (m_pData->szPrefix)
	{
		String str(m_pData->szPrefix);
		str.append(szKey);
		pAttribute->SetKey(str.c_str());
	}
	else
	{
		pAttribute->SetKey(szKey);
	}
    pAttribute->SetParentKey(m_pData->szParentKey);
    if (_this)
        pAttribute->SetBindFuction(_this, _setter, _getter);

    m_list.push_back(pAttribute);
    return pAttribute;
}

// ����һ������

// AttributeBase*  AttributeSerializer::GetAttributeClassByKey(LPCTSTR szKey)
// {
//     UIASSERT(szKey);
// 
//     _AttrIter iter = m_list.begin();
//     for (; iter != m_list.end(); ++iter)
//     {
//         AttributeBase* pAttribute = *iter;
//         if (!pAttribute->IsKey(szKey))
//             continue;
// 
//         return pAttribute;
//     }
//     return NULL;
// }

void  AttributeSerializer::Load()
{
    if (!m_pData->pMapAttrib)
        return;

    bool bEraseAttr = m_pData->NeedErase();

	if (m_pData->IsReload())
	{
		_AttrIter iter = m_list.begin();
		for (; iter != m_list.end(); ++iter)
		{
			AttributeBase* pAttribute = *iter;
			if (pAttribute->IsData())
				continue;

			LPCTSTR szKey = pAttribute->GetKey();
			LPCTSTR szText = m_pData->pMapAttrib->GetAttr(NULL, szKey, bEraseAttr);
			if (!szText)
			{
				LPCTSTR szCompatibleKey = pAttribute->GetCompatibleKey();
				if (szCompatibleKey && szCompatibleKey[0])
					szText = m_pData->pMapAttrib->GetAttr(NULL, szCompatibleKey, bEraseAttr);
			}
			
			if (szText)
			{
				pAttribute->Set(szText);
			}
			// û�����ø����ԣ����á�
			// ��Ҳ���õ�setter���ÿؼ�ȥ�ͷ�һЩ��Դ����renderbase��
			// �����Ͳ�����ҪUI_WM_RESETEATTRIBUTE
			else if (!pAttribute->IsDefaultValue())
			{
				pAttribute->Reset();
			}
		}
	}
	else
	{
		_AttrIter iter = m_list.begin();
		for (; iter != m_list.end(); ++iter)
		{
			AttributeBase* pAttribute = *iter;

			LPCTSTR szKey = pAttribute->GetKey();
			LPCTSTR szText = m_pData->pMapAttrib->GetAttr(
                    NULL, szKey, bEraseAttr);

			if (!szText)
			{
				LPCTSTR szCompatibleKey = pAttribute->GetCompatibleKey();
				if (szCompatibleKey && szCompatibleKey[0])
					szText = m_pData->pMapAttrib->GetAttr(NULL, szCompatibleKey, bEraseAttr);
			}

			if (szText)
            {
				pAttribute->Set(szText);
            }
            else
            {
                // ���Ӹô��룬�Է������ڹ��캯���еĸ�ֵ�������Ե�Ĭ��ֵ
                // �����ϲ�һ�������±������л�ʧ��
                pAttribute->Reset(); 
            }
		}
	}
}

void  AttributeSerializer::Save()
{
    String  strKey;
    _AttrIter iter = m_list.begin();
    for (; iter != m_list.end(); ++iter)
    {
        AttributeBase* pAttribute = *iter;

        LPCTSTR szKey = pAttribute->GetKey();
        if (!szKey)
            continue;

        strKey = szKey;

        LPCTSTR szValue = pAttribute->Get();
        if (szValue && !pAttribute->IsDefaultValue())
        {
            m_pData->pListAttrib->AddAttr(strKey.c_str(), szValue);
        }
    }
}

// ��AttributeBase*�б��AttributeSerializer�������룬����UIEditorά���͹���
void  AttributeSerializer::Editor()
{
    if (!m_pData->pUIApplication)
        return;

    IAttributeEditorProxy* pIProxy = m_pData->pAttributeEditorProxy;
    if (!pIProxy)
        return;

    AttributeEditorProxy* pProxy = pIProxy->GetImpl();
    if (!pProxy)
        return;
    
    _AttrIter iter = m_list.begin();
    for (; iter != m_list.end(); ++iter)
    {
        AttributeBase* pAttribute = *iter;

        //pAttribute->SetSerializer(NULL);  // AttributeSerializer��һ���ֲ�����������������
        pProxy->AddAttribute(pAttribute, m_strGroupName.c_str());
    }
    m_list.clear();
}

//////////////////////////////////////////////////////////////////////////

AttributeEditorProxy::AttributeEditorProxy(IAttributeEditorProxy& o, IUIEditor* p) :
        m_oIProxy(o),
        m_pEditor(p)
{
    m_bLoaded = false;
}

void  AttributeEditorProxy::Clear()
{
    _AttrIter iter = m_list.begin();
    for (; iter != m_list.end(); ++iter)
    {
        (*iter)->Release();
    }
    m_list.clear();
}

AttributeEditorProxy::~AttributeEditorProxy()
{
    Clear();
}

void  AttributeEditorProxy::AddAttribute(AttributeBase* p, LPCTSTR szGroupName)
{
    UIASSERT(p);
    m_list.push_back(p);
	p->SetGroupName(szGroupName);
    p->Editor(this, EDITOR_ATTRIBUTE_ADD);  // �ᴥ��AddString2Editor/AddBool2Editor��
}

AttributeBase*  AttributeEditorProxy::FindAttributeByKey(LPCTSTR szKey)
{
    if (!szKey)
        return NULL;

    _AttrIter iter = m_list.begin();
    for (; iter != m_list.end(); ++iter)
    {
        if ((*iter)->IsKey(szKey))
            return *iter;
    }

    return NULL;
}

bool  AttributeEditorProxy::Set(LPCTSTR szKey, LPCTSTR szValue)
{
    AttributeBase*  pAttribute = FindAttributeByKey(szKey);
    if (!pAttribute)
        return false;

    pAttribute->Set(szValue);
    return true;
}

LPCTSTR  AttributeEditorProxy::Get(LPCTSTR szKey)
{
    AttributeBase*  pAttribute = FindAttributeByKey(szKey);
    if (!pAttribute)
        return NULL;

    return pAttribute->Get();
}

void  AttributeEditorProxy::LoadAttribute2Editor(IObject* pObj)
{
    if (!pObj)
        return;

    if (!m_bLoaded)
    {
        m_bLoaded = true;
        SERIALIZEDATA data = {0};
        data.pAttributeEditorProxy = &m_oIProxy;
        data.pUIApplication = pObj->GetUIApplication();
		data.pSkinRes = pObj->GetSkinRes();
        data.nFlags = SERIALIZEFLAG_EDITOR;
        UISendMessage(static_cast<IMessage*>(pObj), UI_MSG_SERIALIZE, (WPARAM)&data);
    }
    else
    {
        _AttrIter iter = m_list.begin();
        for (; iter != m_list.end(); ++iter)
        {
            (*iter)->Editor(this, EDITOR_ATTRIBUTE_ADD);
        }
    }
}

UpdateAttribute2EditorResult  AttributeEditorProxy::UpdateAttribute2Editor(LPCTSTR szKey)
{
	if (!szKey)
		return UpdateAttribute2EditorFailed;

	AttributeBase* pAttrib = FindAttributeByKey(szKey);
	if (!pAttrib)
		return UpdateAttribute2EditorFailed;

    if (pAttrib->IsReloadOnChanged())
    {
        return UpdateAttribute2EditorNeedReload;
    }

    pAttrib->Editor(this, EDITOR_ATTRIBUTE_UPDATE);
    return UpdateAttribute2EditorSuccess;
}

void  AttributeEditorProxy::String2Editor(StringAttribute* p, EditorAttributeFlag e)
{
	m_pEditor->EditorStringAttribute(p->GetIStringAttribute(), e);
}
void  AttributeEditorProxy::CharArray2Editor(CharArrayAttribute* p, EditorAttributeFlag e)
{
    UIASSERT(0); // TODO:
    //m_pEditor->EditorStringAttribute(p->GetICharArrayAttribute(), e);
}
void  AttributeEditorProxy::Bool2Editor(BoolAttribute* p, EditorAttributeFlag e)
{
	m_pEditor->EditorBoolAttribute(p->GetIBoolAttribute(), e);
}
void  AttributeEditorProxy::Long2Editor(LongAttribute* p, EditorAttributeFlag e)
{
	m_pEditor->EditorLongAttribute(p->GetILongAttribute(), e);
}
void  AttributeEditorProxy::Enum2Editor(EnumAttribute* p, EditorAttributeFlag e)
{
	m_pEditor->EditorEnumAttribute(p->GetIEnumAttribute(), e);
}
void  AttributeEditorProxy::Flags2Editor(FlagsAttribute* p, EditorAttributeFlag e)
{
    m_pEditor->EditorFlagsAttribute(p->GetIFlagsAttribute(), e);
}
void  AttributeEditorProxy::Rect2Editor(RectAttribute* p, EditorAttributeFlag e)
{
    m_pEditor->EditorRectAttribute(p->GetIRectAttribute(), e);
}
void  AttributeEditorProxy::Size2Editor(SizeAttribute* p, EditorAttributeFlag e)
{
	m_pEditor->EditorSizeAttribute(p->GetISizeAttribute(), e);
}
void  AttributeEditorProxy::Region92Editor(Region9Attribute* p, EditorAttributeFlag e)
{
	m_pEditor->EditorRegion9Attribute(p->GetIRegion9Attribute(), e);
}
void  AttributeEditorProxy::StringEnum2Editor(StringEnumAttribute* p, EditorAttributeFlag e)
{
    m_pEditor->EditorStringEnumAttribute(p->GetIStringEnumAttribute(), e);
}
// void  AttributeEditorProxy::RenderBase2Editor(RenderBaseAttribute* p, EditorAttributeFlag e)
// {
//     m_pEditor->EditorRenderBase(p->GetIRenderBaseAttribute(), e);
// }
// void  AttributeEditorProxy::TextRenderBase2Editor(TextRenderBaseAttribute* p, EditorAttributeFlag e)
// {
//     m_pEditor->EditorTextRenderBase(p->GetITextRenderBaseAttribute(), e);
// }