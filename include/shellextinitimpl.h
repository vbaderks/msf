//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"
#include "cfhdrop.h"
#include "dataobjectptr.h"
#include "catchhandler.h"

namespace MSF
{

class IShellExtInitImpl :  public IShellExtInit
{
public:
    // IShellExtInit
    STDMETHOD(Initialize)(_In_opt_ PCIDLIST_ABSOLUTE pidlFolder, _In_opt_ IDataObject* pDataObject, _In_opt_ HKEY /*hkeyProgID*/) 
    {
        (pidlFolder);
        ATLTRACE2(atlTraceCOM, 0, L"IShellExtInitImpl::IShellExtInit::Initialize (instance=%p, pidlFolder=%p, pDataObject=%p)\n", this, pidlFolder, pDataObject);
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
        return std::find_if(filenames.begin(), filenames.end(),
            [=](const CString& fileName) { return IsUnknownExtension(fileName); }) != filenames.end();
    }

    bool IsUnknownExtension(const wchar_t* szFileName) const
    {
        CString strExtension(PathFindExtension(szFileName));
        strExtension.MakeLower();
        return std::find(m_extensions.begin(), m_extensions.end(), strExtension) == m_extensions.end();
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
