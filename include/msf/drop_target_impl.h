//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msf_base.h"
#include "update_registry.h"

namespace msf {

/// <summary>Basic implementation functionality for a Drop Target Handler shell extension COM object.</summary>
/// <remarks>
/// Classes can use this mix-in implementation class to add support to act as an Drop Target Handler COM object.
/// Standard ATL template classes can be used for the generic COM implementation (addref, release, etc).
/// The actual Drop Target Handler class need to setup the COM interface map.
/// The following interfaces should be enabled:
///  - IPersistsFile
///  - IDropTarget
/// </remarks>
template <typename T>
class __declspec(novtable) DropTargetImpl :
    public IPersistFile,
    public IDropTarget
{
public:
    /// <summary>Registration function to register the infotip COM object and a ProgId/extension.</summary>
    static HRESULT __stdcall UpdateRegistry(BOOL bRegister, uint32_t nResId,
        PCWSTR description, PCWSTR rootKey) noexcept
    {
        return UpdateRegistryFromResource(nResId, bRegister,
            description, T::GetObjectCLSID(), rootKey);
    }

    DropTargetImpl(const DropTargetImpl&) = delete;
    DropTargetImpl(DropTargetImpl&&) = delete;
    DropTargetImpl& operator=(const DropTargetImpl&) = delete;
    DropTargetImpl& operator=(DropTargetImpl&&) = delete;

    // IPersistFile
    HRESULT __stdcall GetClassID(__RPC__out CLSID* classId) noexcept override
    {
        ATLTRACE(L"DropTargetImpl::GetClassID (instance=%p, classId=%p)\n", this, classId);

        if (!classId)
            return E_POINTER;

        *classId = T::GetObjectCLSID();
        return S_OK;
    }

    HRESULT __stdcall IsDirty() noexcept override
    {
        ATLTRACENOTIMPL(L"DropTargetImpl::IsDirty");
    }

    HRESULT __stdcall Save(LPCOLESTR, BOOL) noexcept override
    {
        ATLTRACENOTIMPL(L"DropTargetImpl::Save");
    }

    HRESULT __stdcall SaveCompleted(LPCOLESTR) noexcept override
    {
        ATLTRACENOTIMPL(L"DropTargetImpl::SaveCompleted");
    }

    HRESULT __stdcall GetCurFile(LPOLESTR*) noexcept override
    {
        ATLTRACENOTIMPL(L"DropTargetImpl::GetCurFile");
    }

    HRESULT __stdcall Load(LPCOLESTR filename, [[maybe_unused]] DWORD mode) noexcept override
    {
        ATLTRACE(L"DropTargetImpl::Load (instance=%p, mode=%d, filename=%s)\n", this, mode, filename);
        try
        {
            m_filename = filename;
            return S_OK;
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    // IDropTarget
    HRESULT __stdcall DragEnter(_In_ IDataObject* dataObject, DWORD grfKeyState, POINTL pt, _In_ DWORD* effect) noexcept override
    {
        ATLTRACE(L"DropTargetImpl::IDropTarget::DragEnter (instance=%p, grfKeyState=%d, effect=%d)\n", this, grfKeyState, *effect);

        try
        {
            // Derived class needs to implement: bool IsSupportedClipboardFormat(IDataObject* dataObject)
            if (dataObject || !static_cast<T*>(this)->IsSupportedClipboardFormat(dataObject))
            {
                *effect = DROPEFFECT_NONE;
                m_isCachedSupportedClipboardFormat = false;
            }
            else
            {
                *effect = static_cast<T*>(this)->OnDragOver(grfKeyState, pt, *effect);
                m_isCachedSupportedClipboardFormat = true;
            }

            return S_OK;
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    HRESULT __stdcall DragOver(DWORD grfKeyState, POINTL pt, _In_ DWORD* effect) noexcept override
    {
        ATLTRACE(L"DropTargetImpl::IDropTarget::DragOver (instance=%p, grfKeyState=%d, effect=%d)\n", this, grfKeyState, *effect);

        try
        {
            if (m_isCachedSupportedClipboardFormat)
            {
                // Derived class needs to implement: DWORD OnDragOver(DWORD grfKeyState, POINTL pt, DWORD dwEffect)
                *effect = static_cast<T*>(this)->OnDragOver(grfKeyState, pt, *effect);
            }
            else
            {
                *effect = DROPEFFECT_NONE;
            }
            return S_OK;
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    HRESULT __stdcall DragLeave() noexcept override
    {
        ATLTRACE(L"DropTargetImpl::IDropTarget::DragLeave (instance=%p)\n", this);
        m_isCachedSupportedClipboardFormat = false;
        return S_OK;
    }

    HRESULT __stdcall Drop(_In_ IDataObject* dataObject, DWORD modifierFlagsfKeyState, POINTL cursorLocation, _In_ DWORD* effect) noexcept override
    {
        ATLTRACE(L"DropTargetImpl::IDropTarget::Drop (instance=%p, grfKeyState=%d, effect=%d)\n", this, modifierFlagsfKeyState, *effect);

        try
        {
            // Derived class needs to implement: DWORD OnDrop(IDataObject* dataObject, DWORD grfKeyState, POINTL cursorLocation, DWORD effect)
            *effect = static_cast<T*>(this)->OnDrop(dataObject, modifierFlagsfKeyState, cursorLocation, *effect);
            return S_OK;
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

protected:
    DropTargetImpl()
    {
        ATLTRACE(L"DropTargetImpl::DropTargetImpl (instance=%p)\n", this);
    }

    ~DropTargetImpl()
    {
        ATLTRACE(L"DropTargetImpl::~DropTargetImpl (instance=%p)\n", this);
    }

private:
    std::wstring m_filename;
    bool m_isCachedSupportedClipboardFormat{};
};

} // namespace msf
