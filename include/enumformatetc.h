//
// (C) Copyright by Victor Derks <vba64@xs4all.nl>
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "msfbase.h"


namespace MSF
{


inline CComPtr<IEnumFORMATETC> SHCreateStdEnumFmtEtc(UINT cfmt, const FORMATETC* pformatetc)
{
	CComPtr<IEnumFORMATETC> renumformatetc;

	RaiseExceptionIfFailed(::SHCreateStdEnumFmtEtc(cfmt, pformatetc, &renumformatetc));

	return renumformatetc;
}


// Purpose: copy policy class: required for the ATL::CComEnumOnSTL template.
class CFORMATETCToFORMATETC
{
public:
	static void init(FORMATETC*)
	{
		// No init needed.
	}


	static HRESULT copy(FORMATETC* pTo, const FORMATETC* pFrom)
	{
		*pTo = *pFrom;
		return S_OK;
	}


	static void destroy(FORMATETC*)
	{
	}
};


class CEnumFORMATETC :
	public CComEnumOnSTL<IEnumFORMATETC,
	                     &IID_IEnumFORMATETC,     // name and IID of enumerator interface
	                     FORMATETC,               // type of object to return
	                     CFORMATETCToFORMATETC,   // copy policy class
	                     std::vector<FORMATETC> > // type of collection holding the data
{
public:
	typedef std::vector<FORMATETC> CFormatEtcs;

	static CComPtr<CEnumFORMATETC> CreateInstance(std::auto_ptr<CFormatEtcs> qformatetcs)
	{
		CComObject<CEnumFORMATETC>* pEnum;
		RaiseExceptionIfFailed(CComObject<CEnumFORMATETC>::CreateInstance(&pEnum));

		CComPtr<CEnumFORMATETC> renum(pEnum);
		renum->Init(qformatetcs);
		
		return renum;
	}


	CEnumFORMATETC() throw()
	{
		ATLTRACE2(atlTraceCOM, 0, _T("CEnumFORMATETC::CEnumFORMATETC (instance=%p)\n"), this);
	}


	~CEnumFORMATETC() throw()
	{
		ATLTRACE2(atlTraceCOM, 0, _T("CEnumFORMATETC::~CEnumFORMATETC (instance=%p)\n"), this);
	}


	// Purpose: 'Constructor' for our ATL based COM object.
	// Note: must be public for VC .NET 2002
	void Init(std::auto_ptr<CFormatEtcs> qformatetcs) throw()
	{
		_qformatetcs = qformatetcs;

		ATLVERIFY(SUCCEEDED(__super::Init(NULL, *_qformatetcs)));
	}

private:

	// Member variables.
	std::auto_ptr<CFormatEtcs> _qformatetcs;
};


// Purpose: alternative for the SHCreateStdEnumFmtEtc function.
//          Created before I discovered SHCreateStdEnumFmtEtc.
//          Source can act as sample, or as easy debug alternative.
//
inline HRESULT CreateStdEnumFmtEtc(std::auto_ptr<std::vector<FORMATETC> > qformatetcs, IEnumFORMATETC** ppenumFormatEtc)
{
	try
	{
		CComPtr<CEnumFORMATETC> renum = CEnumFORMATETC::CreateInstance(qformatetcs);
		*ppenumFormatEtc = renum.Detach();

		return S_OK;
	}
	MSF_COM_CATCH_HANDLER()
}

} // end of MSF namespace
