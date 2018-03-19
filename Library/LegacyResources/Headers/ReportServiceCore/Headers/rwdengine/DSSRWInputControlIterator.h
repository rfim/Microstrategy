//
//  DSSRWInputControlIterator.h
//  ReportServiceCore
//
//  Created by Fan Yang on 9/8/11.
//  Copyright 2011 microstrategy. All rights reserved.
//

#import "DSSRWControlIterator.h"

class DSSDataModelControlImpl;
class DSSRWControl;
class DSSObjectContext;
class DSSTabularUnit;
class DSSTabularAttribute;

class DSSRWInputControlIterator : public DSSRWControlIterator
{
public:
	DSSRWInputControlIterator();
	virtual ~DSSRWInputControlIterator();
	
	int Init(std::string iKey, DSSObjectContext* ipObjectContext, DSSDataModelControlImpl* ipControlModel, string iParentNodeKey = "");
	
	virtual int ReInit(bool ibRefresh = true, bool ibDDICOnly = false);
    
    virtual int getElementFormValue(DSSBaseElementProxy* ipElement, int iBaseForm, std::string& orValue);

    virtual int getElementFormRawValue(DSSBaseElementProxy* ipElement, int iBaseForm, const char **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag);
    
    int getParentNodeKey(std::string& orKey);
    
    virtual int getCachedWriteBackValues(vector<string>** opWritebackValues);
    virtual int getCachedElementDisplayTexts(vector<string>** oppTexts);
private:
    int hGetControlElementList();
    
    string mParentNodeKey;
};
