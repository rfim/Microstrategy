/*
 *  DSSCManipulationSortTemplate.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-4-21.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CMANIPULATION_SORT_TEMPLATE_H
#define CMANIPULATION_SORT_TEMPLATE_H

#import "GUID.h"
#import "DSSRWIterator.h"
#import "DSSCellHandle.h"
#include "DSSCManipulation.h"
#include "DSSAOSort.h"
class DSSRWTemplateNode;
class DSSTemplateAttribute;
class DSSTemplate;

class DSSCManipulationSortTemplate : public DSSCManipulation
{
private:
	DSSRWTemplateNode* mpTemplateNode;
	DSSCellHandle* mpCellHandle;
	bool mAscending;
	DSSRWIterator* mpIterator;
	int hFindMatchedBaseFormID(DSSTemplateAttribute* ipAttribute, GUID iFormID);
    EnumDSSSortFlag mSortFlag;
	
	// members for the case from AO:
	EnumDSSAxisName mAxisIndex;
	GUID mID;
	GUID mFormID;
	StringVector* mpElementIDs;
    bool mAOXML;
    int mSubtotalPosition;
    EnumDSSObjectType mObjectType;
    
    bool mIsClearSort;
    int mBaseFormID;
    EnumDSSSortType mSortType;
public:
	DSSCManipulationSortTemplate(DSSRWIterator* ipIterator, DSSCellHandle* ipCellHandle, bool iAscending, bool iAOXML = true, EnumDSSSortFlag iSortFlag = DssSortFlagDefault);
    DSSCManipulationSortTemplate(DSSActionObject* ipAO, DSSObjectContext* ipContext);
    // used for clear sort
    DSSCManipulationSortTemplate(DSSRWIterator* ipIterator, DSSCellHandle* ipCellHandle);
    DSSCManipulationSortTemplate(DSSRWIterator* ipIterator, int iAxisIndex);
	virtual ~DSSCManipulationSortTemplate();
	int Execute();
    
    DSSRWTemplateNode* getTemplateNode();
    EnumDSSSortFlag getSortFlag();
    static int getSubtotalPosition(DSSSorts* ipSorts, GUID iObjectID, GUID iFormID = GUID_NULL);
    bool getIsAscending();
    
private:
    void GenerateAODeltaXML(bool isRemoveSort = false);
    
    std::string hBuildAODeltaXML();
    bool hInit();
    
    void hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext);
    
};

inline EnumDSSSortFlag DSSCManipulationSortTemplate::getSortFlag()
{
    return mSortFlag;
}

#endif
