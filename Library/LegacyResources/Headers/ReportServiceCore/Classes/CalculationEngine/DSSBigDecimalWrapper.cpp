//
//  DSSBigDecimalWrapper.cpp
//  ReportServiceCore
//
//  Created by Jia Zhang on 10/11/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#include "DSSBigDecimalWrapper.h"
#include "DSSAEHelper.h"

DSSBigDecimalWrapper::DSSBigDecimalWrapper(MDataType::DSSBigDecimal *ipBigDecimal):
mpBigDecimal(ipBigDecimal), mRefCnt(0)
{
}

DSSBigDecimalWrapper::~DSSBigDecimalWrapper()
{
    --mRefCnt;
    if (mRefCnt <= 0)
    {
        if (mpBigDecimal)
        {
            delete mpBigDecimal;
            mpBigDecimal = NULL;
        }
    }
}

HRESULT STDMETHODCALLTYPE DSSBigDecimalWrapper::QueryInterface(/* [in] */ REFIID riid, /* [iid_is][out] */ void ** ppvObject)
{
    return E_NOTIMPL;
}

DSSBigDecimalWrapper * DSSBigDecimalWrapper::CreateInstance(MDataType::DSSBigDecimal *ipBigDecimal)
{
    if (! ipBigDecimal)
        return NULL;
    
    return new DSSBigDecimalWrapper(ipBigDecimal);
}

ULONG STDMETHODCALLTYPE DSSBigDecimalWrapper::AddRef(void)
{
    ++mRefCnt;
    return mRefCnt;
}

ULONG STDMETHODCALLTYPE DSSBigDecimalWrapper::Release(void)
{
    --mRefCnt;
    if (mRefCnt <= 0)
    {
        delete this;
        return 0;
    } 
    return mRefCnt;
}

HRESULT STDMETHODCALLTYPE DSSBigDecimalWrapper::Invoke(/* [in] */ DISPID dispIdMember,
                                                       /* [in] */ REFIID riid,
                                                       /* [in] */ LCID lcid,
                                                       /* [in] */ WORD wFlags,
                                                       /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
                                                       /* [out] */ VARIANT __RPC_FAR *pVarResult,
                                                       /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
                                                       /* [out] */ UINT __RPC_FAR *puArgErr)
{
    return E_NOTIMPL;
}

const MDataType::DSSBigDecimal * DSSBigDecimalWrapper::getBigDecimal() const
{
    return mpBigDecimal;
}