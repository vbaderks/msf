//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//

#include "pch.h"

#include <msf/item_base.h>
#include <msf/shell_folder_impl.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace msf;
using namespace ATL;
using std::wstring;


class TestItem : public msf::ItemBase
{
public:
    static int GetMaxNameLength(PCWSTR /*pszName*/) noexcept
    {
        return 10; // note: limit is 10 for easy testing.
    }

    static PUIDLIST_RELATIVE CreateItemIdList(unsigned int id, unsigned int size, bool folder, const std::wstring& name)
    {
        const PUIDLIST_RELATIVE pidl = msf::ItemIDList::CreateItemIdListWithTerminator(sizeof(SItemData));

        InitializeItemData(reinterpret_cast<SItemData*>(pidl->mkid.abID),
                           id, size, folder, name);

        return pidl;
    }

    explicit TestItem(PCUIDLIST_RELATIVE pidl)
        : ItemBase(pidl)
    {
        // Shell item IDs can be passed from external sources, validate
        // It is valid to pass a PIDL that is larger then the original (done by Search functionality in XP).
        const bool valid = GetDataSize() >= sizeof(SItemData) && GetItemData().nTypeID == TypeID;
#ifdef _DEBUG
        if (!valid)
        {
            ATLTRACE(L"VVVItem::Constructor, PIDL not valid (data_size=%d, s_item_size=%d)\n", GetDataSize(), sizeof(SItemData));
        }
#endif
        msf::RaiseExceptionIf(!valid);
    }

    std::wstring GetDisplayName(SHGDNF shellGetDisplayNameType) const
    {
        switch (shellGetDisplayNameType)
        {
        case SHGDN_NORMAL:
        case SHGDN_INFOLDER:
            break;

        case SHGDN_INFOLDER | SHGDN_FOREDITING:
            break; // override if item has a special edit name

        case SHGDN_INFOLDER | SHGDN_FORADDRESSBAR:
            break; // override if item has a special name for the address bar

        case SHGDN_INFOLDER | SHGDN_FORPARSING:
        case SHGDN_FORPARSING:               // note parent should append folder name before item name.
            return std::to_wstring(GetID()); // return unique string (VVV items are unique by ID)

        default:
            break;
        }

        return GetName();
    }

    SFGAOF GetAttributeOf(bool bSingleSelect, bool bReadOnly) const noexcept;

    unsigned int GetID() const noexcept
    {
        return GetItemData().id;
    }

    unsigned int GetSize() const noexcept
    {
        return GetItemData().size;
    }

    std::wstring GetName() const
    {
        return GetItemData().name;
    }

    bool IsFolder() const noexcept
    {
        return GetItemData().folder;
    }

    int Compare(const TestItem& /*item*/, int /*compareBy*/, bool /*bCanonicalOnly*/) const noexcept
    {
        return 0;
    }

    std::wstring GetItemDetailsOf(uint32_t /*columnIndex*/) const
    {
        return L"";
    }

    std::wstring GetInfoTipText() const
    {
        return L"";
    }

    int GetIconOf(uint32_t /*flags*/) const noexcept
    {
        return 0;
    }

private:
    // By setting and checking for a TypeId (or cookie) we can ensure that the PIDL
    // was created by us. Using a version # will allows to handle older persisted PIDLs
    constexpr static unsigned int TypeID = 0x5601; // 'V' + version #

// By using a struct with a version # it becomes possible to detected old persisted PIDLs.
#pragma pack(1) // By using an explicit pack the memory layout is better fixed then trusting the project settings.
    struct SItemData
    {
        unsigned short nTypeID;
        bool folder;
        unsigned int id;
        unsigned int size;
        wchar_t name[MAX_PATH];
    };
#pragma pack()

    static void InitializeItemData(SItemData* itemData, unsigned int id, unsigned int size, bool folder, const std::wstring& name) noexcept
    {
        itemData->nTypeID = TypeID;
        itemData->id = id;
        itemData->folder = folder;
        itemData->size = size;
        wcscpy_s(itemData->name, _countof(itemData->name), name.c_str());
    }

    const SItemData& GetItemData() const noexcept
    {
        return *static_cast<const SItemData*>(GetData());
    }

    int CompareByName(const TestItem& item) const noexcept;
};


class __declspec(novtable) __declspec(uuid("DAD00CC0-9F8C-42CF-AF83-9F01336D50E4")) ShellFolderTest
    : public ATL::CComObjectRootEx<ATL::CComSingleThreadModel>,
      public ATL::CComCoClass<ShellFolderTest, &__uuidof(ShellFolderTest)>,
      public msf::ShellFolderImpl<ShellFolderTest, TestItem>
{
public:
    BEGIN_COM_MAP(ShellFolderTest)
        COM_INTERFACE_ENTRY2(IPersist, IPersistFolder2)
        COM_INTERFACE_ENTRY(IPersistFolder)
        COM_INTERFACE_ENTRY(IPersistFolder2)
        COM_INTERFACE_ENTRY(IPersistFolder3)
        COM_INTERFACE_ENTRY(IPersistIDList)
        COM_INTERFACE_ENTRY(IShellFolder2)
        COM_INTERFACE_ENTRY(IShellIcon)
        COM_INTERFACE_ENTRY(IDropTarget)               // enable drag and drop support.
        COM_INTERFACE_ENTRY(IObjectWithFolderEnumMode) // used by Windows 7 and up
        COM_INTERFACE_ENTRY(IExplorerPaneVisibility)   // used by Windows Vista and up.
    END_COM_MAP()

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    // Purpose: called by msf when the shell folder needs to show a sub folder.
    void InitializeSubFolder(const std::vector<TestItem>& items)
    {
        m_strSubFolder.clear();

        for (auto item : items)
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
    ATL::CComPtr<IShellFolderViewCB> CreateShellFolderViewCB() const noexcept
    {
        return nullptr;
    }

    // Purpose: called by msf/shell when a number of items are selected and a IDataObject
    //          that contains the items is required.
    ATL::CComPtr<IDataObject> CreateDataObject(PCIDLIST_ABSOLUTE /*pidlFolder*/, uint32_t /*cidl*/, PCUITEMID_CHILD_ARRAY /*ppidl*/) noexcept
    {
        return nullptr;
    }

    // Purpose: called by msf/shell when it want the current list of
    //          all items  The shell will walk all IDs and then release the enum.
    ATL::CComPtr<IEnumIDList> CreateEnumIDList(HWND /*hwnd*/, DWORD /*grfFlags*/) const noexcept
    {
        return nullptr;
    }

    // Purpose: called by msf when there is no global settings for all items.
    SFGAOF GetAttributeOf(unsigned int /*cidl*/, const TestItem& /*item*/, SFGAOF /*sfgofMask*/) const noexcept
    {
        return 0;
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
    static HRESULT OnDfmMergeContextMenu(IDataObject* /*dataObject*/, uint32_t /*uFlags*/, QCMINFO& /*mergeInfo*/) noexcept
    {
        return S_OK;
    }

    // Purpose: Called to get the help string for added menu items.
    static wstring OnDfmGetHelpText(unsigned short /*nCmdId*/)
    {
        return L"";
    }

    HRESULT OnDfmInvokeAddedCommand(HWND /*hwnd*/, IDataObject* /*dataObject*/, int /*nId*/) const noexcept
    {
        return S_OK;
    }

    // Purpose: handle 'open' by showing the name of the selected item.
    void OnOpen(HWND /*hwnd*/, IDataObject* /*dataObject*/) const noexcept
    {
    }

    // Purpose: Called by the shell/msf when an item must be renamed.
    PUIDLIST_RELATIVE OnSetNameOf(HWND /*hwnd*/, const TestItem& /*item*/, const wchar_t* /*szNewName*/, SHGDNF /*shgndf*/) const noexcept
    {
        return nullptr;
    }

    // Purpose: handles the 'properties request.
    //          The property sheet/page allows the user to change
    //          the name and size of an item.
    long OnProperties(HWND /*hwnd*/, std::vector<TestItem>& /*items*/) noexcept
    {
        return 0;
    }

    // Purpose: Called by msf/shell when items must be deleted.
    long OnDelete(HWND /*hwnd*/, const std::vector<TestItem>& /*items*/) const noexcept
    {
        return SHCNE_DELETE;
    }

    // Purpose: called by the standard msf drag handler during drag operations.
    static bool IsSupportedClipboardFormat(IDataObject* dataObject)
    {
        return msf::ClipboardFormatHDrop::IsFormat(dataObject);
    }

    // Purpose: called when items are pasted or dropped on the shellfolder.
    DWORD AddItemsFromDataObject(DWORD dwEffect, IDataObject* dataObject) const
    {
        msf::ClipboardFormatHDrop cfhdrop(dataObject);

        const auto nFiles = cfhdrop.GetFileCount();
        for (unsigned int i = 0; i < nFiles; ++i)
        {
            AddItem(wstring(cfhdrop.GetFile(i)));
        }

        // The VVV sample cannot use optimized move. Just return dwEffect as passed.
        return dwEffect;
    }

    static void OnError(HRESULT /*hr*/, HWND /*hwnd*/, ErrorContext /*errorContext*/) noexcept
    {
    }

protected:
    ShellFolderTest() noexcept
    {
    }

private:
    // Purpose: Ask the user if he is really sure about the file delete action.
    //          Deleted files cannot be restored from the recycle bin.
    static bool UserConfirmsFileDelete(HWND /*hwnd*/, const std::vector<TestItem>& /*items*/) noexcept
    {
    }

    void AddItem(const wstring& /*strFile*/) const noexcept
    {
    }

    static bool IsReadOnly(const wstring& /*strFileName*/) noexcept
    {
        return false;
    }

    // Member variables
    wstring m_strSubFolder;
};

_COM_SMARTPTR_TYPEDEF(IPersistFolder3, __uuidof(IPersistFolder3));

TEST_CLASS(ShellFolderImplTest)
{
public :
    TEST_METHOD(IPersistFolder3_InitializeEx)
    {
        LPUNKNOWN m_pInnerUnk;
        CComCoClass<ShellFolderTest>::CreateInstance(nullptr, &m_pInnerUnk);
        IPersistFolder3Ptr persistPtr(m_pInnerUnk);
        m_pInnerUnk->Release();

        MSF_WARNING_SUPPRESS(6387)
        HRESULT hresult = persistPtr->InitializeEx(nullptr, nullptr, nullptr);
        MSF_WARNING_UNSUPPRESS()

        Assert::AreEqual(E_INVALIDARG, hresult);

        ItemIDList itemIdList(L"C:");
        hresult = persistPtr->InitializeEx(nullptr, itemIdList.GetAbsolute(), nullptr);
        Assert::AreEqual(S_OK, hresult);
    }
};
