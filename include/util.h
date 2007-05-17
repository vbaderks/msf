//
// (C) Copyright by Victor Derks <vba64@xs4all.nl>
//
// See README.TXT for the details of the software licence.
//
#pragma once


// Purpose: this file contains small helper functions. 
#include "msfbase.h"
#include <algorithm>
#include <shellapi.h>


namespace MSF
{

inline CString GetModuleDirectory()
{
	TCHAR sz[MAX_PATH];
	ATLVERIFY(::GetModuleFileName(GetModuleHandle(NULL), sz,  MAX_PATH));

	PathRemoveFileSpec(sz);

	return sz + CString(_T("\\"));
}


inline CStringW GetModuleDirectoryW()
{
	wchar_t wz[MAX_PATH];
	ATLVERIFY(::GetModuleFileNameW(GetModuleHandle(NULL), wz,  MAX_PATH));

	PathRemoveFileSpecW(wz);

	return wz + CStringW(L"\\");
}


inline CString GetSystemDirectory()
{
	TCHAR tsz[MAX_PATH];
	ATLVERIFY(::GetSystemDirectory(tsz,MAX_PATH));

	return tsz;
}


// Note: use ANSI version and convert self. Win 9x only has ANSI version.
inline CStringW GetFolderPath(int nFolder)
{
	TCHAR tszFolderPath[MAX_PATH];

	tszFolderPath[0] = 0;
	ATLVERIFY(SHGetSpecialFolderPath(NULL, tszFolderPath, nFolder, false));
	return CStringW(CT2W(tszFolderPath));
}


inline DWORD GetFileSize(const CString& strFile)
{
	HANDLE hFile = CreateFile(strFile, 0, 0, NULL, OPEN_EXISTING, 0, 0);
	RaiseExceptionIf(hFile == INVALID_HANDLE_VALUE);

	DWORD dwSize = ::GetFileSize(hFile, NULL);
	CloseHandle(hFile);
	RaiseExceptionIf(dwSize == INVALID_FILE_SIZE && GetLastError() != NO_ERROR);

	return dwSize;
}


// Purpose: 'StrCmp' for numeric values. Usefull for IShellFolder::CompareIDs
inline int IntCmp(int n1, int n2) throw()
{
	if (n1 < n2)
		return -1;

	if (n1 > n2)
		return 1;

	return 0;
}


inline int UIntCmp(unsigned int n1, unsigned int n2) throw()
{
	if (n1 < n2)
		return -1;

	if (n1 > n2)
		return 1;

	return 0;
}


inline CString FormatLastError(DWORD dwLastError)
{
	LPTSTR lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
	              FORMAT_MESSAGE_FROM_SYSTEM |
	              FORMAT_MESSAGE_IGNORE_INSERTS,
	              NULL,
	              dwLastError,
	              MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
	              (LPTSTR) &lpMsgBuf,
	              0,
	              NULL);

	CString str(lpMsgBuf);
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
		ATLASSERT(sizeof(CStartupInfo) == sizeof(STARTUPINFO) && "Helper should not add size!");

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
#ifdef _PREFAST_
#pragma warning(push)
#pragma warning(disable: 6335) // supress sa noise: leaking process handle
#endif

inline void CreateProcess(LPCTSTR szApplicationName, LPTSTR szCmdLine, LPCTSTR lpCurrentDirectory = NULL)
{
	CStartupInfo startupinfo;
	CProcessInformation process_information; 

	RaiseLastErrorExceptionIf(!::CreateProcess(szApplicationName, szCmdLine, 
		NULL, NULL, false, 0, NULL, lpCurrentDirectory, &startupinfo, &process_information));
}

#ifdef _PREFAST_
#pragma warning(pop)
#endif

// Purpose: helper function to retrieve string from registry. Function will return string and
//          and true if string could be obtained.
inline bool QueryRegKeyStringValue(CRegKey& regkey, LPCTSTR pszValueName, CString& strValue) throw()
{
	unsigned long ulLength;
	if (regkey.QueryStringValue(pszValueName, NULL, &ulLength) != ERROR_SUCCESS)
		return false;

	if (regkey.QueryStringValue(pszValueName, strValue.GetBuffer(ulLength), &ulLength) != ERROR_SUCCESS)
		return false;

	strValue.ReleaseBuffer();
	return true;
}


// Purpose: Helper for use cases to read optional registry keys.
inline CString QueryRegKeyStringValue(CRegKey& regkey, LPCTSTR pszValueName, const CString& strDefault = CString()) throw()
{
	CString strValue;
	if (QueryRegKeyStringValue(regkey, pszValueName, strValue))
	{
		return strValue;
	}
	else
	{
		return strDefault;
	}
}


inline void QueryMultiStringValue(CRegKey& regkey, LPCTSTR pszValueName, std::vector<CString>& rgStrings)
{
	rgStrings.clear();

	unsigned long ulLength;
	if (regkey.QueryMultiStringValue(pszValueName, NULL, &ulLength) != ERROR_SUCCESS)
		return;

	std::vector<wchar_t> buffer(ulLength);
	if (regkey.QueryMultiStringValue(pszValueName, &(buffer[0]), &ulLength) != ERROR_SUCCESS)
		return;

	for (unsigned int i = 0; i < buffer.size() && buffer[i] != NULL;
	     i += rgStrings.back().GetLength() + 1)
	{
		rgStrings.push_back(CString(&(buffer[i])));
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
		_T("Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\") + strApp,
		KEY_READ) != ERROR_SUCCESS)
		return CString();

	return QueryRegKeyStringValue(key, _T(""));
}


// Purpose: allocates memory that can be used in OLE clipboard transactions.
//          The SDK docs are very uncleary about GMEM_FIXED / GMEM_MOVEABLE.
//          The shell itself uses GMEM_FIXED, which is easier to use as there
//          is no need to lock/unlock (GMEM_FIXED is ofcourse still just virtual memory).
inline HGLOBAL GlobalAllocThrow(SIZE_T dwBytes, UINT uFlags = GMEM_FIXED)
{
	HGLOBAL hg = GlobalAlloc(uFlags, dwBytes);
	RaiseExceptionIf(hg == NULL, E_OUTOFMEMORY);
	return hg;
}


inline CLIPFORMAT RegisterCf(LPCTSTR lpszFormat)
{
	unsigned int n = RegisterClipboardFormat(lpszFormat);
	RaiseExceptionIf(n == 0);

	return static_cast<CLIPFORMAT>(n);
}


inline CString GetClipboardFormatName(UINT format)
{
	switch (format)
	{
	case 0:               return _T("Undefined");
	case CF_TEXT:         return _T("Text");
	case CF_BITMAP:       return _T("Bitmap");
	case CF_METAFILEPICT: return _T("Metafilepct");
	case CF_SYLK:         return _T("Sylk");
	case CF_DIF:          return _T("Dif");
	case CF_TIFF:         return _T("Tiff");
	case CF_OEMTEXT:      return _T("Oemtext");
	case CF_DIB:          return _T("Dib");
	case CF_PALETTE:      return _T("Palette");
	case CF_PENDATA:      return _T("Pendata");
	case CF_RIFF:         return _T("Riff");
	case CF_WAVE:         return _T("Wave");
	case CF_UNICODETEXT:  return _T("Unicodetext");
	case CF_ENHMETAFILE:  return _T("Enhmetafile");
	case CF_HDROP:        return _T("Hdrop");
	case CF_LOCALE:       return _T("Locale");
	default:
		{
			TCHAR szName[255];

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

		if (pDllGetVersion != NULL)
		{
			DLLVERSIONINFO dvi;

			ZeroMemory(&dvi, sizeof(dvi));
			dvi.cbSize = sizeof(dvi);

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


inline bool IsShell5OrHigher()
{
	return GetDllVersion(_T("shell32.dll")) >= MSF_PACKVERSION(5, 00);
}


// Source: Effective STL, by Scott Meyers. ISBN: 0-201-74968-9, Item 7, page 38
class CDeleteObject
{
public:
	template <typename T>
	void operator()(const T* p) const throw()
	{
		delete p;
	}
};


template <typename TContainer, typename TFunctor>
inline TFunctor for_each(TContainer container, TFunctor functor)
{
	return std::for_each(container.begin(), container.end(), functor);
}


inline int GetSystemImageListIndex(const TCHAR* pszPath)
{
	SHFILEINFO sfi;
	if (!SHGetFileInfo(pszPath, FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(sfi),
	                   SHGFI_USEFILEATTRIBUTES | SHGFI_ICON))
		return 0;

	// Only need the index: clean-up the icon.
	ATLVERIFY(DestroyIcon(sfi.hIcon));

	return sfi.iIcon;
}

} // end namespace.
