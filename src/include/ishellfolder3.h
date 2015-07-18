//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"

// INTERFACE: IShellFolder3
// IShellFolder3 is an undocumented interface introduced in Windows Vista. The definition below is based on info from Geoff Chappell.

interface IFilterCondition;
interface IQueryUnit;
interface IItemFilter;
typedef enum tagFILTERIDLISTTYPE FILTERIDLISTTYPE;

interface IShellFolder3 : public IShellFolder2
{
public:
    virtual HRESULT STDMETHODCALLTYPE CreateFilteredIDList(IFilterCondition *, FILTERIDLISTTYPE, IPropertyStore *, PITEMID_CHILD *) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetFilteredIDListType(PCITEMID_CHILD, FILTERIDLISTTYPE *) = 0;
    virtual HRESULT STDMETHODCALLTYPE ModifyFilteredIDList(PCITEMID_CHILD, IFilterCondition *, PITEMID_CHILD *) = 0;
    virtual HRESULT STDMETHODCALLTYPE ReparentFilteredIDList(PCIDLIST_RELATIVE, PIDLIST_RELATIVE *) = 0;
    virtual HRESULT STDMETHODCALLTYPE CreateStackedIDList(PROPERTYKEY const &, PIDLIST_ABSOLUTE *) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetStackedKey(PROPERTYKEY *) = 0;
    virtual HRESULT STDMETHODCALLTYPE EnumObjectsEx(HWND, IBindCtx *, ULONG, IItemFilter *, IEnumIDList **) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetConditions(PROPERTYKEY const &, IQueryUnit *, REFIID, PVOID *) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetAutoListFlags(ULONG *) = 0;
};

struct __declspec(uuid("2EC06C64-1296-4F53-89E5-ECCE4EFC2189")) IShellFolder3;
