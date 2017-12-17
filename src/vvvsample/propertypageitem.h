//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "vvvitem.h"
#include "resource.h"
#include <msf.h>
#include <atlsnap.h>


class PropertyPageItem : public ATL::CSnapInPropertyPageImpl<PropertyPageItem>
{
public:
    static HPROPSHEETPAGE CreateInstance(VVVItem& item, long& wEventId, IShellFolder* shellFolder)
    {
        auto ppage = new PropertyPageItem(item, wEventId, shellFolder);
        return ppage->Create();
    }

    enum { IDD = IDD_PROPERTY_PAGE_ITEM };

    BEGIN_MSG_MAP(PropertyPageItem)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        CHAIN_MSG_MAP(CSnapInPropertyPageImpl<PropertyPageItem>)
    END_MSG_MAP()

    PropertyPageItem(VVVItem& item, long& wEventId, IShellFolder* shellFolder) :
        m_item(item),
        m_wEventId(wEventId),
        m_shellFolder(shellFolder)
    {
    }

    ~PropertyPageItem() = default;
    PropertyPageItem(const PropertyPageItem&) = delete;
    PropertyPageItem(PropertyPageItem&&) = delete;
    PropertyPageItem& operator=(const PropertyPageItem&) = delete;
    PropertyPageItem& operator=(PropertyPageItem&&) = delete;

    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
    {
        InitializeStaticString();
        InitializeControls();
        return 1;
    }

    BOOL OnApply()
    {
        m_wEventId = 0;

        ATL::CString strName;
        GetDlgItemText(IDC_EDIT_ITEM_NAME, strName);
        strName.Trim();
        const std::wstring name(strName); // TODO

        if (m_item.GetDisplayName() != name)
        {
            m_wEventId |= SHCNE_RENAMEITEM;
        }

        const unsigned int nSize = GetDlgItemInt(IDC_EDIT_ITEM_SIZE, nullptr, false);
        if (m_item.GetSize() != nSize)
        {
            m_wEventId |= SHCNE_ATTRIBUTES;
        }

        if (m_wEventId != 0)
        {
            m_pidlNew.Attach(VVVItem::CreateItemIdList(m_item.GetID(),
                nSize, m_item.IsFolder(), strName));
        }

        return true;
    }

private:
    // It is easier to patch the dialog during runtime with the locale
    // strings then to maintain copies of the dialog resource.
    void InitializeStaticString()
    {
        ATLVERIFY(SetDlgItemText(IDC_STATIC_ITEM_NAME, (msf::LoadResourceString(IDS_SHELLEXT_NAME) + L":").c_str()));
        ATLVERIFY(SetDlgItemText(IDC_STATIC_ITEM_SIZE, (msf::LoadResourceString(IDS_SHELLEXT_SIZE) + L":").c_str()));
    }

    void InitializeControls()
    {
        ATLVERIFY(SetDlgItemText(IDC_EDIT_ITEM_NAME, m_item.GetName().c_str()));
        ATLVERIFY(SetDlgItemInt(IDC_EDIT_ITEM_SIZE, m_item.GetSize()));

        // Note: SHLimitInputEdit is only supported on Windows XP.
        //ATLVERIFY(SUCCEEDED(SHLimitInputEdit(GetDlgItem(IDC_EDITm_item_NAME), m_rshellfolder)));
    }

    // Member variables
    const VVVItem &       m_item;
    msf::ItemIDList       m_pidlNew;
    long &                m_wEventId;
    ATL::CComPtr<IShellFolder> m_shellFolder;
};
