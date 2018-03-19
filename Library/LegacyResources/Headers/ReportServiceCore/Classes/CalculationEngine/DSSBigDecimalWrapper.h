//
//  DSSBigDecimalWrapper.h
//  ReportServiceCore
//
//  Created by Jia Zhang on 10/11/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#ifndef __ReportServiceCore__DSSBigDecimalWrapper__
#define __ReportServiceCore__DSSBigDecimalWrapper__

#include <iostream>
#import "../../../Common/MsiCOM/MsiCOM/Msi_oaidl.h"
#import "DSSBigDecimal.h"

// Oct/12/2013, The purpose to define DSSBigDecimalWrapper is to support big decimail.
//      When a VARIANT object represent a big decimal, its members will be as below:
//          (1) VARIANT::pdispVal point to object of DSSBigDecimalWrapper.
//          (2) VARIANT::vt is VT_DISPATCH
class DSSBigDecimalWrapper: public IDispatch
{
private:
    DSSBigDecimalWrapper(MDataType::DSSBigDecimal *ipBigDecimal);
    
public:
    virtual ~DSSBigDecimalWrapper();
    
    static DSSBigDecimalWrapper *CreateInstance(MDataType::DSSBigDecimal *ipBigDecimal);
    
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(/* [in] */ REFIID riid, /* [iid_is][out] */ void ** ppvObject);
    virtual ULONG STDMETHODCALLTYPE AddRef( void);
    virtual ULONG STDMETHODCALLTYPE Release( void);
    virtual /* [local] */ HRESULT STDMETHODCALLTYPE Invoke(
                                                           /* [in] */ DISPID dispIdMember,
                                                           /* [in] */ REFIID riid,
                                                           /* [in] */ LCID lcid,
                                                           /* [in] */ WORD wFlags,
                                                           /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
                                                           /* [out] */ VARIANT __RPC_FAR *pVarResult,
                                                           /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
                                                           /* [out] */ UINT __RPC_FAR *puArgErr);
    
    const MDataType::DSSBigDecimal * getBigDecimal() const;
    
private:
    int mRefCnt; // Oct/12/2013, there is no lock when accessing this reference count, which should be enhanced in the future.
    MDataType::DSSBigDecimal *mpBigDecimal;
};

#endif /* defined(__ReportServiceCore__DSSBigDecimalWrapper__) */
