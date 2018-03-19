//
//  TemplateSelectionUnit.h
//  ReportServiceCore
//
//  Created by Zhang, Lina on 9/9/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#ifndef ReportServiceCore_TemplateSelectionUnit_h
#define ReportServiceCore_TemplateSelectionUnit_h


#include <string>
#include "DSSBaseElementProxy.h"


class TemplateSelectionUnit
{
public:
    TemplateSelectionUnit();
    
    // std::string getControlId();
    // void setControlId(std::string value);
    GUID getControlID();
    void setControlID(GUID iID);
    
    //IDSSElement getDssElement();
    DSSBaseElementProxy* getDssElement();
    //void setDssElement(IDSSElement value);
    void setDssElement(DSSBaseElementProxy* value);
    std::string getSubtotalElementId();
    void setSubtotalElementId(std::string value);
private:
	//To store the IDSSRWControl source id
    //std::string controlId;
    GUID controlId;
    //To store the IDSSElement this unit contains
    //IDSSElement dssElement;
    DSSBaseElementProxy* dssElement;
    //To store the local to global converted subtotal id
    std::string subtotalElementId;
};


#endif /* defined(__ReportServiceCore__TemplateSelectionUnit__) */
