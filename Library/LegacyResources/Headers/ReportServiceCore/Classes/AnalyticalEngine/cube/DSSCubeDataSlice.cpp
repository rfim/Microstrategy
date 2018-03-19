/*
 *  DSSCubeDataSlice.cpp
 *  DashboardViewer
 *
 *  Created by Xiaolong Wang on 12/29/09.
 *  Copyright 2010 MicroStrategy, Inc.. All rights reserved.
 *
 */

#import <stdio.h>
#include "DSSCubeDataSlice.h"
#import "DSSDataColumn.h"
#import "DSSTabularIndex.h"
#import "DSSTabularData.h"
#import "DSSCube.h"
#import "DSSTabularIndexPool.h"
#import "DSSTabularAttribute.h"
#import "MCE/MCESchema.h"
#import "DSSGlobalLookUpTable.h"

#include <algorithm>
DSSCubeDataSlice::DSSCubeDataSlice(): mpIndex(NULL), mpColumn(NULL), mpThresholdID(NULL), mfCreate(false), mMetricPos(-1), mpMask(NULL), mpCondition(NULL), mUnitPos(-1), mpRefColumn(NULL), mfSFBFlag(false),
mpLocalToGlobalMap(NULL), mbColumnIsShared(false), mOrigRowCount(-1){}

DSSCubeDataSlice::~DSSCubeDataSlice()
{
    //tliao, 900323, 5/27/2014, add ColumnIsShared flag to avoid delete twice
	if (mpColumn && !mbColumnIsShared)
    {
		delete mpColumn;
        mpColumn = NULL;
    }
	
	if (mpThresholdID)
    {
		delete[] mpThresholdID;
        mpThresholdID = NULL;
    }
    
    if (mpMask)
    {
		delete mpMask;
        mpMask = NULL;
    }
    
    if (mpLocalToGlobalMap)
    {
        delete mpLocalToGlobalMap;
        mpLocalToGlobalMap = NULL;
    }
    
    VariantClear(&mMinValue);
    VariantClear(&mMaxValue);
}

int DSSCubeDataSlice::Init(DSSDataColumn *ipColumn, DSSTabularIndex *ipIndices, bool ibColumnShared)
{
	mpIndex = ipIndices;
	mpColumn = ipColumn;
    mbColumnIsShared = ibColumnShared;
	return S_OK;
}

int DSSCubeDataSlice::SetMetricPosInfo(bool ifCreate, int iMetricPos)
{
    mfCreate = ifCreate;
    mMetricPos = iMetricPos;
    return S_OK;
}

EnumDSSDataType DSSCubeDataSlice::getDataType()
{
    if(mpColumn ==NULL)
    {
        return mpRefColumn->getDataType();
    }
    else
    {
        if (mpColumn->getDivider() > 1)
        {
            return DssDataTypeDouble;
        }
        return mpColumn->getDataType();
    }
}

int DSSCubeDataSlice::getSizeOfData (int &orRowSize)
{
	if(mpColumn == NULL)
		return E_POINTER;
	orRowSize = mpColumn->getSizeOfData();
    if (mpColumn->getDivider() > 1) {
        orRowSize = sizeof(double);
    }
	return S_OK;
}

int DSSCubeDataSlice::getFlag(int nKey, int *ipKey, DSSDataFlag &orFlag)
{
	if(mpColumn == NULL || mpIndex == NULL)
		return E_POINTER;
	
	int lOffset = 0;
	int hr = mpIndex->LookUp(nKey, ipKey, &lOffset);
	CHECK_HR;
	
    if (mpMask && mpMask->Get(lOffset) == false)
	{
		orFlag = DssDataInvalid;
		return S_OK;
	}
    
	return mpColumn->getFlag(lOffset, orFlag);
}

int DSSCubeDataSlice::setVal(int nKey, int *ipKey, char *ipData, int iSizeInByte, EnumDSSDataType iType, DSSDataFlag iFlag)
{
	if(mpColumn == NULL || mpIndex == NULL)
		return E_POINTER;
	int lOffset = -1;
	int hr = mpIndex->LookUp(nKey, ipKey, &lOffset);
	CHECK_HR;
	return mpColumn->setData(lOffset, ipData, iSizeInByte, iFlag);
}

int DSSCubeDataSlice::getVal(int nKey, int *ipKey, const char **oppData, int &orSize, EnumDSSDataType &orType, DSSDataFlag &orFlag, int *opThresholdID)
{
	if (!mpIndex)		//index is yet to be created.
		return S_OK;
    
    DSSDataColumn* lpColumn = mpColumn ? mpColumn : mpRefColumn;
	if(lpColumn == NULL || mpIndex == NULL)
		return E_POINTER;
    
	int lOffset = 0;
	int hr = mpIndex->LookUp(nKey, ipKey, &lOffset);
	CHECK_HR;
	
    
    if (mpMask && mpMask->Get(lOffset) == false)
	{
        *oppData = NULL;
		orFlag = DssDataInvalid;
		return S_OK;
	}
    
	if (opThresholdID)
	{
		if (!mpThresholdID)
			*opThresholdID = 0;
		else
			*opThresholdID = mpThresholdID[lOffset];
	}
	
    return lpColumn->getData(lOffset, oppData, orSize, orType, orFlag) ;
}

int DSSCubeDataSlice::setValByOffset(int iOffset, char *ipData, int iSizeInByte, EnumDSSDataType iType, DSSDataFlag iFlag)
{
    if (mpMask && mpMask->Get(iOffset) == false)
	{
		//don't know if this will happen. if it happens, there's nothing we should set
		_ASSERT(false);
		return S_OK;
	}
    
	if(mpColumn == NULL)
		return E_POINTER;
	return mpColumn->setData(iOffset, ipData, iSizeInByte, iFlag);
}

int DSSCubeDataSlice::getValByOffset(int iOffset, const char **oppData, int &orSize, EnumDSSDataType &orType, DSSDataFlag &orFlag, int *opThresholdID)
{
    if (mpMask && mpMask->Get(iOffset) == false)
	{
		*oppData = NULL;
		orSize = 0;
		orFlag = DssDataInvalid;
		return S_OK;
	}
    
	if(mpColumn == NULL)
		return E_POINTER;
    
	if (opThresholdID)
	{
		if (!mpThresholdID)
			*opThresholdID = 0;
		else
			*opThresholdID = mpThresholdID[iOffset];
	}
	return mpColumn->getData(iOffset, oppData, orSize, orType, orFlag);
}

int DSSCubeDataSlice::getNumeric(int nKey, int *ipKey, double &orVal, DSSDataFlag &orFlag, int *opThresholdID)
{
	if (!mpIndex)		//index is yet to be created.
		return S_OK;
	
	if(mpColumn == NULL || mpIndex == NULL)
		return E_POINTER;
    
	int lOffset = 0;
	int hr = mpIndex->LookUp(nKey, ipKey, &lOffset);
	CHECK_HR;
	
	if (opThresholdID)
	{
		if (!mpThresholdID)
			*opThresholdID = 0;
		else
			*opThresholdID = mpThresholdID[lOffset];
	}
    
    if (mpMask && mpMask->Get(lOffset) == false)
	{
		orVal = 0;
		orFlag = DssDataInvalid;
		return S_OK;
	}
    
	return mpColumn->getNumericVal(lOffset, orVal, orFlag);
}

int DSSCubeDataSlice::getNumericByOffset(int iOffset, double &orVal, DSSDataFlag &orFlag, int *opThresholdID)
{
	if(mpColumn == NULL)
		return E_POINTER;
    
	if (opThresholdID)
	{
		if (!mpThresholdID)
			*opThresholdID = 0;
		else
			*opThresholdID = mpThresholdID[iOffset];
	}
    
	return mpColumn->getNumericVal(iOffset, orVal, orFlag);
}

int DSSCubeDataSlice::setNumericByOffset(int iOffset, double &irVal, DSSDataFlag &irFlag)
{
	if (!mpColumn)
		return E_POINTER;
	return mpColumn->setNumeric(iOffset, irVal, irFlag);
}


int DSSCubeDataSlice::setValueAndThreshold(int nKey, int *ipKey, char *ipData, int iSizeInByte, EnumDSSDataType iType, DSSDataFlag iFlag, int iThresholdID)
{
	if(mpColumn == NULL || mpIndex == NULL)
		return E_POINTER;
	int lOffset = -1;
	int hr = mpIndex->LookUp(nKey, ipKey, &lOffset);
	CHECK_HR;
	
	hr = mpColumn->setData(lOffset, ipData, iSizeInByte, iFlag);
	CHECK_HR;
    
	if (iThresholdID > 0)
	{
		if (!mpThresholdID)
		{
			mpThresholdID = new unsigned char[mpIndex->Count()];
			CHECK_PTR(mpThresholdID);
			memset(mpThresholdID, 0, sizeof(unsigned char) * mpIndex->Count());
		}
		
		mpThresholdID[lOffset] = (unsigned char)iThresholdID;
	}
	return S_OK;
}

int DSSCubeDataSlice::setValueAndThresholdByOffset(long long offset, char *ipData, int iSizeInByte, EnumDSSDataType iType, DSSDataFlag iFlag, int iThresholdID)
{
	if(mpColumn == NULL)
    {
        if(IsConstantSlice())
            return S_OK;
  		return E_POINTER;          
    }

	
	int hr = mpColumn->setData(offset, ipData, iSizeInByte, iFlag);
	CHECK_HR;
    
	if (iThresholdID > 0)
	{
		if (!mpThresholdID)
		{
			mpThresholdID = new unsigned char[mpColumn->getRowCount()];
			CHECK_PTR(mpThresholdID);
			memset(mpThresholdID, 0, sizeof(unsigned char) * mpColumn->getRowCount());
		}
		
		mpThresholdID[offset] = (unsigned char)iThresholdID;
	}
	return S_OK;
}

int DSSCubeDataSlice::size()
{
	int lSize = sizeof(this);
    
	if (mpColumn)
		lSize += mpColumn->size();
	
	if (mpThresholdID && mpIndex)
	{
		lSize += sizeof(unsigned char) * mpIndex->Count();
	}
	return lSize;
}

int DSSCubeDataSlice::updateValueWithString(int offset, char *ipString, int iStrLength)
{
	if(mpColumn == NULL)
		return E_POINTER;
	
	int hr = S_OK;
	
	if (ipString == NULL)
	{
		//set as NULL
		return mpColumn->setData(offset, ipString, iStrLength, DssDataNull);
	}
	
	EnumDSSDataType lDataType = mpColumn->getDataType();
	if (lDataType == DssDataTypeBigDecimal)
	{
		MDataType::DSSBigDecimal lTempBD(ipString);
		
		hr = mpColumn->setData(offset, (char*)&lTempBD, sizeof(MDataType::DSSBigDecimal), DssDataOk);
		CHECK_HR;
	}
	else if (lDataType != DssDataTypeVarChar && lDataType != DssDataTypeBinary
	       && lDataType != DssDataTypeUTF8Char)    //606272 UTF8 char
	{
		int lBufferSize = 0;
		lBufferSize = mpColumn->getSizeOfData();
		CHECK_HR;
		
		vector<char> lvNumericalData(lBufferSize);
		//fali, 926979, 895676, special handling for case that divider > 1
        if (mpColumn->getDivider() > 1 && lDataType == DssDataTypeInteger)
        {
            lDataType = DssDataTypeDouble;
            lBufferSize = sizeof(double);
        }
		int lDataSize = 0;
		hr = DSSAEHelper::ConvertStringToData(ipString, lDataType, &lvNumericalData[0], lBufferSize, lDataSize);
		CHECK_HR;
        //fali, 926979, 895676, special handling for case that divider > 1
        if (mpColumn->getDivider() > 1)
        {
            lDataSize = sizeof(int);
        }
		hr = mpColumn->setData(offset, &lvNumericalData[0], lDataSize, DssDataOk);
		CHECK_HR;
	}
	else
	{
		hr = mpColumn->setData(offset, ipString, iStrLength + 1, DssDataOk);	//include the terminate character '\0'
		CHECK_HR;
	}
	
	return S_OK;
}

void DSSCubeDataSlice::RecycleMemory()
{
	if (mpColumn)
		mpColumn->RecycleMemory();
	return;
}

int DSSCubeDataSlice::SetThreshold(DSSDataColumn *lpThresholdColumn)
{
    int hr = S_OK;
    if(!lpThresholdColumn)
        return hr;
    
    if (!mpThresholdID)
    {
        mpThresholdID = new unsigned char[mpIndex->Count()];
        CHECK_PTR(mpThresholdID);
        memset(mpThresholdID, 0, sizeof(unsigned char) * mpIndex->Count());
    }

    for(int i = 0; i < mpIndex->Count(); ++i)
    {
        double ldThreshold;
        DSSDataFlag lFlag;
        hr = lpThresholdColumn->getNumericVal(i, ldThreshold, lFlag);
        CHECK_HR;
        
        if(lFlag != DssDataOk)
        {
            ldThreshold = 0;
        }
        
        int lThreshold = ldThreshold;
        mpThresholdID[i] = (unsigned char)lThreshold;
    }
    return hr;
}

int DSSCubeDataSlice::CopyThreshold(DSSCubeDataSlice *ipSourceDataSlice)
{
    int hr = S_OK;
	if (ipSourceDataSlice->mpThresholdID && mpColumn && ipSourceDataSlice->mpColumn)
	{
		int lnRowCount = mpColumn->getRowCount();
		_ASSERT(ipSourceDataSlice->mpColumn->getRowCount() == lnRowCount);
		
		if (mpThresholdID)
		{
			delete [] mpThresholdID;
		}
		
		mpThresholdID = new unsigned char[lnRowCount];
		CHECK_NEWED_PTR(mpThresholdID);
		
		memmove(mpThresholdID, ipSourceDataSlice->mpThresholdID, sizeof(unsigned char) * lnRowCount);
	}
	return hr;
}

int DSSCubeDataSlice::setThresholdSlice(DSSCubeDataSlice *ipThreshholdSlice)
{
    int hr = S_OK;
    if (mpThresholdID)
    {
        delete [] mpThresholdID;
        mpThresholdID = NULL;
    }
    
    if(!mpColumn || !ipThreshholdSlice->mpColumn)
        return hr;
    
    int lnRowCount = mpColumn->getRowCount();
    
    if (lnRowCount < 0) {
        return E_INVALIDARG;
    }
    
    int lnThresholdRowCount = ipThreshholdSlice->mpColumn->getRowCount();
    // Veracode: CWE ID 195: Signed to Unsigned Conversion Error
    signed long long tmp = (signed long long) lnRowCount;
    if (tmp > 0 && tmp <= INT_MAX && ipThreshholdSlice->mpColumn && lnThresholdRowCount < lnRowCount)
    {
        if (1 == lnThresholdRowCount)
        {
            double lVal = .0;;
            DSSDataFlag lFlag = DssDataRWInvalid;
            hr = ipThreshholdSlice->getNumericByOffset(0, lVal, lFlag);
            CHECK_HR;
            
            char lThresholdID = (int)lVal;
            mpThresholdID = new unsigned char[lnRowCount];
            memset(mpThresholdID, 0, lnRowCount);
            
            for (long long i = 0; i < lnRowCount; ++i)
                mpThresholdID[i] = lThresholdID;
        }
        
        return hr;
    }
    if (tmp > 0 && tmp <= INT_MAX) {
        mpThresholdID = new unsigned char[lnRowCount];
    }
    // Veracode
    for(long long i = 0; i < lnRowCount; ++i)
    {
        double lVal;
        DSSDataFlag lFlag;
        hr = ipThreshholdSlice->getNumericByOffset(i, lVal, lFlag);
        int lIVal = lVal;
        mpThresholdID[i] = (unsigned char)lIVal;
    }
    return hr;
}

int DSSCubeDataSlice::setCondition(DFCSimpleExpr* ipCondition, DSSXTabBitVector* ipMask)
{
    mpCondition = ipCondition;
    
    //yimliu, 2014/07/18, 914818, memory leak. Create a copy of the BitVector ipMask points to. Instead of directly setting mpMask = ipMask.
    if (mpMask == NULL)
        mpMask = new DSSXTabBitVector;
    mpMask->Populate(ipMask);
    return S_OK;
}

//rewritten by Xhan 11/15/02
//The originial slice may be from different cube/tabulardata. If pUnitsMap != NULL, the originial slice is from
// different cube, if  pUnitsMap == NULL, then the original slice is from same tabular data and the function is equivalent to
// a duplication
int DSSCubeDataSlice::Populate (DSSCubeDataSlice *pOriginalSlice,
                                DSSTabularData *ipTabularData,
                                Int32 *pUnitsMap,
                                std::vector<Int32> *ipConvertedAttributes, 
                                //ICDSSSource *ipSource, 
                                bool iRestoreDouble, 
                                Int32** ippGToLMap1, 
                                IntToIntMap** ippGToLMap2,  
                                MCESchema * ipSchema)
{
    /*
	BEGIN_MCM_USAGE
	AE_ENTER_FUNC("DSSCubeDataSlice::Populate", AE_TRACE_COMMON);
    
	if(mfStatus != DSSXTAB_OBJ_UNINITIALIZED)
		return DSSXTAB_E_ALREADYINITIALIZED;
    
	//make it corrupted, when the whole load finished it will be set to initialized
	mfStatus = DSSXTAB_OBJ_CORRUPTED;
    
	mfCreate = true;
    */
    
    int hr = S_OK;
	AE_ASSERT(pOriginalSlice);
    
    
    hr = pOriginalSlice->get_OriginalRowCount(&mOrigRowCount);
	CHECK_HR;
    
	hr = pOriginalSlice->get_MetricValueRange(&mMinValue, &mMaxValue);
	CHECK_HR;
    
	DSSTabularIndex *lpOriginalIndices = pOriginalSlice -> getIndices();
	CHECK_PTR(lpOriginalIndices);
    
	Int32 lnUnit = lpOriginalIndices -> getNumUnits();
    
	Int32 lnOriginalUnit = lnUnit;
	Int32 lpOriginalUnit[lnUnit];
	Int32 lpUnit[lnUnit];
    
	hr = lpOriginalIndices -> getUnits(lnUnit, lpOriginalUnit);
	CHECK_HR;
    
	Int32 iUnit = 0;
	//ctang: sometimes in cube join, the index may in different order of original index
	bool lfNeedSort = false;
	if (pUnitsMap){
		for(iUnit = 0; iUnit < lnUnit; iUnit ++)
		{
			lpUnit[iUnit] = pUnitsMap[lpOriginalUnit[iUnit]];
			if (iUnit > 0 && lpUnit[iUnit - 1] >= lpUnit[iUnit])// if the lpUnit is not asending, need to do sort.
				lfNeedSort = true;
		}
	} else { // slices are in the same tabular data structure, we don't need sort
		for(iUnit = 0; iUnit < lnUnit; iUnit ++)
		{
			lpUnit[iUnit] = lpOriginalUnit[iUnit];
		}
	}
    
	// The metric level needs to be recomputed. 
	// lweng, 2012-3-7, TQMS 558935, if this a SFB metric, keep the metric level
    
	bool lfSFBSlice = false;
	lfSFBSlice = pOriginalSlice->IsSFBSlice();
    
	// lweng, 2012-5-24, TQMS 592525, set the SFB flag
	mfSFBFlag = lfSFBSlice;
    /*
	if (ipConvertedAttributes && ipConvertedAttributes->size() > 0 && !lfSFBSlice)
	{
		vector<Int32> lNewUnits;
		hr = HandleDEIndex(pMyTabularData, ipSource, lnUnit, lpUnit, ipConvertedAttributes, lNewUnits, ipSchema);
		CHECK_HR;
        
		if (lNewUnits.size() > lnUnit)
		{
			lnUnit = lNewUnits.size();
			lpUnit = reinterpret_cast<Int32 *>(_alloca(sizeof(Int32) * lnUnit));
			if (lnUnit > 0)
			{
				memmove(lpUnit, &lNewUnits[0], sizeof(Int32)*lnUnit);
			}
		}
	}
    */
	//ctang 2011-5-11
	if (lfNeedSort)
		sort(lpUnit, lpUnit + lnUnit);
    
	DSSTabularIndexPool * lpIndicesPool = ipTabularData->getIndexPool();
    CHECK_PTR(lpIndicesPool);
	hr = lpIndicesPool -> getIndex(lnUnit, lpUnit, &mpIndex);
    //fali, 934758, it may return E_OUT_OF_MEMORY status, seperate CHECK_HR and CHECK_PTR
    CHECK_HR;
	CHECK_PTR(mpIndex);
	//mpIndex->AddReference(); // ldong, 07/28/2010
    
	Int32 lRowCount = mpIndex -> Count();
    
	// weiwang 2010-06-07 global to local
	Int32* lGToLMap[lnUnit];
	IntToIntMap* lGToLMap2[lnUnit];
	memset(lGToLMap, 0, sizeof(Int32*) * lnUnit);
	memset(lGToLMap2, 0, sizeof(IntToIntMap*) * lnUnit);
    
	if (ippGToLMap1 && ippGToLMap2)
	{
		for (iUnit = 0; iUnit < lnUnit; iUnit++)
		{
			lGToLMap[iUnit] = ippGToLMap1[lpUnit[iUnit]];
			lGToLMap2[iUnit] = ippGToLMap2[lpUnit[iUnit]];
		}
	}
    
	//need to get indices one by one from the
	//later on need to take the filter into consideration
	Int32 lRowSize = 0;
	hr = pOriginalSlice -> getSizeOfData(lRowSize);
	CHECK_HR;
    
	DSSDataType_Type lDataType = pOriginalSlice -> getDataType();
    
	// weiwang 2009-06-05
	bool lbUTF8ToWChar = false;
	if (lDataType == DssDataTypeUTF8Char)
	{
		lDataType = DssDataTypeVarChar;
		lbUTF8ToWChar = true;
	}
    
    
    
	DSSDataColumn *lpOriginalColumn = pOriginalSlice -> getColumn();
    
	// qjiang 06/06/2009
	bool lfUseNumeric = false;
    
	double lDivider = static_cast<DSSDataColumn*>(lpOriginalColumn) -> getDivider();
	EnumDSSDataType lOrigType = lpOriginalColumn -> getOriginalType();
	if (lDivider > 1 || (iRestoreDouble && lOrigType == DssDataTypeDouble))
	{
		lDataType = DssDataTypeDouble;
		lRowSize = sizeof(double);
        
		lfUseNumeric = true;
	}
    /*
	hr = hNewColumnWithLockCheck(lDataType,
                                 lbUTF8ToWChar ? 0 : lRowSize,
                                 lRowCount,
                                 mpBuffer,
                                 &mpColumn);
	CHECK_HR;
    */
    hr = DSSAEHelper::GenXTabCol(lDataType, lRowCount, &mpColumn);
    CHECK_HR_PTR(mpColumn);
    
	//need to get indices one by one from the
	//later on need to take the filter into consideration
    
    char *lpData = new char[lRowSize];
    CHECK_NEWED_PTR(lpData);
    MBase::StrongArrayPtr<char> lpStrongPtrData(lpData);

	DSSData_Flags lFlag;
	double lVal = 0;
    /*
	CComPtr<ICDSSXTabColumn> lpSrcCol;
	hr = pOriginalSlice->get_Column(&lpSrcCol);
    */
    DSSDataColumn *lpSrcCol = pOriginalSlice -> getColumn();
	//this code needed to be changed later
	if (hr != S_OK){
        
		if (lnUnit == 0){//suppose it is a constant slice
            
			if (lfUseNumeric)
			{
				hr = pOriginalSlice->getNumeric(0, NULL, lVal, lFlag);
				CHECK_HR;
                
				hr = mpColumn->setNumeric(0, lVal, lFlag);
				CHECK_HR;
			}
			else
			{
				hr = pOriginalSlice->getVal(
                                             0,
                                             NULL,
                                             (const char**)&lpData,
                                             lRowSize,
                                             lDataType,
                                             lFlag);
				CHECK_HR;
                
				// weiwang 2009-06-01
				if (lbUTF8ToWChar)
				{
					const MBase::ReturnWString lString(MBase::UTF8ToWideChar((char*)lpData));//CDSSAEHelper::UTF8ToWideCharEx((char*)lpData));
					hr = mpColumn -> setData(0,
                                             (char*)(lString.c_str()),
                                             lRowSize,
                                             lFlag);
				}
				else
					hr = mpColumn -> setData(0,
                                             lpData,
                                             lRowSize,
                                             lFlag);
				CHECK_HR;
			}
		} else
			return hr = E_FAIL;
        
	} else {
		//The original slice is in the same tabular data, and has the same index structure
        
		// aye, 287516, 11/3/07,
		// with the DE, the above assumption is no longer valid. The new index may be different than the original index
		// e.g., input index is Day, while there is a DE on Year, causing it to change to CustomGroup
		// the new index becomes (YearCG, Day), which has the same number of rows as {Day} index,
		// but they are in different sorting order. First one is sorted by {Year, Day}, the other sorted
		// by {Day}
        
		// At the moment, we have two index, lInputIndex, lOutputIndex
		// For the output index, it contains units in lpUnits
		// The input index contains lpOriginalUnits, which can be translated to lpUnits by pUnitsMap
        
		// One example:
		// Base Template: Year, Quarter, Month, Category,
		// InputIndex: (2, 3), i.e., (Month, Category)
        
		// View Template: Category, Month, Quarter, DE defined on Quarter
		// OutputIndex: (0, 1, 2), i.e., (Category, Month, QuarterCG)
		// pUnitsMap: [x, 2, 1, 0]
		// We should go through the output index, for each row of it, get the keys, and lookup in the
		// input index
        
		// When the two indices are different, we check their keys to see if we need to perform any lookup
		Int32 lDifferentKeys = 1;	// change default to 1
		
		if (lnOriginalUnit != lnUnit)
		{
			// if the original index is at lowest level, the current index must be at the same order as the
			// original one. We made effort to make sure that the same order is kept when copying over the
			// index.
			if (lpOriginalIndices->getKeyTable() == NULL)
				lDifferentKeys = 0;
			else
				lDifferentKeys = 1;
		}
		else if (lfNeedSort)//ctang: if the index is not in same order, the key is different.
			lDifferentKeys = 1;
		else if (mpIndex != lpOriginalIndices)
		{
			if (mpIndex->getKeyTable() && lpOriginalIndices->getKeyTable())
			{
                if (mpIndex->Count() != lpOriginalIndices->Count())
                {
                    lDifferentKeys = 1;
                }
                else
                {
                    lDifferentKeys = memcmp(mpIndex->getKeyTable(),
                                         lpOriginalIndices->getKeyTable(),
                                         sizeof(Int32)*lnUnit*lRowCount);                    
                }
			}
			// jxie, if the lpOriginalIndices has IndexSpace but not at lowest level, still need to do the lookup
			else if (lpOriginalIndices->getKeyTable() == NULL && 
                     lpOriginalIndices->IsMainIndexOrder() && 
                     mpIndex->IsMainIndexOrder() )
				lDifferentKeys = 0;
		}
		else
			lDifferentKeys = 0;
        
		vector<Int32> lvKey;
		vector<Int32> lvOriginalKey;
		if (lDifferentKeys)
		{
			// Get the unit number again, in case the units order is different in the index
			hr = mpIndex->getUnits(lnUnit, lpUnit);
			CHECK_HR;
            
			// find out how the units are mapped from lpOriginalUnits to lpUnits
			Int32 lMaxOriginalUnit = 0;
			for (Int32 i = 0; i < lnOriginalUnit; i++)
				if (lpOriginalUnit[i] > lMaxOriginalUnit)
					lMaxOriginalUnit = lpOriginalUnit[i];
			lvOriginalKey.resize(lMaxOriginalUnit+1);
            
			Int32 lMaxUnit = 0;
			for (Int32 j = 0; j < lnUnit; j++)
				if (lpUnit[j] > lMaxUnit)
					lMaxUnit = lpUnit[j];
			lvKey.resize(lMaxUnit+1);
		}
        
		DSSXTabBitVector* lpMask = pOriginalSlice->GetMask();
        
		// lweng, 2012-06-07, copy the trim string flag
		/*
        bool lbTrimFlag;
		hr = lpSrcCol->GetTrimFlagC(&lbTrimFlag);
		CHECK_HR;
        
		hr = mpColumn->SetTrimFlagC(lbTrimFlag);
		CHECK_HR;
        */
		Int32 lnOrignalRowCount = lpSrcCol->getRowCount();
        
		for(Int32 iRow = 0; iRow < lRowCount; iRow ++)
		{
			Int32 lSrcRow = iRow;
			bool lIsNewRow = false;
			
			if (lDifferentKeys)
			{
				// Get the keys from mpIndex, then look up the lpOriginalIndices
				hr = mpIndex->Item(iRow, lvKey.size(), &lvKey[0]);
				CHECK_HR;
                
				//ctang 2011-1-21
				if (ippGToLMap1 && ippGToLMap2)
				/*if (lpGUnitContainer)*/
				{
					for (Int32 i = 0; i < lnUnit; i++)
					{
						if (lGToLMap[i])
							lvKey[lpUnit[i]] = lGToLMap[i][lvKey[lpUnit[i]]];
						else if (lGToLMap2[i])
							lvKey[lpUnit[i]] = (*(lGToLMap2[i]))[lvKey[lpUnit[i]]];
					}
				}
                
				for (Int32 i = 0; i < lnOriginalUnit; i++)
				{
					Int32 lOriginalUnit = lpOriginalUnit[i];
					Int32 lNewUnit = pUnitsMap[lOriginalUnit];
					lvOriginalKey[lOriginalUnit] = lvKey[lNewUnit];
				}
                
				hr = lpOriginalIndices->LookUp(lvOriginalKey.size(), &lvOriginalKey[0], &lSrcRow);
				//in global RWD case, sometimes the row in target slice may not exist in orignal slice
				//set it as null
				if (hr == S_FALSE)
				{
					lIsNewRow = true;
				}
				else
					CHECK_HR;
			}
			//ctang: sometimes the row in target slice may not exist in orignal slice
			else if (lSrcRow >= lnOrignalRowCount )
				lIsNewRow = true;
            
			// conditional metric
			if (lpMask && lpMask->Get(lSrcRow) == 0)
			{
				//mpColumn->setFlag(iRow, DssDataIgnoreCheckNull); not supported
				continue;
			}
            
			if (lIsNewRow)
			{
				//set as null
				hr = mpColumn->setFlag(iRow, DssDataInvalid);
				CHECK_HR;
			}
			else
			{
				if (lfUseNumeric)
				{
					hr = lpSrcCol->getNumericVal(lSrcRow, lVal, lFlag);
					CHECK_HR;
                    
					hr = mpColumn->setNumeric(iRow, lVal, lFlag);
					CHECK_HR;
				}
				else
				{
					hr = lpSrcCol->getData(	lSrcRow,
                                           (const char**)&lpData,
                                           lRowSize,
                                           lDataType,
                                           lFlag);
					CHECK_HR;
                    
					// weiwang 2009-06-01
					if (lbUTF8ToWChar)
					{
                        const MBase::ReturnWString lString(MBase::UTF8ToWideChar((char*)lpData));//CDSSAEHelper::UTF8ToWideCharEx((char*)lpData));
                        hr = mpColumn -> setData(iRow,
                                                 (char*)(lString.c_str()),
                                                 lRowSize,
                                                 lFlag);
					}
					else
						hr = mpColumn -> setData(iRow,
                                                 lpData,
                                                 lRowSize,
                                                 lFlag);
					CHECK_HR;
				}
                
			}
            
		}
        
	}
    
	//mfStatus = DSSXTAB_OBJ_INITIALIZED;
	return hr = S_OK;

}

int DSSCubeDataSlice::hFindUnitPosition(DSSTabularIndex* ipIndex, Int32 iUnit, Int32 &orUnitPos)
{
	int hr = S_OK;
    
	orUnitPos = -1;
    
	Int32 lnUnits = ipIndex->getNumUnits();
    
	Int32 lpUnits[lnUnits];
	hr = ipIndex->getUnits(lnUnits, lpUnits);
	CHECK_HR;
    
	if (iUnit < lnUnits && iUnit == lpUnits[iUnit])
	{
		orUnitPos = iUnit;
		return S_OK;
	}
	else
	{
		for (Int32 i = 0; i < lnUnits; i++)
		{
			if (iUnit == lpUnits[i])
			{
				orUnitPos = i;
				return S_OK;
			}
		}
		return S_FALSE;
	}
}

int DSSCubeDataSlice::AttrFromInit(DSSTabularData *ipTabularData, Int32 iUnit, Int32 iOffset, bool iUseLookupForAttributes, vector<int>* ipLocalToGlobalMap)
{
    int hr = S_OK;
    
    DSSTabularIndexPool* lpIndexPool = ipTabularData->getIndexPool();
    if (iUseLookupForAttributes)
    {
        hr = lpIndexPool->getIndex(1, &iUnit, &mpIndex);
        CHECK_HR_PTR(mpIndex);
        
        mUnitPos = 0;
    }
    else
    {
        hr = lpIndexPool->getMainIndex(&mpIndex);
        CHECK_HR_PTR(mpIndex);
        
        hr = hFindUnitPosition(mpIndex, iUnit, mUnitPos);
        CHECK_HR;
        
        assert(mUnitPos >= 0);
    }
    
    //tliao, 12/19/2013, if there is LocalToGlobalMap exsiting, we need to do convert
    //if (!iUseLookupForAttributes && ipLocalToGlobalMap != NULL && ipLocalToGlobalMap->size() != 0)
    //tliao, 946983, 2014/08/12, we don't need to use iUseLookupForAttributes
    if (ipLocalToGlobalMap != NULL && ipLocalToGlobalMap->size() != 0)
    {
        mpLocalToGlobalMap = new vector<int>();
        mpLocalToGlobalMap->assign(ipLocalToGlobalMap->begin(), ipLocalToGlobalMap->end());
    }
    
    DSSTabularUnit* lpUnit = ipTabularData->Item(iUnit);
    CHECK_PTR(lpUnit);
    
    DSSTabularAttribute* lpLookupTable = (DSSTabularAttribute*)lpUnit;
    
    DSSDataColumn *lpSrcCol = NULL;
    hr = lpLookupTable->Item(iOffset, &lpSrcCol);
    CHECK_HR_PTR(lpSrcCol);
    
    //11/14/2013, 810082, fali, handle the attribute column mapping to the index.
    //TQMS 239600 we can't simply combine index with attribute column as before, because the index iteself is for
    //the mapping between attribute elements and metric cells. So we have to do some reverse copy for
    // attribute column
    //DSSDataType_Type lDataType = DssDataTypeUnknown;
    //lDataType = lpSrcCol->getDataType();
    
    //tliao, 1/9/2014, 831053, 831056, the column contains join null in the last row on client side, change to use DSSGlobalLookUpTable
    //Int32 lRowCount = lpSrcCol->getRowCount()-1;
    Int32 lRowCount = ((DSSGlobalLookUpTable*)lpLookupTable)->CountRows();
    
    //Int32 lDataSize = 0;
    //hr = lpSrcCol->getRowSize(0, lDataSize);
    
    // aye, 318371, 7/9/2008
    // when duplicate rows are present, the index may contain duplicates therefore may be different
    // than the size of the lookup table.
    
    Int32 lIndexRowCount = mpIndex->Count();
    CHECK_HR;
    
    // aye, 303560, 8/14/2008
    // if the index uses is in the same order (0, 1, 2, ...), we should be able to reuse the column
    // weiwang 2009-08-28 TQMS372398 there may be unref elements at the end of the source column.
    // 854903, fali, add condition mpIndex->isOrderedLinearKey() as aye's explanation
    bool lReuseColumn = (lRowCount == lIndexRowCount) && mpIndex->isOrderedLinearKey();
    
    if (lReuseColumn)
    {
        mpColumn = lpSrcCol;
        mbColumnIsShared = true;
    }
    else
    {
        //qjiang 06/08/2009 TQMS 348157
        //if we cannot reuse the column, set mpColumn to NULL, and any operation on mpColumn will be redirected to mpRefColumn
        mpColumn = NULL;
        mpRefColumn = lpSrcCol;
        
        //ctang 2011-9-6: if type is unkown, try to build the column. else return fail
        //        if (lDataType == DssDataTypeUnknown)
        //        {
        //            hr = hCreateColumnFromRefColumn();
        //            CHECK_HR_PTR(mpColumn);
        //            mpRefColumn = NULL;
        //        }
        // do not create the column unless necessary as it is too costly and sometimes it is useless, it will happen in getColumn method
        
    }
    lpSrcCol->GetRange(&mMinValue, &mMaxValue);
    // aye, 306188, 6/23/2008
    // this is a slice created by AE calculation, i.e., it does not have a corresponding column in TabularData
    // set mfCreate to true
    mfCreate = true;
    //mfStatus = DSSXTAB_OBJ_INITIALIZED;

    return S_OK;
}

int DSSCubeDataSlice::hCreateColumnFromRefColumn()
{
    int hr = S_OK;
    
    // if mpColumn is NULL, we must use mpRefColumn
    CHECK_PTR(mpRefColumn);
    
    DSSDataType_Type lDataType = DssDataTypeUnknown;
    Int32 lDataSize = 0;
    Int32 lRowCount = 0;
    
    DSSXTabKeyIterator lIt(mpIndex);
    
    lDataType = mpRefColumn->getDataType();
    hr = mpRefColumn->getRowSize(0, lDataSize);
    lRowCount = mpIndex -> Count();
    
    //bool lfUseMergedColumn = mpRefColumn->IsMergedColumn();
    DSSAEHelper::GenXTabCol(lDataType, lRowCount, &mpColumn);
    DSSData_Flags lFlag;
    
    //    if (lDataType == DssDataTypeUnknown)
    //    {
    //        //ctang: it maybe the non-compatible column in global lookup table, search by the index to get correct column type
    //        DSSXTabKeyIterator lIt2(mpIndex);
    //        Int32 lSize = -1;
    //        DSSDataType_Type lRowType;
    //        unsigned char* lpTmp = NULL;      // jxie, 2012-9-5
    //        for (Int32 i = 0; i < lRowCount; i ++)
    //        {
    //            Int32 lKey = lIt2.GetCol(mUnitPos);
    //            lIt2.NextRow();
    //
    //            double ltmp;
    //            hr = mpRefColumn->GetDataC( lKey,
    //                                       &lpTmp,
    //                                       &lcbWritten,
    //                                       &lRowType,
    //                                       &lFlag,
    //                                       &ltmp);
    //            CHECK_HR;
    //            if (lSize < 0)
    //            {
    //                lDataType = lRowType;
    //                lSize = lcbWritten;
    //            }
    //            // rzhu, 08/16/2013, 783876, we can support numeric combine with string type
    //            else if(lRowType != DssDataTypeMissing && (!((CDSSAEHelper::IsNumeric(lDataType) || CDSSAEHelper::IsString(lDataType)) && (CDSSAEHelper::IsNumeric(lRowType) || CDSSAEHelper::IsString(lRowType))))
    //                    && lDataType != lRowType)
    //            {
    //                return AE_E_WRONGDATATYPE;
    //            }
    //            else if (lcbWritten > lSize)
    //            {
    //                lDataType = lRowType;
    //                lSize = lcbWritten;
    //            }
    //        }
    //        lDataSize = lSize;
    //    }
    
    // rzhu, 08/16/2013, 783690, for 0 rows and the dataype is DssDataTypeUnknown, we create a column with DssDataTypeMissing type
    //    if (lRowCount == 0)
    //    {
    //        hr = hNewColumnWithLockCheck(lDataType == DssDataTypeUnknown ? DssDataTypeMissing : lDataType, lDataSize, lRowCount, mpBuffer, &mpColumn);
    //        CHECK_HR;
    //        return S_OK;
    //    }
    
    //    unsigned char * lpData = (unsigned char *) _alloca (sizeof(unsigned char) * lDataSize);
    //    //the conversted size should not more then lDataSize
    //    unsigned char * lpConvertedData = (unsigned char *) _alloca (sizeof(unsigned char) * lDataSize);
    //    Int32 lConvertedSize = 0;
    //fali, we didn't have mpBuffer at client-side
    //    hr = hNewColumnWithLockCheck(lDataType, lDataSize, lRowCount, mpBuffer, &mpColumn);
    //    CHECK_HR;
    char* lCellData=NULL;
    DSSDataType_Type lRowType;
    for (Int32 i = 0; i < lRowCount; i ++)
    {
        Int32 lKey = lIt.GetCol(mUnitPos);
        lKey = mpLocalToGlobalMap ? (*mpLocalToGlobalMap)[lKey] : lKey;
        lIt.NextRow();
        
        hr = mpRefColumn->getData( lKey,
                                  (const char **)(&lCellData),
                                  lDataSize,
                                  lRowType,
                                  lFlag);
        CHECK_HR;
        
        //        if (lfUseMergedColumn && lRowType != lDataType)
        //        {
        //            //ctang: 712675, if type is not match, for New RWD case, try convert
        //            CComVariant lVar;
        //            int hr = S_OK;
        //
        //            hr = DFCData_ConvertToVariant(lpData, lRowType, lcbWritten, &lVar);
        //            if (hr != S_OK)
        //                return AE_E_WRONGDATATYPE;
        //
        //            hr = DFCData_VariantSize(&lVar, lDataType, &lConvertedSize);
        //            if (hr != S_OK)
        //                return AE_E_WRONGDATATYPE;
        //
        //            // weiwang 2009-06-05 TQMS352644 UTF8 support
        //            if (lDataType == DssDataTypeVarChar || lDataType == DssDataTypeUTF8Char)
        //            {
        //                if (lConvertedSize > lDataSize) { //reallocate result buffer if necessary
        //                    lDataSize = lConvertedSize;
        //                    lpConvertedData = new (mpBuffer->p) BYTE[lConvertedSize];
        //                    CHECK_NEWED_PTR(lpConvertedData);
        //                }
        //
        //            }
        //
        //            if (lConvertedSize > lDataSize)
        //                return E_UNEXPECTED;
        //
        //            hr = DFCData_ConvertFromVariant(&lVar, lDataType, (BYTE*)(lpConvertedData), &lConvertedSize);
        //            if (hr != S_OK)
        //                return AE_E_WRONGDATATYPE;
        //            
        //            lpData = lpConvertedData;
        //            lcbWritten = lConvertedSize;
        //        }
        hr = mpColumn->setData( i,
                                   lCellData,
                                   lDataSize,
                                   lFlag);
        CHECK_HR;
        
    }
    return S_OK;
}


DSSDataColumn * DSSCubeDataSlice::getColumn(){
    if (!mpColumn)
    {
        hCreateColumnFromRefColumn();
        mpRefColumn = NULL;
    }
    return mpColumn;
}

int DSSCubeDataSlice::get_ColumnC(DSSDataColumn **oppColumn, bool iCreate)
{
    if (!mpColumn && iCreate)
    {
        hCreateColumnFromRefColumn();
        mpRefColumn = NULL;
    }
    if (oppColumn)
        *oppColumn = mpColumn;
    return S_OK;
}

bool DSSCubeDataSlice::IsSFBSlice()
{
    return mfSFBFlag;
}
int DSSCubeDataSlice::setSFBFlag(bool iSFBFlag)
{
    mfSFBFlag = iSFBFlag;
    return S_OK;
}

int DSSCubeDataSlice::get_RefColumnC(DSSDataColumn **oppColumn)
{
    int hr = S_OK;
    
    CHECK_PTR(oppColumn);
    
    *oppColumn = mpRefColumn;
    
    return hr;
    
}

// translate the mask to real flag
int DSSCubeDataSlice::CompleteCondition()
{
	if (mpCondition && mpMask)
		//|| (mfConditionMask && mpMask))//calculate slice window for conditional logic support
	{
		if (mpMask->IsAllTrue())
			mpMask = NULL;
		else
		{
			// qjiang 02/10/2011 we support conditional metric for attr@Form now.
			/*
            DSSDataColumn* lpColumn = mpColumn;
			DSSDataColumn* lpRefColumn = mpRefColumn;
            
			if (lpColumn)
				lpColumn->Clone(&mpColumn, mpBuffer);
			else
				lpRefColumn->Clone(&mpRefColumn, mpBuffer);
            */
			DSSXTabKeyIterator lIt(mpIndex);
			lIt.SetRow(0);
            
			Int32 lRow = mpMask->GetLength();
			for (Int32 iRow = 0; iRow < lRow; iRow++)
			{
				if (mpMask->Get(iRow) == false)
				{
					// qjiang, add support new flag
					if (mpColumn)
						mpColumn->setFlag(iRow, DssDataIgnoreCheckNull);
					else
					{
						lIt.SetRow(iRow);
						Int32 lRow = lIt.GetCol(mUnitPos);
						mpRefColumn->setFlag(lRow, DssDataIgnoreCheckNull);
					}
				}
			}
            
			//mColumnIsShared = false;
		}
	}
	//mfConditionMask = false;
	mpCondition = NULL;
    
	return S_OK;
}


int DSSCubeDataSlice::get_MetricValueRange(VARIANT * pMin, VARIANT * pMax)
{
	int hr = S_OK;
    
	//zhyang, 615078
	//zhyang, 591659
	//hr = CalculateRange();
	//CHECK_HR;
    
	hr = VariantCopy(pMin, &mMinValue);
	CHECK_HR;
    
	hr = VariantCopy(pMax, &mMaxValue);
	CHECK_HR;
    
	return S_OK;
}

int DSSCubeDataSlice::put_MetricValueRange(VARIANT * pMin, VARIANT * pMax)
{
	int hr = S_OK;
    
	hr = VariantCopy(&mMinValue, pMin);
	CHECK_HR;
    
	hr = VariantCopy(&mMaxValue, pMax);
	CHECK_HR;
    
	return S_OK;
}

int DSSCubeDataSlice::get_OriginalRowCount(Int32* pRowCount)
{
	if (pRowCount == NULL)
		return E_POINTER;
    
	*pRowCount = mOrigRowCount;
    
	return S_OK;
}

int DSSCubeDataSlice::put_OriginalRowCount(Int32 nRowCount)
{
	mOrigRowCount = nRowCount;
    
	return S_OK;
}


int DSSCubeDataSlice::CalculateRange(DSSTabularData* ipTabularData)
{
	if (mMinValue.vt == VT_EMPTY)
	{// if not calculated, calculate it here
        
		// qjiang
		// it's confusing to get the count here, but to make things simple, we assume the count should go along with the range, which
		// is calculated before ML event
		mOrigRowCount = mpIndex->Count();
        
        //xiazhou: On server side, ipTabularData is passed in to handle DE.
        //But so far smart client can't handle DE.
		/*if (ipTabularData != NULL)
        {
            int lnUnits = mpIndex->getNumUnits();
            
            // TQMS 769475 ignore hidden rows when calculating range
            Int32* lpUnits = reinterpret_cast<Int32*>(_alloca(sizeof(Int32) * lnUnits));
            CHECK_NEWED_PTR(lpUnits);
            
            hr = mpIndex->getUnits(lnUnits, lpUnits);
            CHECK_HR;
            
            MAE::Vector<Int32> lvDEUnitIDs;
            MAE::Vector<ICDSSXTabConsolidation2*> lvDEUnitPtrs;
            
            // collect the DE units
            for (Int32 iUnit = 0; iUnit < lnUnits; iUnit++)
            {
                CComPtr<ICDSSTabularDataUnit> lpTDUnit;
                hr = ipTabularData->Item(lpUnits[iUnit], &lpTDUnit);
                CHECK_HR_PTR(lpTDUnit);
                
                SQIPtr(ICDSSXTabConsolidation2) lpCS2(lpTDUnit);
                
                if (lpCS2)
                {
                    VARIANT_BOOL lFlag = VARIANT_FALSE;
                    hr = lpCS2->get_IsDerivedElement(&lFlag);
                    CHECK_HR;
                    
                    if (lFlag == VARIANT_TRUE)
                    {
                        lvDEUnitPtrs.push_back(lpCS2.p);
                        lvDEUnitIDs.push_back(iUnit);
                    }
                }
            }
            
            Int32 lnDEUnit = lvDEUnitIDs.size();
            CDSSXTabBitVector lBV;
            
            if (lnDEUnit > 0)
            {
                // use a bitvector to remember which rows are hidden
                lBV.Init(mOrigRowCount, false);
                
                CDSSXTabKeyIterator lIt(mpIndices);
                lIt.SetRow(0);
                
                for (Int32 iRow = 0; iRow < mOrigRowCount; iRow++)
                {
                    bool lbHidden = false;
                    for (Int32 iDEUnit = 0; iDEUnit < lnDEUnit; iDEUnit++)
                    {
                        Int32 lKey = lIt.GetCol(lvDEUnitIDs[iDEUnit]);
                        
                        Int32 lOrdinal = -1;
                        hr = lvDEUnitPtrs[iDEUnit]->get_Ordinal(lKey, &lOrdinal);
                        CHECK_HR;
                        
                        // find one hidden, then this row should be ignored
                        if (lOrdinal < 0)
                        {
                            lbHidden = true;
                            break;
                        }
                    }
                    
                    if (!lbHidden)
                        lBV.Set(iRow, true);
                    
                    lIt.NextRow();
                }
            }
        }*/
        
		if (mpColumn)
			return mpColumn->GetRange(&mMinValue, &mMaxValue);
		else if (mpRefColumn)
			return mpRefColumn->GetRange(&mMinValue, &mMaxValue);
		else
			return E_FAIL;
	}
    
	return S_OK;
}
