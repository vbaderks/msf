//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "updateregistry.h"
#include "catchhandler.h"
#include "queryinfoimpl.h"
#include "shelluuids.h"

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
/// To support XP, Win98, ME:
///  - IPersistFile
/// </remarks>
template <typename T>
class ATL_NO_VTABLE IInfoTipImpl :
    public IInitializeWithFile,
    public IPersistFile,
    public IQueryInfoImpl<T>
{
public:
    /// <summary>Registration function to register the infotip COM object and a ProgId/extension.</summary>
    static HRESULT WINAPI UpdateRegistry(BOOL bRegister, UINT nResId,
        PCWSTR szDescription, PCWSTR szRootKey) throw()
    {
        return UpdateRegistryFromResource(nResId, bRegister,
            szDescription, T::GetObjectCLSID(), szRootKey);
    }

    IInfoTipImpl() : m_bInitialized(false)
    {
        ATLTRACE2(atlTraceCOM, 0, L"IInfoTipImpl::IInfoTipImpl (instance=%p)\n", this);
    }

    // IInitializeWithFile
    STDMETHOD(Initialize)(LPCWSTR pszFilePath, DWORD dwMode)
    {
        ATLTRACE2(atlTraceCOM, 0, L"IInfoTipImpl::Initialize (withfile) (instance=%p, mode=%d, filename=%s)\n", this, dwMode, pszFilePath);

        try
        {
            if (m_bInitialized)
                return HRESULT_FROM_WIN32(ERROR_ALREADY_INITIALIZED);

            // Note: InitializeImpl must be implemented by the derived class.
            static_cast<T*>(this)->InitializeImpl(CW2T(pszFilePath), dwMode);
            m_bInitialized = true;
            return S_OK;
        }
        MSF_COM_CATCH_HANDLER()
    }

    // IPersistFile (used by XP and older)
    // Note: A lot of functions are defined by the interface, but not for infotip objects.
    STDMETHOD(GetClassID)(_Out_ LPCLSID)
    {
        MSF_TRACENOTIMPL(L"IInfoTipImpl::GetClassID");
    }

    STDMETHOD(IsDirty)()
    {
        MSF_TRACENOTIMPL(L"IInfoTipImpl::IsDirty");
    }

    STDMETHOD(Save)(LPCOLESTR, BOOL)
    {
        MSF_TRACENOTIMPL(L"IInfoTipImpl::Save");
    }

    STDMETHOD(SaveCompleted)(LPCOLESTR)
    {
        MSF_TRACENOTIMPL(L"IInfoTipImpl::SaveCompleted");
    }

    STDMETHOD(GetCurFile)(LPOLESTR*)
    {
        MSF_TRACENOTIMPL(L"IInfoTipImpl::GetCurFile");
    }

    STDMETHOD(Load)(LPCOLESTR wszFilename, DWORD dwMode)
    {
        ATLTRACE2(atlTraceCOM, 0, L"IInfoTipImpl::Load (instance=%p, mode=%d, filename=%s)\n", this, dwMode, wszFilename);
        try
        {
            if (m_bInitialized)
                return HRESULT_FROM_WIN32(ERROR_ALREADY_INITIALIZED);

            // Note: InitializeImpl must be implemented by the derived class.
            static_cast<T*>(this)->InitializeImpl(CW2T(wszFilename), dwMode);
            m_bInitialized = true;
            return S_OK;
        }
        MSF_COM_CATCH_HANDLER()
    }

private:
    bool m_bInitialized;
};

} // namespace MSF
