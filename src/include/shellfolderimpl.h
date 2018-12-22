//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"
#include "updateregistry.h"
#include "pidl.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "shelluuids.h"
#include "dfmdefines.h"
#include "cfshellidlist.h"
#include "cfpreferreddropeffect.h"
#include "cfperformeddropeffect.h"
#include "cfpastesucceeded.h"
#include "performeddropeffectsink.h"
#include "idldatacreatefromidarray.h"
#include "itopviewawareitem.h"
#include "iframelayoutdefinition.h"
#include "shellfoldercontextmenu.h"
#include "queryinfo.h"
#include "extracticon.h"
#include "smartptr/shellbrowserptr.h"

#if (!defined(_WIN32_IE) || _WIN32_IE < 0x0501)
    #error _WIN32_IE needs to be at least 0x501 for IShellFolder2 definition.
#endif

namespace msf
{

#ifndef WM_GETISHELLBROWSER
   #define WM_GETISHELLBROWSER (WM_USER + 7)
#endif

const SFGAOF SFGAO_UNDEFINED = 0xFFFFFFFF;

struct __declspec(uuid("6FE2B64C-5012-4B88-BB9D-7CE4F45E3751")) IConnectionFactory;    // Undocumented interface, seen on Windows 8
struct __declspec(uuid("93F81976-6A0D-42C3-94DD-AA258A155470")) IShellUndocumented93;  // Seen on Windows 8
struct __declspec(uuid("CAD9AE9F-56E2-40F1-AFB6-3813E320DCFD")) IShellUndocumentedCA;  // Seen on Windows 8

// The definitions in ShlGuid have no __declspec. Create 'MSF' definitions to make '==' easy.
struct __declspec(uuid("CB316B22-25F7-42B8-8A09-540D23A43C2F")) EP_NavPane;       // The pane on the left side of the Windows Explorer window that hosts the folders tree and Favorites
struct __declspec(uuid("D27524A8-C9F2-4834-A106-DF8889FD4F37")) EP_Ribbon;        // The ribbon. New in Windows 8. Not defined in Win 7 SDK.
struct __declspec(uuid("893C63D1-45C8-4D17-BE19-223BE71BE365")) EP_PreviewPane;   // Pane on the right of the Windows Explorer window that shows a large reading preview of the file.
struct __declspec(uuid("43ABF98B-89B8-472D-B9CE-E69B8229F019")) EP_DetailsPane;   // Pane showing metadata along the bottom of the Windows Explorer window.

template <typename T, typename TItem>
class __declspec(novtable) ShellFolderImpl :
    public IPersistFolder3,
    public IPersistIDList,
    public IShellDetails,
    public IShellFolder2,
    public IObjectWithFolderEnumMode,
    public IShellIcon,
    public IDropTarget,
    public IShellFolderContextMenuSink,
    public IPerformedDropEffectSink,
    public IExplorerPaneVisibility
{
public:
    enum class ErrorContext
    {
        Unknown           = 0,
        CreateEnumIDList  = 1,
        OnSetNameOf       = 2,
        OnProperties      = 3,
        OnDelete          = 4,
        OnCopy            = 5,
        OnCut             = 6,
        OnDrop            = 7,
        OnPaste           = 8,
        OnInvokeAddedCmd  = 9
    };

    ShellFolderImpl(const ShellFolderImpl&) = delete;
    ShellFolderImpl(ShellFolderImpl&&) = delete;
    ShellFolderImpl& operator=(const ShellFolderImpl&) = delete;
    ShellFolderImpl& operator=(ShellFolderImpl&&) = delete;

    // Registration function to register the COM object + the root extension.
    static HRESULT __stdcall UpdateRegistry(BOOL bRegister, uint32_t nResId,
        PCWSTR szDescription, PCWSTR rootExt, uint32_t friendlyTypeNameId) noexcept
    {
        OleString classId;
        StringFromCLSID(T::GetObjectCLSID(), classId);

        const auto friendlyTypeName = std::to_wstring(friendlyTypeNameId);

        ATL::_ATL_REGMAP_ENTRY regmapEntries[] = {
            { L"DESCRIPTION", szDescription },
            { L"CLSID", classId },
            { L"ROOTTYPE", rootExt },
            { L"FRIENDLY_TYPE_NAME", friendlyTypeName.c_str() },
            { nullptr, nullptr } };

        return ATL::_pAtlModule->UpdateRegistryFromResource(nResId, bRegister, regmapEntries);
    }

    static ATL::CString SHGetPathFromIDList(PCIDLIST_ABSOLUTE idList)
    {
        ATL::CString strPath;
        RaiseExceptionIf(!::SHGetPathFromIDList(idList, strPath.GetBufferSetLength(MAX_PATH)));

        strPath.ReleaseBuffer();
        return strPath;
    }

    // Purpose: small helper to support 'type' system.
    static void ChangeNotifyPidl(long wEventId, uint32_t uFlags, LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2 = nullptr) noexcept
    {
        SHChangeNotify(wEventId, uFlags | SHCNF_IDLIST, pidl1, pidl2);
    }

    static void StrToStrRet(const wchar_t* sz, STRRET* pstrret)
    {
        pstrret->uType = STRRET_WSTR;
        RaiseExceptionIfFailed(SHStrDup(sz, &pstrret->pOleStr));
    }

    static void MergeMenus(QCMINFO& qcminfo, HMENU menu,
        ULONG uFlags = MM_ADDSEPARATOR | MM_SUBMENUSHAVEIDS | MM_DONTREMOVESEPS) noexcept
    {
        qcminfo.idCmdFirst =
            Shell_MergeMenus(qcminfo.hmenu, menu, qcminfo.indexMenu,
                             qcminfo.idCmdFirst, qcminfo.idCmdLast, uFlags);
    }

    static ATL::CComPtr<T> CreateInstance()
    {
        ATL::CComObject<T>* instance;
        RaiseExceptionIfFailed(ATL::CComObject<T>::CreateInstance(&instance));

        ATL::CComPtr<T> shellFolder(instance);

        return shellFolder;
    }

    // IPersistFolder
    HRESULT __stdcall GetClassID(__RPC__out CLSID* classID) noexcept override
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::IPersistFolder::GetClassID (instance=%p)\n", this);

        if (!classID)
            return E_POINTER;

        *classID = T::GetObjectCLSID();
        return S_OK;
    }

    HRESULT __stdcall Initialize(__RPC__in PCIDLIST_ABSOLUTE childItem) noexcept override
    {
        try
        {
            ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::IPersistFolder::Initialize (instance=%p, pidl=%p)\n", this, childItem);

            if (!childItem)
                return E_INVALIDARG;

            m_pidlFolder.CloneFrom(childItem);
            return S_OK;
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    // IPersistFolder2
    HRESULT __stdcall GetCurFolder(__RPC__deref_out_opt PIDLIST_ABSOLUTE* childItem) noexcept override
    {
        try
        {
            ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::IPersistFolder2::GetCurFolder (instance=%p)\n", this);

            *childItem = m_pidlFolder.CloneFull();
            return S_OK;
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    // IPersistFolder3
    HRESULT __stdcall InitializeEx(__RPC__in_opt IBindCtx* pbc, __RPC__in PCIDLIST_ABSOLUTE pidlRoot, __RPC__in_opt const PERSIST_FOLDER_TARGET_INFO* ppfti) noexcept override
    {
        UNREFERENCED_PARAMETER(pbc);
        UNREFERENCED_PARAMETER(ppfti);
        ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::IPersistFolder3::InitializeEx (instance=%p, pcb=%p, pidlRoot=%p, ppfti=%p)\n", this, pbc, pidlRoot, ppfti);

        // Note: if ppfti is NULL InitializeEx should act as Initialize.
        return Initialize(pidlRoot);
    }

    HRESULT __stdcall GetFolderTargetInfo(__RPC__out PERSIST_FOLDER_TARGET_INFO* /* ppfti */) noexcept override
    {
        ATLTRACENOTIMPL(L"ShellFolderImpl::IPersistFolder3::GetFolderTargetInfo");
    }

    // IPersistIDList
    HRESULT __stdcall SetIDList(__RPC__in PCIDLIST_ABSOLUTE childItem) noexcept override
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::IPersistIDList::SetIDList (instance=%p, pidl=%p)\n", this, childItem);
        return Initialize(childItem);
    }

    HRESULT __stdcall GetIDList(__RPC__deref_out_opt PIDLIST_ABSOLUTE* childItem) noexcept override
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::IPersistIDList::GetIDList (instance=%p)\n", this);
        return GetCurFolder(childItem);
    }

    // IShellDetails
    HRESULT __stdcall ColumnClick(uint32_t uiColumn) noexcept override
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::IShellDetails::ColumnClick (instance=%p, column=%d)\n", this, uiColumn);

        // mark parameters as not used in release build.
        UNREFERENCED_PARAMETER(uiColumn);

        // Shell 6.0 (Vista and up) can sort by itself, return S_FALSE to trigger this.
        return S_FALSE;
    }

    // IShellFolder

    // Purpose: The shell calls this function to get the IShellFolder interface of a sub folder.
    HRESULT __stdcall BindToObject(__RPC__in PCUIDLIST_RELATIVE pidlSubFolder, __RPC__in_opt LPBC, __RPC__in REFIID interfaceId, __RPC__deref_out_opt void** ppRetVal) noexcept override
    {
        try
        {
            ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::IShellFolder::BindToObject (instance=%p)\n", this);

            // Quick check if requested interface is supported at all (on our self).
            const HRESULT result = static_cast<T*>(this)->QueryInterface(interfaceId, ppRetVal);
            if (FAILED(result))
                return result;

            static_cast<IUnknown*>(*ppRetVal)->Release();

            // Get all sub folder items.
            std::vector<TItem> items;
            while (pidlSubFolder)
            {
                items.push_back(TItem(pidlSubFolder));
                pidlSubFolder = ItemIDList::GetNextItem(pidlSubFolder);
            }

            ATL::CComPtr<T> rinstance = CreateInstance();
            RaiseExceptionIfFailed(rinstance->Initialize(GetRootFolder()));
            rinstance->InitializeSubFolder(items);

            return rinstance->QueryInterface(interfaceId, ppRetVal);
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    HRESULT __stdcall BindToStorage(__RPC__in PCUIDLIST_RELATIVE, LPBC, __RPC__in REFIID, __RPC__deref_out_opt LPVOID* ppRetVal) noexcept override
    {
        *ppRetVal = nullptr;
        ATLTRACENOTIMPL(L"ShellFolderImpl::IShellFolder::BindToStorage");
    }

    // Purpose: This function is called to sort items in details view mode.
    HRESULT __stdcall CompareIDs(LPARAM lParam, __RPC__in PCUIDLIST_RELATIVE pidl1, __RPC__in PCUIDLIST_RELATIVE pidl2) noexcept override
    {
        try
        {
            if (pidl1->mkid.cb == 0 && pidl2->mkid.cb == 0)
            {
                // Win98 sometimes tries to compare empty pidls.
                ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::IShellFolder::CompareIDs (lparam=%d, pidl1=%p, pidl2=%p)\n",
                    lParam, pidl1, pidl2);
                return E_INVALIDARG;
            }

            int nResult = 0;
            while (pidl1 != nullptr && pidl2 != nullptr)
            {
                TItem item1(pidl1);
                TItem item2(pidl2);

                ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::IShellFolder::CompareIDs (lparam=%X, name1=%s, name2=%s)\n",
                    lParam, item1.GetDisplayName(SHGDN_NORMAL).c_str(), item2.GetDisplayName(SHGDN_NORMAL).c_str());

                nResult = static_cast<T*>(this)->CompareItems(lParam, item1, item2);
                if (nResult != 0)
                    break; // different items.

                pidl1 = ItemIDList::GetNextItem(pidl1);
                pidl2 = ItemIDList::GetNextItem(pidl2);

                if (pidl1 == nullptr && pidl2 != nullptr)
                {
                    nResult = -1; // pidl1 is at a higher level than pidl2
                    break;
                }

                if (pidl1 != nullptr && pidl2 == nullptr)
                {
                    nResult = 1; // pidl2 is at a higher level than pidl1
                    break;
                }
            }

            return MAKE_HRESULT(SEVERITY_SUCCESS, 0, static_cast<USHORT>(nResult));
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    // Purpose: the shell calls this function to get interfaces to objects such as:
    //          IShellFolder, IContextMenu or IExtractIcon for the complete folder.
    HRESULT __stdcall CreateViewObject(__RPC__in_opt HWND hwndOwner, __RPC__in REFIID interfaceId, __RPC__deref_out_opt void** ppRetVal) noexcept override
    {
        try
        {
            ATLASSERT(!::IsBadReadPtr(&interfaceId, sizeof(IID)) && "Bad pointer detected");

            m_ownerWindow = hwndOwner;

            if (interfaceId == __uuidof(IShellDetails))
            {
                ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::IShellFolder::CreateViewObject (instance=%p, riid=IShellDetails)\n", this);
                return static_cast<T*>(this)->QueryInterface(interfaceId, ppRetVal);
            }

            if (interfaceId == __uuidof(IShellView))
            {
                ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::IShellFolder::CreateViewObject (instance=%p, riid=IShellView)\n", this);
                *ppRetVal = static_cast<T*>(this)->CreateShellFolderView().Detach();
            }
            else if (interfaceId == __uuidof(IDropTarget))
            {
                ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::IShellFolder::CreateViewObject (instance=%p, riid=IDropTarget)\n", this);
                *ppRetVal = static_cast<T*>(this)->CreateDropTarget().Detach();
            }
            else if (interfaceId == __uuidof(IContextMenu))
            {
                ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::IShellFolder::CreateViewObject (instance=%p, riid=IContextMenu)\n", this);
                *ppRetVal = static_cast<T*>(this)->CreateFolderContextMenu().Detach();
            }
            else if (interfaceId == __uuidof(ITopViewAwareItem))
            {
                ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::IShellFolder::CreateViewObject (instance=%p, riid=ITopViewAwareItem)\n", this);
                *ppRetVal = nullptr; // ITopViewAwareItem is an undocumented interface, purpose not clear.
            }
            else if (interfaceId == __uuidof(IFrameLayoutDefinition))
            {
                ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::IShellFolder::CreateViewObject (instance=%p, riid=IFrameLayoutDefinition)\n", this);
                *ppRetVal = nullptr; // IFrameLayoutDefinition is an undocumented interface, purpose not clear.
            }
            else if (interfaceId == __uuidof(IConnectionFactory))
            {
                ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::IShellFolder::CreateViewObject (instance=%p, riid=IConnectionFactory)\n", this);
                *ppRetVal = nullptr; // IConnectionFactory is an undocumented interface, purpose not clear.
            }
            else if (interfaceId == __uuidof(IShellUndocumented93))
            {
                ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::IShellFolder::CreateViewObject (instance=%p, riid=IShellUndocumented93)\n", this);
                // stack trace analysis: Called when CDefView class initializes the CDefCollection.
                *ppRetVal = nullptr; // IShellUndocumented93 is an undocumented interface, purpose not clear.
            }
            else if (interfaceId == __uuidof(IShellUndocumentedCA))
            {
                ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::IShellFolder::CreateViewObject (instance=%p, riid=IShellUndocumentedCA)\n", this);
                // stack trace analysis: called from CShellItem::BindToHandler to hook an kind of interrupt source.
                *ppRetVal = nullptr; // IShellUndocumentedCA is an undocumented interface, purpose not clear.
            }
            else
            {
#ifdef _ATL_DEBUG_QI
                ATL::AtlDumpIID(interfaceId, L"ShellFolderImpl::CreateViewObject (?)", E_NOINTERFACE);
#endif //  _ATL_DEBUG_QI

                *ppRetVal = nullptr;
            }

            return !(*ppRetVal) ? E_NOINTERFACE : S_OK;
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    // Purpose: The shell will call this function to get an object that can be applied
    //          to a collection of items contained in the folder.
    HRESULT __stdcall GetUIObjectOf(__RPC__in_opt HWND window, uint32_t idListCount, __RPC__in_ecount_full_opt(idListCount) PCUITEMID_CHILD_ARRAY childItem, __RPC__in REFIID interfaceId, __reserved uint32_t* /*reserved*/, __RPC__deref_out_opt void** ppv) noexcept override
    {
        try
        {
            if (!childItem)
                return E_POINTER; // note: is marked with SAL as optional, but docs state that it is required.

            if (interfaceId == __uuidof(IContextMenu))
            {
                ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::IShellFolder::GetUIObjectOf (instance=%p, idListCount=%d, interfaceId=IContextMenu)\n", this, idListCount);
                *ppv = static_cast<T*>(this)->CreateItemContextMenu(window, idListCount, childItem).Detach();
            }
            else if (interfaceId == __uuidof(IDataObject))
            {
                ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::IShellFolder::GetUIObjectOf (instance=%p, idListCount=%d, interfaceId=IDataObject)\n", this, idListCount);
                *ppv = static_cast<T*>(this)->CreateDataObject(m_pidlFolder.GetAbsolute(), idListCount, childItem).Detach();
            }
            else if (interfaceId == __uuidof(IQueryInfo))
            {
                ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::IShellFolder::GetUIObjectOf (instance=%p, idListCount=%d, interfaceId=IQueryInfo)\n", this, idListCount);

                if (idListCount != 1)
                    return E_FAIL; // can only request a tooltip for 1 selected item!

                *ppv = static_cast<T*>(this)->CreateQueryInfo(TItem(childItem[0])).Detach();
            }
            else if (interfaceId == __uuidof(IExtractIcon))
            {
                ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::IShellFolder::GetUIObjectOf (instance=%p, idListCount=%d, interfaceId=IExtractIcon)\n", this, idListCount);

                if (idListCount != 1)
                    return E_FAIL; // can only request a icon for 1 selected item!

                *ppv = static_cast<T*>(this)->CreateExtractIcon(TItem(childItem[0])).Detach();
            }
            else if (interfaceId == __uuidof(IQueryAssociations))
            {
                ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::IShellFolder::GetUIObjectOf (instance=%p, idListCount=%d, interfaceId=IQueryAssociations)\n", this, idListCount);
                *ppv = nullptr;
            }
            else
            {
                ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::IShellFolder::GetUIObjectOf (instance=%p, idListCount=%d, interfaceId=?)\n", this, idListCount);
                #ifdef _ATL_DEBUG_QI
                ATL::AtlDumpIID(interfaceId, L"ShellFolderImpl::IShellFolder::GetUIObjectOf", E_NOINTERFACE);
                #endif //  _ATL_DEBUG_QI

                *ppv = nullptr;
            }

            return !(*ppv) ? E_NOINTERFACE : S_OK;
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    // Purpose: The Shell will call this function to get the name (string) of the item.
    //          (column 0 in details view mode).
    HRESULT __stdcall GetDisplayNameOf(__RPC__in_opt PCUITEMID_CHILD childItem, SHGDNF shgdnf, __RPC__out LPSTRRET lpname) noexcept override
    {
        try
        {
            TItem item(childItem);
            StrToStrRet(item.GetDisplayName(shgdnf).c_str(), lpname);

            ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::GetDisplayNameOf (instance=%p, shgdnf=%x, name=%s)\n",
                      this, shgdnf, item.GetDisplayName(shgdnf).c_str());
            return S_OK;
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    // Purpose: The shell uses this function to retrieve info about what can be done with an item.
    HRESULT __stdcall GetAttributesOf(uint32_t idListCount, __RPC__in_ecount_full_opt(idListCount) PCUITEMID_CHILD_ARRAY apidl, __RPC__inout SFGAOF* prgfInOut) noexcept override
    {
        try
        {
            if (!apidl)
                return E_POINTER; // note: marked with SAL as optional, but docs state that it is required.

            ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::GetAttributesOf (instance=%p, apidl=%p, rgfInOut=%X)\n", this, apidl, *prgfInOut);

            SFGAOF sfgaof = static_cast<T*>(this)->GetAttributesOfGlobal(idListCount, *prgfInOut);

            if (sfgaof == SFGAO_UNDEFINED)
            {
                sfgaof = 0xFFFFFFFF;

                for (uint32_t i = 0; i < idListCount; ++i)
                {
                    sfgaof &= static_cast<T*>(this)->GetAttributeOf(idListCount, TItem(apidl[i]), *prgfInOut);
                }
            }

            // Mask and return the requested attributes;
            *prgfInOut = sfgaof & *prgfInOut;
            return S_OK;
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    HRESULT __stdcall ParseDisplayName(__RPC__in_opt HWND window, LPBC pbc, LPOLESTR pwszDisplayName, __reserved DWORD*, __RPC__deref_out_opt PIDLIST_RELATIVE*, __RPC__inout_opt DWORD* pdwAttributes) noexcept override
    {
        // mark parameters as not used in release build.
        UNREFERENCED_PARAMETER(window);
        UNREFERENCED_PARAMETER(pbc);
        UNREFERENCED_PARAMETER(pwszDisplayName);
        UNREFERENCED_PARAMETER(pdwAttributes);

        #ifdef _DEBUG
        const DWORD dwAttributes = pdwAttributes == nullptr ? 0 : *pdwAttributes;
            ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::ParseDisplayName (hwnd=%d, pcb=%p, dname=%s, attrib=%d)\n", hwnd, pbc, pwszDisplayName, dwAttributes);
        #endif

        return E_NOTIMPL;
    }

    HRESULT __stdcall SetNameOf(_In_opt_ HWND hwndOwner, _In_ PCUITEMID_CHILD childItem, _In_ const OLECHAR* pszNewName, SHGDNF uFlags, _Outptr_opt_ PITEMID_CHILD* ppidlOut) noexcept override
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::SetNameOf (hwnd=%d, szName=%s)\n", hwndOwner, pszNewName);

        try
        {
            // Documented interface specifications requires that ppidlOut is always set.
            if (ppidlOut)
            {
                *ppidlOut = nullptr;
            }

            ItemIDList pidlNewItem(static_cast<T*>(this)->OnSetNameOf(hwndOwner, TItem(childItem), pszNewName, uFlags));

            ChangeNotifyPidl(SHCNE_RENAMEITEM, 0,
                ItemIDList(m_pidlFolder, static_cast<PCUIDLIST_RELATIVE>(childItem)), ItemIDList(m_pidlFolder, pidlNewItem));

            if (ppidlOut)
            {
                *ppidlOut = static_cast<PITEMID_CHILD>(pidlNewItem.DetachRelative());
            }

            return S_OK;
        }
        catch (...)
        {
            return HandleException(hwndOwner, ErrorContext::OnSetNameOf);
        }
    }

    // IShellFolder2
    HRESULT __stdcall EnumObjects(__RPC__in_opt HWND window, DWORD grfFlags, __RPC__deref_out_opt IEnumIDList** ppRetVal) noexcept override
    {
        // TODO: move to IShellFolder1

        try
        {
            ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::EnumObjects (hwnd=%d, grfFlags=%d, path=%s)\n",
                      window, grfFlags, GetPathFolderFile().c_str());
            if (ppRetVal == nullptr)
                return E_POINTER;

            *ppRetVal = static_cast<T*>(this)->CreateEnumIDList(window, grfFlags).Detach();
            return S_OK;
        }
        catch (...)
        {
            return HandleException(window, ErrorContext::CreateEnumIDList);
        }
    }

    HRESULT __stdcall GetDefaultSearchGUID(__RPC__out GUID* /*guid*/) noexcept override
    {
        ATLTRACENOTIMPL(L"ShellFolderImpl::GetDefaultSearchGUID");
    }

    HRESULT __stdcall EnumSearches(__RPC__deref_out_opt IEnumExtraSearch** /*enum*/) noexcept override
    {
        ATLTRACENOTIMPL(L"ShellFolderImpl::EnumSearches");
    }

    HRESULT __stdcall GetDefaultColumn(DWORD /*dwReserved*/, __RPC__out ULONG* pSort, __RPC__out ULONG* pDisplay) noexcept override
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::GetDefaultColumn\n");

        *pSort    = m_sort;
        *pDisplay = m_display;

        return S_OK;
    }

    HRESULT __stdcall GetDefaultColumnState(uint32_t column, __RPC__out SHCOLSTATEF* columnStateFlags) noexcept override
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::GetDefaultColumnState (iColumn=%d)\n", column);

        if (column >= m_columnInfos.size())
            return E_FAIL;

        *columnStateFlags = m_columnInfos[column].m_csFlags;
        return S_OK;
    }

    HRESULT __stdcall GetDetailsEx(__RPC__in_opt PCUITEMID_CHILD /*pidl*/, __RPC__in const SHCOLUMNID* /*pscid*/, __RPC__out VARIANT* /*pv*/) noexcept override
    {
        ATLTRACENOTIMPL(L"ShellFolderImpl::GetDetailsEx");
    }

    HRESULT __stdcall MapColumnToSCID(uint32_t /*iColumn*/, __RPC__out SHCOLUMNID* /*pscid*/) noexcept override
    {
        ATLTRACENOTIMPL(L"ShellFolderImpl::MapColumnToSCID");
    }

    // Purpose: The Shell will call this function to retrieve column header names and
    //          the individual names of the items in the folder.
    // Note: Some windows versions use GetDisplayName to get column 0.
    HRESULT __stdcall GetDetailsOf(__RPC__in_opt PCUITEMID_CHILD childItem, uint32_t column, __RPC__out SHELLDETAILS* shellDetails) noexcept override
    {
        try
        {
            if (column >= m_columnInfos.size())
            {
                ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::GetDetailsOf (iColumn=%d > max)\n", column);
                return E_INVALIDARG;
            }

            if (childItem)
            {
                GetItemDetailsOf(column, childItem, shellDetails);
            }
            else
            {
                GetColumnDetailsOf(column, shellDetails);
            }

            return S_OK;
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    // IObjectWithFolderEnumMode
    HRESULT __stdcall SetMode(FOLDER_ENUM_MODE mode) noexcept override
    {
        // Note: it seems that the shell always passes FEM_VIEW_RESULT.
        UNREFERENCED_PARAMETER(mode);
        ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::IObjectWithFolderEnumMode::SetMode (feMode=%d, 0=FEM_VIEW_RESULT, 1=FEM_NAVIGATION)\n", mode);
        return S_OK;
    }

    HRESULT __stdcall GetMode(__RPC__out FOLDER_ENUM_MODE *mode) noexcept override
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::IObjectWithFolderEnumMode::GetMode (pfeMode=%p)\n", mode);

        // Note: the MSDN docs are unclear what the difference is between the enum modes.
        // Note2: it seems that the shell only calls SetMode to notify the shell folder in which mode to operate and not this method.
        *mode = FEM_VIEWRESULT;
        return S_OK;
    }

    // IShellIcon
    // Purpose: There are 2 ways for the shell to get a icon for the 'view.'
    //          This functions call is preferred by the shell as it slightly faster.
    HRESULT __stdcall GetIconOf(__RPC__in PCUITEMID_CHILD childItem, uint32_t flags, __RPC__out int* pIconIndex) noexcept override
    {
        try
        {
            ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::GetIconOf (flags=%d)\n", flags);

            *pIconIndex = TItem(childItem).GetIconOf(flags);

            return S_OK;
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    // IDropTarget
    HRESULT __stdcall DragEnter(_In_ IDataObject* dataObject, DWORD modifierKeyState, POINTL cursor, _In_ DWORD* effect) noexcept override
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::IDropTarget::DragEnter (modifierKeyState=%d, effect=%d)\n", modifierKeyState, *effect);

        try
        {
            if (dataObject == nullptr || !static_cast<T*>(this)->IsSupportedClipboardFormat(dataObject))
            {
                *effect = DROPEFFECT_NONE;
                m_cachedIsSupportedClipboardFormat = false;
            }
            else
            {
                *effect = static_cast<T*>(this)->OnDragOver(modifierKeyState, cursor, *effect);
                m_cachedIsSupportedClipboardFormat = true;
            }

            return S_OK;
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    HRESULT __stdcall DragOver(DWORD modifierKeyState, POINTL cursor, _In_ DWORD* effect) noexcept override
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::IDropTarget::DragOver (grfKeyState=%d, dwEffect=%d)\n", modifierKeyState, *effect);

        try
        {
            if (m_cachedIsSupportedClipboardFormat)
            {
                *effect = static_cast<T*>(this)->OnDragOver(modifierKeyState, cursor, *effect);
            }
            else
            {
                *effect = DROPEFFECT_NONE;
            }
            return S_OK;
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    HRESULT __stdcall DragLeave() noexcept override
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::IDropTarget::DragLeave\n");
        m_cachedIsSupportedClipboardFormat = false;
        return S_OK;
    }

    HRESULT __stdcall Drop(_In_ IDataObject* dataObject, DWORD modifierKeyState, POINTL cursor, _In_ DWORD* effect) noexcept override
    {
        UNREFERENCED_PARAMETER(modifierKeyState);
        ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::IDropTarget::Drop (modifierKeyState=%d, effect=%d)\n", modifierKeyState, *effect);

        try
        {
            *effect = static_cast<T*>(this)->OnDrop(dataObject, modifierKeyState, cursor, *effect);
            return S_OK;
        }
        catch (...)
        {
            return HandleException(m_ownerWindow, ErrorContext::OnDrop);
        }
    }

    // IExplorerPaneVisibility (introduced with Vista)
    // The shell will use this interface to request which 'panes' should be visible.
    HRESULT __stdcall GetPaneState(_In_ const EXPLORERPANE& explorerPane, _Out_ EXPLORERPANESTATE* explorerPaneState) noexcept override
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::IExplorerPaneVisibility::GetPaneState (instance=%p, ep=%s)\n", this, GetExplorerPaneName(explorerPane).GetString());

        *explorerPaneState = static_cast<T*>(this)->GetPaneState(explorerPane);
        return S_OK;
    }

    // IShellFolderContextMenuSink
    HRESULT OnPasteCmCmd() noexcept override
    {
        try
        {
            static_cast<T*>(this)->OnPaste();
            return S_OK;
        }
        catch (...)
        {
            return HandleException(m_ownerWindow, ErrorContext::OnPaste);
        }
    }

    void InitializeSubFolder(const std::vector<TItem>& /*items*/)
    {
        ATLASSERT(!"Derived class needs to implement this function, if sub folders are used");
        RaiseException();
    }

protected:
    explicit ShellFolderImpl(ULONG sort = 0, ULONG display = 0) noexcept :
        m_sort(sort),
        m_display(display),
        m_ownerWindow(nullptr),
        m_cachedIsSupportedClipboardFormat(false)
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::ShellFolderImpl (instance=%p)\n", this);
    }

    ~ShellFolderImpl()
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::~ShellFolderImpl (instance=%p)\n", this);
    }

    // Purpose: Handle 'paste' command. If the folder cannot use 'optimize move'.
    //          for source operations the source must be notified of this.
    void OnPaste()
    {
        IDataObjectPtr dataObject = OleGetClipboard();

        // Detect if the source was 'cut'
        DWORD dropEffect = CfPreferredDropEffect::GetOptional(dataObject);
        if (dropEffect == DROPEFFECT_MOVE &&
            !static_cast<T*>(this)->CanPerformOptimizedMove(dataObject))
        {
            CCfPerformedDropEffect::SetOptional(dataObject, dropEffect);
        }

        dropEffect = static_cast<T*>(this)->AddItemsFromDataObject(dropEffect, dataObject);

        // Notify the source.
        if (dropEffect == DROPEFFECT_MOVE &&
            !static_cast<T*>(this)->CanPerformOptimizedMove(dataObject))
        {
            CfPasteSucceeded::SetOptional(dataObject, dropEffect);
        }
    }

    // Note: override this function to get different compare functionality.
    int CompareItems(LPARAM lParam, const TItem& item1, const TItem& item2) const
    {
        if (IsBitSet(static_cast<ULONG>(lParam), SHCIDS_ALLFIELDS))
        {
            return static_cast<const T*>(this)->CompareIDsAllFields(item1, item2);
        }

        return item1.Compare(item2,
                             static_cast<USHORT>(lParam), IsBitSet(static_cast<ULONG>(lParam), SHCIDS_CANONICALONLY));
    }

    std::wstring GetPathFolderFile() const
    {
        return std::wstring(SHGetPathFromIDList(m_pidlFolder.GetAbsolute())); // TODO: use SHGetPathFromIDListEx?
    }

    PIDLIST_ABSOLUTE GetRootFolder() const noexcept
    {
        return m_pidlFolder.GetAbsolute();
    }

    // Purpose: called by the shell when it needs a context menu. There are 2 reasons for this:
    // 1) To display the context menu
    // 2) To act as a command sink for menu commands
    ATL::CComPtr<IContextMenu> CreateItemContextMenu(HWND window, uint32_t idListCount, PCUITEMID_CHILD_ARRAY childItem)
    {
        ATL::CComPtr<IContextMenu> contextmenu;

        RaiseExceptionIfFailed(
            CDefFolderMenu_Create2(m_pidlFolder.GetAbsolute(), window, idListCount, childItem, this,
                                   OnDfmCommand, 0, nullptr, &contextmenu));

        return contextmenu;
    }

    // Purpose: Called by the shell when it needs to pack a IDlist into a data object.
    //          Override this function to provide your own DataObject.
    ATL::CComPtr<IDataObject> CreateDataObject(PCIDLIST_ABSOLUTE pidlFolder, uint32_t idListCount, PCUITEMID_CHILD_ARRAY childItem)
    {
        return CIDLData_CreateFromIDArray(pidlFolder, idListCount, reinterpret_cast<PCUIDLIST_RELATIVE_ARRAY>(childItem));
    }

    // Purpose: Called by the shell/MSF when it needs an object that support an IQueryInfo
    //          interface to display a tooltip for an item.
    //          Override this function to handle tooltip at the shell folder level.
    ATL::CComPtr<IQueryInfo> CreateQueryInfo(const TItem& item)
    {
        auto text = item.GetInfoTipText();
        return text.empty() ? nullptr : QueryInfo::CreateInstance(text);
    }

    ATL::CComPtr<IExtractIcon> CreateExtractIcon(const TItem& item)
    {
        return ExtractIcon<TItem>::CreateInstance(item);
    }

    // Purpose: Called by shell/MSF through the CompareItems function.
    //          MSF will compare column by column.
    //          Override this function if needed.
    int CompareIDsAllFields(const TItem& item1, const TItem& item2) const
    {
        int nResult = 0; // if there are no columns, items are always equal.

        for (size_t i = 0; i < m_columnInfos.size(); ++i)
        {
            nResult = item1.Compare(item2, static_cast<USHORT>(i), false);
            if (nResult != 0)
                break; // final result determined.
        }

        return nResult;
    }

    // Purpose: Called by shell/MSF.
    //          It is essential to override this function to control which explorer panes are visible.
    //          The default implementation will just return 'ignore', which will result in almost not visible.
    EXPLORERPANESTATE GetPaneState(_In_ const EXPLORERPANE& /*explorerPane*/)
    {
        return EPS_DONTCARE;
    }

    // Purpose: default callback, used to receive a command from CDefFolderMenu_Create2.
    static HRESULT CALLBACK OnDfmCommand(IShellFolder* psf, HWND window, IDataObject* dataObject,
                                         uint32_t messageId, WPARAM wParam, LPARAM lParam)
    {
        return static_cast<T*>(psf)->OnDfmCommand(window, dataObject, messageId, wParam, lParam);
    }

    HRESULT OnDfmCommand(HWND window, IDataObject* dataObject,
                         uint32_t messageId, WPARAM wParam, LPARAM lParam)
    {
        try
        {
            switch (messageId)
            {
                case DFM_MERGECONTEXTMENU:
                    ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::OnDfmCommand (uMsg=MergeContextMenu, wParam=%d, lParam=%p)\n", wParam, lParam);
                    return static_cast<T*>(this)->OnDfmMergeContextMenu(dataObject, static_cast<uint32_t>(wParam), *reinterpret_cast<QCMINFO*>(lParam));

                case DFM_INVOKECOMMAND:
                     ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::OnDfmInvokeCommand (wParam=%d, lParam=%s)\n", wParam, lParam);
                    return static_cast<T*>(this)->OnDfmInvokeCommand(window, dataObject, static_cast<int>(wParam), reinterpret_cast<wchar_t*>(lParam));

                case DFM_GETDEFSTATICID:
                    ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::OnDfmCommand (uMsg=GetDefStaticID, wParam=%d, lParam=%p)\n", wParam, lParam);
                    return static_cast<T*>(this)->OnDfmGetStaticID(reinterpret_cast<int*>(lParam));

                case DFM_CREATE:
                    ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::OnDfmCommand (uMsg=Create, wParam=%d, lParam=%d)\n", wParam, lParam);
                    return static_cast<T*>(this)->OnDfmCreate();

                case DFM_GETHELPTEXT:
                    ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::OnDfmCommand (uMsg=GetHelpTextA, nCmdId=%hu)\n", LOWORD(wParam));
                    return static_cast<T*>(this)->OnDfmGetHelpTextA(LOWORD(wParam), reinterpret_cast<char*>(lParam), HIWORD(wParam));

                case DFM_GETHELPTEXTW:
                    ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::OnDfmCommand (uMsg=GetHelpTextW, nCmdId=%hu)\n", LOWORD(wParam));
                    return static_cast<T*>(this)->OnDfmGetHelpTextW(LOWORD(wParam), reinterpret_cast<wchar_t*>(lParam), HIWORD(wParam));

                case DFM_WM_MEASUREITEM:
                    ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::OnDfmCommand (uMsg=MeasureItem, wParam=%d, lParam=%d)\n", wParam, lParam);
                    return static_cast<T*>(this)->OnDfmMeasureItem();

                case DFM_WM_DRAWITEM:
                    ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::OnDfmCommand (uMsg=DrawItem, wParam=%d, lParam=%d)\n", wParam, lParam);
                    return static_cast<T*>(this)->OnDfmDrawItem();

                case DFM_GETVERBW:
                    ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::OnDfmCommand (uMsg=GetVerbW, wParam=%d, lParam=%d)\n", wParam, lParam);
                    return static_cast<T*>(this)->OnGetVerbW();

                case DFM_GETVERBA:
                    ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::OnDfmCommand (uMsg=GetVerbA, wParam=%d, lParam=%d)\n", wParam, lParam);
                    return static_cast<T*>(this)->OnGetVerbA();

                case DFM_DESTROY:
                    ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::OnDfmCommand (uMsg=OnDestruct, wParam=%d, lParam=%d)\n", wParam, lParam);
                    break;

                case DFM_MERGECONTEXTMENU_TOP:
                    ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::OnDfmCommand (uMsg=OnPreCreate, wParam=%d, lParam=%d)\n", wParam, lParam);
                    break;

                case DFM_INVOKECOMMANDEX:
                    ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::OnDfmCommand (uMsg=OnPreInvokeCmd, wParam=%d, lParam=%d)\n", wParam, lParam);
                    break;

                case DFM_MERGECONTEXTMENU_BOTTOM:
                    ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::OnDfmCommand (uMsg=OnQueryMenu, wParam=%d, lParam=%d)\n", wParam, lParam);
                    break;

                case DFM_WM_INITMENUPOPUP:
                    ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::OnDfmCommand (uMsg=OnInitMenuPopup, wParam=%d, lParam=%d)\n", wParam, lParam);
                    break;

                default:
                    ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::OnDfmCommand Undocumented (uMsg=%d, wParam=%d, lParam=%d)\n", messageId, wParam, lParam);
                    break;
            }

            return E_NOTIMPL;
        }
        catch (...)
        {
            return HandleException(window, ErrorContext::Unknown);
        }
    }

    // Purpose: override this function to extend the default DFM menu.
    HRESULT OnDfmMergeContextMenu(IDataObject* /*dataObject*/, uint32_t /*flags*/, QCMINFO& /*qcminfo*/)
    {
        return E_NOTIMPL;
    }

    HRESULT OnDfmInvokeCommand(HWND window, IDataObject* dataObject, int id, wchar_t* /*wszArgs*/)
    {
        ErrorContext errorContext = ErrorContext::Unknown;

        try
        {
            // Crack and forward the notification.
            switch (id)
            {
            case DFM_CMD_PROPERTIES:
                ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::OnDfmInvokeCommand 'DFM_CMD_PROPERTIES'\n");
                return static_cast<T*>(this)->OnDfmCmdProperties(window, dataObject);

            case DFM_CMD_DELETE:
                ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::OnDfmInvokeCommand 'DFM_CMD_DELETE'\n");
                errorContext = ErrorContext::OnDelete;
                static_cast<T*>(this)->OnDeleteFromDataObject(window, dataObject);
                return S_OK;

            case DFM_CMD_MOVE:
                ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::OnDfmInvokeCommand 'DFM_CMD_CUT'\n");
                errorContext = ErrorContext::OnCut;
                static_cast<T*>(this)->OnCut(window, dataObject);
                return S_OK;

            case DFM_CMD_COPY:
                ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::OnDfmInvokeCommand 'DFM_CMD_COPY'\n");
                errorContext = ErrorContext::OnCopy;
                static_cast<T*>(this)->OnCopy(window, dataObject);
                return S_OK;

            case DFM_CMD_PASTE:
                ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::OnDfmInvokeCommand 'DFM_CMD_PASTE'\n");
                return static_cast<T*>(this)->OnDfmCmdPaste(window, dataObject);

            case DFM_CMD_LINK: //DFM_CMD_CREATESHORTCUT:
                ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::OnDfmInvokeCommand 'DFM_CMD_CREATESHORTCUT'\n");
                return static_cast<T*>(this)->OnDfmCmdCreateShortcut(window, dataObject);

            default:
                ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::OnDfmInvokeCommand (id=%d)\n", id);
                errorContext = ErrorContext::OnInvokeAddedCmd;
                return static_cast<T*>(this)->OnDfmInvokeAddedCommand(window, dataObject, id);
            }
        }
        catch (...)
        {
            return HandleException(window, errorContext);
        }
    }

    // Purpose: called to ask the default menu item.
    // Note: if the menu has no default menu item, the shell will make
    //       the first item the default when the user double clicks it or
    //       presses enter.
    HRESULT OnDfmGetStaticID(int* /*defaultId*/) noexcept
    {
        return E_NOTIMPL;
    }

    HRESULT OnDfmCreate() noexcept
    {
        return E_NOTIMPL;
    }

    // Purpose: called to get the help string for added menu items.
    //          (on win9x or if HelpTextW failed).
    HRESULT OnDfmGetHelpTextA(unsigned short commandId, char* buffer, unsigned short bufferMax)
    {
        if (bufferMax == 0)
            return E_INVALIDARG; // cannot handle zero size buffers.

        const auto str = static_cast<T*>(this)->OnDfmGetHelpText(commandId);
        strncpy_s(buffer, bufferMax, ATL::CW2A(str.c_str()), _TRUNCATE);
        buffer[bufferMax - 1] = 0;

        return S_OK;
    }

    // Purpose: called to get the help string for added menu items.
    //          (on Win 2K, XP or if HelpTextA failed).
    HRESULT OnDfmGetHelpTextW(unsigned short commandId, wchar_t* buffer, unsigned short bufferMax)
    {
        if (bufferMax == 0)
            return E_INVALIDARG; // cannot handle zero size buffers.

        const auto str = static_cast<T*>(this)->OnDfmGetHelpText(commandId);
        wcsncpy_s(buffer, bufferMax, str.c_str(), _TRUNCATE);
        buffer[bufferMax - 1] = 0;

        return S_OK;
    }

    std::wstring OnDfmGetHelpText(unsigned short /*commandId*/)
    {
        return std::wstring();
    }

    HRESULT OnDfmMeasureItem() noexcept
    {
        return E_NOTIMPL;
    }

    HRESULT OnDfmDrawItem() noexcept
    {
        return E_NOTIMPL;
    }

    HRESULT OnGetVerbW() noexcept
    {
        return E_NOTIMPL;
    }

    HRESULT OnGetVerbA() noexcept
    {
        return E_NOTIMPL;
    }

    HRESULT OnDfmCmdProperties(HWND window, _In_ IDataObject* dataObject)
    {
        try
        {
            CfShellIdList cfshellidlist(dataObject);

            if (cfshellidlist.IsEmpty())
            {
                ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::OnDfmCmdProperties (nothing to do, list is empty)\n");
            }
            else
            {
                std::vector<TItem> items;
                RetrieveItems(cfshellidlist, items);

                VerifyAttribute(cfshellidlist, SFGAO_HASPROPSHEET);

                const long eventId = static_cast<T*>(this)->OnProperties(window, items);

                if (IsBitSet(eventId, SHCNE_RENAMEITEM))
                {
                    ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::OnDfmCmdProperties (firing SHCNE_RENAMEITEM)\n");
                    ReportRenameChangeNotify(cfshellidlist, items);
                }

                if (IsBitSet(eventId, SHCNE_ATTRIBUTES))
                {
                    ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::OnDfmCmdProperties (firing SHCNE_ATTRIBUTES)\n");
                    ReportChangeNotify(items, SHCNE_ATTRIBUTES);
                }
            }

            return S_OK;
        }
        catch (...)
        {
            return HandleException(window, ErrorContext::OnProperties);
        }
    }

    // Purpose: provides a simple delete loop.
    //          Override this function to handle special delete requirements.
    void OnDeleteFromDataObject(HWND window, _In_ IDataObject* dataObject)
    {
        CfShellIdList cfshellidlist(dataObject);

        if (cfshellidlist.IsEmpty())
        {
            ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::OnDeleteFromDataObject (nothing to do, list is empty)\n");
        }
        else
        {
            std::vector<TItem> items;
            RetrieveItems(cfshellidlist, items);

            VerifyAttribute(cfshellidlist, SFGAO_CANDELETE);

            const long wEventId = static_cast<T*>(this)->OnDelete(window, items);

            if (IsBitSet(wEventId, SHCNE_DELETE))
            {
                ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::OnDeleteFromDataObject (firing SHCNE_DELETEs)\n");
                ReportChangeNotify(cfshellidlist, SHCNE_DELETE);
            }

            if (IsBitSet(wEventId, SHCNE_UPDATEDIR))
            {
                ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::OnDeleteFromDataObject (firing SHCNE_UPDATEDIR)\n");
                ChangeNotifyPidl(SHCNE_UPDATEDIR, SHCNF_IDLIST, m_pidlFolder);
            }
        }
    }

    // Purpose: override this function to enable item delete support.
    long OnDelete(HWND, const std::vector<TItem>& /*items*/)
    {
        ATLASSERT(!"If SFGAO_CANDELETE is true this function must be implemented!");
        return 0;
    }

    // Purpose: called by MSF/shell when items (selected earlier with GetUIObjectOf)
    //          must be copied to the clipboard.
    void OnCopy(HWND, IDataObject* dataObject) const
    {
        VerifyAttribute(dataObject, SFGAO_CANCOPY);

        CfPreferredDropEffect::Set(dataObject, DROPEFFECT_COPY);

        RaiseExceptionIfFailed(OleSetClipboard(dataObject));
    }

    // Purpose: called by MSF/shell when the user has selected 'cut'.
    //          (with ctrl-x or menu). Items were are selected with GetUIObjectOf.
    void OnCut(HWND, IDataObject* dataObject) const
    {
        VerifyAttribute(dataObject, SFGAO_CANMOVE);

        CfPreferredDropEffect::Set(dataObject, DROPEFFECT_MOVE);

        RaiseExceptionIfFailed(OleSetClipboard(dataObject));

        // Notify the folder-view object that our items are on the clipboard.
        // This will allow the view to update the look of the items and to handle
        // the escape key to abort a move operation.
        // Note: SDK docs are incorrect. Lparam is actual the command id.
        ShellFolderView_SetClipboard(GetHwndOwner(), DFM_CMD_MOVE);
    }

    HRESULT OnDfmCmdPaste(HWND, IDataObject*) noexcept
    {
        return E_NOTIMPL;
    }

    HRESULT OnDfmCmdCreateShortcut(HWND, IDataObject*) noexcept
    {
        return E_NOTIMPL;
    }

    // Purpose: this function handles 'added' commands.
    HRESULT OnDfmInvokeAddedCommand(HWND, IDataObject*, int /*id*/)
    {
        return E_NOTIMPL;
    }

    // IPerformedDropEffectSink
    void OnDeleteAfterPaste(IDataObject* dataObject) override
    {
        OnDeleteFromDataObject(nullptr, dataObject);
    }

    // Purpose: override this function to enable paste and drop into the shell folder.
    DWORD AddItemsFromDataObject(DWORD /*effect*/, IDataObject*)
    {
        return DROPEFFECT_NONE;
    }

    // Purpose: override this function to enable item property support.
    long OnProperties(HWND, const std::vector<TItem>& /*items*/)
    {
        return 0;
    }

    void GetColumnDetailsOf(uint32_t column, SHELLDETAILS* shellDetails) noexcept
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::GetColumnDetailsOf (column=%d, cxChar=%d)\n", column, shellDetails->cxChar);

        shellDetails->fmt = m_columnInfos[column].m_fmt;
        StrToStrRet(m_columnInfos[column].m_name.c_str(), &shellDetails->str);
    }

    void GetItemDetailsOf(uint32_t column, PCUITEMID_CHILD childItem, SHELLDETAILS* shellDetails) const
    {
        TItem item(childItem);

        ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::GetItemDetailsOf (name=%s, iColumn=%d, str=%s)\n",
                  item.GetDisplayName(SHGDN_NORMAL).c_str(), column,
                  item.GetItemDetailsOf(column).c_str());

        StrToStrRet(item.GetItemDetailsOf(column).c_str(), &shellDetails->str);
    }

    // The ShellFolderImpl uses the system provided shellfolderview.
    // Override this function if you want to use your own shellfolderview object.
    ATL::CComPtr<IShellView> CreateShellFolderView()
    {
        const ATL::CComPtr<IShellFolderViewCB> shellfolderviewcb =
            static_cast<T*>(this)->CreateShellFolderViewCB();

        SFV_CREATE shellFolderViewCreate{sizeof(SFV_CREATE), this, nullptr, shellfolderviewcb};

        ATL::CComPtr<IShellView> shellView;
        RaiseExceptionIfFailed(SHCreateShellFolderView(&shellFolderViewCreate, &shellView));

        return shellView;
    }

    // Override this function to handle drop functionality in a separate object.
    ATL::CComPtr<IDropTarget> CreateDropTarget() noexcept
    {
        ATL::CComPtr<IDropTarget> dropTarget;

        static_cast<T*>(this)->QueryInterface(__uuidof(IDropTarget), reinterpret_cast<void **>(&dropTarget));
        return dropTarget;
    }

    // Override to support own context menu when the user clicks in the folder.
    // The system folder view has a default impl that will be displayed (if NULL is returned).
    ATL::CComPtr<IContextMenu> CreateFolderContextMenu()
    {
        return ShellFolderContextMenu::CreateInstance(this);
    }

    // IShellFolderImpl override functions: derived class must implement these functions.
    ATL::CComPtr<IShellFolderViewCB> CreateShellFolderViewCB()
    {
        // It is not required to use a callback object.
        // It just makes it impossible to catch events.
        return nullptr;
    }

    // Purpose: Standard 'on-drag-over' handler. Override if special drag handling is required.
    DWORD OnDragOver(DWORD modifierKeyState, POINTL /*cursor*/, DWORD effect) noexcept
    {
        if (IsBitSet(effect, DROPEFFECT_MOVE) && IsBitSet(modifierKeyState, MK_SHIFT))
            return DROPEFFECT_MOVE;

        if (IsBitSet(effect, DROPEFFECT_COPY))
            return DROPEFFECT_COPY;

        return DROPEFFECT_NONE;
    }

    // Purpose: called when the user drops the 'thing' on the shellfolder.
    DWORD OnDrop(IDataObject* dataObject, DWORD modifierKeyState, POINTL /*cursor*/, DWORD effect)
    {
        if ((effect & (DROPEFFECT_MOVE | DROPEFFECT_COPY)) == 0 ||
             !dataObject || !static_cast<T*>(this)->IsSupportedClipboardFormat(dataObject))
            return DROPEFFECT_NONE;

        effect = static_cast<T*>(this)->AddItemsFromDataObject(effect, dataObject);

        if (IsBitSet(modifierKeyState, MK_SHIFT) && IsBitSet(effect, DROPEFFECT_MOVE) &&
            !static_cast<T*>(this)->CanPerformOptimizedMove(dataObject))
        {
            CCfPerformedDropEffect::SetOptional(dataObject, DROPEFFECT_MOVE);
            return DROPEFFECT_MOVE;
        }

        return DROPEFFECT_COPY;
    }

    // Purpose: override this function to control which clipboards formats are supported.
    bool IsSupportedClipboardFormat(IDataObject* /*dataObject*/) const
    {
        return false;
    }

    // Purpose: override this function to control what the source should do after a move.
    //          See SDK 'Handling Shell Data Transfer Scenarios' for more info.
    bool CanPerformOptimizedMove(IDataObject* /*dataObject*/) const noexcept
    {
        return false;
    }

#pragma warning(push)
#pragma warning(disable: 26495) // m_csFlags' is uninitialized (False warning in VS 2017 15.8.0)
    class ColumnInfo final
    {
    public:
        ColumnInfo(std::wstring name, int fmt, SHCOLSTATEF csFlags) :
            m_name(std::move(name)), m_fmt(fmt), m_csFlags(csFlags)
        {
        }

        std::wstring m_name;
        int         m_fmt;
        SHCOLSTATEF m_csFlags;
    };
#pragma warning(pop)

    void RegisterColumn(const wchar_t* szName, int fmt, SHCOLSTATEF csFlags = SHCOLSTATE_TYPE_STR | SHCOLSTATE_ONBYDEFAULT)
    {
        ColumnInfo columnInfo(szName, fmt, csFlags);
        m_columnInfos.push_back(columnInfo);
    }

    void RegisterColumn(uint32_t resourceID, int fmt, SHCOLSTATEF csFlags = SHCOLSTATE_TYPE_STR | SHCOLSTATE_ONBYDEFAULT)
    {
        RegisterColumn(LoadResourceString(resourceID).c_str(), fmt, csFlags);
    }

    // Implement this function and return the attributes or SFGAO_UNDEFINED
    SFGAOF GetAttributesOfGlobal(uint32_t /*cidl*/, SFGAOF /*sfgofMask*/) const noexcept
    {
        // As default implementation, return SFGAO_UNDEFINED to start the
        // loop to ask attributes for every item explicit.
        return SFGAO_UNDEFINED;
    }

    // If GetAttributesOfGlobal returns SFGAO_UNDEFINED MSF will call explicit
    // for every selected item this function.
    SFGAOF GetAttributeOf(unsigned int /*cidl*/, const TItem& /*item*/, SFGAOF /*sfgofMask*/) const
    {
        return 0;
    }

    // Purpose: derived classes need to implement this function if they want to support
    //          renaming of items.
    //          Even if SFGAO_CANRENAME is not set, clients can try to set the name.
    ITEMIDLIST* OnSetNameOf(HWND /*hwnd*/, const TItem& /*item*/, const wchar_t* /*newName*/, SHGDNF /*shgdnf*/)
    {
        RaiseException();
    }

    HRESULT OnErrorHandler(HRESULT result, HWND window, ErrorContext errorContext)
    {
        if (window)
        {
            static_cast<T*>(this)->OnError(result, window, errorContext);
        }

        return result;
    }

    // Purpose: derive this function to report error to the user.
    void OnError(HRESULT /*hr*/, HWND /*hwnd*/, ErrorContext /*errorContext*/)
    {
    }

    void RetrieveItems(IDataObject* dataObject, std::vector<TItem>& items) const
    {
        const CfShellIdList cfShellIdList(dataObject);

        RetrieveItems(cfShellIdList, items);
    }

    void RetrieveItems(const CfShellIdList& cfshellidlist, std::vector<TItem>& items) const
    {
        for (size_t i = 0; i < cfshellidlist.GetItemCount(); ++i)
        {
            PCUIDLIST_RELATIVE childItem = cfshellidlist.GetItem(i);
            TItem item(childItem);
            items.push_back(item);
        }
    }

    void VerifyAttribute(IDataObject* dataObject, SFGAOF sfgaofMask) const
    {
        const CfShellIdList cfshellidlist(dataObject);

        VerifyAttribute(cfshellidlist, sfgaofMask);
    }

    void VerifyAttribute(const CfShellIdList& cfshellidlist, SFGAOF sfgaofMask) const
    {
        if (!HasAttributesOf(cfshellidlist, sfgaofMask))
        {
            ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellFolderImpl::VerifyAttribute failure\n");
            RaiseException(E_INVALIDARG);
        }
    }

    bool HasAttributesOf(const CfShellIdList& cfshellidlist, SFGAOF sfgaofMask) const
    {
        const size_t itemCount = cfshellidlist.GetItemCount();

        SFGAOF sfgaof = static_cast<const T*>(this)->GetAttributesOfGlobal(static_cast<uint32_t>(itemCount), sfgaofMask);
        if (sfgaof == SFGAO_UNDEFINED)
        {
            sfgaof = 0xFFFFFFFF;

            for (size_t i = 0; i < itemCount; ++i)
            {
                PCUIDLIST_RELATIVE childItem = cfshellidlist.GetItem(i);
                TItem item(childItem);
                sfgaof &= static_cast<const T*>(this)->GetAttributeOf(static_cast<uint32_t>(itemCount), item, sfgaofMask);

                if (!IsBitSet(sfgaof, sfgaofMask))
                    return false; // no need to continue the search.
            }

            return true;
        }

        return IsBitSet(sfgaof, sfgaofMask);
    }

    void ReportAddItem(PCUIDLIST_RELATIVE item) const
    {
        ChangeNotifyPidl(SHCNE_CREATE, SHCNF_FLUSH, ItemIDList(m_pidlFolder, item));
    }

    void ReportChangeNotify(const std::vector<TItem>& items, long eventId, uint32_t uFlags = SHCNF_FLUSH) const
    {
        for (auto item : items)
        {
            ChangeNotifyPidl(eventId, uFlags, ItemIDList(m_pidlFolder, item.GetItemIdList()));
        }
    }

    void ReportChangeNotify(const CfShellIdList& cfshellidlist, long eventId, uint32_t flags = SHCNF_FLUSH) const
    {
        for (size_t i = 0; i < cfshellidlist.GetItemCount(); ++i)
        {
            const PCUIDLIST_RELATIVE childItem = cfshellidlist.GetItem(i);

            ChangeNotifyPidl(eventId, flags, ItemIDList(m_pidlFolder.GetAbsolute(), childItem));
        }
    }

    void ReportUpdateItemChangeNotify(IDataObject* dataObject) const
    {
        CfShellIdList cfshellidlist(dataObject);

        for (size_t i = 0; i < cfshellidlist.GetItemCount(); ++i)
        {
            const PCUIDLIST_RELATIVE childItem = cfshellidlist.GetItem(i);

            ChangeNotifyPidl(SHCNE_ATTRIBUTES, SHCNF_FLUSH, ItemIDList{m_pidlFolder, childItem});
        }
    }

    void ReportRenameChangeNotify(const CfShellIdList& cfshellidlist, const std::vector<TItem>& itemsNew) const
    {
        for (size_t i = 0; i < cfshellidlist.GetItemCount(); ++i)
        {
            const PCUIDLIST_RELATIVE pidlOld = cfshellidlist.GetItem(i);

            ChangeNotifyPidl(SHCNE_RENAMEITEM, SHCNF_FLUSH,
                ItemIDList(m_pidlFolder, pidlOld), ItemIDList(m_pidlFolder, itemsNew[i].GetItemIdList()));
        }
    }

    // Note: if hwndOwner is NULL, errors should only be returned as COM failures.
    HWND GetHwndOwner() const noexcept
    {
        return m_ownerWindow;
    }

    IShellBrowserPtr GetShellBrowser() const
    {
        const auto shellBrowser = reinterpret_cast<IShellBrowser*>(SendMessage(m_ownerWindow, WM_GETISHELLBROWSER, 0, 0));
        RaiseExceptionIf(shellBrowser == nullptr);
        return shellBrowser;
    }

    // Update these members if dynamic behavior is required.
    ULONG m_sort;    // initial column used for sorting.
    ULONG m_display; // column that is used when item is displayed in tree view

private:
    HRESULT HandleException(HWND window, ErrorContext errorContext)
    {
        HRESULT result;

        try
        {
            throw;
        }
        catch (const _com_error& e)
        {
            result = e.Error();
        }
        catch (const std::bad_alloc&)
        {
            result = E_OUTOFMEMORY;
        }
        catch (const std::exception&)
        {
            result = E_UNEXPECTED;
        }

        return OnErrorHandler(result, window, errorContext);
    }

    ATL::CString GetExplorerPaneName(_In_ REFEXPLORERPANE explorerPane)
    {
        if (explorerPane == __uuidof(EP_NavPane))
            return L"EP_NavPane";

        if (explorerPane == __uuidof(EP_PreviewPane))
            return L"EP_PreviewPane";

        if (explorerPane == __uuidof(EP_DetailsPane))
            return L"EP_DetailsPane";

        if (explorerPane == __uuidof(EP_Ribbon))
            return L"EP_Ribbon";

        OleString epId;
        StringFromCLSID(explorerPane, epId);
        return ATL::CString(epId);
    }

    ItemIDList m_pidlFolder;
    std::vector<ColumnInfo> m_columnInfos;
    HWND m_ownerWindow{};
    bool m_cachedIsSupportedClipboardFormat{};
};

} // namespace msf
