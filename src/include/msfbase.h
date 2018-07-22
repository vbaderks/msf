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

#pragma warning(push)
#pragma warning(disable: 4191) // 'type cast' : unsafe conversion from 'FARPROC' to 'PFN' (VS2017)
#pragma warning(disable: 4201) // nonstandard extension used : nameless struct / union (VS2017)
#pragma warning(disable: 4668) // is not defined a preprocessor macro, replacing it with 0. (VS2017)
#pragma warning(disable: 4768) // __declspec attributes before linkage specification are ignored (VS2017)
#pragma warning(disable: 4946) // reinterpret_cast used between related classes  (VS2017)
#pragma warning(disable: 4986) // exception specification does not match previous declaration (VS2017)
#pragma warning(disable: 4987) // nonstandard extension used : 'throw (...) (VS2017)
#pragma warning(disable: 5038) // data member '' will be initialized after data member '' (2017)

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

// Standard ATL include files.
#include <atlbase.h>
#include <atlcom.h>
#include <atlstr.h>
#include <atlsnap.h>

#pragma warning(pop)

// Core MSF include files
#include "macros.h"
