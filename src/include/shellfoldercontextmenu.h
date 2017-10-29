//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "contextmenuimpl.h"


namespace msf
{

struct IShellFolderContextMenuSink : IUnknown
{
    virtual HRESULT OnPasteCmCmd() noexcept = 0;

protected:
    ~IShellFolderContextMenuSink() = default;
};


class __declspec(novtable) ShellFolderContextMenu :
    public ATL::CComObjectRootEx<ATL::CComSingleThreadModel>,
    public ContextMenuImpl<ShellFolderContextMenu>
{
public:
    DECLARE_NOT_AGGREGATABLE(ShellFolderContextMenu)

    BEGIN_COM_MAP(ShellFolderContextMenu)
        COM_INTERFACE_ENTRY(IContextMenu)
    END_COM_MAP()

    static ATL::CComPtr<IContextMenu> CreateInstance(IShellFolderContextMenuSink* pshellfoldercontextmenusink)
    {
        ATL::CComObject<ShellFolderContextMenu>* pinstance;
        HRESULT hr = ATL::CComObject<ShellFolderContextMenu>::CreateInstance(&pinstance);
        if (FAILED(hr))
            RaiseException(hr);

        ATL::CComPtr<IContextMenu> contextmenu(pinstance);

        pinstance->Init(pshellfoldercontextmenusink);

        return contextmenu;
    }

    HRESULT __stdcall InvokeCommand(_In_ CMINVOKECOMMANDINFO* pici) noexcept override
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderContextMenu::IContextMenu::InvokeCommand (instance=%p)\n", this);

        if (strcmp(pici->lpVerb, "paste") == 0)
            return _rshellfoldercontextmenusink->OnPasteCmCmd();

        return S_OK;
    }

private:

    void Init(IShellFolderContextMenuSink* pshellfoldercontextmenusink)
    {
        _rshellfoldercontextmenusink = pshellfoldercontextmenusink;
    }

    ATL::CComPtr<IShellFolderContextMenuSink> _rshellfoldercontextmenusink;
};

} // end namespace msf
