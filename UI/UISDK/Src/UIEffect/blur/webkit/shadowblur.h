#pragma once

// ����webkit�ķ�ʽ������Ӱ

// QT shadow blurע��
// https://gitorious.org/ofi-labs/x2/commit/2bb67f86d262ccf20fefccdb29056c092b2398cf?diffmode=sidebyside



namespace UI
{
	//
	// ������
	//
	//	hBitmap
	//		[in,out] ������32λ��top-downλͼ
	//  colorShadow
	//		[in] ��Ӱ��ɫ
	//	RECT
	//		[in] HBITMAP��Ҫ����blur���������ΪNULL��ʾ����ͼƬ��Χ
	//	nRadius
	//		[in] 2-128֮��
	//
    void  ShadowBlur(HBITMAP hBitmap, COLORREF colorShadow, RECT* prcBlur, int nRadius);
}