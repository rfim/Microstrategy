//
//  DSSRWDDICInputControlIterator.h
//  ReportServiceCore
//
//  Created by Taimin Liao on 11/23/12.
//  Copyright (c) 2012 MicroStrategy, Inc. All rights reserved.
//


#import "DSSRWInputControlIterator.h"
#import "DSSBaseElementsProxy.h"

class DSSRWGridModel;
class DSSRWDDICCompare;

class DSSRWDDICInputControlIterator : public DSSRWInputControlIterator
{
public:
	DSSRWDDICInputControlIterator();
	virtual ~DSSRWDDICInputControlIterator();
	
	int Init(std::string iKey, DSSObjectContext* ipObjectContext, DSSRWNodeModel* ipNodeModel, DSSRWDataModel* ipDataModel);
	
	virtual int ReInit(bool ibRefresh = true, bool ibDDICOnly = false);
    
    virtual int getElementFormValue(DSSBaseElementProxy* ipElement, int iBaseForm, std::string& orValue);
    
    virtual int getElementFormRawValue(DSSBaseElementProxy* ipElement, int iBaseForm, const char **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag);
    
    virtual int getCachedWriteBackValues(vector<string>** opWritebackValues);
    
    int getAllRawValuesForColumn(GUID iObjID, int iBaseForm, vector<string>* opValues);
    
    int getElementText(int iIndex,std::string &orText);
    
    int getElementMatchText(DSSBaseElementProxy* ipElement, std::string &orText);
    
    virtual ControlCell getControlCell();
    
    virtual ControlCell getControlCellAt(int index);
    
    //zhyang
    int getTabularUnit(GUID &iGUID, DSSTabularUnit **oppUnit);
    
    int generateMatchFormTexts();
    int getElementMatchTexts(vector<string>** oppTexts);
    int getElementDisplayTexts(vector<string>** oppTexts);
    virtual int getCachedElementDisplayTexts(vector<string>** oppTexts);
    //xiazhou
    inline DSSRWDataModel *getDataModel(){return mpDataModel;}
    int findLocalUnitKey(GUID &iGUID, int &oKey);
    inline DSSRWNodeModel *getContaineNodeModel(){return mpNodeModel;}
    DSSBaseElementsProxy *getReplacedElementsForGB(GUID &iGUID);
    
    //xiazhou, override these methods for incremental fetch
    virtual DSSBaseElementProxy *getElementAt(int index);
    virtual int FindElement(DSSBaseElementProxy *ipElement);
    virtual int FindElementDisplayIndex(DSSBaseElementProxy *ipElement);
    
    inline map<int, string>* getDisplayTextCache(){ return mpDisplayTextMap;}
    inline map<int, DSSBaseElementProxy*>* getElementProxyCache() { return mpElementProxyMap;}
private:
    DSSRWNodeModel* mpNodeModel;
    std::string mContainerKey;//xiazhou, record container key to update mpNodeModel
    DSSRWDataModel* mpDataModel;
    vector<std::string> mvDisplayText;
    vector<std::string> mvMatchText;
    vector<std::string> mvWritebackText;
    std::map<GUID, DSSBaseElementsProxy *> mReplaceElementsMap;
    
    //cached elements
    map<int, string> *mpDisplayTextMap;
    map<int,DSSBaseElementProxy*>  *mpElementProxyMap;
    
    GUID mGUID;
    int mGlobalLookupVer;
    
    int hGetDisplayText(DSSRWControl *ipControl,vector<int>* iGlobalKeys = NULL);
    int hGenerateElements(DSSRWControl *ipControl);
    int hGenerateSingleElement(int index, DSSBaseElementProxy **oppElementProxy);
    int hDoInit(DSSRWControl *ipControl);
    int EnsureContainerModelData();
    int hFindElementInLocal(DSSBaseElementProxy *ipElement, DSSTabularUnit *ipUnit, int &oLocalKey);
};



