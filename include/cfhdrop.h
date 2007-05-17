//
// (C) Copyright by Victor Derks <vba64@xs4all.nl>
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "dataobjectptr.h"
#include "stgmedium.h"
#include "formatetc.h"


namespace MSF
{

class CCfHDrop
{
public:
	static bool IsFormat(IDataObject* pdataobject)
	{
		CFormatEtc formatetc(CF_HDROP);

		return SUCCEEDED(pdataobject->QueryGetData(&formatetc));
	}


	CCfHDrop(IDataObjectPtr dataobject)
	{
		dataobject.GetData(CFormatEtc(CF_HDROP), m_stgmedium);
	}


	bool empty() const throw()
	{
		return GetFileCount() == 0;
	}


	unsigned int GetFileCount() const throw()
	{
		ATLASSERT(m_stgmedium.tymed == TYMED_HGLOBAL && "Unable to retrieve filecount");

		return ::DragQueryFile((HDROP) m_stgmedium.hGlobal, (UINT)-1, NULL, 0);
	}


	CString GetFile(unsigned int iFile) const
	{
		ATLASSERT(iFile < GetFileCount() && "Index out of bounds");

		TCHAR szFileName[MAX_PATH];
		RaiseExceptionIf(
			::DragQueryFile(reinterpret_cast<HDROP>(m_stgmedium.hGlobal), iFile, szFileName,
		                    MSF_ARRAY_SIZE(szFileName)) == 0);

		return szFileName;
	}

private:

	CStgMedium m_stgmedium;
};

} // namespace MSF
