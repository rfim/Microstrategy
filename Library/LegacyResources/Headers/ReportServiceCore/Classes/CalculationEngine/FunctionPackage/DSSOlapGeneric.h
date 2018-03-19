//
//  DSSOlapGeneric.h
//  ReportServiceCore
//
//  Created by Qiang Xue on 7/21/06.
//  Copyright 2006 MicroStrategy, Inc. All rights reserved.
//

#ifndef OlapGeneric_h
#define OlapGeneric_h 1

#import "DSSGenericFunction.h"
#import "VariantCommonInclude.h"
#import "MsiCOM/MsiCOM/Msi_ccombstr.h"
#import "Base/Base/Allocator.h"

class DSSOlapGeneric : public DSSGenericFunction
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
    DSSOlapGeneric();
    
    ~DSSOlapGeneric();
    
    int  Calculate ();
    
    int  SetInputParam (Int32 nParamIndex, Int32 nSize, double *pData, DSSDataFlag *pFlag);
    
    int  GetOutput (Int32 nSize,        // Input buffer size
                    Int32 *nDataSize, 	// return data size
                    double *pData,      // Double array stores the result of function
                    DSSDataFlag *pFlag	// Output flags
                    );
    
    int  GetOutputType (EnumDSSParameterType *pType);
    
    int  GetInputParamType (Int32 nParamIndex, EnumDSSParameterType *pType);
    
    int  IsNullProcessable (bool *pCanProcessNULL);
    
    int  Clear ();
    
    // jxie, 2010-10-13
    // support direct calculate
    virtual int  DirectCalculate (Int32 nParam, Int32* ipSize, Int32** ipOffsets,
                          void** ipData, double* ipDivider, DSSDataFlag** ipFlags, VARIANT_BOOL* ipAllDataValid,
                          EnumDSSDataType* iType, vector<Int32> * ipvDataByGroup,
                          Int32 nResSize, double* opData, DSSDataFlag* opFlag, VARIANT_BOOL* opAllDataValid, 
                          EnumNullCheckingOption iNullCheckingOption);
    
    virtual bool SupportDirectCalculate();

private:
    int SumByWindow(Int32 row);

    int AvgByWindow(Int32 row);
    
    int SumDistinct();
    
    int AvgDistinct();
    
     int hCalculateByWindow(Int32 row, Int32 startRow, Int32 endRow);
    
    // jxie, 2010-11-9
    int STDMETHODCALLTYPE hDirectSumDistinct();
    
    int STDMETHODCALLTYPE hDirectAvgDistinct();
        
    Int32 mDataSize;
    double *mpData;
    DSSDataFlag *mpDataFlag;
    double *mpResult;
    DSSDataFlag *mpResultFlag;
    
    // jxie, 2010-11-9
    vector<Int32> * mpvDataByGroup;
    bool mfIsDirect;
    
    // jxie, 2010-12-3
    // to speed up direct calculate with windowSize
    Int32 mOldStartIndex;
    Int32 mOldEndIndex;
    Int32 mOldCount;
    double mOldNum;   
};

#endif