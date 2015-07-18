//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include <emptyvc.h>

namespace MSF
{

template <typename T>
class ATL_NO_VTABLE DiskCleanupImpl : IEmptyVolumeCache2
{
public:
    DiskCleanupImpl() : _bInitialized(false)
    {
        ATLTRACE2(atlTraceCOM, 0, L"DiskCleanupImpl::DiskCleanupImpl (instance=%p)\n", this);
    }

    // IEmptyVolumeCache2
    STDMETHOD(InitializeEx)(HKEY hkRegKey, LPCWSTR pcwszVolume, LPCWSTR pcwszKeyName, LPWSTR *ppwszDisplayName, LPWSTR *ppwszDescription, LPWSTR *ppwszBtnText, DWORD *pdwFlags)
    {
        try
        {
            if (m_bInitialized)
                return HRESULT_FROM_WIN32(E_FAIL);
        }
        MSF_COM_CATCH_HANDLER()
    }

private:
    bool _bInitialized;
};
} // namespace MSF
