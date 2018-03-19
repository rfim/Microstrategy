/*
 *  DSSDocumentViews.h
 *  ReportServiceCore
 *
 *  Created by Cao Lei on 11-8-12.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef DOCUMENT_VIEWS_H
#define DOCUMENT_VIEWS_H

#include "DSSDocumentView.h"
#include "DSSBinaryDataReader.h"
#include <vector>

class DSSDocumentViews
{
private:
	std::vector<DSSDocumentView*> mvDocumentViews;
	int mnCurrentDocumentView;
public:
	DSSDocumentViews();
	~DSSDocumentViews();

	int Count();
	DSSDocumentView* Item(int iIndex);
	void Add(DSSDocumentView* ipDocumentView, int iIndex = -1);
	void Remove(int iIndex);
	void Clear();
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
	int getCurrentDocumentView();
	void setCurrentDocumentView(int iViewIdx);
};

#endif
