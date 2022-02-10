//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msf_base.h"
#include "update_registry.h"
#include "query_info_impl.h"

namespace msf
{

/// <summary>Basic implementation functionality for a InfoTip shell extension COM object.</summary>
/// <remarks>
/// Classes can use this mix-in implementation class to add support to act as an InfoTip COM object.
/// Standard ATL template classes can be used for the generic COM implementation (addref, release, etc).
/// The actual InfoTip class need to setup the COM interface map.
/// The following interfaces should be enabled:
///  - IQueryInfo
/// One of the following for initialization:
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
    [[nodiscard]] static HRESULT __stdcall UpdateRegistry(BOOL registerObject, uint32_t resourceId,
        PCWSTR description, PCWSTR rootKey) noexcept
    {
        return UpdateRegistryFromResource(resourceId, registerObject,
            description, T::GetObjectCLSID(), rootKey);
    }

    // IInitializeWithFile
    HRESULT __stdcall Initialize(PCWSTR filePath, DWORD mode) override
    try
    {
        ATLTRACE(L"InfoTipImpl::Initialize (with file) (instance=%p, mode=%d, filename=%s)\n", this, mode, filePath);

        if (m_initialized)
            return HRESULT_FROM_WIN32(ERROR_ALREADY_INITIALIZED);

        InitializeCore(filePath, mode);
        m_initialized = true;
        return S_OK;
    }
    catch (...)
    {
        return ExceptionToHResult();
    }

    InfoTipImpl(const InfoTipImpl&) = delete;
    InfoTipImpl(InfoTipImpl&&) = delete;
    InfoTipImpl& operator=(const InfoTipImpl&) = delete;
    InfoTipImpl& operator=(InfoTipImpl&&) = delete;

protected:
    InfoTipImpl() noexcept
    {
        ATLTRACE(L"InfoTipImpl::InfoTipImpl (instance=%p)\n", this);
    }

    ~InfoTipImpl()
    {
        ATLTRACE(L"InfoTipImpl::~InfoTipImpl (instance=%p)\n", this);
    }

    virtual void InitializeCore(PCWSTR filePath, DWORD mode) = 0;

private:
    bool m_initialized{};
};

} // namespace msf
