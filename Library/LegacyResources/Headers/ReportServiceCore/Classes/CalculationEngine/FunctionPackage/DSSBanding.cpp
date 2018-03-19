//
//  DSSBanding.cpp
//  ReportServiceCore
//
//  Created by Yang Zhuo on 7/26/11.
//  Copyright 2011 Microstrategy. All rights reserved.
//

#include "DSSBanding.h"
#import "DSSFunctionUtility.h"

void CalculateBandsWithStepSize(Int32, double, double, double, double, double *, DSSDataFlag *);

DSSBanding::DSSBanding()

: mHasResidue(false)
{
}


DSSBanding::~DSSBanding()
{
}


int DSSBanding::Calculate(Int32 nParam, double *pParam, double *pResult, DSSDataFlag *pFlag)
{
	if(!pParam || !pResult || !pFlag) return E_POINTER;
    
	switch(mFunctionIndex)
	{
            // banding using step size
            // Function input should be: 
            //          Banding(Metric, StartAt, StopAt, StepSize)
            //			with property HasResidue  
        case DssFuncBanding:
            if(nParam != 4)
            {
                return E_FAIL;
            }
            
            CalculateBandsWithStepSize(0, pParam[0], pParam[1], pParam[2], pParam[3], pResult, pFlag);
            
            break;
            
            // Function input should be:
            //      BandingC(Metric, StartAt, StopAt, BandsCount)
            //		with property HasResidue
        case DssFuncBandingC:
            if(nParam != 4)
            {
                return E_FAIL;
            }
            
            double lStepSize;
            // BandsCount not 0
            if(pParam[3] != 0)
            {
                // calculate stepsize = (stopat - startat)/bandscount
                Int32 lNumBand = pParam[3];
                lStepSize = (pParam[2] - pParam[1]) / lNumBand;
                CalculateBandsWithStepSize(lNumBand, pParam[0], pParam[1], pParam[2], lStepSize, pResult, pFlag);
            }
            else
                *pFlag = DssDataInvalid;
            
            break;
            
            // Function input should be:
            //      BandingP(Metric, Boundary1, Boundary2, .....)
            //		with property HasResidue
        case DssFuncBandingP:
        {
            // expect 3 input parameters
            if(nParam < 3)
            {
                return E_FAIL;
            }
                          
            Int32 i = 0;
            // check whether the banding points are ascending sequence
            bool lAscend = true;
            for(i = 1; i < (nParam - 1); i++)
                lAscend = lAscend && DoubleLessEqual(pParam[i], pParam[i+1]);
            
            // banding points are ascending sequence
            if (lAscend)
            {
                *pResult = 0;
                for(i = 1; i < (nParam - 1); i++)
                {
                    // last upper boundary
                    if (i == (nParam - 2))
                    {
                        if(DoubleGreaterEqual(pParam[0], pParam[i]) && DoubleLessEqual(pParam[0], pParam[i+1]))
                            *pResult = i;
                    }
                    // inner boundary
                    else
                    {
                        if(DoubleGreaterEqual(pParam[0], pParam[i]) && DoubleLessThan(pParam[0], pParam[i+1]))
                            *pResult = i;
                    }
                }
                *pFlag = DssDataOk;
            }
            // not ascending sequence
            else
            {
                //check whether the banding points are descending sequence
                bool lDescend = true;
                for(i = 1; i < (nParam - 1); i++)
                    lDescend = lDescend && DoubleGreaterEqual(pParam[i], pParam[i+1]);
                
                // banding points are desending sequence
                if(lDescend)
                {
                    *pResult = 0;
                    for(i = 1; i < (nParam - 1); i++)
                    {
                        // last boundary
                        if (i == (nParam - 2))
                        {
                            if(DoubleLessEqual(pParam[0], pParam[i]) && DoubleGreaterEqual(pParam[0], pParam[i+1]))
                                *pResult = i;
                        }
                        // inner boundary
                        else
                        {
                            if(DoubleLessEqual(pParam[0], pParam[i]) && DoubleGreaterThan(pParam[0], pParam[i+1]))
                                *pResult = i;
                        }
                    }
                    *pFlag = DssDataOk;
                }
                // when banding points are not ascending sequence nor descending sequence
                // we can not calculate, just return invalid.
                else 
                    *pFlag = DssDataInvalid;
            }
        }
            break;
            
        default:
            return E_FAIL;
	}
    
    
	return S_OK;
}

int DSSBanding::setProperty(Int32 Index, CComVariant *pValue)
{
	if(!pValue) return E_POINTER;
	HRESULT hr;
    
	// convert to variant bool type
	hr = VariantChangeType(pValue, pValue, 0, VT_BOOL);
	if ( FAILED(hr) ) 
		return E_FAIL;
    
    
	// only one property for this function
	mHasResidue = pValue->boolVal ? true : false;
    
	return S_OK;
}

//int DSSBanding::Init(EnumSimpleOperatorType iOperatorType)
//{
//	mFunctionIndex = iOperatorType;
//	return S_OK;
//}


// Additional Declarations
//## begin CDSSBanding.declarations preserve=yes

// given the StartAt, StopAt and StepSize
// calculate the band for the given iValue
// |-> .... |-> .... | ..... |-> .. <-|
// Bands start from 1 	
// assign 0 for residues
void CalculateBandsWithStepSize(Int32 iNumBand, double iValue, double iStartAt, double iStopAt, double iStepSize,
                                double *opBand, DSSDataFlag *opFlag)
{
	if(iStepSize > 0)
	{
		// iStartAt should less than or equal to iStopAt
		if(iStartAt > iStopAt)
		{
			*opFlag = DssDataInvalid;
			return;
		}
        
		// Assign band 0 to all residues
		if(DoubleLessThan(iValue, iStartAt) || DoubleGreaterThan(iValue, iStopAt))
		{
			*opBand = 0;
			*opFlag = DssDataOk;
			return;
		}
	}
	else if(iStepSize < 0)
	{
		// iStartAt should greater than or equal to iStopAt
		if(iStartAt < iStopAt)
		{
			*opFlag = DssDataInvalid;
			return;
		}
        
		// Assign band 0 to all residues
		if(DoubleGreaterThan(iValue, iStartAt) || DoubleLessThan(iValue, iStopAt))
		{
			*opBand = 0;
			*opFlag = DssDataOk;
			return;
		}
	}
	// iStepSize == 0
	else 
	{
		*opFlag = DssDataInvalid;
		return;
	}
    
    
	Int32 lBand = 0;
	double lStartingValue, lEndingValue = iStartAt;
	while(1)
	{
		lBand++;
		lStartingValue = lEndingValue;
		lEndingValue += iStepSize;
        
		// ascending order
		if(iStepSize > 0)
		{
			if(DoubleLessThan(lEndingValue, iStopAt) && (lBand != iNumBand))
			{
				// if is BandStepsize, iNumBand is 0
				// if is BandCount, iNumBand is band count
				if(DoubleGreaterEqual(iValue, lStartingValue) && DoubleLessThan(iValue,lEndingValue) )
				{
					*opBand = lBand;
					*opFlag = DssDataOk;
                    
					return;
				}
                
                
			}
			else
			{
				*opBand = lBand;
				*opFlag = DssDataOk;
				
				return;
			}
            
		}
		// descending order
		else
		{
			if(DoubleGreaterThan(lEndingValue, iStopAt) && (lBand != iNumBand) )
			{
				// if is BandStepsize, iNumBand is 0
				// if is BandCount, iNumBand is band count
				if(DoubleLessEqual(iValue, lStartingValue) && DoubleGreaterThan(iValue, lEndingValue)  )
				{
					*opBand = lBand;
					*opFlag = DssDataOk;
                    
					return;
				}
			}
			else
			{
				*opBand = lBand;
				*opFlag = DssDataOk;
				
				return;
			}
		}
	}
    
    
	return;
	
}

//int DSSBanding::get_OperatorType(EnumSimpleOperatorType * opType)
//{
//	if (!opType) return E_POINTER;
//	*opType = mFunctionIndex;
//	return S_OK;
//}

// jxie, 2010-9-25
int DSSBanding::DirectCalculate(Int32 nParam, Int32 *ipSize, Int32** ipOffsets, void **ipData, double* ipDivider, 
                                DSSDataFlag **ipFlags, VARIANT_BOOL* ipAllDataValid, EnumDSSDataType *iType, 
                                DSSXTabBitVector *ipMask, 
                                Int32 nResSize, double *opData, DSSDataFlag *opFlag,
                                VARIANT_BOOL* opAllDataValid, EnumNullCheckingOption iNullCheckingOption)
{
	Int32 i = 0, j = 0;
 	AE_ASSERT(nParam > 0);
	Int32 lnSize = ipSize[0];
	if (lnSize <= 0)
		return S_OK;
    
	vector<double> lvData;
	bool lAscend = true, lDescend = true;
	*opAllDataValid = VARIANT_TRUE;
    
	for (i = 0; i < lnSize; i++)
		opFlag[i] = DssDataUnknown;
    
	switch(mFunctionIndex)
	{
            // banding using step size
            // Function input should be: 
            //          Banding(Metric, StartAt, StopAt, StepSize)
            //			with property HasResidue  
        case DssFuncBanding:
            
            // expect 4 input parameters
            if(nParam != 4)
            {
                return E_FAIL;
            }
            
            lvData.resize(nParam);
            for (i = 0; i < nParam; i++)
            {
                if (i > 0 && ipFlags[i][0] != DssDataOk)
                    return S_OK;
                int lRow = 0; //GetRow(ipOffsets[i],0);
                lvData[i] = GetNumericData(ipData[i], lRow, iType[i]);
            }
            
            for (i = 0; i < lnSize; i++)
            {
                if (ipMask && ipMask->Get(i) == false)
                    continue;
                
                for (j = 0; j < nParam; j++)
                {
                    if (ipSize[j] > 1)	// not const
                    {
                        if (ipFlags[j][i] == DssDataOk)
                        {
                            int lRow = i; //GetRow(ipOffsets[j],i);
                            lvData[j] = GetNumericData(ipData[j], lRow, iType[j]);
                        }
                        else if (iNullCheckingOption == DssNumericErrorTreatAs)
                            lvData[j] = 0;
                        else
                        {
                            opFlag[i] = DssDataInvalid;
                            *opAllDataValid = VARIANT_FALSE;
                            continue;
                        }
                    }
                }
                
                CalculateBandsWithStepSize(0, lvData[0], lvData[1], lvData[2], lvData[3], &(opData[i]), &(opFlag[i]) );
            }
            
            break;
            
            // Function input should be:
            //      BandingC(Metric, StartAt, StopAt, BandsCount)
            //		with property HasResidue
        case DssFuncBandingC:
            
            // expect 4 input parameters
            if(nParam != 4)
            {
                return E_FAIL;
            }
            
            double lStepSize;
            lvData.resize(nParam);
            for (i = 0; i < nParam; i++)
            {
                if (i > 0 && ipFlags[i][0] != DssDataOk)
                    return S_OK;
                int lRow = 0; //GetRow(ipOffsets[i],0);
                lvData[i] = GetNumericData(ipData[i], lRow, iType[i]);
            }
            
            for (i = 0; i < lnSize; i++)
            {
                if (ipMask && ipMask->Get(i) == false)
                    continue;
                
                for (j = 0; j < nParam; j++)
                {
                    if (ipSize[j] > 1)	// not const
                    {
                        if (ipFlags[j][i] == DssDataOk)
                        {
                            int lRow = i; //GetRow(ipOffsets[j],i);
                            lvData[j] = GetNumericData(ipData[j], lRow, iType[j]);
                        }
                        else if (iNullCheckingOption == DssNumericErrorTreatAs)
                            lvData[j] = 0;
                        else
                        {
                            opFlag[i] = DssDataInvalid;
                            *opAllDataValid = VARIANT_FALSE;
                            continue;
                        }
                    }
                }
                
                // BandsCount not 0
                if(lvData[3] != 0)
                {
                    // calculate stepsize = (stopat - startat)/bandscount
                    Int32 lNumBand = (Int32)lvData[3];
                    lStepSize = (lvData[2] - lvData[1]) / lNumBand;
                    CalculateBandsWithStepSize(lNumBand, lvData[0], lvData[1], lvData[2], lStepSize, &(opData[i]), &(opFlag[i]) );
                }
                else
                {
                    opFlag[i] = DssDataInvalid;
                    *opAllDataValid = VARIANT_FALSE;
                }
            }
            
            break;
            
            // Function input should be:
            //      BandingP(Metric, Boundary1, Boundary2, .....)
            //		with property HasResidue
        case DssFuncBandingP:
        {
            // expect 3 input parameters
            if(nParam < 3)
            {
                return E_FAIL;
            }
            
            lvData.resize(nParam);
            for (i = 0; i < nParam; i++)
            {
                if (i > 0 && ipFlags[i][0] != DssDataOk)
                    return S_OK;
                int lRow = 0; //GetRow(ipOffsets[i],0);
                lvData[i] = GetNumericData(ipData[i], lRow, iType[i]);
                
                if (i > 1)
                {
                    //check whether the banding points are ascending or descending sequence
                    lAscend = lAscend && DoubleLessEqual(lvData[i-1], lvData[i]);
                    lDescend = lDescend && DoubleGreaterEqual(lvData[i-1], lvData[i]);
                }
            }
            
            for (i = 0; i < lnSize; i++)
            {
                if (ipMask && ipMask->Get(i) == false)
                    continue;
                
                for (j = 0; j < nParam; j++)
                {
                    if (ipSize[j] > 1)	// not const
                    {
                        if (ipFlags[j][i] == DssDataOk)
                        {
                            int lRow = i; //GetRow(ipOffsets[j],i);
                            lvData[j] = GetNumericData(ipData[j], lRow, iType[j]);
                        }
                        else if (iNullCheckingOption == DssNumericErrorTreatAs)
                            lvData[j] = 0;
                        else
                        {
                            opFlag[i] = DssDataInvalid;
                            *opAllDataValid = VARIANT_FALSE;
                            continue;
                        }
                    }
                }
                
                // banding points are ascending sequence
                if (lAscend)
                {
                    opData[i] = 0;
                    for(j = 1; j < (nParam - 1); j++)
                    { 
                        // last upper boundary
                        if (j == (nParam - 2))
                        {
                            if(DoubleGreaterEqual(lvData[0], lvData[j]) && DoubleLessEqual(lvData[0], lvData[j+1]))
                                opData[i] = j;
                        }
                        // inner boundary
                        else
                        {
                            if(DoubleGreaterEqual(lvData[0], lvData[j]) && DoubleLessThan(lvData[0], lvData[j+1]))
                                opData[i] = j;
                        }
                    }
                    opFlag[i] = DssDataOk;
                }
                // banding points are desending sequence
                else if(lDescend)
                {
                    opData[i] = 0;
                    for(j = 1; j < (nParam - 1); j++)
                    {
                        // last boundary
                        if (j == (nParam - 2))
                        {
                            if(DoubleLessEqual(lvData[0], lvData[j]) && DoubleGreaterEqual(lvData[0], lvData[j+1]))
                                opData[i] = j;
                        }
                        // inner boundary
                        else
                        {
                            if(DoubleLessEqual(lvData[0], lvData[j]) && DoubleGreaterThan(lvData[0], lvData[j+1]))
                                opData[i] = j;
                        }
                    }
                    opFlag[i] = DssDataOk;
                }
                // when banding points are not ascending sequence nor descending sequence
                // we can not calculate, just return invalid.
                else 
                {
                    opFlag[i] = DssDataInvalid;
                    *opAllDataValid = VARIANT_FALSE;
                }
            }
            
            break;
        }
            
        default:
            return E_FAIL;
	}
    
	return S_OK;
}

bool DSSBanding::SupportDirectCalculate()
{    
	bool lbResult = false;
    
	switch (mFunctionIndex)
	{
        case DssFuncBanding:
        case DssFuncBandingC:
        case DssFuncBandingP:
            lbResult = true;
            break;
        default:
            break;
	}
	return lbResult;
}
