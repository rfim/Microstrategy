//
//  DSSTimeSpan.h
//  ReportServiceCore
//
//  Created by He Tengfei on 2/24/12.
//  Copyright 2012 MicroStrategy, Inc. All rights reserved.
//
#ifndef DSSTimeSpan_h 
#define DSSTimeSpan_h 1

#include "Base/Defines/BasicTypes.h"

namespace MDataType 
{
    // 2/24/2012 tehe we just implement a partial of TimeSpan to support financial package
    class DSSTimeSpan
    {
    public:
        
        DSSTimeSpan(Int64 inMilliSeconds=0);
        ~DSSTimeSpan();
        
        inline Int64 GetTotalMilliseconds() const;
        
    private:
        Int64 mnMilliseconds;
        
    };
    
    inline Int64 DSSTimeSpan::GetTotalMilliseconds() const
    {
        return mnMilliseconds;
    }
    
}

#endif