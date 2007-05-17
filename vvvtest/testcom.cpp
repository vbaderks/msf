//
// (C) Copyright by Victor Derks <vba64@xs4all.nl>
//
// See README.TXT for the details of the software licence.
//
#include "stdafx.h"

#include "testcom.h"

//#include "../include/infotipptr.h"
//#include "../include/util.h"
#include "../include/macros.h"
#include "../vvvsample/infotipclsid.h"


//CString GetInprocServer32()
//{

//}


void CallDllFunction(const CString& strDllname, const char* pszFunction)
{
	typedef HRESULT (STDAPICALLTYPE* LPFNDLL)();

	HINSTANCE hLib = LoadLibrary(strDllname);
	RaiseExceptionIf(hLib == NULL);

	try
	{
		LPFNDLL lpDllEntryPoint;

		lpDllEntryPoint = (LPFNDLL)GetProcAddress(hLib, pszFunction);
		RaiseExceptionIf(lpDllEntryPoint == NULL);

		HRESULT hr = (*lpDllEntryPoint)();
		RaiseExceptionIf(FAILED(hr));
	}
	catch (const _com_error&)
	{
		ATLVERIFY(FreeLibrary(hLib));
		throw;
	}
}


void RegisterServer(const CString& strDllname)
{
	CallDllFunction(strDllname, "DllRegisterServer");
}


void UnregisterServer(const CString& strDllname)
{
	CallDllFunction(strDllname, "DllUnregisterServer");
}


void TestCOM::RegisterAndUnregister()
{
	// TODO
	// Get regserver

	// Register
	// Unregister
	// Unregister
	// Register
}
