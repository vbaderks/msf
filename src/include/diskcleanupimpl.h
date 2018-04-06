//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "msfbase.h"
#include "updateregistry.h"
#include <emptyvc.h>


namespace msf {

template <typename T>
class __declspec(novtable) DiskCleanupImpl : public IEmptyVolumeCache2
{
public:
    /// <summary>Registration function to register the COM object and a ProgId/extension.</summary>
    static HRESULT WINAPI UpdateRegistry(BOOL bRegister, UINT nResId,
        PCWSTR szDescription, PCWSTR szRootKey) noexcept
    {
        return UpdateRegistryFromResource(nResId, bRegister, szDescription, T::GetObjectCLSID(), szRootKey);
    }

    DiskCleanupImpl() : _bInitialized(false)
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"DiskCleanupImpl::DiskCleanupImpl (instance=%p)\n", this);
    }

    // IEmptyVolumeCache
    HRESULT __stdcall Initialize(HKEY hkRegKey, LPCWSTR pcwszVolume, LPWSTR* ppwszDisplayName, LPWSTR* ppwszDescription, DWORD* pdwFlags) noexcept override
    {
        LPWSTR pwszBtnText;
        const auto result = InitializeEx(hkRegKey, pcwszVolume, nullptr, ppwszDisplayName, ppwszDescription, &pwszBtnText, pdwFlags);
        // Release BtnText;
        return result;

        // TODO: research if this function is still used or if we can just return E_FAIL / E_NOTIMPLEMENTED.
    }

    HRESULT __stdcall GetSpaceUsed(__RPC__out DWORDLONG* pdwlSpaceUsed, __RPC__in_opt IEmptyVolumeCacheCallBack* picb) noexcept override
    {
        try
        {
            if (!_bInitialized)
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
            if (!_bInitialized)
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
            if (!_bInitialized)
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

            if (!_bInitialized)
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
    HRESULT __stdcall InitializeEx(HKEY hkRegKey, LPCWSTR pcwszVolume, LPCWSTR pcwszKeyName, LPWSTR* ppwszDisplayName, LPWSTR* ppwszDescription, LPWSTR* ppwszBtnText, DWORD* pdwFlags) noexcept override
    {
        try
        {
            if (_bInitialized)
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

    void ShowPropertiesCore(HWND /*hwnd*/)
    {
    }

    // Override this function and return true when the disk cleanup handler should only run once.
    bool DeactivateCore()
    {
        return false;
    }

private:
    bool _bInitialized;
};

} // namespace msf
