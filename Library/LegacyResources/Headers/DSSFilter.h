/*
 *  DSSFilter.h
 *  ReportServiceCore
 *
 *  Created by dong shi on 12/2/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef FILTER_H
#define FILTER_H

#import <string>
#import "DSSObjectInfo.h"

class DSSCExpression;
class DSSFilter : public DSSObjectInfo
{
public:
	DSSFilter(GUID& iID, std::string iName, DSSObjectContext* ipContext, GUID iContainerID);
    ~DSSFilter();	
	
	DSSCExpression* getExpression();
	void setExpression(DSSCExpression* ipExpression);
	
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
    
    void Duplicate(DSSFilter* ipFilter);
	
private:
	DSSCExpression* mpExpression;
};

inline 	DSSCExpression* DSSFilter::getExpression()
{
	return mpExpression;
}
inline void DSSFilter::setExpression(DSSCExpression* ipExpression)
{
	mpExpression = ipExpression;
}


#endif
