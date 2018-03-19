/*
 *  DSSRWDEngine.h
 *  DashboardViewer
 *
 *  Created by Xiaolong Wang on 12/29/09.
 *  Copyright 2010 MicroStrategy, Inc.. All rights reserved.
 *
 */

#import <vector>
#import <string>

//#include "MsiCOM/MsiCOM/Msi_atlcom.h"
//#include "DSSCOMMaster/DSSCOMMaster_i.h"

#import "DSSrwdconstants.h"

class DSSRWIterator;
class DSSDocumentInstance;
class DSSRWNode;

class DSSRWDEngine
{
public:
	DSSRWDEngine();
	~DSSRWDEngine();
	
	int Init (DSSDocumentInstance *ipDocument, int numLayout = 1);
	
	int CreateIterator(int layout, EnumDSSRWTreeType iTreeType, DSSRWIterator **oppIterator);
    
    //junwang,new rwd, 11/15/2011
    int CreateIteratorNewRWD(int layout, EnumDSSRWTreeType iTreeType, DSSRWIterator **oppIterator);
	
	/*!
	 @abstract	traverse document tree to refresh all leaf iterators (grid, control, field group)
	 @discussion We provide this API to support incremental binary data loading feature. When the initial layout binary is loaded
				 to replace current slice binary, viewer has not re-initialized iterators.
	 */
	int ReloadDataModels(std::vector<DSSRWNode*>* ipNodesLoadedOnDemand);
	
	/*!
	 @abstract		find the iterator given the node key and NGB group-by path
	 @discussion	1. irvNGBPath is the NGB group-by path collected by DSSRWIterator::getNGBPath()
					2. if the NGB path is given, only research the first NGB section
	 */
	int FindIterator(std::string &irNodeKey, std::vector<int> &irvNGBPath, DSSRWIterator **oppChildIterator);
	
    
	/*!
	 @abstract		add or remove an interator from the iterator tree
	 @discussion	1. ipNode is the one to be added or removed.
                    2. right now, we only support remove or add a CGB/UC section node, which only has one child
                    3. we don't support adding a node directly under the NGB section node (DssRWSectionGroupBySection)
	 */
    int UpdateIteratorRemoveNode(DSSRWNode* ipNode, std::vector<DSSRWIterator*>* ipIteratorsRmved=NULL);
    int UpdateIteratorAddNode(DSSRWNode* ipNode);
    
private:

	int hTraverseItertors(DSSRWIterator *ipIterator, std::vector<DSSRWNode*>* ipNodesLoadedOnDemand);
    DSSRWIterator* hFindParentNodeIterator(DSSRWNode* ipNode);
	DSSDocumentInstance *mpDocumentInstance;	//week reference
	
	int mNumLayouts;
	std::vector<DSSRWIterator *> mvContent;
	std::vector<DSSRWIterator*> mvHeader;
	std::vector<DSSRWIterator*> mvFooter;
	
};
