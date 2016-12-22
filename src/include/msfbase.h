//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


// Purpose: base include file for msf header files. Includes most common header files.
// MSF supports the following compilers:
// _MSC_VER = 1900 : Visual Studio 2015
// _MSC_VER = 1910 : Visual Studio 2017

// _PREFAST_ is defined when the Visual Studio C\C++ compiler is executed with the /analyze switch, it activates the static analyzer.

// To support testing with the /Wall switch certain warning must be disabled when including
// Win32, C\C++ runtime or ATL header files.

#pragma warning(push)
#pragma warning(disable: 4191) // 'type cast' : unsafe conversion from 'FARPROC' to 'PFN' (VS2015)
#pragma warning(disable: 4625) // '...': copy constructor was implicitly defined as deleted (VS2015)
#pragma warning(disable: 4668) // is not defined a preprocessor macro, replacing it with 0. (VS2015)
#pragma warning(disable: 4917) // a GUID can only be defines to a class, interface, etc. (VS2015)
#pragma warning(disable: 4987) // nonstandard extension used : 'throw (...) (VS2015)
#pragma warning(disable: 5027) // '...': move assignment operator was implicitly defined as deleted (VS2015)

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

// Core MSF include files
#include "shlobjhidden.h"
#include "macros.h"
#include "catchhandler.h"
