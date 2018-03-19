//
//  DSSFormatHelper.h
//  ReportServiceCore
//
//  Created by hyan on 1/16/12.
//  Copyright 2012 Microstrategy. All rights reserved.
//

#include "DSSAEHelper.h"

class DSSFormatHelper
{
public:
    static int getFormatValue(const void*ipData, int iSizeInByte,int iDataType,int iDataFlag,int iFormType,void*iFormat,int* oColor,MBase::CString &orFormatValueString);
};