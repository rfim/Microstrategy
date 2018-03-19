
/*
 *  DSSRWControlModelIterator.h
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 9/26/11.
 *  Copyright 2011 MicroStrategy, Inc. All rights reserved.
 *
 */

#ifndef RWControlModelIterator_H
#define RWControlModelIterator_H

#import "DSSRWNodeModelIterator.h"
#import "DSSTabularIndex.h"

#include "DSSRWControlModel.h"
#include "DSSRWSectionModelIterator.h"
#include "DSSRWControlIterator.h"

class DSSRWNode;

//msun, financial report
struct ControlElementInfo
{
    int parentIndex;
    int level;
    bool isExpand;
    bool hasChildren;
};

class DSSRWControlModelIterator:public DSSRWControlIterator,public DSSRWNodeModelIterator
{
public:
	
	DSSRWControlModelIterator();
	
	virtual ~DSSRWControlModelIterator();
    
    int Init(DSSRWDataModel* pDataModel, DSSRWNodeModel *pNodeModel, DSSRWNode *pNode, DSSRWSectionModelIterator *pParentIterator,vector<Int32> & pCurrentElements, DSSTabularIndex *pGroupingIndex);
    
    int  ReInit();
    
    
    virtual int getCommandList(DSSCellHandle &irHandle, vector<EnumManipulationCommandTypes> &orvCommand);
    
    
    int get_Type(EnumDSSIteratorType * pType);
    
    int FindUnit(GUID UnitID, Int32 * pUnit);
    
    
    int get_ElementIndexByRowNum(Int32 iRow, Int32 iUnit, Int32 * pElementIndex);
    
    int get_Tag(Int32 * pTag);
    
    //hyan,16/11/2011, new RWD
	virtual int getElementText(int iIndex,std::string &orText);
    
    DSSBaseElementProxy* CreateElementFromElementID(MBase::CString &irElementID);
    
    //for sort
    virtual int hget_ElementIndex(int iRow, int iUnit, int *pElementIndex);
    
    
    virtual int getMetricValueMin(const CComVariant ** oppVal);
    
	virtual int getMetricValueMax(const CComVariant ** oppVal);
	
	virtual int getMetricValueCount(const CComVariant ** oppVal);
    
    
    virtual ControlCell getControlCell();
    
    virtual ControlCell getControlCellAt(int index);
    
    
    
    //hyan,2012/07/12,since now we don't need to support remove null element feature
    //comment these APIs
//    virtual ControlCell getNonNullControlCellAt(int index);
//    
//    virtual int CountNonNullRows();
    
    virtual int FindElement(DSSBaseElementProxy* ipElement);
    
    virtual void setElementAt(int iIndex, DSSBaseElementProxy* ipElement);
    
    virtual DSSBaseElementProxy* getElementAt(int index);
    
    virtual int getElementDisplayText(DSSBaseElementProxy* ipElement, std::string &orText);
    
    //Conditional logic support
    virtual int AnalyzeTransactionThreshold(DSSThreshold* ipThreshold, bool* opResult);
    
    //financial report
    virtual ControlElementInfo* getElementInfoAt(int index);
    virtual bool isRecursiveUnit();
    virtual int getLevelInfo(int iLevel, int& oCount, int& oSelected, string& oName);
    virtual int getTotalLevels();
    
private:
    int hGenerateElements(DSSRWControl *ipControl);
    int hgetDisplayText(DSSRWControl *ipControl,vector<MBase::CString> &ivDisplayText);
    
    //hyan,2012/03/12
    //we need to check whether an parent selector is empty choose to decide current selector to show nothing or not
    bool hCheckParentSelectorEmptyChoose();
    
    //hyan,2012/03/31
    //check whether current selector has a parent selector
    bool hCheckHasParentSelector();
    
    
    //hyan,2012/03/30
    int hGenerateSingleElement(int index,DSSBaseElementProxy** ippElementProxy);
    
    
    //hyan,2012/07/03
    //hyan,2012/07/12,since now we don't need to support remove null element feature
    //comment these APIs
    //int hGenerateNonNullIndexArray();
    
    //generate recursive unit level information
    int hGenerateLevelInfo();
    int hClearCacheValues();
private:
    vector<MBase::CString> mvDisplayText;
    DSSTabularIndex *mpGroupingIndex;
    
    //use for metrics selector
    vector<GUID> mvTemplateMetricsGUID;
    vector<GUID> mvTemplateAttributesGUID;
    
    //use for panel selector
    vector<string> mvPanelNodeKeys;
    
    
    map<int, string> *mpDisplayTextMap;
    map<int,DSSBaseElementProxy*>  *mpElementProxyMap;
    
    //financial report
    vector<ControlElementInfo*> mvElementInfo;
    //vector<vector<int> > mvLevelMap;
    map<int, vector<int>* > mvLevelMap;
    
    //hyan,2012/03/30
    DSSRWControl *mpControl;

    
    
    bool mbAfterManipulation;
    
    
    
    //hyan,2012/03/31
    //flag to figure whether we cache displayText and ElementProxy in Map
    bool mbCache;
    
    
    //hyan,2012/05/23
    //add for optimize getElementText
    DSSObjectInfo *mpObjectInfo;
    DSSFormat *mpControlFormat;

    //hyan,2012/05/28
    DSSTabularAttribute *mpTabularAttribute;
    
    //hyan,2012/07/03
    //hyan,2012/07/12,since now we don't need to support remove null element feature
    //comment this member
    //vector<int> mvNonNullIndex;

};




#endif