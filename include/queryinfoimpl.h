//
// (C) Copyright by Victor Derks <vba64@xs4all.nl>
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "msfbase.h"


namespace MSF
{

class ATL_NO_VTABLE IQueryInfoImpl :
	public IQueryInfo
{
public:
	IQueryInfoImpl()
	{
		ATLTRACE2(atlTraceCOM, 0, _T("IQueryInfoImpl::IQueryInfoImpl (instance=%p)\n"), this);
	}


	~IQueryInfoImpl()
	{
		ATLTRACE2(atlTraceCOM, 0, _T("IQueryInfoImpl::~IQueryInfoImpl (instance=%p)\n"), this);
	}


	// IQueryInfo
	STDMETHOD(GetInfoFlags)(DWORD* /* pdwFlags */)
	{
		ATLTRACENOTIMPL(_T("IInfoTipImpl::GetInfoFlags"));
	}


	STDMETHOD(GetInfoTip)(DWORD dwFlags, WCHAR** ppwszTip)
	{
		(dwFlags); // unused in release.
		ATLTRACE2(atlTraceCOM, 0, _T("IInfoTipImpl::GetInfoTip (dwFlags=%d)\n"), dwFlags);
		return SHStrDup(m_strTip, ppwszTip);
	}

protected:

	void SetInfoTipText(const CString& strTip)
	{
		m_strTip = strTip;
	}

private:

	CString m_strTip;
};

} // namespace MSF
