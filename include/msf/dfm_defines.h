//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include <cstdint>

// Undocumented DefFolderMenu callback notifications
// Discovered by Maksym Schipka, Axel Sommerfeldt, Henk Devos and myself.
// Almost all values are now defined in the Window SDK (except the 2 below)
constexpr uint32_t DFM_CREATE  = 3;
constexpr uint32_t DFM_DESTROY = 4; // Fired during the destruction of the menu (seen on XP).
