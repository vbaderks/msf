//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msf_base.h"

#include "update_registry.h"
#include "shell_ext_init_impl.h"
#include "str_util.h"
#include "context_command.h"
#include "custom_menu_handler.h"

#include <memory>
#include <strsafe.h>

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
            const auto menu = CreatePopupMenu();
            RaiseLastErrorExceptionIf(!menu);
            return menu;
        }

        Menu() = default;
        ~Menu() = default;

        Menu(HMENU menu, uint32_t indexMenu, uint32_t& idCmd, uint32_t idCmdLast, ContextMenuImpl<T>* menuHost) noexcept :
            m_menu{menu},
            m_indexMenu{indexMenu},
            m_pidCmd{&idCmd},
            m_idCmdLast{idCmdLast},
            m_menuHost{menuHost}
        {
        }

        Menu(const Menu& other) :
            m_menu{other.m_menu},
            m_indexMenu{other.m_indexMenu},
            m_pidCmd{other.m_pidCmd},
            m_idCmdLast{other.m_idCmdLast},
            m_menuHost{other.m_menuHost}
        {
        }

        Menu(Menu&& other) = default;

        Menu& operator=(const Menu& rhs)
        {
            if (this != &rhs)
            {
                m_menu      = rhs.m_menu;
                m_indexMenu = rhs.m_indexMenu;
                m_pidCmd    = rhs.m_pidCmd;
                m_idCmdLast = rhs.m_idCmdLast;
                m_menuHost  = rhs.m_menuHost;
            }

            return *this;
        }

        Menu& operator=(Menu&&) = delete;

        // ReSharper disable once CppNonExplicitConversionOperator
        operator HMENU() const
        {
            return m_menu;
        }

        // Purpose: Create and add a sub menu to the context menu.
        Menu AddSubMenu(std::wstring text, std::wstring help)
        {
            auto subMenu = CreateSubMenu();
            const MenuItemInfo menuiteminfo(*m_pidCmd, std::move(text), subMenu);
            InsertMenuItem(menuiteminfo, std::move(help),
                           std::move(std::unique_ptr<ContextMenuCommand>(nullptr)), std::move(std::unique_ptr<CustomMenuHandler>(nullptr)));

            return Menu(subMenu, 0, *m_pidCmd, m_idCmdLast, m_menuHost);
        }

        // Purpose: alternative format, that loads the strings from the resource.
        Menu AddSubMenu(uint32_t nIDText, uint32_t nIDHelp)
        {
            return AddSubMenu(LoadResourceString(nIDText).c_str(), LoadResourceString(nIDHelp).c_str());
        }

        // Purpose: create and add a owner drawn custom sub menu to the context menu.
        Menu AddSubMenu(std::wstring strHelp, std::unique_ptr<CustomMenuHandler> customMenuHandler)
        {
            auto subMenu = CreateSubMenu();
            MenuItemInfo menuiteminfo(*m_pidCmd, subMenu);
            customMenuHandler->InitializeItemInfo(menuiteminfo);
            InsertMenuItem(menuiteminfo, std::move(strHelp), std::move(std::unique_ptr<ContextMenuCommand>(nullptr)), std::move(customMenuHandler));

            return Menu(subMenu, 0, *m_pidCmd, m_idCmdLast, m_menuHost);
        }

        // Purpose: alternative format, that loads the string from the resource.
        Menu AddSubMenu(uint32_t nIDHelp, std::unique_ptr<CustomMenuHandler> custommenuhandler)
        {
            return AddSubMenu(LoadResourceString(nIDHelp), std::move(custommenuhandler));
        }

        void AddItem(std::wstring text, std::wstring helpText,
                     std::unique_ptr<ContextMenuCommand> contextCommand)
        {
            const MenuItemInfo menuiteminfo(*m_pidCmd, std::move(text));
            InsertMenuItem(menuiteminfo, std::move(helpText), std::move(contextCommand), std::unique_ptr<CustomMenuHandler>(nullptr));
        }

        // Purpose: alternative format, that loads the strings from the resource.
        void AddItem(uint32_t nIDText, uint32_t nIDHelp,
                     std::unique_ptr<ContextMenuCommand> contextCommand)
        {
            AddItem(LoadResourceString(nIDText), LoadResourceString(nIDHelp), std::move(contextCommand));
        }

        void AddItem(std::wstring helpText,
                     std::unique_ptr<ContextMenuCommand> contextCommand,
                     std::unique_ptr<CustomMenuHandler> customMenuHandler)
        {
            MenuItemInfo menuItemInfo(*m_pidCmd);

            customMenuHandler->InitializeItemInfo(menuItemInfo);
            InsertMenuItem(menuItemInfo, std::move(helpText), std::move(contextCommand), std::move(customMenuHandler));
        }

        // Purpose: alternative format, that loads the strings from the resource.
        void AddItem(uint32_t nIDHelp,
                     std::unique_ptr<ContextMenuCommand> contextCommand,
                     std::unique_ptr<CustomMenuHandler> customMenuHandler)
        {
            AddItem(LoadResourceString(nIDHelp), std::move(contextCommand), std::move(customMenuHandler));
        }

        void AddSeparator()
        {
            RaiseLastErrorExceptionIf(
                !InsertMenu(m_menu, m_indexMenu, MF_BYPOSITION | MF_SEPARATOR, 0, nullptr));

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
                ATLTRACE(L"Menu::CheckID: Out of id space (idCmd=%d, idCmdLast=%d)\n", *m_pidCmd, m_idCmdLast);
                RaiseException();
            }
        }

        void AddItem(HMENU hSubmenu, std::wstring strText, std::wstring strHelp)
        {
            MenuItemInfo menuItemInfo(*m_pidCmd, std::move(strText));

            menuItemInfo.SetSubMenu(hSubmenu);
            InsertMenuItem(menuItemInfo, std::move(strHelp), std::move(std::unique_ptr<ContextMenuCommand>(nullptr)), std::move(std::unique_ptr<CustomMenuHandler>(nullptr)));
        }

        void InsertMenuItem(const MenuItemInfo& menuItemInfo,
                            std::wstring strHelp,
                            std::unique_ptr<ContextMenuCommand> contextCommand,
                            std::unique_ptr<CustomMenuHandler> customMenuHandler)
        {
            CheckID();

            RaiseLastErrorExceptionIf(
                !::InsertMenuItem(m_menu, m_indexMenu, true, &menuItemInfo));
            PostAddItem(std::move(strHelp), std::move(contextCommand), std::move(customMenuHandler));
        }

        void PostAddItem(std::wstring strHelp,
                         std::unique_ptr<ContextMenuCommand> contextCommand,
                         std::unique_ptr<CustomMenuHandler> customMenuHandler)
        {
            m_menuHost->OnAddMenuItem(std::move(strHelp), std::move(contextCommand), std::move(customMenuHandler));
            ++m_indexMenu;
            ++*m_pidCmd;
        }

        // Member variables.
        HMENU m_menu{};
        uint32_t m_indexMenu{};
        uint32_t* m_pidCmd{};
        uint32_t m_idCmdLast{};
        ContextMenuImpl<T>* m_menuHost{};
    };

    ContextMenuImpl(const ContextMenuImpl&) = delete;
    ContextMenuImpl(ContextMenuImpl&&) = delete;
    ContextMenuImpl& operator=(const ContextMenuImpl&) = delete;
    ContextMenuImpl& operator=(ContextMenuImpl&&) = delete;

    /// <summary>Registration function to register the COM object and a ProgId/extension.</summary>
    static HRESULT __stdcall UpdateRegistry(BOOL bRegister, uint32_t nResId,
        PCWSTR szDescription, PCWSTR szRootKey) noexcept
    {
        return UpdateRegistryFromResource(nResId, bRegister,
            szDescription, T::GetObjectCLSID(), szRootKey);
    }

    // IContextMenu
    HRESULT __stdcall QueryContextMenu( _In_ HMENU menu, uint32_t indexMenu, uint32_t idCmdFirst, uint32_t idCmdLast, uint32_t uFlags) noexcept override
    {
        ATLTRACE(L"ContextMenuImpl::IContextMenu::QueryContextMenu, instance=%p, iM=%d, idFirst=%d, idLast=%d, flag=%x\n",
            this, indexMenu, idCmdFirst, idCmdLast, uFlags);

        try
        {
            // If the flags include CMF_DEFAULTONLY then we shouldn't do anything.
            if (uFlags & CMF_DEFAULTONLY)
                return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, 0);

            ClearMenuItems();

            m_idCmdFirst = idCmdFirst;
            auto id = m_idCmdFirst;
            Menu contextMenu(menu, indexMenu, id, idCmdLast, this);

            QueryContextMenuCore(contextMenu, GetFilenames());

            const auto nAdded = id - m_idCmdFirst;
            return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, static_cast<USHORT>(nAdded));
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    HRESULT __stdcall GetCommandString(UINT_PTR idCmd, uint32_t uFlags, __reserved uint32_t* /* pwReserved */, LPSTR pszName, uint32_t cchMax) noexcept override
    {
        ATLTRACE(L"ContextMenuImpl::IContextMenu::GetCommandString, instance=%p, flags=%x", this, uFlags);

        try
        {
            if ((uFlags & (GCS_HELPTEXTA & GCS_HELPTEXTW)) != 0)
            {
                const auto& helpText = GetMenuItem(static_cast<uint32_t>(idCmd)).GetHelpText();

                if (uFlags & GCS_UNICODE)
                {
                    ATLTRACE(L" (unicode help text)\n");
                    return StringCchCopy(reinterpret_cast<wchar_t*>(pszName), cchMax, helpText.c_str());
                }

                ATLTRACE(L" (ansi help text)\n");
                return StringCchCopyA(pszName, cchMax, ATL::CT2CA(helpText.c_str()));
            }

            ATLTRACE(L"\n");
            return E_NOTIMPL;
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    HRESULT __stdcall InvokeCommand(_In_ CMINVOKECOMMANDINFO* pici) noexcept override
    {
        ATLTRACE(L"CContextMenu::IContextMenu::InvokeCommand, instance=%p\n", this);

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
    HRESULT __stdcall HandleMenuMsg(uint32_t uMsg, WPARAM wParam, LPARAM lParam) noexcept override
    {
        ATLTRACE(L"ContextMenuImpl::IContextMenu2::HandleMenuMsg (forwarding to HandleMenuMsg2)\n");
        return HandleMenuMsg2(uMsg, wParam, lParam, nullptr);
    }


    // IContextMenu3

    // Note: The SDK docs tell that this function is only called for
    //       WM_MENUCHAR but this is not true (seen on XP sp2).
    //       HandleMenuMsg2 is called also directly for WM_INITMENUPOPUP, etc when
    //       the shell detects that IContextMenu3 is supported.
    HRESULT __stdcall HandleMenuMsg2(uint32_t message, WPARAM wParam, LPARAM lParam,  _Out_opt_ LRESULT* result) noexcept override
    {
        try
        {
            // Always initialize out parameters.
            if (result)
            {
                *result = 0;
            }

            switch (message)
            {
            case WM_INITMENUPOPUP:
                ATLTRACE(L"ContextMenuImpl::IContextMenu3::HandleMenuMsg2 (OnInitMenuPopup)\n");
                return static_cast<T*>(this)->OnInitMenuPopup(reinterpret_cast<HMENU>(wParam), LOWORD(lParam));

            case WM_DRAWITEM:
                ATLTRACE(L"ContextMenuImpl::IContextMenu3::HandleMenuMsg2 (OnDrawItem)\n");
                return static_cast<T*>(this)->OnDrawItem(reinterpret_cast<DRAWITEMSTRUCT*>(lParam));

            case WM_MEASUREITEM:
                ATLTRACE(L"ContextMenuImpl::IContextMenu3::HandleMenuMsg2 (OnMeasureItem)\n");
                return static_cast<T*>(this)->OnMeasureItem(reinterpret_cast<MEASUREITEMSTRUCT*>(lParam));

            case WM_MENUCHAR:
                ATLTRACE(L"ContextMenuImpl::IContextMenu3::HandleMenuMsg2 (OnMenuChar)\n");
                if (!result)
                    return E_FAIL;

                *result = static_cast<T*>(this)->OnMenuChar(reinterpret_cast<HMENU>(lParam), LOWORD(wParam));
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
    void OnAddMenuItem(std::wstring helpText,
                       std::unique_ptr<ContextMenuCommand> contextCommand,
                       std::unique_ptr<CustomMenuHandler> customMenuHandler)
    {
#ifdef _DEBUG
        if (customMenuHandler)
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
        ATLTRACE(L"ContextMenuImpl::Constructor (instance=%p)\n", this);
    }

    ~ContextMenuImpl()
    {
        ATLTRACE(L"ContextMenuImpl::~ContextMenuImpl (instance=%p)\n", this);
    }

    // Derived classes need to implement this function if they want to extend
    // the context menu.
    virtual void QueryContextMenuCore(Menu& /*menu*/, const std::vector<std::wstring>& /*filenames*/) noexcept(false)
    {
    }

    HRESULT OnInitMenuPopup(HMENU /*menu*/, unsigned short /*index*/) noexcept
    {
        return S_OK;
    }

    HRESULT OnDrawItem(const DRAWITEMSTRUCT* drawItem)
    {
        if (drawItem->CtlType != ODT_MENU)
            return E_INVALIDARG;

        GetMenuItem(drawItem->itemID - m_idCmdFirst).GetCustomMenuHandler().Draw(*drawItem);
        return S_OK;
    }

    HRESULT OnMeasureItem(MEASUREITEMSTRUCT* measureItem)
    {
        GetMenuItem(measureItem->itemID - m_idCmdFirst).GetCustomMenuHandler().Measure(*measureItem);
        return S_OK;
    }

    LRESULT OnMenuChar(HMENU menu, unsigned short nChar)
    {
        for (auto& menuItem : m_menuItems)
        {
            LRESULT result;
            if (menuItem.GetCustomMenuHandler().OnMenuChar(menu, nChar, result))
            {
                return result;
            }
        }

        return MAKELONG(0, MNC_IGNORE);
    }

private:
    class MenuItem
    {
    public:
        MenuItem(std::wstring helpText,
                 std::unique_ptr<ContextMenuCommand> contextCommand,
                 std::unique_ptr<CustomMenuHandler> customMenuHandler) noexcept :
            m_helpText{ std::move(helpText) },
            m_contextCommand{ std::move(contextCommand) },
            m_customMenuHandler{ std::move(customMenuHandler) }
        {
        }

        const std::wstring& GetHelpText() const noexcept
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

    const MenuItem& GetMenuItem(uint32_t nIndex)
    {
        RaiseExceptionIf(nIndex >= m_menuItems.size(), E_INVALIDARG);
        return m_menuItems[nIndex];
    }

    // Member variables
    std::vector<MenuItem> m_menuItems;
    unsigned int m_idCmdFirst{};
};

} // end namespace msf
