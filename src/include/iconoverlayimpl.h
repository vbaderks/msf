//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"

namespace msf
{

template <typename T>
class __declspec(novtable) IconOverlayImpl : public IShellIconOverlayIdentifier
{
public:
    IconOverlayImpl(const IconOverlayImpl&) = delete;
    IconOverlayImpl(IconOverlayImpl&&) = delete;
    IconOverlayImpl& operator=(const IconOverlayImpl&) = delete;
    IconOverlayImpl& operator=(IconOverlayImpl&&) = delete;

    /// <summary>Registration function to register the COM object.</summary>
    static HRESULT __stdcall UpdateRegistry(BOOL bRegister, UINT nResId, PCWSTR description) noexcept
    {
        return UpdateRegistryFromResource(nResId, bRegister, description, T::GetObjectCLSID());
    }

    // IShellIconOverlayIdentifier
    HRESULT __stdcall IsMemberOf(PCWSTR pwszPath, DWORD dwAttrib) noexcept override
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"IconOverlayImpl::IsMemberOf (instance=%p, filename=%s, mode=%d)\n", this, pwszPath, dwAttrib);

        if (!pwszPath)
            return E_POINTER;

        try
        {
            // Note: IsMemberOfImpl must be implemented by the derived class.
            return static_cast<T*>(this)->IsMemberOfImpl(pwszPath, dwAttrib) ? S_OK : S_FALSE;
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    HRESULT __stdcall GetOverlayInfo(PWSTR pwszIconFile, int cchMax, int* pIndex, DWORD* pdwFlags) noexcept override
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"IconOverlayImpl::GetOverlayInfo (instance=%p, pwszIconFile=%s, cchMax=%d, pIndex=%p, pdwFlags=%p)\n", this, pwszIconFile, cchMax, pIndex, pdwFlags);

        if (!pwszIconFile || !pIndex || !pdwFlags)
            return E_POINTER;

        if (!GetModuleFileNameW(ATL::_AtlBaseModule.GetModuleInstance(), pwszIconFile, cchMax))
            return HRESULT_FROM_WIN32(GetLastError());

        *pIndex = m_iconIndex;
        *pdwFlags = ISIOI_ICONFILE | ISIOI_ICONINDEX;
        return S_OK;
    }

    HRESULT __stdcall GetPriority(int* pIPriority) noexcept override
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"IconOverlayImpl::GetPriority (instance=%p)\n", this);

        if (!pIPriority)
            return E_POINTER;

        // In most cases the value zero (highest priority) is fine.
        // In rare cases multiple Icon overlay handler could be registered for one file type.
        *pIPriority = m_priority;
        return S_OK;
    }

protected:
    explicit IconOverlayImpl(int iconIndex = 0, int priority = 0) noexcept :
        m_iconIndex(iconIndex),
        m_priority(priority)
    {
        ATLASSERT(iconIndex >= 0);
        ATLASSERT(priority >= 0 && priority <= 100);
        ATLTRACE2(ATL::atlTraceCOM, 0, L"IconOverlayImpl::IconOverlayImpl (instance=%p)\n", this);
    }

    ~IconOverlayImpl()
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"IconOverlayImpl::~IconOverlayImpl (instance=%p)\n", this);
    }

private:
    int m_iconIndex;
    int m_priority;
};

} // namespace msf
