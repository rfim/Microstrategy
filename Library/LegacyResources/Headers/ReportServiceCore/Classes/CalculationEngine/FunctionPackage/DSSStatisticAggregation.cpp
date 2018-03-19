//
//  CDSSStatisticAggregation.cpp
//  ReportServiceCore
//
//  Created by Yang Zhuo on 6/27/11.
//  Copyright 2011 Microstrategy. All rights reserved.
// 

#include "DSSStatisticAggregation.h"
#include "math.h"
#include "DSSFunctionUtility.h"
#import "CellDataHelper.h"

DSSStatisticAggregation::DSSStatisticAggregation()
{
    
}

DSSStatisticAggregation::~DSSStatisticAggregation()
{
    
}

int DSSStatisticAggregation::Calculate(Int32 nSize, double *pVec, double *opResult, DSSDataFlag *pFlag)
{
    enum MathError error = MathNoError;
    
    if(nSize < 1)
        return E_INVALIDARG;
    

    CHECK_PTR(pVec);
	CHECK_PTR(opResult);
	CHECK_PTR(pFlag);
    
    *pFlag = DssDataOk;
    
    switch (mFunctionIndex) {
        case DssFuncStdev:
            if(!Variance(nSize, pVec, opResult))
            {
                *pFlag = DssDataInvalid;
                return S_OK;
            }
            *opResult = sqrt(*opResult);
            break;
            
        case DssFuncStdevP:
            if(!VarianceP(nSize, pVec, opResult))
            {
                *pFlag = DssDataInvalid;
                return S_OK;
            }
            *opResult = sqrt(*opResult);
            break;
            
        case DssFuncVar:
            if(!Variance(nSize, pVec, opResult))
            {
                *pFlag = DssDataInvalid;
                return S_OK;
            }
            break;
            
        case DssFuncVarP:
            if(!VarianceP(nSize, pVec, opResult))
            {
                *pFlag = DssDataInvalid;
                return S_OK;
            }
            break;
            
        case DssFuncGeomean:
            if(Geomean(nSize, pVec, opResult))
            {
                *pFlag = DssDataInvalid;
                return S_OK;
            }
            break;
        default:
            return E_FAIL;
    }
    
    CheckValue(&error, *opResult);
    
	if(error != MathNoError)
	{
		*opResult = 0;
		*pFlag = DssDataInvalid;
	}
    
    return S_OK;
    
}

int DSSStatisticAggregation::Init(EnumSimpleOperatorType iOperatorType)
{
    mFunctionIndex = iOperatorType;
    return S_OK;
}

int DSSStatisticAggregation::GetOutputType(EnumDSSDataType InputType, EnumDSSDataType* pOutputType)
{
    if(!pOutputType)
    {
        return E_POINTER;
    }
    
    // Only support cell formatting data for now
	*pOutputType = DssDataTypeCellFormatData;
    
	return S_OK;
}

bool DSSStatisticAggregation::SupportDirectCalculate()
{
    return true;
}

int DSSStatisticAggregation::DirectCalculate(Int32 iSize,
                                   Int32 * ipGroupLinkList,
                                   Int32 * ipBlockAddress,
                                   void* ipData, double iDivider,
                                   EnumDSSDataType iDataType,
                                   DSSDataFlag* ipDataFlags,
                                   bool iAllDataValid,
                                   EnumNullCheckingOption iNullCheckingOption,
                                   double* opResult,
                                   DSSDataFlag* opDataFlag)
{
    enum MathError error = MathNoError;
    
    if(iSize < 1)
        return E_INVALIDARG;
    
    DSSDataFlag lFlag = DssDataOk;
    bool lfAllSkipCheckNull = true;
    Int32 offset = 0;
	double d1 = 0.0;
	double lM1 = 0.0;
	double lSS1 = 0.0;
    
    switch (mFunctionIndex) {
        case DssFuncStdev:
            hDirectCaluclateVariance(iSize, ipGroupLinkList, ipBlockAddress, ipData,
                                     iDataType, ipDataFlags, iNullCheckingOption, 
                                     d1, lM1, lSS1, offset, lFlag, lfAllSkipCheckNull);
            if(S_OK == hCheckDirectCalculateFlag(opDataFlag, lfAllSkipCheckNull, lFlag, offset, 2))
            {
                *opResult = sqrt(lSS1 / (offset - 1) );
                *opDataFlag = lFlag;
            }
            break;
        case DssFuncStdevP:
            hDirectCaluclateVariance(iSize, ipGroupLinkList, ipBlockAddress, ipData,
                                     iDataType, ipDataFlags, iNullCheckingOption, 
                                     d1, lM1, lSS1, offset, lFlag, lfAllSkipCheckNull);
            if(S_OK == hCheckDirectCalculateFlag(opDataFlag, lfAllSkipCheckNull, lFlag, offset, 1))
            {
                *opResult = sqrt(lSS1 / (offset) );
                *opDataFlag = lFlag;
            }
            break;
        case DssFuncVar:
            hDirectCaluclateVariance(iSize, ipGroupLinkList, ipBlockAddress, ipData,
                                     iDataType, ipDataFlags, iNullCheckingOption, 
                                     d1, lM1, lSS1, offset, lFlag, lfAllSkipCheckNull);
            if(S_OK == hCheckDirectCalculateFlag(opDataFlag, lfAllSkipCheckNull, lFlag, offset, 2))
            {
                *opResult = lSS1 / (offset - 1) ;
                *opDataFlag = lFlag;
            }
            break;
        case DssFuncVarP:
            hDirectCaluclateVariance(iSize, ipGroupLinkList, ipBlockAddress, ipData,
                                     iDataType, ipDataFlags, iNullCheckingOption, 
                                     d1, lM1, lSS1, offset, lFlag, lfAllSkipCheckNull);
            if(S_OK == hCheckDirectCalculateFlag(opDataFlag, lfAllSkipCheckNull, lFlag, offset, 1))
            {
                *opResult = lSS1 / offset;
                *opDataFlag = lFlag;
            }
            break;
        case DssFuncGeomean:
        {
            double lnSum = 0;
            double lData = 0;
            int lnCount = 0;

            if(ipBlockAddress == NULL)
            {
                DSSDataFlag lFlag = DssDataOk;
                Int32 i = 0;
                for(i = 0; i < iSize; ++i)
                {
                    lFlag = ipDataFlags[ipGroupLinkList[i]];
                    //!!!!
                    //if(lFlag == DssDataIgnoreCheckNull)
                    //    continue;
                    
                    lfAllSkipCheckNull = false;
                    
                    if(lFlag != DssDataOk)
                    {
                        if(DssNumericErrorTreatAs == iNullCheckingOption)
                        {
                            lData = 0;
                        }
                        else
                        {
                            continue;
                        }
                    }
                    else
                    {
                        lData = GetNumericData(ipData, ipGroupLinkList[i], iDataType);
                    }
                    
                    if(lData > 0)
                    {
                        lnSum += log(lData);
                    }
                    else
                    {
                        *opDataFlag = DssDataInvalid;
                        return S_OK;
                    }
                    ++lnCount;
                }
            }
            else
            {
                Int32 *lpBlock = ipGroupLinkList;
                Int32 i = 0;
				for (i = iSize; i > 0; i--)
				{			
					Int32 *lpStart = &ipBlockAddress[*lpBlock];
					Int32 lStart = *lpStart;
					Int32 lBlockSize = *(lpStart + 1) - lStart;
                    
					for(Int32 j = 0; j < lBlockSize; j++)	// jxie, 2011-1-14, TQMS 451086
					{
						lFlag = ipDataFlags[lStart];
				
                        //!!!!
                        //	if (lFlag == DssDataIgnoreCheckNull)
				        //	continue;
                        
						lfAllSkipCheckNull = false;
                        
						if (lFlag != DssDataOk)
						{
							if (iNullCheckingOption == DssNumericErrorTreatAs)
								lData = 0;
							else
								continue;
						}
						else
							lData = GetNumericData(ipData, lStart++, iDataType);
                        
						if(lData > 0)
							lnSum += log(lData);
						else
						{
							// failed
							*opDataFlag = DssDataInvalid;
							return S_OK;
						}
						lnCount++;
					}
					lpBlock++;
				}
            }
            
            if (lfAllSkipCheckNull)
			{
				*opResult = 0;
				//!!!!!!
                //*opDataFlag = DssDataIgnoreCheckNull;
				return S_OK;
			}
			
			if (lnCount == 0)
			{
				*opResult = 0;
				*opDataFlag = DssDataInvalid;
				return S_OK;
			}
			
			*opResult = exp(lnSum / lnCount);
			*opDataFlag = DssDataOk;
        }
            break;
        default:
            break;
    }
    CheckValue(&error, *opResult);
    
	if(error != MathNoError)
	{
		*opResult = 0;
		*opDataFlag = DssDataInvalid;
	}
	
	if (iDivider > 1)
		*opResult /= iDivider;
    
	return S_OK;
}


void DSSStatisticAggregation::hDirectCaluclateVariance(Int32 iSize, 
                                                       Int32* ipGroupStart, 
                                                       Int32* ipBlockAddress, 
                                                       void* ipData, 
                                                       EnumDSSDataType iDataType,
                                                       DSSDataFlag* ipDataFlag, 
                                                       EnumNullCheckingOption iNullCheckingOption,
                                                       double &d1, 
                                                       double &lM1, 
                                                       double &lSS1, 
                                                       Int32 &offset, 
                                                       DSSDataFlag &oFlag, 
                                                       bool& ofAllSkipCheckNull)
{
    DSSDataFlag lFlag = DssDataOk;
    bool lfAllSkipNull = true;
    if(NULL == ipBlockAddress)
    {
        int i = 0;
        for(i =0; i < iSize; ++i)
        {
            lFlag = ipDataFlag[ipGroupStart[i]];
         //   if(lFlag == DssdataignoreCheckNull)
         //       continue;
        
            lfAllSkipNull = false;
            if(lFlag != DssDataOk)
            {
                if(iNullCheckingOption == DssNumericErrorTreatAs)
                {
                    lM1 = 0;
                }
                else
                {
                    continue;
                }
            }
            else
            {
                // jimzhang sep/27/2011 TQMS:506256  
                //lM1 = GetNumericData(ipData, i, iDataType);
                lM1 = GetNumericData(ipData, ipGroupStart[i], iDataType);
            }
            offset++;
            break;
        }
        
        if(i == iSize)
        {
            oFlag = DssDataInvalid;
        }
        else if(i < (iSize - 1) )
        {
            VarianceSegment(iSize - 1 - i, ipData, iDataType, ipDataFlag, iNullCheckingOption,
							&(ipGroupStart[i+1]), d1, lM1, lSS1, offset);
        }
    }
    else
    {
        Int32 *lpBlock = ipGroupStart;
        int i = 0;
		for (i = iSize; i > 0; i--)
		{			
			Int32 *lpStart = &ipBlockAddress[*lpBlock];
			Int32 lStart = *lpStart;
			Int32 lBlockSize = *(lpStart + 1) - lStart;
            
			if (0 == offset)
			{
				// jxie, 2010-12-13
				// find the first true data
                int j = 0;
				for (j = 0; j < lBlockSize; j++)
				{
					lFlag = ipDataFlag[lStart];
                //!!!! Should not be deleted!!!
				//	if (lFlag == DssDataIgnoreCheckNull)
				//		continue;
                    
					lfAllSkipNull = false;
					if (lFlag != DssDataOk)
					{
						if (iNullCheckingOption == DssNumericErrorTreatAs)
							lM1 = 0;
						else
							continue;
					}
					else
						lM1 = GetNumericData(ipData, lStart++, iDataType);
                    
					offset++;
					break;
				}
				if (j > lBlockSize - 2)
					continue;
                
				// if it comes here, there must be at least 1 valid element, no need to get the checknull flag
				VarianceSegment(lBlockSize - 1 - j, ipData, iDataType, ipDataFlag, iNullCheckingOption,
                                lStart, d1, lM1, lSS1, offset);
			}
			else
				VarianceSegment(lBlockSize, ipData, iDataType, ipDataFlag, iNullCheckingOption,
                                lStart, d1, lM1, lSS1, offset);
            
			lpBlock++;
		}
    }
    ofAllSkipCheckNull = lfAllSkipNull;
}

int DSSStatisticAggregation::hCheckDirectCalculateFlag(DSSDataFlag* opDataFlag, bool& ifAllSkipCheckNull, 
                                                   DSSDataFlag iFlag, int& offset, int offsetMin)
{
    if(ifAllSkipCheckNull)
    {
        //*opDataFlag = DssdataIgnoreCheckNull;
        return E_FAIL;
    }
    if (iFlag != DssDataOk && offset < offsetMin) {
        *opDataFlag = DssDataInvalid;
        return E_FAIL;
    }
    return S_OK;
}
//tliao, 822148, 2014/09/23, support cell format data calculation
int DSSStatisticAggregation::CalculateCellData(Int32 nParam, CComVariant * pParam, CComVariant * pResult, DSSData_Flags * pFlag)
{
	int hr;
    
	if (nParam < 1)
		return E_FAIL;
    
	if (!pParam || !pResult || !pFlag)
		return E_POINTER;
    
	if ((pParam->vt & (VT_ARRAY | VT_VARIANT)) != (VT_ARRAY | VT_VARIANT))
		return E_FAIL;
    
	vector<double> lvValues(nParam);
	vector<EnumCellFormatDataTypes> lvTypes(nParam);
	vector<DSS_ID> lvFormatIDs(nParam);
    
	hr=CellDataHelper::SplitCellData(nParam, pParam, lvValues, lvTypes, lvFormatIDs);
	CHECK_HR;
    
	*pFlag=DssDataOk;
    
	double lResult;
	hr=Calculate(nParam, &lvValues[0], &lResult, pFlag);
	CHECK_HR;
    
	EnumCellFormatDataTypes lResultType=CellDataHelper::CalculateCellDataType(nParam,&lvTypes[0]);
	DSS_ID lResultFormatID=CellDataHelper::CalculateCellDataFormatID(nParam,&lvFormatIDs[0]);
    
	hr=CellDataHelper::CreateVariantFromCellData(lResult, lResultType, lResultFormatID, pResult);
	CHECK_HR;
    
	return S_OK;	
    
}


