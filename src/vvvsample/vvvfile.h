//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software license.
//
#pragma once


#include "vvvitem.h"


class VVVFile
{
public:
    explicit VVVFile(std::wstring filename, std::wstring folder = std::wstring()) noexcept :
        m_filename{std::move(filename)},
        m_folder{std::move(folder)}
    {
    }

    std::wstring GetLabel() const;
    void SetLabel(const std::wstring& label) const;
    unsigned int GetFileCount() const;
    LPITEMIDLIST GetNextItem(DWORD grfFlags, unsigned int& itemIterator) const;
    void DeleteItems(const std::vector<VVVItem>& items) const;
    void SetItem(const VVVItem& item) const;

    PUIDLIST_RELATIVE AddItem(const std::wstring& file) const;
    PUIDLIST_RELATIVE AddItem(unsigned int size, const std::wstring& name) const;

private:
    void AddItem(const VVVItem& item) const;
    void DeleteItem(const VVVItem& item) const;

    void SetFileCount(unsigned int newFileCount) const;
    unsigned int FindFreeEntry() const;

    std::wstring GetAppNameDirectory() const;
    std::wstring GetAppNameItem(unsigned int id) const;

    std::wstring GetPrivateProfileString(const wchar_t* appName, const wchar_t* keyName) const;
    unsigned int GetPrivateProfileInt(const wchar_t* appName, const wchar_t* keyName, int defaultValue = -1) const noexcept;

    void WritePrivateProfileString(const wchar_t* appName, const wchar_t* keyName, const wchar_t* value) const;
    void WritePrivateProfileInt(const wchar_t* appName, const wchar_t* keyName, unsigned int value) const;

    // Member variables
    std::wstring m_filename;
    std::wstring m_folder;
};
