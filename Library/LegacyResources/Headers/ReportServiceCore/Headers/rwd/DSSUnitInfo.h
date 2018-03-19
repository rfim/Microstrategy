//
//  DSSUnitInfo.h
//  ReportServiceCore
//
//  Created by Zeng Jinliang on 8/1/14.
//  Copyright (c) 2014 MicroStrategy, Inc. All rights reserved.
//

#ifndef __ReportServiceCore__DSSUnitInfo__
#define __ReportServiceCore__DSSUnitInfo__

#include "DSSSecondClassObject.h"

class DSSUnitInfo : public DSSSecondClassObject
{
public:
	DSSUnitInfo(DSSObjectContext* ipObjectContext);
	virtual ~DSSUnitInfo();
	DSSUnitInfo(GUID iID, DSSObjectContext* ipObjectContext, std::string iAlias = "", bool iIsHide = false, int lFormatRefID = -1);
    
    int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);

	const GUID& getID() { return mID; }
    bool& isHide() { return mIsHide; }
    const bool isHide() const { return mIsHide; }
    std::string& alias() { return mAlias; }
    const std::string& alias() const { return mAlias; }
    std::string& comment() { return mComment; }
    const std::string& comment() const { return mComment; }
    int& formatRefID() { return mFormatRefID; }
    const int formatRefID() const { return mFormatRefID; }

private:
	GUID mID;
    bool mIsHide;
    std::string mAlias;
    std::string mComment;
    int mFormatRefID;
    
    typedef enum EnumPropertiesSend
    {
        DssNonSend = 0x00000000,
        DssHideSend = 0x00000001,
        DssAliasSend = 0x00000002,
        DssCommentSend = 0x00000004,
        DssFormInfoSend = 0x00000008,
        DssGridFormatSend = 0x00000010
    } EnumPropertiesSend;
};

#endif /* defined(__ReportServiceCore__DSSUnitInfo__) */
