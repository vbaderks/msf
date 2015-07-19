//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//


#include "stdafx.h"
#include "resource.h"
#include "../include/diskcleanupimpl.h"


// Attach a GUID to the COM class.
// Note: Don't copy this GUID. Always use the 'Create GUID' tool to create new GUIDs.
class DECLSPEC_UUID("C48ED8C8-1D34-4DAC-AB52-5D1DC53002B6") CDiskCleanup;

class ATL_NO_VTABLE CDiskCleanup :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CDiskCleanup, &__uuidof(CDiskCleanup)>,
    public DiskCleanupImpl<CDiskCleanup>
{
public:
    BEGIN_COM_MAP(CDiskCleanup)
        COM_INTERFACE_ENTRY(IEmptyVolumeCache2)
    END_COM_MAP()

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    static HRESULT WINAPI UpdateRegistry(BOOL bRegister) throw()
    {
        return DiskCleanupImpl<CDiskCleanup>::UpdateRegistry(bRegister, IDR_INFOTIP, // tODO FIX
            L"Sample Disk Clean Handler", wszVVVFileRootExt);
    }

    bool InitializeCore(HKEY /*hkRegKey*/, LPCWSTR /*pcwszVolume*/, LPCWSTR /*pcwszKeyName*/, LPWSTR* /*ppwszDisplayName*/, LPWSTR* /*ppwszDescription*/, LPWSTR* /*ppwszBtnText*/, DWORD* /*pdwFlags*/)
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
};

OBJECT_ENTRY_AUTO(__uuidof(CDiskCleanup), CDiskCleanup)
