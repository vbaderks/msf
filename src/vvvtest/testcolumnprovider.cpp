//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#include "stdafx.h"

#include "testcolumnprovider.h"
#include "../vvvsample/columnproviderclsid.h"
#include "../include/columnproviderptr.h"
#include "../include/util.h"


namespace
{

void GetAllColumnInfo(MSF::IColumnProviderPtr columnprovider, PCWSTR wszPathname, ULONG dwflags = 0)
{
	const unsigned long nColumns = columnprovider.GetColumnCount();

	for (unsigned long i = 0; i < nColumns; ++i)
	{
		SHCOLUMNINFO columninfo;

		if (!columnprovider.GetColumnInfo(i, columninfo))
			_com_raise_error(E_FAIL);

		_variant_t varData;
		if (!columnprovider.GetItemData(wszPathname, columninfo.scid, varData, dwflags))
			_com_raise_error(E_FAIL);
	}
}


void CreateAndCheckColumnProvider(PCWSTR wszFilename, ULONG dwflags = 0)
{
	MSF::IColumnProviderPtr columnprovider(__uuidof(CColumnProvider));

	CStringW strPathname = MSF::GetModuleDirectoryW() + wszFilename;

	GetAllColumnInfo(columnprovider, strPathname, dwflags);
}

} // namespace


void TestColumnProvider::Sanity()
{
	CreateAndCheckColumnProvider(L"sample1.vvv");
}


void TestColumnProvider::GetItemDataWithUpdateItem()
{
	CreateAndCheckColumnProvider(L"sample1.vvv", SHCDF_UPDATEITEM);
}


void TestColumnProvider::Directory()
{
	MSF::IColumnProviderPtr columnprovider(__uuidof(CColumnProvider));

	GetAllColumnInfo(columnprovider, CT2W(GetSystemDirectory()));
}


void TestColumnProvider::DesktopDirectory()
{
	MSF::IColumnProviderPtr columnprovider(__uuidof(CColumnProvider));
	
	CStringW strPathname = GetFolderPath(CSIDL_DESKTOPDIRECTORY) + L"\\sample_not_there.vvv";

	GetAllColumnInfo(columnprovider, strPathname);
}



void TestColumnProvider::FileNotFound()
{
	try
	{
		CreateAndCheckColumnProvider(L"filenotfound.vvv");
	}
	catch (const _com_error&)
	{
		// expect failed HRESULT.
		return;
	}

	_com_raise_error(E_FAIL);
}


void TestColumnProvider::IncorrectType()
{
	MSF::IColumnProviderPtr columnprovider(__uuidof(CColumnProvider));

	unsigned long nColumns = columnprovider.GetColumnCount();

	CStringW strFile = MSF::GetModuleDirectoryW() + L"sample.yyy";

	for (unsigned long i = 0; i < nColumns; ++i)
	{
		SHCOLUMNINFO columninfo;

		if (!columnprovider.GetColumnInfo(i, columninfo))
			_com_raise_error(E_FAIL);

		_variant_t varData;
		if (columnprovider.GetItemData(strFile, columninfo.scid, varData))
			_com_raise_error(E_FAIL); // should not return data.
	}

	SHCOLUMNINFO columninfo;
	if (columnprovider.GetColumnInfo(nColumns + 1, columninfo))
		_com_raise_error(E_FAIL); // should not return ok.
}


void TestColumnProvider::StandardSortPerformance()
{
	CStringW strSample1 = MSF::GetModuleDirectoryW() + L"sample1.vvv";
	CStringW strSample2 = MSF::GetModuleDirectoryW() + L"sample2.vvv";

	MSF::IColumnProviderPtr columnprovider(__uuidof(CColumnProvider));

	SHCOLUMNINFO columninfo;

	if (!columnprovider.GetColumnInfo(0, columninfo))
		_com_raise_error(E_FAIL);

	columnprovider.GetItemData(strSample1, columninfo.scid);
	columnprovider.GetItemData(strSample2, columninfo.scid);

	if (!columnprovider.GetColumnInfo(1, columninfo))
		_com_raise_error(E_FAIL);

	columnprovider.GetItemData(strSample1, columninfo.scid);
	columnprovider.GetItemData(strSample2, columninfo.scid);
}


void TestColumnProvider::SimpleSort()
{
	CStringW strSample1 = MSF::GetModuleDirectoryW() + L"sample1.vvv";
	CStringW strSample2 = MSF::GetModuleDirectoryW() + L"sample2.vvv";

	MSF::IColumnProviderPtr columnprovider(__uuidof(CColumnProvider));

	SHCOLUMNINFO columninfo;

	if (!columnprovider.GetColumnInfo(0, columninfo))
		_com_raise_error(E_FAIL);

	columnprovider.GetItemData(strSample1, columninfo.scid);
	columnprovider.GetItemData(strSample2, columninfo.scid);
}
