/*
 *  DSSRWFieldGroupNode.h
 *  DashboardViewer
 *
 *  Created by Luo Ming on 10-1-27.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef RWFieldGroupNode_h
#define RWFieldGroupNode_h

#include <vector>
#include "DSSField.h"
#include "DSSTransactionReport.h"

class DSSDataModelFieldGroupImpl;
class DSSRWFieldGroupModel;
class IDSSFieldGroupModel;

class DSSRWFieldGroupNode : public DSSRWNode
{
private:
	DSSFields* mpFields;
	DSSSorts* mpSorts;
	int mDatalevel;
	DSSDataModelFieldGroupImpl* mpFieldGroupNodeModel; // old one
	DSSDataModelFieldGroupImpl* mpFieldGroupNodeModelLatest; // new latest one for on-demand loading feature
	GUID mTransactionReportID;
	EnumDSSRWNodeTransactionChangeType mTransactionChangeType;
    int mRecordNumber;
    std::string mstrSeperator;
    DSSRWFieldGroupModel* mpFieldGroupModelNewRWD;    //new rwd engine
    std::string mOfflineTransactionCSIKey;
public:
	DSSRWFieldGroupNode();
	virtual ~DSSRWFieldGroupNode();
	DSSRWFieldGroupNode(DSSObjectContext* ipObjectContext);
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader, bool ibDelSubTree=true);
	DSSFields* getFields();
	DSSSorts* getSorts();
	int getDataLevel();
	DSSDataModelFieldGroupImpl* getFiledGroupNodeModel();
	void setFiledGroupNodeModel(DSSDataModelFieldGroupImpl* ipFieldGroupNodeModel);
	void UpdateDataModel();
	void MakeCleanDefn(bool ibDelSubTree=true);
	DSSTransactionReport* getTransactionReport();
	EnumDSSRWNodeTransactionChangeType getTransactionChangeType();
    int getTransactionRecordNumber();
	bool IsTransactionEnabled();
	std::vector<int> getRequiredColumnIndexs();
    void Duplicate(DSSRWNode* ipNode);
	void DiscardTransaction();
    int LoadInputControlModel(std::string iKey, DSSDataModelControlImpl** oppControl);
    std::string getSeperator();
    inline void setSeperator(std::string iValue);
    
    //added for new rwd engine to keep backward compatibility
    inline void setFieldGroupModelForNewRWD(DSSRWFieldGroupModel *ipNodeModel){mpFieldGroupModelNewRWD = ipNodeModel;};
    inline DSSRWFieldGroupModel* getFieldGroupModelForNewRWD(){return mpFieldGroupModelNewRWD;};
    DSSDataModelTransaction *getTransactionDataModel();
    //fyang; added for new RWD engine to keep backward compatibility
    IDSSFieldGroupModel* getFieldGroupModelInterface();
    DSSOfflineTransactionCSIInfo* getOfflineTransactionCSIInfo();
};

#endif
