//
//  DSSGBTable.h
//  ReportServiceCore
//
//  Created by Tang, Cheng on 9/6/12.
//  Copyright (c) 2012 MicroStrategy, Inc. All rights reserved.
//

#ifndef ReportServiceCore_DSSGBTable_h
#define ReportServiceCore_DSSGBTable_h

class DSSTabularIndex;
class DSSXTabBitVector;
class DSSBinaryDataReader;
class DSSGUnitContainer;

class DSSGBTable
{
private:
    int mnRows;
    int mnTargetRows;
    
    vector<int> mvColumns;
    
    DSSTabularIndex* mpXTabGBTable;
    
    vector<DSSXTabBitVector*> mpvConsolidationBVs;
    
    int* mpRowMap;
    
    int mType;
    DSSXTabBitVector *mpInitialSelectedRows;
    
public:
    DSSGUnitContainer* mpGUnitContainer;
    
    DSSGBTable();
    
    ~DSSGBTable();
    
    int loadFromBinary(DSSBinaryDataReader *ipReadStream, int iType = 1);
    
    int hGetAvailableElements(vector<Int32>& irSelectedUnits, 
                              vector<vector<Int32> >& irSelectedElements, 
                              vector<Int32>& irTargetUnits, 
                              vector<Int32>* opvElements);
    
    int hCalculateMask(vector<Int32>& irSelectedUnits, 
                                   vector<vector<Int32> >& irSelectedElements,
                                   DSSXTabBitVector &orMask);
    
    int GetSelectedTargetRow(vector<GUID>& irSelectedUnits, 
                                         vector<vector<Int32> >& irSelectedElements,
                             DSSXTabBitVector& orMask);
    
    int GetAvalibleElement(vector<GUID>& irSelectedUnits, 
                                       vector<vector<Int32> >& irSelectedElements, 
                                       GUID iTargetUnit, 
                                       vector<Int32>& orElements);
    
    inline int GetTargetRowCount() {return mnTargetRows;}
    
    int FindUnitI(GUID* ipID, Int32* opIndex);
    
    
    int GetUnitID(Int32 iUnit, GUID* opID);
    
    // 9/10/2012 tehe
    inline int Count() { return mvColumns.size(); }

    int getType(){return mType;};
};

#endif
