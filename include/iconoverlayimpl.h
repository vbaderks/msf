//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"
#include "catchhandler.h"

namespace MSF
{

template <typename T>
class ATL_NO_VTABLE IconOverlayImpl : IShellIconOverlayIdentifier
{
public:
    IconOverlayImpl() : _bInitialized(false)
    {
        ATLTRACE2(atlTraceCOM, 0, L"IconOverlayImpl::IconOverlayImpl (instance=%p)\n", this);
    }

    // IShellIconOverlayIdentifier
    STDMETHOD(IsMemberOf)(PCWSTR pwszPath, DWORD dwAttrib)
    {
        ATLTRACE2(atlTraceCOM, 0, L"IconOverlayImpl::IsMemberOf (instance=%p, filename=%s, mode=%d)\n", this, pwszPath, dwAttrib);

        try
        {
            // Note: IsMemberOfImpl must be implemented by the derived class.
            return static_cast<T*>(this)->IsMemberOfImpl(pwszPath, dwAttrib) ? S_OK : S_FALSE;
        }
        MSF_COM_CATCH_HANDLER()
    }

    STDMETHOD(GetOverlayInfo)(PWSTR pwszIconFile, int cchMax, int* pIndex, DWORD* pdwFlags)
    {
        ATLTRACE2(atlTraceCOM, 0, L"IconOverlayImpl::GetOverlayInfo (instance=%p)\n", this);
    }

    STDMETHOD(GetPriority)(int* pIPriority)
    {
        ATLTRACE2(atlTraceCOM, 0, L"IconOverlayImpl::GetPriority (instance=%p)\n", this);

        // In most cases the value zero (highest priority) is fine.
        // In rare cases multiple Icon overlay handler could be registered for one file type.
        *pIPriority = 0;
        return S_OK;
    }

    // IEmptyVolumeCache2
    STDMETHOD(InitializeEx)(HKEY hkRegKey, LPCWSTR pcwszVolume, LPCWSTR pcwszKeyName, LPWSTR *ppwszDisplayName, LPWSTR *ppwszDescription, LPWSTR *ppwszBtnText, DWORD *pdwFlags)
    {
        try
        {
            if (_bInitialized)
                return HRESULT_FROM_WIN32(E_FAIL);
        }
        MSF_COM_CATCH_HANDLER()
    }

private:
    bool _bInitialized;
};

} // namespace MSF
