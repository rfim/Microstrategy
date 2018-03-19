//
//  DSSStatistics_include.h
//  ReportServiceCore
//
//  Created by He Tengfei on 3/7/12.
//  Copyright 2012 MicroStrategy, Inc. All rights reserved.
//
#ifndef _DSSSTATISTICS_INCLUDE_H
#define _DSSSTATISTICS_INCLUDE_H

typedef enum DssStatFuncType
{
	DssStatFuncLinearTrend_S = 49, 
	DssStatFuncLinearTrend_V = 50,
	DssStatFuncGrowth_S = 51,
	DssStatFuncGrowth_V = 52,
	DssStatFuncForecast_S = 53,
	DssStatFuncForecast_V = 54,
} DssStatFuncType;

#endif // _DSSSTATISTICS_INCLUDE_H