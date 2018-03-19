//
//  DSSGenericFunction.cpp
//  ReportServiceCore
//
//  Created by Zhuo Yang on 11/5/12.
//  Copyright (c) 2012 Microstrategy. All rights reserved.
//

#include "DSSGenericFunction.h"

DSSGenericFunction::DSSGenericFunction()
{}

DSSGenericFunction::~DSSGenericFunction()
{}

bool DSSGenericFunction::SupportDirectCalculate()
{
    return false;
}

int DSSGenericFunction::DirectCalculate(Int32 nParam, Int32* ipSize, 
                    Int32** ipOffsets, void** ipData, double* ipDivider, 
                    DSSData_Flags** ipFlags, VARIANT_BOOL* ipAllDataValid,
                    DSSDataType_Type* iType, vector<Int32> * ipvDataByGroup,
                    Int32 nResSize, double* opData, DSSData_Flags* opFlag, VARIANT_BOOL* opAllDataValid,
                    EnumNullCheckingOption iNullCheckingOption)
{
    return E_NOTIMPL;
}

int DSSGenericFunction::get_PropertyInfo(Int32 PropertyIndex, BSTR *pName, BSTR *pDescription, VARIANT *pValue)
{
    return E_NOTIMPL;
}