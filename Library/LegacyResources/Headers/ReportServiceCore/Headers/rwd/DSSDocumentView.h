/*
 *  DSSDocumentView.h
 *  ReportServiceCore
 *
 *  Created by Cao Lei on 11-8-12.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef DOCUMENT_VIEW_H
#define DOCUMENT_VIEW_H

#include "DSSBinaryDataReader.h"

class DSSDocumentView
{
private:
	int mnScreenResolutionX;
	int mnScreenResolutionY;
	int mnScreenDensity;
	int mnOrientation;

public:
	DSSDocumentView();
	~DSSDocumentView();

	int getScreenResolutionX();
	void setScreenResolutionX(int inScreenResolutionX);
	int getScreenResolutionY();
	void setScreenResolutionY(int inScreenResolutionY);
	int getScreenDensity();
	void setScreenDensity(int inScreenDensity);
	int getOrientation();
	void setOrientation(int inOrientation);
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
};

#endif
