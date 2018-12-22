//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"
#include "updateregistry.h"


namespace msf
{

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
        ATLTRACE2(ATL::atlTraceCOM, 0, L"DropTargetImpl::GetClassID (instance=%p, ClassId=%p)\n", this, classId);

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

    HRESULT __stdcall Load(LPCOLESTR filename, DWORD mode) noexcept override
    {
        UNREFERENCED_PARAMETER(mode); // unused in release.

        ATLTRACE2(ATL::atlTraceCOM, 0, L"DropTargetImpl::Load (instance=%p, mode=%d, filename=%s)\n", this, mode, filename);
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
    HRESULT __stdcall DragEnter(_In_ IDataObject* dataObject, DWORD grfKeyState, POINTL pt, _In_ DWORD* pdwEffect) noexcept override
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"DropTargetImpl::IDropTarget::DragEnter (instance=%p, grfKeyState=%d, dwEffect=%d)\n", this, grfKeyState, *pdwEffect);

        try
        {
            // Derived class needs to implement: bool IsSupportedClipboardFormat(IDataObject* dataObject)
            if (dataObject || !static_cast<T*>(this)->IsSupportedClipboardFormat(dataObject))
            {
                *pdwEffect = DROPEFFECT_NONE;
                m_isCachedSupportedClipboardFormat = false;
            }
            else
            {
                *pdwEffect = static_cast<T*>(this)->OnDragOver(grfKeyState, pt, *pdwEffect);
                m_isCachedSupportedClipboardFormat = true;
            }

            return S_OK;
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    HRESULT __stdcall DragOver(DWORD grfKeyState, POINTL pt, _In_ DWORD* pdwEffect) noexcept override
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"DropTargetImpl::IDropTarget::DragOver (instance=%p, grfKeyState=%d, dwEffect=%d)\n", this, grfKeyState, *pdwEffect);

        try
        {
            if (m_isCachedSupportedClipboardFormat)
            {
                // Derived class needs to implement: DWORD OnDragOver(DWORD grfKeyState, POINTL pt, DWORD dwEffect)
                *pdwEffect = static_cast<T*>(this)->OnDragOver(grfKeyState, pt, *pdwEffect);
            }
            else
            {
                *pdwEffect = DROPEFFECT_NONE;
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
        ATLTRACE2(ATL::atlTraceCOM, 0, L"DropTargetImpl::IDropTarget::DragLeave (instance=%p)\n", this);
        m_isCachedSupportedClipboardFormat = false;
        return S_OK;
    }

    HRESULT __stdcall Drop(_In_ IDataObject* dataObject, DWORD grfKeyState, POINTL pt, _In_ DWORD* pdwEffect) noexcept override
    {
        UNREFERENCED_PARAMETER(grfKeyState);
        ATLTRACE2(ATL::atlTraceCOM, 0, L"DropTargetImpl::IDropTarget::Drop (instance=%p, grfKeyState=%d, dwEffect=%d)\n", this, grfKeyState, *pdwEffect);

        try
        {
            // Derived class needs to implement: DWORD OnDrop(IDataObject* dataObject, DWORD grfKeyState, POINTL pt, DWORD dwEffect)
            *pdwEffect = static_cast<T*>(this)->OnDrop(dataObject, grfKeyState, pt, *pdwEffect);
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
        ATLTRACE2(ATL::atlTraceCOM, 0, L"DropTargetImpl::DropTargetImpl (instance=%p)\n", this);
    }

    ~DropTargetImpl()
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"DropTargetImpl::~DropTargetImpl (instance=%p)\n", this);
    }

private:
    std::wstring m_filename;
    bool m_isCachedSupportedClipboardFormat{};
};

} // namespace msf
