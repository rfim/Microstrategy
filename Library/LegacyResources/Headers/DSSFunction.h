//
//  DSSFunction.h
//  ReportServiceCore
//
//  Created by Zhu, Wei on 3/13/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#ifndef ReportServiceCore_DSSFunction_h
#define ReportServiceCore_DSSFunction_h

#include "DSSObjectInfo.h"

class DSSFunction : public DSSObjectInfo
{
private:
    int mnFunctionIdx;
    GUID mPackageID;
    // issue 964482, move it from DSSExpNodeOperator to function level
    // properties (id-name-value): the size of the following 3 vectors are the same
    std::vector<CComVariant> mFunctionPropertyVec;
    std::vector<int> mFunctionPropertyIDVec;
    std::vector<std::string> mFunctionPropertyNameVec;
    
    EnumDSSFunctionType mAnalyticalType;
    EnumDSSFunction mType;
	EnumDSSFunctionSQLType mSqlType;
    bool mOrderSignificant;
    
public:
	DSSFunction();
	virtual ~DSSFunction();
	DSSFunction(GUID& iID, EnumDSSObjectType iType, std::string iName, DSSObjectContext* iObjectContext, GUID iContainerID);
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);

    int getFunctionIndex();
    void setFunctionIndex(int inFunctionIdx);
    
    GUID getPackageID();
    void setPackageID(GUID iPackageID);
    
    EnumDSSFunction getFunctionType();
    void setFunctionType(EnumDSSFunction iType);
    
    EnumDSSFunctionType getAnalyticalType();
    void setAnalyticalType(EnumDSSFunctionType iType);
    
    EnumDSSFunctionSQLType getSQLType();
    void setSQLType(EnumDSSFunctionSQLType iType);
    
    const std::vector<CComVariant>& getFunctionPropertyVec();
    void setFunctionPropertyVec(const std::vector<CComVariant>& iFunctionPropertyVec);
    int getFunctionPropertyID(const std::string& lPropertyName);
    std::string getFunctionPropertyName(int lPropertyID);
    
    DSSFunction* getPartitionFunction();
    
    bool getOrderSignificant() { return mOrderSignificant; }
    void setOrderSignificant(bool lOrderSignificant) { mOrderSignificant = lOrderSignificant; }
    
	virtual void MakeCleanDefn();
};

inline int DSSFunction::getFunctionIndex()
{
    return mnFunctionIdx;
}

inline void DSSFunction::setFunctionIndex(int inFunctionIdx)
{
    mnFunctionIdx = inFunctionIdx;
}

inline GUID DSSFunction::getPackageID()
{
    return mPackageID;
}

inline void DSSFunction::setPackageID(GUID iPackageID)
{
    mPackageID = iPackageID;
}

inline const std::vector<CComVariant>& DSSFunction::getFunctionPropertyVec()
{
    return mFunctionPropertyVec;
}

inline void DSSFunction::setFunctionPropertyVec(const std::vector<CComVariant>& iFunctionPropertyVec)
{
    mFunctionPropertyVec = iFunctionPropertyVec;
}


inline EnumDSSFunction DSSFunction::getFunctionType()
{
    return mType;
}

inline void DSSFunction::setFunctionType(EnumDSSFunction iType)
{
    mType = iType;
}

inline EnumDSSFunctionType DSSFunction::getAnalyticalType()
{
    return mAnalyticalType;
}

inline void DSSFunction::setAnalyticalType(EnumDSSFunctionType iType)
{
    mAnalyticalType = iType;
}

inline EnumDSSFunctionSQLType DSSFunction::getSQLType()
{
    return mSqlType;
}

inline void DSSFunction::setSQLType(EnumDSSFunctionSQLType iType)
{
    mSqlType = iType;
}

#endif
