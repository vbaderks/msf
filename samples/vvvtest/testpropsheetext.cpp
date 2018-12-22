//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#include "stdafx.h"

#include "testpropsheetext.h"
#include "../vvvsample/shellpropsheetextclsid.h"
#include "../include/shellextinitptr.h"
#include "../include/shellpropsheetextptr.h"
#include "../include/filelist.h"
#include "../include/propsheethost.h"
#include "../include/util.h"


namespace
{

void TestExecutePropSheetExt(IDataObject* pdataobject)
{
	MSF::CPropSheetHost propsheethost;

	{
		MSF::IShellExtInitPtr rshellextinit(__uuidof(CShellPropSheetExt));

		rshellextinit.Initialize(NULL, pdataobject, 0);

		MSF::IShellPropSheetExtPtr rshellpropsheetext(rshellextinit);

		rshellpropsheetext.AddPages(MSF::CPropSheetHost::AddPage, &propsheethost);
	}

	CoFreeUnusedLibraries(); // trigger COM to unload to locked DLLs: shellext should still be locked.
}


void TestExecutePropSheetExt(const TCHAR* tszfilename)
{
	CComPtr<MSF::CFileList> rfilelist = MSF::CFileList::CreateInstance();

	rfilelist->Add(tszfilename);

	TestExecutePropSheetExt(rfilelist);
}

} // end namespace


void TestPropSheetExt::Sanity()
{
	TestExecutePropSheetExt(MSF::GetModuleDirectory() + _T("sample1.vvv"));
}


void TestPropSheetExt::EmptyList()
{
	CComPtr<MSF::CFileList> rfilelist = MSF::CFileList::CreateInstance();

	TestExecutePropSheetExt(rfilelist);
}


void TestPropSheetExt::MultipleSelection()
{
	CComPtr<MSF::CFileList> rfilelist = MSF::CFileList::CreateInstance();

	rfilelist->Add(MSF::GetModuleDirectory() + _T("sample1.vvv"));
	rfilelist->Add(MSF::GetModuleDirectory() + _T("sample2.vvv"));

	TestExecutePropSheetExt(rfilelist);
}


void TestPropSheetExt::FileNotFound()
{
	try
	{
		TestExecutePropSheetExt(_T("filenotfound.vvv"));
	}
	catch (const _com_error&)
	{
		// expect failed HRESULT.
		return;
	}

	_com_raise_error(E_FAIL);
}


void TestPropSheetExt::IncorrectType()
{
	TestExecutePropSheetExt(MSF::GetModuleDirectory() + _T("sample.yyy"));
}
