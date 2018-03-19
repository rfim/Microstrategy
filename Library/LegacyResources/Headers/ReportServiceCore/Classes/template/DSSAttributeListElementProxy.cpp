//
//  DSSAttributeListElementProxy.cpp
//  ReportServiceCore
//
//  Created by Sun, Mingmin on 9/2/15.
//  Copyright (c) 2015 MicroStrategy, Inc. All rights reserved.
//

#include "DSSAttributeListElementProxy.h"

DSSAttributeListElementProxy::DSSAttributeListElementProxy()
{
    mElementID = "";
}

DSSAttributeListElementProxy::~DSSAttributeListElementProxy()
{}

DSSAttributeListElementProxy::DSSAttributeListElementProxy(DSSObjectContext* ipObjectContext) : DSSBaseElementProxy(ipObjectContext)
{
    mElementID = "";
}


int DSSAttributeListElementProxy::Parse(std::string iElementID)
{
    mElementID = iElementID;
    mElementType = DssElementAttribute;
    mUnitType = DssTemplateAttribute;

    std::vector < std::string > lTokens; //we'll put all of the tokens in here
    
    size_t start = 0, pos = std::string::npos;
    std::string sep = ";";//terse id
    if (iElementID[2] - ':' == 0)
        sep = ":"; // long id
    
    while (start < iElementID.length() && (pos = iElementID.find(sep, start)) != std::string::npos) {
        lTokens.push_back(iElementID.substr(start, pos - start));
        start = pos + 1;
    }
    
    lTokens.push_back(iElementID.substr(start));
    
    DSSBaseElementProxy::ConvertFromStringToGUID(lTokens[1], mObjectID);
    
    return S_OK;
}

DSSAttribute* DSSAttributeListElementProxy::getAttribute()
{
    return (DSSAttribute*)(getObjectContext()->FindObjectH(mObjectID));
}