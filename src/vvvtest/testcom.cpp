//
// (C) Copyright by Victor Derks
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
	if (hLib == NULL)
		RaiseException();

	try
	{
		LPFNDLL lpDllEntryPoint;

		lpDllEntryPoint = (LPFNDLL)GetProcAddress(hLib, pszFunction);
		if (lpDllEntryPoint == NULL)
			RaiseException();

		HRESULT hr = (*lpDllEntryPoint)();
		if (FAILED(hr))
			RaiseException();
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
