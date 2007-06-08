//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "cfhdrop.h"
#include "dataobjectptr.h"
#include "catchhandler.h"


namespace MSF
{

class IShellExtInitImpl :  public IShellExtInit
{
public:
	// IShellExtInit
	STDMETHOD(Initialize)(const ITEMIDLIST* /*pidlFolder*/, IDataObject* pDataObject, HKEY /*hkeyProgID*/) 
	{
		ATLTRACE2(atlTraceCOM, 0, _T("IContextMenuImpl::Initialize (instance=%p)\n"), this);
		ATLASSERT(pDataObject != NULL && "Invalid argument");

		try
		{
			CacheFiles(pDataObject);
			return S_OK;
		}
		MSF_COM_CATCH_HANDLER()
	}


protected:

	void RegisterExtension(CString strExtension)
	{
		m_extensions.push_back(strExtension.MakeLower());
	}


	void CacheFiles(IDataObject* pDataObject)
	{
		CCfHDrop cfhdrop(pDataObject);

		m_filenames.clear();

		const unsigned int uFiles = cfhdrop.GetFileCount();

		for (unsigned int i = 0; i < uFiles; ++i)
		{
			CString strFilename = cfhdrop.GetFile(i);

			m_filenames.push_back(strFilename);
		}
	}


	// Purpose: helper function, can be used to filter unsupported filename in a collection.
	bool ContainsUnknownExtension(const std::vector<CString>& filenames) const
	{
		for (std::vector<CString>::const_iterator it = filenames.begin(); it != filenames.end(); ++it)
		{
			if (IsUnknownExtension(*it))
				return true; // found at least 1 unknown extension.
		}

		return false;
	}


	bool IsUnknownExtension(const TCHAR* szFileName) const
	{
		CString strExtension(PathFindExtension(szFileName));
		strExtension.MakeLower();

		std::vector<CString>::const_iterator result =
			std::find(m_extensions.begin(), m_extensions.end(), strExtension);

		return result == m_extensions.end();
	}


	const std::vector<CString>& GetFilenames() const
	{
		return m_filenames;
	}

private:

	// Member variables.
	std::vector<CString> m_extensions;
	std::vector<CString> m_filenames;
};

} // end MSF namespace
