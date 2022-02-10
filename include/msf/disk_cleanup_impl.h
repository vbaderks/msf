//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "msf_base.h"
#include "update_registry.h"

#include <EmptyVC.h>


namespace msf {

template <typename T>
class __declspec(novtable) DiskCleanupImpl : public IEmptyVolumeCache2
{
public:
    /// <summary>Registration function to register the COM object and a ProgId/extension.</summary>
    static HRESULT __stdcall UpdateRegistry(BOOL registerObject, uint32_t resourceId,
        PCWSTR description, PCWSTR rootKey) noexcept
    {
        return UpdateRegistryFromResource(resourceId, registerObject, description, T::GetObjectCLSID(), rootKey);
    }

    // IEmptyVolumeCache
    HRESULT __stdcall Initialize(HKEY hkRegKey, PCWSTR pcwszVolume, PWSTR* ppwszDisplayName, PWSTR* ppwszDescription, DWORD* pdwFlags) noexcept override
    {
        PWSTR pwszBtnText;
        const auto result = InitializeEx(hkRegKey, pcwszVolume, nullptr, ppwszDisplayName, ppwszDescription, &pwszBtnText, pdwFlags);
        // Release BtnText;
        return result;

        // TODO: research if this function is still used or if we can just return E_FAIL / E_NOTIMPLEMENTED.
    }

    HRESULT __stdcall GetSpaceUsed(__RPC__out DWORDLONG* pdwlSpaceUsed, __RPC__in_opt IEmptyVolumeCacheCallBack* picb) noexcept override
    {
        try
        {
            if (!m_bInitialized)
                return E_FAIL;

            // Note: function must be implemented by the derived class.
            return static_cast<T*>(this)->GetSpaceUsedCore(pdwlSpaceUsed, picb) ? S_OK : S_FALSE;
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    HRESULT __stdcall Purge(DWORDLONG dwSpaceToFree, __RPC__in_opt IEmptyVolumeCacheCallBack* picb) noexcept override
    {
        try
        {
            if (!m_bInitialized)
                return E_FAIL;

            // Note: function must be implemented by the derived class.
            static_cast<T*>(this)->PurgeCore(dwSpaceToFree, picb);
            return S_OK;
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    HRESULT __stdcall ShowProperties(__RPC__in HWND hwnd) noexcept override
    {
        try
        {
            if (!m_bInitialized)
                return E_FAIL;

            // Note: function must be implemented by the derived class.
            static_cast<T*>(this)->ShowPropertiesCore(hwnd);
            return S_OK;
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    HRESULT __stdcall Deactivate(__RPC__out DWORD* pdwFlags) noexcept override
    {
        try
        {
            *pdwFlags = 0;

            if (!m_bInitialized)
                return E_FAIL;

            // Note: this function can be implemented by the derived class.
            if (static_cast<T*>(this)->DeactivateCore())
            {
                *pdwFlags = EVCF_REMOVEFROMLIST;
            }
            return S_OK;
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    // IEmptyVolumeCache2
    HRESULT __stdcall InitializeEx(HKEY hkRegKey, PCWSTR pcwszVolume, PCWSTR pcwszKeyName, PWSTR* ppwszDisplayName, PWSTR* ppwszDescription, PWSTR* ppwszBtnText, DWORD* pdwFlags) noexcept override
    {
        try
        {
            if (m_bInitialized)
                return HRESULT_FROM_WIN32(ERROR_ALREADY_INITIALIZED);

            // Note: function must be implemented by the derived class.
            return static_cast<T*>(this)->InitializeCore(hkRegKey, pcwszVolume, pcwszKeyName, ppwszDisplayName, ppwszDescription, ppwszBtnText, pdwFlags)
                ? S_OK : S_FALSE;
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    void ShowPropertiesCore(HWND /*hwnd*/) noexcept
    {
    }

    // Override this function and return true when the disk cleanup handler should only run once.
    bool DeactivateCore() noexcept
    {
        return false;
    }

    DiskCleanupImpl(const DiskCleanupImpl&) = delete;
    DiskCleanupImpl(DiskCleanupImpl&&) = delete;
    DiskCleanupImpl& operator=(const DiskCleanupImpl&) = delete;
    DiskCleanupImpl& operator=(DiskCleanupImpl&&) = delete;

protected:
    DiskCleanupImpl() noexcept(false)
    {
        ATLTRACE(L"DiskCleanupImpl::DiskCleanupImpl (instance=%p)\n", this);
    }

    ~DiskCleanupImpl()
    {
        ATLTRACE(L"DiskCleanupImpl::~DiskCleanupImpl (instance=%p)\n", this);
    }

private:
    bool m_bInitialized{};
};

} // namespace msf
