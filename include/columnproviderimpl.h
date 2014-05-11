//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"

#pragma warning(push)
#pragma warning(disable: 4619) // #pragma warning : there is no warning number 'xxx'
#include <map>
#pragma warning(pop)

#include "updateregistry.h"
#include "strutil.h"
#include "catchhandler.h"
#include "shelluuids.h"
#include "util.h"

namespace MSF
{

// Note: Due to a bug in explorer it will not release the interface if the
//       folder is the 'desktop'. The workaround is to return 0 columns in that case.
//       This will prevent the resource leak. The drawback is that there are no
//       columns available if the 'desktop' folder is viewed in 'detailed' mode (which is a rare case).

template <typename T, bool t_bEnableDesktopBugWorkAround = true>
class ATL_NO_VTABLE IColumnProviderImpl : public IColumnProvider
{
public:
    typedef std::map<CStringW, std::vector<CString> > mapCacheInfo;

    struct CCompareShColumnId
    {
        bool operator()(const SHCOLUMNID& left, const SHCOLUMNID& right) const
        {
            return memcmp(&left, &right, sizeof(SHCOLUMNID)) < 0;
        }
    };

    typedef std::map<SHCOLUMNID, unsigned int, CCompareShColumnId> mapColumnIdToIndex;

    template <bool t_bEnableDesktopBugWorkAround>
    class CDesktopBugWorkAround
    {
    public:
        void Initialize(PCWSTR /*wszFolder*/)
        {
        }

        bool operator()() const throw()
        {
            return false;
        }
    };

    template <>
    class CDesktopBugWorkAround<true>
    {
    public:

        // Purpose: returns true if the wszFolder equals the 'all users' or user
        //          desktop folder. 
        static bool IsDesktopPath(PCWSTR wszFolder)
        {
            return GetFolderPath(CSIDL_COMMON_DESKTOPDIRECTORY) == wszFolder ||
                GetFolderPath(CSIDL_DESKTOPDIRECTORY) == wszFolder;
        }

        void Initialize(PCWSTR wszFolder)
        {
            m_bHideDesktopColumns = IsDesktopPath(wszFolder);
        }

        bool operator()() const throw()
        {
            return m_bHideDesktopColumns;
        }

    private:
        bool m_bHideDesktopColumns;
    };

    /// <summary>Registration function to register the COM object and a ProgId/extension.</summary>
    static HRESULT WINAPI UpdateRegistry(BOOL bRegister, UINT nResId, PCWSTR szDescription) throw()
    {
        return UpdateRegistryFromResource(nResId, bRegister, szDescription, T::GetObjectCLSID(), L"");
    }

    IColumnProviderImpl() :
        m_bInitialized(false),
        m_pCachedInfo(nullptr),
        m_emptyColumnInfo()
    {
        ATLTRACE2(atlTraceCOM, 0, L"IColumnProviderImpl::IColumnProviderImpl (instance=%p)\n", this);
    }

    ~IColumnProviderImpl() throw()
    {
        ATLTRACE2(atlTraceCOM, 0, L"IColumnProviderImpl::~IColumnProviderImpl (instance=%p)\n", this);
    }

    // IColumnProvider
    STDMETHOD(Initialize)(_In_ const SHCOLUMNINIT* psci)
    {
        try
        {
            ATLTRACE2(atlTraceCOM, 0,
                L"IColumnProviderImpl::Initialize, i=%p, tid=%d, dwFlags=%d, wszFolder=%s\n",
                this, GetCurrentThreadId(), psci->dwFlags, psci->wszFolder);

            // Note: the SDK docs are unclear if 
            // TODO: check if explorer calls this function twice.

            // Clear internal caching variables.
            m_pCachedInfo = nullptr;
            m_strCachedFilename.Empty();
            m_mapCacheInfo.clear();

            m_desktopbugworkaround.Initialize(psci->wszFolder);

            m_columninfos.clear();
            m_columnidtoindex.clear();

            // Note: OnInitialize needs to be implemented by the derived class.
            static_cast<T*>(this)->OnInitialize(psci->wszFolder);

            m_bInitialized = true;
            return S_OK;
        }
        MSF_COM_CATCH_HANDLER()
    }

    // Purpose: GetColumnInfo is called by the shell to retrieve the column names.
    // By calling this repeatedly the shell can detect how many columns there are.
    STDMETHOD(GetColumnInfo)(DWORD dwIndex, _Out_ SHCOLUMNINFO* psci)
    {
        ATLTRACE2(atlTraceCOM, 0, L"IColumnProviderImpl::GetColumnInfo, i=%p, tid=%d, dwIndex=%d\n",
            this, GetCurrentThreadId(), dwIndex);

        if (!m_bInitialized)
        {
            ATLTRACE2(atlTraceCOM, 0, L"IColumnProviderImpl::GetColumnInfo, i=%p, Initialize was not called\n", this);
            return E_FAIL;
        }

        if (m_desktopbugworkaround() || dwIndex >= m_columninfos.size())
        {
            *psci = m_emptyColumnInfo;
            return S_FALSE; // tell the shell there are no more columns.
        }

        *psci = m_columninfos[dwIndex];
        return S_OK;
    }

    STDMETHOD(GetItemData)(_In_ const SHCOLUMNID* pscid, _In_ const SHCOLUMNDATA* pscd, _Out_ VARIANT* pvarData)
    {
        try
        {
            ATLTRACE2(atlTraceCOM, 0, L"IColumnProviderImpl::GetItemData, i=%p, tid=%d, f=%d, c=%d, file=%s\n",
                this, GetCurrentThreadId(), pscd->dwFlags, pscid->pid, CW2T(pscd->wszFile));

            if (!m_bInitialized)
            {
                ATLTRACE2(atlTraceCOM, 0, L"IColumnProviderImpl::GetItemData, i=%p, Initialize was not called\n", this);
                return E_FAIL;
            }

            if (!IsSupportedItem(*pscd))
            {
                VariantInit(pvarData); // must initialise out args as we return a success code.
                return S_FALSE;
            }

            PCWSTR wszFilename = PathFindFileNameW(pscd->wszFile);

            bool bFlushCache = IsBitSet(pscd->dwFlags, SHCDF_UPDATEITEM);
            const std::vector<CString>* pCachedInfo;

            if (bFlushCache)
            {
                pCachedInfo = nullptr;
            }
            else
            {
                // User of this interface (explorer.exe) are expected to ask 
                // all active item data per file.
                pCachedInfo = FindInLastUsedCache(wszFilename);
            }

            if (!pCachedInfo)
            {
                pCachedInfo = GetAndCacheFileInfo(wszFilename, pscd->wszFile, bFlushCache);
            }

            pvarData->bstrVal = (*pCachedInfo)[GetIndex(*pscid)].AllocSysString();
            pvarData->vt = VT_BSTR;

            return S_OK;
        }
        MSF_COM_CATCH_HANDLER()
    }

protected:
    // Purpose: provides a default GUID for the columns.
    const GUID& GetStandardFormatIdentifier() const
    {
        return T::GetObjectCLSID();
    }

    void RegisterColumn(const GUID& fmtid, DWORD pid, PCWSTR wszTitle, UINT cChars, DWORD fmt = LVCFMT_LEFT, DWORD csFlags = SHCOLSTATE_TYPE_STR)
    {
        SHCOLUMNID columnid;

        columnid.pid   = pid;
        columnid.fmtid = fmtid;

        SHCOLUMNINFO columninfo;

        columninfo.scid = columnid;

        ATLASSERT(wcslen(wszTitle) < MAX_COLUMN_NAME_LEN && "wszTitle is too long");
        wcscpy(columninfo.wszTitle, wszTitle);
        columninfo.cChars = cChars;
        columninfo.fmt = fmt;
        columninfo.csFlags = csFlags | SHCOLSTATE_EXTENDED | SHCOLSTATE_SECONDARYUI;
        // Note: VT_LPSTR/VT_BSTR works ok. Other types seems to have issues with sorting.
        columninfo.vt = VT_BSTR;
        columninfo.wszDescription[0] = 0; // not used by the shell.

        m_columninfos.push_back(columninfo);
        m_columnidtoindex[columnid] = static_cast<unsigned int>(m_columninfos.size() - 1);
    }

    void RegisterColumn(const GUID& fmtid, DWORD pid, UINT nResourceID, UINT cChars, DWORD fmt = LVCFMT_LEFT, DWORD csFlags = SHCOLSTATE_TYPE_STR)
    {
        RegisterColumn(pid, fmtid, LoadStringW(nResourceID), cChars, fmt, csFlags);
    }

    void RegisterColumn(PCWSTR wszTitle, UINT cChars, DWORD fmt = LVCFMT_LEFT, DWORD csFlags = SHCOLSTATE_TYPE_STR)
    {
        RegisterColumn(GetStandardFormatIdentifier(), static_cast<DWORD>(m_columninfos.size()),
            wszTitle, cChars, fmt, csFlags);
    }

    void RegisterColumn(UINT nResourceID, UINT cChars, DWORD fmt = LVCFMT_LEFT, DWORD csFlags = SHCOLSTATE_TYPE_STR)
    {
        RegisterColumn(LoadStringW(nResourceID), cChars, fmt, csFlags);
    }

    void RegisterExtension(CStringW strExtension)
    {
        m_extensions.push_back(strExtension.MakeLower());
    }

    bool IsSupportedItem(const SHCOLUMNDATA& scd) const
    {
        if (scd.dwFileAttributes & static_cast<const T*>(this)->GetFileAttributeMask())
            return false;

        // Check file extension.
        return IsSupportedFileType(scd.pwszExt);
    }

    bool IsSupportedFileType(CStringW strExtension) const
    {
        strExtension.MakeLower();
        return std::find(m_extensions.begin(), m_extensions.end(), strExtension) != m_extensions.end();
    }

    unsigned int GetIndex(const SHCOLUMNID& scid) const
    {
        auto it = m_columnidtoindex.find(scid);
        RaiseExceptionIf(it == m_columnidtoindex.end());
        return it->second;
    }

    DWORD GetFileAttributeMask() const
    {
        return FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_OFFLINE;
    }

private:
    const std::vector<CString>* GetAndCacheFileInfo(const CStringW& strFilename, PCWSTR wszFile, bool bFlushCache)
    {
        const std::vector<CString>* pCachedInfo;

        if (bFlushCache)
        {
            pCachedInfo = nullptr;
        }
        else
        {
            pCachedInfo = FindInCache(strFilename);
        }

        if (!pCachedInfo)
        {
            // New file info must be stored in the cache.
            vector<CString> strColumnInfos;

            // Note: GetAllColumnInfo must be implemented by the derived class.
            static_cast<T*>(this)->GetAllColumnInfo(CString(CW2CT(wszFile)), strColumnInfos);

            ATLASSERT(strColumnInfos.size() == m_columninfos.size() && "Missing info");

            pCachedInfo = InsertInCache(strFilename, strColumnInfos);
        }

        return InsertInLastUsedCache(strFilename, pCachedInfo);
    }

    const std::vector<CString>* FindInCache(const CStringW& strFilename) const
    {
        auto it = m_mapCacheInfo.find(strFilename);
        return it == m_mapCacheInfo.end() ? nullptr : &(*it).second;
    }

    const std::vector<CString>* InsertInCache(const CStringW& strFilename, const std::vector<CString>& strColumnInfos)
    {
        return &(m_mapCacheInfo[strFilename] = strColumnInfos);
    }

    const std::vector<CString>* InsertInLastUsedCache(const CStringW& strFilename, const std::vector<CString>* pCachedInfo)
    {
        m_strCachedFilename = strFilename;
        m_pCachedInfo = pCachedInfo;

        return m_pCachedInfo;
    }

    const std::vector<CString>* FindInLastUsedCache(PCWSTR wszFilename) const
    {
        if (!m_pCachedInfo)
            return nullptr; // last used cache is empty.

        if (m_strCachedFilename != wszFilename)
            return nullptr; // last used cache was for a different file.

        return m_pCachedInfo;
    }

    // Member variables.
    bool                        m_bInitialized;
    std::vector<SHCOLUMNINFO>   m_columninfos;
    std::vector<CStringW>       m_extensions;
    mapColumnIdToIndex          m_columnidtoindex;

    CStringW                    m_strCachedFilename;
    const std::vector<CString>* m_pCachedInfo;
    mapCacheInfo                m_mapCacheInfo;
    SHCOLUMNINFO                m_emptyColumnInfo;

    CDesktopBugWorkAround<t_bEnableDesktopBugWorkAround> m_desktopbugworkaround;
};


} // namespace MSF
