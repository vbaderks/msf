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

    ContextMenuCommand(const ContextMenuCommand& other) = delete;
    ContextMenuCommand(ContextMenuCommand&& other) = delete;
    ContextMenuCommand& operator=(const ContextMenuCommand&) = delete;
    ContextMenuCommand& operator=(ContextMenuCommand&& other) = delete;

    virtual void operator()(const CMINVOKECOMMANDINFO*, const std::vector<std::wstring>& /*filenames*/) = 0;

protected:
    ContextMenuCommand() = default;
};

} // end namespace msf
