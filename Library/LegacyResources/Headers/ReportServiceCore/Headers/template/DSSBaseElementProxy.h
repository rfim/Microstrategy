/*
 *  DSSBaseElementProxy.h
 *  DashboardViewer
 *
 *  Created by Luo Ming on 09-12-30.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef BASE_ELEMENT_PROXY_H
#define BASE_ELEMENT_PROXY_H

#include "DSSThresholds.h"
#include <vector>

class DSSBaseElementProxy : public DSSSecondClassObject
{
    
protected:
	std::string mElementID;
	EnumDSSElementType mElementType;
	EnumDSSTemplateUnitType mUnitType;
	GUID mObjectID;
    // DE34057 weilin 2016/06/27 parse string with delimiter
	std::vector<std::string> hParseString(std::string iString, std::string iDelimiter=":");
	std::string mDisplayText;
    int mGlobalKey;
    int mGlobalKeyVer;
    //lizhang, 20160728, de40022, store terse id, move this property from DSSAttributeElementProxy to here.
    std::string mTerseID;
	
public:
	DSSBaseElementProxy();
	virtual ~DSSBaseElementProxy();
	DSSBaseElementProxy(DSSObjectContext* ipObjectContext);
	std::string getElementID();
	GUID getObjectID();
	EnumDSSElementType getElementType();
	EnumDSSTemplateUnitType getUnitType();
	int Parse(std::string iElementID);
	static EnumDSSElementType ParseElementType(std::string iElementID);
	static EnumDSSTemplateUnitType ParseUnitType(std::string iElementID);
	virtual bool IsEqual(DSSBaseElementProxy* ipElement);
    virtual bool IsEqualTrim(DSSBaseElementProxy* ipElement);
	void Clone(DSSBaseElementProxy* ipElement);
	static bool ConvertFromStringToGUID(std::string iString, GUID& pGUID);
	static void ConvertGUIDToString(GUID iID, char* iString); //The string parameter should be newed & deleted out of this function
	std::string getDisplayText();
	void setDisplayText(std::string iDisplayText);
    int getGlobalKey();
    void setGlobalKey(int iGlobalKey, int iGlobalKeyVer = -1);
    int getGlobalKeyVer();
    static void clearVector(std::vector<DSSBaseElementProxy*> &vec);
    //lizhang, 20160728, de40022, store terse id, move this property from DSSAttributeElementProxy to here.
    std::string getTerseID();
    //lizhang, 20160803, de38382, custom border color logic refinement
    void setElementType(EnumDSSElementType iElementType);
};

#endif
