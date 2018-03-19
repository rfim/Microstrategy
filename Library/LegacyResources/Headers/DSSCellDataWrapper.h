//
//  DSSCellDataWrapper.h
//  ReportServiceCore
//
//  Created by Liao Taimin on 9/23/14.
//  Copyright (c) 2014 MicroStrategy, Inc. All rights reserved.
//

#ifndef ReportServiceCore_DSSCellDataWrapper_h
#define ReportServiceCore_DSSCellDataWrapper_h

#include <iostream>
#import "../../../Common/MsiCOM/MsiCOM/Msi_oaidl.h"
#import "DSSCellFmtData.h"

// 2014/09/23, The purpose to define DSSCellDataWrapper is to support cell format data.
//      When a VARIANT object represent a cell format data, its members will be as below:
//          (1) VARIANT::pdispVal point to object of DSSCellDataWrapper.
//          (2) VARIANT::vt is VT_DISPATCH
class DSSCellDataWrapper: public IDispatch
{
private:
    DSSCellDataWrapper(MDataType::DSSCellFmtData *ipCellFmtData);
    
public:
    virtual ~DSSCellDataWrapper();
    
    static DSSCellDataWrapper *CreateInstance(MDataType::DSSCellFmtData *ipCellFmtData);
    
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
    
    MDataType::DSSCellFmtData * getCellFormatData();
    
private:
    int mRefCnt; // Oct/12/2013, there is no lock when accessing this reference count, which should be enhanced in the future.
    MDataType::DSSCellFmtData *mpCellFmtData;
};

#endif
