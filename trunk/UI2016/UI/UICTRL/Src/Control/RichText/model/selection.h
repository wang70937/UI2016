#pragma once


namespace UI { namespace RT {
 
    class Element;
    class Unit;
    class Run;
    class Doc;
    class Node;

    // richedit��ѡ������
    // https://msdn.microsoft.com/zh-cn/library/windows/desktop/bb774037(v=vs.85).aspx

    // ��ҳ�淶
    // https://www.w3.org/TR/selection-api/
    // https://developer.mozilla.org/zh-CN/docs/Web/API/Selection
    //
    // DOMSelection 
    // http://www.netmite.com/android/mydroid/external/webkit/WebCore/page/DOMSelection.h
    //
    // �����
    //  ê�㣨anchor��
    //     êָ����һ��ѡ������ʼ�㣨��ͬ��HTML�е�ê�����ӣ�����ע����������ʹ������ѡһ�������ʱ��ê�����������갴��˲����Ǹ��㡣���û��϶����ʱ��ê���ǲ����ġ�
    //  ���㣨focus��
    //     ѡ���Ľ����Ǹ�ѡ�����յ㣬����������ѡһ��ѡ����ʱ�򣬽������������ɿ�˲������¼���Ǹ��㡣�����û��϶���꣬�����λ�û����Ÿı䡣
    //  ��Χ��range��
    //     ��Χָ�����ĵ���������һ���֡�һ����Χ���������ڵ㣬Ҳ���԰����ڵ��һ���֣������ı��ڵ��һ���֡��û�ͨ����ֻ��ѡ��һ����Χ
    //
    // ����
    //   anchorNode
    //     ���ظ�ѡ��������ڵĽڵ㣨Node����
    //   anchorOffset
    //    ����һ�����֣����ʾ����ѡ������� anchorNode �е�λ��ƫ������
    //     ��� anchorNode �����ֽڵ㣬��ô���صľ��ǴӸ����ֽڵ�ĵ�һ���ֿ�ʼ��ֱ����ѡ�еĵ�һ����֮��������������һ���־ͱ�ѡ�У���ôƫ����Ϊ�㣩��
    //     ��� anchorNode ��һ��Ԫ�أ���ô���صľ�����ѡ����һ���ڵ�֮ǰ��ͬ���ڵ�������(��Щ�ڵ㶼�� anchorNode ���ӽڵ�)
    //   focusOffset
    //     ����һ�����֣����ʾ����ѡ���յ��� focusNode �е�λ��ƫ������
    //      ��� focusNode �����ֽڵ㣬��ôѡ��ĩβδ��ѡ�еĵ�һ���֣��ڸ����ֽڵ����ǵڼ����֣���0��ʼ�ƣ����ͷ�������
    //      ��� anchorNode ��һ��Ԫ�أ���ô���صľ�����ѡ��ĩβ֮���һ���ڵ�֮ǰ��ͬ���ڵ�������
    //   isCollapsed
    //      ����һ������ֵ�������ж�ѡ������ʼ����յ��Ƿ���ͬһ��λ�á�


    // ÿ��������Ҳ��ʶ���Լ���ѡ��״̬
    enum class SelectionState {
        None,   // The object is not selected.
        Selected,
    };

    // ���������ַ����������ķ�Χ���ַ����ȴ�1
    class CaretPos  
    {
    public:
        //operator uint() { return pos; }
        CaretPos() : pos(0) {}
        CaretPos(uint n) : pos(n){}
        CaretPos& operator= (uint n) { pos = n; return *this; }

    public:
        uint pos;
    };
    inline void SortCaretPos(CaretPos& start, CaretPos& end)
    {
        if (start.pos > end.pos)
        {
            swap(start.pos, end.pos);
        }
    }

    //
    // ѡ��
    // 1. ��elementΪѡ��������λ����Ϊrun�ǲ��ֶ��󣬲��ǹ̶��ģ�����������Ϊѡ��������Ԫ��
    // 2. ÿ��element������һ��selection state�������Լ���ǰ�Ƿ�ѡ�У�����element���ơ�
    //    ÿ��ѡ�������ı�ʱ�������element��selection state��
    //
    class Selection //: public ISelection
    {
    public:
        Selection(Doc&);

//         virtual INode*  GetAnchorNode() override;
//         virtual INode*  GetFocusNode() override;
        Node*  GetAnchor();
        Node*  GetFocus();

        CaretPos  GetAnchorOffset();
        CaretPos  GetFocusOffset();

        void  SetAnchor(Node* p, CaretPos offset);
        void  SetFocus(Node* p, CaretPos offset);
        void  SetAnchorFocus(Node* p, CaretPos offset);
        void  Clear();
        bool  IsEmpty();
        void  ToString(String& str);

        bool  GetSelectionRange(Node* p, CaretPos& start, CaretPos& end);
        bool  GetSelectionRange(Run* p, CaretPos& start, CaretPos& end);

    private:
        void  clear_state();
        void  set_state_range(Node* p1, Node* p2, SelectionState s);

        void  update_state();

    private:
        Doc&  m_doc;

        Node*  m_pAnchor = nullptr;
        CaretPos  m_nAnchorOffset;
        Node*  m_pFocus = nullptr;
        CaretPos  m_nFocusOffset;
    };
}
}
