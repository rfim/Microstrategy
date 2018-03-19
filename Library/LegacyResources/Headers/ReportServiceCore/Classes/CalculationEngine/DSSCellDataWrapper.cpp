//
//  DSSCellDataWrapper.cpp
//  ReportServiceCore
//
//  Created by Liao Taimin on 9/23/14.
//  Copyright (c) 2014 MicroStrategy, Inc. All rights reserved.
//

#include "DSSCellDataWrapper.h"
#include "DSSAEHelper.h"

DSSCellDataWrapper::DSSCellDataWrapper(MDataType::DSSCellFmtData *ipCellFmtData):
mpCellFmtData(ipCellFmtData), mRefCnt(0)
{
}

DSSCellDataWrapper::~DSSCellDataWrapper()
{
    --mRefCnt;
    if (mRefCnt <= 0)
    {
        if (mpCellFmtData)
        {
            delete mpCellFmtData;
            mpCellFmtData = NULL;
        }
    }
}

HRESULT STDMETHODCALLTYPE DSSCellDataWrapper::QueryInterface(/* [in] */ REFIID riid, /* [iid_is][out] */ void ** ppvObject)
{
    return E_NOTIMPL;
}

DSSCellDataWrapper * DSSCellDataWrapper::CreateInstance(MDataType::DSSCellFmtData *ipCellFmtData)
{
    if (! ipCellFmtData)
        return NULL;
    
    return new DSSCellDataWrapper(ipCellFmtData);
}

ULONG STDMETHODCALLTYPE DSSCellDataWrapper::AddRef(void)
{
    ++mRefCnt;
    return mRefCnt;
}

ULONG STDMETHODCALLTYPE DSSCellDataWrapper::Release(void)
{
    --mRefCnt;
    if (mRefCnt <= 0)
    {
        delete this;
        return 0;
    }
    return mRefCnt;
}

HRESULT STDMETHODCALLTYPE DSSCellDataWrapper::Invoke(/* [in] */ DISPID dispIdMember,
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

MDataType::DSSCellFmtData * DSSCellDataWrapper::getCellFormatData()
{
    return mpCellFmtData;
}