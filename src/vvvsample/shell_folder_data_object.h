//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software license.
//
#pragma once

#include "cf_file_descriptor_handler.h"
#include "cf_file_contents_handler.h"

#include <msf.h>
#include <memory>


class ShellFolderDataObject :
    public ATL::CComObjectRootEx<ATL::CComSingleThreadModel>,
    public msf::ShellFolderDataObjectImpl<ShellFolderDataObject>
{
public:
    DECLARE_NOT_AGGREGATABLE(ShellFolderDataObject)

    BEGIN_COM_MAP(ShellFolderDataObject)
        COM_INTERFACE_ENTRY(IDataObject)
    END_COM_MAP()

    static ATL::CComPtr<IDataObject> CreateInstance(PCIDLIST_ABSOLUTE pidlFolder, uint32_t cidl,
        PCUITEMID_CHILD_ARRAY ppidl, msf::IPerformedDropEffectSink* performedDropEffectSink)
    {
        ATL::CComObject<ShellFolderDataObject>* instance;
        const HRESULT hr = ATL::CComObject<ShellFolderDataObject>::CreateInstance(&instance);
        if (FAILED(hr))
            msf::RaiseException(hr);

        ATL::CComPtr<IDataObject> dataObject(instance);
        instance->Initialize(pidlFolder, cidl, ppidl, performedDropEffectSink);
        return dataObject;
    }

    ShellFolderDataObject(const ShellFolderDataObject&) = delete;
    ShellFolderDataObject(ShellFolderDataObject&&) = delete;
    ShellFolderDataObject& operator=(const ShellFolderDataObject&) = delete;
    ShellFolderDataObject& operator=(ShellFolderDataObject&&) = delete;

protected:
    ShellFolderDataObject() noexcept(false) = default; // noexcept(false) needed as ATL base class is not defined noexcept.
    ~ShellFolderDataObject() = default;

private:
    void Initialize(PCIDLIST_ABSOLUTE pidlFolder, uint32_t cidl, PCUITEMID_CHILD_ARRAY ppidl,
                    msf::IPerformedDropEffectSink* performedDropEffectSink)
    {
        Init(pidlFolder, cidl, ppidl, performedDropEffectSink);

        RegisterClipboardFormatHandler(std::make_unique<CfFileDescriptorHandler>(this));
        RegisterClipboardFormatHandler(std::make_unique<CfFileContentsHandler>(this));
    }
};
