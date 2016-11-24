#pragma once
#include "Inc\Interface\irichtext.h"

namespace UI
{
namespace RT
{

struct Style
{
    Style()
    {
        SetRectEmpty(&page_padding);
    }

    // hover/pressʱ�Զ����������»���
    bool  draw_link_underline = true;  

    // ����������뷽ʽ
    VALIGN  page_content_valign = VALIGN::top;  

    // ���ݺ�����뷽ʽ
    HALIGN  page_content_halign = HALIGN::left;  

    // ҳ�����ܿհ�
    REGION4  page_padding;

    // �м��
    long  line_space = 0;  
};

}
}