//
//  DSSElementsPropertiesMap.h
//  ReportServiceCore
//
//  Created by Zeng Jinliang on 9/11/14.
//  Copyright (c) 2014 MicroStrategy, Inc. All rights reserved.
//

#ifndef ReportServiceCore_DSSElementsPropertiesMap_h
#define ReportServiceCore_DSSElementsPropertiesMap_h

#include "DSSSecondClassObject.h"

class DSSElementsPropertiesMapKey;
class DSSElementsPropertiesMapValue;
class DSSBaseElementsProxy;
struct DSSDropZoneUnit;

typedef std::map<DSSElementsPropertiesMapKey*, DSSElementsPropertiesMapValue*> ElementsPropertiesMapType;
class DSSElementsPropertiesMap : public DSSSecondClassObject
{
public:
    DSSElementsPropertiesMap(DSSObjectContext* ipObjectContext);
    virtual ~DSSElementsPropertiesMap();
    
    int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
    
    
    size_t Count() const { return mElementsPropertiesMap.size(); }
    void Clear();
    bool Add(DSSElementsPropertiesMapKey* ipKey, DSSElementsPropertiesMapValue* ipValue);
    // ipKey will be a wide point after delete; if delete failed, caller release it
    bool Delete(DSSElementsPropertiesMapKey* ipKey);

    const ElementsPropertiesMapType& getElementsPropertiesMap() const { return mElementsPropertiesMap; }
    
    std::string getPropertieByKey(const std::vector<DSSDropZoneUnit*>& ivDZUnits, DSSBaseElementsProxy* ipElements);
    
    std::string getPropertieByKey(const std::vector<GUID>& ivIDs, DSSBaseElementsProxy* ipElements);
    
    std::string getPropertieByKey(const std::vector<GUID>& ivIDs, const std::vector<std::string>& ivDisplayNames);
    
    bool getPropertieEmptyByKey(const std::vector<GUID>& ivIDs);
    bool isAttrElementIDsEqual(std::string IDString1, std::string IDString2);
    class MatchDZUnits
    {
    public:
        MatchDZUnits(const std::vector<DSSDropZoneUnit*>& rhs) : mRef(rhs)
        {
        }
        bool operator()(const ElementsPropertiesMapType::value_type &pair);
    private:
        const std::vector<DSSDropZoneUnit*>& mRef;
    };
    
    class MatchUnitIDs
    {
    public:
        MatchUnitIDs(const std::vector<GUID>& rhs) : mRef(rhs)
        {
        }
        bool operator()(const ElementsPropertiesMapType::value_type &pair);
    private:
        const std::vector<GUID>& mRef;
    };
private:
    ElementsPropertiesMapType mElementsPropertiesMap;
};

#endif
