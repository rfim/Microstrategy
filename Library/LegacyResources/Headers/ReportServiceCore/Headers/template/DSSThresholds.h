/*
 *  DSSThresholds.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-3-31.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef THRESHOLDS_H
#define THRESHOLDS_H

#include "DSSThreshold.h"

class DSSThresholds : public DSSSecondClassObject
{
private:
	std::vector<DSSThreshold*> mThresholds;
	bool mEnabled;
    std::string mNodeKey;
public:
	DSSThresholds();
	virtual ~DSSThresholds();
	DSSThresholds(DSSObjectContext* ipObjectContext);
	int Count();
	DSSThreshold* Item(int iIndex);
	void Add(DSSThreshold* iControl, int iIndex=-1);
    void Clear();
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
	bool getEnabled();
    void setEnabled(bool iEnabled);
    std::string getNodeKey();
    void setNodeKey(std::string iNodeKey);
    // check all threshold's Semantics whether there is replace text or quick symbol or image
    bool hasReplaceText();
};

#endif
