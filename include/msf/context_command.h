//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msf_base.h"

namespace msf {

class ContextMenuCommand
{
public:
    virtual ~ContextMenuCommand() = default;

    ContextMenuCommand(const ContextMenuCommand&) = delete;
    ContextMenuCommand(ContextMenuCommand&&) = delete;
    ContextMenuCommand& operator=(const ContextMenuCommand&) = delete;
    ContextMenuCommand& operator=(ContextMenuCommand&&) = delete;

    virtual void operator()(const CMINVOKECOMMANDINFO*, const std::vector<std::wstring>& /*filenames*/) = 0;

protected:
    ContextMenuCommand() = default;
};

} // end namespace msf
