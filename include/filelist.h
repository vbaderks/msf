//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "msfbase.h"
#include <atlctl.h>
#include "stgmedium.h"
#include "catchhandler.h"
#include "strutil.h"


namespace MSF
{

class CFileList :
	public CComObjectRootEx<CComSingleThreadModel>,
	public ATL::IDataObjectImpl<CFileList>
{
public:
	DECLARE_NOT_AGGREGATABLE(CFileList)

	BEGIN_COM_MAP(CFileList)
		COM_INTERFACE_ENTRY(IDataObject)
	END_COM_MAP()


	static CComPtr<CFileList> CreateInstance()
	{
		CComObject<CFileList>* pinstance;
		RaiseExceptionIfFailed(
			CComObject<CFileList>::CreateInstance(&pinstance));

		CComPtr<CFileList> filelist(pinstance);

		return filelist;
	}


	virtual HRESULT IDataObject_GetData(FORMATETC* pformatetc, STGMEDIUM* pmedium)
	{
		try
		{
			ValidateFormatEtc(pformatetc);

			pmedium->tymed = TYMED_HGLOBAL;
			pmedium->pUnkForRelease = NULL;
			pmedium->hGlobal = CreateData();

			return S_OK;
		}
		MSF_COM_CATCH_HANDLER()
	}


	void Add(const TCHAR* tszFilename)
	{
		m_filenames.push_back(tszFilename);
	}


public:
	LPDATAADVISEHOLDER m_spDataAdviseHolder;

private:
	void ValidateFormatEtc(const FORMATETC* pformatetc) const
	{
		RaiseExceptionIf(pformatetc->cfFormat != CF_HDROP, DV_E_FORMATETC);
		RaiseExceptionIf(pformatetc->dwAspect != DVASPECT_CONTENT, DV_E_DVASPECT);
		RaiseExceptionIf(pformatetc->lindex != -1, DV_E_LINDEX);
		RaiseExceptionIf(pformatetc->tymed != TYMED_HGLOBAL, DV_E_TYMED);
	}


	HGLOBAL CreateData() const
	{
		HGLOBAL hg = GlobalAllocThrow(GetDataSize());

		DROPFILES* pdropfiles = static_cast<DROPFILES*>(hg);

		pdropfiles->pFiles = sizeof(DROPFILES);
		#ifdef _UNICODE
		pdropfiles->fWide = TRUE;
		#else
		pdropfiles->fWide = FALSE;
		#endif

		// copy filenames into data block.
		TCHAR* pszBuf =
			reinterpret_cast<TCHAR*>(reinterpret_cast<BYTE*>(pdropfiles) + sizeof(DROPFILES));

		for (std::vector<CString>::const_iterator it = m_filenames.begin(); it != m_filenames.end(); ++it)
		{
			ATLVERIFY(lstrcpy(pszBuf, it->GetString()) != NULL);
			pszBuf += (it->GetLength() + 1);
		}

		*pszBuf = _T('\0');

		return hg;
	}


	size_t GetDataSize() const
	{
		UINT nchars = 0;

		for (std::vector<CString>::const_iterator it = m_filenames.begin(); it != m_filenames.end(); ++it)
		{
			nchars += it->GetLength() + 1;
		}

		return sizeof(DROPFILES) + (sizeof(TCHAR) * (nchars + 1));
	}

	std::vector<CString> m_filenames;
};

} // end MSF namespace
