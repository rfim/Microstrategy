/*
 *  DSSTemplateAttribute.h
 *  DashboardViewer
 *
 *  Created by Luo Ming on 09-12-30.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef TEMPLATE_ATTRIBUTE_H
#define TEMPLATE_ATTRIBUTE_H

#include "DSSTemplateMetrics.h"
#include "DSSInputControlSetting.h"

class DSSTemplate;
class DSSAttribute;
class DSSAttributeFormWrappers;

class DSSTemplateAttribute : public DSSTemplateUnit
{
private:
	DSSAttributeFormWrappers* mFormWrappers;
	int hLoadDefnFromBinaryHelper(DSSBinaryDataReader* ipReader, DSSTemplate* ipOwnerTemplate = NULL);
	std::map<GUID, DSSInputControlSetting*> mInputControlSettingMap;
    
    // weilin 3/22/2016 support recursive attribute
    std::vector<std::string> mExpandElementId;
    std::vector<int> mExpandLevel;
    std::vector<std::string> mCollapseElementId;
    bool mbIsExpandAll;
    
    DSSThresholds* mpThresholds;
public:
	DSSTemplateAttribute();
	virtual ~DSSTemplateAttribute();
	DSSTemplateAttribute(DSSObjectContext* ipObjectContext);
	DSSAttribute* getAttribute();
	DSSAttributeFormWrappers* getFormWrappers();
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader, int iIndex, DSSAxis* ipAxis, DSSTemplate* ipOwnerTemplate = NULL);
	EnumUnitTransactionFlags IsEditable();
	EnumUnitTransactionFlags IsEditableByForm(GUID iFormID);
	EnumUnitTransactionFlags IsRequired();
	EnumUnitTransactionFlags IsRequiredByForm(GUID iFormID);
	int getColumnIndexByForm(GUID iFormID);
	std::string getInputFormatByForm(GUID iFormID);	
	bool IsMappedForTransaction();
    bool IsMappedForOfflineTransaction();
	DSSRWControl* getInputControlByForm(GUID iFormID);
	std::string getInputControlKeyByForm(GUID iFormID);
    void PopulateDefaultForms();
    int getOfflineColumnIndexByForm(GUID iFormID);
    void clone(DSSTemplateUnit* iAttribute, DSSAxis * ipAxis);
    // US45416 attribute threshold
    DSSThresholds* getThresholds();
    void setThreholds(DSSThresholds* iThresholds);
};

#endif
