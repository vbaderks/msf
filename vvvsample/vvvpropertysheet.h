//
// (C) Copyright by Victor Derks <vba64@xs4all.nl>
//
// See README.TXT for the details of the software licence.
//


#include "../include/propertysheet.h"
#include "propertypageitem.h"


class CVVVPropertySheet : public CPropertySheet
{
public:
	CVVVPropertySheet(CVVVItem& item, IShellFolder* pshellfolder) : 
		CPropertySheet(item.GetDisplayName(), PSH_NOAPPLYNOW | PSH_PROPTITLE | PSH_NOCONTEXTHELP),
		_wEventId(0)
	{
		AddPage(CPropertyPageItem::CreateInstance(item, _wEventId, pshellfolder));
	}

	
	int DoModal(HWND hwnd, long& wEventId)
	{
		_wEventId = 0;
		int result = __super::DoModal(hwnd);

		wEventId = _wEventId;
		return result;
	}

private:

	CVVVPropertySheet(const CVVVPropertySheet&);            // not implemented by design
	CVVVPropertySheet& operator=(const CVVVPropertySheet&); // not implemented by design

	// Members variables
	long _wEventId;
};
