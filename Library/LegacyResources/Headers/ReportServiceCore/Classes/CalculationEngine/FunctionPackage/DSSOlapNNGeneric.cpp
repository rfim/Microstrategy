//
//  DSSOlapNNGeneric.cpp
//  ReportServiceCore
//
//  Created by Qiang Xue on 7/21/06.
//  Copyright 2006 MicroStrategy, Inc. All rights reserved.
//

#include "DSSOlapNNGeneric.h"

#import "DSSFunctionUtility.h"

DSSOlapNNGeneric::DSSOlapNNGeneric(): 
// jimzhang July/25/2011 begin : ported from CDSSOlapFunction in server
mHasWindow(true), mWinType(0), mWinBDNum(2), mWinStartType(0),
mWinStartOffset(0), mWinEndType(0), mWinEndOffset(0), mDistinct(false),
// jimzhang July/25/2011 end
mpDataFlag(NULL), mpResultFlag(NULL), 
mDataSize(-1), mOffset(1), mpDefaultValueFlag(NULL), mpOffsetFlag(NULL),
// jxie, 2010-11-15
mfIsNumeric(false), mpNumericResultData(NULL), mpNumericInputData(NULL),
mpNumericOffset(NULL), mpNumericDefaultValue(NULL), mfIsDirect(false),
mpvDataByGroup(NULL), mOldStartIndex(0), mOldEndIndex(0), mOldCount(0), mOlapPattern(NULL), mOlapPatternLength(0)
{
}

DSSOlapNNGeneric::~DSSOlapNNGeneric()
{
	Clear();
}

// jimzhang July/25/2011 begin : ported from CDSSOlapFunction in server
int  DSSOlapNNGeneric::setProperty(Int32 Index, CComVariant *pValue)
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

int DSSOlapNNGeneric::get_PropertyInfo(Int32 PropertyIndex, BSTR *pName, BSTR *pDescription, VARIANT *pValue)
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
	}
    
	return hr;
}


bool DSSOlapNNGeneric::GetDistinct()
{
	return mDistinct;
}

bool DSSOlapNNGeneric::GetCountNull()
{
	return mCountNull;
}

int DSSOlapNNGeneric::GetWindow(Int32 currentIndex, Int32 dataSize, Int32& startIndex, Int32& endIndex)
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

int DSSOlapNNGeneric::SetInputParam (Int32 nParamIndex, Int32 nSize, EnumDSSDataType dataType, CComVariant *pData, DSSDataFlag *pFlag)
{
	int hr=S_OK;
	if(nSize < 1) return E_FAIL;
	
	if(!pData)
		return E_FAIL;
    
	if(nParamIndex == 1)
	{
		if ((pData->vt & (VT_ARRAY|VT_VARIANT)) != (VT_ARRAY|VT_VARIANT))
			return E_FAIL;
        
		if(mDataSize < 0)
		{
			mDataSize = nSize;
		}
		mpDataFlag = new DSSDataFlag[mDataSize];
		CHECK_NEWED_PTR(mpDataFlag);
		memmove(mpDataFlag, pFlag, mDataSize * sizeof(DSSDataFlag));
        
		mInputData.Copy(pData);
	}
	else if(mFunctionIndex==DssFuncLag || mFunctionIndex==DssFuncLead)
	{
		// mpittkin, 325608, 325757 - 10/08
		// take the offset and default values as vectors, to support expressions as parameters
		if (nParamIndex == 2)
		{
			if ((pData->vt & (VT_ARRAY|VT_VARIANT)) != (VT_ARRAY|VT_VARIANT))
				return E_FAIL;
			if(mDataSize < 0)
			{
				mDataSize = nSize;
			}
			mpOffsetFlag = new DSSDataFlag[mDataSize];
			CHECK_NEWED_PTR(mpOffsetFlag);
			memmove(mpOffsetFlag, pFlag, mDataSize * sizeof(DSSDataFlag));
            
			mOffset.Copy(pData);
		}
		else if(nParamIndex == 3)
		{
			if ((pData->vt & (VT_ARRAY|VT_VARIANT)) != (VT_ARRAY|VT_VARIANT))
				return E_FAIL;
			if(mDataSize < 0)
			{
				mDataSize = nSize;
			}
			mpDefaultValueFlag = new DSSDataFlag[mDataSize];
			CHECK_NEWED_PTR(mpDefaultValueFlag);
			memmove(mpDefaultValueFlag, pFlag, mDataSize * sizeof(DSSDataFlag));
            
			mDefaultValue.Copy(pData);
		}
		else
			hr = E_FAIL;
	}
	else
		hr = E_FAIL;
	
	CHECK_HR;
    
	return hr;
}

int DSSOlapNNGeneric::GetOutput (Int32 nSize, Int32 *nDataSize, CComVariant *pData, DSSDataFlag *pFlag)
{    
	CHECK_PTR(nDataSize);
	CHECK_PTR(pData);
	CHECK_PTR(pFlag);
        
	if(nSize < mDataSize) 
    {
        return S_FALSE;
    }
    
	*nDataSize = mDataSize;
    
	AE_ASSERT(mpResultFlag != NULL);
    
	VariantCopy(pData, &mResultData);
    
	memmove(pFlag, mpResultFlag, mDataSize*sizeof(DSSDataFlag));
	
	return S_OK;
}

int DSSOlapNNGeneric::GetOutputType (Int32 nSize, EnumDSSDataType *pInputType, EnumDSSDataType *pOutputDataType, EnumDSSParameterType *pOutParamType)
{	
	CHECK_PTR(pInputType);
	CHECK_PTR(pOutputDataType);
	CHECK_PTR(pOutParamType);
    
	bool lfInputIsInteger = false;
	switch(pInputType[0])
	{
        case DssDataTypeInteger:
        case DssDataTypeShort:
            lfInputIsInteger = true;
            break;
        default:
            break;
	}
    
	*pOutParamType = DssParameterVector;
    
	if(mFunctionIndex == DssFuncOLAPCount)
		*pOutputDataType = DssDataTypeLong;
	else
        if(lfInputIsInteger && (mFunctionIndex == DssFuncLag || mFunctionIndex == DssFuncLead))
        { // qjiang 04/27/2009 as we may convert a double column to integer/short column, 
            // the default value (type is double) for olap function Lead/Lag may be out of range,
            // currently use double as output type
            *pOutputDataType = DssDataTypeDouble;
        }
        else
        {
            *pOutputDataType = pInputType[0];
        }
	
	return S_OK;
}

int DSSOlapNNGeneric::GetInputParamType (Int32 nParamIndex, EnumDSSParameterType *pType)
{
	int hr = S_OK;
    
	if (!pType) 
		hr = E_FAIL;
    
	switch (nParamIndex)
	{
        case 1:
        case 2:
        case 3:
            *pType = DssParameterVector;
            hr = S_OK;
            break;
        default:
            hr = E_FAIL;
            break;
	}
    
	return hr;
}

int DSSOlapNNGeneric::IsNullProcessable (bool *pCanProcessNULL)
{
    int hr = S_OK;
	
	if(!pCanProcessNULL) 
		return hr = E_FAIL;
	
	*pCanProcessNULL = true; // always assume input data is valid
    
	return hr = S_OK;
}

int DSSOlapNNGeneric::Clear()
{    
	// jxie, 2010-11-16
	if (!mfIsDirect)
	{
		if (mpDataFlag)
			delete [] mpDataFlag;
        
		if (mpResultFlag)
			delete [] mpResultFlag;
        
		if(mpDefaultValueFlag)
			delete [] mpDefaultValueFlag;
        
		if (mpOffsetFlag)
			delete [] mpOffsetFlag;
	}
    
	mpDataFlag = NULL;
	mpOffsetFlag = NULL;
	mpDefaultValueFlag = NULL;
	mpResultFlag = NULL;
    
	mDataSize = -1;
    
	mInputData.Clear();
	mResultData.Clear();
	mOffset.Clear();
	mDefaultValue.Clear();
    
	// jxie, 2010-11-15
	if (mfIsNumeric)
	{
		delete [] mpNumericInputData;
		delete [] mpNumericOffset;
		delete [] mpNumericDefaultValue;
		delete [] mpNumericResultData;
	}
    
	mpNumericInputData = NULL;
	mpNumericOffset = NULL;
	mpNumericDefaultValue = NULL;
	mpNumericResultData = NULL;
	mpvDataByGroup = NULL;
	mfIsNumeric = false;
	mfIsDirect = false;
    
	return S_OK;
}

int DSSOlapNNGeneric::Calculate ()
{
	int hr;
	Int32 i;
	if (mDataSize == -1)
		return hr = E_FAIL;	// input parameters not set
    
	if (mfIsNumeric)
		return hNumericCalculate();
    
	mpResultFlag = new DSSDataFlag[mDataSize];
	CHECK_NEWED_PTR(mpResultFlag);
    
	for (i = 0; i < mDataSize; i++)
		mpResultFlag[i] = DssDataUnknown;
    
	mResultData.vt = VT_ARRAY|VT_VARIANT;
	SAFEARRAYBOUND lSABound;
	lSABound.lLbound = 0;
	lSABound.cElements = mDataSize;
	mResultData.parray = SafeArrayCreate(VT_VARIANT, 1, &lSABound);
    
	mPrevStart = -1;
	mPrevEnd = -1;
	mPrevIndex = -1;
    
	switch (mFunctionIndex)
	{
        case DssFuncOLAPCount:
            if(GetDistinct())
                CountDistinct();
            else
            {
                for(i=0;i<mDataSize;i++)
                    CountByWindow(i);
            }
            break;
        case DssFuncOLAPMax:
        case DssFuncOLAPMin:
            if(GetDistinct())
                MaxMinDistinct();
            else
            {
                for(i=0;i<mDataSize;i++)
                    MaxMinByWindow(i);
            }
            break;
        case DssFuncLag:
        case DssFuncLead:
            for(i=0;i<mDataSize;i++)
                LagLead(i);
            break;
        default:
            return hr = E_FAIL;
	}
    
	return hr = S_OK;
}

int DSSOlapNNGeneric::LagLead(Int32 row)
{
	int hr = S_OK;
	CComVariant lvResult, lvDefault, lvOffset;
	Int32 lnOffset;
	hr = getVariant(&mOffset, row, lvOffset);
	CHECK_HR;
    
	Int32 nLongSize = sizeof(Int32);
	hr = DFCDataConversion::ConvertFromVariant(&lvOffset, DssDataTypeLong, (BYTE *)&lnOffset, &nLongSize);
	CHECK_HR;
    
	Int32 index=(mFunctionIndex==DssFuncLag)?row-lnOffset:row+lnOffset;
	
	if(index<0 || index>=mDataSize) // outlier rows, use default value
	{	
		// mpittkin 325608 : 10/08, fix lag lead to use vector for default value paramter
		if(mpDefaultValueFlag[row] == DssDataOk)
		{
			hr = getVariant(&mDefaultValue, row, lvResult);
			CHECK_HR;
			hr = setVariant(&mResultData, row, lvResult);
			CHECK_HR;
		}
		mpResultFlag[row] = mpDefaultValueFlag[row];
	}
	else
	{
		hr = getVariant(&mInputData,index,lvDefault);
		CHECK_HR;
		hr = setVariant(&mResultData,row,lvDefault);
		CHECK_HR;
		mpResultFlag[row] = DssDataOk;//mpDataFlag[row];	
	}
    
	return hr = S_OK;
}

int DSSOlapNNGeneric::CountByWindow(Int32 row)
{
	Int32 startRow, endRow;
	int hr=GetWindow(row,mDataSize,startRow,endRow);
	CHECK_HR;
    
	Int32 count=0;
	for(Int32 i=startRow;i<=endRow;++i)
	{
		if(mpDataFlag[i]==DssDataOk || GetCountNull())
			count++;
	}
	
	CComVariant result;
	hr = DFCDataConversion::ConvertToVariant((unsigned char *)&count, DssDataTypeLong, 0, &result);
	CHECK_HR;
    
	setVariant(&mResultData, row, result);
    
	mpResultFlag[row]=DssDataOk;
    
	return S_OK;
}

bool VariantGreater(const CComVariant& v1,const CComVariant& v2)
{
	DSSDataFlag flag=DssDataOk;
    
	CComVariant a;
	CComVariant b;
	a.Copy(&v1);
	b.Copy(&v2);
	return variantCompare(a,b,&flag)<0;
}

int DSSOlapNNGeneric::CountDistinct()
{
	vector<CComVariant> values;
    
	Int32 i;
	for(i=0;i<mDataSize;++i)
	{
		if(mpDataFlag[i]==DssDataOk || GetCountNull())
		{
			CComVariant value;
			getVariant(&mInputData, i, value);
			values.push_back(value);
		}
	}
    
	Int32 count=0;
    
	if(values.size())
	{
		sort(values.begin(),values.end(),VariantGreater);
		vector<CComVariant>::iterator iter=values.begin();
		CComVariant previousValue=*iter;
		count = 1;
		for(iter++;iter!=values.end();iter++)
		{
			DSSDataFlag flag=DssDataOk;
			Int32 comparison=variantCompare(*iter,previousValue,&flag);
			if(flag==DssDataOk && comparison!=0)
			{
				count++;
				previousValue = *iter;
			}
		}
	}
    
	CComVariant result;
    DFCDataConversion::ConvertToVariant((unsigned char *)&count, DssDataTypeLong, 0, &result);
    
	for(i=0;i<mDataSize;i++)
	{	
		// mpittkin: 327655, 10/08, match SQL behavior where this function returns null for null metric rows
		if(mpDataFlag[i] == DssDataOk)
		{
			setVariant(&mResultData, i, result);
			mpResultFlag[i]=DssDataOk;
		}
		else
		{
			mpResultFlag[i] = mpDataFlag[i];
		}
	}
    
	return S_OK;
}

int DSSOlapNNGeneric::MaxMinDistinct()
{
	Int32 firstFlag =0;
	CComVariant result;
	for(Int32 i=0;i<mDataSize;i++)
	{
		if(mpDataFlag[i]==DssDataOk)
		{	
			CComVariant value;
			getVariant(&mInputData, i, value);
			if(firstFlag==0){
				firstFlag=1;
				result.Copy(&value);
			}
			else
			{
				DSSDataFlag flag=DssDataOk;
				Int32 comparison=variantCompare(result,value,&flag);
				if(flag==DssDataOk)
				{
					if(comparison>0 && mFunctionIndex==DssFuncOLAPMin)
						result.Copy(&value);
					else if (comparison<0 && mFunctionIndex==DssFuncOLAPMax)
						result.Copy(&value);
				}
			}
		}
	}
	for(Int32 i=0;i<mDataSize;i++)
	{	
		// match SQL behavior where this function returns null for null metric rows
		if(mpDataFlag[i] == DssDataOk)
		{
			setVariant(&mResultData, i, result);
			mpResultFlag[i]=DssDataOk;
		}
		else
		{
			mpResultFlag[i] = mpDataFlag[i];
		}
	}
	return S_OK;
}

int DSSOlapNNGeneric::MaxMinByWindow(Int32 row)
{
	Int32 i, startRow, endRow;
	int hr=GetWindow(row,mDataSize,startRow,endRow);
	CHECK_HR;
    
	if(mPrevIndex<0 || mPrevIndex<startRow || mPrevIndex>endRow)
	{
		// first round, or previous result is out of the current window
		// in these cases, we need to scan the whole window
		mPrevIndex=-1;
		for(i=startRow;i<=endRow;++i)
		{
			if (mpDataFlag[i]==DssDataOk)  // check valid data only
			{				
				if(mPrevIndex<0)  // the first valid element
					mPrevIndex=i;
				else
					MinMaxCompare(mPrevIndex,i,mPrevIndex);
			}
		}
	}
	else
	{
		// here is the optimization: we only scan the disjoint part of the window
		if(startRow<mPrevStart)
		{
			for(i=startRow;i<mPrevStart;++i)
				MinMaxCompare(mPrevIndex,i,mPrevIndex);
		}
		if(mPrevEnd<endRow)
		{
			for(i=mPrevEnd+1;i<=endRow;++i)
				MinMaxCompare(mPrevIndex,i,mPrevIndex);
		}
	}
    
	mPrevStart=startRow;
	mPrevEnd=endRow;
    
	// if there is any result, save it to the current row
	if(mPrevIndex>=0)
	{
		CComVariant result;
		getVariant(&mInputData, mPrevIndex, result);
		setVariant(&mResultData, row, result);
		mpResultFlag[row]=DssDataOk;
	}
    
	return S_OK;
}

void DSSOlapNNGeneric::MinMaxCompare(Int32 row1, Int32 row2, Int32 &result)
{
	if(mpDataFlag[row1]==DssDataOk && mpDataFlag[row2]==DssDataOk)
	{
		CComVariant value1;
		CComVariant value2;
		getVariant(&mInputData, row1, value1);
		getVariant(&mInputData, row2, value2);
		DSSDataFlag flag=DssDataOk;
		Int32 c=variantCompare(value1,value2,&flag);
		if(flag==DssDataOk)
		{
			if(mFunctionIndex==DssFuncOLAPMin)
				result=c>0 ? row2 : row1;
			else if(mFunctionIndex==DssFuncOLAPMax)
				result=c>0 ? row1 : row2;
		}
	}
}

// jxie, 2010-9-27
int  DSSOlapNNGeneric::GetOutput(Int32 nSize, Int32 *nDataSize,  double * pData, DSSDataFlag * pFlag)
{
	CHECK_PTR(nDataSize);
	CHECK_PTR(pData);
	CHECK_PTR(pFlag);
    
	if(nSize < mDataSize) 
    {
        
        return S_FALSE;
    }
    
	*nDataSize = mDataSize;
    
	AE_ASSERT(mpResultFlag != NULL);
    
	memmove(pData, mpNumericResultData, mDataSize * sizeof(double) );
	memmove(pFlag, mpResultFlag, mDataSize * sizeof(DSSDataFlag) );
	
	return S_OK;
}

// jxie, 2010-9-27
int  DSSOlapNNGeneric::GetOutputType(EnumDSSParameterType * pType)
{
	if(!pType) return E_POINTER;
    
	*pType = DssParameterVector;
	
	return S_OK;
}

// jxie, 2010-9-27
int  DSSOlapNNGeneric::SetInputParam(Int32 nParamIndex, Int32 nSize, double * pData, DSSDataFlag * pFlag)
{
	int hr = S_OK;
	if(nSize < 1) return E_FAIL;
	
	if(!pData)
		return E_FAIL;
    
	mfIsNumeric = true;
    
	//Int32 i = 0;
	if(nParamIndex == 1)
	{
		if(mDataSize < 0)
		{
			mDataSize = nSize;
		}
		mpDataFlag = new DSSDataFlag[mDataSize];
		CHECK_NEWED_PTR(mpDataFlag);
		memmove(mpDataFlag, pFlag, mDataSize * sizeof(DSSDataFlag));
		mpNumericInputData = new double[mDataSize];
		memmove(mpNumericInputData, pData, mDataSize * sizeof(double));
	}
	else if(mFunctionIndex==DssFuncLag || mFunctionIndex==DssFuncLead)
	{
		// mpittkin, 325608, 325757 - 10/08
		// take the offset and default values as vectors, to support expressions as parameters
		if (nParamIndex == 2)
		{
			if(mDataSize < 0)
			{
				mDataSize = nSize;
			}
			mpOffsetFlag = new DSSDataFlag[mDataSize];
			CHECK_NEWED_PTR(mpOffsetFlag);
			memmove(mpOffsetFlag, pFlag, mDataSize * sizeof(DSSDataFlag));
			mpNumericOffset = new double[mDataSize];
			memmove(mpNumericOffset, pData, mDataSize * sizeof(double));
		}
		else if(nParamIndex == 3)
		{
			if(mDataSize < 0)
			{
				mDataSize = nSize;
			}
			mpDefaultValueFlag = new DSSDataFlag[mDataSize];
			CHECK_NEWED_PTR(mpDefaultValueFlag);
			memmove(mpDefaultValueFlag, pFlag, mDataSize * sizeof(DSSDataFlag));
			mpNumericDefaultValue = new double[mDataSize];
			memmove(mpNumericDefaultValue, pData, mDataSize * sizeof(double));
		}
		else
			hr = E_FAIL;
	}
	else
		hr = E_FAIL;
	
	CHECK_HR;
    
	return hr;
}

// jxie, 2010-9-28
int  DSSOlapNNGeneric::hNumericCalculate()
{
	int hr = S_OK;
	mpResultFlag = new DSSDataFlag[mDataSize];
	CHECK_NEWED_PTR(mpResultFlag);
    
	Int32 i;
	for (i = 0; i < mDataSize; i++)
		mpResultFlag[i] = DssDataUnknown;
    
	mpNumericResultData = new double[mDataSize];
    
	mPrevStart = -1;
	mPrevEnd = -1;
	mPrevIndex = -1;
    
	switch (mFunctionIndex)
	{
        case DssFuncOLAPCount:
            if(GetDistinct())
                hNumericCountDistinct();
            else
            {
                for(i=0;i<mDataSize;i++)
                    hNumericCountByWindow(i);
            }
            break;
        case DssFuncOLAPMax:
        case DssFuncOLAPMin:
            if(GetDistinct())
                hNumericMaxMinDistinct();
            else
            {
                for(i=0;i<mDataSize;i++)
                    hNumericMaxMinByWindow(i);
            }
            break;
        case DssFuncLag:
        case DssFuncLead:
            for(i=0;i<mDataSize;i++)
                hNumericLagLead(i);
            break;
        default:
            return hr = E_FAIL;
	}
    
	return hr = S_OK;
}

// jxie, 2010-9-28
int  DSSOlapNNGeneric::hNumericCountDistinct()
{
	vector<double> values;
    
	Int32 i;
	for(i=0;i<mDataSize;++i)
	{
		if(mpDataFlag[i]==DssDataOk || GetCountNull())
		{
			values.push_back(mpNumericInputData[i]);
		}
	}
    
	Int32 count=0;
    
	if(values.size())
	{
		sort(values.begin(),values.end());
		vector<double>::iterator iter=values.begin();
		double previousValue=*iter;
		count = 1;
		for(iter++;iter!=values.end();iter++)
		{
			DSSDataFlag flag=DssDataOk;
			bool comparison = DoubleEqual(*iter,previousValue);
			if(flag==DssDataOk && comparison == false)
			{
				count++;
				previousValue = *iter;
			}
		}
	}
    
	double result = count;
    
	for(i=0;i<mDataSize;i++)
	{	
		// mpittkin: 327655, 10/08, match SQL behavior where this function returns null for null metric rows
		if(mpDataFlag[i] == DssDataOk)
		{
			mpNumericResultData[i] = result;
			mpResultFlag[i]=DssDataOk;
		}
		else
		{
			mpResultFlag[i] = mpDataFlag[i];
		}
	}
    
	return S_OK;
}

// jxie, 2010-9-28
int  DSSOlapNNGeneric::hNumericMaxMinDistinct()
{
	Int32 firstFlag =0;
	double result;
	for(Int32 i=0;i<mDataSize;i++)
	{
		if(mpDataFlag[i]==DssDataOk)
		{	
			double value = mpNumericInputData[i];
			if(firstFlag==0){
				firstFlag=1;
				result = value;
			}
			else
			{
				//DSSDataFlag flag=DssDataOk;
				if(result > value && mFunctionIndex==DssFuncOLAPMin)
					result = value;
				else if (result < value && mFunctionIndex==DssFuncOLAPMax)
					result = value;
			}
		}
	}
	for(Int32 i=0;i<mDataSize;i++)
	{	
		// match SQL behavior where this function returns null for null metric rows
		if(mpDataFlag[i] == DssDataOk)
		{
			mpNumericResultData[i] = result;
			mpResultFlag[i]=DssDataOk;
		}
		else
		{
			mpResultFlag[i] = mpDataFlag[i];
		}
	}
	return S_OK;
}

// jxie, 2010-9-28
int  DSSOlapNNGeneric::hNumericMaxMinByWindow(Int32 row)
{
	Int32 i, startRow, endRow;
	int hr=GetWindow(row,mDataSize,startRow,endRow);
	CHECK_HR;
    
	Int32 lRow = 0;
    
	if(mPrevIndex<0 || mPrevIndex<startRow || mPrevIndex>endRow)
	{
		// first round, or previous result is out of the current window
		// in these cases, we need to scan the whole window
		mPrevIndex=-1;
		for(i=startRow;i<=endRow;++i)
		{
			if (mpvDataByGroup)
				lRow = (*mpvDataByGroup)[i];
			else
				lRow = i;
            
			if (mpDataFlag[lRow]==DssDataOk)  // check valid data only
			{				
				if(mPrevIndex<0)  // the first valid element
					mPrevIndex=i;
				else
					hNumericMinMaxCompare(mPrevIndex,i,mPrevIndex);
			}
		}
	}
	else
	{
		// here is the optimization: we only scan the disjoint part of the window
		if(startRow<mPrevStart)
		{
			for(i=startRow;i<mPrevStart;++i)
				hNumericMinMaxCompare(mPrevIndex,i,mPrevIndex);
		}
		if(mPrevEnd<endRow)
		{
			for(i=mPrevEnd+1;i<=endRow;++i)
				hNumericMinMaxCompare(mPrevIndex,i,mPrevIndex);
		}
	}
    
	mPrevStart=startRow;
	mPrevEnd=endRow;
    
	// if there is any result, save it to the current row
	if(mPrevIndex>=0)
	{
		lRow = row;
		Int32 lRow2 = mPrevIndex;
		if (mpvDataByGroup)
		{
			lRow = (*mpvDataByGroup)[row];
			lRow2 = (*mpvDataByGroup)[mPrevIndex];
		}
        
		mpNumericResultData[lRow] = mpNumericInputData[lRow2];
		mpResultFlag[lRow]=DssDataOk;
	}
    
	return S_OK;
}

// jxie, 2010-9-28
void  DSSOlapNNGeneric::hNumericMinMaxCompare(Int32 row1, Int32 row2, Int32 &result)
{
	Int32 lRow1 = row1;
	Int32 lRow2 = row2;
	if (mpvDataByGroup)
	{
		lRow1 = (*mpvDataByGroup)[row1];
		lRow2 = (*mpvDataByGroup)[row2];
	}
    
	if(mpDataFlag[lRow1]==DssDataOk && mpDataFlag[lRow2]==DssDataOk)
	{
		double value1 = mpNumericInputData[lRow1];
		double value2 = mpNumericInputData[lRow2];
        
		if(mFunctionIndex==DssFuncOLAPMin)
			result = value1 > value2 ? row2 : row1;
		else if(mFunctionIndex==DssFuncOLAPMax)
			result = value1 > value2 ? row1 : row2;
	}
}

// jxie, 2010-9-28
int  DSSOlapNNGeneric::hNumericLagLead(Int32 row)
{
	int hr = S_OK;
    
	Int32 lnOffset = ChangeDoubleToInt( (mpNumericOffset[row]) );
    
	Int32 index=(mFunctionIndex==DssFuncLag)?row-lnOffset:row+lnOffset;
    
	if(index<0 || index>=mDataSize) // outlier rows, use default value
	{	
		// mpittkin 325608 : 10/08, fix lag lead to use vector for default value paramter
		if(mpDefaultValueFlag[row] == DssDataOk)
		{
			mpNumericResultData[row] = mpNumericDefaultValue[row];
		}
		mpResultFlag[row] = mpDefaultValueFlag[row];
	}
	else
	{
		mpNumericResultData[row] = mpNumericInputData[index];
		mpResultFlag[row] = DssDataOk;//mpDataFlag[row];	
	}
    
	return hr = S_OK;
}

// jxie, 2010-9-28
int  DSSOlapNNGeneric::hNumericCountByWindow(Int32 row)
{
	Int32 startRow, endRow;
	int hr=GetWindow(row,mDataSize,startRow,endRow);
	CHECK_HR;
    
	Int32 count=0;
	Int32 lRow2 = 0;
    
	if (row > 0 && startRow < mOldEndIndex &&
		startRow <= mOldStartIndex + 1 && startRow >= mOldStartIndex - 1 &&
		endRow <= mOldEndIndex + 1 && endRow >= mOldEndIndex - 1 )
	{
		count = mOldCount;
		if (startRow == mOldStartIndex - 1)
		{
			lRow2 = startRow;
			if (mpvDataByGroup)
				lRow2 = (*mpvDataByGroup)[startRow];
            
			if(mpDataFlag[lRow2] == DssDataOk || GetCountNull())
				count++;
		}
		else if (startRow == mOldStartIndex + 1)
		{
			lRow2 = mOldStartIndex;
			if (mpvDataByGroup)
				lRow2 = (*mpvDataByGroup)[mOldStartIndex];
            
			if(mpDataFlag[lRow2] == DssDataOk || GetCountNull())
				count--;
		}
        
		if (endRow == mOldEndIndex - 1)
		{
			lRow2 = mOldEndIndex;
			if (mpvDataByGroup)
				lRow2 = (*mpvDataByGroup)[mOldEndIndex];
            
			if(mpDataFlag[lRow2] == DssDataOk || GetCountNull())
				count--;
		}
		else if (endRow == mOldEndIndex + 1)
		{
			lRow2 = endRow;
			if (mpvDataByGroup)
				lRow2 = (*mpvDataByGroup)[endRow];
            
			if(mpDataFlag[lRow2] == DssDataOk || GetCountNull())
				count++;
		}
	}
	else
	{
		for(Int32 i=startRow;i<=endRow;++i)
		{
			lRow2 = i;
			if (mpvDataByGroup)
				lRow2 = (*mpvDataByGroup)[i];
            
			if(mpDataFlag[lRow2] == DssDataOk || GetCountNull())
				count++;
		}
	}
	mOldStartIndex = startRow;
	mOldEndIndex = endRow;
	mOldCount = count;
	
	Int32 lRow = row;
	if (mpvDataByGroup)
		lRow = (*mpvDataByGroup)[row];
    
	mpNumericResultData[lRow] = count;
	mpResultFlag[lRow]=DssDataOk;
    
	return S_OK;
}

// jxie, 2010-10-13
// support direct calculate
int  DSSOlapNNGeneric::DirectCalculate (Int32 nParam, Int32* ipSize, Int32** ipOffsets, 
                                        void** ipData, double* ipDivider, DSSDataFlag** ipFlags, VARIANT_BOOL* ipAllDataValid,
                                        EnumDSSDataType* iType, vector<Int32> * ipvDataByGroup,
                                        Int32 nResSize, double* opData, DSSDataFlag* opFlag, VARIANT_BOOL* opAllDataValid, 
                                        EnumNullCheckingOption iNullCheckingOption)
{
    int hr = S_OK;	
    
	if(!ipSize || ipSize[0] < 1 || nParam > 3)	return E_FAIL;
	
	if(!ipData)	return E_FAIL;
    
	// jxie, only support double array input now
	if (iType[0] != DssDataTypeDouble)
		return S_FALSE;
    
	mfIsDirect = true;
    
	// the 1st parameter
	if (ipvDataByGroup)
		mDataSize = ipvDataByGroup->size();
	else
		mDataSize = ipSize[0];
    
	mpDataFlag = ipFlags[0];
	mpNumericInputData = (double*)(ipData[0]);
	mpvDataByGroup = ipvDataByGroup;
	
	if(mFunctionIndex==DssFuncLag || mFunctionIndex==DssFuncLead)
	{
		// mpittkin, 325608, 325757 - 10/08
		// take the offset and default values as vectors, to support expressions as parameters
        
		// jxie, only support double array input now
		if (iType[1] != DssDataTypeDouble || iType[2] != DssDataTypeDouble)
			return S_FALSE;
		
		// the 2nd parameter
		mpOffsetFlag = ipFlags[1];
		mpNumericOffset = (double*)(ipData[1]);
        
		// the 3rd parameter
		mpDefaultValueFlag = ipFlags[2];
		mpNumericDefaultValue = (double*)(ipData[2]);
	}
    
	Int32 i;
	Int32 lRow = 0;
	if (mDataSize < 1)
		return hr = E_FAIL;	// input parameters not valid
    
	mpResultFlag = opFlag;
	mpNumericResultData = opData;
	
	mPrevStart = -1;
	mPrevEnd = -1;
	mPrevIndex = -1;
    
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
        case DssFuncOLAPCount:
            if(GetDistinct())
                hDirectCountDistinct();
            else
            {
                for(i=0;i<mDataSize;i++)
                {
                    hNumericCountByWindow(i);
                }
            }
            break;
        case DssFuncOLAPMax:
        case DssFuncOLAPMin:
            if(GetDistinct())
                hDirectMaxMinDistinct();
            else
            {
                for(i=0;i<mDataSize;i++)
                {
                    hNumericMaxMinByWindow(i);
                }
            }
            break;
        case DssFuncLag:
        case DssFuncLead:
            for(i=0;i<mDataSize;i++)
            {
                if (mpvDataByGroup != NULL)
                    lRow = (*mpvDataByGroup)[i];
                else
                    lRow = i;
                
                hNumericLagLead(lRow);
            }
            break;
        default:
            return hr = E_FAIL;
	}
    
	return hr = S_OK;
}

// jxie, 2010-10-13
// support direct calculate on all functions in this class 
bool DSSOlapNNGeneric::SupportDirectCalculate()
{
	return true;
}

// jxie, 2010-11-9
int  DSSOlapNNGeneric::hDirectCountDistinct()
{
	vector<double> values;
    
	Int32 i;
	Int32 lRow = 0;
	for(i=0;i<mDataSize;++i)
	{
		if (mpvDataByGroup != NULL)
			lRow = (*mpvDataByGroup)[i];
		else
			lRow = i;
        
		if(mpDataFlag[lRow]==DssDataOk || GetCountNull())
		{
			values.push_back(mpNumericInputData[lRow]);
		}
	}
    
	Int32 count=0;
    
	if(values.size())
	{
		sort(values.begin(),values.end());
		vector<double>::iterator iter=values.begin();
		double previousValue=*iter;
		count = 1;
		for(iter++;iter!=values.end();iter++)
		{
			DSSDataFlag flag=DssDataOk;
			bool comparison = DoubleEqual(*iter,previousValue);
			if(flag==DssDataOk && comparison == false)
			{
				count++;
				previousValue = *iter;
			}
		}
	}
    
	double result = count;
    
	for(i=0;i<mDataSize;i++)
	{	
		if (mpvDataByGroup != NULL)
			lRow = (*mpvDataByGroup)[i];
		else
			lRow = i;
        
		// mpittkin: 327655, 10/08, match SQL behavior where this function returns null for null metric rows
		if(mpDataFlag[lRow] == DssDataOk)
		{
			mpNumericResultData[lRow] = result;
			mpResultFlag[lRow]=DssDataOk;
		}
		else
		{
			mpResultFlag[lRow] = mpDataFlag[lRow];
		}
	}
    
	return S_OK;
}

// jxie, 2010-11-9
int  DSSOlapNNGeneric::hDirectMaxMinDistinct()
{
	Int32 firstFlag =0;
	double result;
	Int32 lRow = 0;
	for(Int32 i=0;i<mDataSize;i++)
	{
		if (mpvDataByGroup != NULL)
			lRow = (*mpvDataByGroup)[i];
		else
			lRow = i;
        
		if(mpDataFlag[lRow]==DssDataOk)
		{	
			double value = mpNumericInputData[lRow];
			if(firstFlag==0){
				firstFlag=1;
				result = value;
			}
			else
			{
				if(result > value && mFunctionIndex==DssFuncOLAPMin)
					result = value;
				else if (result < value && mFunctionIndex==DssFuncOLAPMax)
					result = value;
			}
		}
	}
	for(Int32 i=0;i<mDataSize;i++)
	{	
		if (mpvDataByGroup != NULL)
			lRow = (*mpvDataByGroup)[i];
		else
			lRow = i;
        
		// match SQL behavior where this function returns null for null metric rows
		if(mpDataFlag[lRow] == DssDataOk)
		{
			mpNumericResultData[lRow] = result;
			mpResultFlag[lRow]=DssDataOk;
		}
		else
		{
			mpResultFlag[lRow] = mpDataFlag[lRow];
		}
	}
	return S_OK;
}
