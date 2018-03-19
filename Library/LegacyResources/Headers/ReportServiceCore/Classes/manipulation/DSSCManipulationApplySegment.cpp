/*
 *  DSSCManipulationApplySegment.cpp
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 11-8-17.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "DSSCManipulationApplySegment.h"
#include "DSSAODeltaXML.h"
#include "DSSSimpleXML.h"

DSSCManipulationApplySegment::DSSCManipulationApplySegment(DSSRWNode* ipRootNode, GUID iSegmentID)
{
	mpRootNode = ipRootNode;
	mSegmentID = iSegmentID;
	mIsFromActionObject = false;
    
    mType = ManipulationApplySegment;
}

//DSSCManipulationApplySegment(DSSActionObject* ipAO, DSSObjectContext* ipContext);

DSSCManipulationApplySegment::~DSSCManipulationApplySegment()
{}

int DSSCManipulationApplySegment::Execute()
{
	std::vector<DSSRWNode*> lNodesModelChanged;
	
	hCheckControlNode(mpRootNode, &lNodesModelChanged);
	
	// put all control nodes to the end of the vector
	
	std::vector<DSSRWNode*> lNodesModelChangedControl;
	std::vector<DSSRWNode*>::iterator it1;
	for ( it1=lNodesModelChanged.begin() ; it1 != lNodesModelChanged.end(); it1++ )
	{
		if ((*it1)->getType()!=DssRWNodeControl)
			mNodesModelChanged.push_back(*it1);
		else
			lNodesModelChangedControl.push_back(*it1);
	}
	
	std::vector<DSSRWNode*>::iterator it2 =lNodesModelChangedControl.begin();
	std::vector<DSSRWNode*>::iterator it3 =lNodesModelChangedControl.end();
	for (; it2 != it3; it2++ )
	{
		mNodesModelChanged.push_back(*it2);
	}	
	
	mpActionObject = new DSSAODeltaXML(ManipulationApplySegment);
	std::string lXML = hBuildExpressionXML();	
	(static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lXML);
	
	return S_OK;
}

void DSSCManipulationApplySegment::hCheckControlNode(DSSRWNode* ipNode, std::vector<DSSRWNode*>* ioNodesModelChanged)
{
	if (ipNode->getType()==DssRWNodeControl)
	{
		DSSRWControls* lpControls = ((DSSRWControlNode*)ipNode)->getControls();
		if (lpControls && lpControls->Count()>0)
		{

			std::map < std::string, DSSRWNode* >::iterator iter = mNodeMapServerDef.find(ipNode->getKey());
			if (iter==mNodeMapServerDef.end())
				mNodeMapServerDef[ipNode->getKey()] = ipNode;
			
			std::vector<DSSRWNode*>::iterator lModelChangedNodesVector = std::find(ioNodesModelChanged->begin(), ioNodesModelChanged->end(), ipNode);
			if (lModelChangedNodesVector==ioNodesModelChanged->end())
				ioNodesModelChanged->push_back(ipNode);
		
			for (int j=0; j<lpControls->Count(); j++)
			{
				DSSRWControl* lpControl = lpControls->Item(j);
				
				int lTargetCount = lpControl->getCountTargets();
				for (int i=0; i<lTargetCount; i++)
				{
					DSSRWNode* lpTargetNode = lpControl->getTargetNode(i);
					hCheckTargetNode(lpTargetNode, ioNodesModelChanged, true);
				}	
			}	
		}
	}
	else if (ipNode->getType()==DssRWNodeSection)
	{
		for (int i=0; i<ipNode->Count(); i++)
			hCheckControlNode(ipNode->Item(i), ioNodesModelChanged);
	}
}

void DSSCManipulationApplySegment::hCheckTargetNode(DSSRWNode* ipNode, std::vector<DSSRWNode*>* ioNodesModelChanged, bool isTargetNode)
{
	if (isTargetNode)
	{
		std::map < std::string, DSSRWNode* >::iterator iter = mNodeMapServer.find(ipNode->getKey());
		if (iter==mNodeMapServer.end())
			mNodeMapServer[ipNode->getKey()] = ipNode;
		
		iter = mNodeMapServerDef.find(ipNode->getKey());
		if (iter==mNodeMapServerDef.end())
			mNodeMapServerDef[ipNode->getKey()] = ipNode;
	}
	
	std::vector<DSSRWNode*>::iterator lModelChangedNodesVector = std::find(ioNodesModelChanged->begin(), ioNodesModelChanged->end(), ipNode);
	if (lModelChangedNodesVector==ioNodesModelChanged->end())
		ioNodesModelChanged->push_back(ipNode);
	
	for (int i=0; i<ipNode->Count(); i++)
		hCheckTargetNode(ipNode->Item(i), ioNodesModelChanged, false);
}

NodeMap* DSSCManipulationApplySegment::getNodeMapServerDef()
{
	return &mNodeMapServerDef;
}

std::string DSSCManipulationApplySegment::hBuildExpressionXML()
{
	SimpleXMLBuilder lBuilder;
	SimpleXMLNode lXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
	
	SimpleXMLNode lManipulationXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
	SimpleXMLNode lManipulationXMLContent(SimpleXMLBuilder::CreateNodePtr("123", true));
	lManipulationXML.AddChild(lManipulationXMLContent);
	lXML.AddChild(lManipulationXML);
	
	SimpleXMLNode lTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
	SimpleXMLNode lTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr("1", true));
	lTreeTypeXML.AddChild(lTreeTypeXMLContent);
	lXML.AddChild(lTreeTypeXML);
	
	std::string lStr = mpRootNode->getKey();
	SimpleXMLNode lNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
	SimpleXMLNode lNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(lStr.c_str(), true));
	lNodeKeyXML.AddChild(lNodeKeyXMLContent);
	lXML.AddChild(lNodeKeyXML);
	
	char lIDString[33];
	DSSBaseElementProxy::ConvertGUIDToString(mSegmentID, lIDString);
	lStr = lIDString;
	SimpleXMLNode lControlKeyXML(SimpleXMLBuilder::CreateNodePtr("object_id"));
    SimpleXMLNode lControlKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(lStr.c_str(), true));                         
	lControlKeyXML.AddChild(lControlKeyXMLContent);
	lXML.AddChild(lControlKeyXML);
	
	SimpleXMLNode lRoot(lBuilder.GetRoot());
	lRoot.AddChild(lXML);
	
	return lBuilder.ToString(false);
}
