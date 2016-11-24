#pragma once
#include "numberfilterbase.h"

// Ҫʵ����Сֵ������ѡ�������С9��Ҫ��9���10û�취��
// ��������ﲻ����Сֵ������

namespace UI
{
class RichEdit;
class UIntTextFilter : public RETextFilterBase
{
public:
    UIntTextFilter();
    ~UIntTextFilter();

    static UIntTextFilter* CreateInstance(IRichEdit*  pRE);

    virtual TextFilterType  GetType() { return TFT_UINT; }
    virtual void   KeyFilter(UINT nKey, TextFilterResult& bHandled);
    virtual TCHAR   CharFilter(const TCHAR szSrc, TextFilterResult& bHandled);
    virtual LPCTSTR   StringFilter(LPCTSTR szSrc, TextFilterResult& bHandled);

    void  SetMinMax(UINT nMax);

protected:
    void  SetText(UINT nValue);
    UINT  GetText();
    void  UIntChar(TCHAR c, TextFilterResult& bHandled);

private:

    UINT   m_nMax;
    UINT   m_nMaxTextLength;   // ���ֵ�ı�����
    //UINT   m_nCurValue;  // ά���ɱ�̫�ߣ�����ÿ������get��


    // Temp
    String  m_strResult;

};

}