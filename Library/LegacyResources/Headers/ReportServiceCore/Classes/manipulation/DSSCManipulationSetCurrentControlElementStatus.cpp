//
//  DSSCManipulationSetCurrentControlElementStatus.cpp
//  ReportServiceCore
//
//  Created by Zhu, Wei on 12/21/12.
//
//

#include "DSSCManipulationSetCurrentControlElementStatus.h"
#include "DSSCExpression.h"
#include "DSSRWControl.h"
#include "DSSAODeltaXML.h"
#include "DSSExpNode.h"

#ifdef __ANDROID__
#include "TinyXml/tinyxm.h"
#else
#include "DSSSimpleXML.h"
#endif


DSSCManipulationSetCurrentControlElementStatus::DSSCManipulationSetCurrentControlElementStatus(DSSRWTemplateNode* iNode, std::string& iControlKey, EnumDSSCurrentElementStatus iStatus)
{
	mpTemplateNode = iNode;
    mControlKey = iControlKey;
	mpControl = NULL;
    mStatus = iStatus;
    
	mIsFromActionObject = false;
}

// replay manipulations
DSSCManipulationSetCurrentControlElementStatus::DSSCManipulationSetCurrentControlElementStatus(DSSActionObject* ipAO, DSSObjectContext* ipContext)
{
    if(!ipAO->IsDeltaXML())
        return;
    
    DSSAODeltaXML* lpAO = static_cast<DSSAODeltaXML*>(ipAO);
    std::string lXMLString = lpAO->getXML();
#ifndef __ANDROID__
    hParseAOXML(lXMLString, ipContext);
#endif
    
	mIsFromActionObject = true;
}

DSSCManipulationSetCurrentControlElementStatus::~DSSCManipulationSetCurrentControlElementStatus()
{
    
}

int DSSCManipulationSetCurrentControlElementStatus::Execute()
{
    if (!mpTemplateNode)
        return S_FALSE;
    
    DSSRWControls* lpControls =mpTemplateNode->getControls();
    int lSize = lpControls->Count();
    
    for (int i = 0; i < lSize; i++)
    {
        DSSRWControl* lpControl = lpControls->Item(i);
        
        if (lpControl->getKey() == mControlKey)
        {
            mpControl = lpControl;
            break;
        }
    }
    
    if (!mpControl)
        return S_FALSE;
    
    if (mStatus == DssCurrentElementUnset)
    {
        mpControl->ClearCurrentElements();
        
        DSSCExpression* lpExpression = mpControl->getExpression();
        if (lpExpression)
            lpExpression->Clear();
    }

    mpControl->setCurrentElementsStatus(mStatus);
    
    std::vector<DSSRWNode*> lNodesModelChanged;
    DSSObjectContext* lpObjectContext = mpTemplateNode->getObjectContext();
    
    EnumDSSRWControlType lType = mpControl->getType();
	int lTargetCount;
	switch (lType)
	{
		case DssRWControlTypeElementList:
		case DssRWControlTypeMetricList:
		{
			lTargetCount = mpControl->getCountTargets();
			// TQMS 529238: sort the targets in the hierachical order in content tree
            std::vector<DSSRWNode*> lTargets;
            for(int i = 0; i < lTargetCount; i++)
            {
                std::string lTargetKey = mpControl->getTargetKey(i);
                DSSRWNode* lpTargetNode = lpObjectContext->getNode(lTargetKey);
                lTargets.push_back(lpTargetNode);
            }
            SortControlTarget(lTargets);
            
			for (int i=0; i<lTargetCount; i++)
			{
				// std::string lTargetKey = mpControl->getTargetKey(i);
				// DSSRWNode* lpTargetNode = lpObjectContext->getNode(lTargetKey);
				DSSRWNode* lpTargetNode = lTargets[i];
				if (!lpTargetNode || lpTargetNode->getType()!=DssRWNodeSection)
					continue;
				DSSRWSectionNode* lpSectionNode = (DSSRWSectionNode*)(lpTargetNode);
				
				if (lType == DssRWControlTypeElementList)
					lpSectionNode->DiscardTransaction();
				lpSectionNode->setLatestControlKey(mpControl->getKey());
				
				if (lpSectionNode->getSectionType()!=DssRWSectionControlGroupBySection && lpSectionNode->getSectionType()!=DssRWSectionUnitCondition)
					continue;
				DSSGroupByUnits* lpUnits = lpSectionNode->getGroupByUnits();
				if (!lpUnits || lpUnits->Count()==0)
					continue;
				DSSGroupByUnit* lpUnit = lpUnits->Item(0);
				if (!lpUnit)
					continue;
				
                if (mStatus == DssCurrentElementUnset)
                {
                    lpUnit->ClearCurrentElements();
                    
                    if (lpUnit->getExpression())
                        lpUnit->getExpression()->Clear();
                }
                
                lpUnit->setCurrentElementsStatus(mStatus);
                
				// Add the UC node's highest CGB node into the Server node map (the root nodes needing go back to server to get partial data retrieval)
				bool isUC = false;
				if (lpSectionNode->getSectionType()==DssRWSectionUnitCondition)
				{
					isUC = true;
					
					// get highest CGB node if UC
					DSSRWNode* lHighestCGB = lpSectionNode->getHighestCGB();
					if (lHighestCGB)
						lpTargetNode = lHighestCGB;
					
					mNodeMapServer[lpTargetNode->getKey()] = lpTargetNode;
					
				}
				lpTargetNode->AddNodeMap(lNodesModelChanged, mNodeMapServer, mNodeMapSelectionUpdate, mpTemplateNode->getKey(), isUC, lpTargetNode->getKey());
			}
			
			break;
		}
		default:
			break;
	}
	
    
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
	
    if (mIsFromActionObject)
        mNodesModelChanged.push_back(mpTemplateNode);
    
	std::vector<DSSRWNode*>::iterator it2 =lNodesModelChangedControl.begin();
	std::vector<DSSRWNode*>::iterator it3 =lNodesModelChangedControl.end();
	for (; it2 != it3; it2++ )
	{
		mNodesModelChanged.push_back(*it2);
	}
    
	if (!mIsFromActionObject)
		GenerateAODeltaXML();
    
	return S_OK;
}

/*
 <rw_manipulation><rw_manipulation_method>82</rw_manipulation_method><rw_tree_type>1</rw_tree_type><rw_node_key>F48</rw_node_key><rw_control_key>W48</rw_control_key><element_status>1</element_status></rw_manipulation>
 */
#ifndef __ANDROID__
void DSSCManipulationSetCurrentControlElementStatus::GenerateAODeltaXML()
{
    mpActionObject = new DSSAODeltaXML(ManipulationSetCurrentControlElementStatus);
    
    // tree type string
    SimpleXMLBuilder lBuilder;
    
    SimpleXMLNode lXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
    
	SimpleXMLNode lManipulationXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
	SimpleXMLNode lManipulationXMLContent(SimpleXMLBuilder::CreateNodePtr("82", true));
	lManipulationXML.AddChild(lManipulationXMLContent);
	lXML.AddChild(lManipulationXML);
    
    EnumDSSRWTreeType lTreeType = mpTemplateNode->getTreeType();
	SimpleXMLNode lTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
	SimpleXMLNode lTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lTreeType).c_str(), true));
	lTreeTypeXML.AddChild(lTreeTypeXMLContent);
	lXML.AddChild(lTreeTypeXML);
	
	std::string lStr = mpTemplateNode->getKey();
	SimpleXMLNode lNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
	SimpleXMLNode lNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(lStr.c_str(), true));
	lNodeKeyXML.AddChild(lNodeKeyXMLContent);
	lXML.AddChild(lNodeKeyXML);
    
	SimpleXMLNode lControlXML(SimpleXMLBuilder::CreateNodePtr("rw_control_key"));
	SimpleXMLNode lControlXMLContent(SimpleXMLBuilder::CreateNodePtr(mControlKey.c_str(), true));
	lControlXML.AddChild(lControlXMLContent);
	lXML.AddChild(lControlXML);
	
	SimpleXMLNode lStatus(SimpleXMLBuilder::CreateNodePtr("element_status"));
	SimpleXMLNode lStatusXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString((int)mStatus).c_str(), true));
	lStatus.AddChild(lStatusXMLContent);
	lXML.AddChild(lStatus);
    
    
    SimpleXMLNode lRoot(lBuilder.GetRoot());
	lRoot.AddChild(lXML);
    
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lBuilder.ToString(false));
}

void DSSCManipulationSetCurrentControlElementStatus::hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext)
{
	SimpleXMLParser lParser(iXMLString);
	if (!lParser.IsValid())
		return;
	
	SimpleXMLNode lCurrentNode = lParser.GetRoot();
	
	if (!lCurrentNode.IsElementNode() || lCurrentNode.GetName().compare("rw_manipulation") != 0)
		return;
	
	std::string lstrContent;
	for (lCurrentNode = lCurrentNode.GetFirstChild(); lCurrentNode.IsValid(); lCurrentNode = lCurrentNode.GetNextSibling())
	{
		if (lCurrentNode.IsElementNode())
		{
			if (lCurrentNode.GetName().compare("rw_node_key") == 0) // template node
			{
				lCurrentNode.GetContent(lstrContent);
                mpTemplateNode = (DSSRWTemplateNode*)(ipContext->getNode(lstrContent));
			}
			else if (lCurrentNode.GetName().compare("rw_control_key") == 0)
			{
                lCurrentNode.GetContent(lstrContent);
                mControlKey = lstrContent;
			}
            else if (lCurrentNode.GetName().compare("element_status") == 0)
            {
                lCurrentNode.GetContent(lstrContent);
                mStatus = (EnumDSSCurrentElementStatus) DSSExpNode::StringToInt(lstrContent);
            }
		}
	}
}

#else
void DSSCManipulationSetCurrentControlElementStatus::GenerateAODeltaXML()
{
    DSSObjectContext* lObjectContext = mpTemplateNode->getObjectContext();
    
   	TiXmlDocument lDoc;
	TiXmlElement* lpRootElement = new TiXmlElement("rw_manipulation");
    
	TiXmlElement* lpManipulationMethodElement = new TiXmlElement("rw_manipulation_method");
	TiXmlText* lpManipulationMethodText = new TiXmlText("82");
	lpManipulationMethodElement->LinkEndChild(lpManipulationMethodText);
	lpRootElement->LinkEndChild(lpManipulationMethodElement);
    
    EnumDSSRWTreeType lTreeType = mpTemplateNode->getTreeType();
	TiXmlElement* lpTreeTypeElement = new TiXmlElement("rw_tree_type");
	TiXmlText* lpTreeTypeText = new TiXmlText(DSSExpNode::IntToString(lTreeType).c_str());
	lpTreeTypeElement->LinkEndChild(lpTreeTypeText);
	lpRootElement->LinkEndChild(lpTreeTypeElement);
    
	std::string lStr = mpTemplateNode->getKey();
	TiXmlElement* lpNodeKeyElement = new TiXmlElement("rw_node_key");
	TiXmlText* lpNodeKeyText = new TiXmlText(lStr);
	lpNodeKeyElement->LinkEndChild(lpNodeKeyText);
	lpRootElement->LinkEndChild(lpNodeKeyElement);
    
    TiXmlElement* lpControlKeyElement = new TiXmlElement("rw_control_key");
	TiXmlText* lpControlKeyText = new TiXmlText(mControlKey);
	lpControlKeyElement->LinkEndChild(lpControlKeyText);
	lpRootElement->LinkEndChild(lpControlKeyElement)
    
    std::string lStr = DSSExpNode::IntToString((int)mStatus);
	TiXmlElement* lpStatusElement = new TiXmlElement("element_status");
	TiXmlText* lpStatusElementText = new TiXmlText(lStr);
	lpStatusElement->LinkEndChild(lpStatusElementText);
	lpRootElement->LinkEndChild(lpStatusElement);
                                                      
    TiXmlPrinter printer;
    printer.SetIndent(NULL);
    printer.SetLineBreak(NULL);
    lDoc.LinkEndChild(lpRootElement);
    lDoc.Accept( &printer );
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(printer.Str());
}
#endif


