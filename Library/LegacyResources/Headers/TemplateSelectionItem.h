//
//  TemplateSelectionItem.h
//  ReportServiceCore
//
//  Created by Zhang, Lina on 9/9/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#ifndef __ReportServiceCore__TemplateSelectionItem__
#define __ReportServiceCore__TemplateSelectionItem__

#include "TemplateSelectionUnit.h"
#include <vector>
#include "DSSBaseElementProxy.h"
#include "DSSBaseElementsProxy.h"
#include "DSSRWControl.h"
#include "DSSTemplate.h"
#include "DSSTemplateUnit.h"


class TemplateSelectionItem{
public:
    TemplateSelectionItem();
    ~TemplateSelectionItem();
    void createTemplateSelectionItem(DSSTemplate* mTemplate);
    void createTemplateSelectionUnit(DSSBaseElementProxy* dssElement,DSSRWControl* control, DSSTemplate* mTemplate);
    
    DSSRWControl* getControl();
    
    void setControl(DSSRWControl* value);
    
    DSSBaseElementsProxy* getDssElements();
    
    void setDssElements(DSSBaseElementsProxy* value);
    
    std::vector<TemplateSelectionUnit *>* getUnits();
    bool match(DSSBaseElementProxy* ssElement);
    
    bool matchControlId(DSSTemplateUnit* templateUnit);
private:
    DSSRWControl* control;
    DSSBaseElementsProxy* dssElements;
    std::vector<TemplateSelectionUnit *> templateSelectionUnits;
};

#endif /* defined(__ReportServiceCore__TemplateSelectionItem__) */
