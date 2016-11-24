#include "stdafx.h"
#include "object_unit.h"
#include "..\layout_context.h"
#include "..\line.h"

using namespace RT;

#if 0
BOOL ObjectUnit::OnSetCursor()
{
    SetCursor(LoadCursor(nullptr, IDC_ARROW));
    return TRUE;
}

void ObjectUnit::LayoutMultiLine(MultiLineLayoutContext* pContext)
{
    if (!pContext->pPage || !pContext->pLine)
    {
        UIASSERT(0);
        return;
    }

    // ��ǰҳ�����ݴ�С
    SIZE sizePageContent = {
        pContext->nPageContentWidth, pContext->nPageContentHeight
    };

    // ��ǰ��ʣ����
    int nLineRemain = pContext->nPageContentWidth - pContext->xLineOffset;

    // Ԫ��ʣ����
    SIZE  nObjSize = this->GetLayoutSize(sizePageContent, nLineRemain);

    // �����ܹ��Ų��¸�Ԫ��
    if (nObjSize.cx > nLineRemain)
    {
        // ���Ѿ���һ�����У�ǿ�Ʒ�
        if (pContext->pLine->HasRun())
        {
            // �Ų��£�����һ��
            pContext->AppendNewLine();
        }
    }

    Run* pRun = pContext->pLine->AppendRun(&GetElement(), nObjSize.cx, nObjSize.cy);
    pRun->SetRange(0, 0);
    pRun->SetPosInLine(pContext->xLineOffset);

    // ���з���
    pContext->xLineOffset += nObjSize.cx;
    if (pContext->xLineOffset >= pContext->nPageContentWidth)
        pContext->SetLineFinish();
}
#endif