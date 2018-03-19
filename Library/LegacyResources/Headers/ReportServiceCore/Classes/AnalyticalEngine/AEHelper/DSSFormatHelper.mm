//
//  DSSFormatHelper.mm
//  ReportServiceCore
//
//  Created by hyan on 1/16/12.
//  Copyright 2012 Microstrategy. All rights reserved.
//

#include "DSSFormatHelper.h"

#import "DashboardViewer/GridIterator/NumberFormatter.h"

int DSSFormatHelper::getFormatValue(const void*ipData, int iSizeInByte,int iDataType,int iDataFlag,int iFormType,void*iFormat,int* oColor,MBase::CString &orFormatValueString)
{
    if(ipData==NULL)
    {
        orFormatValueString="";
        return 0;
    }
    
    if(iDataType==DssDataTypeVarChar ||iDataType==DssDataTypeUTF8Char)
    {
        orFormatValueString=(char *)ipData;
    }
    else
    {
        NSString *lpFormatValueString=[NumberFormatter formatValue:ipData Size:iSizeInByte Type:iDataType Flag:iDataFlag FormType:iFormType Format:iFormat Color:oColor FormatSourceType:FMTSTP_NATIVE];
        
        orFormatValueString=[lpFormatValueString UTF8String];
    }
    
    return 0;
}