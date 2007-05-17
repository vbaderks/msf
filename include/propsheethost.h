//
// (C) Copyright by Victor Derks <vba64@xs4all.nl>
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "util.h"

#pragma comment(lib, "comctl32") // required to link DestroyPropertySheetPage.


namespace MSF
{

class CPropSheetHost
{
public:
	~CPropSheetHost() throw()
	{
		for_each(_propsheetpages, CDestroyPropertySheet());
	}


	void Add(HPROPSHEETPAGE hpropsheetpage)
	{
		_propsheetpages.push_back(hpropsheetpage);
	}


	static BOOL CALLBACK AddPage(HPROPSHEETPAGE hpropsheetpage, LPARAM lparam)
	{
		CPropSheetHost* ppropsheethost = reinterpret_cast<CPropSheetHost*>(lparam);

		ppropsheethost->Add(hpropsheetpage);

		return TRUE;
	}

private:
	class CDestroyPropertySheet
	{
	public:
		void operator()(HPROPSHEETPAGE hpropsheetpage) const throw()
		{
			ATLVERIFY(::DestroyPropertySheetPage(hpropsheetpage));
		}
	};

	std::vector<HPROPSHEETPAGE> _propsheetpages;
};

} // end MSF namespace
