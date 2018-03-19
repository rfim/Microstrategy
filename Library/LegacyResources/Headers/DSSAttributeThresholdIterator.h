//
//  DSSAttributeThresholdIterator.h
//  ReportServiceCore
//
//  Created by Zeng, Jinliang on 3/22/16.
//  Copyright © 2016 MicroStrategy, Inc. All rights reserved.
//

#ifndef DSSAttributeThresholdIterator_h
#define DSSAttributeThresholdIterator_h

#include <vector>
class DSSXTabView;
class DSSCube;

class DSSAttributeThresholdIterator
{
public:
    DSSAttributeThresholdIterator(DSSXTabView* ipXTabView);
    DSSAttributeThresholdIterator();
    ~DSSAttributeThresholdIterator();
    
    
    int setXTabView(DSSXTabView* ipXTabView);
    /**
     Optionally set the start row and number of rows for incremental fetch.
     */
    int Init(int iStartRow = -1, int iNumRows = -1, VARIANT_BOOL iMergeRow = VARIANT_FALSE, VARIANT_BOOL iMergeCol = VARIANT_FALSE);
    
    /**
     Get the axis name and row number corresponding to the current threshold that meets condition: axis name could be either DssAxisColumns or DssAxisRows.
     */
    int getRow(EnumDSSAxisName* opAxis, int* opRow);
    
    
    /**
     The current threshold.
     */
    int getThreshold(DSS_ID* opAttributeID, int* opThresholdID);
    
    /**
     Go to first threshold
     */
    int First();
    
    /**
     Moves to the next threshold
     */
    int Next();
    
    /**
     Retrieve the threshold in the specific position and associated with the specific attribute, return S_FALSE if not found.
     The current iterator's position will not be updated by this function (which will be updated by First() and Next() function).
     */
    int getSpecificThreshold(EnumDSSAxisName iAxis, int iOrdinal, DSS_ID* ipAttrID, int* opThresholdId);
private:
    int hgetCellThresholdId(int iRow, int iCol, DSS_ID* ipAttrID, DSSCube* ipCube, bool* iobIndex, DSSXTabKey* iopIndex, int* opThresholdId);
    
    DSSXTabView* mpXTabView;
    
    // traversal order would be: Axis (Row/Column) => attributes => different levels in the same row/column.
    DSS_ID mCurrAttr;
    int mCurrRow;
    int mCurrColumn;
    int mCurrThresholdId;
    EnumDSSAxisName mCurrAxis;
    
    // incremental fetch and merged headers
    bool mbMergeRow;
    bool mbMergeCol;
    
    // row range for incremental fetch
    int mStartRow;
    int mRowNum;
    
    std::vector<DSS_ID> mvRowUnitIDs, mvColUnitIDs;
    std::vector<DSS_ID>::iterator mUnitIter;
};

#endif /* DSSAttributeThresholdIterator_h */
