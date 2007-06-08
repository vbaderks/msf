//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once
#include "../include/propertypageimpl.h"
#include "../include/strutil.h"
#include "../include/util.h"
#include "vvvfile.h"
#include "resource.h"


class CPropertyPageVVV : public CShellExtPropertyPageImpl<CPropertyPageVVV>
{
public:
	static HPROPSHEETPAGE CreateInstance(const CString& strFilename)
	{
		CPropertyPageVVV* ppage = new CPropertyPageVVV(strFilename);
		return ppage->Create();
	}

	enum { IDD = IDD_PROPERTY_PAGE_VVV };

	BEGIN_MSG_MAP(CPropertyPageVVV)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_EDIT_LABEL, EN_CHANGE, OnChangeEditName)
		CHAIN_MSG_MAP(CShellExtPropertyPageImpl<CPropertyPageVVV>)
	END_MSG_MAP()

	CPropertyPageVVV(const CString& strFilename) :
		_strFilename(strFilename)
	{
		CVVVFile vvvfile(strFilename);

		// Retrieve the file data: the sample don't shows the page if this fails.
		_strLabel   = vvvfile.GetLabel();
		_nFileCount = vvvfile.GetFileCount();
	}


	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		InitializeStaticString();
		InitializeControls();
		return 1;
	}


	LRESULT OnChangeEditName(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& bHandled)
	{
		CString str = GetLabel();
		SetModified(str != _strLabel);
		bHandled = true;
		return 1;
	}


	BOOL OnApply()
	{
		CString str = GetLabel();
		if (str.IsEmpty())
			return false; // an empty name is invalid (in our VVV example).

		try
		{
			CVVVFile(_strFilename).SetLabel(str);
			_strLabel = str;
			return true;
		}
		catch (const _com_error& e)
		{
			CString strMsg = LoadString(IDS_PROPERTYPAGE_UNABLE_TO_UPDATE) +
			                 FormatLastError(static_cast<DWORD>(e.Error()));
			IsolationAwareMessageBox(GetParent().m_hWnd, strMsg, MSF::LoadString(IDS_SHELLEXT_ERROR_CAPTION), MB_OK | MB_ICONERROR);
		}

		return false;
	}

private:
	// It is easier to patch the dialog during runtime with the locale
	// strings then to maintain copies of the dialog resource.
	void InitializeStaticString()
	{
		ATLVERIFY(SetDlgItemText(IDC_STATIC_LABEL, LoadString(IDS_SHELLEXT_LABEL) + _T(":")));
		ATLVERIFY(SetDlgItemText(IDC_STATIC_FILECOUNT, LoadString(IDS_SHELLEXT_FILECOUNT) + _T(":")));
	}


	void InitializeControls()
	{
		ATLVERIFY(SetDlgItemText(IDC_EDIT_LABEL, _strLabel));
		ATLVERIFY(SetDlgItemInt(IDC_EDIT_FILECOUNT, _nFileCount));
	}


	CString GetLabel()
	{
		CString str;

		GetDlgItemText(IDC_EDIT_LABEL, str);
		str.Trim();

		return str;
	}


	// Member variables
	CString _strFilename;
	CString _strLabel;
	UINT    _nFileCount;
};
