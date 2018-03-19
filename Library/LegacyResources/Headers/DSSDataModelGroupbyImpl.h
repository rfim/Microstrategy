/*
 *  DSSDataModelGroupbyImpl.h
 *  ReportServiceCore
 *
 *  Created by xiaoyan yu on 4/21/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#import "DSSDataModelBase.h"

enum EnumAEUnsetType {//only choosefirst is used currently
	DssAEUnsetTypeChooseFirst = 0,
	DssAEUnsetTypeChooseFirstNotAll = 1,
	DssAEUnsetTypeChooseLast = 2
};
struct GroupbyElement{
	int index;//the current element index but the total length of the integer array in bin data for root node
	vector<GroupbyElement> children;
    int flag;
    GroupbyElement* parent;
	void generateAllElementList(int curLev, int totLev);
    int getLeaveCount(vector<GroupbyElement*> *opvGroupbyElements);
};

typedef struct{
	MBase::CString displayText;
	DSSBaseElementProxy *element;
}SpecialElement;

class DSSDataModelGroupbyImpl : public DSSDataModelBase
{
public:
	DSSDataModelGroupbyImpl();
	~DSSDataModelGroupbyImpl();//TODO: clear mpRootElement
	
	void SetRootElement(GroupbyElement *ipRootElement);
	GroupbyElement* GetRootElement();
	int SetTexts(int index,vector<MBase::CString> &irvDisplayTexts);
	void SetSpecialCount(int n);
	int SetSpeical(int iOffset,MBase::CString &irDisplayText, MBase::CString &irElementID);
	
	int GetText(int index, int unit, MBase::CString &orText);//index corresonding to select method
	
	int GetElements(vector<int> &irvIndexes, int unit, vector<DSSBaseElementProxy *> &orvElements);
	
	int GetAllText(MBase::CString &orText);//display text for all
	DSSBaseElementProxy *GetAllElement();
	int Select(vector<DSSBaseElementProxy*> &irvGBPath, bool iRemoveAll, int iUnsetType, vector<int> &orvIndexes, bool iUseMask=false);
	
	
	void toString();
	
	virtual int size();

	int GetElementCount(int iUnit);
	int GetSpecialElementCount();
	DSSBaseElementProxy* getSpecialElement(int i);
	std::string getSpecialElementText(int i);
    
    //tliao, 06/09/2013, add for NGB=All 
    int createGBElementVector(GroupbyElement* ipRootElement,  int &oLeaveCount);
    int SetNGBWindow(int iStart, int iEnd);
    int ClearNGBWindow();
    GroupbyElement* GetElement(vector<DSSBaseElementProxy*> &irvGBPath, int iUnsetType);
    bool IsGBElementReady();


private:
	int GetElementIndexList(GroupbyElement * ipElement, int unit,bool iRemoveAll, vector<int> &orvIndexes, bool iUseMask);
	
	GroupbyElement *mpRootElement;
	vector<SpecialElement> mvSpecialElements;//like subtoal and NULL
	int mlAllElementOffset;
	DSSBaseElementProxy *mpAllElement;
	
	vector< vector<MBase::CString> > mvDisplayTexts;
	vector< vector<DSSBaseElementProxy *> > mvUnitElements;
    vector<GroupbyElement*> mvGroupbyElements;
};
