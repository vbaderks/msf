//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "contextmenuimpl.h"


namespace MSF
{

struct IShellFolderContextMenuSink : public IUnknown
{
    virtual HRESULT OnPasteCmCmd() throw() = 0;
};


class ATL_NO_VTABLE CShellFolderContextMenu :
    public CComObjectRootEx<CComSingleThreadModel>,
    public IContextMenuImpl<CShellFolderContextMenu>
{
public:
    DECLARE_NOT_AGGREGATABLE(CShellFolderContextMenu)

    BEGIN_COM_MAP(CShellFolderContextMenu)
        COM_INTERFACE_ENTRY(IContextMenu)
    END_COM_MAP()


    static CComPtr<IContextMenu> CreateInstance(IShellFolderContextMenuSink* pshellfoldercontextmenusink)
    {
        CComObject<CShellFolderContextMenu>* pinstance;
        HRESULT hr = CComObject<CShellFolderContextMenu>::CreateInstance(&pinstance);
        if (FAILED(hr))
            RaiseException(hr);

        CComPtr<IContextMenu> contextmenu(pinstance);

        pinstance->Init(pshellfoldercontextmenusink);

        return contextmenu;
    }


    void Init(IShellFolderContextMenuSink* pshellfoldercontextmenusink)
    {
        _rshellfoldercontextmenusink = pshellfoldercontextmenusink;
    }


    STDMETHOD(InvokeCommand)(_In_ CMINVOKECOMMANDINFO* pici)
    {
        ATLTRACE2(atlTraceCOM, 0, _T("CShellFolderContextMenu::InvokeCommand, instance=%p\n"), this);

        if (strcmp(pici->lpVerb, "paste") == 0)
            return _rshellfoldercontextmenusink->OnPasteCmCmd();

        return S_OK;
    }

private:
    CComPtr<IShellFolderContextMenuSink> _rshellfoldercontextmenusink;
};

} // end namespace MSF
