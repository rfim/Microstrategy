//
//  DSSCManipulationApplySortsToTemplate.h
//  ReportServiceCore
//
//  Created by Zhu, Wei on 12/3/12.
//
//

#ifndef ReportServiceCore_DSSCManipulationApplySortsToTemplate_h
#define ReportServiceCore_DSSCManipulationApplySortsToTemplate_h

#import "GUID.h"
#import "DSSRWIterator.h"
#import "DSSCellHandle.h"
#import "DSSBaseElementProxy.h"
#include "DSSSorts.h"
#ifdef __ANDROID__
#include "TinyXml/tinyxml.h"
#else
#include "DSSSimpleXML.h"
#endif
#include "DSSCManipulation.h"
#include "DSSAOSort.h"
#include <vector>

class DSSRWTemplateNode;

class SortInfo
{
public:
    SortInfo(DSSCellHandle* ipCellHandle, bool& iAscending, std::vector<DSSBaseElementProxy*>& iElements)
    {
        mpCellHandle = ipCellHandle;
        mAscending = iAscending;
        mvElements = iElements;
    }
    
    DSSCellHandle* mpCellHandle;
    bool mAscending;
    std::vector<DSSBaseElementProxy*> mvElements; //array of CDSSBaseElementProxy
};

class DSSCManipulationApplySortsToTemplate : public DSSCManipulation
{
private:
	DSSRWTemplateNode* mpTemplateNode;
    std::vector<SortInfo*> mvSortInfo;
    DSSSorts* mpOldSortsRow;
    DSSSorts* mpOldSortsColumn;
    bool mbNeedClear;
    DSSRWIterator* mpIterator;
    EnumDSSSortFlag mSortFlag;
    
    // for replay manipluations
    int mAxisIndex;
    GUID mID;
    GUID mFormID;
    bool mAscending;
    int mSubtotalsPosition;
    
    int hFindMatchedBaseFormID(DSSTemplateAttribute* ipAttribute, GUID iFormID);
    
#ifndef __ANDROID__
    std::vector<SimpleXMLNode> mvReportManipulationXMLs;
    void hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext);
#else
    std::vector<TiXmlElement> mvReportManipulationXMLs;
#endif
    
public:
	DSSCManipulationApplySortsToTemplate(DSSRWIterator* ipIterator, std::vector<SortInfo*>& ivSortInfo, EnumDSSSortFlag iFlag = DssSortFlagDefault);
	DSSCManipulationApplySortsToTemplate(DSSActionObject* ipAO, DSSObjectContext* ipContext);
	virtual ~DSSCManipulationApplySortsToTemplate();
	int Execute();
    
    void GenerateAODeltaXML();
    void GenerateAODeltaXML(int iSortIndex, int iSortType, int iObjType, GUID& iObjID,
                            bool iAscending, int iAxisId,  int iSubtotalsPosition,  DSSBaseElementsProxy* ipElements, GUID iFormID);
    void GenerateAODeltaXMLClear(int iAxisId, int iSubtotalPosition);
    
    EnumDSSSortFlag getSortFlag();
    
    DSSRWTemplateNode* getTemplateNode()
    {
        return mpTemplateNode;
    }
};

inline EnumDSSSortFlag DSSCManipulationApplySortsToTemplate::getSortFlag()
{
    return mSortFlag;
}
#endif
