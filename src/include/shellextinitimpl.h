//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"
#include "cfhdrop.h"

namespace MSF
{

class IShellExtInitImpl :  public IShellExtInit
{
public:
    // IShellExtInit
    STDMETHOD(Initialize)(_In_opt_ PCIDLIST_ABSOLUTE pidlFolder, _In_opt_ IDataObject* pDataObject, _In_opt_ HKEY /*hkeyProgID*/) override
    {
        (pidlFolder);
        ATLTRACE2(atlTraceCOM, 0, L"IShellExtInitImpl::IShellExtInit::Initialize (instance=%p, pidlFolder=%p, pDataObject=%p)\n", this, pidlFolder, pDataObject);
        ATLASSERT(pDataObject && "Invalid argument");

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
        _extensions.push_back(strExtension.MakeLower());
    }

    void CacheFiles(IDataObject* pDataObject)
    {
        CCfHDrop cfhdrop(pDataObject);

        _filenames.clear();

        const auto uFiles = cfhdrop.GetFileCount();
        for (unsigned int i = 0; i < uFiles; ++i)
        {
            auto strFilename = cfhdrop.GetFile(i);
            _filenames.push_back(strFilename);
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
        return std::find(_extensions.begin(), _extensions.end(), strExtension) == _extensions.end();
    }

    const std::vector<CString>& GetFilenames() const
    {
        return _filenames;
    }

private:
    // Member variables.
    std::vector<CString> _extensions;
    std::vector<CString> _filenames;
};

} // end MSF namespace
