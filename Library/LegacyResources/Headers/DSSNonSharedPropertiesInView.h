/*
 *  DSSNonSharedPropertiesInView.h
 *  ReportServiceCore
 *
 *  Created by Cao Lei on 11-8-14.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef NON_SHARED_PROPERTIES_IN_VIEW_H
#define NON_SHARED_PROPERTIES_IN_VIEW_H

#include "DSSBinaryDataReader.h"

class DSSNonSharedPropertiesInView
{
private:
	double mdblTop;
	double mdblLeft;
	double mdblWidth;
	double mdblHeight;
	double mdblTitleHeight;
	bool mbVisible;
	int mnHeightMode;
	int mnWidthMode;
	bool mbUsePageWidthAsLayoutWidth;
	double mdblMaxHeight;
	bool mbCanGrow;
	bool mbCanShrink;
	bool mbHideIfEmpty;
    int mnSelectorOrientation;
	
public:
	DSSNonSharedPropertiesInView();
	~DSSNonSharedPropertiesInView();

	double getTop();
	void setTop(double idblTop);
	double getLeft();
	void setLeft(double idblLeft);
	double getWidth();
	void setWidth(double idblWidth);
	double getHeight();
	void setHeight(double idblHeight);
	double getTitleHeight();
	void setTitleHeight(double idblTitleHeight);
	bool getVisible();
	void setVisible(bool ibVisible);
	int getHeightMode();
	void setHeightMode(int inHeightMode);
	int getWidthMode();
	void setWidthMode(int inWidthMode);
	bool getUsePageWidthAsLayoutWidth();
	void setUsePageWidthAsLayoutWidth(bool ibUsePageWidthAsLayoutWidth);
	double getMaxHeight();
	void setMaxHeight(double idblMaxHeight);
	bool getCanGrow();
	void setCanGrow(bool ibCanGrow);
	bool getCanShrink();
	void setCanShrink(bool ibCanShrink);
	bool getHideIfEmpty();
	void setHideIfEmpty(bool ibHideIfEmpty);
    int getSelectorOrientation();
    void setSelectorOrientation(int inSelectorOrientation);
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
    void Duplicate(DSSNonSharedPropertiesInView* ipProperties);
};

#endif
