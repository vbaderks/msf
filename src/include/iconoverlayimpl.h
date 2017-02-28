//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"

namespace MSF
{

template <typename T>
class ATL_NO_VTABLE IconOverlayImpl : public IShellIconOverlayIdentifier
{
public:
    /// <summary>Registration function to register the COM object.</summary>
    static HRESULT WINAPI UpdateRegistry(BOOL bRegister, UINT nResId, PCWSTR szDescription) noexcept
    {
        return UpdateRegistryFromResource(nResId, bRegister, szDescription, T::GetObjectCLSID());
    }

    // IShellIconOverlayIdentifier
    STDMETHOD(IsMemberOf)(PCWSTR pwszPath, DWORD dwAttrib) override
    {
        ATLTRACE2(atlTraceCOM, 0, L"IconOverlayImpl::IsMemberOf (instance=%p, filename=%s, mode=%d)\n", this, pwszPath, dwAttrib);

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

    STDMETHOD(GetOverlayInfo)(PWSTR pwszIconFile, int cchMax, int* pIndex, DWORD* pdwFlags) override
    {
        ATLTRACE2(atlTraceCOM, 0, L"IconOverlayImpl::GetOverlayInfo (instance=%p, pwszIconFile=%s, cchMax=%d, pIndex=%p, pdwFlags=%p)\n", this, pwszIconFile, cchMax, pIndex, pdwFlags);

        if (!pwszIconFile || !pIndex || !pdwFlags)
            return E_POINTER;

        if (!GetModuleFileNameW(_AtlBaseModule.GetModuleInstance(), pwszIconFile, cchMax))
            return HRESULT_FROM_WIN32(GetLastError());

        *pIndex = _iconIndex;
        *pdwFlags = ISIOI_ICONFILE | ISIOI_ICONINDEX;
        return S_OK;
    }

    STDMETHOD(GetPriority)(int* pIPriority) override
    {
        ATLTRACE2(atlTraceCOM, 0, L"IconOverlayImpl::GetPriority (instance=%p)\n", this);

        if (!pIPriority)
            return E_POINTER;

        // In most cases the value zero (highest priority) is fine.
        // In rare cases multiple Icon overlay handler could be registered for one file type.
        *pIPriority = _priority;
        return S_OK;
    }

protected:

    explicit IconOverlayImpl(int iconIndex = 0, int priority = 0) :
        _iconIndex(iconIndex),
        _priority(priority)
    {
        ATLASSERT(iconIndex >= 0);
        ATLASSERT(priority >= 0 && priority <= 100);
        ATLTRACE2(atlTraceCOM, 0, L"IconOverlayImpl::IconOverlayImpl (instance=%p)\n", this);
    }

    IconOverlayImpl(const IconOverlayImpl&) = delete;

    ~IconOverlayImpl()
    {
        ATLTRACE2(atlTraceCOM, 0, L"IconOverlayImpl::~IconOverlayImpl (instance=%p)\n", this);
    }

private:
    int _iconIndex;
    int _priority;
};

} // namespace MSF
