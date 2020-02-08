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
#pragma warning(push)
#pragma warning(disable : 4946) // reinterpret_cast used between related classes  (VS2019)
#include <shlobj.h>
#include <shobjidl.h>
#pragma warning(pop)


// Note: comdef.h will create only smart pointers for defined interfaces. Include all required
//       Win32 header files before including comdef.h
#pragma warning(push)
#pragma warning(disable : 4619) // #pragma warning : there is no warning number '5204'
#pragma warning(disable : 5204) // class has virtual functions, but its trivial destructor is not virtual;
#include <comdef.h>
#pragma warning(pop)

// Standard ATL include files.
#pragma warning(push)
#pragma warning(disable : 4201) // nonstandard extension used : nameless struct / union (VS2019)
#pragma warning(disable : 4619) // #pragma warning : there is no warning number '5204'
#pragma warning(disable : 4755) // Conversion rules for arithmetic operations in the comparison  mean that one branch cannot be executed in an inlined function. (VS2017)
#pragma warning(disable : 4986) // exception specification does not match previous declaration (VS2019)
#pragma warning(disable : 5204) // class has virtual functions, but its trivial destructor is not virtual;

// Shell Extensions are COM apartment threaded.
#ifndef _ATL_APARTMENT_THREADED
#define _ATL_APARTMENT_THREADED
#endif

#include <atlbase.h>
#include <atlcom.h>
#include <atlstr.h>
#include <atlsnap.h>

#pragma warning(pop)

// Core MSF include files
#include "macros.h"
