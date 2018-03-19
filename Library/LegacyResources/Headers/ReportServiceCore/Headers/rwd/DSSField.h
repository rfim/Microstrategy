/*
 *  DSSField.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-3-27.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef Field_h
#define Field_h

#include <vector>
#include "DSSFields.h"
#include "DSSInputControlSetting.h"
#include "DSSNonSharedPropertiesInViews.h"

class DSSFields;

class DSSField : public DSSSecondClassObject
{
private:
	std::string mKey;
	EnumDSSRWFieldType mType;
	std::string mName;
	std::string mText;
	int mIndex;
	double mLeft;
	double mTop;
	double mWidth;
	double mHeight;
	int mZIndex;
	std::string mToolTipText;
	std::string mURL;
	std::string mHTML;
	std::string mHyperlink;
	int mFormatRefID;
	EnumDSSObjectType mObjectType;
    EnumDSSAutoText   mTextType;
	GUID mObjectID;
    GUID mDataSourceObjectID;
	DSSAttributeFormWrappers* mpAttributeForms;
    std::vector<DSSFields*> mpChildFields;
	DSSThresholds* mpThresholds;
    DSSCExpression* mpExpression;
	EnumUnitTransactionFlags mEditable;
	DSSInputControlSetting* mpInputControlSetting;
	DSSNonSharedPropertiesInViews* mpNonSharedPrptiesInViews;
    bool mCalculationResolved;
	DSSRWFieldGroupNode* mpFieldGroupNode;
    DSSThresholds* mpTransactionThresholds;
    
	int hLoadAttributeFormsFromBinary(DSSBinaryDataReader* ipReader);	
						
public:
	DSSField();
	virtual ~DSSField();
	DSSField(DSSObjectContext* ipObjectContext);
	EnumDSSRWFieldType getType();
    void setType(EnumDSSRWFieldType iType);
	std::string getKey();
    int get_KeyNS(std::string *opKey);
    void setKey(std::string iKey);
	int getIndex();
    void setIndex(int iIndex);
	double getLeft();
    void setLeft(double iLeft);
	double getTop();
    void setTop(double iTop);
	virtual double getWidth();
    void setWidth(double iWidth);
	virtual double getHeight();
    void setHeight(double iHeight);
	int getWidthMode();
	int getHeightMode();
	bool getVisible();
    bool getShowBadge();
	int getZIndex();
    void setZIndex(int iIndex);
	std::string getName();
    void setName(std::string iName);
	std::string getToolTipText();
    void setTooltipText(std::string iTooltipText);
    std::string& getTextRef();
	std::string getText();
    void setText(std::string iText);
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
	DSSFormat* getFormat();
	DSSObjectInfo* getFieldObject();
    void setFieldObject(EnumDSSObjectType iObjectType, GUID iObjectID);
    void setFormatRefId(int iFormatRefId);
	std::string getFieldObjectName();
	DSSAttributeFormWrappers* getAttributeForms();
	DSSThresholds* getThresholds();
    DSSCExpression* getExpression();
    GUID getDataSourceObjectID();
    EnumDSSAutoText getAutoTextType();
	std::string getHyperLink();
	
	//hyan;2011/12/21
    std::string getHtml();
    std::string getURL();
	DSSFields* getChildFields();
    std::vector<DSSFields*>* getAllChildFields();
    
    //hyan;2011/01/18
    DSSFields* getLinkChildFields(EnumDSSRWLinkType iLinkType);
    
    
	EnumUnitTransactionFlags IsEditable();
	EnumUnitTransactionFlags IsRequired();
	int getColumnIndex();
	std::string getInputFormat();
	bool IsMappedForTransaction();
	DSSRWControl* getInputControl();
	std::string getInputControlKey();
    bool getCalculationResolved();

    DSSNonSharedPropertiesInViews* getNonSharedPropertiesInViews();
    
    void Duplicate(DSSField* ipField);
    DSSRWFieldGroupNode* getFieldGroupNode();
    void setFieldGroupNode(DSSRWFieldGroupNode* ipFieldGroupNode);
    
    DSSThresholds* getTransactionThresholds();
    
    bool IsMappedForOfflineTransaction();
    int getOfflineColumnIndex();
    
    EnumDSSObjectType getObjectType() {return mObjectType; }
    GUID getObjectID() { return mObjectID; }
};

inline DSSRWFieldGroupNode* DSSField::getFieldGroupNode()
{
    return mpFieldGroupNode;
}

inline void DSSField::setFieldGroupNode(DSSRWFieldGroupNode *ipFieldGroupNode)
{
    mpFieldGroupNode = ipFieldGroupNode;
}

#endif

