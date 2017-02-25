//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


// Purpose: this file contains small helper functions. 
#include "msfbase.h"
#include <shellapi.h>


namespace MSF
{

inline ATL::CStringW GetModuleDirectoryW()
{
    wchar_t wz[MAX_PATH];
    ATLVERIFY(::GetModuleFileNameW(GetModuleHandle(nullptr), wz, MAX_PATH));

    PathRemoveFileSpecW(wz);

    return wz + ATL::CStringW(L"\\");
}


inline ATL::CString GetSystemDirectory()
{
    wchar_t tsz[MAX_PATH];
    if (!::GetSystemDirectory(tsz, MAX_PATH))
        throw _com_error(HRESULT_FROM_WIN32(GetLastError()));
    return tsz;
}


// Note: use ANSI version and convert self. Win 9x only has ANSI version.
inline ATL::CStringW GetFolderPath(int nFolder)
{
    wchar_t tszFolderPath[MAX_PATH];

    tszFolderPath[0] = 0;
    ATLVERIFY(SHGetSpecialFolderPath(nullptr, tszFolderPath, nFolder, false));
    return ATL::CStringW(ATL::CT2W(tszFolderPath));
}


inline DWORD GetFileSize(const ATL::CString& strFile)
{
    auto hFile = CreateFile(strFile, 0, 0, nullptr, OPEN_EXISTING, 0, nullptr);
    RaiseExceptionIf(hFile == INVALID_HANDLE_VALUE);

    DWORD dwSize = ::GetFileSize(hFile, nullptr);
    CloseHandle(hFile);
    RaiseExceptionIf(dwSize == INVALID_FILE_SIZE && GetLastError() != NO_ERROR);

    return dwSize;
}


// Purpose: 'StrCmp' for numeric values. Usefull for IShellFolder::CompareIDs
inline int IntCmp(int n1, int n2) noexcept
{
    if (n1 < n2)
        return -1;

    if (n1 > n2)
        return 1;

    return 0;
}


inline int UIntCmp(unsigned int n1, unsigned int n2) noexcept
{
    if (n1 < n2)
        return -1;

    if (n1 > n2)
        return 1;

    return 0;
}


inline ATL::CString FormatLastError(DWORD dwLastError)
{
    LPTSTR lpMsgBuf;
    if (!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                  FORMAT_MESSAGE_FROM_SYSTEM |
                  FORMAT_MESSAGE_IGNORE_INSERTS,
                  nullptr,
                  dwLastError,
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                  reinterpret_cast<LPTSTR>(&lpMsgBuf),
                  0,
                  nullptr))
        throw _com_error(HRESULT_FROM_WIN32(GetLastError()));

    ATL::CString str(lpMsgBuf);
    LocalFree(lpMsgBuf);

    return str;
}


#ifdef ISOLATION_AWARE_ENABLED

// Purpose: Helper function to enable visual Windows XP styles.
inline void IsolationAwareDllMain(DWORD dwReason)
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        // Note1: IsolationawareInit will leak a HINSTANCE LoadLibrary handle.
        // Note2: IsolationAwareInit will fail on a OS that doesn't support it (win98, nt4, etc)
        IsolationAwareInit();
        break;

    case DLL_PROCESS_DETACH:
        IsolationAwareCleanup();
        break;

    default:
        break;
    }
}

#endif // ISOLATION_AWARE_ENABLED


// Small helper class that initializes WIN32-STARTUPINFO use by Win32 CreateProcess function.
class CStartupInfo : public STARTUPINFO
{
public:
    CStartupInfo()
    {
        static_assert(sizeof(CStartupInfo) == sizeof(STARTUPINFO), "Helper should not add size!");

        ZeroMemory(static_cast<STARTUPINFO*>(this), sizeof(STARTUPINFO));
        cb = sizeof(STARTUPINFO);
    }
};


// Small helper class that initializes and cleans PROCESS_INFORMATION (used by CreateProcess)
class CProcessInformation : public PROCESS_INFORMATION
{
public:
    CProcessInformation()
    {
        hProcess = INVALID_HANDLE_VALUE;
        hThread  = INVALID_HANDLE_VALUE;
    }


    ~CProcessInformation()
    {
        ATLVERIFY(CloseHandle(hProcess));
        ATLVERIFY(CloseHandle(hThread));
    }
};


// Purpose: 'short' version of Win32 function CreateProcess. 
//          Usefull for shell extensions that just need a quick way to start apps.
ATLPREFAST_SUPPRESS(6335) // supress sa noise: leaking process handle
inline void CreateProcess(LPCTSTR szApplicationName, LPTSTR szCmdLine, LPCTSTR lpCurrentDirectory = nullptr)
{
    CStartupInfo startupinfo;
    CProcessInformation process_information; 

    RaiseLastErrorExceptionIf(!::CreateProcess(szApplicationName, szCmdLine, 
        nullptr, nullptr, false, 0, nullptr, lpCurrentDirectory, &startupinfo, &process_information));
}
ATLPREFAST_UNSUPPRESS()


// Purpose: helper function to retrieve string from registry. Function will return string and
//          and true if string could be obtained.
inline bool QueryRegKeyStringValue(ATL::CRegKey& regkey, LPCTSTR pszValueName, ATL::CString& strValue) noexcept
{
    unsigned long ulLength = 0;
    if (regkey.QueryStringValue(pszValueName, nullptr, &ulLength) != ERROR_MORE_DATA)
        return false;

    if (regkey.QueryStringValue(pszValueName, strValue.GetBuffer(static_cast<int>(ulLength)), &ulLength) != ERROR_SUCCESS)
        return false;

    strValue.ReleaseBuffer();
    return true;
}


// Purpose: Helper for use cases to read optional registry keys.
inline ATL::CString QueryRegKeyStringValue(ATL::CRegKey& regkey, LPCTSTR pszValueName, const ATL::CString& strDefault = ATL::CString()) noexcept
{
    ATL::CString strValue;
    if (QueryRegKeyStringValue(regkey, pszValueName, strValue))
    {
        return strValue;
    }

    return strDefault;
}


inline void QueryMultiStringValue(CRegKey& regkey, LPCTSTR pszValueName, std::vector<ATL::CString>& rgStrings)
{
    rgStrings.clear();

    unsigned long ulLength = 0;
    if (regkey.QueryMultiStringValue(pszValueName, nullptr, &ulLength) != ERROR_MORE_DATA)
        return;

    std::vector<wchar_t> buffer(ulLength);
    if (regkey.QueryMultiStringValue(pszValueName, buffer.data(), &ulLength) != ERROR_SUCCESS)
        return;

    for (size_t i = 0; i < buffer.size() && buffer[i];
         i += rgStrings.back().GetLength() + 1)
    {
        rgStrings.push_back(CString(&buffer[i]));
    }
}


// Purpose: The App Path registry entry is used by the shell to find apps 
// (.exe's) that are not in the PATH. The user can start these apps by 
// using the 'run' menu.
// Shell extensions can use these entries to detect installed software.
//
inline CString GetAppPath(const CString& strApp)
{
    CRegKey key;

    if (key.Open(HKEY_LOCAL_MACHINE,
        L"Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\" + strApp,
        KEY_READ) != ERROR_SUCCESS)
        return CString();

    return QueryRegKeyStringValue(key, L"");
}


/// <summary>Allocates memory that can be used in OLE clipboard transactions.</summary>
/// <remarks>
/// The SDK docs are very unclear about GMEM_FIXED / GMEM_MOVEABLE.
/// The shell itself uses GMEM_FIXED, which is easier to use as there
/// is no need to lock/unlock (GMEM_FIXED is of course still just movable virtual memory).
/// </remarks>
inline HGLOBAL GlobalAllocThrow(SIZE_T dwBytes, UINT uFlags = GMEM_FIXED)
{
    auto hg = GlobalAlloc(uFlags, dwBytes);
    RaiseExceptionIf(!hg, E_OUTOFMEMORY);
    return hg;
}


inline CLIPFORMAT RegisterCf(LPCTSTR lpszFormat)
{
    unsigned int n = RegisterClipboardFormat(lpszFormat);
    RaiseExceptionIf(n == 0);

    return static_cast<CLIPFORMAT>(n);
}


inline ATL::CString GetClipboardFormatName(UINT format)
{
    switch (format)
    {
    case 0:               return L"Undefined";
    case CF_TEXT:         return L"Text";
    case CF_BITMAP:       return L"Bitmap";
    case CF_METAFILEPICT: return L"Metafilepct";
    case CF_SYLK:         return L"Sylk";
    case CF_DIF:          return L"Dif";
    case CF_TIFF:         return L"Tiff";
    case CF_OEMTEXT:      return L"Oemtext";
    case CF_DIB:          return L"Dib";
    case CF_PALETTE:      return L"Palette";
    case CF_PENDATA:      return L"Pendata";
    case CF_RIFF:         return L"Riff";
    case CF_WAVE:         return L"Wave";
    case CF_UNICODETEXT:  return L"Unicodetext";
    case CF_ENHMETAFILE:  return L"Enhmetafile";
    case CF_HDROP:        return L"Hdrop";
    case CF_LOCALE:       return L"Locale";
    default:
        {
            wchar_t szName[255];

            ATLVERIFY(::GetClipboardFormatName(format, szName, MSF_ARRAY_SIZE(szName)));
            return szName;
        }
    }
}


inline IDataObjectPtr OleGetClipboard()
{
    IDataObjectPtr dataobject;

    RaiseExceptionIfFailed(::OleGetClipboard(&dataobject));

    return dataobject;
}


inline DWORD MSF_PACKVERSION(DWORD major, DWORD minor)
{
    return static_cast<DWORD>(MAKELONG(minor, major));
}


#pragma warning(push)
#pragma warning(disable: 4191) // unsafe conversion from FARPROC (DLLGETVERSIONPROC is defined different then FARPROC)

inline DWORD GetDllVersion(LPCTSTR lpszDllName)
{
    DWORD dwVersion = 0;

    HINSTANCE hinstDll = LoadLibrary(lpszDllName);
    if (hinstDll)
    {
        DLLGETVERSIONPROC pDllGetVersion = 
            reinterpret_cast<DLLGETVERSIONPROC>(GetProcAddress(hinstDll, "DllGetVersion"));

        // Because some DLLs might not implement this function, you
        // must test for it explicitly. Depending on the particular 
        // DLL, the lack of a DllGetVersion function can be a useful
        // indicator of the version.
        if (pDllGetVersion)
        {
            DLLVERSIONINFO dvi;

            ZeroMemory(&dvi, sizeof dvi);
            dvi.cbSize = sizeof dvi;

            HRESULT hr = (*pDllGetVersion)(&dvi);
            if (SUCCEEDED(hr))
            {
                dwVersion = MSF_PACKVERSION(dvi.dwMajorVersion, dvi.dwMinorVersion);
            }
        }

        FreeLibrary(hinstDll);
    }

    return dwVersion;
}

#pragma warning(pop)


/// <summary>Determines if running on Shell 6.0 or higher.</summary>
/// <remarks>
/// Windows XP is the first version to use the 6.0 shell version.
/// </remarks>
inline bool IsShell6OrHigher()
{
    return GetDllVersion(L"shell32.dll") >= MSF_PACKVERSION(6, 0);
}


inline int GetSystemImageListIndex(const wchar_t* pszPath)
{
    SHFILEINFO sfi = {nullptr};
    if (!SHGetFileInfo(pszPath, FILE_ATTRIBUTE_NORMAL, &sfi, sizeof sfi,
                       SHGFI_USEFILEATTRIBUTES | SHGFI_ICON))
        return 0;

    // Only need the index: clean-up the icon.
    __analysis_assume(sfi.hIcon);
    ATLVERIFY(DestroyIcon(sfi.hIcon));

    return sfi.iIcon;
}

} // end namespace.
