//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//

// Enabled improved type checking of the Win32 SDK header files.
#define STRICT

#define WINVER 0x0600 // Target Vista and up.

#define _ATL_ALL_WARNINGS // turns off ATL's hiding of some common and often safely ignored warning messages.

// To support testing with the /Wall switch certain warning must be disabled when including
// Win32, C\C++ runtime or ATL header files. Some warning must be kept disabled as templates will
// only generate the warning when they are actual used.
#pragma warning(disable: 4265) // class has virtual functions, but destructor is not virtual
#pragma warning(disable: 4505) // unreferenced local function has been removed.
#pragma warning(disable: 4514) // unreferenced inline function has been removed.
#pragma warning(disable: 4710) // function not inlined.
#pragma warning(disable: 4711) // function xxx is selected for automatic inline expansion.
#pragma warning(disable: 4820) // 'x' bytes padding added after member.

#include "../include/msfbase.h"

#pragma warning(push)

#include <tchar.h>
#include <atlctl.h>

#pragma warning(pop)

using namespace std;
using namespace msf;
