//
// (C) Copyright by Victor Derks
//
// See README.TXT for detailed details of the software licence.
//
#include "stdafx.h"
#include "vvvfile.h"
#include "../include/util.h"
#include "../include/strutil.h"
#include "../include/pidl.h"


const TCHAR* TSZ_APP_NAME_DIRECTORY = _T("directory");
const TCHAR* TSZ_FILE_COUNT         = _T("filecount");
const TCHAR* TSZ_FILE_FORMAT        = _T("file%d");
const TCHAR* TSZ_LABEL              = _T("label");
const TCHAR* TSZ_NAME               = _T("name");
const TCHAR* TSZ_SIZE               = _T("size");
const TCHAR* TSZ_ACTIVE             = _T("active");
const TCHAR* TSZ_FOLDER             = _T("folder");


CString CVVVFile::GetLabel() const
{
	return GetPrivateProfileString(GetAppNameDirectory(), TSZ_LABEL);
}


void CVVVFile::SetLabel(const CString& strLabel) const
{
	WritePrivateProfileString(GetAppNameDirectory(), TSZ_LABEL, strLabel);
}


unsigned int CVVVFile::GetFileCount() const
{
	unsigned int nCount = GetPrivateProfileInt(TSZ_APP_NAME_DIRECTORY, TSZ_FILE_COUNT);
	RaiseExceptionIf(nCount == static_cast<UINT>(-1));

	return nCount;
}


// Purpose: Scans a .vvv file and return the next active item.
//          VVV items are unique by index. To support this concept
//          deleted items become inactive but are not removed.
//          This to prevent that a delete action causes other items
//          to get a new index.
ITEMIDLIST* CVVVFile::GetNextItem(DWORD grfFlags, unsigned int& nItemIterator) const
{
	for (;;)
	{
		CString strAppName = GetAppNameItem(nItemIterator);

		unsigned int nActive = GetPrivateProfileInt(strAppName, TSZ_ACTIVE, -1);
		if (nActive == -1)
			return NULL;

		++nItemIterator;
		
		if (nActive == 1)
		{
			bool bFolder = GetPrivateProfileInt(strAppName, TSZ_FOLDER) == 1;

			if ((IsBitSet(grfFlags, SHCONTF_NONFOLDERS) && !bFolder) ||
				(IsBitSet(grfFlags, SHCONTF_FOLDERS) && bFolder))
			{
				unsigned int nSize = GetPrivateProfileInt(strAppName, TSZ_SIZE);
				RaiseExceptionIf(nSize == static_cast<UINT>(-1));
				CString strName = GetPrivateProfileString(strAppName, TSZ_NAME);

				return CVVVItem::CreateItemIdList(nItemIterator, nSize, bFolder, strName);
			}
		}
	}
}


void CVVVFile::DeleteItems(const CVVVItemList& items) const
{
	for (CVVVItemList::const_iterator it = items.begin(); it != items.end(); ++it)
	{
		DeleteItem(*it);
	}

	SetFileCount(GetFileCount() - items.size());

	// Note: due to .ini file implementation, inactive items will stay in .ini file.
}


void CVVVFile::SetItem(const CVVVItem& item) const
{
	CString strAppNameItem = GetAppNameItem(item.GetID());

	WritePrivateProfileString(strAppNameItem, TSZ_NAME, CString(item.GetName()));
	WritePrivateProfileInt(strAppNameItem, TSZ_SIZE, item.GetSize());
}


ITEMIDLIST* CVVVFile::AddItem(const CString& strFile) const
{
	CString strName = PathFindFileName(strFile);
	DWORD dwSize    = GetFileSize(strFile);

	return AddItem(dwSize, strName);
}


ITEMIDLIST* CVVVFile::AddItem(unsigned int nSize, const CString& strName) const
{
	unsigned int nId = FindFreeEntry();

	CPidl pidlItem(CVVVItem::CreateItemIdList(nId, nSize, false, strName));

	AddItem(CVVVItem(pidlItem));

	return pidlItem.Detach();
}


void CVVVFile::AddItem(const CVVVItem& item) const
{
	SetItem(item);

	WritePrivateProfileInt(GetAppNameItem(item.GetID()), TSZ_ACTIVE, 1);

	SetFileCount(GetFileCount() + 1);
}


void CVVVFile::DeleteItem(const CVVVItem& item) const
{
	CString strAppNameItem = GetAppNameItem(item.GetID());

	ATLASSERT(GetPrivateProfileInt(strAppNameItem, TSZ_ACTIVE) == 1); // item was already deleted!

	WritePrivateProfileInt(strAppNameItem, TSZ_ACTIVE, 0);
}


unsigned int CVVVFile::FindFreeEntry() const
{
	for (unsigned int n = 0;; ++n)
	{
		CString strAppName = GetAppNameItem(n);

		unsigned int nActive = GetPrivateProfileInt(strAppName, TSZ_ACTIVE);

		if (nActive != 1)
			return n;
	}
}


void CVVVFile::SetFileCount(UINT nNewFileCount) const
{
	return WritePrivateProfileInt(GetAppNameDirectory(), TSZ_FILE_COUNT, nNewFileCount);
}


CString CVVVFile::GetAppNameDirectory() const
{
	CString strAppName;

	if (_strFolder.IsEmpty())
	{
		strAppName = TSZ_APP_NAME_DIRECTORY;
	}
	else
	{
		strAppName = _strFolder + _T("\\") + TSZ_APP_NAME_DIRECTORY;
	}

	return strAppName;
}


CString CVVVFile::GetAppNameItem(unsigned int nID) const
{
	CString strAppName;
	strAppName.Format(TSZ_FILE_FORMAT, nID);

	if (!_strFolder.IsEmpty())
	{
		strAppName = _strFolder + _T("\\") + strAppName;
	}

	return strAppName;
}


CString CVVVFile::GetPrivateProfileString(LPCTSTR lpAppName, LPCTSTR lpKeyName) const
{
	TCHAR tszBuf[255];

	::GetPrivateProfileString(lpAppName, lpKeyName, _T(""), tszBuf, 255, _strFilename);

	return tszBuf;
}


UINT CVVVFile::GetPrivateProfileInt(LPCTSTR lpAppName, LPCTSTR lpKeyName, int nDefault) const
{
	return ::GetPrivateProfileInt(lpAppName, lpKeyName, nDefault, _strFilename);
}


void CVVVFile::WritePrivateProfileString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpString) const
{
	RaiseLastErrorExceptionIf(
		!::WritePrivateProfileString(lpAppName, lpKeyName, lpString, _strFilename));
}


void CVVVFile::WritePrivateProfileInt(LPCTSTR lpAppName, LPCTSTR lpKeyName, UINT nValue) const
{
	WritePrivateProfileString(lpAppName, lpKeyName, ToString(nValue));
}
