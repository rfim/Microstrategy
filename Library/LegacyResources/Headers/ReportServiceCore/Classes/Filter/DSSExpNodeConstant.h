/*
 *  DSSExpNodeConstant.h
 *  ReportServiceCore
 *
 *  Created by dong shi on 12/2/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef EXPNODECONSTANT_H
#define EXPNODECONSTANT_H
#import "DSSExpNode.h"
class DSSExpNodeConstant : public DSSExpNode
{
public:
	DSSExpNodeConstant(DSSObjectContext* ipObjectContext, EnumDSSNodeType iNodeType, const std::string& irKey, DSSCExpression* ipExpression);
	~DSSExpNodeConstant();
	
	CComVariant* getValue();
	CComVariant* getText();
	void setValue(CComVariant ipValue);
	int getDataType();
	void setDataType(int iType);
	
	
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);	//<to be finished.
	void* createXML(void* ipParentNode, std::vector<DSSObjectInfo*>* ipObjects); 
	
	void Duplicate(DSSExpNode* ipSourceNode); 
	
private:
	CComVariant mText;
	CComVariant mValue;
	int mDataType;
};

inline CComVariant* DSSExpNodeConstant::getText()
{
	return &mText;
}
inline CComVariant* DSSExpNodeConstant::getValue()
{
	return &mValue;
}
inline void DSSExpNodeConstant::setValue(CComVariant iValue)
{
	mValue = iValue;
	mText = iValue;
}
inline int DSSExpNodeConstant::getDataType()
{
	return mDataType;
}
inline void DSSExpNodeConstant::setDataType(int iType)
{
	mDataType = iType;
}
#endif

