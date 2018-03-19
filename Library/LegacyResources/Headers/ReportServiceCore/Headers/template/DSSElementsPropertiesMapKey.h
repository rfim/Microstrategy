//
//  DSSElementsPropertiesMapKey.h
//  ReportServiceCore
//
//  Created by Zeng Jinliang on 9/9/14.
//  Copyright (c) 2014 MicroStrategy, Inc. All rights reserved.
//

#ifndef ReportServiceCore_DSSElementsPropertiesMapKey_h
#define ReportServiceCore_DSSElementsPropertiesMapKey_h

#include "DSSDropZone.h"

class DSSElementsPropertiesMapKey : public DSSSecondClassObject
{
public:
    DSSElementsPropertiesMapKey(DSSObjectContext* ipObjectContext);
    virtual ~DSSElementsPropertiesMapKey();
    // overloading is used to loop compare; map::find need overload operator<
    bool operator==(const DSSElementsPropertiesMapKey& rhs) const;
    int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
    void Clear();
    
    int Count() const { return (int)mDZUnits.size(); }
    DSSDropZoneUnit* getItem(int index) const;
    const std::vector<DSSDropZoneUnit*>& getDZUnits() const { return mDZUnits; }
private:
    std::vector<DSSDropZoneUnit*> mDZUnits;
};

#endif
