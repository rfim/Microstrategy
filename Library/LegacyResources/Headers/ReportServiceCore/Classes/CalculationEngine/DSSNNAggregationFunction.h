//
//  DSSNNAggregationFunction.h
//  ReportServiceCore
//
//  Created by Yang Zhuo on 7/22/11.
//  Copyright 2011 Microstrategy. All rights reserved.
//

#ifndef DSSNNAGGREGATIONFUNCTION_H
#define DSSNNAGGREGATIONFUNCTION_H

#import "DSSAggregationFunction.h"

class DSSNNAggregationFunction : public DSSAggregationFunction
{
public:
    virtual int Calculate (int nSize, CComVariant* pVec, EnumDSSDataType iType, CComVariant* pResult,
                           DSSDataFlag * pFlag );
    
    virtual int getOutputType (EnumDSSDataType InputType, EnumDSSDataType * OutputType );
};

#endif