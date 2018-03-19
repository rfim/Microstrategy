//
//  DSSOlapNNGeneric.h
//  ReportServiceCore
//
//  Created by Qiang Xue on 7/21/06.
//  Copyright 2006 MicroStrategy, Inc. All rights reserved.
//

#ifndef OlapNNGeneric_h
#define OlapNNGeneric_h 1

#import "DSSNNGenericFunction.h"
#import "Base/Base/Allocator.h"
#import "MsiCOM/MsiCOM/Msi_ccombstr.h"

class DSSOlapNNGeneric : public DSSNNGenericFunction
{    
    // jimzhang July/25/2011 begin : ported from CDSSOlapFunction in server  
private:
    typedef enum 
    {
		UnboundedPreceding = 0,
		UnboundedFollowing,
		CurrentRow,
		Preceding,
		Following
	} WindowBoundaryType;
    
    bool mDistinct;
	bool mCountNull;
	bool mHasWindow;
	Int32 mWinType;
	Int32 mWinBDNum;
	Int32 mWinStartType;
	Int32 mWinStartOffset;
	Int32 mWinEndType;
	Int32 mWinEndOffset;
//	CComBSTR mOlapPattern;
    BYTE* mOlapPattern;
    int mOlapPatternLength;
    
protected:
    /**
	 * Returns a value indicating whether the DISTINCT flag is set
	 */
	bool GetDistinct();
	
    /**
	 * Returns a value indicating whether to count NULL values in for OLAPCount
	 */
	bool GetCountNull();
	
    /**
	 * Calculates the window with which the current row calculation should be performed
	 */
	int GetWindow(Int32 currentIndex, Int32 dataSize, Int32& startIndex, Int32& endIndex);
    
public:
    /**
	 * Sets function properties.
	 * This method is required by ICDSSFunctionObject
	 */
	virtual int setProperty(Int32 Index, CComVariant *pValue);
    
    int get_PropertyInfo(Int32 PropertyIndex, BSTR *pName, BSTR *pDescription, VARIANT *pValue);    
    // jimzhang July/25/2011 end    
    
public:
    DSSOlapNNGeneric();
    
    ~DSSOlapNNGeneric();
    
    int Calculate ();
    
    int SetInputParam (Int32 nParamIndex, Int32 nSize, EnumDSSDataType dataType, CComVariant *pData, DSSDataFlag *pFlag);
    
    int GetOutput (Int32 nSize,         // Input buffer size
                   Int32 *nDataSize, 	// return data size
                   CComVariant *pData,      // Double array stores the result of function
                   DSSDataFlag *pFlag	// Output flags
                   );
    
    int GetOutputType (Int32 nSize, EnumDSSDataType *pInputType, EnumDSSDataType *pOutputDataType, EnumDSSParameterType *pOutParamType);
    
    int GetInputParamType (Int32 nParamIndex, EnumDSSParameterType *pType);
    
    int IsNullProcessable (bool *pCanProcessNULL);
    
    int Clear ();
    
    // jxie, 2010-9-27
    // support numeric interface
    int GetOutput(Int32 nSize, Int32 *nDataSize,  double * pData, DSSDataFlag * pFlag);
    
    int GetOutputType(EnumDSSParameterType * pType);
    
    int SetInputParam(Int32 nParamIndex, Int32 nSize, double * pData, DSSDataFlag * pFlag);
    
    // jxie, 2010-10-13
    // support direct calculate
    virtual int DirectCalculate (Int32 nParam, Int32* ipSize, Int32** ipOffsets,
                         void** ipData, double* ipDivider, DSSDataFlag** ipFlags, VARIANT_BOOL* ipAllDataValid,
                         EnumDSSDataType* iType, vector<Int32> * ipvDataByGroup,
                         Int32 nResSize, double* opData, DSSDataFlag* opFlag, VARIANT_BOOL* opAllDataValid, EnumNullCheckingOption iNullCheckingOption);
    
    virtual bool SupportDirectCalculate();
    
private:
    Int32 mDataSize;
    
    CComVariant mInputData;     //Input variant safe array 
    DSSDataFlag *mpDataFlag;
   
    CComVariant mResultData;    //Result variant	
    DSSDataFlag *mpResultFlag;    
    
    CComVariant mOffset;        // row offset for lag/lead
    DSSDataFlag *mpOffsetFlag;    
    
    CComVariant mDefaultValue;  // default value for lag/lead
    DSSDataFlag *mpDefaultValueFlag;
    
    Int32 mPrevStart;
    Int32 mPrevEnd;
    Int32 mPrevIndex;
    
    // jxie, 2010-9-27
    bool mfIsNumeric;
    bool mfIsDirect;
    double* mpNumericResultData;
    double* mpNumericInputData;
    double* mpNumericOffset;		// row offset for lag/lead
    double* mpNumericDefaultValue;	// default value for lag/lead
    
    // jxie, 2010-11-9
    vector<Int32> * mpvDataByGroup;
    
    int CountDistinct();
    int CountByWindow(Int32 row);
    int MaxMinByWindow(Int32 row);
    int MaxMinDistinct();
    int LagLead(Int32 row);
    
    void MinMaxCompare(Int32 row1, Int32 row2, Int32 &result);	
    
    // jxie, 2010-9-28
    // functions for numeric interface
    int hNumericCalculate();
    int hNumericCountDistinct();
    int hNumericCountByWindow(Int32 row);
    int hNumericMaxMinByWindow(Int32 row);
    int hNumericMaxMinDistinct();
    int hNumericLagLead(Int32 row);
    void hNumericMinMaxCompare(Int32 row1, Int32 row2, Int32 &result);
    
    // jxie, 2010-11-9
    // functions for direct calculate
    int hDirectCountDistinct();
    int hDirectMaxMinDistinct();
    
    // jxie, 2010-12-6
    // speed up for calculation within windows
    Int32 mOldStartIndex;
    Int32 mOldEndIndex;
    Int32 mOldCount;
};

#endif