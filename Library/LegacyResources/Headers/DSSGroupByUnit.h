/*
 *  DSSGroupByUnit.h
 *  DashboardViewer
 *
 *  Created by Luo Ming on 09-12-30.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef GroupByUnit_h
#define GroupByUnit_h

#include "DSSDocumentSubtotalRefs.h"
#include <iostream>
#include <vector>
class DSSCExpression;
class DSSExpNode;
class DSSGroupByUnit : public DSSSecondClassObject
{
private:
	GUID mID;
	DSSBaseElementsProxy* mpElements;
	EnumDSSCurrentElementStatus mStatus;
	std::string mAlias;
	bool mHasSubtotal;
	DSSDocumentSubtotalRefs* mpSubtotals;
	
	// the following two only used for NGB
	bool mIsReplaced; 
	DSSBaseElementProxy* mpReplacedElement;
	
	EnumDSSXMLRWSelectorUnsetStatus mUnsetStatus;
	int mUnsetElementNumber;
	DSSCExpression* mpExpression;
	//bool mIsExpressionInternal;
    int mFormatRefID;
    
    std::map<int, DSSCExpression*> mPlaceholderConditions;
    std::map<int, std::string> mPlaceholderLatestControlKeyMap;
	
public:
	DSSGroupByUnit();
	virtual ~DSSGroupByUnit();
	DSSGroupByUnit(DSSObjectContext* ipObjectContext);
	GUID getObjectID();
    void setObjectID(GUID iID);
	EnumDSSObjectType getObjectType();
	DSSObjectInfo* getObject();
	DSSBaseElementsProxy* getCurrentElements();
	EnumDSSCurrentElementStatus getCurrentElementsStatus();
	void setCurrentElementsStatus(EnumDSSCurrentElementStatus iStatus);
	std::string getAlias();
	void setAlias(std::string iAlias);
	bool HasSubtotal();
	DSSDocumentSubtotalRefs* getSubtotals();
	void AddCurrentElement(DSSBaseElementProxy* ipElement);
	void SetCurrentElement(DSSBaseElementProxy* ipElement);
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
	int LoadPartialDefnFromBinary(DSSBinaryDataReader* ipReader);
	bool getIsReplaced();
	DSSBaseElementProxy * getReplacedElement();
	void setIsReplaced(bool iIsReplaced);
	void setReplacedElement(DSSBaseElementProxy* ipElement);
	void setUnsetStatus(EnumDSSXMLRWSelectorUnsetStatus value);
	EnumDSSXMLRWSelectorUnsetStatus getUnsetStatus();
	void setUnsetElementNumber(int value);
	int getUnsetElementNumber();

	DSSCExpression* getExpression();
	void setExpression(DSSCExpression* ipExpression);
	void ClearCurrentElements();
    DSSFormat* getFormat();
    
    void Duplicate(DSSGroupByUnit* ipGroupByUnit);
    
    // function only copy the input elements/expression, caller hold owner
    int pushFromPlaceholderControl(DSSRWControl* ipRWControl, const std::string& iTargetUCNodeKey, DSSBaseElementsProxy* ipElements, DSSCExpression* ipControlCondition, bool ibIsNotIn = false);
    // callee hold owner, caller don't release the output point 
    int getElementsFromPlaceholderCondition(const int iPlaceHolderIndex, bool& oIsExpression, DSSBaseElementsProxy** oppElements);
    int getPlaceholderConditionByPlaceholderIndex(const int iPlaceHolderIndex, std::string& oLatestControlKey, DSSCExpression** oppExpression);
    int getPlaceholderConditionIndexes(std::vector<int>& opIndexes);
	void ClearPlaceholderCondition();

private:
    int hFillElementsCondition(DSSCExpression* lpExpression, DSSObjectInfo* ipTarget, DSSBaseElementsProxy* ipElements, EnumDSSFunction iFunctionType = DssFunctionIn, bool iIncludeTotal = false);
};

inline void DSSGroupByUnit::setObjectID(GUID iID)
{
    mID = iID;
}

#endif
