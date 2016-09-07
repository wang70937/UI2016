#pragma once
#include "Inc\Interface\iobjectdescription.h"
#include "Inc\Interface\ihwndhost.h"

class HwndHostDescription : public IObjectDescription
{
public:
	static IObjectDescription*  Get()
	{
		static HwndHostDescription s;
		return &s;
	}

    virtual void  CreateInstance(UI::ISkinRes* p, void** pp) override
    {
        *pp = IHwndHost::CreateInstance(p);
    }

	virtual OBJ_TYPE  GetMajorType() override
	{
		return OBJ_HWNDHOST;
	}
	virtual long  GetMinorType() override
	{
		return 0;
	}
	virtual const wchar_t*  GetCategory() override
	{
		return CATEGORY_CONTROL; 
	}
	virtual const wchar_t*  GetTagName() override
	{
		return L"HwndHost";
	}
	virtual const GUID&  GetGUID() override
	{
		return __uuidof(IHwndHost);
	}

    virtual void  GetDefaultSize(long& w, long h) override
    {
        w = 100;
        h = 100;
    }
    virtual HBITMAP  LoadIcon() override 
    {
        return NULL;
    }
};