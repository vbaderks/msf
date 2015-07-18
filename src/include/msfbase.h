//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


// Purpose: base include file for msf header files. Includes most common header files.
// MSF supports the following compilers:
// _MSC_VER = 1800 : Visual Studio 2013
// _MSC_VER = 1900 : Visual Studio 2015

// _PREFAST_ is defined when the Visual Studio C\C++ compiler is executed with the /analyze switch, it activates the static analyzer.

// To support testing with the /Wall switch certain warning must be disabled when including
// Win32, C\C++ runtime or ATL header files.

#pragma warning(push)
#pragma warning(disable: 4100) // unreferenced formal parameter
#pragma warning(disable: 4127) // conditional expression is constant
#pragma warning(disable: 4191) // 'type cast' : unsafe conversion from 'FARPROC' to 'PFN'
#pragma warning(disable: 4263) // 'function' : member function does not override any base class virtual member function
#pragma warning(disable: 4264) // auto generated after C4263
#pragma warning(disable: 4201) // nonstandard extension used : nameless struct/union
#pragma warning(disable: 4512) // assignment operator could not be generated
#pragma warning(disable: 4619) // #pragma warning : there is no warning number 'xxx'
#pragma warning(disable: 4668) // not a preprocessor macro
#pragma warning(disable: 4917) // a GUID can only be defines to a class, interface, etc.

// Standard C++ library
#include <vector>
#include <algorithm>

// Win32 include files.
#include <shlobj.h>
#include <shobjidl.h>

// Note: comdef.h will create only smartpointers for defined interfaces. Include all required
//       Win32 header files before including comdef.h
#include <comdef.h>

// Standard ATL include files.
#include <atlbase.h>
#include <atlcom.h>
#include <atlstr.h>
#include <atlsnap.h>

#pragma warning(pop)

// core MSF include files
#include "shlobjhidden.h"
#include "macros.h"
