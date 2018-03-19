/*
 *  DSSExpNodeTime.h
 *  ReportServiceCore
 *
 *  Created by Lei Cao on 2/3/12.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef EXPNODETIME_H
#define EXPNODETIME_H
#import "DSSExpNode.h"
class DSSExpNodeTime : public DSSExpNode
{
public:
	DSSExpNodeTime(DSSObjectContext* ipObjectContext, EnumDSSNodeType iNodeType, const std::string& irKey, DSSCExpression* ipExpression);
	~DSSExpNodeTime();
	
	CComVariant* getRawValue();
	CComVariant* getFormattedValue();
	int getDataType();
    void setRawValue(CComVariant iValue);
	void setFormattedValue(CComVariant iValue);
    void setDataType(int iValue);
	
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);	//<to be finished.
	void* createXML(void* ipParentNode, std::vector<DSSObjectInfo*>* ipObjects); 
	
	void Duplicate(DSSExpNode* ipSourceNode); 
	
private:
	CComVariant mRawValue;
	CComVariant mFormattedValue;
    int mDataType;
};

inline CComVariant* DSSExpNodeTime::getRawValue()
{
	return &mRawValue;
}
inline CComVariant* DSSExpNodeTime::getFormattedValue()
{
	return &mFormattedValue;
}
inline void DSSExpNodeTime::setRawValue(CComVariant iValue)
{
	mRawValue = iValue;
}
inline void DSSExpNodeTime::setFormattedValue(CComVariant iValue)
{
	mFormattedValue = iValue;
}
inline int DSSExpNodeTime::getDataType()
{
    return mDataType;
}
inline void DSSExpNodeTime::setDataType(int iValue)
{
    mDataType = iValue;
}
#endif

