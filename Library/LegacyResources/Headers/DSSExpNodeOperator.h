/*
 *  DSSExpNodeOperator.h
 *  ReportServiceCore
 *
 *  Created by dong shi on 12/2/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef EXPNODEOPERATOR_H
#define	EXPNODEOPERATOR_H


#import "DSSExpNode.h"

class DSSFunction;
class DSSSorts;

class DSSExpNodeOperator : public DSSExpNode
{
public:
	DSSExpNodeOperator(DSSObjectContext* ipObjectContext, const std::string& irKey, DSSCExpression* ipExpression);
	~DSSExpNodeOperator();
	
	int getOperatorType();
	void setOperatorType(int iOperatorType);
	
	bool getByValue();
	void setByValue(bool iByValue);
	
	bool getAscending();
	void setAscending(bool iAscending);
    
    int getFunctionIndex();
    void setFunctionIndex(int inFunctionIdx);
    
    GUID getPackageID();
    void setPackageID(GUID iPackageID);
    
    DSSFunction* getFunction();
    DSSSorts* getSorts();

    std::vector<CComVariant> getFunctionPropertyVec();
    void setFunctionPropertyVec(std::vector<CComVariant> iFunctionPropertyVec);
    
    bool getUseLookupForAttributes();
    int getNBBuckets();
    
	void* createXML(void* ipParentNode, std::vector<DSSObjectInfo*>* ipObjects); 
	
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
	
	void Duplicate(DSSExpNode* ipSourceNode);
    
    GUID getFunctionID();
    void setFunctionID(); // set the function id according to the operator type
    void setFunctionID(GUID iFunctionID);
    
    int getFunctionPropertyID(std::string lPropertyName);
    std::string getFunctionPropertyName(int lPropertyID);
private:
    void hSetDefaultSystemProperty();
	bool mIsAscending;
	bool mIsByValue;
	int mOperatorType;
    int mnFunctionIdx;
    GUID mPackageID;
    
    // properties (id-name-value): the size of the following 3 vectors are the same
    // issue 964482, move non-system properties name/Id to function level
    std::vector<CComVariant> mFunctionPropertyVec;
    std::vector<int> mFunctionPropertyIDVec;
    std::vector<std::string> mFunctionPropertyNameVec;
    
    GUID mFunctionID;
    DSSSorts* mpSorts;
    
    bool mUseLookupForAttributes;
    int mNBuckets;
};

inline int DSSExpNodeOperator::getOperatorType()
{
	return mOperatorType;
}
inline void DSSExpNodeOperator::setOperatorType(int iOperatorType)
{
	mOperatorType = iOperatorType;
}
inline bool DSSExpNodeOperator::getByValue()
{
	return mIsByValue;
}
inline void DSSExpNodeOperator::setByValue(bool iByValue)
{
	mIsByValue = iByValue;
}
inline bool DSSExpNodeOperator::getAscending()
{
	return mIsAscending;
}
inline void DSSExpNodeOperator::setAscending(bool iAscending)
{
	mIsAscending = iAscending;
}

inline int DSSExpNodeOperator::getFunctionIndex()
{
    return mnFunctionIdx;
}

inline void DSSExpNodeOperator::setFunctionIndex(int inFunctionIdx)
{
    mnFunctionIdx = inFunctionIdx;
}

inline GUID DSSExpNodeOperator::getPackageID()
{
    return mPackageID;
}

inline void DSSExpNodeOperator::setPackageID(GUID iPackageID)
{
    mPackageID = iPackageID;
}

inline std::vector<CComVariant> DSSExpNodeOperator::getFunctionPropertyVec()
{
    return mFunctionPropertyVec;
}

inline void DSSExpNodeOperator::setFunctionPropertyVec(std::vector<CComVariant> iFunctionPropertyVec)
{
    mFunctionPropertyVec = iFunctionPropertyVec;
}

inline bool DSSExpNodeOperator::getUseLookupForAttributes(){
    return mUseLookupForAttributes;
}

inline int DSSExpNodeOperator::getNBBuckets()
{
    return mNBuckets;
}

inline GUID DSSExpNodeOperator::getFunctionID()
{
	return mFunctionID;
}

inline void DSSExpNodeOperator::setFunctionID(GUID iFunctionID)
{
    mFunctionID = iFunctionID;
}

#endif



