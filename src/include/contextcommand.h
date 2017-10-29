//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "msfbase.h"


namespace msf
{

class ContextMenuCommand
{
public:
    virtual ~ContextMenuCommand() = default;
    virtual void operator()(const CMINVOKECOMMANDINFO* /* pici */, const std::vector<std::wstring>& /* filenames */) = 0;
};

} // end namespace msf
