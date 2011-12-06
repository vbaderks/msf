//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "../include/enumidlistimpl.h"
#include "vvvfile.h"


class ATL_NO_VTABLE CEnumIDList :
	public CComObjectRootEx<CComSingleThreadModel>,
	public IEnumIDListImpl<CEnumIDList>
{
public:
	DECLARE_NOT_AGGREGATABLE(CEnumIDList)

	BEGIN_COM_MAP(CEnumIDList)
		COM_INTERFACE_ENTRY(IEnumIDList)
	END_COM_MAP()

	static CComPtr<IEnumIDList> CreateInstance(const CString& strFilename, const CString& strFolder, DWORD grfFlags)
	{
		CComObject<CEnumIDList>* pinstance;
		RaiseExceptionIfFailed(
			CComObject<CEnumIDList>::CreateInstance(&pinstance));

		CComPtr<IEnumIDList> renumidlist(pinstance);

		pinstance->Initialize(strFilename, strFolder, grfFlags);

		return renumidlist;
	}


	LPITEMIDLIST GetNextItem()
	{
		return m_qfile->GetNextItem(m_grfFlags, m_nItemIterator);
	}

	// Note: VC++ 7.0 cannot access private members from static class functions.
	void Initialize(const CString& strFilename, const CString& strFolder, DWORD grfFlags)
	{
		m_grfFlags = grfFlags;
		m_qfile.reset(new CVVVFile(strFilename, strFolder));
		m_nItemIterator = 0;
	}

private:

	// Member variables
	DWORD              m_grfFlags;
	auto_ptr<CVVVFile> m_qfile;
	unsigned int       m_nItemIterator;
};
