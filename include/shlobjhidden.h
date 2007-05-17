//
// (C) Copyright by Victor Derks <vba64@xs4all.nl>
//
// See README.TXT for the details of the software licence.
//
#pragma once


#ifndef __IPersistIDList_INTERFACE_DEFINED__
#define __IPersistIDList_INTERFACE_DEFINED__

EXTERN_C const IID IID_IPersistIDList;

MIDL_INTERFACE("1079acfc-29bd-11d3-8e0d-00c04f6837d5")
IPersistIDList : public IPersist
{
public:
    virtual HRESULT STDMETHODCALLTYPE SetIDList( 
        /* [in] */ __RPC__in PCIDLIST_ABSOLUTE pidl) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetIDList( 
        /* [out] */ __RPC__deref_out_opt PIDLIST_ABSOLUTE *ppidl) = 0;
    
    };
#endif
