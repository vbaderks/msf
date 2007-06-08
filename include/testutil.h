//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


// Purpose: this file contains small unit test helper functions. 

#include "macros.h"


namespace MSF
{

inline void RaiseExceptionIfImplemented(HRESULT hr)
{
	if (hr != E_NOTIMPL)
		RaiseException(hr);
}

} // end MSF namespace.
