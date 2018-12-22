//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "msf_base.h"
#include "update_registry.h"

#include <strsafe.h>

namespace msf
{

template <typename T>
class __declspec(novtable) ExtractImageImpl :
    public IPersistFile,
    public IExtractImage2
{
public:
    // Registration function to register the extension.
    static HRESULT __stdcall UpdateRegistry(uint32_t nResId, BOOL bRegister,
        PCWSTR description, const CLSID& clsidShellFolder, PCWSTR szExtension) noexcept
    {
        return UpdateRegistryFromResource(nResId, bRegister,
            description, T::GetObjectCLSID(), clsidShellFolder, szExtension);
    }

    // Registration function to register the COM object + the root extension.
    static HRESULT __stdcall UpdateRegistryForRootExt(uint32_t nResId, BOOL bRegister,
        PCWSTR szDescription, PCWSTR szRootExt) noexcept
    {
        return UpdateRegistryForRootExt(nResId, bRegister,
            szDescription, T::GetObjectCLSID(), szRootExt);
    }

    // All-in-one registration function for 1 extension, call 'ForExt' to register
    // additional functions.
    static HRESULT __stdcall UpdateRegistry(uint32_t nResIdRoot, uint32_t nResIdExt, BOOL bRegister,
        PCWSTR szDescription, PCWSTR szRootExt, PCWSTR szExtension) noexcept
    {
        return T::UpdateRegistry(nResIdRoot, nResIdExt, bRegister,
            szDescription, T::GetObjectCLSID(), szRootExt, szExtension);
    }

    // IPersistFile
    HRESULT __stdcall GetClassID(__RPC__out CLSID* classId) noexcept override
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"ExtractImageImpl::GetClassID\n");

        if (!classId)
            return E_POINTER;

        *classId = T::GetObjectCLSID();
        return S_OK;
    }

    HRESULT __stdcall IsDirty() noexcept override
    {
        ATLTRACENOTIMPL(L"ExtractImageImpl::IsDirty");
    }

    HRESULT __stdcall Save(LPCOLESTR, BOOL) noexcept override
    {
        ATLTRACENOTIMPL(L"ExtractImageImpl::Save");
    }

    HRESULT __stdcall SaveCompleted(LPCOLESTR) noexcept override
    {
        ATLTRACENOTIMPL(L"ExtractImageImpl::SaveCompleted");
    }

    HRESULT __stdcall GetCurFile(LPOLESTR*) noexcept override
    {
        ATLTRACENOTIMPL(L"ExtractImageImpl::GetCurFile");
    }

    HRESULT __stdcall Load(LPCOLESTR filename, [[maybe_unused]] DWORD mode) noexcept override
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"ExtractImageImpl::Load (instance=%p, mode=%d, filename=%s)\n", this, mode, filename);
        try
        {
            m_filename = filename;
            return S_OK;
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    // IExtractImage
    HRESULT __stdcall GetLocation(PWSTR pszPathBuffer, DWORD cch, __RPC__inout_opt DWORD* pdwPriority,
                                  __RPC__in const SIZE* psize, DWORD dwRecClrDepth, __RPC__inout DWORD* pdwFlags) noexcept override
    {
        try
        {
            ATLTRACE2(ATL::atlTraceCOM, 0, "ExtractImageImpl::GetLocation (instance=%p, flags=%d)\n", this, *pdwFlags);

            Dispose();
            m_bitmap = static_cast<T*>(this)->CreateImage(*psize, dwRecClrDepth, *pdwFlags);

            RaiseExceptionIfFailed(StringCchCopy(pszPathBuffer, cch, static_cast<T*>(this)->GetPathBuffer().c_str()));
            *pdwFlags |= IEIFLAG_CACHE;

            //  Note: The SDK docs are unclear if it passing a NULL pointer is allowed.
            if (pdwPriority)
            {
                *pdwPriority = IEIT_PRIORITY_NORMAL;
            }

            return S_OK;
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    HRESULT __stdcall Extract(__RPC__deref_out_opt HBITMAP* thumbnail) noexcept override
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, "ExtractImageImpl::Extract (instance=%p)\n", this);

        if (!m_bitmap)
            return E_FAIL;

        *thumbnail = m_bitmap;
        m_bitmap = nullptr;

        return S_OK;
    }

    // IExtractImage2
    HRESULT __stdcall GetDateStamp(__RPC__in FILETIME* dateStamp) noexcept override
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"ExtractImageImpl::GetDateStamp (instance=%p, pdatastamp=%p)\n", this, dateStamp);

        if (!dateStamp)
            return E_POINTER;

        WIN32_FILE_ATTRIBUTE_DATA fileattributedata;
        if (!GetFileAttributesEx(static_cast<T*>(this)->GetPathBuffer().c_str(), GetFileExInfoStandard, &fileattributedata))
            return E_FAIL;

        *dateStamp = fileattributedata.ftLastWriteTime;
        return S_OK;
    }

    // Purpose: returns the identifier for the file that is used to
    //          extract the image from. Can be used to optimize
    //          cases were the image is extracted from a different file
    //          then set by IPersistFile::Load.
    const std::wstring& GetPathBuffer() const noexcept
    {
        return m_filename;
    }

    ExtractImageImpl(const ExtractImageImpl&) = delete;
    ExtractImageImpl(ExtractImageImpl&&) = delete;
    ExtractImageImpl& operator=(const ExtractImageImpl&) = delete;
    ExtractImageImpl& operator=(ExtractImageImpl&&) = delete;

protected:
    ExtractImageImpl() noexcept
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"ExtractImageImpl::ExtractImageImpl (instance=%p)\n", this);
    }

    ~ExtractImageImpl()
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"ExtractImageImpl::~ExtractImageImpl (instance=%p)\n", this);
        Dispose();
    }

    void Dispose() noexcept
    {
        if (m_bitmap)
        {
            ATLVERIFY(DeleteObject(m_bitmap));
            m_bitmap = nullptr;
        }
    }

    // member variables.
    std::wstring m_filename;
    HBITMAP      m_bitmap{};
};

} // namespace msf
