//
//  DSSSimpleOperatorGeneric.cpp
//  ReportServiceCore
//
//  Created by Yang Zhuo on 7/25/11.
//  Copyright 2011 Microstrategy. All rights reserved.
//

#include "DSSSimpleOperatorGeneric.h"
#include "DSSFunctionUtility.h"

DSSSimpleOperatorGeneric::DSSSimpleOperatorGeneric()
{
    //zhyang, 25/07/2011, What is the client side dssfunctionUnknown
    //    mFunctionIndex = DssFuncUnknown;
    mpbResult = NULL;
    mpResultFlag = NULL;
}

DSSSimpleOperatorGeneric::~DSSSimpleOperatorGeneric()
{
    Clear();
}

int DSSSimpleOperatorGeneric::setProperty(int Index, CComVariant *ipVal)
{
    return E_NOTIMPL;
}

/*
int DSSSimpleOperatorGeneric::Init(EnumDSSFunction Index)
{
    assert(DssFunctionAnd == Index || DssFunctionOr == Index || DssFunctionNot == Index);
    if(DssFunctionAnd != Index || DssFunctionOr != Index || DssFunctionNot != Index)
    {
        return E_FAIL;
    }
    mFunctionIndex = Index;
    return S_OK;
}*/

int DSSSimpleOperatorGeneric::Calculate()
{
    int hr = S_OK;
    
    mpbResult = new bool [mnSize];
	if (!mpbResult)
		return E_OUTOFMEMORY;
    
	mpResultFlag = new DSSDataFlag [mnSize];
	if (!mpResultFlag)
		return E_OUTOFMEMORY;
    
	
	if (mFunctionIndex == DssFuncNot)
	{
		DSSDataFlag* pFlag = mpFlagList.front();
		bool* pValue = mpValueList.front();
        
		for (Int32 i = 0; i < mnSize; i ++)
		{
			if (pFlag[i] == DssDataOk){
				mpbResult[i] = ! pValue[i];
				mpResultFlag[i] = DssDataOk;
			} else {
				mpbResult[i] = false;
				mpResultFlag[i] = DssDataNull;
			}
            
		}
	}
	else
	{
		for (Int32 i = 0; i < mnSize; i ++)
		{
			std::list<bool*>::iterator lValueIterator = mpValueList.begin();
			std::list<DSSDataFlag*>::iterator lFlagIterator = mpFlagList.begin();
			bool lValue = (*lValueIterator)[i];
			DSSDataFlag lFlag = (*lFlagIterator)[i];
			++lValueIterator;
			++lFlagIterator;
			for (; lValueIterator != mpValueList.end(); ++lValueIterator, ++lFlagIterator )
			{
				bool lNextValue = (*lValueIterator)[i];
				DSSDataFlag lNextFlag  = (*lFlagIterator)[i];
				if ( mFunctionIndex == DssFuncAnd)
				{
					if (lFlag == DssDataOk &&  lNextFlag == DssDataOk)
					{
                        lValue = lValue && lNextValue;
					}
					else if ( (lFlag != DssDataOk && lNextFlag != DssDataOk) ||
                             (lFlag == DssDataOk && lValue) ||
                             (lNextFlag == DssDataOk && lNextValue) )
					{
						lValue = false;
						lFlag = DssDataNull;
					}
					else
					{
						lValue = false;
						lFlag = DssDataOk;
					}
				}
				else
				{
					if (lFlag == DssDataOk &&  lNextFlag == DssDataOk)
					{
                        lValue = lValue || lNextValue;
					}
					else if ( (lFlag != DssDataOk && lNextFlag != DssDataOk) ||
                             (lFlag == DssDataOk && !lValue)  ||
                             (lNextFlag == DssDataOk && !lNextValue))
					{
						lValue = false;
						lFlag = DssDataNull;
					}
					else
					{
						lValue = true;
						lFlag = DssDataOk;
					}
                    
				}
				
				
			}
            
			assert(lFlagIterator == mpFlagList.end());
			mpbResult[i]=lValue;
			mpResultFlag[i]=lFlag;
		}
        
	}
    
    return hr;
}

int DSSSimpleOperatorGeneric::GetOutput(int nSize, int *nDataSize, double *pData, DSSDataFlag *pFlag)
{
    if(!nDataSize || !pData || !pFlag)
    {
        return E_POINTER;
    }
    if(nSize < mnSize)
    {
        return E_FAIL;
    }
    for(Int32 i = 0; i < mnSize; ++i)
    {
        pData[i] = mpbResult[i];
        pFlag[i] = mpResultFlag[i];
    }
    *nDataSize = mnSize;
    return S_OK;
}

int DSSSimpleOperatorGeneric::GetOutputType(EnumDSSParameterType *pType)
{
    if(!pType)
    {
        return E_POINTER;
    }
    
    *pType = DssParameterVector;
    return S_OK;
}

int DSSSimpleOperatorGeneric::Clear()
{
    for(std::list<bool *>::iterator it = mpValueList.begin(); it != mpValueList.end(); ++it)
    {
        delete[] *it;
    }
    for(std::list<DSSDataFlag*>::iterator it = mpFlagList.begin(); it != mpFlagList.end(); ++it)
    {
        delete[] *it;
    }
    
    mpValueList.clear();
    mpFlagList.clear();
    delete [] mpbResult;
    mpbResult = NULL;
    delete mpResultFlag;
    mpResultFlag = NULL;
    mnSize = 0;
    
    return S_OK;
}

int DSSSimpleOperatorGeneric::SetInputParam(int nParamIndex, int nSize, double *pData, DSSDataFlag *pFlag)
{
    if(nSize < 1)
    {
        return E_INVALIDARG;
    }
    if(!pData || !pFlag)
    {
        return E_POINTER;
    }
    
    if(1 == nParamIndex)
    {
        mnSize = nSize;
    }
    else if(mnSize < nSize)
    {
        return E_INVALIDARG;
    }
    
    bool *pValue = new bool[mnSize];
    if(!pValue)
    {
        return E_OUT_OF_MEMORY;
    }
    DSSDataFlag *pFlag2 = new DSSDataFlag[mnSize];
    if(!pFlag2)
    {
        return E_OUT_OF_MEMORY;
    }
    
    for(Int32 i = 0; i < mnSize; ++i)
    {
        pValue[i] = static_cast<bool> (pData[i]);
        pFlag2[i] = pFlag[i];
    }
    mpValueList.push_back(pValue);
    mpFlagList.push_back(pFlag2);
    return S_OK;
}

int DSSSimpleOperatorGeneric::GetInputParamType(int nParamIndex, EnumDSSParameterType *pType)
{
    if(!pType)
    {
        return E_POINTER;
    }
    *pType = DssParameterVector;
    return S_OK;
}

int DSSSimpleOperatorGeneric::IsNullProcessable(bool *pCanProcessNull)
{
    if(!pCanProcessNull)
    {
        return E_POINTER;
    }
    *pCanProcessNull = true;
    return S_OK;
}

int DSSSimpleOperatorGeneric::DirectCalculate(int nParam, int *ipSize, Int32** ipOffsets, void **ipData, double *ipDivider,
                                              DSSDataFlag **ipFlags, VARIANT_BOOL *ipAllDataValid, 
                                              EnumDSSDataType *iType, vector<int> *ipvDataByGroup, 
                                              int nResSize, double *opData, DSSDataFlag *opFlag, 
                                              VARIANT_BOOL *opAllDataValid, EnumNullCheckingOption iNullCheckingOption)
{
    int hr = S_OK;
    
    assert(nParam > 0);
    Int32 lSize = ipSize[0];
    if(ipvDataByGroup != NULL)
    {
        lSize = ipvDataByGroup->size();
    }
    
    DSSDataFlag *lFlags = ipFlags[0];
    void *lpData = ipData[0];
    EnumDSSDataType lType = iType[0];
    
    Int32 i = 0;
    Int32 lRow = 0;
    
    if(DssFuncNot == mFunctionIndex)
    {
        assert(1 == nParam);
        for(i = 0; i < lSize; ++i)
        {
            if(NULL != ipvDataByGroup)
            {
                lRow = (*ipvDataByGroup)[i];
            }
            else
            {
                lRow = i;
            }
            
            if(DssDataOk == lFlags[lRow])
            {
                opData[lRow] = double(!GetNumericData(lpData, lRow, lType));
                opFlag[lRow] = DssDataOk;
            }
            else
            {
                opData[lRow] = double(false);
                opFlag[lRow] = DssDataNull;
            }
        }
    }
    else
    {
        for(i = 0; i < lSize; ++i)
        {
            if(NULL != ipvDataByGroup)
            {
                lRow = (*ipvDataByGroup)[i];
            }
            else
            {
                lRow = i;
            }
            
            bool lValue = (bool)GetNumericData(lpData, lRow, lType);
            DSSDataFlag lFlag = lFlags[lRow];
            for(Int32 j = 1; j < nParam; ++j)
            {
                bool lNextValue = (bool)GetNumericData(ipData[j], lRow, iType[j]);
                DSSDataFlag lNextFlag = ipFlags[j][lRow];
                if(DssFuncAnd == mFunctionIndex)
                {
                    if(DssDataOk == lFlag && DssDataOk == lNextFlag)
                    {
                        lValue = lValue && lNextValue;
                    }
                    else if ( (DssDataOk != lFlag && DssDataOk != lNextFlag) ||
                             (DssDataOk == lFlag && lValue) ||
                             (DssDataOk == lNextFlag && lNextValue) )
					{
						lValue = false;
						lFlag = DssDataNull;
					}
					else
					{
						lValue = false;
						lFlag = DssDataOk;
					}
                }
                else
                {
                    if(DssDataOk == lFlag && DssDataOk == lNextFlag)
                    {
                        lValue = lValue || lNextValue;
                    }
                    else if ( (DssDataOk != lFlag && DssDataOk != lNextFlag) ||
                             (DssDataOk == lFlag && !lValue) ||
                             (DssDataOk == lNextFlag && !lNextValue) )
					{
						lValue = false;
						lFlag = DssDataNull;
					}
					else
					{
						lValue = true;
						lFlag = DssDataOk;
					}
                }
            }
            opData[lRow] = lValue;
            opFlag[lRow] = lFlag;
        }
    }
    
    return hr;
}

bool DSSSimpleOperatorGeneric::SupportDirectCalculate()
{
    return true;
}