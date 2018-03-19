/*
 *  SimpleOperator.h
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 2/4/11.
 *  Copyright 2011 MicroStrategy, Inc. All rights reserved.
 *
 */

#ifndef SimpleOperatorFunc_h
#define SimpleOperatorFunc_h 1

#import "CalculationEngine/DSSNNSimpleFunction.h"

class DSSSimpleOperatorFunc : public DSSNNSimpleFunction
{
public:
	DSSSimpleOperatorFunc();
	
	virtual ~DSSSimpleOperatorFunc();
	
	virtual int Calculate(int nParam, double *ipParams, double *opResult, DSSDataFlag *opFlag);
    
    virtual int Calculate(int nParam, CComVariant *ipParam, CComVariant *opResult, DSSDataFlag *opFlag);
    
    virtual int GetOutputType (int size, EnumDSSDataType *inputType, EnumDSSDataType *outputType); 
    
    virtual int DirectCalculate(Int32 nParam, Int32* ipSize, 
                                Int32** ipOffsets, void** ipData, double* ipDivider, 
                                DSSDataFlag** ipFlags, VARIANT_BOOL* ipAllDataValid,
                                DSSDataType_Type* iType, DSSXTabBitVector* ipMask,
                                Int32 nResSize, double* opData, DSSDataFlag* opFlag, VARIANT_BOOL* opAllDataValid,
                                EnumNullCheckingOption iNullCheckingOption);
    
    virtual bool SupportDirectCalculate();
    //tliao, 822148, 2014/09/23, support cell format data calculation
    virtual bool SupportCellFormat() {return true;}
    virtual int CalculateCellData(Int32 nParam, CComVariant * pParam, CComVariant * pResult, DSSDataFlag * pFlag);
    
private:
	
    int CalculateCellTypeAndFormat(Int32 nParam, EnumCellFormatDataTypes *pTypes, DSS_ID *pFormatIDs, EnumCellFormatDataTypes *oResultType, DSS_ID *oResultFormatID);
    
	double hAvgImpl(int nSize, double* pData);
    
    EnumDSSDataType mOutputType;
};

#endif
