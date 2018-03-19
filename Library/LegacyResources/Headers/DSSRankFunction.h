//
//  DSSRankFunction.h
//  ReportServiceCore
//
//  Created by Zhang Jia on 7/20/11.
//  Copyright 2011 MicroStrategy Beijing R&D Center. All rights reserved.
//

#ifndef RankFunction_h
#define RankFunction_h 1

#import "DSSNNGenericFunction.h"
#import "../Common/Base/Base/Allocator.h"

class DSSRankFunction : public DSSNNGenericFunction
{
public:    
    //ctang 2010-9-2
    typedef enum{First = -1, Ignore = 0, Last = 1} EnumNullInclude;
    
    DSSRankFunction();
    
    ~DSSRankFunction();
    
    virtual int setProperty (Int32 Index, CComVariant *pValue);

    int SetPropertyByValue (CComVariant *pValue);
    
    int SetPropertyAsc (VARIANT *pValue);
    
    //ctang add null include attribute
    int SetPropertyNullInclude(VARIANT *pValue);
    
    int get_FunctionType (EnumDSSFunctionType *  pType);
    
    int Calculate ();
    
    // jxie, 2010-9-17
    // support direct calculate for rank function
    virtual int DirectCalculate (Int32 nParam, Int32* ipSize, Int32** ipOffsets, 
                         void** ipData, double* ipDivider, DSSDataFlag** ipFlags, VARIANT_BOOL* ipAllDataValid,
                         EnumDSSDataType* iType, vector<Int32> * ipvDataByGroup,
                         Int32 nResSize, double* opData, DSSDataFlag* opFlag,
                         VARIANT_BOOL* opAllDataValid, EnumNullCheckingOption iNullCheckingOption);
    
    virtual bool SupportDirectCalculate();
    
    int GetOutput (Int32 nSize,       // Input buffer size
                   Int32 *nDataSize, 	// return data size
                   CComVariant *pData,
                   DSSDataFlag *pFlag	// Output flags
                   );
    
    int GetOutputType (Int32 nSize,
                       EnumDSSDataType * pInputType,
                       EnumDSSDataType * pOutputDataType,
                       EnumDSSParameterType *  pType);
    
    int Clear ();
    
    int SetInputParam (Int32 nParamIndex, // The index of input parameter
                       Int32 nSize,       // Size of input double array
                       EnumDSSDataType dataType,
                       CComVariant *pData,
                       DSSDataFlag *pFlag);
    
    int GetInputParamType (Int32 nParamIndex, EnumDSSParameterType *pType);
    
    int IsNullProcessable(bool *pCanProcessNULL); 
    
    //ctang 2010-8-1
    int GetOutput(Int32 nSize, Int32 *nDataSize,  double * pData, DSSDataFlag * pFlag);
    
    int GetOutputType(EnumDSSParameterType * pType);
    
    int SetInputParam(Int32 nParamIndex, Int32 nSize, double * pData, DSSDataFlag * pFlag);
    
private:
    int rankDate(Int32 nSize, MDataType::DSSDateTime *pDate, double *pResult, bool fAsc);
    
    // jxie, 2011-1-12, TQMS 450452
    // Support Rank for string input
    int hRankVariant(Int32 nSize, CComVariant *pVar, double *pResult, bool fAsc);
    
    // true (default) : return absolute value.
    // false : return percentage value.
    bool mByValue;
    
    // true (default) : ascending sort
    // false : descending sort.
    bool mAsc;
    
    Int32 mNullInclude;
    double *mpValues;
    Int32	  mnDataSize;
    Int32	  mnNoNullSize;    
    MDataType::DSSDateTime *mpDates;    
    double  *mpResults;
    DSSDataFlag *mpFlags;
    
    // jxie, 2011-1-12, TQMS 450452
    // Support Rank for string input
    CComVariant mpVars;    
};

#endif