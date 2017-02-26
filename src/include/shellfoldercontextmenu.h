//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "contextmenuimpl.h"


namespace MSF
{

struct IShellFolderContextMenuSink : IUnknown
{
    virtual HRESULT OnPasteCmCmd() noexcept = 0;

protected:
    ~IShellFolderContextMenuSink() = default;
};


class ATL_NO_VTABLE ShellFolderContextMenu :
    public CComObjectRootEx<CComSingleThreadModel>,
    public ContextMenuImpl<ShellFolderContextMenu>
{
public:
    DECLARE_NOT_AGGREGATABLE(ShellFolderContextMenu)

    BEGIN_COM_MAP(ShellFolderContextMenu)
        COM_INTERFACE_ENTRY(IContextMenu)
    END_COM_MAP()

    static CComPtr<IContextMenu> CreateInstance(IShellFolderContextMenuSink* pshellfoldercontextmenusink)
    {
        CComObject<ShellFolderContextMenu>* pinstance;
        HRESULT hr = CComObject<ShellFolderContextMenu>::CreateInstance(&pinstance);
        if (FAILED(hr))
            RaiseException(hr);

        CComPtr<IContextMenu> contextmenu(pinstance);

        pinstance->Init(pshellfoldercontextmenusink);

        return contextmenu;
    }

    STDMETHOD(InvokeCommand)(_In_ CMINVOKECOMMANDINFO* pici) override
    {
        ATLTRACE2(atlTraceCOM, 0, L"ShellFolderContextMenu::IContextMenu::InvokeCommand (instance=%p)\n", this);

        if (strcmp(pici->lpVerb, "paste") == 0)
            return _rshellfoldercontextmenusink->OnPasteCmCmd();

        return S_OK;
    }

private:

    void Init(IShellFolderContextMenuSink* pshellfoldercontextmenusink)
    {
        _rshellfoldercontextmenusink = pshellfoldercontextmenusink;
    }

    CComPtr<IShellFolderContextMenuSink> _rshellfoldercontextmenusink;
};

} // end namespace MSF
