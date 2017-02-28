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


namespace MSF
{

template <typename T>
class ATL_NO_VTABLE ContextMenuImpl :
    public IShellExtInitImpl,
    public IContextMenu3
{
public:
    class CMenu
    {
    public:
        static HMENU CreateSubMenu()
        {
            auto hmenu = CreatePopupMenu();
            RaiseLastErrorExceptionIf(!hmenu);
            return hmenu;
        }

        CMenu() :
            _hmenu(nullptr),
            _indexMenu(0),
            _pidCmd(nullptr),
            _idCmdLast(0),
            _pmenuhost(nullptr)
        {
        }

        CMenu(HMENU hmenu, UINT indexMenu, UINT& idCmd, UINT idCmdLast, ContextMenuImpl<T>* pmenuhost) :
            _hmenu(hmenu),
            _indexMenu(indexMenu),
            _pidCmd(&idCmd),
            _idCmdLast(idCmdLast),
            _pmenuhost(pmenuhost)
        {
        }

        CMenu(const CMenu& other)
        {
            _hmenu = other._hmenu;
            _indexMenu = other._indexMenu;
            _pidCmd = other._pidCmd;
            _idCmdLast = other._idCmdLast;
            _pmenuhost = other._pmenuhost;
        }

        CMenu& operator=(const CMenu& rhs)
        {
            if (this != &rhs)
            {
                _hmenu     = rhs._hmenu;
                _indexMenu = rhs._indexMenu;
                _pidCmd    = rhs._pidCmd;
                _idCmdLast = rhs._idCmdLast;
                _pmenuhost = rhs._pmenuhost;
            }

            return *this;
        }

        operator HMENU() const
        {
            return _hmenu;
        }

        // Purpose: Create and add a sub menu to the context menu.
        CMenu AddSubMenu(const CString& strText, const CString& strHelp)
        {
            auto hmenu = CreateSubMenu();
            CMenuItemInfo menuiteminfo(*_pidCmd, strText, hmenu);
            InsertMenuItem(menuiteminfo, strHelp, CContextCommandPtr(nullptr), CCustomMenuHandlerPtr(nullptr));

            return CMenu(hmenu, 0, *_pidCmd, _idCmdLast, _pmenuhost);
        }


        // Purpose: alternative format, that loads the strings from the resource.
        CMenu AddSubMenu(UINT nIDText, UINT nIDHelp)
        {
            return AddSubMenu(LoadString(nIDText), LoadString(nIDHelp));
        }


        // Purpose: create and add a owner drawn custom sub menu to the context menu.
        CMenu AddSubMenu(const CString& strHelp, std::unique_ptr<CCustomMenuHandler> qcustommenuhandler)
        {
            auto hmenu = CreateSubMenu();
            CMenuItemInfo menuiteminfo(*_pidCmd, hmenu);
            qcustommenuhandler->InitializeItemInfo(menuiteminfo);
            InsertMenuItem(menuiteminfo, strHelp, std::move(std::unique_ptr<CContextCommand>(nullptr)), std::move(qcustommenuhandler));

            return CMenu(hmenu, 0, *_pidCmd, _idCmdLast, _pmenuhost);
        }


        // Purpose: alternative format, that loads the string from the resource.
        CMenu AddSubMenu(UINT nIDHelp, std::unique_ptr<CCustomMenuHandler> qcustommenuhandler)
        {
            return AddSubMenu(LoadString(nIDHelp), std::move(qcustommenuhandler));
        }


        void AddItem(const CString& strText, const CString& strHelp,
                     std::unique_ptr<CContextCommand> qcontextcommand)
        {
            CMenuItemInfo menuiteminfo(*_pidCmd, strText);
            InsertMenuItem(menuiteminfo, strHelp, std::move(qcontextcommand), std::unique_ptr<CCustomMenuHandler>(nullptr));
        }


        // Purpose: alternative format, that loads the strings from the resource.
        void AddItem(UINT nIDText, UINT nIDHelp,
                     std::unique_ptr<CContextCommand> qcontextcommand)
        {
            AddItem(LoadString(nIDText), LoadString(nIDHelp), std::move(qcontextcommand));
        }


        void AddItem(const CString& strHelp,
                     std::unique_ptr<CContextCommand> qcontextcommand,
                     std::unique_ptr<CCustomMenuHandler> qcustommenuhandler)
        {
            CMenuItemInfo menuiteminfo(*_pidCmd);

            qcustommenuhandler->InitializeItemInfo(menuiteminfo);

            InsertMenuItem(menuiteminfo, strHelp, std::move(qcontextcommand), std::move(qcustommenuhandler));
        }


        // Purpose: alternative format, that loads the strings from the resource.
        void AddItem(UINT nIDHelp,
                     std::unique_ptr<CContextCommand> qcontextcommand,
                     std::unique_ptr<CCustomMenuHandler> qcustommenuhandler)
        {
            AddItem(LoadString(nIDHelp), std::move(qcontextcommand), std::move(qcustommenuhandler));
        }


        void AddSeparator()
        {
            RaiseLastErrorExceptionIf(
                !InsertMenu(_hmenu, _indexMenu, MF_BYPOSITION | MF_SEPARATOR, 0, nullptr));

            ++_indexMenu;
        }

    private:

        // Purpose: Every menu item must have an id between idCmdFirst and idCmdLast.
        //          If we don't have enough space, throw an exception. This will prevent
        //          that we add an incomplete menu.
        void CheckID() const
        {
            if (*_pidCmd >= _idCmdLast)
            {
                ATLTRACE2(atlTraceCOM, 0, L"CMenu::CheckID: Out of id space (idCmd=%d, idCmdLast=%d)\n", *_pidCmd, _idCmdLast);
                RaiseException();
            }
        }


        void AddItem(HMENU hSubmenu, const CString& strText, const CString& strHelp)
        {
            CMenuItemInfo menuiteminfo(*_pidCmd, strText);

            menuiteminfo.SetSubMenu(hSubmenu);

            InsertMenuItem(menuiteminfo, strHelp, CContextCommandPtr(nullptr), CCustomMenuHandlerPtr(nullptr));
        }


        void InsertMenuItem(const CMenuItemInfo& menuiteminfo,
                            const CString& strHelp,
                            std::unique_ptr<CContextCommand> qcontextcommand,
                            std::unique_ptr<CCustomMenuHandler> qcustommenuhandler)
        {
            CheckID();

            RaiseLastErrorExceptionIf(
                !::InsertMenuItem(_hmenu, _indexMenu, true, &menuiteminfo));

            PostAddItem(strHelp, std::move(qcontextcommand), std::move(qcustommenuhandler));
        }


        void PostAddItem(const CString& strHelp,
                         std::unique_ptr<CContextCommand> qcontextcommand,
                         std::unique_ptr<CCustomMenuHandler> qcustommenuhandler)
        {
            _pmenuhost->OnAddMenuItem(strHelp, std::move(qcontextcommand), std::move(qcustommenuhandler));

            ++_indexMenu;
            ++(*_pidCmd);
        }

        // Member variables.
        HMENU                _hmenu;
        UINT                 _indexMenu;
        UINT*                _pidCmd;
        UINT                 _idCmdLast;
        ContextMenuImpl<T>* _pmenuhost;
    };


    /// <summary>Registration function to register the COM object and a ProgId/extension.</summary>
    static HRESULT WINAPI UpdateRegistry(BOOL bRegister, UINT nResId,
        PCWSTR szDescription, PCWSTR szRootKey) noexcept
    {
        return UpdateRegistryFromResource(nResId, bRegister,
            szDescription, T::GetObjectCLSID(), szRootKey);
    }


    ContextMenuImpl()
    {
        ATLTRACE2(atlTraceCOM, 0, L"ContextMenuImpl::Constructor (instance=%p)\n", this);
    }


    ~ContextMenuImpl()
    {
        ATLTRACE2(atlTraceCOM, 0, L"ContextMenuImpl::~ContextMenuImpl (instance=%p)\n", this);

        ClearMenuItems();
    }


    // IContextMenu
    STDMETHOD(QueryContextMenu)( _In_ HMENU hmenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags) override
    {
        ATLTRACE2(atlTraceCOM, 0, L"ContextMenuImpl::IContextMenu::QueryContextMenu, instance=%p, iM=%d, idFirst=%d, idLast=%d, flag=%x\n",
            this, indexMenu, idCmdFirst, idCmdLast, uFlags);

        try
        {
            // If the flags include CMF_DEFAULTONLY then we shouldn't do anything.
            if (uFlags & CMF_DEFAULTONLY)
                return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, 0);

            ClearMenuItems();

            _idCmdFirst = idCmdFirst;
            UINT nID = _idCmdFirst;
            CMenu menu(hmenu, indexMenu, nID, idCmdLast, this);

            static_cast<T*>(this)->OnQueryContextMenu(menu, GetFilenames());

            const UINT nAdded = nID - _idCmdFirst;
            return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, static_cast<USHORT>(nAdded));
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    STDMETHOD(GetCommandString)(UINT_PTR idCmd, UINT uFlags, __reserved UINT* /* pwReserved */, LPSTR pszName, UINT cchMax) override
    {
        ATLTRACE2(atlTraceCOM, 0, L"ContextMenuImpl::IContextMenu::GetCommandString, instance=%p, flags=%x", this, uFlags);

        try
        {
            if ((uFlags & (GCS_HELPTEXTA & GCS_HELPTEXTW)) != 0)
            {
                CString str = GetMenuItem(static_cast<UINT>(idCmd)).GetHelpString();

                if (uFlags & GCS_UNICODE)
                {
                    ATLTRACE2(atlTraceCOM, 0, L" (unicode help text)\n");
                    if (!lstrcpynW(reinterpret_cast<wchar_t*>(pszName), CT2CW(str), static_cast<int>(cchMax)))
                        return E_FAIL;
                }
                else
                {
                    ATLTRACE2(atlTraceCOM, 0, L" (ansi help text)\n");
                    if (!lstrcpynA(pszName, CT2CA(str), static_cast<int>(cchMax)))
                        return E_FAIL;
                }

                return S_OK;
            }

            ATLTRACE2(atlTraceCOM, 0, L"\n");
            return E_NOTIMPL;
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    STDMETHOD(InvokeCommand)(_In_ CMINVOKECOMMANDINFO* pici) override
    {
        ATLTRACE2(atlTraceCOM, 0, L"CContextMenu::IContextMenu::InvokeCommand, instance=%p\n", this);

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
    STDMETHOD(HandleMenuMsg)(UINT uMsg, WPARAM wParam, LPARAM lParam) override
    {
        ATLTRACE2(atlTraceCOM, 0, L"ContextMenuImpl::IContextMenu2::HandleMenuMsg (forwarding to HandleMenuMsg2)\n");
        return HandleMenuMsg2(uMsg, wParam, lParam, nullptr);
    }


    // IContextMenu3
    
    // Note: The SDK docs tell that this function is only called for 
    //       WM_MENUCHAR but this is not true (seen on XP sp2).
    //       HandleMenuMsg2 is called also directly for WM_INITMENUPOPUP, etc when 
    //       the shell detects that IContextMenu3 is supported.
    STDMETHOD(HandleMenuMsg2)(UINT uMsg, WPARAM wParam, LPARAM lParam,  _Out_opt_ LRESULT* plResult) override
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
                ATLTRACE2(atlTraceCOM, 0, L"ContextMenuImpl::IContextMenu3::HandleMenuMsg2 (OnInitMenuPopup)\n");
                return static_cast<T*>(this)->OnInitMenuPopup(reinterpret_cast<HMENU>(wParam), LOWORD(lParam));

            case WM_DRAWITEM:
                ATLTRACE2(atlTraceCOM, 0, L"ContextMenuImpl::IContextMenu3::HandleMenuMsg2 (OnDrawItem)\n");
                return static_cast<T*>(this)->OnDrawItem(reinterpret_cast<DRAWITEMSTRUCT*>(lParam));

            case WM_MEASUREITEM:
                ATLTRACE2(atlTraceCOM, 0, L"ContextMenuImpl::IContextMenu3::HandleMenuMsg2 (OnMeasureItem)\n");
                return static_cast<T*>(this)->OnMeasureItem(reinterpret_cast<MEASUREITEMSTRUCT*>(lParam));

            case WM_MENUCHAR:
                ATLTRACE2(atlTraceCOM, 0, L"ContextMenuImpl::IContextMenu3::HandleMenuMsg2 (OnMenuChar)\n");
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
    void OnAddMenuItem(const CString& strHelp,
                       std::unique_ptr<CContextCommand> qcontextcommand,
                       std::unique_ptr<CCustomMenuHandler> qcustommenuhandler)
    {
#ifdef _DEBUG
        if (qcustommenuhandler.get()) // TODO use boolean overload.
        {
            CComQIPtr<IContextMenu2> rcontextmenu(this);
            ATLASSERT(rcontextmenu && "custom draw handler requires IContextMenu2");
        }
#endif

        _menuitems.push_back(CMenuItem(strHelp, qcontextcommand.get(), qcustommenuhandler.get()));

        // the auto_ptrs are now stored in _menuitems: take ownership.
        qcontextcommand.release();
        qcustommenuhandler.release();
    }

protected:

    // Derived classes need to implement this function if they want to extend
    // the context menu.
    void OnQueryContextMenu(CMenu& /*menu*/, const std::vector<CString>& /*filenames*/)
    {
    }

    HRESULT OnInitMenuPopup(HMENU /*hmenu*/, unsigned short /*nIndex*/)
    {
        return S_OK;
    }

    HRESULT OnDrawItem(DRAWITEMSTRUCT* pdrawitem)
    {
        if (pdrawitem->CtlType != ODT_MENU)
            return E_INVALIDARG;

        GetMenuItem(pdrawitem->itemID - _idCmdFirst).GetCustomMenuHandler().Draw(*pdrawitem);
        return S_OK;
    }

    HRESULT OnMeasureItem(MEASUREITEMSTRUCT* pmeasureitem)
    {
        GetMenuItem(pmeasureitem->itemID - _idCmdFirst).GetCustomMenuHandler().Measure(*pmeasureitem);
        return S_OK;
    }

    LRESULT OnMenuChar(HMENU hmenu, unsigned short nChar)
    {
        for (auto& menuItem : _menuitems)
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

    class CMenuItem
    {
    public:
        CMenuItem(const CString& strHelp,
                  CContextCommand* pcontextcommand,
                  CCustomMenuHandler* pcustommenuhandler) :
            _strHelp(strHelp),
            _pcontextcommand(pcontextcommand),
            _pcustommenuhandler(pcustommenuhandler)
        {
        }

        void Clear() noexcept
        {
            delete _pcontextcommand;
            _pcontextcommand = nullptr;

            delete _pcustommenuhandler;
            _pcustommenuhandler = nullptr;
        }

        CString GetHelpString() const
        {
            return _strHelp;
        }

        CContextCommand& GetContextCommand() const noexcept
        {
            return *_pcontextcommand;
        }


        CCustomMenuHandler& GetCustomMenuHandler() const noexcept
        {
            return *_pcustommenuhandler;
        }

    private:

        CString             _strHelp;
        CContextCommand*    _pcontextcommand;
        CCustomMenuHandler* _pcustommenuhandler;
    };

    void ClearMenuItems() noexcept
    {
        for (auto& menuitem : _menuitems)
        {
            menuitem.Clear();
        }
        _menuitems.clear();
    }

    const CMenuItem& GetMenuItem(UINT nIndex)
    {
        RaiseExceptionIf(nIndex >= _menuitems.size(), E_INVALIDARG);
        return _menuitems[nIndex];
    }

    // Member variables
    std::vector<CMenuItem> _menuitems;
    UINT                   _idCmdFirst;
};

} // end namespace MSF
