//
//  DSSDynamicTime.h
//  ReportServiceCore
//
//  Created by He Tengfei on 2/24/12.
//  Copyright 2012 MicroStrategy, Inc. All rights reserved.
//
#ifndef DSSDYNAMICTIME_H
#define DSSDYNAMICTIME_H 1

#include "DSSDateTime.h"

namespace MDataType {
    typedef enum  
    {
        SECONDS	= 1,
        MINUTES	= 2,
        HOUR	= 3,
        DAY		= 4,
        MONTH	= 5,
        YEAR	= 6,
        WEEK	= 7
    } TIME_GRANULARITY;
    
    // 2/24/2012 here we just implement a partial of DynamicTime in order to support financial package
    class DSSDynamicTime
    {
    public:
        static void MoveNthTimePointBefore(DSSDateTime& iorTime, int inCount, TIME_GRANULARITY inLevel);
    };
}

#endif