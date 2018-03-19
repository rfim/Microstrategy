//
//  DSSElementsPropertiesMapValue.h
//  ReportServiceCore
//
//  Created by Zeng Jinliang on 9/11/14.
//  Copyright (c) 2014 MicroStrategy, Inc. All rights reserved.
//

#ifndef ReportServiceCore_DSSElementsPropertiesMapValue_h
#define ReportServiceCore_DSSElementsPropertiesMapValue_h

#include "DSSBaseElementsProxy.h"

class DSSElementsPropertiesMapValue : public DSSSecondClassObject
{
public:
    DSSElementsPropertiesMapValue(DSSObjectContext* ipObjectContext);
    virtual ~DSSElementsPropertiesMapValue();
    
    int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
    
    int Count() const { return (int)mElementsPropertiesValueMap.size(); }
    void Clear();
    bool Add(DSSBaseElementsProxy* ipElements, const std::string& iValue);
    bool Delete(DSSBaseElementsProxy* ipElements);

	typedef std::map<DSSBaseElementsProxy*, std::string> ElementsPropertiesValueMapType;
    const ElementsPropertiesValueMapType& getElementsPropertiesValueMap() const { return mElementsPropertiesValueMap; }
    ElementsPropertiesValueMapType& elementsPropertiesValueMap() { return mElementsPropertiesValueMap; }
    
private:
    ElementsPropertiesValueMapType mElementsPropertiesValueMap;
};

#endif
