#ifndef _UI_IRICHEDIT_H_
#define _UI_IRICHEDIT_H_

namespace UI
{
    
enum TextFilterResult
{
    TFR_NOT_HANDLED,  // δ����ʹ��Ĭ��RE����
    TFR_HANDLED,      // �Ѵ�������ҪRE����
    TFR_REPLACE_TEXT  // δ���������ı������滻���ٴ���
};
enum TextFilterType
{
    TFT_NONE,
    TFT_DIGIT,
    TFT_UINT,
    TFT_PHONENUMBER,
    TFT_ACCOUNT,
	TFT_PASSWORD,
};
interface ITextFilter
{   
    // ��RE����ʱ������Ҫ�ٻ�ԭ״̬�ˣ�bRestoreState����Ϊfalse
    // ���滻textfilterʱ������Ҫ��bRestoreState����Ϊtrue
    virtual void  Release(bool bRestoreState) = 0;
    virtual TextFilterType  GetType() = 0;

    virtual void   KeyFilter(UINT nKey, TextFilterResult& bHandled) = 0;
    virtual TCHAR   CharFilter(const TCHAR szSrc, TextFilterResult& bHandled) = 0;
    virtual LPCTSTR   StringFilter(LPCTSTR szSrc, TextFilterResult& bHandled) = 0;
};


}

#endif // _UI_IRICHEDIT_H_