//
// (C) Copyright by Victor Derks <vba64@xs4all.nl>
//
// See README.TXT for the details of the software licence.
//
#include "stdafx.h"

#include "testcontextmenu.h"
#include "../vvvsample/contextmenuclsid.h"
#include "../include/shellextinitptr.h"
#include "../include/contextmenuptr.h"
#include "../include/filelist.h"
#include "../include/menu.h"
#include "../include/util.h"
#include "../include/macros.h"


namespace
{

void TestExecuteContextMenu(IDataObject* pdataobject)
{
	MSF::IShellExtInitPtr rshellextinit(__uuidof(CContextMenu));

	rshellextinit.Initialize(NULL, pdataobject, 0);

	MSF::IContextMenuPtr rcontextmenu(rshellextinit);

	CMenu menu;

	UINT nAdded = rcontextmenu.QueryContextMenu(menu, 0, 0, 10, CMF_NORMAL);

	for (UINT i = 0; i < nAdded; ++i)
	{
		char cbuffer[40];
		rcontextmenu.GetCommandString(i, GCS_HELPTEXTA, cbuffer, MSF_ARRAY_SIZE(cbuffer));

		wchar_t wbuffer[40];
		rcontextmenu.GetCommandString(i, GCS_HELPTEXTW, wbuffer, MSF_ARRAY_SIZE(wbuffer));
	}
}


void TestExecuteContextMenu(const TCHAR* tszfilename)
{
	CComPtr<CFileList> rfilelist = CFileList::CreateInstance();

	rfilelist->Add(tszfilename);

	TestExecuteContextMenu(rfilelist);
}

} // end namespace



void TestContextMenu::Sanity()
{
	TestExecuteContextMenu(GetModuleDirectory() + _T("sample1.vvv"));
}



void TestContextMenu::EmptyList()
{
	CComPtr<CFileList> rfilelist = CFileList::CreateInstance();

	TestExecuteContextMenu(rfilelist);
}


void TestContextMenu::MultipleSelection()
{
	CComPtr<MSF::CFileList> rfilelist = MSF::CFileList::CreateInstance();

	rfilelist->Add(GetModuleDirectory() + _T("sample1.vvv"));
	rfilelist->Add(GetModuleDirectory() + _T("sample2.vvv"));

	TestExecuteContextMenu(rfilelist);
}


void TestContextMenu::IncorrectType()
{
	TestExecuteContextMenu(GetModuleDirectory() + _T("sample.yyy"));
}
