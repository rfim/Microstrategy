//
//  DSSRelation.h
//  ReportServiceCore
//
//  Created by ctang on 1/27/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef ReportServiceCore_DSSRelation_h
#define ReportServiceCore_DSSRelation_h
#import "GUID.h"
#import <vector>

class DSSBinaryDataReader;

class DSSRelation
{
    friend class DSSGUnitContainer;
    
public:
    DSSRelation();
    ~DSSRelation();
    
    int LoadFromBinary(DSSBinaryDataReader *ipReadStream);
    
    inline GUID getParentID();
    
    inline GUID getChildID();
    
    int getParentList(int iChild, int** oppParentList, int* opnParent);
    
    int getChildList(int iParent, int** oppChildList, int* opnChild);
    //ctang: the caller should be responsible for the release of oppElement
    int getRelatedElements(GUID& iUnitID, int inElement, int* ipElement, int *opnElement, int** oppElement);
    
    
    
    //TQMS 712887,712845, hyan, 1/22/2013
    int getRelatedElements(GUID& iUnitID, int inElement, int* ipElement, int *opnElement, int** oppElement,int iNullKey, bool ibReturnNullRelated);
    int getChildElementHasNullParent(std::vector<int> &ivChildElements);
    int getParentElementHasNullChild(std::vector<int> &ivParentElements);
    
    int getIDs(GUID *opParentID, GUID *opChildID);
    
    int get_NumUnits(Int32 *opUnits);
    int get_AllIDs(Int32 inUnits, GUID* opIDs);
    
private:
    
    GUID mParentID;
    GUID mChildID;
    
    int mnParent;
    int mnChild;
    
    bool mb1ToM;
    
    int mnChildKey;
    int mnParentKey;
    
    int* mpChildGroupPos;
    int* mpSortedChildKey;
    
    int* mpSortedParentKey;
    int* mpParentGroupPos;
    
    //TQMS 712887,712845, hyan, 1/22/2013
    std::vector<std::vector<int> >mpChildGroups;
    std::vector<std::vector<int> >mpParentGroups;
    
    
    
};

inline GUID DSSRelation::getParentID()
{
    return mParentID;
}

inline GUID DSSRelation::getChildID()
{
    return mChildID;
}

#endif
