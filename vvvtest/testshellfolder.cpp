//
// (C) Copyright by Victor Derks <vba64@xs4all.nl>
//
// See README.TXT for the details of the software licence.
//
#include "stdafx.h"


#include "testshellfolder.h"
#include "../vvvsample/shellfolderclsid.h"
#include "../include/shellfolderptr.h"
#include "../include/shellviewptr.h"
#include "../include/enumidlistptr.h"
#include "../include/pidl.h"
#include "../include/util.h"


namespace
{

MSF::IShellFolderPtr CreateShellFolder(const TCHAR* szFilename)
{
	return MSF::IShellFolderPtr(__uuidof(CShellFolder), MSF::GetModuleDirectory() + szFilename);
}

} // end namespace


void TestShellFolder::Sanity()
{
	CreateShellFolder(_T("sample1.vvv"));
}


void TestShellFolder::FileNotFound()
{
	try
	{
		CreateShellFolder(_T("filenotfound"));
	}
	catch (const _com_error&)
	{
		// expect failed HRESULT.
		return;
	}

	_com_raise_error(E_FAIL);
}


void TestShellFolder::IncorrectType()
{
	CreateShellFolder(_T("sample.yyy"));
}


void TestShellFolder::FileTwice()
{
	MSF::IShellFolderPtr shellfolder = CreateShellFolder(_T("sample1.vvv"));

	shellfolder.Initialize(MSF::GetModuleDirectory() + _T("sample1.vvv"));
}


void TestShellFolder::GetShellFolderView()
{
	MSF::IShellFolderPtr shellfolder = CreateShellFolder(_T("sample1.vvv"));

	MSF::IShellViewPtr shellview = shellfolder.CreateViewObject(__uuidof(IShellView));
}


void TestShellFolder::EnumObjects()
{
	MSF::IShellFolderPtr shellfolder = CreateShellFolder(_T("sample1.vvv"));

	MSF::IEnumIDListPtr enumidlist = shellfolder.EnumObjects(SHCONTF_NONFOLDERS | SHCONTF_FOLDERS);

	CPidl pidl;

	while (enumidlist.Next(&pidl))
	{
		SFGAOF sfgaof = 0xFFFFFFFF;
		shellfolder.GetAttributesOf(pidl, &sfgaof);
	}
}

