//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "queryinfoimpl.h"


namespace MSF
{
class ATL_NO_VTABLE CQueryInfo :
	public CComObjectRootEx<CComSingleThreadModel>,
	public IQueryInfoImpl
{
public:
	static CComPtr<IQueryInfo> CreateInstance(const CString& strTip)
	{
		CComObject<CQueryInfo>* p;

		RaiseExceptionIfFailed(CComObject<CQueryInfo>::CreateInstance(&p));
		p->SetInfoTipText(strTip);

		return p;
	}

	DECLARE_NOT_AGGREGATABLE(CQueryInfo)

	BEGIN_COM_MAP(CQueryInfo)
		COM_INTERFACE_ENTRY(IQueryInfo)
	END_COM_MAP()
};

} // namespace MSF
