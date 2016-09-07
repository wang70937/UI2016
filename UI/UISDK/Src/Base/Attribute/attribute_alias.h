#pragma once
#include "Inc\Interface\iattribute.h"

//
// ����ӳ�� (����ö����������)
//
// �������һЩ������ֵҲ���ã�����"auto" --> AUTO
// 

namespace UI
{
class LongAttributeAlias
{
public:
    LPCTSTR  GetAlias(long);
    bool  GetAlias(LPCTSTR, long* pOut);
    void  AddAlias(long, LPCTSTR);

	uint  GetCount();
	long  EnumAlias(pfnEnumAliasCallback, WPARAM, LPARAM);

private:
    typedef std::multimap<long, String>  AliasMap;
    typedef AliasMap::iterator  AliasIter;

    AliasMap  m_mapAlias;
};
}