#include "stdafx.h"
#include "element.h"
#include "..\run.h"
#include "unit.h"
#include "..\doc.h"

using namespace UI;
using namespace UI::RT;

Element::Element()
{
    // m_unit.GetDoc().SetNeedUpdateElementIndex();
}

Element::~Element()
{

}

void  Element::AddRun(Run* p)
{
	m_listRuns.push_back(p);
}
void  Element::ClearRun()
{
	m_listRuns.clear();
}

// ��ȡԪ��������
void  Element::GetRelativeLines(list<Line*>& listLines)
{
	list<Run*>::iterator iter = m_listRuns.begin();
	for (; iter != m_listRuns.end(); ++iter)
	{
		listLines.push_back((*iter)->GetLine());
	}
}

// ��ȡԪ���ڵĵ�һ����
Line*  Element::GetFirstLine()
{
    if (m_listRuns.empty())
        return nullptr;

    return (*(m_listRuns.begin()))->GetLine();
}

// ��ȡԪ���ڵ����һ��
Line*  Element::GetLastLine()
{
    if (m_listRuns.empty())
        return nullptr;

    return (*(m_listRuns.rbegin()))->GetLine();
}


void  Element::DrawSelectionBackground(HDC hDC, RECT* prc)
{
    HBRUSH hBrush = CreateSolidBrush(RGB(168,205,241));
    ::FillRect(hDC, prc, hBrush);
    DeleteObject(hBrush);
}

void  Element::LayoutSingleLine(SingleLineLayoutContext* context)
{
    context->PushElement(this);
}