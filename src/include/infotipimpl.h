//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"
#include "updateregistry.h"
#include "queryinfoimpl.h"

namespace msf
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
class __declspec(novtable) InfoTipImpl :
    public IInitializeWithFile,
    public IQueryInfoImpl
{
public:
    /// <summary>Registration function to register the info tip COM object and a ProgId/extension.</summary>
    static HRESULT WINAPI UpdateRegistry(BOOL bRegister, UINT nResId,
        PCWSTR szDescription, PCWSTR szRootKey) noexcept
    {
        return UpdateRegistryFromResource(nResId, bRegister,
            szDescription, T::GetObjectCLSID(), szRootKey);
    }

    // IInitializeWithFile
    HRESULT __stdcall Initialize(PCWSTR pszFilePath, DWORD dwMode) override
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"InfoTipImpl::Initialize (withfile) (instance=%p, mode=%d, filename=%s)\n", this, dwMode, pszFilePath);

        try
        {
            if (m_initialized)
                return HRESULT_FROM_WIN32(ERROR_ALREADY_INITIALIZED);

            // Note: InitializeImpl must be implemented by the derived class.
            InitializeCore(pszFilePath, dwMode);
            m_initialized = true;
            return S_OK;
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

protected:
    InfoTipImpl()
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"InfoTipImpl::InfoTipImpl (instance=%p)\n", this);
    }

    ~InfoTipImpl()
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"InfoTipImpl::~InfoTipImpl (instance=%p)\n", this);
    }

    virtual void InitializeCore(PCWSTR pszFilePath, DWORD dwMode) = 0;

private:
    bool m_initialized{};
};

} // namespace msf
