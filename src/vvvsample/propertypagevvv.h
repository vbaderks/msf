//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "vvvfile.h"
#include "resource.h"
#include <msf.h>

class PropertyPageVVV : public MSF::ShellExtPropertyPageImpl<PropertyPageVVV>
{
public:
    static HPROPSHEETPAGE CreateInstance(std::wstring filename)
    {
        auto ppage = new PropertyPageVVV(filename);
        return ppage->Create();
    }

    enum { IDD = IDD_PROPERTY_PAGE_VVV };

    BEGIN_MSG_MAP(PropertyPageVVV)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        COMMAND_HANDLER(IDC_EDIT_LABEL, EN_CHANGE, OnChangeEditName)
        CHAIN_MSG_MAP(ShellExtPropertyPageImpl<PropertyPageVVV>)
    END_MSG_MAP()

    explicit PropertyPageVVV(std::wstring strFilename) :
        m_filename(strFilename)
    {
        VVVFile vvvfile(strFilename);

        // Retrieve the file data: the sample don't shows the page if this fails.
        m_label   = vvvfile.GetLabel();
        m_fileCount = vvvfile.GetFileCount();
    }

    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
    {
        InitializeStaticString();
        InitializeControls();
        return 1;
    }

    LRESULT OnChangeEditName(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& bHandled)
    {
        auto label = GetLabel();
        SetModified(label != m_label);
        bHandled = true;
        return 1;
    }

    BOOL OnApply()
    {
        auto label = GetLabel();
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
            auto message = MSF::LoadResourceString(IDS_PROPERTYPAGE_UNABLE_TO_UPDATE) +
                MSF::FormatLastError(static_cast<DWORD>(e.Error()));
            IsolationAwareMessageBox(GetParent().m_hWnd, message.c_str(), MSF::LoadString(IDS_SHELLEXT_ERROR_CAPTION), MB_OK | MB_ICONERROR);
        }

        return false;
    }

private:
    // It is easier to patch the dialog during runtime with the locale
    // strings then to maintain copies of the dialog resource.
    void InitializeStaticString()
    {
        ATLVERIFY(SetDlgItemText(IDC_STATIC_LABEL, (MSF::LoadResourceString(IDS_SHELLEXT_LABEL) + L":").c_str()));
        ATLVERIFY(SetDlgItemText(IDC_STATIC_FILECOUNT, (MSF::LoadResourceString(IDS_SHELLEXT_FILECOUNT) + L":").c_str()));
    }

    void InitializeControls()
    {
        ATLVERIFY(SetDlgItemText(IDC_EDIT_LABEL, m_label.c_str()));
        ATLVERIFY(SetDlgItemInt(IDC_EDIT_FILECOUNT, m_fileCount));
    }

    std::wstring GetLabel() const
    {
        auto label = GetDlgItemText(IDC_EDIT_LABEL);
        MSF::trim(label);
        return label;
    }

    // Member variables
    std::wstring m_filename;
    std::wstring m_label;
    unsigned int m_fileCount;
};
