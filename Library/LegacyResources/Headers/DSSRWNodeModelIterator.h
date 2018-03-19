/*
 *  DSSRWNodeModelIterator.h
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 9/26/11.
 *  Copyright 2011 MicroStrategy, Inc. All rights reserved.
 *
 */

#ifndef RWNodeModelIterator_H
#define RWNodeModelIterator_H


#import "DSSRWNode.h"
#import "DSSRWDataModel.h"
#import "DSSRWNodeModel.h"

class DSSCellHandle;
class DSSBinaryDataReader;
class DSSDataModelTransaction;
class DSSDataModelBase;
class GBElement;
class DSSTabularUnit;
class DSSRWSectionModelIterator;

class DSSRWNodeModelIterator
{
public:
	
    virtual int Init(DSSRWDataModel *ipDataModel, DSSRWNode *, DSSRWNodeModel *, DSSRWSectionModelIterator *ipParentIterator);
    
	DSSRWNodeModelIterator();
	
	virtual ~DSSRWNodeModelIterator();
    
	
	virtual int Init(DSSRWNode* ipNode, DSSRWSectionModelIterator * ipParent, vector<DSSBaseElementProxy*>& ivNGBElements);
	
	virtual int ReInit();
	
	DSSRWNode *getRWNode();
	
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
    
    
    //junwang, new rwd, 12/27/2011, from root to current node(not include self)
    void getNonTrivialParents(vector<DSSRWSectionModelIterator*>& ovParentIterators);
    
    void ResetCurrentNodeModel(DSSRWNodeModel *ipNewNodeModel) { mpNodeModel = ipNewNodeModel; }
    
    DSSRWNodeModel* getNodeModel(bool iFromDataModel = false);
    
public:
    DSSRWNode *mpNode;
	DSSRWDataModel *mpDataModel;
    DSSRWNodeModel *mpNodeModel;
    DSSRWSectionModelIterator *mpParentIterator;
    
    // tehe: it should be initialized in Init method
    DSSObjectContext * mpObjectContext;	// weak reference
    EnumDSSIteratorType mType;
    // We don't cache DSSRWNode directly, but the node key and DI so partial definition retrieval can be supported
	std::string mNodeKey;
    
    
    int getGroupbyPaths(DSSDataModelBase *ipModel, vector<int> &orGBPaths, int &ornPaths, int &ornGroupbyUnits, vector<GUID> &irvUnitGUID, bool ibIncludeSelfNode = true, bool ibExcludeUC = false);
	
	// ytang, 2011-02-07, to support IsNotIn
	int getGroupbyPaths2(DSSDataModelBase *ipModel, vector<int> &orGBPaths, int &ornPaths, int &ornGroupbyUnits, vector<GUID> &irvUnitGUID, bool ibIncludeSelfNode, bool ibExcludeUC, bool &obIsNotIn, bool ibReplaceWithAll);
	int hGetCurentElements(DSSDataModelBase *ipModel, vector<vector<GBElement> > &orGBElements, vector<DSSTabularUnit*> &orvTabularUnits, bool& obIsNotIn, bool ibReplaceWithAll = false, bool ibIncludeSelf = true, bool ibExcludeUC = false);
    
protected:
	
    int mGlobalLookupVer;
	
};

#endif