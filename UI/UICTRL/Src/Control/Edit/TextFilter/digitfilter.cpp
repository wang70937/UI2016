#include "stdafx.h"
#include "digitfilter.h"
#include "numberop.h"

using namespace UI;

DigitTextFilter::DigitTextFilter()
{
}
DigitTextFilter::~DigitTextFilter()
{
}

DigitTextFilter* DigitTextFilter::CreateInstance(IRichEdit*  pRE)
{
    DigitTextFilter* p = new DigitTextFilter;
	if (pRE)
		p->BindRichEdit(pRE);
    return p;
}

void  DigitTextFilter::KeyFilter(UINT nKey, TextFilterResult& bHandled)
{
    bHandled = TFR_NOT_HANDLED;

    // �����ܻس� (�޸�Ϊ����ģʽ����)
//     if (nKey == VK_RETURN)
//     {
//         bHandled = TFR_HANDLED;
//     }
}

TCHAR   DigitTextFilter::CharFilter(const TCHAR szSrc, TextFilterResult& bHandled)
{
    bHandled = TFR_NOT_HANDLED;
    switch (szSrc)
    {
    case TEXT('1'):
    case TEXT('2'):
    case TEXT('3'):
    case TEXT('4'):
    case TEXT('5'):
    case TEXT('6'):
    case TEXT('7'):
    case TEXT('8'):
    case TEXT('9'):
    case TEXT('0'):
    case TEXT('-'):
        break;

    default:
        OnInvalid();
        bHandled = TFR_HANDLED;
    }
    return szSrc;
}

LPCTSTR  DigitTextFilter::StringFilter(LPCTSTR szSrc, TextFilterResult& bHandled)
{
    bHandled = TFR_HANDLED;
    if (!szSrc)
        return NULL;

    UINT nLength = _tcslen(szSrc);

    // �����������ַ�
    for (UINT i = 0; i < nLength; i++)
    {
        switch (szSrc[i])
        {
        case TEXT('1'):
        case TEXT('2'):
        case TEXT('3'):
        case TEXT('4'):
        case TEXT('5'):
        case TEXT('6'):
        case TEXT('7'):
        case TEXT('8'):
        case TEXT('9'):
        case TEXT('0'):
            continue;

        default:
            return NULL;
        }
    }

    bHandled = TFR_NOT_HANDLED;
    return NULL;
}


