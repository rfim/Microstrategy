//
//  CDSSStatisticAggregation.h
//  ReportServiceCore
//
//  Created by Yang Zhuo on 6/27/11.
//  Copyright 2011 Microstrategy. All rights reserved.
//

#ifndef DSSStatisticAggregation_h
#define DSSStatisticAggregation_h 1


#import "DSSNNAggregationFunction.h"

class DSSStatisticAggregation : public DSSNNAggregationFunction  //public ICDSSCellFormatFunction // Cell format data support
{
public:
	DSSStatisticAggregation();
    
	~DSSStatisticAggregation();
    
	int Calculate(Int32 nSize, double *pVec, double *opResult, DSSDataFlag *pFlag);
    
	int Init(EnumSimpleOperatorType iOperatorType);
    
	int GetOutputType(EnumDSSDataType InputType, EnumDSSDataType* pOutputType);
    
	// jxie, 2010-12-10
	// enhance to support different numeric types
	int DirectCalculate(Int32 iSize,
                        Int32 * ipGroupLinkList,
                        Int32 * ipBlockAddress,
                        void* ipData, double iDivider,
                        EnumDSSDataType iDataType,
                        DSSDataFlag* ipDataFlags,
                        bool iAllDataValid,
                        EnumNullCheckingOption iNullCheckingOption,
                        double* opResult,
                        DSSDataFlag* opDataFlag);
    
	bool SupportDirectCalculate();
    //tliao, 822148, 2014/09/23, support cell format data calculation
    virtual bool SupportCellFormat() {return true;}
    virtual int CalculateCellData(Int32 nParam, CComVariant * pParam, CComVariant * pResult, DSSDataFlag * pFlag);
    
protected:
    //zhyang, 22/07/2011, This member is defined in DSSFunctionObject, no need to redefine it
   // EnumDSSFunction mFunctionIndex;
    
private:
	
    
	// jxie, 2010-9-27
	// calculate variance for directCalculate
	void hDirectCaluclateVariance(Int32 iSize, 
                                  Int32* ipGroupStart, 
                                  Int32* ipBlockAddress, 
                                  void* ipData, 
                                  EnumDSSDataType iDataType,
                                  DSSDataFlag* ipDataFlag, 
                                  EnumNullCheckingOption iNullCheckingOption,
                                  double &d1, double &lM1, double &lSS1, 
                                  Int32 &offset, 
                                  DSSDataFlag &lFlag, 
                                  bool& ofAllSkipCheckNull);
    
    int hCheckDirectCalculateFlag(DSSDataFlag* opDataFlag, bool& ifAllSkipCheckNull, 
                                  DSSDataFlag iFlag, int& offset, int offsetMin);

};

// Class CDSSStatisticAggregation 
#endif


