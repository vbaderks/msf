//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "vvvitem.h"


class VVVFile
{
public:
    explicit VVVFile(const CString& filename, const CString& folder = CString()) :
        _filename(filename),
        _folder(folder)
    {
    }

    CString GetLabel() const;
    void SetLabel(const CString& strLabel) const;
    unsigned int GetFileCount() const;
    LPITEMIDLIST GetNextItem(DWORD grfFlags, unsigned int& nItemIterator) const;
    void DeleteItems(const CVVVItemList& itemsToDelete) const;
    void SetItem(const VVVItem& item) const;

    LPITEMIDLIST AddItem(const CString& strFile) const;
    LPITEMIDLIST AddItem(unsigned int nSize, const CString& strName) const;

private:

    void AddItem(const VVVItem& item) const;
    void DeleteItem(const VVVItem& item) const;

    void SetFileCount(unsigned int nNewFileCount) const;
    unsigned int FindFreeEntry() const;

    CString GetAppNameDirectory() const;
    CString GetAppNameItem(unsigned int nID) const;

    CString GetPrivateProfileString(const wchar_t* lpAppName, const wchar_t* lpKeyName) const;
    unsigned int GetPrivateProfileInt(const wchar_t* lpAppName, const wchar_t* lpKeyName, int nDefault = -1) const;

    void WritePrivateProfileString(const wchar_t* lpAppName, const wchar_t* lpKeyName, const wchar_t* lpString) const;
    void WritePrivateProfileInt(const wchar_t* lpAppName, const wchar_t* lpKeyName, unsigned int nValue) const;

    // Member variables
    CString _filename;
    CString _folder;
};
