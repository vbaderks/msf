//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "context_menu_impl.h"


namespace msf
{

struct IShellFolderContextMenuSink : IUnknown
{
    IShellFolderContextMenuSink() = default;
    IShellFolderContextMenuSink(const IShellFolderContextMenuSink&) = delete;
    IShellFolderContextMenuSink(IShellFolderContextMenuSink&&) = delete;
    IShellFolderContextMenuSink& operator=(const IShellFolderContextMenuSink&) = delete;
    IShellFolderContextMenuSink& operator=(IShellFolderContextMenuSink&&) = delete;

    virtual HRESULT OnPasteCmCmd() noexcept = 0;

protected:
    ~IShellFolderContextMenuSink() = default;
};


class __declspec(novtable) ShellFolderContextMenu :
    public ATL::CComObjectRootEx<ATL::CComSingleThreadModel>,
    public ContextMenuImpl<ShellFolderContextMenu>
{
public:
    ShellFolderContextMenu() noexcept(false) = default;  // noexcept(false) needed as ATL base class is not defined noexcept.
    ShellFolderContextMenu(const ShellFolderContextMenu&) = delete;
    ShellFolderContextMenu(ShellFolderContextMenu&&) = delete;
    ShellFolderContextMenu& operator=(const ShellFolderContextMenu&) = delete;
    ShellFolderContextMenu& operator=(ShellFolderContextMenu&&) = delete;

    static ATL::CComPtr<IContextMenu> CreateInstance(IShellFolderContextMenuSink* pshellfoldercontextmenusink)
    {
        ATL::CComObject<ShellFolderContextMenu>* pinstance;
        const HRESULT hr = ATL::CComObject<ShellFolderContextMenu>::CreateInstance(&pinstance);
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
            return m_rshellfoldercontextmenusink->OnPasteCmCmd();

        return S_OK;
    }

    DECLARE_NOT_AGGREGATABLE(ShellFolderContextMenu)

    BEGIN_COM_MAP(ShellFolderContextMenu)
        COM_INTERFACE_ENTRY(IContextMenu)
    END_COM_MAP()

protected:
    ~ShellFolderContextMenu() = default;

private:
    void Init(IShellFolderContextMenuSink* pshellfoldercontextmenusink) noexcept
    {
        m_rshellfoldercontextmenusink = pshellfoldercontextmenusink;
    }

    ATL::CComPtr<IShellFolderContextMenuSink> m_rshellfoldercontextmenusink;
};

} // end namespace msf
