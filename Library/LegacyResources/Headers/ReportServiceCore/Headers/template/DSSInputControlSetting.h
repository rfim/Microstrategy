/*
 *  DSSInputControlSetting.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 11-2-27.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef INPUTCONTROLSETTING_H
#define INPUTCONTROLSETTING_H
#include "DSSIPhoneEnums.h"
#include <string>
#include "DSSSecondClassObject.h"

class DSSRWControls;

class DSSInputControlSetting : public DSSSecondClassObject
{
private:
	EnumUnitTransactionFlags mEditable;
	EnumUnitTransactionFlags mRequired;
	int mColumnIndex;
    int mOfflineColumnIndex;
	std::string mFormatString;
	DSSRWControls* mpControls;
    DSSRWNode* mpRWNode;
	
public:
	DSSInputControlSetting();
	virtual ~DSSInputControlSetting();
	DSSInputControlSetting(DSSObjectContext* ipObjectContext);

	EnumUnitTransactionFlags getEditable();
	EnumUnitTransactionFlags getRequired();
	int getColumnIndex();
    int getOfflineColumnIndex();
	std::string getFormatString();

    DSSRWControls* getControls();
	DSSRWControl * getControl();
	
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
    DSSRWNode* getRWNode();
    void setRWNode(DSSRWNode* ipRWNode);
};

#endif