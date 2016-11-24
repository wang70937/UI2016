#pragma once
#include "Inc/Util/color.h"

namespace UI
{
interface IColorResItem;
interface IColorRes;
class SkinRes;

//
//	color�е�һ����Ϣ����
//
class  ColorResItem
{
public:
	ColorResItem();
	~ColorResItem();

    IColorResItem*  GetIColorResItem();
	LPCTSTR GetColorString();
	bool  IsMyColor(Color* p);

private:
	String    m_strId;
	String    m_strColor;     // ��ɫ�ַ���
    ColorPtr  m_pColor;

	COLORREF*  m_pOriginColorValue;  // ��COLORδ�ı�ɫ��ǰ����ɫֵ�����ڻ�ԭ

	// TODO: �����ٽ�����չ���������ֵĺڰ���ɫ�任���Ǹ������ֺ���ı���ɫ�������ģ�������������
	bool      m_bUseSkinHLS;  // ��COLOR�Ƿ����Ƥ��ɫ���ı� 

public:
	void  SetId(LPCTSTR ID);
	LPCTSTR  GetId();
	const String&   GetIdRef();
	void  SetUseSkinHLS(bool b);
	bool  GetUseSkinHLS();
	const String&   GetColorStringRef();
	bool  GetColor(ColorPtr* pColor, bool *bFirstTimeCreate = NULL);
	void  SetColor(const String& strColor); 

	void  SetAttribute(IMapAttribute* p);
	bool  ModifyHLS(short h, short l, short s, int nFlag);

private:
    IColorResItem*  m_pIColorResItem;
};

//
//	color�б�
//
class ColorRes
{
public:
	ColorRes(SkinRes* p);
	~ColorRes();

    IColorRes&  GetIColorRes();

	long  GetColorCount();
	IColorResItem*  GetColorResItem(long lIndex);
	void  GetColor(LPCTSTR szColorId, Color** pp);
	LPCTSTR  GetColorId(Color* p);

public:
	ColorResItem* GetColorItem( int nIndex );
	ColorResItem* GetColorItem( const String& strID );
	bool  InsertColor( const String& strID, LPCTSTR szColor, ColorResItem** pItem );
	bool  ModifyColor( const String& strID, const String& strColor );
	bool  RemoveColor( const String& strID );
	void  Clear();

	bool  LoadItem(IMapAttribute* pMapAttrib, LPCTSTR szData);
	bool  ChangeSkinHLS(short h, short l, short s, int nFlag);

private:
	SkinRes*  m_pSkinRes;
    IColorRes*  m_pIColorRes;
    vector<ColorResItem*>   m_vColors;
};

}