//
//  DSSOlapGeneric.cpp
//  ReportServiceCore
//
//  Created by Qiang Xue on 7/21/06.
//  Copyright 2006 MicroStrategy, Inc. All rights reserved.
//

#include "DSSOlapGeneric.h"
#include "DFCDataConversion.h"
#include <algorithm>

#define UNINITIALIZED -1

DSSOlapGeneric::DSSOlapGeneric(): 
// jimzhang July/25/2011 begin : ported from CDSSOlapFunction in server 
mHasWindow(true), mWinType(0), mWinBDNum(2), mWinStartType(0),
mWinStartOffset(0), mWinEndType(0), mWinEndOffset(0), mDistinct(false),
// jimzhang July/25/2011 end
mDataSize(UNINITIALIZED), mpData(NULL), mpResult(NULL), 
mpDataFlag(NULL), mpResultFlag(NULL), 
// jxie, 2010-11-16
mfIsDirect(false), mpvDataByGroup(NULL), mOldStartIndex(0), mOldEndIndex(0), mOldCount(0), mOldNum(0), mOlapPattern(NULL), mOlapPatternLength(0)
{
}

DSSOlapGeneric::~DSSOlapGeneric()
{
	Clear();
}

// jimzhang July/25/2011 begin : ported from CDSSOlapFunction in server 
int  DSSOlapGeneric::setProperty(Int32 Index, CComVariant *pValue)
{
	int hr;
    
	if(Index<0) 
	{
		if(pValue->vt != VT_BOOL)
		{
			hr = VariantChangeType(pValue, pValue, 0, VT_BOOL);
			CHECK_HR;
		}
		switch (Index)
		{
            case -1:
                mDistinct = pValue->boolVal ? true : false;
                break;
            case -2:
                mCountNull = pValue->boolVal ? true : false;
                break;
		}
		
		hr = S_OK;
	}
	else if(Index==2) // OLAPPattern 
	{
		if(pValue->vt != VT_BSTR)
		{
			hr = VariantChangeType(pValue, pValue, 0, VT_BSTR);
			CHECK_HR;
		}
        mOlapPatternLength = 0;
        hr = DFCDataConversion::VariantSize(pValue, DssDataTypeVarChar, &mOlapPatternLength);
        CHECK_HR;
        
        mOlapPattern = new BYTE[mOlapPatternLength];

        hr = DFCDataConversion::ConvertFromVariant(pValue, DssDataTypeVarChar, mOlapPattern, &mOlapPatternLength);
        CHECK_HR;

//		mOlapPattern = pValue->bstrVal;
		hr = S_OK;
	} 
	else if(Index==3) // OLAPHasWindow
	{
		// 08/05/2008 now new olap functions will always set haswindow=true by default
        
		// fix issue 319905
		mHasWindow = true;
		hr = S_OK;
	}
	else if(Index>=4 && Index<=9)
	{
		if(pValue->vt != VT_I4)
		{
			hr = VariantChangeType(pValue, pValue, 0, VT_I4);
			CHECK_HR;
		}
		Int32 lValue = pValue->lVal;
		hr = E_FAIL;
		switch(Index)
		{
			case 4: // OLAPWinType
				if(lValue==0)
				{
					mWinType = lValue;
					hr = S_OK;
				}
				break;
			case 5: // OLAPWinBDNum
                // fix issue 319905
                mWinBDNum = 2;
                hr = S_OK;
                
				break;
			case 6: // OLAPWinStType
				if(lValue>=0 && lValue<=4)
				{
					if(lValue == 1) //mpittkin 343529 - Extra validation to match SQL Engine, OLAPWinStType can only be 0,2,3,4
						mWinStartType = 0;
					else
						mWinStartType = lValue;
					hr = S_OK;
				}
				break;
			case 7: // OLAPWinStOffset
				if(lValue>=0)
				{
					mWinStartOffset = lValue;
					hr = S_OK;
				}
				break;
			case 8: // OLAPWinEndType
				if(lValue>=0 && lValue<=4)
				{
					if(lValue == 0) //mpittkin 343529 - Extra validation to match SQL Engine, OLAPWinEndType can only be 1,2,3,4
						mWinEndType = 1;
					else
						mWinEndType = lValue;
					hr = S_OK;
				}
				break;
			case 9: // OLAPWinEndOffset
				if(lValue>=0)
				{
					mWinEndOffset = lValue;
					hr = S_OK;
				}
				break;
			default: 
				hr = S_OK;
				break;
		}
	}
	else
		hr = E_FAIL;
    
	CHECK_HR;
    
	return hr;
}

int DSSOlapGeneric::get_PropertyInfo(Int32 PropertyIndex, BSTR *pName, BSTR *pDescription, VARIANT *pValue)
{
	int hr;
    
	switch (PropertyIndex)
	{
        case 2: // olapPattern
            pValue->bstrVal = SysAllocStringLen(MBase::UTF8ToWideChar(reinterpret_cast<char*>(mOlapPattern)).c_str(),mOlapPatternLength);
            pValue->vt = VT_BSTR;
            hr = S_OK;
            break;
        case 3: // haswindow
            pValue->boolVal = mHasWindow ? VARIANT_TRUE : VARIANT_FALSE;
            pValue->vt = VT_BOOL;
            hr = S_OK;
            break;
        case -1:
            pValue->boolVal = mDistinct ? VARIANT_TRUE : VARIANT_FALSE;
            pValue->vt = VT_BOOL;
            hr = S_OK;
            break;
        case -2:
            pValue->boolVal = mCountNull ? VARIANT_TRUE : VARIANT_FALSE;
            pValue->vt = VT_BOOL;
            hr = S_OK;
            break;
        case -10:
            pValue->intVal = mFunctionIndex;
            pValue->vt = VT_I4;
            hr = S_OK;
            break;
            
        default:
            hr = E_NOTIMPL;
            break;
	}
    
	return hr;
}

bool DSSOlapGeneric::GetDistinct()
{
	return mDistinct;
}

bool DSSOlapGeneric::GetCountNull()
{
	return mCountNull;
}

int DSSOlapGeneric::GetWindow(Int32 currentIndex, Int32 dataSize, Int32& startIndex, Int32& endIndex)
{
	if (mHasWindow)
	{
		if (mWinBDNum==1)  // only start offset is specified
		{
			switch (mWinStartType)
			{
                case UnboundedPreceding : // unbounded preceding
                    startIndex = 0;
                    endIndex = currentIndex;
                    break;
                case UnboundedFollowing:  // unbounded following
                    startIndex = currentIndex;
                    endIndex = dataSize-1;
                    break;
                case CurrentRow : // current row
                    startIndex = currentIndex;
                    endIndex = currentIndex;
                    break;
                case Preceding : // preceding
                    startIndex = currentIndex-mWinStartOffset;
                    endIndex = currentIndex;
                    break;
                case Following : // following
                    startIndex = currentIndex;
                    endIndex = currentIndex+mWinStartOffset;
                    break;
                default :
                    break;
			}
		}
		else  // both start and end offsets are specified
		{
			switch (mWinStartType)
			{
                case UnboundedPreceding : // unbounded preceding
                    startIndex = 0;
                    break;
                case UnboundedFollowing :  // unbounded following
                    startIndex = dataSize-1;
                    break;
                case CurrentRow : // current row
                    startIndex = currentIndex;
                    break;
                case Preceding : // preceding
                    startIndex = currentIndex-mWinStartOffset;
                    break;
                case Following : // following
                    startIndex = currentIndex+mWinStartOffset;
                    break;
                default :
                    break;
			}
			switch (mWinEndType)
			{
                case UnboundedPreceding : // unbounded preceding
                    endIndex = 0;
                    break;
                case UnboundedFollowing:  // unbounded following
                    endIndex = dataSize-1;
                    break;
                case CurrentRow : // current row
                    endIndex = currentIndex;
                    break;
                case Preceding : // preceding
                    endIndex = currentIndex-mWinEndOffset;
                    break;
                case Following : // following
                    endIndex = currentIndex+mWinEndOffset;
                    break;
                default :
                    break;
			}
		}
		
		if(startIndex > endIndex)
			return E_FAIL;
        
		if(startIndex>=dataSize || endIndex<0) // out of range
			endIndex=startIndex-1; // so that this represents an empty set
		else
		{
			if(startIndex<0)
				startIndex=0;
			if(endIndex>=dataSize)
				endIndex=dataSize-1;
		}
	}
	else  // the default window is unbound proceeding to current row
	{
		startIndex = 0;
		endIndex = currentIndex;
	}
	return S_OK;
}
// jimzhang July/25/2011 end

int DSSOlapGeneric::Calculate ()
{
 	Int32 i = 0;
    if(mDataSize == UNINITIALIZED || mFunctionIndex==DssFuncUnknown)
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
        case DssFuncOLAPSum:
            if(GetDistinct())
                SumDistinct();
            else
            {
                for(i=0;i<mDataSize;i++)
                    SumByWindow(i);
            }
            break;
        case DssFuncOLAPAvg:
            if(GetDistinct())
                AvgDistinct();			
            else
            {
                for(i=0;i<mDataSize;i++)
                    AvgByWindow(i);
            }
            break;
        default:
            return E_FAIL;
	}
    
	return S_OK;
}

int DSSOlapGeneric::SetInputParam (Int32 nParamIndex, Int32 nSize, double *pData, DSSDataFlag *pFlag)
{
	if(nParamIndex!=1)
		return E_INVALIDARG;
    
	if(nSize < 1) return E_INVALIDARG;
	
	if(!pData) return E_POINTER;
    
	mDataSize = nSize;
	// allocat memory to record input data
	mpData = new double[nSize];
	if(!mpData) return E_OUTOFMEMORY;
	memmove(mpData, pData, nSize * sizeof(double));
	mpDataFlag = new DSSDataFlag[nSize];
	if(!mpDataFlag) return E_OUTOFMEMORY;
	memmove(mpDataFlag, pFlag, nSize * sizeof(DSSDataFlag));
    
	return S_OK;
}

int DSSOlapGeneric::GetOutput (Int32 nSize, Int32 *nDataSize, double *pData, DSSDataFlag *pFlag)
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

int DSSOlapGeneric::GetOutputType (EnumDSSParameterType *pType)
{
 	if(!pType) return E_POINTER;
    
	*pType = DssParameterVector;
	
	return S_OK;
}

int DSSOlapGeneric::GetInputParamType (Int32 nParamIndex, EnumDSSParameterType *pType)
{
	if(!pType) return E_POINTER;
    
	if(nParamIndex==1)
	{
		*pType = DssParameterVector;
		return S_OK;
	}
	else
		return E_FAIL;
}

int DSSOlapGeneric::IsNullProcessable (bool *pCanProcessNULL)
{
	if(!pCanProcessNULL) return E_POINTER;
	
	*pCanProcessNULL = true; // 05/14/2008 mwang, now all olap func can process Null
    
	return S_OK;
}

int DSSOlapGeneric::Clear ()
{
	// jxie, 2010-11-16
	if (!mfIsDirect)
	{
        if (mpData)
        {
            delete [] mpData;
            mpData = NULL;
        }
        if (mpResult)
        {
            delete [] mpResult;
            mpResult = NULL;
        }
        if (mpDataFlag)
        {
        	delete [] mpDataFlag;
            mpDataFlag = NULL;
        }
        if (mpResultFlag)
        {
            delete [] mpResultFlag;
            mpResultFlag = NULL;
        }
        if (mOlapPattern)
        {
            delete [] mOlapPattern;
            mOlapPattern = NULL;
        }
	}
	mfIsDirect = false;
	mpvDataByGroup = NULL;
    
	//lwang; 12/01/2008; 337727, reset pointer
	mpResult = NULL;
	mpResultFlag = NULL;
	mpData = NULL;
	mpDataFlag = NULL;
	mDataSize = UNINITIALIZED;
    
	return S_OK;
}

int DSSOlapGeneric::SumDistinct()
{
	vector <double> values;
    
	Int32 i;
	for(i=0;i<mDataSize;++i)
	{
		if(mpDataFlag[i]==DssDataOk)
			values.push_back(mpData[i]);
	}
    
	if(values.size())
	{
		sort(values.begin(),values.end());
        
		vector <double>::iterator iter=values.begin();
		double previousValue=*iter;
		double sum=previousValue;
		for(iter++;iter!=values.end();iter++)
		{
			if(*iter!=previousValue)
			{
				sum += *iter;
				previousValue = *iter;
			}
		}
        
		for(i=0;i<mDataSize;i++)
		{
			mpResult[i]=sum;
			mpResultFlag[i]=DssDataOk;
		}
	}
    
	return S_OK;
}

int DSSOlapGeneric::AvgDistinct()
{
	vector <double> values;
    
	Int32 i;
	for(i=0;i<mDataSize;++i)
	{
		if(mpDataFlag[i]==DssDataOk)
			values.push_back(mpData[i]);
	}
    
	if(values.size())
	{
		sort(values.begin(),values.end());
        
		vector <double>::iterator iter=values.begin();
		double previousValue=*iter;
		double sum=previousValue;
		Int32 count=1;
		for(iter++;iter!=values.end();iter++)
		{
			if(*iter!=previousValue)
			{
				sum += *iter;
				previousValue = *iter;
				count++;
			}
		}
        
		sum/=count;
        
		for(i=0;i<mDataSize;i++)
		{
			mpResult[i]=sum;
			mpResultFlag[i]=DssDataOk;
		}
	}
    
	return S_OK;
}

int DSSOlapGeneric::SumByWindow(Int32 row)
{
	Int32 startRow, endRow;
	int hr=GetWindow(row,mDataSize,startRow,endRow);
	CHECK_HR;
    
	// jxie, 2010-11-16
	Int32 lRow = row;
	if (mpvDataByGroup != NULL)
		lRow = (*mpvDataByGroup)[row];
    
	// jxie, 2010-12-6
	// move to this function for reuse
	hr = hCalculateByWindow(row, startRow, endRow);
	
	mpResult[lRow] = mOldNum;
	if (mOldCount > 0)
		mpResultFlag[lRow]=DssDataOk;
    
	return S_OK;
}

int DSSOlapGeneric::AvgByWindow(Int32 row)
{
	Int32 startRow, endRow;
	int hr=GetWindow(row,mDataSize,startRow,endRow);
	CHECK_HR;
    
	// jxie, 2010-11-16
	Int32 lRow = row;
	if (mpvDataByGroup != NULL)
		lRow = (*mpvDataByGroup)[row];
    
	// jxie, 2010-12-6
	// move to this function for reuse
	hr = hCalculateByWindow(row, startRow, endRow);
	
	mpResult[lRow] = mOldNum;
	if (mOldCount > 0)
	{
		mpResult[lRow] /= mOldCount;
		mpResultFlag[lRow]=DssDataOk;
	}
    
	return S_OK;
}

// jxie, 2010-10-13
// support direct calculate
int DSSOlapGeneric::DirectCalculate (Int32 nParam, Int32* ipSize, Int32** ipOffsets, 
                                     void** ipData, double* ipDivider, DSSDataFlag** ipFlags, VARIANT_BOOL* ipAllDataValid,
                                     EnumDSSDataType* iType, vector<Int32> * ipvDataByGroup,
                                     Int32 nResSize, double* opData, DSSDataFlag* opFlag, VARIANT_BOOL* opAllDataValid, 
                                     EnumNullCheckingOption iNullCheckingOption)
{
    int hr = S_OK;	
    
	if(nParam != 1)
		return E_INVALIDARG;
    
	// jxie, only support double array input now
	if (iType[0] != DssDataTypeDouble)
		return S_FALSE;
    
	if (ipvDataByGroup)
		mDataSize = ipvDataByGroup->size();
	else
		mDataSize = ipSize[0];
    
	if(mDataSize < 1) return E_INVALIDARG;
	if(!ipData || !opData || !opFlag) return E_POINTER;
    
	mfIsDirect = true;
    
	mpData = (double*)(ipData[0]);
	mpDataFlag = ipFlags[0];
	mpResult = opData;
	mpResultFlag = opFlag;
	mpvDataByGroup = ipvDataByGroup;
    
	Int32 i = 0;
	Int32 lRow = 0;
	// check whether all the required input parameters have been set.
 	if(mFunctionIndex==DssFuncUnknown || !mpData || !mpDataFlag) 
    {
        return S_FALSE;
    }
    
	// jxie, 2010-12-7
	// initialize the flags
	for (i = 0; i < mDataSize; i++)
	{
		if (mpvDataByGroup != NULL)
			lRow = (*mpvDataByGroup)[i];
		else
			lRow = i;
        
		mpResultFlag[lRow] = DssDataUnknown;
	}
    
	switch (mFunctionIndex)
	{
        case DssFuncOLAPSum:
            if(GetDistinct())
                hDirectSumDistinct();
            else
            {
                for(i = 0; i < mDataSize; i++)
                {
                    SumByWindow(i);
                }
            }
            break;
        case DssFuncOLAPAvg:
            if(GetDistinct())
                hDirectAvgDistinct();			
            else
            {
                for(i = 0; i < mDataSize; i++)
                {
                    AvgByWindow(i);
                }
            }
            break;
        default:
            return E_FAIL;
	}
    
	return hr = S_OK;
}

// jxie, 2010-10-13
// support direct calculate on all functions in this class 
bool DSSOlapGeneric::SupportDirectCalculate()
{
	return true;
}

// jxie, 2010-11-9
int DSSOlapGeneric::hDirectSumDistinct()
{
	vector <double> values;
    
	Int32 i;
	Int32 lRow = 0;
	for(i = 0; i < mDataSize; i++)
	{
		if (mpvDataByGroup != NULL)
			lRow = (*mpvDataByGroup)[i];
		else
			lRow = i;
        
		if(mpDataFlag[lRow] == DssDataOk)
			values.push_back(mpData[lRow]);
	}
    
	if(values.size())
	{
		sort(values.begin(),values.end());
        
		vector <double>::iterator iter=values.begin();
		double previousValue=*iter;
		double sum=previousValue;
		for(iter++;iter!=values.end();iter++)
		{
			if(*iter!=previousValue)
			{
				sum += *iter;
				previousValue = *iter;
			}
		}
        
		for(i = 0;i < mDataSize; i++)
		{
			if (mpvDataByGroup != NULL)
				lRow = (*mpvDataByGroup)[i];
			else
				lRow = i;
            
			mpResult[lRow] = sum;
			mpResultFlag[lRow] = DssDataOk;
		}
	}
    
	return S_OK;
}

// jxie, 2010-11-9
int DSSOlapGeneric::hDirectAvgDistinct()
{
	vector <double> values;
    
	Int32 i;
	Int32 lRow = 0;
	for(i=0;i<mDataSize;++i)
	{
		if (mpvDataByGroup != NULL)
			lRow = (*mpvDataByGroup)[i];
		else
			lRow = i;
        
		if(mpDataFlag[lRow] == DssDataOk)
			values.push_back(mpData[lRow]);
	}
    
	if(values.size())
	{
		sort(values.begin(),values.end());
        
		vector <double>::iterator iter=values.begin();
		double previousValue=*iter;
		double sum=previousValue;
		Int32 count=1;
		for(iter++;iter!=values.end();iter++)
		{
			if(*iter!=previousValue)
			{
				sum += *iter;
				previousValue = *iter;
				count++;
			}
		}
        
		sum/=count;
        
		for(i=0;i<mDataSize;i++)
		{
			if (mpvDataByGroup != NULL)
				lRow = (*mpvDataByGroup)[i];
			else
				lRow = i;
            
			mpResult[lRow] = sum;
			mpResultFlag[lRow] = DssDataOk;
		}
	}
    
	return S_OK;
}

// jxie, 2010-12-3
// to speed up calculation within windows
int DSSOlapGeneric::hCalculateByWindow(Int32 row, Int32 startRow, Int32 endRow)
{
	Int32 lRow2 = 0;
	Int32 count=0;
	double lResult = 0;
    
	if (row > 0 && startRow < mOldEndIndex &&
		startRow <= mOldStartIndex + 1 && startRow >= mOldStartIndex - 1 &&
		endRow <= mOldEndIndex + 1 && endRow >= mOldEndIndex - 1 )
	{
		lResult = mOldNum;
		count = mOldCount;
		if (startRow == mOldStartIndex - 1)
		{
			lRow2 = startRow;
			if (mpvDataByGroup)
				lRow2 = (*mpvDataByGroup)[startRow];
            
			if(mpDataFlag[lRow2] == DssDataOk)
			{
				lResult += mpData[lRow2];
				count++;
			}
		}
		else if (startRow == mOldStartIndex + 1)
		{
			lRow2 = mOldStartIndex;
			if (mpvDataByGroup)
				lRow2 = (*mpvDataByGroup)[mOldStartIndex];
            
			if(mpDataFlag[lRow2] == DssDataOk)
			{
				lResult -= mpData[lRow2];
				count--;
			}
		}
        
		if (endRow == mOldEndIndex - 1)
		{
			lRow2 = mOldEndIndex;
			if (mpvDataByGroup)
				lRow2 = (*mpvDataByGroup)[mOldEndIndex];
            
			if(mpDataFlag[lRow2] == DssDataOk)
			{
				lResult -= mpData[lRow2];
				count--;
			}
		}
		else if (endRow == mOldEndIndex + 1)
		{
			lRow2 = endRow;
			if (mpvDataByGroup)
				lRow2 = (*mpvDataByGroup)[endRow];
            
			if(mpDataFlag[lRow2] == DssDataOk)
			{
				lResult += mpData[lRow2];
				count++;
			}
		}
	}
	else
	{
		for(Int32 i=startRow;i<=endRow;++i)
		{
			lRow2 = i;
			if (mpvDataByGroup)
				lRow2 = (*mpvDataByGroup)[i];
            
			if(mpDataFlag[lRow2] == DssDataOk)
			{
				lResult += mpData[lRow2];
				count++;
			}
		}
	}
    
	mOldStartIndex = startRow;
	mOldEndIndex = endRow;
	mOldCount = count;
	mOldNum = lResult;
	
	return S_OK;
}
