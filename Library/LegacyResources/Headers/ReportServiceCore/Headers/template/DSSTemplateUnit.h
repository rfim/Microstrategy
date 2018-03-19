/*
 *  DSSTemplateUnit.h
 *  DashboardViewer
 *
 *  Created by Luo Ming on 09-12-30.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef TEMPLATE_UNIT_H
#define TEMPLATE_UNIT_H

#include "DSSMetric.h"

class DSSAxis;
class DSSDrillPathRefs;
class DSSCExpression;

class DSSTemplateUnit : public DSSSecondClassObject
{
private:
	DSSAxis* mpAxis;
	std::map<int, int> mColumnWidths; // map from depth to width
	int mFormatRefIDs[4];
	int mResolvedFormatRefIDs[4];
	bool mHasDataHeaderFormat;
	bool mHasDataGridFormat;
	DSSDrillPathRefs* mpDrillPathRefs;
    bool mInvalidatResolvedFormat;
    DSSCExpression* mpExpression;
    std::string mCachedDocumentLayerAlias; // jzeng 04/29/2016, for performance
	
protected:
	EnumDSSTemplateUnitType mType;
	GUID mID;
	std::string mAlias;
	std::string mUnitData;
	int mIndex;
    bool mbHasAlias;
    bool mIsPushedDown;
    GUID mReplacedByUnitID;
    bool mIsRecursive;
public:
	DSSTemplateUnit();
	virtual ~DSSTemplateUnit();
	DSSTemplateUnit(DSSObjectContext* ipObjectContext);	
	EnumDSSTemplateUnitType getType();
	void setType(EnumDSSTemplateUnitType iType);
	DSSAxis* getAxis();
	int getIndex();
	std::string& getAliasRef();
    std::string getAlias();
	GUID getID();
    void setID(GUID& iGUID);
	std::string getUnitData();
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader, int iIndex, DSSAxis* ipAxis);
    int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
	DSSObjectInfo* getObject();
	int getColumnWidth(int iDepth);
	DSSFormat* getHeaderFormat(bool ibUseResolved=false, bool ibOriginal=false);
	DSSFormat* getGridFormat(bool ibUseResolved=false, bool ibOriginal=false);
	DSSFormat* getSubtotalHeaderFormat(bool ibUseResolved=false, bool ibOriginal=false);
	DSSFormat* getSubtotalGridFormat(bool ibUseResolved=false, bool ibOriginal=false);
	bool getHasDataHeaderFormat();
	bool getHasDataGridFormat();
    int getResolvedFormatRefID(int iIndex);
	void setResolvedFormatRefID(int iIndex, int iFormatRefID);
	void setHasDataHeaderFormat(bool ibHasDataHeaderFormat);
	void setHasDataGridFormat(bool ibHasDataGridFormat);
    void setAlias(std::string iAlias);
	DSSDrillPathRefs* getDrillPaths();
	void setAxis(DSSAxis* ipAxis);
    int getColumnWidths(int iDepth);
    void setColumnWidths(int iDepth, int iWidth);
    
	virtual EnumUnitTransactionFlags IsEditable();
	virtual EnumUnitTransactionFlags IsRequired();
	virtual int getColumnIndex();
    virtual int getOfflineColumnIndex();
	virtual bool IsMappedForTransaction();
    virtual bool IsMappedForOfflineTransaction();
	void setIndex(int iIndex);
	void IncreaseIndex();
	void DecreaseIndex();
    virtual bool IsInvalidateResolvedFormat();
    virtual void SetInvalidateResolvedFormat(bool iInvalidate);
    
    DSSFormat* getFormat(int iFormatType, bool iIsReadOnly = true);
    
    std::map<int, int> getColumnWidths();
    void Duplicate(DSSTemplateUnit* ipUnit, DSSAxis* ipAxis = NULL);
    void clone(DSSTemplateUnit* ipUnit, DSSAxis* ipAxis);
    void duplicateFormat(DSSTemplateUnit* ipUnit);
    void clearFormat();
    bool isPushedDown();
    GUID getReplaceByUnitID();
    void pushDown();
    void setReplaceByUnitID(GUID iID);
    void setRecursive();
    bool isRecursive();
};

#endif
