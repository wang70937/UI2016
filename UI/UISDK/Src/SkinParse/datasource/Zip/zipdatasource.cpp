#include "stdafx.h"
#include "zipdatasource.h"
#include "Inc\Util\iimage.h"
#include "Src\Atl\image.h"
#include "Src\SkinParse\datasource\Zip\bytebufferreader.h"
#include "Src\SkinParse\xml\xmlwrap.h"

namespace UI
{

ZipDataSource::ZipDataSource() : m_ISkinDataSource(this)
{
    m_hZip = NULL;
}
ZipDataSource::~ZipDataSource()
{
    if (m_hZip)
    {
        CloseZip(m_hZip);
        m_hZip = NULL;
    }
}

ZipDataSource*  ZipDataSource::Create()
{
	return new ZipDataSource();
}
void  ZipDataSource::Release()
{
	delete this;
}
ISkinDataSource*  ZipDataSource::GetISkinDataSource()
{
	return &m_ISkinDataSource;
}

SKIN_PACKET_TYPE  ZipDataSource::GetType()
{
    return SKIN_PACKET_TYPE_ZIP;
}

void  ZipDataSource::SetPath(LPCTSTR szPath)
{
	if (szPath)
		m_strPath = szPath;
	else
		m_strPath.clear();
}
LPCTSTR  ZipDataSource::GetPath()
{
	return m_strPath.c_str();
}

bool  ZipDataSource::Init()
{
    if (NULL == m_hZip)
    {
        m_hZip = OpenZip(m_strPath.c_str(), NULL);
        if (NULL == m_hZip)
        {
            UI_LOG_ERROR(_T("OpenZip Failed. file=%s"), m_strPath.c_str());
			return false;
        }
    }
	return true;
}

bool  ZipDataSource::Load_UIDocument(UIDocument* pDocument, LPCTSTR szPath)
{
    if (!m_hZip && !Init())
	{
		return false;
	}

    ZIPENTRY ze;
    int index = 0; 

    TCHAR szPath2[MAX_PATH];
    TranslatePath(szPath, szPath2);

    FindZipItem(m_hZip, szPath2, true, &index, &ze);
    if (-1 == index)
    {
        UI_LOG_ERROR(_T("FindZipItem Failed. path=%s"), szPath);
        return false;
    }

	int nSize = ze.unc_size;
    byte* szbuf = new byte[nSize];  // ���һ����0��䣬�����ַ�������
    memset(szbuf, 0, nSize);
    UnzipItem(m_hZip, index, szbuf, ze.unc_size);

    bool bRet = pDocument->LoadData(szbuf, nSize);    
    SAFE_ARRAY_DELETE(szbuf);
    return bRet;
}

void  ZipDataSource::TranslatePath(LPCTSTR szOrignPath, TCHAR* szLastPath)
{
    LPCTSTR p = szOrignPath;
    TCHAR* p2 = szLastPath;

    // ���� .\xxx ��ʾ�ĵ�ǰĿ¼
    if (_tcslen(szOrignPath) > 2 &&
        szOrignPath[0] == _T('.') && 
        (szOrignPath[1] == _T('/') || szOrignPath[1] == _T('\\')))
    {
        p += 2;
    }

    while (*p)
    {
        if (*p == '\\')
            *p2 = '/';
        else
            *p2 = *p;

        p++;
        p2++;
    }
    *p2 = _T('');
}

// ע��zip�ڲ���·��������/��������\ 
bool  ZipDataSource::Load_RenderBitmap(IRenderBitmap* pBitmap, LPCTSTR szPath, RENDER_BITMAP_LOAD_FLAG e)
{
    if (!m_hZip || !Init())
        return false;

    if (NULL == pBitmap || NULL == szPath)
        return false;

    ZIPENTRY ze;
    int index = 0; 

    // �滻·������
    TCHAR szPath2[MAX_PATH] = _T("");
    TranslatePath(szPath, szPath2);
	
    FindZipItem(m_hZip, szPath2, true, &index, &ze);
    if (-1 == index)
    {
        UI_LOG_ERROR(_T("FindZipItem Failed. path=%s"), szPath);
        return false;
    }

    char* szbuf = new char[ze.unc_size];
    memset(szbuf, 0, ze.unc_size);
    UnzipItem(m_hZip, index, szbuf, ze.unc_size);

    bool bRet = pBitmap->LoadFromData((byte*)szbuf, ze.unc_size, e);
    delete[] szbuf;

    return bRet;
}

bool  ZipDataSource::Load_Image(LPCTSTR szPath, ImageWrap* pImage)
{
    if (!m_hZip || !Init())
        return false;

    if (NULL == pImage || NULL == szPath)
        return false;

    ZIPENTRY ze;
    int index = 0; 

    // �滻·������
    TCHAR szPath2[MAX_PATH] = _T("");
    TranslatePath(szPath, szPath2);

    FindZipItem(m_hZip, szPath2, true, &index, &ze);
    if (-1 == index)
    {
        UI_LOG_ERROR(_T("FindZipItem Failed. path=%s"), szPath);
        return false;
    }

    char* szbuf = new char[ze.unc_size];
    memset(szbuf, 0, ze.unc_size);
    UnzipItem(m_hZip, index, szbuf, ze.unc_size);

    pImage->GetImpl()->LoadFromData((byte*)szbuf, ze.unc_size);
    delete[] szbuf;

    return pImage->IsNull()? false:true;
}

bool  ZipDataSource::Load_GdiplusImage(LPCTSTR szPath, GdiplusBitmapLoadWrap* pImage)
{
    if (!m_hZip || !Init())
        return false;

    if (NULL == pImage || NULL == szPath)
        return false;

    ZIPENTRY ze;
    int index = 0; 

    // �滻·������
    TCHAR szPath2[MAX_PATH] = _T("");
    TranslatePath(szPath, szPath2);

    FindZipItem(m_hZip, szPath2, true, &index, &ze);
    if (-1 == index)
    {
        UI_LOG_ERROR(_T("FindZipItem Failed. path=%s"), szPath);
        return false;
    }

    char* szbuf = new char[ze.unc_size];
    memset(szbuf, 0, ze.unc_size);
    UnzipItem(m_hZip, index, szbuf, ze.unc_size);

    bool bRet = pImage->LoadFromByte((byte*)szbuf, ze.unc_size);
    delete[] szbuf;

    return bRet;
}

bool  ZipDataSource::Load_StreamBuffer(LPCTSTR szPath, IStreamBufferReader** pp)
{
    if (!m_hZip || !Init())
        return false;

    if (NULL == szPath || NULL == pp)
        return false;

    ZIPENTRY ze;
    int index = 0; 

    // �滻·������
    TCHAR szPath2[MAX_PATH] = _T("");
    TranslatePath(szPath, szPath2);

    FindZipItem(m_hZip, szPath2, true, &index, &ze);
    if (-1 == index)
    {
        UI_LOG_ERROR(_T("FindZipItem Failed. path=%s"), szPath);
        return false;
    }

    char* szbuf = new char[ze.unc_size];
    memset(szbuf, 0, ze.unc_size);
    UnzipItem(m_hZip, index, szbuf, ze.unc_size);

    ByteBufferReader* pBuffer = new ByteBufferReader;
    pBuffer->load((const byte*)szbuf, ze.unc_size, true);
    *pp = pBuffer;

    return true;
}

bool ZipDataSource::FileExist(LPCTSTR szPath)
{
	ZIPENTRY ze;
	int index = 0;

	// �滻·������
	TCHAR szPath2[MAX_PATH] = _T("");
	TranslatePath(szPath, szPath2);

	FindZipItem(m_hZip, szPath2, true, &index, &ze);

	return (-1 == index) ? false : true;
}

}
