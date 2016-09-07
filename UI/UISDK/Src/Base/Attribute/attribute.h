#pragma once
#include "Inc\Interface\iattribute.h"
#include "string_attribute.h"
#include "enum_attribute.h"
#include "long_attribute.h"
#include "bool_attribute.h"
#include "rect_attribute.h"
#include "size_attribute.h"
#include "flags_attribute.h"

/*
  2015-2-27 11:28 ���Լ��ش����ع�

  Ŀ�꣺���ڵ����Զ�ȡ���룬���ԵĻ�ȡ��һЩ�߼����뽻����һ����ʾ���ҡ�
        ���������Ҫ�����ԵĶ�ȡ����һЩ��ص��߼����뾡���ķ��룬���Ե�
        ��ȡҲ��͸���ģ�ֻ��Ҫ�������Ծ��С�����Ҳ���¿��Ƕ�UIEditor��֧�֡�

    Q1: render.type���������ʵ��

    Q2: ���Եļ���˳��������������������ʵ�֣���ֻ���Ƚ�����render.type��
        ��� render.font/render.image 

    Q3: ����������޸�Ĭ�����ԡ�����object.transparentĬ��Ϊfalse������label�У�Ĭ��Ϊtrue
        . ��֧�֣�����֧�֣����� panel Ҫ��֧��transparent�ǳ��鷳
        --> ר������һ�����������ڱ�ʾ������Ե�Ĭ��ֵ��ʲô�� SetDefaultTransparent��SetDefaultRejectMsgAll
*/
namespace UI
{

class AttributeBase;
class StringEnumAttribute;

// ����һ�������ຯ������
typedef AttributeBase* (*pfnCreateAttributeClass)();

// �����๤��
class AttributeClassFactory
{
public:
    AttributeClassFactory();
    ~AttributeClassFactory();
    bool  Register(long, pfnCreateAttributeClass);
    AttributeBase*  CreateInstance(long);

private:
    typedef map<long, pfnCreateAttributeClass>  _ClassMap;
    typedef _ClassMap::iterator  _ClassIter;

    _ClassMap  m_map;
};

interface IMapAttribute;

// xml�������л�
class AttributeSerializer
{
public:
    AttributeSerializer(SERIALIZEDATA*, LPCTSTR szGroupName);
    ~AttributeSerializer();
    AttributeBase*  Add(long eType, LPCTSTR szKey, void* pBindValue);
    AttributeBase*  Add(long eType, LPCTSTR szKey, void* _this, void* _setter, void* _getter);

    void  DoAction();
    void  Load();
    void  Save();
    void  Editor();

    IUIApplication*  GetUIApplication();
    SERIALIZEDATA*  GetSerializeData();

public:
    // ��װ

    StringAttribute*  AddString(LPCTSTR, String& sBindValue); // ���ڲ�ʹ��
	StringAttribute*  AddString(LPCTSTR, void* _this, pfnStringSetter s, pfnStringGetter g);
    StringEnumAttribute*  AddStringEnum(LPCTSTR, void* _this, pfnStringSetter s, pfnStringGetter g);

    BoolAttribute*  AddBool(LPCTSTR, bool& bBindValue);
    BoolAttribute*  AddBool(LPCTSTR, void* _this, pfnBoolSetter s, pfnBoolGetter g);

    LongAttribute*  AddLong(LPCTSTR, long& lBindValue);
    LongAttribute*  AddLong(LPCTSTR, void* _this, pfnLongSetter s, pfnLongGetter g);

    FlagsAttribute*  AddFlags(LPCTSTR, long& lBindValue);

    EnumAttribute*  AddEnum(LPCTSTR, long& lBindValue);
    EnumAttribute*  AddEnum(LPCTSTR, void* _this, pfnLongSetter s, pfnLongGetter g);

	RectAttribute*  AddRect(LPCTSTR, RECT& rcBindValue);
	RectAttribute*  AddRect(LPCTSTR, void* _this, pfnRectSetter s, pfnRectGetter g);

	SizeAttribute*  AddSize(LPCTSTR, SIZE& sBindValue);
	SizeAttribute*  AddSize(LPCTSTR, void* _this, pfnSizeSetter s, pfnSizeGetter g);

	Region9Attribute*  Add9Region(LPCTSTR, C9Region& rBindValue);
	Region9Attribute*  Add9Region(LPCTSTR, void* _this, pfnRectSetter s, pfnRectGetter g);

	ColorAttribute*  AddColor(LPCTSTR, Color*& pBindValue);
    RenderBaseAttribute*  AddRenderBase(LPCTSTR szPrefix, Object* pObj, IRenderBase*& pBindValue);
    TextRenderBaseAttribute*  AddTextRenderBase(LPCTSTR szPrefix, Object* pObj, ITextRenderBase*& pBindValue);

private:
    // AttributeBase*  GetAttributeClassByKey(LPCTSTR);

private:
    typedef  list<AttributeBase*>  _AttrList;
    typedef  _AttrList::iterator   _AttrIter;

    _AttrList  m_list;
    SERIALIZEDATA*  m_pData;
	String  m_strGroupName;
};


// IUIEditor �� ����Attribute ֮�������
class AttributeEditorProxy
{
public:
    AttributeEditorProxy(IAttributeEditorProxy&, IUIEditor* p);
    ~AttributeEditorProxy();
    
    void  Clear();
    void  AddAttribute(AttributeBase* p, LPCTSTR szGroupName);
    AttributeBase*  FindAttributeByKey(LPCTSTR szKey);
    bool  Set(LPCTSTR szKey, LPCTSTR szValue);
    LPCTSTR  Get(LPCTSTR szKey);

    void  LoadAttribute2Editor(IObject* pObj);
	UpdateAttribute2EditorResult  UpdateAttribute2Editor(LPCTSTR szKey);

    void  String2Editor(StringAttribute* p, EditorAttributeFlag e);
    void  CharArray2Editor(CharArrayAttribute* p, EditorAttributeFlag e);
    void  Bool2Editor(BoolAttribute* p, EditorAttributeFlag e);
    void  Long2Editor(LongAttribute* p, EditorAttributeFlag e);
	void  Enum2Editor(EnumAttribute* p, EditorAttributeFlag e);
    void  Flags2Editor(FlagsAttribute* p, EditorAttributeFlag e);
    void  Rect2Editor(RectAttribute* p, EditorAttributeFlag e);
	void  Size2Editor(SizeAttribute* p, EditorAttributeFlag e);
	void  Region92Editor(Region9Attribute* p, EditorAttributeFlag e);
    void  StringEnum2Editor(StringEnumAttribute* p, EditorAttributeFlag e);

private:
    IAttributeEditorProxy&  m_oIProxy;
    IUIEditor*  m_pEditor;

    typedef  list<AttributeBase*>  _AttrList;
    typedef  _AttrList::iterator   _AttrIter;

    _AttrList  m_list;
    bool  m_bLoaded;  // ��ʶ�Ƿ��Ѽ��ع�
};

}