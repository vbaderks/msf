//
// (C) Copyright by Victor Derks
//
// See README.TXT for detailed details of the software licence.
//

#include "stdafx.h"

#include "vvvfile.h"
#include <filesystem>

using std::wstring;
namespace fs = std::experimental::filesystem;

const wchar_t* TSZ_APP_NAME_DIRECTORY = L"directory";
const wchar_t* TSZ_FILE_COUNT         = L"filecount";
const wchar_t* TSZ_FILE_FORMAT        = L"file%d";
const wchar_t* TSZ_LABEL              = L"label";
const wchar_t* TSZ_NAME               = L"name";
const wchar_t* TSZ_SIZE               = L"size";
const wchar_t* TSZ_ACTIVE             = L"active";
const wchar_t* TSZ_FOLDER             = L"folder";


std::wstring VVVFile::GetLabel() const
{
    return GetPrivateProfileString(GetAppNameDirectory().c_str(), TSZ_LABEL);
}


void VVVFile::SetLabel(const std::wstring& label) const
{
    WritePrivateProfileString(GetAppNameDirectory().c_str(), TSZ_LABEL, label.c_str());
}


unsigned int VVVFile::GetFileCount() const
{
    //CComPtr<IStream> rFileStream;
    //HRESULT hr = SHCreateStreamOnFile(_strFilename, STGM_READ, &rFileStream);
    //RaiseExceptionIfFailed(hr);

    const auto nCount = GetPrivateProfileInt(TSZ_APP_NAME_DIRECTORY, TSZ_FILE_COUNT);
    msf::RaiseExceptionIf(nCount == static_cast<UINT>(-1));

    return nCount;
}


// Purpose: Scans a .vvv file and return the next active item.
//          VVV items are unique by index. To support this concept
//          deleted items become inactive but are not removed.
//          This to prevent that a delete action causes other items
//          to get a new index.
LPITEMIDLIST VVVFile::GetNextItem(DWORD grfFlags, unsigned int& nItemIterator) const
{
    for (;;)
    {
        auto strAppName = GetAppNameItem(nItemIterator);

        const auto nActive = static_cast<int>(GetPrivateProfileInt(strAppName.c_str(), TSZ_ACTIVE, -1));
        if (nActive == -1)
            return nullptr;

        ++nItemIterator;

        if (nActive == 1)
        {
            const auto bFolder = GetPrivateProfileInt(strAppName.c_str(), TSZ_FOLDER) == 1;

            if ((msf::IsBitSet(grfFlags, SHCONTF_NONFOLDERS) && !bFolder) ||
                (msf::IsBitSet(grfFlags, SHCONTF_FOLDERS) && bFolder))
            {
                const auto nSize = GetPrivateProfileInt(strAppName.c_str(), TSZ_SIZE);
                msf::RaiseExceptionIf(nSize == static_cast<UINT>(-1));
                const auto strName = GetPrivateProfileString(strAppName.c_str(), TSZ_NAME);

                return VVVItem::CreateItemIdList(nItemIterator, nSize, bFolder, strName);
            }
        }
    }
}


void VVVFile::DeleteItems(const std::vector<VVVItem>& items) const
{
    for (auto item : items)
    {
        DeleteItem(item);
    }

    SetFileCount(GetFileCount() - static_cast<unsigned int>(items.size()));

    // Note: due to .ini file implementation, inactive items will stay in .ini file.
}


void VVVFile::SetItem(const VVVItem& item) const
{
    auto strAppNameItem = GetAppNameItem(item.GetID());

    WritePrivateProfileString(strAppNameItem.c_str(), TSZ_NAME, item.GetName().c_str());
    WritePrivateProfileInt(strAppNameItem.c_str(), TSZ_SIZE, item.GetSize());
}


PUIDLIST_RELATIVE VVVFile::AddItem(const std::wstring& strFile) const
{
    const std::wstring strName(PathFindFileName(strFile.c_str()));
    return AddItem(static_cast<unsigned int>(fs::file_size(strFile)), strName);
}


PUIDLIST_RELATIVE VVVFile::AddItem(unsigned int nSize, const std::wstring& name) const
{
    const auto nId = FindFreeEntry();

    msf::ItemIDList pidlItem(VVVItem::CreateItemIdList(nId, nSize, false, name));
    AddItem(VVVItem(pidlItem.GetRelative()));

    return pidlItem.DetachRelative();
}


void VVVFile::AddItem(const VVVItem& item) const
{
    SetItem(item);
    WritePrivateProfileInt(GetAppNameItem(item.GetID()).c_str(), TSZ_ACTIVE, 1);
    SetFileCount(GetFileCount() + 1);
}


void VVVFile::DeleteItem(const VVVItem& item) const
{
    auto strAppNameItem = GetAppNameItem(item.GetID());
    ATLASSERT(GetPrivateProfileInt(strAppNameItem.c_str(), TSZ_ACTIVE) == 1); // item was already deleted!
    WritePrivateProfileInt(strAppNameItem.c_str(), TSZ_ACTIVE, 0);
}


unsigned int VVVFile::FindFreeEntry() const
{
    for (unsigned int n = 0;; ++n)
    {
        auto strAppName = GetAppNameItem(n);
        const auto nActive = GetPrivateProfileInt(strAppName.c_str(), TSZ_ACTIVE);

        if (nActive != 1)
            return n;
    }
}


void VVVFile::SetFileCount(unsigned int nNewFileCount) const
{
    return WritePrivateProfileInt(GetAppNameDirectory().c_str(), TSZ_FILE_COUNT, nNewFileCount);
}


wstring VVVFile::GetAppNameDirectory() const
{
    wstring strAppName;

    if (m_folder.empty())
    {
        strAppName = TSZ_APP_NAME_DIRECTORY;
    }
    else
    {
        strAppName = m_folder + L"\\" + TSZ_APP_NAME_DIRECTORY;
    }

    return strAppName;
}


wstring VVVFile::GetAppNameItem(unsigned int nID) const
{
    ATL::CString strAppName;
    strAppName.Format(TSZ_FILE_FORMAT, nID);

    if (!m_folder.empty())
    {
        strAppName = m_folder.c_str() + ATL::CString(L"\\") + strAppName;
    }

    return strAppName.GetString();
}


wstring VVVFile::GetPrivateProfileString(const wchar_t* lpAppName, const wchar_t* lpKeyName) const
{
    wchar_t tszBuf[255];
    ::GetPrivateProfileString(lpAppName, lpKeyName, L"", tszBuf, 255, m_filename.c_str());
    return tszBuf;
}


unsigned int  VVVFile::GetPrivateProfileInt(const wchar_t* lpAppName, const wchar_t* lpKeyName, int nDefault) const noexcept
{
    return ::GetPrivateProfileInt(lpAppName, lpKeyName, nDefault, m_filename.c_str());
}


void VVVFile::WritePrivateProfileString(const wchar_t* lpAppName, const wchar_t* lpKeyName, const wchar_t* lpString) const
{
    msf::RaiseLastErrorExceptionIf(!::WritePrivateProfileString(lpAppName, lpKeyName, lpString, m_filename.c_str()));
}


void VVVFile::WritePrivateProfileInt(const wchar_t* lpAppName, const wchar_t* lpKeyName, unsigned int nValue) const
{
    WritePrivateProfileString(lpAppName, lpKeyName, std::to_wstring(nValue).c_str());
}
