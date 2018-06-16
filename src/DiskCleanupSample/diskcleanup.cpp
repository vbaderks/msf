//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//


#include "stdafx.h"

////#include "resource.h"
#include <diskcleanupimpl.h>

class __declspec(novtable) __declspec(uuid("C48ED8C8-1D34-4DAC-AB52-5D1DC53002B6")) DiskCleanup :
    public ATL::CComObjectRootEx<ATL::CComSingleThreadModel>,
    public ATL::CComCoClass<DiskCleanup, &__uuidof(DiskCleanup)>,
    public msf::DiskCleanupImpl<DiskCleanup>
{
public:
    BEGIN_COM_MAP(DiskCleanup)
        COM_INTERFACE_ENTRY(IEmptyVolumeCache2)
    END_COM_MAP()

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    static HRESULT __stdcall UpdateRegistry(BOOL /*bRegister*/) noexcept
    {
        return S_OK;
        ////return DiskCleanupImpl<CDiskCleanup>::UpdateRegistry(bRegister, IDR_INFOTIP, // tODO FIX
        ////            L"Sample Disk Clean Handler", wszVVVFileRootExt);
    }

    DiskCleanup(const DiskCleanup&) = delete;
    DiskCleanup(DiskCleanup&&) = delete;
    DiskCleanup& operator=(const DiskCleanup&) = delete;
    DiskCleanup& operator=(DiskCleanup&&) = delete;

    bool InitializeCore(HKEY /*hkRegKey*/, PCWSTR /*pcwszVolume*/, PCWSTR /*pcwszKeyName*/, PWSTR* /*ppwszDisplayName*/, PWSTR* /*ppwszDescription*/, PWSTR* /*ppwszBtnText*/, DWORD* /*pdwFlags*/)
    {
        return false;
    }

    bool GetSpaceUsedCore(DWORDLONG* /*pdwlSpaceUsed*/, IEmptyVolumeCacheCallBack* /*picb*/)
    {
        return false;
    }

    void PurgeCore(DWORDLONG /*dwSpaceToFree*/, IEmptyVolumeCacheCallBack* /*picb*/)
    {
    }

protected:
    DiskCleanup() = default;
    ~DiskCleanup() = default;
};

OBJECT_ENTRY_AUTO(__uuidof(DiskCleanup), DiskCleanup)
