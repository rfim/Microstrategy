/*
 *  DSSRWIterator.h
 *  DashboardViewer
 *
 *  Created by Xiaolong Wang on 12/29/09.
 *  Copyright 2010 MicroStrategy, Inc.. All rights reserved.
 *
 */

#import "DSSRWDHelper.h"
#import <map>
class DSSDocumentInstance;
class DSSRWNode;
class DSSDataModelBase;
class DSSBaseElementProxy;
class DSSTabularUnit;
class DSSCellHandle;
class DSSSubtotalElementProxy;
class DSSObjectContext;
class DSSRWSectionIterator;
class DSSDataModelTransaction;
class CComVariant;
class DSSGroupByUnit;
class DSSDataModelControlImpl;
class DSSRWInputControlIterator;
class DSSThreshold;

class DSSRWDataModel;
class DSSRWSectionModelIterator;
class DSSRWNodeModel;

class DSSRWIterator
{
public:
	DSSRWIterator();
	
	virtual ~DSSRWIterator();
	
	virtual int Init(DSSRWNode* ipNode, DSSRWSectionIterator * ipParent, vector<DSSBaseElementProxy*>& ivNGBElements);
	
	virtual int ReInit();
	
	virtual DSSRWNode *getRWNode();
	
	EnumDSSIteratorType getType();

	
	/*!
	 @method     
	 @abstract  get the element from handle
	 @discussion
	 */
	virtual int getCommandList(DSSCellHandle &irHandle, vector<EnumManipulationCommandTypes> &orvCommand) = 0;
	
	/*!
	 @method     
	 @abstract  get element from handle, the caller owns the element
	 @discussion
	 */
	virtual DSSBaseElementProxy * createElementFromHandle(DSSCellHandle &irHandle);
	
	/*!
	 @method     
	 @abstract  get elements from handle, the caller owns the elements
	 @discussion
	 */
	virtual int createElementsFromHandle(DSSCellHandle &irHandle, int iAxisIndex, vector<DSSBaseElementProxy *> &orvpElements);
	
	/*!
	 @method     
	 @abstract  get elements from handle, the caller owns the elements
	 @discussion
	 */
	virtual int createSiblingElementsFromHandle(DSSCellHandle &irHandle, bool iParentElementsOnly, vector<DSSBaseElementProxy *> &orvpElements);
	
	/*!
	@method     
	@abstract  get DSSAxis from Handle
	@discussion
	*/
	virtual EnumDSSAxisName getAxisFromHandle(DSSCellHandle &irHandle);
	
	/*!
	 @method     
	 @abstract  get a form's GUID from Handle
	 @discussion
	 */
	int getFormFromHandle(DSSCellHandle &irHandle, GUID &orGUID);
	
	/*!
	 @method     
	 @abstract  return if user made any UC changes
	 @discussion
	 */
	bool getUCWarning();
	
	void UpdateNGBElemets(vector<DSSBaseElementProxy*>& ivNGBElements);

	std::vector<DSSBaseElementProxy*>& getNGBElements();
	
	/*!
	 @abstract  return the current element of group-by section node (NGB, CGB & UC) from root to self
	 @discussion 1. if there's duplicate groupby unit on the path, keep the last one
	 */
	int getGroupByElements(vector<DSSBaseElementProxy*> & orvElements);
    
	/*!
	 @method    transaction data model
	 @abstract  change the content of a data cell by the passed in value
	 @discussion
	 */
	virtual int UpdateData(DSSCellHandle &irHandle, CComVariant &irNewValue);
	
	/*!
	 @method    transaction data model
	 @abstract  delete a row specified by the cell handle
	 @discussion
	 */
	virtual int DeleteData(DSSCellHandle &irHandle);
	
	/*!
	 @method    transaction data model
	 @abstract  insert a new row to the data model
	 @discussion
	 */
	virtual int InsertData(vector<std::string> &irNewValue);
	
	/*!
	 @method    transaction data model
	 @abstract  undo the data change made on a particular data cell
	 @discussion
	 */
	virtual int UndoDataChange(DSSCellHandle &irHandle);
	
	/*!
	 @method    transaction data model
	 @abstract  return the embeded transaction data model
	 @discussion
	 */
	virtual DSSDataModelTransaction* getTransactionDataModel();
	
	/*!
	 @method    transaction data model
	 @abstract  return transaction data model's content in XML format
	 @discussion
	 */
	virtual int getTransactionXML(std::string &orXMLContent);
	
	/*!
	 @method    mark all transaction record as committed
	 @abstract
	 @discussion
	 */
	virtual int setAllCommit();
	
	/*!
	 @method    mark a row as insert or delete
	 @abstract
	 @discussion
	 */
	virtual int MarkRows(int iRowOrdinal, EnumDSSRWMarkRowAction iActionType);
	
	/*!
	 @method    check the data change type of a data cell
	 @abstract
	 @discussion
	 */
	virtual int getDataChangeType(DSSCellHandle &irHandle, bool ibTrxSubPreset = false);
	
	/*!
	 @method    check any data is changed
	 @abstract
	 @discussion
	 */
	virtual bool HasDataChanged();
	
	/*!
	 @abstract		delete some data that can be loaded back on demand, to reduce appplication's memory foot print
	 @discussion
	 */
	virtual void ReleaseUnusedMemory();
	
	
	/*!
	 @abstract		get the collection of NGB section iterator's current element from ipChildIterator to its root
	 @discussion	1. for example, document has NGB {Year} = "2010, 2011"; NGB {Country} = "USA, Web".
					the NGB path of the grid iterator under section "2011, USA" is {1,0}
	 */
	int getNGBPath(vector<int> &orvNGBPath);
	
	/*!
	 @abstract		find the iterator given the node key and NGB group-by path
	 @discussion	1. irvNGBPath is the NGB group-by path collected by getNGBPath()
					2. if the NGB path is given, only research the first NGB section
	 */
	virtual int FindChildIterator(std::string &irNodeKey, vector<int> &irvNGBPath, DSSRWIterator **oppChildIterator);
    
    virtual int CreateInputControlIterator(std::string iKey, DSSRWInputControlIterator** oppControlIterator);
    
    // jzeng 09/24/15 for performance, extract the reinit logic from create
    virtual int ReInitInputControlIterator();
    virtual int ReInitInputControlIterator(std::string iKey);
    virtual void ClearInputControlIterator(bool keepObsolete = false);
    
    DSSObjectContext* getObjectContext();
    
    void setParentIterator(DSSRWSectionIterator* ipParent){mpParentIterator = ipParent;}
	
    virtual int AnalyzeTransactionThreshold(DSSThreshold* ipThreshold, bool* opResult);
    
    virtual int UpdateRequiredColumnStatus();
    
    virtual int DiscardTransactionChange(DSSCellHandle &irHandle);
    
protected:
    // 11/7/2012 tehe
	int Init(DSSRWNode *ipNode, DSSRWSectionModelIterator *ipParentIterator, bool isNewRWD);
    
	typedef struct GBElement {
		DSSBaseElementProxy *element;
//		DSSTabularUnit *srcunit;		//source tabular unit if the groupby element is not from definition
//		int srckey;					//element key in source unit
		bool selectTotal;			//if the group-by unit enables Total
		bool mIsNotIn;
		GBElement():element(NULL), selectTotal(true), mIsNotIn(false)/* ,srcunit(NULL),ß srckey(0)*/{};
	}GBElement;

	int getGroupbyPaths(DSSDataModelBase *ipModel, vector<int> &orGBPaths, int &ornPaths, int &ornGroupbyUnits, vector<GUID> &irvUnitGUID, bool ibIncludeSelfNode = true, bool ibExcludeUC = false);
	
	// ytang, 2011-02-07, to support IsNotIn
	int getGroupbyPaths2(DSSDataModelBase *ipModel, vector<int> &orGBPaths, int &ornPaths, int &ornGroupbyUnits, vector<GUID> &irvUnitGUID, bool ibIncludeSelfNode, bool ibExcludeUC, bool &obIsNotIn, bool ibReplaceWithAll);
	int hGetCurentElements(DSSDataModelBase *ipModel, vector<vector<GBElement> > &orGBElements, vector<DSSTabularUnit*> &orvTabularUnits, bool& obIsNotIn, bool ibReplaceWithAll = false, bool ibIncludeSelf = true, bool ibExcludeUC = false);
	
    int hGetCurentElements(vector<vector<GBElement> > &lTempGBElements,
                           vector<bool> &lTempIsUC,
                           bool &irbHasNotInCGB,
                           bool ibReplaceWithAll,
                           bool ibExcludeUC);
    
	virtual int hGetNGBPathForChild(DSSRWIterator *ipChildIterator, vector<int> &orvNGBPath);
	
	// We don't cache DSSRWNode directly, but the node key and DI so partial definition retrieval can be supported
	std::string mNodeKey;
	DSSObjectContext * mpObjectContext;	// weak reference
	
	DSSRWSectionIterator *mpParentIterator;
	//DSSDataModelBase *mpModel;
	EnumDSSIteratorType mType;
	
	bool mbUCWarning;
	vector<DSSBaseElementProxy*> mvNGBElements;	
	
	DSSBaseElementProxy *mpAllElement;
	
    std::map<std::string, DSSRWInputControlIterator*> mapKey2InputControl;
    vector<DSSRWInputControlIterator*> mvObsoleteInputControl;
    
private:
	
	int hGenerateGroupbyPath(DSSDataModelBase *ipModel, vector<vector<GBElement> > &irGBElements,
							 vector<DSSTabularUnit*> &orvTabularUnits, bool ibReplaceExlcudedElements,
							 vector<int> &orGBPaths, int &ornPaths);
	
	int hFindElementsFromModel(DSSDataModelBase *ipModel, vector<GBElement> &irGBElements, DSSTabularUnit *ipUnit,
							   bool ibReplaceExlcudedElements, vector<int> &orElementKeys);
	
	int hResolveAllElement(DSSTabularUnit *ipUnit, vector<int> &orElementKeys, bool ibHasSubtotal);
	
	int hResolveSubtotalElement(DSSSubtotalElementProxy *ipElement, int &orKey);

	/*!
	//recursively check with parent if the Normal Group-By Path is already synchronized.
	//return true if it's already sync. Otherwise, update ivNGBPath and return false.
	*/
	bool hSyncNGBWithParent(vector<DSSBaseElementProxy*>& ivNGBPath);
	
	bool hIsMetricConditionSelector(DSSGroupByUnit * ipUnit);

    bool mbNewRWD;
    
};

inline DSSObjectContext* DSSRWIterator::getObjectContext()
{
    return mpObjectContext;
}
