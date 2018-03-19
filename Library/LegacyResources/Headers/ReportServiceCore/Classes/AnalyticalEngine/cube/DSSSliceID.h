//
//  DSSSliceID.h
//  ReportServiceCore
//
//  Created by Jun Wang on 1/24/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#ifndef ReportServiceCore_DSSSliceID_h
#define ReportServiceCore_DSSSliceID_h

class DSSCubeDataSlice;
class DSSCube;

struct DSSSliceID
{
    DSSCube *mpCube;
    int mSliceID;
    
    DSSSliceID(DSSCube *ipCube, int iSliceID);
    DSSSliceID();
    
    ~DSSSliceID(){}
    
    int GetSlice(DSSCubeDataSlice **oppSlice);
    int GetSliceC(DSSCubeDataSlice **oppSlice);

    bool operator == (const DSSSliceID&) const;
    
};

inline bool DSSSliceID::operator == (const DSSSliceID& iSliceID) const
{
    return (mpCube == iSliceID.mpCube && mSliceID == iSliceID.mSliceID);
}

#endif
