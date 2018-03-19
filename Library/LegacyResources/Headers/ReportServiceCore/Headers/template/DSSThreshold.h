/*
 *  DSSThreshold.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-3-27.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef THRESHOLD_H
#define THRESHOLD_H

#include "DSSFormat.h"

class DSSCExpression;

class DSSThreshold : public DSSSecondClassObject
{
private:
	std::string mPrefix;
	std::string mSuffix;
	std::string mReplaceText;
	int mFormatRefID;
	int mSemantics;
	std::string mName;
	DSSCExpression* mpExpression;
    EnumDSSThresholdScope mThresholdScope;
    bool mIsEnabled;
    EnumDSSTransactionAction mTransactionAction;
	int hLoadFormatsFromBinary(DSSBinaryDataReader* ipReader);
    std::string mNodeKey;
	
public:
	DSSThreshold();
	virtual ~DSSThreshold();
	DSSThreshold(DSSObjectContext* ipObjectContext);
	std::string getPrefix();
	std::string getSuffix();
	std::string getReplaceText();
    void setReplaceText(std::string iReplaceText);
	DSSFormat* getFormat(bool iIsReadOnly = true);
    void setFormatRefId(int iFormatRefID);
	int getSemantics();
    void setSemantics(int iSemantics);
	std::string getName();
    void setName(std::string iName);
	DSSCExpression* getExpression();
    void setExpression(DSSCExpression* ipExpression);
    EnumDSSThresholdScope getScope();
    void setScope(EnumDSSThresholdScope iScope);
    bool getIsEnabled();
    void setIsEnabled(bool iIsEnabled);
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
    void Duplicate(DSSThreshold *ipThreshold);
    std::string getNodeKey();
    void setNodeKey(std::string iNodeKey);
    EnumDSSTransactionAction getTransactionAction();
    
};

#endif
