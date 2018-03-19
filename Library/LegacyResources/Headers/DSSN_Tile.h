//
//  DSSN_Tile.h
//  ReportServiceCore
//
//  Created by Zhang Jia on 7/20/11.
//  Copyright 2011 MicroStrategy Beijing R&D Center. All rights reserved.
//

#ifndef N_Tile_h
#define N_Tile_h 1

#import "DSSGenericFunction.h"
#import "../Common/Base/Base/Allocator.h"
#import "VariantCommonInclude.h"

class DSSN_Tile : public DSSGenericFunction
{    
public:
    DSSN_Tile();
    
    ~DSSN_Tile();
    
    // int Init (EnumDSSFunction iFunctionIndex);
    
    virtual int setProperty (Int32 Index, CComVariant *pValue);
    
    int Calculate ();
    
    int GetOutput (Int32 nSize,       // Input buffer size
                   Int32 *nDataSize,  // return data size
                   double *pData,     // Double array stores the result of function
                   DSSDataFlag *pFlag // Output flags
                   );
    
    int GetOutput(Int32, Int32 *, VARIANT *, DSSDataFlag *);
    
    int GetOutputType (EnumDSSParameterType *  pType);
    
    int GetOutputType(Int32, EnumDSSDataType *, EnumDSSDataType *, EnumDSSParameterType *);
    
    int Clear ();
    
    int SetInputParam (Int32 nParamIndex, // The index of input parameter
                       Int32 nSize,       // Size of input double array
                       double *pData,
                       DSSDataFlag *pFlag);
    
    int SetInputParam(Int32, Int32, EnumDSSDataType,CComVariant *, DSSDataFlag *);
    
    int GetInputParamType (Int32 nParamIndex, EnumDSSParameterType *pType);
    
    int IsNullProcessable(bool *pCanProcessNULL); 
    
    // jxie, 2010-10-11
    // support direct calculate
    virtual int DirectCalculate (Int32 nParam, Int32* ipSize, Int32** ipOffsets, 
                         void** ipData, double* ipDivider, DSSDataFlag** ipFlags, VARIANT_BOOL* ipAllDataValid,
                         EnumDSSDataType* iType, vector<Int32> * ipvDataByGroup,
                         Int32 nResSize, double* opData, DSSDataFlag* opFlag, VARIANT_BOOL* opAllDataValid, 
                         EnumNullCheckingOption iNullCheckingOption);
    
    virtual bool SupportDirectCalculate();
    
private:    
    double *mpData;
    Int32 mnDataSize;    
    double *mpResults;    
    Int32 mNBuckets;
    bool mAsc;    
    double mStepValue; //step value for NTile_SV
};

#endif