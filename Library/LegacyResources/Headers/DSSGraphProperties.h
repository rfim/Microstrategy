/*
 *  DSSGraphProperties.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-4-20.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef GRAPH_PROPERTIES_H
#define GRAPH_PROPERTIES_H

#include <map>
#include "DSSBinaryDataReader.h"

typedef std::map < short, CComVariant> MapAttribute;
typedef std::map < short, MapAttribute > MapObject;
typedef std::map < int, MapObject > MapSeries;

class DSSGraphProperties
{
private:
	EnumGraphMajorType mGraphType;
	int mGraphSubType;
	int mMaxColumns;
	int mMaxRows;
	bool mSeriesByColumn;
	bool mSubTotalsEnabled;
	int mHeight;
	int mWidth;
	int mSeriesCount;
	bool mIsUseMaxCategoriesAsMin;
	MapSeries mProperties;
	bool mbUseSpecialSeries;
	
public:
	DSSGraphProperties();
	virtual ~DSSGraphProperties();

	EnumGraphMajorType getGraphType();
    void setGraphType(EnumGraphMajorType iGraphType);
	int getGraphSubType();
    void setGraphSubType(int iGraphSubType);
	int getMaxColumns();
	int getMaxRows();
	bool getSeriesByColumn();
	bool getSubTotalsEnabled();
	int getHeight();
	int getWidth();
	int getSeriesCount();
	bool getIsUseMaxCategoriesAsMin();
    bool getUseSpecialSeries();
    MapSeries getProperties();
	
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
	
	CComVariant* getPropertyValue(int iSeriesID, short iObjectID, short iAttributeID);
    void setPropertyValue(int iSeriesID, short iObjectID, short iAttributeID, CComVariant iValue);
    
    void Duplicate(DSSGraphProperties* ipGraphProperties);
};

#endif
