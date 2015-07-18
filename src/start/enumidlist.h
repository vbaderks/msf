#pragma once

#include "../include/enumidlistimpl.h"

class ATL_NO_VTABLE CEnumIDList :
    public CComObjectRootEx<CComSingleThreadModel>,
    public IEnumIDListImpl<CEnumIDList>
{
public:
    DECLARE_NOT_AGGREGATABLE(CEnumIDList)

    BEGIN_COM_MAP(CEnumIDList)
        COM_INTERFACE_ENTRY(IEnumIDList)
    END_COM_MAP()

    static CComPtr<IEnumIDList> CreateInstance(/* initialization parameters */)
    {
        CComObject<CEnumIDList>* pinstance;
        RaiseExceptionIfFailed(
            CComObject<CEnumIDList>::CreateInstance(&pinstance));

        CComPtr<IEnumIDList> renumidlist(pinstance);

        pinstance->Initialize(/* initialization parameters*/);

        return renumidlist;
    }

    ITEMIDLIST* GetNextItem()
    {
        // TODO: implement iterator.
        return NULL;
    }

#if _MSC_VER > 1300 // VC++ cannot access private from static members.
private:
#endif

    void Initialize(/* initialization parameters */)
    {
    }
};
