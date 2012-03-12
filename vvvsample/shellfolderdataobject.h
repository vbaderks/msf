//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "../include/shellfolderdataobjectimpl.h"
#include "cffilecontentshandler.h"
#include "cffiledescriptorhandler.h"


class CShellFolderDataObject :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CShellFolderDataObjectImpl<CShellFolderDataObject>
{
public:
    DECLARE_NOT_AGGREGATABLE(CShellFolderDataObject)

    BEGIN_COM_MAP(CShellFolderDataObject)
        COM_INTERFACE_ENTRY(IDataObject)
    END_COM_MAP()


    static CComPtr<IDataObject> CreateInstance(LPCITEMIDLIST pidlFolder, UINT cidl, 
        LPCITEMIDLIST* ppidl, IPerformedDropEffectSink* pperformeddropeffectsink)
    {
        CComObject<CShellFolderDataObject>* pinstance;
        HRESULT hr = CComObject<CShellFolderDataObject>::CreateInstance(&pinstance);
        if (FAILED(hr))
            RaiseException(hr);

        CComPtr<IDataObject> rdataobject(pinstance);
        pinstance->Init(pidlFolder, cidl, ppidl, pperformeddropeffectsink);
        return rdataobject;
    }


    void Init(LPCITEMIDLIST pidlFolder, UINT cidl, LPCITEMIDLIST* ppidl,
        IPerformedDropEffectSink* pperformeddropeffectsink)
    {
        __super::Init(pidlFolder, cidl, ppidl, pperformeddropeffectsink);

        auto_ptr<CCfHandler> qfiledescriptorhandler(new CCfFileDescriptorHandler(this));
        RegisterCfHandler(qfiledescriptorhandler);

        auto_ptr<CCfHandler> qfilecontentshandler(new CCfFileContentsHandler(this));
        RegisterCfHandler(qfilecontentshandler);
    }
};
