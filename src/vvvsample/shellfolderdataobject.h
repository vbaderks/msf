//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "cffiledescriptorhandler.h"
#include "cffilecontentshandler.h"
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

    static ATL::CComPtr<IDataObject> CreateInstance(PCIDLIST_ABSOLUTE pidlFolder, UINT cidl,
        PCUITEMID_CHILD_ARRAY ppidl, msf::IPerformedDropEffectSink* pperformeddropeffectsink)
    {
        ATL::CComObject<ShellFolderDataObject>* instance;
        HRESULT hr = ATL::CComObject<ShellFolderDataObject>::CreateInstance(&instance);
        if (FAILED(hr))
            msf::RaiseException(hr);

        ATL::CComPtr<IDataObject> dataObject(instance);
        instance->Initialize(pidlFolder, cidl, ppidl, pperformeddropeffectsink);
        return dataObject;
    }

protected:
    ~ShellFolderDataObject() = default;

private:
    void Initialize(PCIDLIST_ABSOLUTE pidlFolder, UINT cidl, PCUITEMID_CHILD_ARRAY ppidl,
                    msf::IPerformedDropEffectSink* pperformeddropeffectsink)
    {
        Init(pidlFolder, cidl, ppidl, pperformeddropeffectsink);

        RegisterClipboardFormatHandler(std::make_unique<CCfFileDescriptorHandler>(this));
        RegisterClipboardFormatHandler(std::make_unique<CfFileContentsHandler>(this));
    }
};
