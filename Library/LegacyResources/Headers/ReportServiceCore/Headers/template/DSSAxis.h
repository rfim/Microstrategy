/*
 *  DSSAxis.h
 *  DashboardViewer
 *
 *  Created by Luo Ming on 09-12-30.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef AXIS_H
#define AXIS_H

#include "DSSAttribute.h"
#include <vector>

class DSSTemplate;

class DSSAxis : public DSSSecondClassObject
{
private:
	int mIndex; // 1:row, 2:column
	std::vector<DSSTemplateUnit*> mUnits;
	DSSSorts* mpSorts;
    DSSSorts* mpInitialSorts;
	DSSTemplate* mpTemplate;
	int mFormatRefIDs[4];
	
public:
	DSSAxis();
	virtual ~DSSAxis();
	DSSAxis(DSSObjectContext* ipObjectContext);
	int getIndex();
	int Count();
	DSSTemplateUnit* Item(int iIndex);
	DSSSorts* getSorts();
    DSSSorts* getInitialSorts();
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader, int iIndex, DSSTemplate* ipTemplate);
	DSSTemplateMetrics* getTemplateMetrics();
	DSSFormat* getHeaderFormat();
	DSSFormat* getGridFormat();
	DSSFormat* getSubtotalHeaderFormat();
	DSSFormat* getSubtotalGridFormat();
	void setSorts(DSSSorts* ipSorts);
	DSSTemplate* getTemplate();
    void setTemplate(DSSTemplate* iTemplate);
	//
	int AppendUnit(DSSTemplateUnit * pUnit);
	int InsertUnit(int iIndex, DSSTemplateUnit* pUnit);
    DSSTemplateUnit* AddNewUnit(int iType, int iPosition);
	bool RemoveUnit(int iIndex,DSSTemplateUnit ** oppUnit);
	bool MoveUnit(int SrcIndex,int DstIndex);
    
    void Duplicate(DSSAxis* ipAxis, DSSTemplate* ipTemplate = NULL);
    int getFormatRefID(int index);
};

#endif
