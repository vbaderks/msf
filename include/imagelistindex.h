//
// (C) Copyright by Victor Derks <vba64@xs4all.nl>
//
// See README.TXT for the details of the software licence.
//
#pragma once


namespace MSF
{

enum EStandardImagelistIndex
{
	STD_IMAGELIST_INDEX_DOCUMENT_BLANK  = 0, // document (blank page) (not associated)
	STD_IMAGELIST_INDEX_DOCUMENT_FILLED = 1, // document (with stuff on the page)
	STD_IMAGELIST_INDEX_APPLICATION     = 2, // application (exe, com, bat)
	STD_IMAGELIST_INDEX_FOLDER_PLAIN    = 3, // folder (plain)
	STD_IMAGELIST_INDEX_FOLDER_OPEN     = 4  // folder (open)
};

} // end of MSF namespace
