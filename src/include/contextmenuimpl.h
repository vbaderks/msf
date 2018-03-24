//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"
#include "updateregistry.h"
#include "shellextinitimpl.h"
#include "strutil.h"
#include "contextcommand.h"
#include "custommenuhandler.h"
#include <memory>


namespace msf
{

template <typename T>
class __declspec(novtable) ContextMenuImpl :
    public IShellExtInitImpl,
    public IContextMenu3
{
public:
    class Menu
    {
    public:
        static HMENU CreateSubMenu()
        {
            const auto hmenu = CreatePopupMenu();
            RaiseLastErrorExceptionIf(!hmenu);
            return hmenu;
        }

        Menu() = default;
        ~Menu() = default;

        Menu(HMENU hmenu, UINT indexMenu, UINT& idCmd, UINT idCmdLast, ContextMenuImpl<T>* pmenuhost) noexcept :
            m_hmenu{hmenu},
            m_indexMenu{indexMenu},
            m_pidCmd{&idCmd},
            m_idCmdLast{idCmdLast},
            m_pmenuhost{pmenuhost}
        {
        }

        Menu(const Menu& other) :
            m_hmenu{other.m_hmenu},
            m_indexMenu{other.m_indexMenu},
            m_pidCmd{other.m_pidCmd},
            m_idCmdLast{other.m_idCmdLast},
            m_pmenuhost{other.m_pmenuhost}
        {
        }

        Menu(Menu&& other) = default;

        Menu& operator=(const Menu& rhs)
        {
            if (this != &rhs)
            {
                m_hmenu     = rhs.m_hmenu;
                m_indexMenu = rhs.m_indexMenu;
                m_pidCmd    = rhs.m_pidCmd;
                m_idCmdLast = rhs.m_idCmdLast;
                m_pmenuhost = rhs.m_pmenuhost;
            }

            return *this;
        }

        Menu& operator=(Menu&& other) = delete;

        operator HMENU() const
        {
            return m_hmenu;
        }

        // Purpose: Create and add a sub menu to the context menu.
        Menu AddSubMenu(const std::wstring strText, const std::wstring strHelp)
        {
            auto hmenu = CreateSubMenu();
            const MenuItemInfo menuiteminfo(*m_pidCmd, std::move(strText), hmenu);
            InsertMenuItem(menuiteminfo, std::move(strHelp),
                           std::move(std::unique_ptr<ContextMenuCommand>(nullptr)), std::move(std::unique_ptr<CustomMenuHandler>(nullptr)));

            return Menu(hmenu, 0, *m_pidCmd, m_idCmdLast, m_pmenuhost);
        }

        // Purpose: alternative format, that loads the strings from the resource.
        Menu AddSubMenu(UINT nIDText, UINT nIDHelp)
        {
            return AddSubMenu(LoadResourceString(nIDText).c_str(), LoadResourceString(nIDHelp).c_str());
        }

        // Purpose: create and add a owner drawn custom sub menu to the context menu.
        Menu AddSubMenu(const std::wstring strHelp, std::unique_ptr<CustomMenuHandler> qcustommenuhandler)
        {
            auto hmenu = CreateSubMenu();
            MenuItemInfo menuiteminfo(*m_pidCmd, hmenu);
            qcustommenuhandler->InitializeItemInfo(menuiteminfo);
            InsertMenuItem(menuiteminfo, std::move(strHelp), std::move(std::unique_ptr<ContextMenuCommand>(nullptr)), std::move(qcustommenuhandler));

            return Menu(hmenu, 0, *m_pidCmd, m_idCmdLast, m_pmenuhost);
        }

        // Purpose: alternative format, that loads the string from the resource.
        Menu AddSubMenu(UINT nIDHelp, std::unique_ptr<CustomMenuHandler> custommenuhandler)
        {
            return AddSubMenu(LoadResourceString(nIDHelp), std::move(custommenuhandler));
        }

        void AddItem(const std::wstring text, const std::wstring helpText,
                     std::unique_ptr<ContextMenuCommand> contextCommand)
        {
            const MenuItemInfo menuiteminfo(*m_pidCmd, std::move(text));
            InsertMenuItem(menuiteminfo, std::move(helpText), std::move(contextCommand), std::unique_ptr<CustomMenuHandler>(nullptr));
        }

        // Purpose: alternative format, that loads the strings from the resource.
        void AddItem(UINT nIDText, UINT nIDHelp,
                     std::unique_ptr<ContextMenuCommand> qcontextcommand)
        {
            AddItem(LoadResourceString(nIDText), LoadResourceString(nIDHelp), std::move(qcontextcommand));
        }

        void AddItem(const std::wstring strHelp,
                     std::unique_ptr<ContextMenuCommand> qcontextcommand,
                     std::unique_ptr<CustomMenuHandler> qcustommenuhandler)
        {
            MenuItemInfo menuiteminfo(*m_pidCmd);

            qcustommenuhandler->InitializeItemInfo(menuiteminfo);

            InsertMenuItem(menuiteminfo, std::move(strHelp), std::move(qcontextcommand), std::move(qcustommenuhandler));
        }

        // Purpose: alternative format, that loads the strings from the resource.
        void AddItem(UINT nIDHelp,
                     std::unique_ptr<ContextMenuCommand> qcontextcommand,
                     std::unique_ptr<CustomMenuHandler> qcustommenuhandler)
        {
            AddItem(LoadResourceString(nIDHelp), std::move(qcontextcommand), std::move(qcustommenuhandler));
        }

        void AddSeparator()
        {
            RaiseLastErrorExceptionIf(
                !InsertMenu(m_hmenu, m_indexMenu, MF_BYPOSITION | MF_SEPARATOR, 0, nullptr));

            ++m_indexMenu;
        }

    private:

        // Purpose: Every menu item must have an id between idCmdFirst and idCmdLast.
        //          If we don't have enough space, throw an exception. This will prevent
        //          that we add an incomplete menu.
        void CheckID() const
        {
            if (*m_pidCmd >= m_idCmdLast)
            {
                ATLTRACE2(ATL::atlTraceCOM, 0, L"Menu::CheckID: Out of id space (idCmd=%d, idCmdLast=%d)\n", *m_pidCmd, m_idCmdLast);
                RaiseException();
            }
        }

        void AddItem(HMENU hSubmenu, const std::wstring strText, const std::wstring strHelp)
        {
            MenuItemInfo menuiteminfo(*m_pidCmd, strText);

            menuiteminfo.SetSubMenu(hSubmenu);

            InsertMenuItem(menuiteminfo, strHelp, std::move(std::unique_ptr<ContextMenuCommand>(nullptr)), std::move(std::unique_ptr<CustomMenuHandler>(nullptr)));
        }

        void InsertMenuItem(const MenuItemInfo& menuiteminfo,
                            const std::wstring strHelp,
                            std::unique_ptr<ContextMenuCommand> qcontextcommand,
                            std::unique_ptr<CustomMenuHandler> qcustommenuhandler)
        {
            CheckID();

            RaiseLastErrorExceptionIf(
                !::InsertMenuItem(m_hmenu, m_indexMenu, true, &menuiteminfo));

            PostAddItem(strHelp, std::move(qcontextcommand), std::move(qcustommenuhandler));
        }

        void PostAddItem(const std::wstring strHelp,
                         std::unique_ptr<ContextMenuCommand> qcontextcommand,
                         std::unique_ptr<CustomMenuHandler> qcustommenuhandler)
        {
            m_pmenuhost->OnAddMenuItem(strHelp, std::move(qcontextcommand), std::move(qcustommenuhandler));

            ++m_indexMenu;
            ++*m_pidCmd;
        }

        // Member variables.
        HMENU m_hmenu{};
        UINT m_indexMenu{};
        UINT* m_pidCmd{};
        UINT m_idCmdLast{};
        ContextMenuImpl<T>* m_pmenuhost{};
    };

    ContextMenuImpl(const ContextMenuImpl&) = delete;
    ContextMenuImpl(ContextMenuImpl&&) = delete;
    ContextMenuImpl& operator=(const ContextMenuImpl&) = delete;
    ContextMenuImpl& operator=(ContextMenuImpl&&) = delete;

    /// <summary>Registration function to register the COM object and a ProgId/extension.</summary>
    static HRESULT WINAPI UpdateRegistry(BOOL bRegister, UINT nResId,
        PCWSTR szDescription, PCWSTR szRootKey) noexcept
    {
        return UpdateRegistryFromResource(nResId, bRegister,
            szDescription, T::GetObjectCLSID(), szRootKey);
    }

    // IContextMenu
    HRESULT __stdcall QueryContextMenu( _In_ HMENU hmenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags) noexcept override
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"ContextMenuImpl::IContextMenu::QueryContextMenu, instance=%p, iM=%d, idFirst=%d, idLast=%d, flag=%x\n",
            this, indexMenu, idCmdFirst, idCmdLast, uFlags);

        try
        {
            // If the flags include CMF_DEFAULTONLY then we shouldn't do anything.
            if (uFlags & CMF_DEFAULTONLY)
                return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, 0);

            ClearMenuItems();

            m_idCmdFirst = idCmdFirst;
            auto nID = m_idCmdFirst;
            Menu menu(hmenu, indexMenu, nID, idCmdLast, this);

            QueryContextMenuCore(menu, GetFilenames());

            const auto nAdded = nID - m_idCmdFirst;
            return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, static_cast<USHORT>(nAdded));
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    HRESULT __stdcall GetCommandString(UINT_PTR idCmd, UINT uFlags, __reserved UINT* /* pwReserved */, LPSTR pszName, UINT cchMax) noexcept override
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"ContextMenuImpl::IContextMenu::GetCommandString, instance=%p, flags=%x", this, uFlags);

        try
        {
            if ((uFlags & (GCS_HELPTEXTA & GCS_HELPTEXTW)) != 0)
            {
                const auto str = GetMenuItem(static_cast<UINT>(idCmd)).GetHelpString();

                if (uFlags & GCS_UNICODE)
                {
                    ATLTRACE2(ATL::atlTraceCOM, 0, L" (unicode help text)\n");
                    if (!lstrcpynW(reinterpret_cast<wchar_t*>(pszName), str.c_str(), static_cast<int>(cchMax)))
                        return E_FAIL;
                }
                else
                {
                    ATLTRACE2(ATL::atlTraceCOM, 0, L" (ansi help text)\n");
                    if (!lstrcpynA(pszName, ATL::CT2CA(str.c_str()), static_cast<int>(cchMax)))
                        return E_FAIL;
                }

                return S_OK;
            }

            ATLTRACE2(ATL::atlTraceCOM, 0, L"\n");
            return E_NOTIMPL;
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    HRESULT __stdcall InvokeCommand(_In_ CMINVOKECOMMANDINFO* pici) noexcept override
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"CContextMenu::IContextMenu::InvokeCommand, instance=%p\n", this);

        try
        {
            if (HIWORD(pici->lpVerb) != 0)
                return E_INVALIDARG; // verbs not supported.

            GetMenuItem(LOWORD(pici->lpVerb)).GetContextCommand()(pici, GetFilenames());

            return S_OK;
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    // IContextMenu2
    HRESULT __stdcall HandleMenuMsg(UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept override
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"ContextMenuImpl::IContextMenu2::HandleMenuMsg (forwarding to HandleMenuMsg2)\n");
        return HandleMenuMsg2(uMsg, wParam, lParam, nullptr);
    }


    // IContextMenu3

    // Note: The SDK docs tell that this function is only called for
    //       WM_MENUCHAR but this is not true (seen on XP sp2).
    //       HandleMenuMsg2 is called also directly for WM_INITMENUPOPUP, etc when
    //       the shell detects that IContextMenu3 is supported.
    HRESULT __stdcall HandleMenuMsg2(UINT uMsg, WPARAM wParam, LPARAM lParam,  _Out_opt_ LRESULT* plResult) noexcept override
    {
        try
        {
            // Always initialize out parameters.
            if (plResult)
            {
                *plResult = 0;
            }

            switch (uMsg)
            {
            case WM_INITMENUPOPUP:
                ATLTRACE2(ATL::atlTraceCOM, 0, L"ContextMenuImpl::IContextMenu3::HandleMenuMsg2 (OnInitMenuPopup)\n");
                return static_cast<T*>(this)->OnInitMenuPopup(reinterpret_cast<HMENU>(wParam), LOWORD(lParam));

            case WM_DRAWITEM:
                ATLTRACE2(ATL::atlTraceCOM, 0, L"ContextMenuImpl::IContextMenu3::HandleMenuMsg2 (OnDrawItem)\n");
                return static_cast<T*>(this)->OnDrawItem(reinterpret_cast<DRAWITEMSTRUCT*>(lParam));

            case WM_MEASUREITEM:
                ATLTRACE2(ATL::atlTraceCOM, 0, L"ContextMenuImpl::IContextMenu3::HandleMenuMsg2 (OnMeasureItem)\n");
                return static_cast<T*>(this)->OnMeasureItem(reinterpret_cast<MEASUREITEMSTRUCT*>(lParam));

            case WM_MENUCHAR:
                ATLTRACE2(ATL::atlTraceCOM, 0, L"ContextMenuImpl::IContextMenu3::HandleMenuMsg2 (OnMenuChar)\n");
                if (!plResult)
                    return E_FAIL;

                *plResult = static_cast<T*>(this)->OnMenuChar(reinterpret_cast<HMENU>(lParam), LOWORD(wParam));
                return S_OK;

            default:
                ATLASSERT(!"detected unsupported message");
                return E_FAIL;
            }
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    // 'IMenuHost'
    void OnAddMenuItem(const std::wstring helpText,
                       std::unique_ptr<ContextMenuCommand> contextCommand,
                       std::unique_ptr<CustomMenuHandler> customMenuHandler)
    {
#ifdef _DEBUG
        if (customMenuHandler.get()) // TODO use boolean overload.
        {
            const ATL::CComQIPtr<IContextMenu2> rcontextmenu(this);
            ATLASSERT(rcontextmenu && "custom draw handler requires IContextMenu2");
        }
#endif

        m_menuItems.push_back(MenuItem(std::move(helpText), std::move(contextCommand), std::move(customMenuHandler)));
    }

protected:

    ContextMenuImpl() noexcept
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"ContextMenuImpl::Constructor (instance=%p)\n", this);
    }

    ~ContextMenuImpl()
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"ContextMenuImpl::~ContextMenuImpl (instance=%p)\n", this);
    }

    // Derived classes need to implement this function if they want to extend
    // the context menu.
    virtual void QueryContextMenuCore(Menu& /*menu*/, const std::vector<std::wstring>& /*filenames*/)
    {
    }

    HRESULT OnInitMenuPopup(HMENU /*hmenu*/, unsigned short /*nIndex*/) noexcept
    {
        return S_OK;
    }

    HRESULT OnDrawItem(const DRAWITEMSTRUCT* pdrawitem)
    {
        if (pdrawitem->CtlType != ODT_MENU)
            return E_INVALIDARG;

        GetMenuItem(pdrawitem->itemID - m_idCmdFirst).GetCustomMenuHandler().Draw(*pdrawitem);
        return S_OK;
    }

    HRESULT OnMeasureItem(MEASUREITEMSTRUCT* pmeasureitem)
    {
        GetMenuItem(pmeasureitem->itemID - m_idCmdFirst).GetCustomMenuHandler().Measure(*pmeasureitem);
        return S_OK;
    }

    LRESULT OnMenuChar(HMENU hmenu, unsigned short nChar)
    {
        for (auto& menuItem : m_menuItems)
        {
            LRESULT lresult;
            if (menuItem.GetCustomMenuHandler().OnMenuChar(hmenu, nChar, lresult))
            {
                return lresult;
            }
        }

        return MAKELONG(0, MNC_IGNORE);
    }

private:

    class MenuItem
    {
    public:
        MenuItem(const std::wstring helpText,
                 std::unique_ptr<ContextMenuCommand> contextCommand,
                 std::unique_ptr<CustomMenuHandler> customMenuHandler) :
            m_helpText{ std::move(helpText) },
            m_contextCommand{ std::move(contextCommand) },
            m_customMenuHandler{ std::move(customMenuHandler) }
        {
        }

        const std::wstring& GetHelpString() const noexcept
        {
            return m_helpText;
        }

        ContextMenuCommand& GetContextCommand() const noexcept
        {
            return *m_contextCommand;
        }

        CustomMenuHandler& GetCustomMenuHandler() const noexcept
        {
            return *m_customMenuHandler;
        }

    private:

        std::wstring m_helpText;
        std::unique_ptr<ContextMenuCommand> m_contextCommand;
        std::unique_ptr<CustomMenuHandler> m_customMenuHandler;
    };

    void ClearMenuItems() noexcept
    {
        m_menuItems.clear();
    }

    const MenuItem& GetMenuItem(UINT nIndex)
    {
        RaiseExceptionIf(nIndex >= m_menuItems.size(), E_INVALIDARG);
        return m_menuItems[nIndex];
    }

    // Member variables
    std::vector<MenuItem> m_menuItems;
    unsigned int m_idCmdFirst{};
};

} // end namespace msf
