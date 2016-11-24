#pragma once
#include "unit.h"
#include "element.h"

namespace UI
{
namespace RT
{

//
// Ϊ��ͳһ���ã����е�unit������һ��element�ӿڣ���������дobject unit����鷳��
// ���ֻΪtext���ͱ���element�����object���͵�unit����element����
//
//
// ���е�object����unit�������������
//

    class ObjectElement : public Element
    {
    public:
        ObjectElement()
        {

        }

        virtual NodeType  GetNodeType() {
            return NodeType::Object;
        }

        virtual void  Release()
        {
            // ObjectElement����Ҫ�ͷţ�����new������
        }
        virtual ElementType  GetElementType()
        {
            return ElementTypeObejct;
        }
        virtual void  Draw(HDC hDC, Run*, RECT* prcRun)
        {
            // return m_objUnit.Draw(hDC, prcRun);
        }
        virtual uint  GetWidth() override
        {
            UIASSERT(0);
            return 0;
        }
        virtual uint  GetHeight() override
        {
            UIASSERT(0);
            return 0;
        }
        virtual uint  GetCharacters() override
        {
            return 1;
        }
    };

#if 0
class ObjectUnit : public Unit
{
public:
    ObjectUnit(Doc& doc) : 
        Unit(doc), m_element(*this)
    {
        m_pFirstElement = &m_element;
    }

//     virtual uint  GetWidth() = 0;
//     virtual uint  GetHeight() = 0;

    // �ڲ��ֹ����У����󷵻����������Ĵ�С���ɸ��ݵ�ǰpage��С/��ʣ���ȶ�̬�仯��
    virtual SIZE  GetLayoutSize(SIZE pageContentSize, int lineRemain) = 0;
    virtual void  Draw(HDC hDC, RECT* prc) = 0;

    // ��ObjectUnit���з�װ��������ֻ��ʵ��GetLayoutSize, ����ʵ��LayoutMultiLine
    virtual void  LayoutMultiLine(MultiLineLayoutContext* pContext) final;
    virtual BOOL  OnSetCursor() override;

    Element&  GetElement()
    {
        return m_element;
    }

private:
    ObjectElement  m_element;
};
#endif
}
}