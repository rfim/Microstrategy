/*
 *  DSSFunctionObject.h
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 2/4/11.
 *  Copyright 2011 MicroStrategy, Inc. All rights reserved.
 *
 */

#ifndef FunctionObject_h
#define FunctionObject_h 1

#import "CalculationEngine/DSSCalculationHelper.h"
#import "VariantCommonInclude.h"
class CComVariant;

class DSSFunctionObject
{
public:
	
	DSSFunctionObject();
	
	virtual ~DSSFunctionObject();
	
	virtual EnumDSSFunctionType getFunctionType() = 0;
	
	virtual int setProperty(int Index, CComVariant *ipVal);
	
    // 3/21/2012 tehe
	inline void Init(EnumSimpleOperatorType Index){mFunctionIndex = Index;};
	
    // 3/21/2012 tehe
	inline EnumSimpleOperatorType getOperatorType() {return mFunctionIndex;};
    //tliao, 822148, 2014/09/23, support cell format data calculation
    virtual bool SupportCellFormat() {return false;}
    virtual int CalculateCellData(Int32 nParam, CComVariant * pParam, CComVariant * pResult, DSSDataFlag * pFlag);
	
protected:
    EnumSimpleOperatorType mFunctionIndex;
	
	// EnumDSSFunction mFunctionIndex;
	
private:
	
};

#endif
