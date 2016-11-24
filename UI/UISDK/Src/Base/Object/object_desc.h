#pragma once
#include "Inc\Interface\iobjectdescription.h"

// Ĭ��desc�����ڻ�ȡdescָ��ʱ���ܷ���һ���ǿ�ָ���ȥ
class UndefineDescription : public IObjectDescription
{
public:
	static IObjectDescription*  Get()
	{
        static UndefineDescription s;
		return &s;
	}

    virtual void  CreateInstance(UI::ISkinRes* p, void** pp) override
    {
        UIASSERT(0);
    }

	virtual OBJ_TYPE  GetMajorType() override
	{
        UIASSERT(0);
		return OBJ_UNKNOWN;
	}
	virtual long  GetMinorType() override
	{
        UIASSERT(0);
		return 0;
	}
	virtual const wchar_t*  GetCategory() override
	{
        UIASSERT(0);
        return CATEGORY_CONTROL;
	}
	virtual const wchar_t*  GetTagName() override
	{
        UIASSERT(0);
		return L"";
	}
	virtual const GUID&  GetGUID() override
	{
        UIASSERT(0);
		return __uuidof(IUnknown);
	}

    virtual void  GetDefaultSize(long& w, long h) override
    {
        UIASSERT(0);
        w = 100;
        h = 100;
    }
    virtual HBITMAP  LoadIcon() override 
    {
        UIASSERT(0);
        return NULL;
    }
};