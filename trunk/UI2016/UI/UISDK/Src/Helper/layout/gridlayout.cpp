#include "stdafx.h"
#include "gridlayout.h"
#include "Src\Base\Object\object.h"
#include "Src\Base\Attribute\attribute.h"
#include "Src\Base\Attribute\long_attribute.h"
#include "Src\Base\Attribute\flags_attribute.h"

using namespace UI;

GridWH::GridWH()
{
	this->last = 0;
	this->xml = 0;
	this->type = GWHT_AUTO;
}

GridLayout::GridLayout()
{
}
GridLayout::~GridLayout()
{
	this->widths.clear();
	this->heights.clear();
}

void  GridLayout::LoadGridWidth(LPCTSTR szWidth)
{
    if (!szWidth)
        return;

    Util::ISplitStringEnum*  pEnum = NULL;
    int nCount = Util::SplitString(szWidth, XML_SEPARATOR, &pEnum);
    for (int i = 0; i < nCount; i++)
    {
        String  str = pEnum->GetText(i);

        // �ж��Ƿ�Ϊ AUTO ����
        if (XML_AUTO == str)
        {
            GridWH  wh;
            wh.type = GWHT_AUTO;

            this->widths.push_back( wh );
            continue;
        }

        // �ж��Ƿ�Ϊ * ����
        String::size_type nIndex = str.find(XML_ASTERISK);
        if (String::npos != nIndex)
        {
            GridWH  wh;
            wh.type = GWHT_ASTERISK;

            int nXml = _ttoi(str.c_str());
            if (nXml == 0)  // ���ָֻ�� "*"����ô��ʾ"1*"
                nXml = 1;

            wh.xml = nXml;
            this->widths.push_back( wh );
            continue;
        }

        // �ж��������
        GridWH  wh;
        wh.type = GWHT_VALUE;
        wh.last = _ttoi( str.c_str() );
        this->widths.push_back( wh );

        continue;
    }
    SAFE_RELEASE(pEnum);
}
LPCTSTR  GridLayout::SaveGridWidth()
{
    String&  strBuffer = GetTempBufferString();
    TCHAR*  szBuffer = GetTempBuffer();

    int nCount = (int)widths.size();
    for (int i = 0; i < nCount; i++)
    {
        if (!strBuffer.empty())
            strBuffer.push_back(XML_SEPARATOR);

        switch (widths[i].type)
        {
        case GWHT_ASTERISK:
            {
                if (widths[i].xml <= 1)
                {
                    strBuffer.append(XML_ASTERISK);
                }
                else
                {
                    _stprintf(szBuffer, TEXT("%d%s"), widths[i].xml, XML_ASTERISK);
                    strBuffer.append(szBuffer);
                }
            }
            break;

        case GWHT_AUTO:
            {
                strBuffer.append(XML_AUTO);
            }
            break;

        default://  GWHT_VALUE:
            {
                _stprintf(szBuffer, TEXT("%d"), widths[i].last);
                strBuffer.append(szBuffer);
            }
            break;
        }
    }

    return strBuffer.c_str();
}

void  GridLayout::LoadGridHeight(LPCTSTR szHeight)
{
    Util::ISplitStringEnum*  pEnum = NULL;
    int nCount = Util::SplitString(szHeight, XML_SEPARATOR, &pEnum);
    for (int i = 0; i < nCount; i++)
    {
        String str = pEnum->GetText(i);

        // �ж��Ƿ�Ϊ AUTO ����
        if (XML_AUTO == str)
        {
            GridWH  wh;
            wh.type = GWHT_AUTO;

            this->heights.push_back( wh );
            continue;
        }

        // �ж��Ƿ�Ϊ * ����
        String::size_type nIndex = str.find(XML_ASTERISK);
        if (String::npos != nIndex)
        {
            GridWH  wh;
            wh.type = GWHT_ASTERISK;

            int nXml = _ttoi(str.c_str());
            if( nXml == 0 )  // ���ָֻ�� "*"����ô��ʾ"1*"
                nXml = 1;

            wh.xml = nXml;

            this->heights.push_back( wh );
            continue;
        }

        // �ж��������
        GridWH  wh;
        wh.type = GWHT_VALUE;
        wh.last = _ttoi( str.c_str() );
        this->heights.push_back( wh );

        continue;
    }
    SAFE_RELEASE(pEnum);
}
LPCTSTR  GridLayout::SaveGridHeight()
{
    String&  strBuffer = GetTempBufferString();
    TCHAR*  szBuffer = GetTempBuffer();

    int nCount = (int)heights.size();
    for (int i = 0; i < nCount; i++)
    {
        if (!strBuffer.empty())
            strBuffer.push_back(XML_SEPARATOR);

        switch (heights[i].type)
        {
        case GWHT_ASTERISK:
            {
                if (heights[i].xml <= 1)
                {
                    strBuffer.append(XML_ASTERISK);
                }
                else
                {
                    _stprintf(szBuffer, TEXT("%d%s"), heights[i].xml, XML_ASTERISK);
                    strBuffer.append(szBuffer);
                }
            }
            break;

        case GWHT_AUTO:
            {
                strBuffer.append(XML_AUTO);
            }
            break;

        default://  GWHT_VALUE:
            {
                _stprintf(szBuffer, TEXT("%d"), heights[i].last);
                strBuffer.append(szBuffer);
            }
            break;
        }
    }

    return strBuffer.c_str();
}
void  GridLayout::Serialize(SERIALIZEDATA* pData)
{
    {
        AttributeSerializer s(pData, TEXT("GridLayout"));

		// ����Grid��ÿһ�еĿ��
        s.AddString(XML_LAYOUT_GRID_WIDTH, this, 
            memfun_cast<pfnStringSetter>(&GridLayout::LoadGridWidth),
            memfun_cast<pfnStringGetter>(&GridLayout::SaveGridWidth));

        s.AddString(XML_LAYOUT_GRID_HEIGHT, this, 
            memfun_cast<pfnStringSetter>(&GridLayout::LoadGridHeight),
            memfun_cast<pfnStringGetter>(&GridLayout::SaveGridHeight));
    }

    if (pData->IsLoad())
    {
        // �������gridlayoutû�ж���layout.width����Ĭ��layout.width="*"
        if (widths.empty())
        {
            GridWH  wh;
            wh.type = GWHT_ASTERISK;
            wh.xml = 1;
            this->widths.push_back( wh );
        }

        // �������gridlayoutû�ж���layout.height����Ĭ��layout.height="*"
        if (heights.empty())
        {
            GridWH  wh;
            wh.type = GWHT_ASTERISK;
            wh.xml = 1;
            this->widths.push_back( wh );
        }
    }
}


SIZE  GridLayout::Measure()
{
	SIZE size = {0,0};

	int nGridRows = (int)this->heights.size();
	int nGridCols = (int)this->widths.size();

	Object*   pChild = NULL;
	while (pChild = this->m_pPanel->EnumChildObject(pChild))
	{
		if (pChild->IsSelfCollapsed())
        {
			continue;
        }

        GridLayoutParam*  pParam = s_GetObjectLayoutParam(pChild);
        if (!pParam)
        {
            continue;;
        }

		int nRow = pParam->GetRow();
        int nCol = pParam->GetCol();
        int nnGridRowspan = pParam->GetRowSpan();
        int nnGridColspan = pParam->GetColSpan();

		// �����˷�Χ
		if (nCol >= nGridCols || nRow >= nGridRows)
		{
			UI_LOG_WARN( _T("GridLayout::MeasureChildObject, Object[ m_strID=\"%s\", col=%d, row=%d ] ����grid��Χ [col=%d,row=%d] )"),
				pChild->GetId(), nCol, nRow, nGridCols, nGridRows );
			continue;
		}
 
		// ���ڿ�͸߶����ںϲ���Ԫ��Ķ��󣬷��������ļ���
		// ͬʱ�����Ժϲ��л�ϲ��еļ��㣬���Ƕ���һ���ϲ��У������ֻռһ�еĻ�
		// ���������еĴ�С
		if (nnGridRowspan != 1 && nnGridColspan != 1)
		{
				continue;
		}
	
		// ����������ڵ��к��ж��й̶���ֵ����ô����Ҫ����
		if (widths[nCol].type == GWHT_VALUE &&
			heights[nRow].type == GWHT_VALUE)
		{
			continue;
		}

		SIZE s = pChild->GetDesiredSize();
		
		// ���ö��������еĿ��
		if (nnGridColspan == 1)
		{
			switch (widths[nCol].type)
			{
			case GWHT_VALUE:     // ���е�ֵ�ǹ̶�ֵ������Ҫ�޸�
				break;

			case GWHT_ASTERISK:  // ������ƽ�ִ�С����������ȡ��һ�е����ֵ����Arrange������پ��帳ֵ
				if( s.cx > widths[nCol].last )
					widths[nCol].last = s.cx;
				break;

			case GWHT_AUTO:      // �������Զ���С����ôȡ��һ�е����ֵ
				if (s.cx > widths[nCol].last)
					widths[nCol].last = s.cx;
				break;

			default:
				UIASSERT(false);
				break;
			}
		}

		//���ö��������еĸ߶�
		if (nnGridRowspan == 1)
		{
			switch (heights[nRow].type)
			{
			case GWHT_VALUE:     // ���е�ֵ�ǹ̶�ֵ������Ҫ�޸�
				break;

			case GWHT_ASTERISK:  // ������ƽ�ִ�С����������ȡ��һ�е����ֵ����Arrange������پ��帳ֵ
				if (s.cy > heights[nRow].last)
					heights[nRow].last = s.cy;
				break;

			case GWHT_AUTO:      // �������Զ���С����ôȥ��һ�е����ֵ
				if (s.cy > heights[nRow].last)
					heights[nRow].last = s.cy;
				break;

			default:
				UIASSERT(false);
				break;
			}
		}
	}// end of while( pChild != this->m_pPanel->EnumChildObject( pChild ) )

	// ���뽫���� * ���͵ı�����ϵ�����ã��� 1* : 2* : 3* �����ʱ������ֱ��Ӧ 5, 5, 10��
	// ��ô���밲�ųɡ�5,10,15��������С�Ǹ���
	// ������Ϊ���ȷֱ����������ϵ��5/1 : 5/2 : 10/3 = 5 : 2.5 : 3.3
	// ȡ�����Ǹ���ֵ���Ը���ֵ��Ϊ�µı���������ȥ���¼����� 1*5 : 2*5 : 3*5
	
	double maxRate = 0;
	// �����������
	for (int i = 0; i < nGridCols; i++)
	{
		if (this->widths[i].type == GWHT_ASTERISK)
		{
			if (widths[i].xml == 0)
				widths[i].xml = 1;

			double rate = (double)widths[i].last / (double)widths[i].xml;
			if (rate > maxRate)
				maxRate = rate;
		}
	}
	// ������Ļ������¼���
	for (int i = 0; i < nGridCols; i++)
	{
		if (this->widths[i].type == GWHT_ASTERISK)
		{
			widths[i].last = (int)(widths[i].xml * maxRate);
		}
	}

	// ͬ��������еĸ߶�
	maxRate = 0;
	for (int i = 0; i < nGridRows; i++)
	{
		if (this->heights[i].type == GWHT_ASTERISK)
		{
			if (heights[i].xml == 0)
				heights[i].xml = 1;

			double rate = (double)heights[i].last / (double)heights[i].xml;
			if (rate > maxRate)
				maxRate = rate;
		}
	}
	for (int i = 0; i < nGridRows; i++)
	{ 
		if (this->heights[i].type == GWHT_ASTERISK)
		{ 
			heights[i].last =(int)(heights[i].xml * maxRate);
		}
	}

	// ����panel��Ҫ�Ĵ�С
	for (int i = 0; i < nGridCols; i++)
	{
		size.cx += widths[i].last;
	}
	for (int i = 0; i < nGridRows; i++)
	{
		size.cy += heights[i].last;
	}

    return size;
}
void  GridLayout::DoArrage(IObject* pObjToArrage)
{
	// ���øú���ʱ���Լ��Ĵ�С�Ѿ����������

	// �����GRID�Ŀ�Ⱥ͸߶ȣ�����Ը����Ӷ���Ĳ��ֶ����Ȼ���GRID��
	CRect rcClient;
	m_pPanel->GetClientRectInObject(&rcClient);
	int  nWidth  = rcClient.Width();  //m_pPanel->GetWidth();
	int  nHeight = rcClient.Height(); //m_pPanel->GetHeight();

	int nGridRows = (int)this->heights.size();  // Grid������
	int nGridCols = (int)this->widths.size();   // Grid������

	// ��ʼ����������һ�εĽ��Ӱ�챾�εĲ���
	for (int i = 0; i < nGridCols; i++)
	{
		if (widths[i].type == GWHT_AUTO)
		{
			widths[i].last = 0;
		}
	}
	for (int i = 0; i < nGridRows; i++)
	{
		if (heights[i].type == GWHT_AUTO)
		{
			heights[i].last = 0;
		}
	}

    // ��һ��. �����Ȱ���auto����
	Object*   pChild = NULL;
	while (pChild = this->m_pPanel->EnumChildObject(pChild))
	{
		// ����IsSelfCollapsed֮ǰ��editor��Ҳ��Ҫ�������ض���Ĳ�������
		GridLayoutParam*  pParam = s_GetObjectLayoutParam(pChild);
		if (!pParam)
			continue;;

		if (pChild->IsSelfCollapsed())
			continue;

        int nRow = pParam->GetRow();
        int nCol = pParam->GetCol();
        // int nnGridRowspan = pParam->GetRowSpan();
        // int nnGridColspan = pParam->GetColSpan();

		// �����˷�Χ
		if (nCol >= nGridCols || nRow >= nGridRows)
		{
			UI_LOG_WARN( _T("GridLayout::ArrangeChildObject, Object[ m_strID=\"%s\", col=%d, row=%d ] ����grid��Χ [col=%d,row=%d] )"),
				pChild->GetId(), nCol, nRow, nGridCols, nGridRows );
			continue;
		}

		// ֻ��auto����
		if (widths[nCol].type != GWHT_AUTO && heights[nRow].type != GWHT_AUTO)
		{
			continue ;
		}

		SIZE s = pChild->GetDesiredSize();

		if (widths[nCol].type == GWHT_AUTO)
		{
			if( widths[nCol].last < s.cx )
				widths[nCol].last = s.cx;
		}
		if (heights[nRow].type == GWHT_AUTO)
		{
			if (heights[nRow].last < s.cy)
				heights[nRow].last = s.cy;
		}
	}
	
	// �ڶ���. ����*����
	int nWidthRemain  = nWidth;   // ��ȥauto��value���ͺ�ʣ��Ŀ��
	int nHeightRemain = nHeight;  // ��ȥauto��value���ͺ�ʣ��Ŀ��
	int nASTERISKWidth  = 0;      // ����е�*������
	int nASTERISKHeight = 0;      // �߶��е�*������

	for (int i = 0; i < nGridCols; i++)
	{
		if (widths[i].type != GWHT_ASTERISK)
		{
			nWidthRemain -= widths[i].last;
		}
		else
		{
			nASTERISKWidth += widths[i].xml;
		}
	}
	for (int i = 0; i < nGridRows; i++)
	{
		if (heights[i].type != GWHT_ASTERISK)
		{
			nHeightRemain -= heights[i].last;
		}
		else
		{
			nASTERISKHeight += heights[i].xml;
		}
	}

	if (nASTERISKWidth == 0)
		nASTERISKWidth = 1;
	if (nASTERISKHeight == 0)
		nASTERISKHeight = 1;

	// ��ʼƽ��(TODO. ���������ĳ��������������ɼ������ص���
	for (int i = 0; i < nGridCols; i++)
	{
		if (widths[i].type == GWHT_ASTERISK)
		{
			widths[i].last = nWidthRemain / nASTERISKWidth * widths[i].xml;
		}
	}
	for (int i = 0; i < nGridRows; i++)
	{
		if (heights[i].type == GWHT_ASTERISK)
		{
			heights[i].last = nHeightRemain / nASTERISKHeight * heights[i].xml;
		}
	}


	// ������. ���Ÿ����ؼ���λ��
	pChild = NULL;
	while (pChild = this->m_pPanel->EnumChildObject(pChild))
	{
        GridLayoutParam*  pParam = s_GetObjectLayoutParam(pChild);
        if (!pParam)
        {
            continue;;
        }

        int nRow = pParam->GetRow();
        int nCol = pParam->GetCol();
        int nnGridRowspan = pParam->GetRowSpan();
        int nnGridColspan = pParam->GetColSpan();


		// (row,col) -> ( rect.left, rect.top, rect.right, rect.bottom )
		// (0,0)   -> (0,0,widths[0],heights[0])
		// (1,1)   -> (widths[0],heights[0],widths[0]+widths[1],heights[0]+heights[1] )

		RECT  rcObjectInGrid;
		rcObjectInGrid.left   = this->getColPos( nCol );
		rcObjectInGrid.right  = this->getColPos( nCol + nnGridColspan );
		rcObjectInGrid.top    = this->getRowPos( nRow );
		rcObjectInGrid.bottom = this->getRowPos( nRow + nnGridRowspan );

		CRect rc;
		rc.CopyRect(&rcObjectInGrid);
		rc.OffsetRect(this->m_pPanel->GetPaddingL(), this->m_pPanel->GetPaddingT());

 		CRegion4 rcMargin;
 		pChild->GetMarginRegion(&rcMargin);
 		Util::DeflatRect(&rc, &rcMargin);

        int nConfigW = pParam->GetConfigWidth();
        int nConfigH = pParam->GetConfigHeight();
        if (nConfigW >= 0)
            rc.right = rc.left + nConfigW;
        if (nConfigH >= 0)
            rc.bottom = rc.top + nConfigH;
	
		pChild->SetObjectPos(&rc, SWP_NOREDRAW|SWP_NOUPDATELAYOUTPOS|SWP_FORCESENDSIZEMSG);
	}
}

// ��ȡ��nCol����GRID�����ľ���
// ע�⣺����Ĳ��������Ƕ����col+colspan����˸�ֵ���ܻᳬ��grid�������
int GridLayout::getColPos(unsigned int nCol)
{
	unsigned int nGridCols = widths.size();
	if (nCol >= nGridCols)
	{
	//	UI_LOG_WARN( _T("GridLayout::getColPos, nCol[%d] > widths.size[%d]"), nCol, nGridCols );
		nCol = nGridCols;
	}

	int nRet = 0;
	for (unsigned int i = 0; i < nCol; i++)
	{
		nRet += widths[i].last;
	}
	return nRet;
}
// ��ȡ��nRow����GRID������ľ���
int GridLayout::getRowPos(unsigned int nRow )
{
	unsigned int nGridRows = heights.size();
	if (nRow >= nGridRows)
	{
	//	UI_LOG_WARN( _T("GridLayout::getRowPos, nCol[%d] > heights.size[%d]"), nRow, nGridRows );
		nRow = nGridRows;
	}

	int nRet = 0;
	for (unsigned int i = 0; i < nRow; i++)
	{
		nRet += heights[i].last;
	}
	return nRet;
}

GridWH*  GridLayout::GetWidth(unsigned int nIndex)
{
    if (nIndex >= widths.size())
        return NULL;

    return &widths[nIndex];
}
GridWH*  GridLayout::GetHeight(unsigned int nIndex)
{
    if (nIndex >= heights.size())
        return NULL;

    return &heights[nIndex];
}


void  GridLayout::ChildObjectVisibleChanged(IObject* pObj)
{
	UIASSERT(pObj);
	UIASSERT(pObj->GetParentObject());
	UIASSERT(pObj->GetParentObject()->GetImpl() == m_pPanel);

    pObj->Invalidate();
}


//////////////////////////////////////////////////////////////////////////

GridLayoutParam::GridLayoutParam()
{
    m_nConfigWidth = AUTO;
    m_nConfigHeight = AUTO;
    m_nConfigLayoutFlags = 0;

    m_nCol = m_nRow = 0;
    m_nColSpan = m_nRowSpan = 1;
}
GridLayoutParam::~GridLayoutParam()
{

}

void  GridLayoutParam::UpdateByRect()
{
    CRect  rcParent;
    m_pObj->GetParentRect(&rcParent);

    if (m_nConfigWidth >= 0)
    {
        m_nConfigWidth = rcParent.Width();
    }
    if (m_nConfigHeight >= 0)
    {
        m_nConfigHeight = rcParent.Height();
    }
}
void  GridLayoutParam::Serialize(SERIALIZEDATA* pData)
{
    AttributeSerializer s(pData, TEXT("GridLayoutParam"));
    s.AddLong(XML_WIDTH, m_nConfigWidth)->AddAlias(AUTO, XML_AUTO)->SetDefault(AUTO);
    s.AddLong(XML_HEIGHT, m_nConfigHeight)->AddAlias(AUTO, XML_AUTO)->SetDefault(AUTO);
    s.AddLong(XML_LAYOUT_GRID_COL, m_nCol);
    s.AddLong(XML_LAYOUT_GRID_ROW, m_nRow);
    s.AddLong(XML_LAYOUT_GRID_COLSPAN, m_nColSpan)->SetDefault(1);
    s.AddLong(XML_LAYOUT_GRID_ROWSPAN, m_nRowSpan)->SetDefault(1);

    s.AddFlags(XML_LAYOUT_ITEM_ALIGN, m_nConfigLayoutFlags)
        ->AddFlag(LAYOUT_ITEM_ALIGN_LEFT, XML_LAYOUT_ITEM_ALIGN_LEFT)
        ->AddFlag(LAYOUT_ITEM_ALIGN_RIGHT, XML_LAYOUT_ITEM_ALIGN_RIGHT)
        ->AddFlag(LAYOUT_ITEM_ALIGN_TOP, XML_LAYOUT_ITEM_ALIGN_TOP)
        ->AddFlag(LAYOUT_ITEM_ALIGN_BOTTOM, XML_LAYOUT_ITEM_ALIGN_BOTTOM)
        ->AddFlag(LAYOUT_ITEM_ALIGN_CENTER, XML_LAYOUT_ITEM_ALIGN_CENTER)
        ->AddFlag(LAYOUT_ITEM_ALIGN_VCENTER, XML_LAYOUT_ITEM_ALIGN_VCENTER);
}

SIZE  GridLayoutParam::CalcDesiredSize()
{
    SIZE size = {0,0};

    bool bWidthNotConfiged = m_nConfigWidth == AUTO ? true:false;
    bool bHeightNotConfiged = m_nConfigHeight == AUTO ? true:false;;

    if (bWidthNotConfiged || bHeightNotConfiged)
    {
        // ��ȡ�Ӷ�������Ҫ�Ŀռ�
        UISendMessage(m_pObj, UI_MSG_GETDESIREDSIZE, (WPARAM)&size);

        // �����ָ��width��height������һ������ô��������һ���еõ���ֵ
        if (this->m_nConfigWidth != AUTO)
            size.cx = this->m_nConfigWidth;
        if (this->m_nConfigHeight!= AUTO)
            size.cy = this->m_nConfigHeight;
    }
    else
    {
        size.cx = this->m_nConfigWidth;
        size.cy = this->m_nConfigHeight;
    }

    // ���� margin �Ĵ�С
    size.cx += m_pObj->GetMarginW();
    size.cy += m_pObj->GetMarginH();

    return size;
}

bool  GridLayoutParam::IsSizedByContent()
{
    if (m_nConfigHeight != AUTO && m_nConfigWidth != AUTO)
        return false;

    // �жϸ��еĸ��Ƿ������ˣ����еĿ��Ƿ�������
    if (!m_pObj || !m_pObj->GetParentObject())
        return true;

    ILayout* pLayout = (ILayout*)UISendMessage(m_pObj->GetParentObject(), UI_MSG_GETLAYOUT);
    if (!pLayout)
        return true;

    GridLayout*  pGridLayout = static_cast<GridLayout*>(pLayout);
    
    for (int i = 0; i < m_nRowSpan; i++)
    {
        GridWH* p = pGridLayout->GetHeight(i);
        if (!p)
            return true;

        if (p->type == GWHT_AUTO)
            return true;
    }
    for (int i = 0; i < m_nColSpan; i++)
    {
        GridWH* p = pGridLayout->GetWidth(i);
        if (!p)
            return true;

        if (p->type == GWHT_AUTO)
            return true;
    }

    return false;
}
