#ifndef IUIACCESSIBLE_H_201505031655
#define IUIACCESSIBLE_H_201505031655

namespace UI
{

// 2015-5-4 22:27
// ����ʵ�ֿؼ���accessible���������μ�IUIAccessibleImpl��ʵ��
// Ĭ�ϵĴ�����objectAccessible��ɣ�
// һЩ����Ĵ�������editֻ��չget_accValue���ɡ�
//
// ����û�в��õ�����ʽ������ֱ���ṩͷ�ļ�

interface IUIAccessible
{
public:
	IUIAccessible()
	{
		m_lRef = 0;
	}
	virtual ~IUIAccessible()
	{
	}

	virtual long  AddRef()
	{
		m_lRef++;
		return m_lRef;
	}
	virtual long  Release()
	{
		m_lRef--;
		if (0 == m_lRef)
		{
			delete this;
			return 0;
		}
		return m_lRef;
	}

	virtual HRESULT  get_accParent( 
			IDispatch **ppdispParent, 
			BOOL& bHandled) 
	{
		bHandled = FALSE;
		return E_NOTIMPL;
	}

	virtual HRESULT  get_accChildCount( 
			long *pcountChildren, 
			BOOL& bHandled) 
	{
		bHandled = FALSE;
		return E_NOTIMPL;
	}

	virtual HRESULT  get_accChild(VARIANT varChild,
			IDispatch **ppdispChild, BOOL& bHandled) 
	{
		bHandled = FALSE;
		return E_NOTIMPL;
	}

	virtual HRESULT  get_accRole(VARIANT varChild,
			VARIANT *pvarRole, BOOL& bHandled) 
	{
		bHandled = FALSE;
		return E_NOTIMPL;
	}

	virtual HRESULT  get_accName(VARIANT varChild,
			BSTR *pszName, BOOL& bHandled) 
	{
		bHandled = FALSE;
		return E_NOTIMPL;
	}

	virtual HRESULT  get_accDescription(VARIANT varChild,
			BSTR *pszDescription, BOOL& bHandled) 
	{
		bHandled = FALSE;
		return E_NOTIMPL;
	}

	virtual HRESULT get_accValue(VARIANT varChild,
			BSTR* pszValue, BOOL& bHandled) 
	{
		bHandled = FALSE;
		return E_NOTIMPL;
	}

	virtual HRESULT  get_accState(VARIANT varChild,
			VARIANT *pvarState, BOOL& bHandled) 
	{
		bHandled = FALSE;
		return E_NOTIMPL;
	}

	virtual HRESULT  accNavigate(long navDir, VARIANT varStart,
			VARIANT *pvarEndUpAt, BOOL& bHandled) 
	{
		bHandled = FALSE;
		return E_NOTIMPL;
	}

	virtual HRESULT  accHitTest(long xLeft, long yTop,
			VARIANT *pvarChild, BOOL& bHandled) 
	{
		bHandled = FALSE;
		return E_NOTIMPL;
	}

	virtual HRESULT  accLocation( 
			long *pxLeft,
			long *pyTop,
			long *pcxWidth,
			long *pcyHeight,
			VARIANT varChild, 
			BOOL& bHandled) 
	{
		bHandled = FALSE;
		return E_NOTIMPL;
	}

private:
	long  m_lRef;
};

}

#endif // IUIACCESSIBLE_H_201505031655
