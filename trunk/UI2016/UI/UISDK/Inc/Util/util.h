#ifndef _UI_UTIL_H_
#define _UI_UTIL_H_

namespace UI { 
	class  C9Region;

	namespace Util 
{
	COLORREF  TranslateHexColor(LPCTSTR szColor);
    COLORREF  UIAPI TranslateColor(LPCTSTR szColor);
	COLORREF  TranslateRGB(LPCTSTR szCol, TCHAR szSep = _T(','));
	bool  TranslateSIZE(LPCTSTR szText, SIZE* pSize, TCHAR szSep = _T(','));
	bool  TranslateRECT(LPCTSTR szRect, RECT* pRect, TCHAR szSep = _T(','));
	bool  TranslateImage9Region(LPCTSTR str, C9Region* p9Region, TCHAR szSep = _T(','));
    bool  UIAPI IsKeyDown(UINT vk);

    void  UIAPI DeflatRect(RECT* pfc, LPCRECT pDeflatRc);

	interface ISplitStringEnum
	{
		virtual void  Release() = 0;
		virtual LPCTSTR GetText(UINT nIndex) = 0;
		virtual int   GetCount() = 0;
	};
	UINT UIAPI SplitString(
		LPCTSTR szText,
		TCHAR szSep,
		ISplitStringEnum** pEnum
		);

	BOOL IsFullPath(LPCTSTR szPath);
	BOOL GetPathFileName(LPCTSTR szPath, TCHAR* szOutDir);
	BOOL GetPathFileExt(LPCTSTR szPath, TCHAR* szOutExt);

	enum FixAlphaMode
	{
		// ֱ�ӽ�alpha����Ϊ255
		SET_ALPHA_255,

		// �����ǰalpha==0����alpha����Ϊ255
		SET_ALPHA_255_IF_ALPHA_IS_0,

		// �����ǰ���ز���0����alpha����Ϊ255
		SET_ALPHA_255_IF_RGBA_ISNOT_0,

		// alpha ��ת 0->255, 255->0
		SET_ALPHA_INVERSE_0_255,

		// ��alpha����Ϊָ��ֵwParam
		SET_ALPHA_VALUE,

		// �����ǰalphaֵ��255, ��alpha����Ϊָ��ֵwParam
		// ����û��ֵԤ�˴���
		SET_ALPHA_VALUE_IF_ALPHA_IS_255

		// ...
		// �����ǰalpha==wParam����alpha����ΪlParam
		//	SET_ALPHA_IF_ALPHA_EQUAL,      

		// �����ǰalpha<wParam����alpha����ΪlParam
		//	SET_ALPHA_IF_ALPHA_LESS,       
	};
	struct FixAlphaData
	{
		// ����ָ����hDC��û��ָ��hBitmapʱ����hDC�л�ȡCurrentBitap
		HDC      hDC;
		HBITMAP  hBitmap;
		BOOL     bTopDownDib;
		RECT*    lprc;
		FixAlphaMode  eMode;
		WPARAM   wParam;
		LPARAM   lParam;
	};
	BOOL UIAPI FixBitmapAlpha(FixAlphaData* pData);
	BOOL UIAPI FixRenderTargetClipRegionAlpha(IRenderTarget*);

	void UIAPI PathInBin(LPCTSTR szRelative, TCHAR szAbsolution[MAX_PATH]);
}
}

#endif // _UI_UTIL_H_