//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


namespace MSF
{

enum class StandardImagelistIndex
{
    DocumentBlank  = 0, // document (blank page) (not associated)
    DocumentFilled = 1, // document (with stuff on the page)
    Application     = 2, // application (exe, com, bat)
    FolderPlain    = 3, // folder (plain)
    FolderOpen     = 4  // folder (open)
};

} // end of MSF namespace
