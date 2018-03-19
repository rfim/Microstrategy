/*
 *  DSSDocumentDefinition.h
 *  DashboardViewer
 *
 *  Created by Luo Ming on 09-12-30.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef DOCUMENT_DEFINITION_H
#define DOCUMENT_DEFINITION_H

#include "DSSRWTemplateNode.h"
#include "DSSDocumentViews.h"
#include "DSSLinkedAssociatedRWNodeSet.h"
#include <map>
#include <vector>
#include "DSSElementsPropertiesMap.h"

class DSSDocumentDefinition : public DSSObjectInfo {
	
private:
	EnumDSSDocumentType mDocumentType;
	DSSRWNode* mRootNodes[3];
	std::string mCurrentLayoutKey;
	std::vector<std::string> mLayoutKeys;
	std::string mInitialLayoutKey;
	std::vector<DSSRWNode*> mNodesLoadedOnDemand;
	DSSDocumentViews* mpDocumentViews;
    
    DSSLinkedAssociatedRWNodeSet* mpLinkedAssociatedRWNodeSet;
	
    DSSElementsPropertiesMap* mpElementsPropertiesMap;
    GUID mThemeRef;
    GUID mPaletteRef;
    std::vector<int> mPaletteColors;
public:
	DSSDocumentDefinition();
	DSSDocumentDefinition(GUID& iID, EnumDSSObjectType iType, std::string iName, DSSObjectContext* iObjectContext, GUID iContainerID, EnumDSSViewMedia iViewMedia = DssViewMediaReserved);
	virtual ~DSSDocumentDefinition();
	EnumDSSDocumentType getDocumentType();
	DSSRWNode* getRootNode(EnumDSSRWTreeType iTreeIndex);
    void ClearRootNode(EnumDSSRWTreeType iTreeIndex, std::vector<DSSRWNode*>* ipNodesRmved=NULL, std::vector<DSSRWIterator*>* ipIteratorsRmved=NULL);
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader, bool iIsPartial=false);
	std::string getCurrentLayoutKey();
	void setCurrentLayoutKey(std::string iKey);
	DSSRWSectionNode* getCurrentLayoutNode();
	bool HasLayout();
	std::vector<std::string>* getLayoutKeys();
	void AddLayoutKey(std::string iKey);
	std::string getInitialLayoutKey();
	void setInitialLayoutKey(std::string iKey);
	std::vector<DSSRWNode*>* getNodesLoadedOnDemand();	
	void getNodesForDatasetSelector(DSSRWControl* ipControl,  std::map < std::string, DSSRWNode* >* ipNodeMapServer, std::vector<DSSRWNode*>* ipNodesModelChanged, std::map<std::string, std::vector<std::string> >* ipDDICModelChanged = NULL);
	int LoadLayoutKeysFromBinary(DSSBinaryDataReader* ipReader);
	DSSDocumentViews* getDocumentViews();
	void setDocumentViews(DSSDocumentViews* ipDocumentViews);
	int getCurrentDocumentView();
	void setCurrentDocumentView(int iViewIdx);
    
    void getLinkedAssociatedRWNodeKeys(std::string iNodeKey, std::vector<std::string>& iLinkedNodes, int iCategory, bool ibExcludeSelection = true);
    void insertAssociatedInfo(std::string iNodeKey1, std::string iNodeKey2);
    
    DSSElementsPropertiesMap* getElementsPropertiesMap() { return mpElementsPropertiesMap; }
    const GUID& getThemeRef() const { return mThemeRef; }
    const GUID& getPaletteRef() const { return mPaletteRef; }
    const std::vector<int>& getPaletteColors() const { return mPaletteColors; }
private:
    void makeLinkedAssociatedRWNodeSet(int iCategory, bool ibExcludeSelection = true);
    void makeLinkedAssociatedRWNodeSetByNode(DSSRWNode* ipNode, int iCategory, bool ibExcludeSelection = true);
    bool IsSelection(DSSRWTemplateNode *ipSourceNode, std::string iTargetNodeKey);
};

#endif
