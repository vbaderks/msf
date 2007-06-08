//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#undef  INTERFACE
#define INTERFACE IShellFolderSearchableCallback
DECLARE_INTERFACE_IID_(IShellFolderSearchableCallback, IUnknown, "F98D8294-2BBC-11d2-8DBD-0000F87A556C")
{
	// *** IUnknown methods ***
	STDMETHOD(QueryInterface) (THIS_ REFIID riid, __out void **ppv) PURE;
	STDMETHOD_(ULONG,AddRef)  (THIS) PURE;
	STDMETHOD_(ULONG,Release) (THIS) PURE;

	// *** IShellFolderSearchableCallback Methods ***
	STDMETHOD(RunBegin)(THIS_ DWORD dwReserved) PURE;
	STDMETHOD(RunEnd)(THIS_ DWORD dwReserved) PURE;
};
