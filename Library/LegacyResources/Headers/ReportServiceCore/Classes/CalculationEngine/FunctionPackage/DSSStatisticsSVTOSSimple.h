//  DSSStatisticsSVTOSSimple.h
//  ReportServiceCore
//
//  Created by He Tengfei on 3/7/12.
//  Copyright 2012 MicroStrategy, Inc. All rights reserved.



#ifndef DSSStatisticsSVTOSSimple_h
#define DSSStatisticsSVTOSSimple_h 1

#include "DSSGenericFunction.h"

// zyu 06/23/2005
// Mimicked from DSSStatisticsVSTOS.
// Note that I did not name this DSSStatisticsSVTOS because 
// I think DSSStasticsVSTOS (not to be confused with DSSStatistics*SV*TOS)
// is a misnomer. DSSStasticsVSTOS contains a member called mHypothesis.
// Obviously, this is not a general property,
// which makes it a bad candidate for general framework for VS to S computation.
// For this reason, I use DSSStatisticsSVTOSSimple instead of DSSStatisticsSVTOS
// to avoid confusion. 
class DSSStatisticsSVTOSSimple : public DSSGenericFunction
{
public:
    DSSStatisticsSVTOSSimple();
    
    ~DSSStatisticsSVTOSSimple();
    
    int setProperty (Int32 Index, CComVariant *pValue);
    
    int Init (Int32 iFunctionIndex);
    
    int Calculate ();
    
    int GetOutput (Int32 nSize, Int32 *nDataSize, double *pData, DSSDataFlag *pFlag);
    
    int GetOutputType (EnumDSSParameterType *pType);
    
    int SetInputParam (Int32 nParamIndex, Int32 nSize, double *pData, DSSDataFlag *pFlag);
    
    int GetInputParamType (Int32 nParamIndex, EnumDSSParameterType *pType);
    
    int Clear ();
    
    int IsNullProcessable (bool *pCanProcessNULL);
    
private:
    
    Int32 mFunctionIndex;
    Int32 mVSize;
    double *mV;
    double mS;
    double mResult; 
    DSSDataFlag mFlag;
};

#endif