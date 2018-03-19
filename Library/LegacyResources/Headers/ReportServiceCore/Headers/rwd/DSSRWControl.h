/*
 *  DSSRWControl.h
 *  DashboardViewer
 *
 *  Created by Luo Ming on 09-12-30.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef RWControl_h
#define RWControl_h

#include "DSSRWSectionNode.h"
#include "DSSNonSharedPropertiesInViews.h"
#include <vector>
#include <map>
#include <set>
class DSSCExpression;
class DSSDataModelControlImpl;
class DSSAttributeFormWrappers;
class DSSGroupByUnits;

typedef std::set<int> PlaceHolderIndexSet;
typedef std::map<std::string, PlaceHolderIndexSet> PlaceHolderMap;

class DSSRWControl: public DSSSecondClassObject
{
private:
	EnumDSSRWControlType mType;
	std::string mName;
	std::string mKey;
	bool mShowAll;
	std::string mAllAlias;
	GUID mSourceID;
	std::vector<std::string> mTargetKeys; // target node keys
    std::vector<std::string> mTargetControlKeys;
	DSSBaseElementsProxy* mpElements;
	EnumDSSCurrentElementStatus mStatus;
	double mLeft;
	double mTop;
	double mWidth;
	double mHeight;
	int mZIndex;
	std::string mToolTipText;
	std::string mTitle;
	DSSDocumentSubtotalRefs* mpSubtotals;
	DSSDataModelControlImpl* mpControlNodeModel;
	DSSDataModelControlImpl* mpControlNodeModelLatest;
	int mFormatRefIDs[DssRWControlFormatLastOne-1];
	int hLoadFormatsFromBinary(DSSBinaryDataReader* ipReader);
	EnumDSSXMLRWSelectorUnsetStatus mUnsetStatus;
	int mUnsetElementNumber;
	DSSCExpression* mpExpression;
	
	EnumDSSRWControlActionType mActionType;
	EnumDSSRWControlSubsequentAction mSubsequentAction;
	
	EnumDSSObjectType mSubsequentActionObjectType;
	GUID mSubsequentActionObjectID;
	std::string mSubsequentDisplayMessage;
	std::string mDisplayText;
	
	int mResolvedFormatRefID;
	
	DSSNonSharedPropertiesInViews* mpNonSharedPrptiesInViews;
	std::vector<GUID> mTargetDatasetIDs;

	//for data driven input
	DSSAttributeFormWrappers* mpWriteBackForms;
	DSSGroupByUnits* mpGroupByPath;

	GUID mDataSourceID;	// corresponding DSSDataElement ID, used for DDIC
    DSSRWNode* mRWNode;
    
    DSSSorts* mpSorts;
    
    // for search selector
    EnumDSSAttributeFormDisplayOption mAttributeFormDisplayOption;
    DSSAttributeFormWrappers* mpCustomAttributeForms;
    void LoadAttributeFormsFormBinary(DSSBinaryDataReader* ipReader);
    
    // for targetting in DDIC
    EnumDSSRWControlFilterType mFilterType;
    std::string mNodeKeyDDIC;
    
    // for barcode
    DSSAttributeFormWrappers* mpMatchForms;
    
    DSSThresholds* mpTransactionThresholds;
    bool mbMarkRequired;
    
    // for placeholder
    // mSelectorValues is used to text searching, which will be realized in future.
    std::vector<std::string> mSelectorValues;
    PlaceHolderMap mPlaceHolderMap;
    
    // for attribute selector
    GUID mReplacementAttributeID;
    DSSAxis* mCandidateTemplateUnits;

public:
	DSSRWControl();
	virtual ~DSSRWControl();
	DSSRWControl(DSSObjectContext* ipObjectContext);	
	EnumDSSRWControlType getType();
	std::string getName();
	void setName(std::string value);
	std::string getKey();
    void setKey(std::string iKey);
	bool getShowAll();
	std::string getAllAlias();
	GUID getSourceID();
	DSSObjectInfo* getSourceObject();
	int getCountTargets();
	std::string getTargetKey(int iIndex);
	DSSRWNode* getTargetNode(int iIndex);
    int getCountTargetControls();
    std::string getTargetControlKey(int iIndex);
    DSSRWControl* getTargetControl(int iIndex);
	DSSBaseElementsProxy* getCurrentElements();
	void setCurrentElements(DSSBaseElementsProxy* ipElements);
	void setCurrentElementsStatus(EnumDSSCurrentElementStatus value);
	EnumDSSCurrentElementStatus getCurrentElementsStatus();
	double getLeft();
	double getTop();
	double getWidth();
	double getHeight();
	void setLeft(double value);
	void setTop(double value);
	void setWidth(double value);
	void setHeight(double value);
	int getZIndex();
	void setZIndex(int value);
	std::string getToolTipText();
	void setToolTipText(std::string value);
	std::string getTitle();
	DSSDocumentSubtotalRefs* getSubtotals();
	
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
	int LoadPartialDefnFromBinary(DSSBinaryDataReader* ipReader);
	DSSDataModelControlImpl* getControlNodeModel();
	void setControlNodeModel(DSSDataModelControlImpl* ipControlNodeModel);
	void UpdateDataModel();
	DSSFormat* getFormat(EnumDSSRWControlFormatType iFormatType);
	void setUnsetStatus(EnumDSSXMLRWSelectorUnsetStatus value);
	EnumDSSXMLRWSelectorUnsetStatus getUnsetStatus();
	void setUnsetElementNumber(int value);
	int getUnsetElementNumber();
	
	DSSCExpression* getExpression();
	void setExpression(DSSCExpression* ipExpression, bool copyExpression = false);
	void setUnset(/*iCGBNodeMap:Array, iUCNodeMap:Array, isus:int = 1, isuc:int = 1*/);
	void ClearCurrentElements();

	EnumDSSRWControlActionType getActionType();
	EnumDSSRWControlSubsequentAction getSubsequentAction();
	
	EnumDSSObjectType getSubsequentActionObjectType();
	GUID getSubsequentActionObjectID();
	std::string getSubsequentDisplayMessage();
	std::string getDisplayText();

	DSSFormat* getResolvedFormat();
	
    // input: the selectorType (EnumSelectorStyle)
    bool isUnset(int = -1); // DE19573 Modified by wenzhou, 09/16/2015. Set a default -1 for old logic as no parameter need passed, add logic to handle when selectorType passed.

    
    DSSGroupByUnits* getGroupbyPath(){return mpGroupByPath;};
    void getGroupByPath(std::vector<DSSGroupByUnit*>& oGroupByPath);
    void setGroupByPath(DSSGroupByUnits* ipUnits);
	
    DSSNonSharedPropertiesInViews* getNonSharedPropertiesInViews();
	DSSAttributeFormWrappers* getWriteBackForms();
	
	int getWidthMode();
	int getHeightMode();
	bool getVisible();
	double getTitleHeight();
    int getSelectorOrientation();

	GUID getDataSourceID();
	virtual bool isUC();
	int getTargetDatasetCount();
	GUID getTargetDatasetItem(int iIndex);
    
    // clark, for carrying-over selections from source document to destination document when doing link drill
    virtual DSSRWNode* getNode();
    void setNode(DSSRWNode* iRWNode);
    
    void setSourceID(GUID iSourceID);
    void setControlType(EnumDSSRWControlType iControlType);
    void setShowAll(bool iShowAll);
    void addTarget(std::string iTargetKey);
    void removeTarget(int iIndex);
    
    void Duplicate(DSSRWControl* ipControl);
    void UpdateTargets(DSSRWControl* ipControl = NULL, std::map<std::string, std::string>* iKeyMap = NULL);
    
    EnumDSSAttributeFormDisplayOption getAttributeFormDisplayOption();
    void setAttributeDisplayOption(EnumDSSAttributeFormDisplayOption iAttributeFormDisplayOption);
    GUID getFormID();
    void getFormIDs(std::vector<GUID>& ioFormIDs);
    // TQMS 634076, add baseFormTypes as one property of selector, use this property to create search string for search selector. Added by lcao, 10/12/2012.
    //std::vector<int>* getBaseFormTypesFromUnitObject();
    
    int getBaseFormTypeFromFormID(GUID& iGUID);
    int getBaseFormIDFromFormID(GUID& iGUID);
    EnumDSSRWControlFilterType getFilterType();
    DSSAttributeFormWrappers* getMatchForms();
    
    DSSSorts* getSorts();
    void setSorts(DSSSorts* lpSorts);
    
    DSSAttributeFormWrappers* getCustomAttributeForms();
    void setDefaultFormat(EnumDSSDefaultFormats iDefaultFormatType, EnumDSSRWControlFormatType iCtrlFmtType);
    DSSFormat* getResolvedMetricFormat();
    void setFormatRefID(int iIndex, int iFormatRefID);
    bool getHighlightedNodeKeys(std::vector<std::string>& oNodeKeys);

    std::string getNodeKeyDDIC();
    void setNodeKeyDDIC(std::string iNodeKeyDDIC);
    void populateTargetControls();
    
    DSSThresholds* getTransactionThresholds();
    bool getMarkRequired();
    void setMarkRequired(bool ibMarkRequired);
    
    const std::vector<std::string>& getSelectorValues() { return mSelectorValues; }
    const PlaceHolderMap& getPlaceHolderMap() { return mPlaceHolderMap; }
    const PlaceHolderIndexSet& getPlaceHolderIndexSet(const std::string& iKey);
    
    bool collectAffectedControlKeys(std::vector<std::string>& oControlKeys);
    
    bool hasTarget() const;
    int getAllTargetNodeCount() const;
    DSSRWNode* getAllTargetNode(int iIndex);
    GUID getReplacementAttributeID();
    DSSAxis* getCandidateAttributeElements();
    DSSTemplateUnit* getTemplateUnit(DSSAttribute* iAttribute);

    // weilin US51811 2016/07/18 get property
    virtual int GetProperty(std::string iPropertySetName, int iPropertyIndex, CComVariant* ComVar);
private:
    bool hGetHighlightedNodeKeys(const std::string& targetKey, std::vector<std::string>& oNodeKeys);
    bool hCollectAffectedControlKeys(DSSRWNode* ipNode, std::vector<std::string>& oControlKeys);
};

inline void DSSRWControl::setKey(std::string iKey)
{
    mKey = iKey;
}

inline EnumDSSRWControlFilterType DSSRWControl::getFilterType()
{
    return mFilterType;
}

inline DSSAttributeFormWrappers* DSSRWControl::getMatchForms()
{
    return mpMatchForms;
}

inline DSSAttributeFormWrappers* DSSRWControl::getCustomAttributeForms()
{
    return mpCustomAttributeForms;
}

inline const PlaceHolderIndexSet& DSSRWControl::getPlaceHolderIndexSet(const std::string& iKey)
{
    PlaceHolderMap::const_iterator it = mPlaceHolderMap.find(iKey);
    if (it != mPlaceHolderMap.end())
    {
        return it->second;
    }
    else
    {
        PlaceHolderIndexSet lPlaceHolderIndexSet;
        std::pair<PlaceHolderMap::const_iterator, bool> lPair = mPlaceHolderMap.insert(std::make_pair(iKey, lPlaceHolderIndexSet));
        if (!lPair.second)
            assert(false);
        return lPair.first->second;
    }
}

inline bool DSSRWControl::hasTarget() const
{
    if (mTargetDatasetIDs.size() > 0)
        return true;
    if ((mTargetKeys.size() > 0) || (mTargetControlKeys.size() > 0) || (mPlaceHolderMap.size() > 0))
        return true;
    
    return false;
}
#endif