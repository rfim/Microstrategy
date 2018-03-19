//
//  DSSXTabLayoutInfo.h
//  ReportServiceCore
//
//  Created by He Tengfei on 11/14/11.
//  Copyright 2011 MicroStrategy, Inc.. All rights reserved.
//

#ifndef DSSXTABLAYOUTINFO_H
#define DSSXTABLAYOUTINFO_H 

#import <vector>

class DSSXTabLayoutUnitInfo;
class DSSTemplate;
class DSSTabularData;
class DSSTemplateUnit;
class DSSTabularUnit;
class DSSTabularCustomGroup;
class DSSTabularConsolidation;
class DSSRWGridModel;

using namespace std;

#ifndef NULL
#define NULL 0
#endif

#define AXIS_COUNT 2

class DSSXTabLayoutInfo 
{
public:
    DSSXTabLayoutInfo();
    
    ~DSSXTabLayoutInfo();
    
    //	Return the information about a specific unit.
    int get_UnitInfo(int UnitID, DSSXTabLayoutUnitInfo **ppUnitInfo);
    
    //	Return the size of the axis.
    int get_AxisSize(int Axis, int *pSize);
    
    //	Return unit from an axis
    int get_UnitInfoByAxis(int Axis, int AxisOffset, DSSXTabLayoutUnitInfo **ppUnitInfo);
    
    //	Init the XTabLayoutInfo object.
    int Init(DSSTemplate *pTemplate, DSSTabularData *pTabularData, DSSRWGridModel *ipGridModel = NULL);
    
    int get_Tag(int *pTag);
    
    //put_Tag
    int put_Tag(int iTag);
    
    int UpdateUnitInfo(int iAxis, const vector<int>& lvRuntimeUnits);
    
    inline int getRuntimeUnitCount(int iAxis) { return mvRunTimeUnits[iAxis]; }
    
    
private:
    //	Array giving the number of units, in the current View,
    //	in each axis.  The array has mNumAxis members.
    // 3/26/2012 tehe: fix memory leak
    // int *mpNumUnit;
    vector<int> mvNumUnit;

    //	The number of axis in the template.
    int mNumAxis;
    
    //	A double pointer to pointer to UnitInfo. Each element in
    //	the array is a pointer to the units a axis contains. The
    //	size of array is mNumAxis
    // DSSXTabLayoutUnitInfo ***mppUnit;
    vector<vector<DSSXTabLayoutUnitInfo *> > mvvUnit;
    
    int mTotalTag;
    
    //	Only used in Init(). It is not a data member kept
    //	through out the object life.
    // DSSTabularData *mpTabularData;
     
private: 
    bool mfTotalTagFinal;
    int mvRunTimeUnits[AXIS_COUNT];
};

// Class DSSXTabLayoutInfo 

#endif