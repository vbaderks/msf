//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "sfvmdefines.h"
#include "shelluuids.h"
#include "pidl.h"


struct SFVM_WEBVIEW_CONTENT_DATA;


namespace MSF
{

template <typename T>
class ATL_NO_VTABLE IShellFolderViewCBImpl :
    public IShellFolderViewCB,
    public IFolderViewSettings
{
public:
    // IShellFolderViewCB
    STDMETHOD(MessageSFVCB)(UINT uMsg, WPARAM wParam, LPARAM lParam) override
    {
        try
        {
            switch (uMsg)
            {
            case SFVM_MERGEMENU:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnMergeMenu)\n");
                return static_cast<T*>(this)->OnMergeMenu(reinterpret_cast<QCMINFO*>(lParam));

            case SFVM_INVOKECOMMAND:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnInvokeCommand, idCmd = %d)\n", wParam);
                return static_cast<T*>(this)->OnInvokeCommand(static_cast<unsigned int>(wParam));

            case SFVM_GETHELPTEXT:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnGetHelpText, idCmd = %d)\n", LOWORD(wParam));
                return static_cast<T*>(this)->OnGetHelpText(LOWORD(wParam), HIWORD(wParam), reinterpret_cast<LPTSTR>(lParam));

            case SFVM_GETTOOLTIPTEXT:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnGetToolTipText)\n");
                return static_cast<T*>(this)->OnGetToolTipText(LOWORD(wParam), HIWORD(wParam), reinterpret_cast<LPTSTR>(lParam));

            case SFVM_GETBUTTONINFO:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnGetButtonInfo)\n");
                return static_cast<T*>(this)->OnGetButtonInfo(reinterpret_cast<TBINFO*>(lParam));

            case SFVM_GETBUTTONS:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnGetButtons)\n");
                return static_cast<T*>(this)->OnGetButtons(LOWORD(wParam), HIWORD(wParam), reinterpret_cast<TBBUTTON*>(lParam));

            case SFVM_INITMENUPOPUP:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnInitMenuPopup)\n");
                return static_cast<T*>(this)->OnInitMenuPopup(LOWORD(wParam), HIWORD(wParam), reinterpret_cast<HMENU>(lParam));

            case SFVM_SELECTIONCHANGED: // Undocumented
                // Notification when the selection state of an item is changed (i.e. an item is selected or deselected)
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCB::MessageSFVCB (OnSelectionChanged)\n");
                break;

            case SFVM_GETNAMELENGTH: // Undocumented
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnGetNameLength)\n");
                break;

            case SFVM_EXITMENULOOP: // Undocumented
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnExitMenuLoop)\n");
                break;

            case SFVM_FSNOTIFY:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnFSNotify)\n");
                return static_cast<T*>(this)->OnFSNotify(reinterpret_cast<LPCITEMIDLIST>(wParam), static_cast<DWORD>(lParam));

            case SFVM_WINDOWCREATED:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnWindowCreated hwnd=%d)\n", wParam);
                return static_cast<T*>(this)->OnWindowCreated(reinterpret_cast<HWND>(wParam));

            case SFVM_GETDETAILSOF:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnGetDetailsOf, iColumn = %d)\n", wParam);
                return static_cast<T*>(this)->OnGetDetailsOf(static_cast<int>(wParam), reinterpret_cast<DETAILSINFO*>(lParam));

            case SFVM_COLUMNCLICK:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnColumnClick, column=%d)\n", wParam);
                return static_cast<T*>(this)->OnColumnClick(static_cast<unsigned int>(wParam));

            case SFVM_QUERYFSNOTIFY:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnQueryFSNotify)\n");
                return static_cast<T*>(this)->OnQueryFSNotify(reinterpret_cast<SHChangeNotifyEntry*>(lParam));

            case SFVM_DEFITEMCOUNT:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnDefItemCount)\n");
                return static_cast<T*>(this)->OnDefItemCount(reinterpret_cast<UINT*>(lParam));

            case SFVM_DEFVIEWMODE:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnDefViewMode)\n");
                return static_cast<T*>(this)->OnDefViewMode(reinterpret_cast<FOLDERVIEWMODE*>(lParam));

            case SFVM_UNMERGEMENU:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnUnmergeMenu)\n");
                return static_cast<T*>(this)->OnUnmergeMenu(reinterpret_cast<HMENU>(lParam));

            case SFVM_UPDATESTATUSBAR:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnUpdateStatusbar)\n");
                return static_cast<T*>(this)->OnUpdateStatusbar(static_cast<BOOL>(wParam));

            case SFVM_BACKGROUNDENUM:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnBackgroundEnum)\n");
                return static_cast<T*>(this)->OnBackGroundEnum();

            case SFVM_DIDDRAGDROP:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnDidDragDrop)\n");
                return static_cast<T*>(this)->OnDidDragDrop(static_cast<DWORD>(wParam), reinterpret_cast<IDataObject*>(lParam));

            case SFVM_SETISFV:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnSetInterfaceSiteFolderView, lparam=%p)\n", lParam);
                return static_cast<T*>(this)->OnSetInterfaceSiteFolderView(reinterpret_cast<IUnknown*>(lParam));

            case SFVM_THISIDLIST:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnThisIDList)\n");
                return static_cast<T*>(this)->OnThisIDList(reinterpret_cast<LPITEMIDLIST>(lParam));

            case SFVM_ADDPROPERTYPAGES:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnAddPropertyPages)\n");
                return static_cast<T*>(this)->OnAddPropertyPages(reinterpret_cast<SFVM_PROPPAGE_DATA*>(lParam));

            case SFVM_BACKGROUNDENUMDONE:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnBackgroundEnumDone)\n");
                return static_cast<T*>(this)->OnBackgroundEnumDone();

            case SFVM_GETNOTIFY:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnGetNotify)\n");
                return static_cast<T*>(this)->OnGetNotify(reinterpret_cast<LPITEMIDLIST*>(wParam),
                                                          reinterpret_cast<long*>(lParam));

            case SFVM_GETSORTDEFAULTS:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnGetSortDefaults)\n");
                return static_cast<T*>(this)->OnGetSortDefaults(reinterpret_cast<int*>(wParam), reinterpret_cast<int*>(lParam));

            case SFVM_SIZE: // Notification that the folder view window has been resized.
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnSize)\n");
                return static_cast<T*>(this)->OnSize();

            case SFVM_GETZONE:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnGetZone)\n");
                return static_cast<T*>(this)->OnGetZone(reinterpret_cast<DWORD*>(lParam));

            case SFVM_GETPANE:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnGetPane, paneid=%d)\n", wParam);
                return static_cast<T*>(this)->OnGetPane(static_cast<int>(wParam), reinterpret_cast<DWORD*>(lParam));

            case SFVM_GETHELPTOPIC:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnGetHelpTopic)\n");
                return static_cast<T*>(this)->OnGetHelpTopic(reinterpret_cast<SFVM_HELPTOPIC_DATA*>(lParam));

            case SFVM_UNDOCUMENTED56: // Undocumented
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnUndocumented56)\n");
                break;

            case SFVM_UNDOCUMENTED60: // Undocumented
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnUndocumented60)\n");
                break;

            case SFVM_GETPERSISTHISTORY: // Undocumented
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnGetPersistHistory)\n");
                break;

            case SFVM_GETANIMATION: // Documented, but not (always) defined in shlobj.h
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnGetAnimation)\n");
                return static_cast<T*>(this)->OnGetAnimation(reinterpret_cast<HINSTANCE*>(wParam), reinterpret_cast<WCHAR*>(lParam));

            case SFVM_GET_VIEW_DATA: // Undocumented
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnGetViewData)\n");
                return static_cast<T*>(this)->OnGetViewData(static_cast<FOLDERVIEWMODE>(wParam), reinterpret_cast<BOOL*>(lParam));

            case SFVM_WNDMAIN:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnWndMain)\n");
                return static_cast<T*>(this)->OnWndMain(reinterpret_cast<HWND>(lParam));

            case SFVM_ENUMERATEDITEMS:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnEnumeratedItems)\n");
                break;

            case SFVM_GETCOLUMNSTREAM:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnGetColumnsStream)\n");
                break;

            case SFVM_WINDOWCLOSING:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnWindowClosing)\n");
                break;

            case SFVM_ADDINGOBJECT:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnAddingObject)\n");
                break;

            case SFVM_REMOVINGOBJECT:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnRemovingObject)\n");
                break;

            case SFVM_GET_CUSTOMVIEWINFO:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnGetCustomViewInfo)\n");
                break;

            case SFVM_WINDOWFOCUSED:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnWindowFocused)\n");
                break;

            case SFVM_LISTREFRESHED:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnListRefreshed)\n");
                break;

            case SFVM_VIEWRELEASE:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnViewRelease, wparam=%d, lparam=%d)\n", wParam, lParam);
                break;

            case SFVM_SORT:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnSort, wparam=%d, lparam=%d)\n", wParam, lParam);
                break;

            case SFVM_CREATEVIEWWND2:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnCreateViewWnd2, wparam=%d, lparam=%d)\n", wParam, lParam);
                break;

            case SFVM_WNDPROC:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnWndProc)\n");
                break;

            case SFVM_SHOWWEBVIEW:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnShowWebView)\n");
                break;

            case SFVM_UNDOCUMENTED78:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnUndocumented78)\n");
                break;

            case SFVM_GET_WEBVIEW_LAYOUT:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnGetWebviewLayout)\n");
                break;

            case SFVM_GET_WEBVIEW_CONTENT:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnGetWebviewContent)\n");
                return static_cast<T*>(this)->OnGetWebViewContent(reinterpret_cast<SFVM_WEBVIEW_CONTENT_DATA*>(lParam));

            case SFVM_GET_WEBVIEW_TASKS:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnGetWebviewTasks)\n");
                break;

            case SFVM_GET_WEBVIEW_THEME:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnGetWebviewTheme)\n");
                break;

            case SFVM_GETEXTVIEWS:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnGetExtViews)\n");
                break;

            case SFVM_FINDITEM: // Undocumented
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnFindItem, wParam=%p, lParam=%p)\n", wParam, lParam);
                return static_cast<T*>(this)->OnFindItem(wParam, reinterpret_cast<LPITEMIDLIST>(lParam));

            case SFVM_GET_WEBVIEW_TEMPLATE:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnGetWebViewTemplate)\n");
                break;

            case SFVM_GET_VIEWINFO_TEMPLATE:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnGetViewInfoTemplate)\n");
                break;

            case SFVM_ISSTRICTREFRESH:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnIsStrictRefresh)\n");
                break;

            case SFVM_UNDOCUMENTED51:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnUndocumented51)\n");
                break;

            case SFVM_GETCOMMANDDIR: // Undocumented
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnCommandDir)\n");
                break;

            case SFVM_CANSELECTALL:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnCanSelectAll)\n");
                break;

            case SFVM_GET_FOLDER_TYPE_DESCRIPTION: // Undocumented, seen on Windows 8
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnGetFolderTypeDescription)\n");
                break;

            case SFVM_POST_CREATE_VIEW_WINDOW3: // Undocumented, seen on Windows 8
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnPostCreateViewWindow3)\n");
                break;

            case SFVM_PRE_CREATE_VIEW_WINDOW3: // Undocumented, seen on Windows 8
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnPreCreateViewWindow3)\n");
                break;

            case SFVM_GET_EMPTY_TEXT: // Undocumented, seen on Windows 8
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnGetEmptyText)\n");
                break;

            case SFVM_GET_MAX_ITEM_COUNT: // Undocumented, seen on Windows 8
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnGetMaxItemCount)\n");
                break;

            case SFVM_CANCEL_PENDING_NAV: // Undocumented, seen on Windows 8
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnCancelPendingNav)\n");
                break;

            case SFVM_REGISTER_FOR_CHANGES: // Undocumented, seen on Windows 8
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (OnRegisterForChanges)\n");
                break;

            default:
                ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCB::MessageSFVCB (uMsg=%d (undocumented))\n", uMsg);
                break;
            }

            return E_NOTIMPL;
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    // IFolderViewSettings
    STDMETHOD(GetColumnPropertyList)(__RPC__in REFIID /*riid*/, __RPC__deref_out_opt void ** /*ppv*/) override
    {
        ATLTRACENOTIMPL(L"IShellFolderViewCBImpl::IFolderViewSettings::GetColumnPropertyList");
    }

    STDMETHOD(GetGroupByProperty)(__RPC__out PROPERTYKEY * /*pkey*/, __RPC__out BOOL * /*pfGroupAscending*/) override
    {
        ATLTRACENOTIMPL(L"IShellFolderViewCBImpl::IFolderViewSettings::GetGroupByProperty");
    }

    STDMETHOD(GetViewMode)(__RPC__out FOLDERLOGICALVIEWMODE *plvm) override
    {
        ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IFolderViewSettings::GetViewMode (instance=%p, plvm=%p)\n", this, plvm);
        *plvm = FLVM_DETAILS;
        return S_OK;
    }

    STDMETHOD(GetIconSize)(__RPC__out UINT * /*puIconSize*/) override
    {
        ATLTRACENOTIMPL(L"IShellFolderViewCBImpl::IFolderViewSettings::GetIconSize");
    }

    STDMETHOD(GetFolderFlags)(__RPC__out FOLDERFLAGS * /*pfolderMask*/, __RPC__out FOLDERFLAGS * /*pfolderFlags*/) override
    {
        ATLTRACENOTIMPL(L"IShellFolderViewCBImpl::IFolderViewSettings::GetFolderFlags");
    }

    STDMETHOD(GetSortColumns)(__RPC__out_ecount_part(cColumnsIn, *pcColumnsOut) SORTCOLUMN * rgSortColumns, UINT cColumnsIn, __RPC__out UINT * pcColumnsOut) override
    {
        UNREFERENCED_PARAMETER(rgSortColumns);
        UNREFERENCED_PARAMETER(cColumnsIn);
        UNREFERENCED_PARAMETER(pcColumnsOut);
        ATLTRACENOTIMPL(L"IShellFolderViewCBImpl::IFolderViewSettings::GetSortColumns");
    }

    STDMETHOD(GetGroupSubsetCount)(__RPC__out UINT * /*pcVisibleRows*/) override
    {
        ATLTRACENOTIMPL(L"IShellFolderViewCBImpl::IFolderViewSettings::GetGroupSubsetCount");
    }

protected:
    explicit IShellFolderViewCBImpl(long notifyevents = 0) :
        _notifyevents(notifyevents)
    {
        ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::IShellFolderViewCBImpl (instance=%p)\n", this);
    }

    ~IShellFolderViewCBImpl()
    {
        ATLTRACE2(atlTraceCOM, 0, L"IShellFolderViewCBImpl::~IShellFolderViewCBImpl (instance=%p)\n", this);
    }

    // Purpose: Controls which folder (file) is watched for change events.
    void SetFolder(LPCITEMIDLIST pidlFolder)
    {
        _pidlFolder.CloneFrom(pidlFolder);
    }

    // Default callback handlers: override in derived class if needed.

    HRESULT OnMergeMenu(QCMINFO* /*pInfo*/)
    {
        return E_NOTIMPL;
    }

    HRESULT OnInvokeCommand(unsigned int /*idCmd*/)
    {
        return E_NOTIMPL;
    }

    HRESULT OnGetHelpText(unsigned short /* idCmd */, unsigned short /* cchMax */, LPTSTR /* pszText */)
    {
        return E_NOTIMPL;
    }

    HRESULT OnGetToolTipText(unsigned short /* idCmd */, unsigned short /* cchMax */, LPTSTR /* pszText */)
    {
        return E_NOTIMPL;
    }

    // Purpose: Notify the system shell view that we have buttons that need to be added
    //          to one of the toolbars.
    //          This notification is documented, but some critical info is missing.
    //          - if this notification is handled, function must return S_OK;
    //          - TBINFO flags also allow to specify which toolbar (see shlobj.h)
    HRESULT OnGetButtonInfo(TBINFO* /* ptbinfo */)
    {
        return E_NOTIMPL;
    }

    HRESULT OnGetButtons(unsigned short /*idCmdFirst*/, unsigned short /*cbtnMax*/, TBBUTTON* /*ptbbutton*/)
    {
        return E_NOTIMPL;
    }

    HRESULT OnInitMenuPopup(unsigned short /* idCmd */, unsigned short /* nIndex */, HMENU /*hemu*/)
    {
        return E_NOTIMPL;
    }

    HRESULT OnFSNotify(LPCITEMIDLIST /*pidl*/, DWORD /*lEvent*/)
    {
        return E_NOTIMPL;
    }

    HRESULT OnAddPropertyPages(SFVM_PROPPAGE_DATA* /*pData*/)
    {
        return E_NOTIMPL;
    }

    HRESULT OnBackgroundEnumDone()
    {
        return E_NOTIMPL;
    }

    HRESULT OnGetSortDefaults(int* /*piDirection*/, int* /*piColumn*/)
    {
        return E_NOTIMPL;
    }

    HRESULT OnGetDetailsOf(int /*iColumn*/, DETAILSINFO*)
    {
        return E_NOTIMPL;
    }

    HRESULT OnDefItemCount(UINT* /*pcItems*/)
    {
        return E_NOTIMPL;
    }

    HRESULT OnGetZone(DWORD* /*pwdZone*/)
    {
        return E_NOTIMPL;
    }

    HRESULT OnSize()
    {
        return E_NOTIMPL;
    }

    HRESULT OnBackGroundEnum()
    {
        return E_NOTIMPL;
    }

    HRESULT OnDidDragDrop(DWORD /*dwEffect*/, IDataObject* /*pIdo*/)
    {
        return E_NOTIMPL;
    }

    HRESULT OnGetAnimation(HINSTANCE* /*phinst*/, WCHAR* /*pwszName*/)
    {
        return E_NOTIMPL;
    }

    // Purpose: return S_OK and TRUE/FALSE to indicate to the defshellview which 
    //          folder view modes should be enabled (only works on XP?)
    HRESULT OnGetViewData(FOLDERVIEWMODE /*folderviewmode*/, BOOL* /*pbSupported*/)
    {
        return E_NOTIMPL;
    }

    // Purpose: save this hwnd to use the SHShellFolderView_Message function.
    HRESULT OnWndMain(HWND /*hwndMain*/)
    {
        return S_OK;
    }

    HRESULT OnDefViewMode(FOLDERVIEWMODE* /*pfolderviewmode*/)
    {
        return E_NOTIMPL;
    }

    HRESULT OnUpdateStatusbar(BOOL /*fInitialize*/)
    {
        return E_NOTIMPL;
    }

    HRESULT OnUnmergeMenu(HMENU /*hmenuCurrent*/)
    {
        return E_NOTIMPL;
    }

    HRESULT OnThisIDList(LPITEMIDLIST /*pidl*/)
    {
        return E_NOTIMPL;
    }

    HRESULT OnSetInterfaceSiteFolderView(IUnknown* /*site*/)
    {
        return S_OK;
    }

    // Purpose: called when the 'SHELLDLL_DefView' window is created.
    //          This window is the parent of the 'syslistview32'.
    HRESULT OnWindowCreated(HWND /*hwnd*/)
    {
        return S_OK;
    }

    HRESULT OnColumnClick(UINT /*uiColumn*/)
    {
        return S_FALSE;
    }

    // Purpose: indicates in which statusbar pane to display internet zone info
    HRESULT OnGetPane(int /*paneID*/, DWORD* /*pdwpane*/)
    {
        return E_NOTIMPL;
    }

    HRESULT OnGetHelpTopic(SFVM_HELPTOPIC_DATA* /*phtd*/)
    {
        return E_NOTIMPL;
    }

    HRESULT OnQueryFSNotify(SHChangeNotifyEntry* /*shcne*/)
    {
        return E_NOTIMPL;
    }

    HRESULT OnFindItem(WPARAM /*wParam*/, LPITEMIDLIST /*ppidlToFind*/)
    {
        // Note: wParam seems to be int* pItemIndex.
        return E_NOTIMPL;
    }

    HRESULT OnGetWebViewContent(SFVM_WEBVIEW_CONTENT_DATA* /*pcontentdata*/)
    {
        return E_NOTIMPL;
    }

    // Purpose: called by the shells default viewfolder implementation to retrieve
    //          the settings for SHChangeNotifyRegister.
    HRESULT OnGetNotify(LPITEMIDLIST* ppidl, long* plevents)
    {
        if (!_pidlFolder || _notifyevents == 0)
            return E_FAIL; // notify not requested by derived class.

        *ppidl    = ILClone(_pidlFolder);
        *plevents = _notifyevents;

        return S_OK;
    }

    // Member variables.
    CPidl _pidlFolder;
    long  _notifyevents;
};

} // MSF namespace
