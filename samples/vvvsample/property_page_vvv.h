﻿//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software license.
//
#pragma once

#include "vvv_file.h"
#include "resource.h"

#include <msf.h>

class PropertyPageVVV final : public msf::ShellExtPropertyPageImpl<PropertyPageVVV>
{
public:
    static HPROPSHEETPAGE CreateInstance(std::wstring filename)
    {
        auto page = std::make_unique<PropertyPageVVV>(std::move(filename));
        const auto result = page->Create();
        if (result)
        {
            // Page will be deleted by ATL base class when the property sheet is done with it.
            std::ignore = page.release(); // -V530
        }
        return result;
    }

    enum { IDD = IDD_PROPERTY_PAGE_VVV };

#pragma warning(push)
#pragma warning(disable: 26433) // use override
    BEGIN_MSG_MAP(PropertyPageVVV)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        COMMAND_HANDLER(IDC_EDIT_LABEL, EN_CHANGE, OnChangeEditName)
        CHAIN_MSG_MAP(ShellExtPropertyPageImpl<PropertyPageVVV>)
    END_MSG_MAP()
#pragma warning(pop)

    explicit PropertyPageVVV(std::wstring strFilename) :
        m_filename(std::move(strFilename))
    {
        const VVVFile vvvFile(m_filename);

        // Retrieve the file data: the sample don't shows the page if this fails.
        m_label   = vvvFile.GetLabel();
        m_fileCount = vvvFile.GetFileCount();
    }

    LRESULT OnInitDialog(uint32_t /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
    {
        InitializeStaticString();
        InitializeControls();
        return 1;
    }

    LRESULT OnChangeEditName(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& bHandled)
    {
        const auto label = GetLabel();
        SetModified(label != m_label);
        bHandled = true;
        return 1;
    }

    BOOL OnApply()
    {
        const auto label = GetLabel();
        if (label.empty())
            return false; // an empty name is invalid (in our VVV example).

        try
        {
            VVVFile(m_filename).SetLabel(label);
            m_label = label;
            return true;
        }
        catch (const _com_error& e)
        {
            const auto message = msf::LoadResourceString(IDS_PROPERTYPAGE_UNABLE_TO_UPDATE) +
                                 msf::FormatLastError(static_cast<DWORD>(e.Error()));
            IsolationAwareMessageBox(GetParent().m_hWnd, message.c_str(), msf::LoadResourceString(IDS_SHELLEXT_ERROR_CAPTION).c_str(), MB_OK | MB_ICONERROR);
        }

        return false;
    }

private:
    // It is easier to patch the dialog during runtime with the locale
    // strings then to maintain copies of the dialog resource.
    void InitializeStaticString()
    {
        ATLVERIFY(SetDlgItemText(IDC_STATIC_LABEL, (msf::LoadResourceString(IDS_SHELLEXT_LABEL) + L":").c_str()));
        ATLVERIFY(SetDlgItemText(IDC_STATIC_FILECOUNT, (msf::LoadResourceString(IDS_SHELLEXT_FILECOUNT) + L":").c_str()));
    }

    void InitializeControls() noexcept
    {
        ATLVERIFY(SetDlgItemText(IDC_EDIT_LABEL, m_label.c_str()));
        ATLVERIFY(SetDlgItemInt(IDC_EDIT_FILECOUNT, m_fileCount));
    }

    [[nodiscard]] std::wstring GetLabel() const
    {
        auto label = GetDlgItemText(IDC_EDIT_LABEL);
        msf::trim(label);
        return label;
    }

    // Member variables
    std::wstring m_filename;
    std::wstring m_label;
    unsigned int m_fileCount;
};
