#pragma once
#include "Inc\Interface\irichedit.h"
namespace UI
{
interface IRichEdit;

class RETextFilterBase : public ITextFilter
{
public:
    RETextFilterBase();
    virtual ~RETextFilterBase() {}

    virtual void  Release(bool bRestoreState);

    void  BindRichEdit(IRichEdit* pRE);
    void  UnbindRichEdit();
    
protected:
    void  OnInvalid();

protected:
    IRichEdit*  m_pRE;

private:
    // ����״̬��ԭ
    bool   m_bOldEnableIme;
    bool   m_bOldRichText;
    bool   m_bOldEnableDragdrop;
    bool   m_bOldMultiline;
};

}