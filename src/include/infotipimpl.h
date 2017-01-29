//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"
#include "updateregistry.h"
#include "queryinfoimpl.h"

namespace MSF
{

/// <summary>Basic implementation functionality for a InfoTip shell extension COM object.</summary>
/// <remarks>
/// Classes can use this mix-in implementation class to add support to act as an InfoTip COM object.
/// Standard ATL template classes can be used for the generic COM implementation (addref, release, etc).
/// The actual InfoTip class need to setup the COM interface map.
/// The following interfaces should be enabled:
///  - IQueryInfo 
/// One of the following for initialization (Vista and up):
///  - IInitializeWithFile
///  - IInitializeWithStream
/// </remarks>
template <typename T>
class ATL_NO_VTABLE InfoTipImpl :
    public IInitializeWithFile,
    public IQueryInfoImpl<T>
{
public:
    /// <summary>Registration function to register the infotip COM object and a ProgId/extension.</summary>
    static HRESULT WINAPI UpdateRegistry(BOOL bRegister, UINT nResId,
        PCWSTR szDescription, PCWSTR szRootKey) noexcept
    {
        return UpdateRegistryFromResource(nResId, bRegister,
            szDescription, T::GetObjectCLSID(), szRootKey);
    }


    // IInitializeWithFile
    STDMETHOD(Initialize)(LPCWSTR pszFilePath, DWORD dwMode) override
    {
        ATLTRACE2(atlTraceCOM, 0, L"InfoTipImpl::Initialize (withfile) (instance=%p, mode=%d, filename=%s)\n", this, dwMode, pszFilePath);

        try
        {
            if (_bInitialized)
                return HRESULT_FROM_WIN32(ERROR_ALREADY_INITIALIZED);

            // Note: InitializeImpl must be implemented by the derived class.
            static_cast<T*>(this)->InitializeImpl(CW2T(pszFilePath), dwMode);
            _bInitialized = true;
            return S_OK;
        }
        MSF_COM_CATCH_HANDLER()
    }

protected:
    InfoTipImpl() : _bInitialized(false)
    {
        ATLTRACE2(atlTraceCOM, 0, L"InfoTipImpl::InfoTipImpl (instance=%p)\n", this);
    }


    ~InfoTipImpl()
    {
        ATLTRACE2(atlTraceCOM, 0, L"InfoTipImpl::~InfoTipImpl (instance=%p)\n", this);
    }

private:
    bool _bInitialized;
};

} // namespace MSF
