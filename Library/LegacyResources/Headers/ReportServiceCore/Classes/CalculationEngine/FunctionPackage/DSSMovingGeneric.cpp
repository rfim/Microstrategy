//
//  DSSMovingGeneric.cpp
//  ReportServiceCore
//
//  Created by Jia Zhang on 7/26/11.
//  Copyright 2011 MicroStrategy, Inc. All rights reserved.
//

#include "DSSMovingGeneric.h"

#import "DSSFunctionUtility.h"
#import "DSSStrongPtr.h"

#define UNINITIALIZED -1

DSSMovingGeneric::DSSMovingGeneric(): mDataSize(UNINITIALIZED), mpData(NULL), 
mpResult(NULL), mpDataFlag(NULL), mpResultFlag(NULL), mWinSize(UNINITIALIZED),
// jxie, 2010-12-16
mOldNum(0), mOldCount(0)
{
}

DSSMovingGeneric::~DSSMovingGeneric()
{
	delete [] mpData;
	delete [] mpResult;
	delete [] mpDataFlag;
	delete [] mpResultFlag;
}

int DSSMovingGeneric::setProperty (Int32 Index, CComVariant *pValue)
{
    return E_NOTIMPL;
}

int DSSMovingGeneric::Calculate ()
{
 	Int32 i = 0;
 
	if(mDataSize == UNINITIALIZED || mFunctionIndex == DssFuncUnknown)
    {
        return S_FALSE;
    }
    
	// check whether all the required input parameters have been set.
	if(!mpData || !mpDataFlag)
    {
        return S_FALSE;
    }
    
	// Allocate space for mpResult and mpFlag
	mpResult = new double[mDataSize];  
	if(!mpResult) return E_OUTOFMEMORY;
    
	mpResultFlag = new DSSDataFlag[mDataSize];
	if(!mpResultFlag) return E_OUTOFMEMORY;
    
	for (i = 0; i < mDataSize; i++)
        mpResultFlag[i] = DssDataUnknown;
        
	switch (mFunctionIndex)
	{
        case DssFuncMovingSum:
            CalculateMovingSum();
            break;
        case DssFuncMovingAvg:
            CalculateMovingAvg();
            break;
            /* These functions become non-numeric
             case DssFuncMovingMax:
             CalculateMovingMax();
             break;
             case DssFuncMovingMin:
             CalculateMovingMin();
             break;
             case DssFuncMovingCount:
             CalculateMovingCount();
             break;
             */
        case DssFuncMovingDifference:
            CalculateMovingDifference();
            break;
        case DssFuncMovingStdev:
            CalculateMovingStdev();
            break;
        case DssFuncMovingStdevP:
            CalculateMovingStdevP();
            break;
        default:
            return E_FAIL;
	}    
    
	return S_OK;
}

int DSSMovingGeneric::SetInputParam (Int32 nParamIndex, Int32 nSize, double *pData, DSSDataFlag*pFlag)
{    
	if((nParamIndex < 1) || (nParamIndex > 2)) return E_INVALIDARG;
    
	if(nSize < 1) return E_INVALIDARG;
	
	if(!pData) return E_POINTER;
    
	// save as an argument
	switch(nParamIndex)
	{
            // value list
        case 1:
            mDataSize = nSize;
            // allocat memory to record input data
            mpData = new double[nSize];
            if(!mpData) return E_OUTOFMEMORY;
            memmove(mpData, pData, nSize * sizeof(double));
            mpDataFlag = new DSSDataFlag[nSize];
            if(!mpDataFlag) return E_OUTOFMEMORY;
            memmove(mpDataFlag, pFlag, nSize * sizeof(DSSDataFlag));
            
            break;
            
            // window size
        case 2:
            mWinSize = *pData;
            break;            
            
        default:
            return S_FALSE;
	}
    
	return S_OK;
}

int DSSMovingGeneric::GetOutput (Int32 nSize, Int32 *nDataSize, double *pData, DSSDataFlag*pFlag)
{
	if(!nDataSize || !pData || !pFlag) return E_POINTER;
    
	// output buffer size shouldn't be smaller than mDataSize
	if(nSize < mDataSize) 
    {
        return S_FALSE;
    }
    
	// copy the result out
	Int32 i = 0;
	for(i = 0; i < mDataSize; i++)
	{
		pData[i] = mpResult[i];
		pFlag[i] = mpResultFlag[i];
	}
    
	*nDataSize = mDataSize;
    
	return S_OK;
}

int DSSMovingGeneric::GetOutputType (EnumDSSParameterType *pType)
{
 	if(!pType) return E_POINTER;
    
	*pType = DssParameterVector;
	
	return S_OK;
}

int DSSMovingGeneric::GetInputParamType (Int32 nParamIndex, EnumDSSParameterType *pType)
{
	if(!pType) return E_POINTER;
    
	switch (nParamIndex) {
        case 1:
            *pType = DssParameterVector;
            break;
        case 2:
            *pType = DssParameterScalar;
            break;
        default:
            return E_FAIL;
	}
    
	return S_OK;
}

int DSSMovingGeneric::IsNullProcessable (bool *pCanProcessNULL)
{
	if(!pCanProcessNULL) return E_POINTER;
	
	*pCanProcessNULL = true; // 05/14/2008 mwang, now all olap func can process Null
    
	return S_OK;
}

int DSSMovingGeneric::Clear ()
{
	delete [] mpData;
	delete [] mpResult;
	delete [] mpDataFlag;
	delete [] mpResultFlag;
    
	//lwang; 12/01/2008; 337727, reset pointer
	mpResult = NULL;
	mpResultFlag = NULL;
	mpData = NULL;
	mpDataFlag = NULL;
	mDataSize = UNINITIALIZED;
	mWinSize = UNINITIALIZED;
    
	// jxie, 2010-12-16
	mOldNum = 0;
	mOldCount = 0;
    
	return S_OK;
    //## end DSSMovingGeneric::Clear%37DCFBC200DA.body
}

int DSSMovingGeneric::SumBackward (Int32 iVal)
{
	Int32 i = 0,  iStart = 0 ,  lCount = 0 ;
    
	lCount = 0;
	if (iVal + 1 >= mWinSize)
		iStart = iVal - mWinSize + 1;
	else
		iStart = 0;
    
	mpResult[iVal] = 0;
	for (i = iStart; i <= iVal; i++)
	{
		if (mpDataFlag[i] == DssDataOk)
		{
			mpResult[iVal] += mpData[i];
			lCount ++;
		}
	}
    
	if (lCount > 0)
		mpResultFlag[iVal] = DssDataOk;
    
	return S_OK;
}

int DSSMovingGeneric::SumForward (Int32 iVal)
{
	Int32 i = 0,  iEnd = 0 ,  lCount = 0 ;
    
	lCount = 0;
    
	if (iVal - mWinSize - 1 < mDataSize - 1)
		iEnd = iVal - mWinSize - 1;
	else
		iEnd = mDataSize - 1;
    
	mpResult[iVal] = 0;
	for (i = iVal; i <= iEnd; i++)
	{
		if (mpDataFlag[i] == DssDataOk)
		{
			mpResult[iVal] += mpData[i];
			lCount ++;
		}
	}
    
	if (lCount > 0)
		mpResultFlag[iVal] = DssDataOk;
    
	return S_OK;
}

int DSSMovingGeneric::AvgBackward (Int32 iVal)
{
 	Int32 i = 0,  iStart = 0 ,  lCount = 0 ;
    
	lCount = 0;
	if (iVal + 1 >= mWinSize)
		iStart = iVal - mWinSize + 1;
	else
		iStart = 0;
    
	mpResult[iVal] = 0;
	for (i = iStart; i <= iVal; i++)
	{
		if (mpDataFlag[i] == DssDataOk)
		{
			mpResult[iVal] += mpData[i];
			lCount ++;
		}
	}
    
	if (lCount > 0)
	{
		mpResultFlag[iVal] = DssDataOk;
		mpResult[iVal] /= lCount;
	}
    
	return S_OK;
}

int DSSMovingGeneric::AvgForward (Int32 iVal)
{
	Int32 i = 0,  iEnd = 0 ,  lCount = 0 ;
    
	lCount = 0;
    
	if (iVal - mWinSize - 1 < mDataSize - 1)
		iEnd = iVal - mWinSize - 1;
	else
		iEnd = mDataSize - 1;
    
	mpResult[iVal] = 0;
	for (i = iVal; i <= iEnd; i++)
	{
		if (mpDataFlag[i] == DssDataOk)
		{
			mpResult[iVal] += mpData[i];
			lCount ++;
		}
	}
    
	if (lCount > 0)
	{
		mpResultFlag[iVal] = DssDataOk;
		mpResult[iVal] /= lCount;
	}
    
	return S_OK;
}

int DSSMovingGeneric::MaxBackward (Int32 iVal)
{
	Int32 i = 0,  iStart = 0 ;
    
	if (iVal + 1 >= mWinSize)
		iStart = iVal - mWinSize + 1;
	else
		iStart = 0;
    
	for (i = iStart; i <= iVal; i++)
	{
		if (mpDataFlag[i] == DssDataOk)
		{
			if (mpResultFlag[iVal] == DssDataUnknown)
			{
				mpResultFlag[iVal] = DssDataOk;
				mpResult[iVal] = mpData[i];
			}
			else if (mpData[i] > mpResult[iVal])
				mpResult[iVal] = mpData[i];
		}
	}
    
	return S_OK;
}

int DSSMovingGeneric::MaxForward (Int32 iVal)
{
 	Int32 i = 0,  iEnd = 0 ;
    
	if (iVal - mWinSize - 1 < mDataSize - 1)
		iEnd = iVal - mWinSize - 1;
	else
		iEnd = mDataSize - 1;
    
	for (i = iVal; i <= iEnd; i++)
	{
		if (mpDataFlag[i] == DssDataOk)
		{
			if (mpResultFlag[iVal] == DssDataUnknown)
			{
				mpResultFlag[iVal] = DssDataOk;
				mpResult[iVal] = mpData[i];
			}
			else if (mpData[i] > mpResult[iVal])
				mpResult[iVal] = mpData[i];
		}
	}
    
	return S_OK;
}

int DSSMovingGeneric::MinBackward (Int32 iVal)
{
	Int32 i = 0,  iStart = 0 ;
    
	if (iVal + 1 >= mWinSize)
		iStart = iVal - mWinSize + 1;
	else
		iStart = 0;
    
	for (i = iStart; i <= iVal; i++)
	{
		if (mpDataFlag[i] == DssDataOk)
		{
			if (mpResultFlag[iVal] == DssDataUnknown)
			{
				mpResultFlag[iVal] = DssDataOk;
				mpResult[iVal] = mpData[i];
			}
			else if (mpData[i] < mpResult[iVal])
				mpResult[iVal] = mpData[i];
		}
	}
    
	return S_OK;
}

int DSSMovingGeneric::MinForward (Int32 iVal)
{
 	Int32 i = 0,  iEnd = 0 ;
    
	if (iVal - mWinSize - 1 < mDataSize - 1)
		iEnd = iVal - mWinSize - 1;
	else
		iEnd = mDataSize - 1;
    
	for (i = iVal; i <= iEnd; i++)
	{
		if (mpDataFlag[i] == DssDataOk)
		{
			if (mpResultFlag[iVal] == DssDataUnknown)
			{
				mpResultFlag[iVal] = DssDataOk;
				mpResult[iVal] = mpData[i];
			}
			else if (mpData[i] < mpResult[iVal])
				mpResult[iVal] = mpData[i];
		}
	}
	return S_OK;
}

int DSSMovingGeneric::CountBackward (Int32 iVal)
{
	Int32 i = 0,  iStart = 0 ;
    
	if (iVal + 1 >= mWinSize)
		iStart = iVal - mWinSize + 1;
	else
		iStart = 0;
    
	mpResult[iVal] = 0;
	for (i = iStart; i <= iVal; i++)
	{
		if (mpDataFlag[i] == DssDataOk)
			mpResult[iVal] ++;
	}
    
	// There will always be a valid return value
	mpResultFlag[iVal] = DssDataOk;
	
    return S_OK;    
}

int DSSMovingGeneric::StdevBackward (Int32 iVal)
{
	Int32 i = 0,  iStart = 0 ;
	double *pTempData;
    
	pTempData = new double[mDataSize];
	if (pTempData == NULL)
		return E_OUTOFMEMORY;
    MBase::DSSStrongPtr<double, MBase::DeleteArray<double> > tempDataPtr(pTempData);
    
	if (iVal + 1 >= mWinSize)
		iStart = iVal - mWinSize + 1;
	else
		iStart = 0;
    
	Int32 lCount = 0;
	for (i = iStart; i <= iVal; i++)
	{
		if (mpDataFlag[i] == DssDataOk)
			pTempData[lCount++] = mpData[i];
	}
    
	if (!Variance(lCount, pTempData, &mpResult[iVal]))
	{
		mpResultFlag[iVal] = DssDataInvalid;
		return S_OK;
	}
    
	mpResultFlag[iVal] = DssDataOk;
	mpResult[iVal] = sqrt(mpResult[iVal]);
    
	return S_OK;
}

int DSSMovingGeneric::StdevBackwardP (Int32 iVal)
{
	Int32 i = 0,  iStart = 0 ;
	double *pTempData;
    
	pTempData = new double[mDataSize];
	if (pTempData == NULL)
		return E_OUTOFMEMORY;    
    MBase::DSSStrongPtr<double, MBase::DeleteArray<double> > tempDataPtr(pTempData);
    
	if (iVal + 1 >= mWinSize)
		iStart = iVal - mWinSize + 1;
	else
		iStart = 0;
    
	Int32 lCount = 0;
	for (i = iStart; i <= iVal; i++)
	{
		if (mpDataFlag[i] == DssDataOk)
			pTempData[lCount++] = mpData[i];
	}
    
	if (!VarianceP(lCount, pTempData, &mpResult[iVal]))
	{
		mpResultFlag[iVal] = DssDataInvalid;
		return S_OK;
	}
    
	mpResultFlag[iVal] = DssDataOk;
	mpResult[iVal] = sqrt(mpResult[iVal]);
     
	return S_OK;
}

int DSSMovingGeneric::DiffBackward (Int32 iVal)
{    
	if (mpDataFlag[iVal] == DssDataOk && iVal + 1 >= mWinSize)
	{
		mpResult[iVal] = mpData[iVal] - mpData[iVal - mWinSize + 1];
		mpResultFlag[iVal] = DssDataOk;
	}
    
	return S_OK;
}

int DSSMovingGeneric::CalculateMovingSum (void )
{
	Int32 i = 0;
	if (mWinSize > 0)
	{
		for (i = 0; i < mDataSize; i++)
			SumBasedOnPreviousWindow(i);
	}
       
	return S_OK;
}

int DSSMovingGeneric::CalculateMovingAvg (void )
{
	Int32 i = 0;
    
	if (mWinSize > 0)
	{
		for (i = 0; i < mDataSize; i++)
			AvgBasedOnPreviousWindow(i);
	}
    
	return S_OK;
}

int DSSMovingGeneric::CalculateMovingMax (void )
{
	Int32 i = 0;
    
	if (mWinSize > 0)
	{
		for (i = 0; i < mDataSize; i++)
			MaxBackward(i);
	}
    
	return S_OK;
}

int DSSMovingGeneric::CalculateMovingMin (void )
{
	Int32 i = 0;
    
	if (mWinSize > 0)
	{
		for (i = 0; i < mDataSize; i++)
			MinBackward(i);
	}
    
	return S_OK;
}

int DSSMovingGeneric::CalculateMovingCount (void )
{    
	Int32 i = 0;
    
	if (mWinSize > 0)
	{
		for (i = 0; i < mDataSize; i++)
			CountBackward(i);
	}
    
	return S_OK;
}

int DSSMovingGeneric::CalculateMovingDifference (void )
{
	Int32 i = 0;
	
	if (mWinSize > 0)
	{
		for (i = 0; i < mDataSize; i++)
			DiffBackward(i);
	}
    
	return S_OK;
}

int DSSMovingGeneric::CalculateMovingStdev (void )
{
	Int32 i = 0;
    
	if (mWinSize > 0)
	{
		for (i = 0; i < mDataSize; i++)
			StdevBackward(i);
	}
    
	return S_OK;
}

int DSSMovingGeneric::CalculateMovingStdevP (void )
{
	Int32 i = 0;
    
	if (mWinSize > 0)
	{
		for (i = 0; i < mDataSize; i++)
			StdevBackwardP(i);
	}
    
	return S_OK;
}

// jxie, 2010-12-16
// reuse the calculation result of previous window to optimize the calculation
int DSSMovingGeneric::SumBasedOnPreviousWindow (Int32 iVal)
{
	if (iVal + 1 > mWinSize)
	{
		Int32 iStart = iVal - mWinSize;
        
		if (mpDataFlag[iStart] == DssDataOk)
		{
			mOldNum -= mpData[iStart];
			mOldCount--;
		}
	}
	
	if (mpDataFlag[iVal] == DssDataOk)
	{
		mOldNum += mpData[iVal];
		mOldCount++;
	}
	
	mpResult[iVal] = mOldNum;
	if (mOldCount > 0)
		mpResultFlag[iVal] = DssDataOk;
    
	return S_OK;
}

// jxie, 2010-12-16
// reuse the calculation result of previous window to optimize the calculation
int DSSMovingGeneric::AvgBasedOnPreviousWindow(Int32 iVal)
{
 	if (iVal + 1 > mWinSize)
	{
		Int32 iStart = iVal - mWinSize;
        
		if (mpDataFlag[iStart] == DssDataOk)
		{
			mOldNum -= mpData[iStart];
			mOldCount--;
		}
	}
	
	if (mpDataFlag[iVal] == DssDataOk)
	{
		mOldNum += mpData[iVal];
		mOldCount++;
	}
	
	mpResult[iVal] = mOldNum;
	if (mOldCount > 0)
	{
		mpResultFlag[iVal] = DssDataOk;
		mpResult[iVal] /= mOldCount;
	}
    
	return S_OK;
}