//
// (C) Copyright by Victor Derks <vba64@xs4all.nl>
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "vvvitem.h"


class CVVVFile
{
public:
	CVVVFile(const CString& strFilename, const CString& strFolder = CString()) : 
		_strFilename(strFilename),
		_strFolder(strFolder)
	{
	}

	CString GetLabel() const;
	void SetLabel(const CString& strLabel) const;
	unsigned int GetFileCount() const;
	ITEMIDLIST* GetNextItem(DWORD grfFlags, unsigned int& nItemIterator) const;
	void DeleteItems(const CVVVItemList& itemsToDelete) const;
	void SetItem(const CVVVItem& item) const;

	ITEMIDLIST* AddItem(const CString& strFile) const;
	ITEMIDLIST* AddItem(unsigned int nSize, const CString& strName) const;

private:

	void AddItem(const CVVVItem& item) const;
	void DeleteItem(const CVVVItem& item) const;

	void SetFileCount(unsigned int nNewFileCount) const;
	unsigned int FindFreeEntry() const;

	CString GetAppNameDirectory() const;
	CString GetAppNameItem(unsigned int nID) const;

	CString GetPrivateProfileString(const TCHAR* lpAppName, const TCHAR* lpKeyName) const;
	unsigned int GetPrivateProfileInt(const TCHAR* lpAppName, const TCHAR* lpKeyName, int nDefault = -1) const;

	void WritePrivateProfileString(const TCHAR* lpAppName, const TCHAR* lpKeyName, const TCHAR* lpString) const;
	void WritePrivateProfileInt(const TCHAR* lpAppName, const TCHAR* lpKeyName, unsigned int nValue) const;

	// Member variables
	CString _strFilename;
	CString _strFolder;
};
