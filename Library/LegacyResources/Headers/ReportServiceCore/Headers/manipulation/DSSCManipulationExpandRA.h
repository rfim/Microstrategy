//
//  DSSCManipulationExpandRA.h
//  ReportServiceCore
//
//  Created by Sun, Mingmin on 5/9/16.
//  Copyright © 2016 MicroStrategy, Inc. All rights reserved.
//

#ifndef DSSCManipulationExpandRA_h
#define DSSCManipulationExpandRA_h

#include "DSSCManipulation.h"
#include "DSSRWTemplateNode.h"
#include "DSSRWGridIterator.h"
#import "DSSCellHandle.h"
class DSSCManipulationExpandRA: public DSSCManipulation
{
public:
    DSSCManipulationExpandRA(DSSRWIterator* ipIterator, int iAxis, int iDepth, DSSCellHandle* iCellHandle, bool iExpand, bool iApplyAll);
    DSSCManipulationExpandRA(DSSActionObject* ipAO, DSSObjectContext* ipContext);
    ~DSSCManipulationExpandRA();
    
    int Execute();
    DSSRWTemplateNode* getTemplateNode();
    void GenerateAODeltaXML();
   // DSSRWTemplateNode* getTemplateNode();
#ifndef __ANDROID__
    void hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext);
#endif
    
private:
    DSSRWTemplateNode* mpTemplateNode;
    DSSRWIterator* mpIterator;
    DSSCellHandle mCellHandle;
    string mID;
    int mDepth;
    int mSliceID;
    int mRowOrdinal;
    int mAxis;
    bool mbExpand;
    bool mbApplyAll;
    vector<DSSBaseElementProxy*> mSiblingElementsProxy;
};


#endif /* DSSCManipulationExpandRA_h */
