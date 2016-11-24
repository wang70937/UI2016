#pragma once
#include "element.h"
#include "unit.h"
#include "..\UISDK\Inc\Util\iimage.h"

// ����֧�ֶ���
namespace UI
{
namespace RT
{
class ImageElement : public Element
{
public:
    ImageElement();

    virtual NodeType  GetNodeType() {
        return NodeType::Image; 
    }
    virtual void LayoutMultiLine(MultiLineLayoutContext*) override;

    virtual void  Draw(HDC hDC, Run* run, RECT* prcRun) override;
    virtual uint  GetWidth() override;
    virtual uint  GetHeight() override;

    uint  GetImageWidth();
    uint  GetImageHeight();

public:
//     virtual void  Draw(HDC hDC, RECT* prcRun) override;
     SIZE  GetLayoutSize(SIZE pageContentSize, int lineRemain);

public:
    bool  Load(LPCTSTR szPath);

protected:
    ImageWrap  m_image;

    // ���Ƹ߶ȡ���ʵ�ʸ߶ȿ��ܲ�һ��������ͼƬ�ܴ�����
    SIZE  m_sizeDraw;
};
}
}