//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"
#include "cfhdrop.h"
#include <algorithm>
#include <cctype>

namespace MSF
{

class IShellExtInitImpl :  public IShellExtInit
{
public:
    // IShellExtInit
    HRESULT __stdcall Initialize(_In_opt_ PCIDLIST_ABSOLUTE pidlFolder, _In_opt_ IDataObject* pDataObject, _In_opt_ HKEY /*hkeyProgID*/) noexcept override
    {
        UNREFERENCED_PARAMETER(pidlFolder);
        ATLTRACE2(ATL::atlTraceCOM, 0, L"IShellExtInitImpl::IShellExtInit::Initialize (instance=%p, pidlFolder=%p, pDataObject=%p)\n", this, pidlFolder, pDataObject);
        ATLASSERT(pDataObject && "Invalid argument");

        try
        {
            CacheFiles(pDataObject);
            return S_OK;
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

protected:

    ~IShellExtInitImpl() = default;

    void RegisterExtension(std::wstring extension)
    {
        std::transform(extension.begin(), extension.end(), extension.begin(), string_tolower_functional);
        m_extensions.push_back(extension);
    }

    void CacheFiles(IDataObject* pDataObject)
    {
        CfHDrop cfhdrop(pDataObject);

        m_filenames.clear();

        const auto uFiles = cfhdrop.GetFileCount();
        for (unsigned int i = 0; i < uFiles; ++i)
        {
            auto strFilename = cfhdrop.GetFile(i);
            m_filenames.push_back(std::wstring(strFilename));
        }
    }

    // Purpose: helper function, can be used to filter unsupported filename in a collection.
    bool ContainsUnknownExtension(const std::vector<std::wstring>& filenames) const
    {
        return std::find_if(filenames.begin(), filenames.end(),
            [=](const std::wstring& fileName) { return IsUnknownExtension(fileName); }) != filenames.end();
    }

    bool IsUnknownExtension(const std::wstring& fileName) const
    {
        std::wstring extension(PathFindExtension(fileName.c_str()));
        std::transform(extension.begin(), extension.end(), extension.begin(), string_tolower_functional);
        return std::find(m_extensions.begin(), m_extensions.end(), extension) == m_extensions.end();
    }

    const std::vector<std::wstring>& GetFilenames() const
    {
        return m_filenames;
    }

private:

    static wchar_t string_tolower_functional(wchar_t c)
    {
        return static_cast<wchar_t>(std::tolower(c));
    }

    // Member variables.
    std::vector<std::wstring> m_extensions;
    std::vector<std::wstring> m_filenames;
};

} // end MSF namespace
