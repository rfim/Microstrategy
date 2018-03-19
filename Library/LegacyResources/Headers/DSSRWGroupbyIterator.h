/*
 *  DSSRWGroupbyIterator.h
 *  ReportServiceCore
 *
 *  Created by xiaoyan yu on 4/27/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#import "DSSRWIterator.h"
#import "DSSBaseString.h"
class GroupbyCell;
class DSSDataModelGroupbyImpl;

class DSSRWDataModel; //junwang
class DSSRWGroupbyCompare;
struct GroupbyElement;

enum EnumDSSRWPageByStyle{
	DssRWPageByStyleReserved = 0,
	DssRWPageByStyleIndividual = 1,
	DssRWPageByStyleAll = 2,
	DssRWPageByStyleBoth = 3
};

class DSSRWGroupbyModelAdapter
{
private:
    DSSDataModelGroupbyImpl *mpModel;
    DSSRWDataModel *mpDataModel;
    DSSRWNode* mpNode;
public:
    DSSRWGroupbyModelAdapter(DSSDataModelGroupbyImpl* ipModel, DSSRWDataModel* ipDataModel, DSSRWNode* ipNode);
    ~DSSRWGroupbyModelAdapter();
    int GetElements(vector<int> &irvIndices, int unit, vector<DSSBaseElementProxy *> &orvElements);
    int GetNGBUnitCount();
    int GetElementTexts(vector<int>& irvIndices, int unit, vector<string> &ovTexts);
    DSSBaseElementProxy* getSpecialElement(int i);
    int GetSpecialElementCount();
    std::string getSpecialElementText(int i);
    int GetElementCount(int iUnit);
    GroupbyElement* GetRootElement();
    DSSRWNode* getNode(){return mpNode;}
    int GetAllText(MBase::CString &orText);
    bool isNewRWD(){return mpDataModel;}
    int GetCandidateElementList(vector<int>& orvElementList);
	int GetCandidateElementList(int iParentSelectedRow, vector<int>& orvElementList);
	int GetRowsByNGBPath(int iParentNGB, vector<int>& lvRows);
	bool isElementCompondNull(int index);
	inline void UpdateGBModel(DSSDataModelGroupbyImpl* ipModel) { mpModel = ipModel; }
	int GetMaxElementIndex(int iUnit);
};

class DSSRWGroupbyIterator : public DSSRWIterator
{
public:
	DSSRWGroupbyIterator(DSSDataModelGroupbyImpl *lpModel,DSSRWDataModel *lpDataModel=NULL);
	virtual ~DSSRWGroupbyIterator();
	int Init(DSSRWNode* ipNode);
	int getCommandList(DSSCellHandle &irHandle, vector<EnumManipulationCommandTypes> &orvCommand);
	int GetCells(vector<GroupbyCell> &orvCells);
	int GetAllCellsForGroupbyAll(vector<GroupbyCell> &orvCells); // return all cells instead of only all element
	DSSRWGroupbyIterator* GetChildIterator();
	DSSRWGroupbyIterator* GetChildIterator(DSSRWNode *lpNode);
    
    //junwang, 12/22/2011, for sort
   virtual int hget_ElementIndex(int iRow, int iUnit, int *pElementIndex);
    	
    inline DSSDataModelGroupbyImpl* getModel() {return mpModel;}
    int setModel(DSSDataModelGroupbyImpl* ipModel);
    DSSRWGroupbyModelAdapter* getModelAdapter();
//    bool hasRWDataModel(){return mpDataModel ? true : false;}
    int getElementCountAndSelectedElementIndex(int *opElementCount, int *opSelectedElementIndex);
    int GetCell(int iIndex, vector<GroupbyCell>& orvCell);
    bool IsNewRWD();
protected:
    //junwang, support new rwd NGB data model
    int GetCellsByNGBPath(vector<GroupbyCell>& orvCells, DSSRWDataModel* ipDataModel);
    //junwang, new rwd, 11/29/2011
    DSSRWGroupbyIterator* GetChildIteratorByNGBPath(DSSRWNode* lpNode);
    

private:
	int hUpdateModel();
	DSSDataModelGroupbyImpl *mpModel;
	DSSRWGroupbyIterator *mpChildIterator;
	string mRootNodeKey;
    DSSRWDataModel *mpDataModel;
    vector<int> mvCandidateElements;
    
    DSSRWGroupbyModelAdapter* mpModelAdapter;
    //tliao, 2014/07/23, incremental fetch
    map<int, string> *mpDisplayTextMap;
    //map<int,DSSBaseElementProxy*>  *mpElementProxyMap;
    int mGlobalLookupVer;
    
    friend class DSSRWGroupbyCompare;
};

class GroupbyCell
{
public:

	
	GroupbyCell(DSSBaseElementProxy *lpElement,bool lSelected, MBase::CString &irText);
	~GroupbyCell();
	/*!
	 @method     
	 @abstract  return groupby element
	 @discussion
	 */
	DSSBaseElementProxy* GetElement(){return mpElement;}
	
	/*!
	 @method     
	 @abstract  return the display text of groupby element
	 @discussion
	 */
	void GetText(MBase::CString &orText){orText=mlText;}
	
	/*!
	 @method     
	 @abstract  is the current groupby element selected
	 @discussion
	 */
	bool IsSelected(){return mlSelected;}
	
	
private:
	DSSBaseElementProxy * mpElement;
	MBase::CString		mlText;
	bool mlSelected;
};

