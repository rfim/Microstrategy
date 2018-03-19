/*
 *  DSSDataModelControlImpl.h
 *  DashboardViewer
 *
 *  Created by Xiaolong Wang on 12/29/09.
 *  Copyright 2010 MicroStrategy, Inc.. All rights reserved.
 *
 */

#import "DSSDataModelBase.h"

class IDSSElement;
class CComVariant;

class DSSDataModelControlImpl : public DSSDataModelBase
{
public:
	DSSDataModelControlImpl();
	~DSSDataModelControlImpl();
	
	int Init(int inElements);
	
	int Init();//set mType
	
	int AddElement(int iDisplayIndex, MBase::CString &irText, MBase::CString &irElementID, int iIndex = -1);
    
    int SetElementIndex(int i, int iIndex);
    
	int CreateViewMap(vector<int> &irArray);
	
	void setCGBFlag(int flag);

	int CountElements();
	
	int getElementText(int index, MBase::CString &orText);
	
	int Select(int nPaths, int nGBUnits, vector<GUID> &irGBUnitID, int *ipGBPaths, vector<int> &orvElements);
	
    int SelectSliceWindows(int nPaths, int nGBUnits, vector<GUID> &irGBUnitID, int *ipGBPaths, vector<int> &lvSlices);
    
    int GetElementsBySliceWindow(vector<int>& irvSlices, vector<int> &orvElements);
    
	bool HasElementID(int index);
	
	int getElementID(int index, MBase::CString &orElementID);
	
	DSSBaseElementsProxy * createElementAt(int index);
	
	virtual int size();
	
	int setMetricRange(int iType, CComVariant *ipVal);
	
	const CComVariant * getMetricRange(int iType);
	
	virtual void ReleaseUnusedMemory();
    
    int AddElementMatchText(int iDisplayIndex, MBase::CString iText);
    
    int getElementMatchText(int iDisplayIndex, MBase::CString &orText);
    
    int setParentNodeKey(std::string iKey);
    
    int getParentNodeKey(std::string& orKey);
    
    int getElementIndex(int iIndex);
	
private:
	void hCleanUp();
    
    //clear mpElementLIst & mpElementOffset
	int hCleanElementList();
    
	int mnElements;
	
	vector<MBase::CString> mvDisplayText;
	vector<MBase::CString> mvElementID;		//element id string for special elements, like subtoal and NULL
    vector<MBase::CString> mvElementMatchText;//for DDIC, barcode match text
	vector<int> mvIndex;    //for DDIC, the elementIndex may be different from display order
    //element list for each group-by
	int* mpElementList;
    int* mpElementOffset;
    int mnSlice;
    int mnElementListCount;
    bool mbMapped;
    
    char* mpElementListPage;
    int mElementListPageSize;
    int mElementListStart;
    int mElementListPagePosInFile;
    
    char* mpElementOffsetPage;
    int mElementOffsetPageSize;
    int mElementOffsetStart;
    int mElementOffsetPagePosInFile;
    
	int mGhostFlag;
	
	CComVariant *mpMetricValueMin;
	CComVariant *mpMetricValueMax;
	CComVariant *mpMetricValueCount;
    
    std::string mParentNodeKey; //template node or FG node that hosts the DDIC model
};
