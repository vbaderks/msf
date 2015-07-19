//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "msfbase.h"
#include "catchhandler.h"
#include "updateregistry.h"
#include <emptyvc.h>


namespace MSF
{

template <typename T>
class ATL_NO_VTABLE DiskCleanupImpl : public IEmptyVolumeCache2
{
public:
    /// <summary>Registration function to register the COM object and a ProgId/extension.</summary>
    static HRESULT WINAPI UpdateRegistry(BOOL bRegister, UINT nResId,
        PCWSTR szDescription, PCWSTR szRootKey) throw()
    {
        return UpdateRegistryFromResource(nResId, bRegister, szDescription, T::GetObjectCLSID(), szRootKey);
    }

    DiskCleanupImpl() : _bInitialized(false)
    {
        ATLTRACE2(atlTraceCOM, 0, L"DiskCleanupImpl::DiskCleanupImpl (instance=%p)\n", this);
    }

    // IEmptyVolumeCache
    STDMETHOD(Initialize)(HKEY hkRegKey, LPCWSTR pcwszVolume, LPWSTR *ppwszDisplayName, LPWSTR *ppwszDescription, DWORD *pdwFlags)
    {
        LPWSTR pwszBtnText;
        auto result = InitializeEx(hkRegKey, pcwszVolume, nullptr, ppwszDisplayName, ppwszDescription, &pwszBtnText, pdwFlags);
        // Release BtnText;
        return result;

        // TODO: research if this function is still used or if we can just return E_FAIL / E_NOTIMPLEMENTED.
    }

    STDMETHOD(GetSpaceUsed)(__RPC__out DWORDLONG *pdwlSpaceUsed, __RPC__in_opt IEmptyVolumeCacheCallBack *picb)
    {
        try
        {
            if (!_bInitialized)
                return E_FAIL;

            // Note: function must be implemented by the derived class.
            return static_cast<T*>(this)->GetSpaceUsedCore(pdwlSpaceUsed, picb)
                ? S_OK : S_FALSE;
        }
        MSF_COM_CATCH_HANDLER()
    }

    STDMETHOD(Purge)(DWORDLONG dwSpaceToFree, __RPC__in_opt IEmptyVolumeCacheCallBack *picb)
    {
        try
        {
            if (!_bInitialized)
                return E_FAIL;

            // Note: function must be implemented by the derived class.
            static_cast<T*>(this)->PurgeCore(dwSpaceToFree, picb);
            return S_OK;
        }
        MSF_COM_CATCH_HANDLER()
    }

    STDMETHOD(ShowProperties)(__RPC__in HWND hwnd)
    {
        try
        {
            if (!_bInitialized)
                return E_FAIL;

            // Note: function must be implemented by the derived class.
            static_cast<T*>(this)->ShowPropertiesCore(hwnd);
            return S_OK;
        }
        MSF_COM_CATCH_HANDLER()
    }

    STDMETHOD(Deactivate)(__RPC__out DWORD *pdwFlags)
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
        MSF_COM_CATCH_HANDLER()
    }

    // IEmptyVolumeCache2
    STDMETHOD(InitializeEx)(HKEY hkRegKey, LPCWSTR pcwszVolume, LPCWSTR pcwszKeyName, LPWSTR *ppwszDisplayName, LPWSTR *ppwszDescription, LPWSTR *ppwszBtnText, DWORD *pdwFlags)
    {
        try
        {
            if (_bInitialized)
                return HRESULT_FROM_WIN32(ERROR_ALREADY_INITIALIZED);

            // Note: function must be implemented by the derived class.
            return static_cast<T*>(this)->InitializeCore(hkRegKey, pcwszVolume, pcwszKeyName, ppwszDisplayName, ppwszDescription, ppwszBtnText, pdwFlags)
                ? S_OK : S_FALSE;
        }
        MSF_COM_CATCH_HANDLER()
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
} // namespace MSF
