//
//  TemplateSelectionRow.h
//  ReportServiceCore
//
//  Created by Zhang, Lina on 9/9/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#ifndef __ReportServiceCore__TemplateSelectionRow__
#define __ReportServiceCore__TemplateSelectionRow__

#include "TemplateSelectionItem.h"

class TemplateSelectionRow{
public:
    TemplateSelectionRow();
    ~TemplateSelectionRow();
    void add(TemplateSelectionItem* templateSelectionItem);
    std::vector<TemplateSelectionItem*>* getItems();
    bool match(DSSBaseElementProxy* dssElement);
    bool match2(std::vector<DSSBaseElementProxy*> dssElements);
    bool matchControlId(DSSTemplateUnit* templateUnit);
    bool getInHeader();
    void setInHeader(bool iInHeader);
private:
    // --------------------------------------------------------------------------------
    // Instance Variables
    // --------------------------------------------------------------------------------
    
    //To store the list of the TemplateSelectionItems in the list
    std::vector<TemplateSelectionItem*> templateSelectionItems;
    bool mInHeader;
};


#endif /* defined(__ReportServiceCore__TemplateSelectionRow__) */
