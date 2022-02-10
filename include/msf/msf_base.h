//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software license.
//
#pragma once


// Purpose: base include file for MSF header files. Includes most common header files.
// MSF supports the following compilers:
// _MSC_VER = 1910 : Visual Studio 2017

// _PREFAST_ is defined when the Visual Studio C\C++ compiler is executed with the /analyze switch, it activates the static analyzer.

// To support testing with the /Wall switch certain warning must be disabled when including
// Win32, C\C++ runtime or ATL header files.

// The Windows SDK supports improved type checking for ITEMIDs, enable it.
#ifndef STRICT_TYPED_ITEMIDS
#define STRICT_TYPED_ITEMIDS
#endif

// Win32 include files.
#include <shlobj.h>
#include <shobjidl.h>

// Note: comdef.h will create only smart pointers for defined interfaces. Include all required
//       Win32 header files before including comdef.h
#include <comdef.h>

// Shell Extensions are COM apartment threaded.
#ifndef _ATL_APARTMENT_THREADED
#define _ATL_APARTMENT_THREADED
#endif

// Standard ATL include files.
#include <atlbase.h>
#include <atlcom.h>
#include <atlstr.h>
#include <atlsnap.h>

// Core MSF include files
#include "macros.h"
