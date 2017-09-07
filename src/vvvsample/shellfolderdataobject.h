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


class CShellFolderDataObject :
    public ATL::CComObjectRootEx<ATL::CComSingleThreadModel>,
    public MSF::ShellFolderDataObjectImpl<CShellFolderDataObject>
{
public:
    DECLARE_NOT_AGGREGATABLE(CShellFolderDataObject)

    BEGIN_COM_MAP(CShellFolderDataObject)
        COM_INTERFACE_ENTRY(IDataObject)
    END_COM_MAP()

    static ATL::CComPtr<IDataObject> CreateInstance(LPCITEMIDLIST pidlFolder, UINT cidl,
        LPCITEMIDLIST* ppidl, MSF::IPerformedDropEffectSink* pperformeddropeffectsink)
    {
        ATL::CComObject<CShellFolderDataObject>* instance;
        HRESULT hr = ATL::CComObject<CShellFolderDataObject>::CreateInstance(&instance);
        if (FAILED(hr))
            MSF::RaiseException(hr);

        ATL::CComPtr<IDataObject> rdataobject(instance);
        instance->Initialize(pidlFolder, cidl, ppidl, pperformeddropeffectsink);
        return rdataobject;
    }

protected:
    ~CShellFolderDataObject() = default;

private:
    void Initialize(LPCITEMIDLIST pidlFolder, UINT cidl, LPCITEMIDLIST* ppidl,
                    MSF::IPerformedDropEffectSink* pperformeddropeffectsink)
    {
        Init(pidlFolder, cidl, ppidl, pperformeddropeffectsink);

        RegisterClipboardFormatHandler(std::make_unique<CCfFileDescriptorHandler>(this));
        RegisterClipboardFormatHandler(std::make_unique<CfFileContentsHandler>(this));
    }
};
