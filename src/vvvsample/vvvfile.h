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
    explicit VVVFile(std::wstring filename, std::wstring folder = std::wstring()) :
        m_filename{std::move(filename)},
        m_folder{std::move(folder)}
    {
    }

    std::wstring GetLabel() const;
    void SetLabel(const std::wstring& label) const;
    unsigned int GetFileCount() const;
    LPITEMIDLIST GetNextItem(DWORD grfFlags, unsigned int& nItemIterator) const;
    void DeleteItems(const CVVVItemList& items) const;
    void SetItem(const VVVItem& item) const;

    PUIDLIST_RELATIVE AddItem(const std::wstring& strFile) const;
    PUIDLIST_RELATIVE AddItem(unsigned int nSize, const ATL::CString& name) const;

private:

    void AddItem(const VVVItem& item) const;
    void DeleteItem(const VVVItem& item) const;

    void SetFileCount(unsigned int nNewFileCount) const;
    unsigned int FindFreeEntry() const;

    std::wstring GetAppNameDirectory() const;
    std::wstring GetAppNameItem(unsigned int nID) const;

    std::wstring GetPrivateProfileString(const wchar_t* lpAppName, const wchar_t* lpKeyName) const;
    unsigned int GetPrivateProfileInt(const wchar_t* lpAppName, const wchar_t* lpKeyName, int nDefault = -1) const;

    void WritePrivateProfileString(const wchar_t* lpAppName, const wchar_t* lpKeyName, const wchar_t* lpString) const;
    void WritePrivateProfileInt(const wchar_t* lpAppName, const wchar_t* lpKeyName, unsigned int nValue) const;

    // Member variables
    std::wstring m_filename;
    std::wstring m_folder;
};
