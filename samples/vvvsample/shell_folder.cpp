﻿//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software license.
//
#include "pch.h"

#include "enum_id_list.h"
#include "resource.h"
#include "shell_folder_class_id.h"
#include "shell_folder_data_object.h"
#include "shell_folder_view_cb.h"
#include "vvv_file.h"
#include "vvv_item.h"
#include "vvv_property_sheet.h"

using std::make_unique;
using std::wstring;

// Defines for the item context menu.
constexpr uint32_t ID_DFM_CMD_OPEN = 0;

class __declspec(novtable) ShellFolder :
    public ATL::CComObjectRootEx<ATL::CComSingleThreadModel>,
    public ATL::CComCoClass<ShellFolder, &__uuidof(ShellFolder)>,
    public msf::ShellFolderImpl<ShellFolder, VVVItem>,
    public msf::IBrowserFrameOptionsImpl,
    public msf::IItemNameLimitsImpl<ShellFolder, VVVItem>
{
public:
    BEGIN_COM_MAP(ShellFolder)
        COM_INTERFACE_ENTRY2(IPersist, IPersistFolder) // Needed as IPersistFolder and IPersistIDList derive both from IPersist.
        COM_INTERFACE_ENTRY(IPersistFolder)
        COM_INTERFACE_ENTRY(IPersistFolder2)
        COM_INTERFACE_ENTRY(IPersistFolder3)
        COM_INTERFACE_ENTRY(IPersistIDList)
        COM_INTERFACE_ENTRY(IShellFolder) // included in this sample for backwards (win9x) compatibility.
        COM_INTERFACE_ENTRY(IShellFolder2)
        COM_INTERFACE_ENTRY(IShellDetails) // included in this sample for backwards (win9x) compatibility.
        COM_INTERFACE_ENTRY(IBrowserFrameOptions)
        COM_INTERFACE_ENTRY(IShellIcon)
        COM_INTERFACE_ENTRY(IItemNameLimits)
        COM_INTERFACE_ENTRY(IDropTarget)               // enable drag and drop support.
        COM_INTERFACE_ENTRY(IObjectWithFolderEnumMode) // used by Windows 7 and up
        COM_INTERFACE_ENTRY(IExplorerPaneVisibility)   // used by Windows Vista and up.
    END_COM_MAP()

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    static HRESULT __stdcall UpdateRegistry(BOOL bRegister) noexcept
    {
        return ShellFolderImpl<ShellFolder, VVVItem>::UpdateRegistry(
            bRegister, IDR_SHELLFOLDER,
            L"VVV Sample ShellFolder ShellExtension ", wszVVVFileRootExt, IDS_SHELLFOLDER_TYPE);
    }

    // Purpose: called by msf when the shell folder needs to show a sub folder.
    void InitializeSubFolder(const std::vector<VVVItem>& items)
    {
        m_strSubFolder.clear();

        for (const auto& item : items)
        {
            if (!m_strSubFolder.empty())
            {
                m_strSubFolder += L"\\";
            }

            m_strSubFolder += std::to_wstring(item.GetID());
        }
    }

    // Purpose: Create the shellfolderviewcb that will be used to catch callback events
    //          generated by the system folder view.
    [[nodiscard]] ATL::CComPtr<IShellFolderViewCB> CreateShellFolderViewCB() const
    {
        return ShellFolderViewCB::CreateInstance(GetRootFolder());
    }

    // Purpose: called by msf/shell when a number of items are selected and a IDataObject
    //          that contains the items is required.
    ATL::CComPtr<IDataObject> CreateDataObject(PCIDLIST_ABSOLUTE pidlFolder, uint32_t cidl, PCUITEMID_CHILD_ARRAY ppidl)
    {
        return ShellFolderDataObject::CreateInstance(pidlFolder, cidl, ppidl, this);
    }

    // Purpose: called by msf/shell when it want the current list of
    //          all items  The shell will walk all IDs and then release the enum.
    ATL::CComPtr<IEnumIDList> CreateEnumIDList(HWND /*hwnd*/, DWORD grfFlags) const
    {
        return EnumIDList::CreateInstance(GetPathJunctionPoint(), m_strSubFolder, grfFlags);
    }

    // Purpose: called by msf when there is no global settings for all items.
    [[nodiscard]] SFGAOF GetAttributeOf(unsigned int cidl, const VVVItem& item, SFGAOF /*sfgofMask*/) const
    {
        return item.GetAttributeOf(cidl == 1, IsReadOnly(GetPathJunctionPoint()));
    }

    // Purpose: called by msf to tell the shell which panes to show.
    // It is essential to override this function to control which explorer panes are visible as by default no panes are shown.
    static EXPLORERPANESTATE GetPaneState(_In_ REFEXPLORERPANE ep) noexcept
    {
        if (ep == EP_Ribbon)
            return EPS_DEFAULT_ON;

        return EPS_DONTCARE;
    }

    // Purpose: called by the default context menu. Gives an option to merge
    //          extra commands into the menu.
    static HRESULT OnDfmMergeContextMenu(IDataObject* dataObject, uint32_t /*uFlags*/, QCMINFO& mergeInfo)
    {
        const msf::CfShellIdList itemList(dataObject);

        if (itemList.size() == 1 && !VVVItem(itemList.GetItem(0)).IsFolder())
        {
            // Add 'open' if only 1 item is selected.
            const msf::CMenu menu(true);
            menu.AddDefaultItem(ID_DFM_CMD_OPEN, L"&Open");
            MergeMenus(mergeInfo, menu);

            // Note: XP will automatic make first menu item the default.
            //       Win98, ME and 2k don't do this, so must add as default item.
        }

        return S_OK;
    }

    // Purpose: Called to get the help string for added menu items.
    static wstring OnDfmGetHelpText(unsigned short nCmdId)
    {
        return msf::LoadResourceString(IDS_SHELLFOLDER_DFM_HELP_BASE + nCmdId);
    }

    HRESULT OnDfmInvokeAddedCommand(HWND hwnd, IDataObject* dataObject, int nId) const
    {
        switch (nId)
        {
        case ID_DFM_CMD_OPEN:
            OnOpen(hwnd, dataObject);
            break;

        default:
            ATLASSERT(false); // unknown command id detected.
            break;
        }

        return S_OK;
    }

    // Purpose: handle 'open' by showing the name of the selected item.
    void OnOpen(HWND hwnd, IDataObject* dataObject) const
    {
        msf::CfShellIdList cfshellidlist(dataObject);
        ATLASSERT(cfshellidlist.size() == 1);

        const VVVItem item(cfshellidlist.GetItem(0));

        if (item.IsFolder())
        {
            GetShellBrowser().BrowseObject(item.GetItemIdList(),
                                           SBSP_DEFBROWSER | SBSP_RELATIVE);
        }
        else
        {
            const auto message = L"Open on: " + item.GetName();
            IsolationAwareMessageBox(hwnd, message.c_str(), L"Open", MB_OK | MB_ICONQUESTION);
        }
    }

    // Purpose: Called by the shell/msf when an item must be renamed.
    PUIDLIST_RELATIVE OnSetNameOf(HWND /*hwnd*/, const VVVItem& item, const wchar_t* szNewName, SHGDNF shgndf) const
    {
        msf::RaiseExceptionIf(shgndf != SHGDN_NORMAL && shgndf != SHGDN_INFOLDER); // not supported 'name'.

        msf::ItemIDList pidl(VVVItem::CreateItemIdList(item.GetID(), item.GetSize(), item.IsFolder(), szNewName));

        VVVFile(GetPathJunctionPoint(), m_strSubFolder).SetItem(VVVItem(pidl.GetRelative()));

        return pidl.DetachRelative();
    }

    // Purpose: handles the 'properties request.
    //          The property sheet/page allows the user to change
    //          the name and size of an item.
    long OnProperties(HWND hwnd, std::vector<VVVItem>& items)
    {
        ATLASSERT(items.size() == 1);
        auto& item = items[0];

        long wEventId;
        if (VVVPropertySheet(item, this).DoModal(hwnd, wEventId) > 0 && wEventId != 0)
        {
            const VVVFile vvvFile(GetPathJunctionPoint(), m_strSubFolder);
            vvvFile.SetItem(item);
        }

        return wEventId;
    }

    // Purpose: Called by msf/shell when items must be deleted.
    long OnDelete(HWND hwnd, const std::vector<VVVItem>& items) const
    {
        if (!hwnd && !UserConfirmsFileDelete(hwnd, items))
            return 0; // user wants to abort the file deletion process.

        VVVFile(GetPathJunctionPoint(), m_strSubFolder).DeleteItems(items);

        return SHCNE_DELETE;
    }

    // Purpose: called by the standard msf drag handler during drag operations.
    static bool IsSupportedClipboardFormat(IDataObject* dataObject)
    {
        return msf::ClipboardFormatHDrop::IsFormat(dataObject);
    }

    // Purpose: called when items are pasted or dropped on the shellfolder.
    DWORD AddItemsFromDataObject(DWORD effectMask, IDataObject* dataObject) const
    {
        msf::ClipboardFormatHDrop clipboardFormat(dataObject);

        const auto fileCount = clipboardFormat.GetFileCount();
        for (unsigned int i = 0; i < fileCount; ++i)
        {
            AddItem(clipboardFormat.GetFile(i));
        }

        // The VVV sample cannot use optimized move. Just return effectMask as passed.
        return effectMask;
    }

    static void OnError(HRESULT hr, HWND hwnd, ErrorContext /*errorContext*/) noexcept
    {
        try
        {
            auto message = msf::LoadResourceString(IDS_SHELLFOLDER_CANNOT_PERFORM) + msf::FormatLastError(static_cast<DWORD>(hr));
            IsolationAwareMessageBox(hwnd, message.c_str(),
                                     msf::LoadResourceString(IDS_SHELLEXT_ERROR_CAPTION).c_str(), MB_OK | MB_ICONERROR);
        }
        catch (...)
        {
            // Suppress all exceptions (should be none), to prevent that an exception escapes from this noexcept function.
            ATLASSERT(false);
        }
    }

protected:
    ShellFolder() noexcept
    {
        // Register the columns the folder supports in 'detailed' mode.
        RegisterColumn(IDS_SHELLEXT_NAME, LVCFMT_LEFT);
        RegisterColumn(IDS_SHELLEXT_SIZE, LVCFMT_RIGHT);
    }

private:
    // Purpose: Ask the user if he is really sure about the file delete action.
    //          Deleted files cannot be restored from the recycle bin.
    static bool UserConfirmsFileDelete(HWND hwnd, const std::vector<VVVItem>& items)
    {
        ATL::CString strMessage;
        uint32_t nCaptionResId;

        if (items.size() == 1)
        {
            strMessage.FormatMessage(IDS_SHELLFOLDER_DELETE, items[0].GetDisplayName().c_str());
            nCaptionResId = IDS_SHELLFOLDER_FILE_DELETE_CAPTION;
        }
        else
        {
            strMessage.FormatMessage(IDS_SHELLFOLDER_MULTIPLE_DELETE, std::to_wstring(items.size()).c_str());
            nCaptionResId = IDS_SHELLFOLDER_FILES_DELETE_CAPTION;
        }

        return IsolationAwareMessageBox(hwnd, strMessage,
                                        msf::LoadResourceString(nCaptionResId).c_str(), MB_YESNO | MB_ICONQUESTION) == IDYES;
    }

    void AddItem(const wstring& strFile) const
    {
        VVVFile vvvfile(GetPathJunctionPoint(), m_strSubFolder);

        msf::ItemIDList pidlItem(vvvfile.AddItem(strFile));

        ReportAddItem(pidlItem.GetRelative());
    }

    static bool IsReadOnly(const wstring& strFileName) noexcept
    {
        const auto attributes = GetFileAttributes(strFileName.c_str());
        return (attributes & FILE_ATTRIBUTE_READONLY) != 0;
    }

    // Member variables
    wstring m_strSubFolder;
};


OBJECT_ENTRY_AUTO(__uuidof(ShellFolder), ShellFolder)
