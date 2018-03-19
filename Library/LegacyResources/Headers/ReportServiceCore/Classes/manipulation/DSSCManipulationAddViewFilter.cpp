//
//  DSSCManipulationAddViewFilter.cpp
//  ReportServiceCore
//
//  Created by Li, Dong on 3/27/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "DSSCManipulationAddViewFilter.h"
#include "DSSAODeltaXML.h"
#include "DSSExpNode.h"
#ifdef __ANDROID__
    #include "TinyXml/tinyxml.h"
#else
    #include "DSSSimpleXML.h"
#endif
#include "DSSExpNodeOperator.h"
#include "DSSExpNodeShortcut.h"
#include "DSSExpNodeConstant.h"
#include "DSSExpNodeElementsObject.h"
#include "DSSConcreteElementProxy.h"

DSSCManipulationAddViewFilter::DSSCManipulationAddViewFilter(DSSRWTemplateNode* ipTemplateNode, DSSCExpression* ipExpression)
{
    mpTemplateNode = ipTemplateNode;
    mpExpression = ipExpression;
    
    mType = ManipulationAddViewFilter;
}

DSSCManipulationAddViewFilter::DSSCManipulationAddViewFilter(DSSActionObject* ipAO, DSSObjectContext* ipContext)
{
    mType = ManipulationAddViewFilter;
    
    if(!ipAO->IsDeltaXML())
        return;
    
    DSSAODeltaXML* lpAO = static_cast<DSSAODeltaXML*>(ipAO);
    std::string lXMLString = lpAO->getXML();
#ifndef __ANDROID__
    hParseAOXML(lXMLString, ipContext);
#endif
}

DSSCManipulationAddViewFilter::~DSSCManipulationAddViewFilter()
{
    
}

int DSSCManipulationAddViewFilter::Execute()
{
    if(mpTemplateNode)
        mpTemplateNode->setViewFilterExpression(mpExpression);
    
    std::vector<std::string> lNodes = mpTemplateNode->GetServerNodeKeys();
    int lSize = lNodes.size();
    for (int i = 0; i < lSize; i++)
    {
        std::string lKey = lNodes[i];
        if (mNodeMapServer.find(lKey) != mNodeMapServer.end())
            continue;
        
        DSSRWNode* lNode = mpTemplateNode->getObjectContext()->getNode(lKey);
        mNodesModelChanged.push_back(lNode);
        mNodeMapServer[lKey] = lNode;
    }

    
    GenerateAODeltaXML();
    return S_OK;
}
#ifndef __ANDROID__
void DSSCManipulationAddViewFilter::GenerateAODeltaXML()
{
    SimpleXMLBuilder lBuilder;
    SimpleXMLNode lXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
    
    SimpleXMLNode lManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
    SimpleXMLNode lManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("49", true));
    lManipulationMethodXML.AddChild(lManipulationMethodXMLContent);
    
    int lTreeType = static_cast<int>(mpTemplateNode->getTreeType());
    SimpleXMLNode lTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
    SimpleXMLNode lTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lTreeType).c_str(), true));
    lTreeTypeXML.AddChild(lTreeTypeXMLContent);
    
    SimpleXMLNode lNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
    SimpleXMLNode lNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpTemplateNode->getKey().c_str(), true));
    lNodeKeyXML.AddChild(lNodeKeyXMLContent);
    
    lXML.AddChild(lManipulationMethodXML);
    lXML.AddChild(lTreeTypeXML);
    lXML.AddChild(lNodeKeyXML);
    
    SimpleXMLNode lFXML(SimpleXMLBuilder::CreateNodePtr("f"));
    mpExpression->getExpressionXML(lFXML.GetNode());
    lXML.AddChild(lFXML);
    
    SimpleXMLNode lRoot(lBuilder.GetRoot());
    lRoot.AddChild(lXML);
    
    mpActionObject = new DSSAODeltaXML(ManipulationAddViewFilter);
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lBuilder.ToString(false));
}

void DSSCManipulationAddViewFilter::hParseAOXML(std::string &iXMLString, DSSObjectContext *ipContext)
{
    SimpleXMLParser lParser(iXMLString);
    if(!lParser.IsValid())
        return;
    SimpleXMLNode lCurrentNode(lParser.GetRoot());
    if(!lCurrentNode.IsElementNode() || lCurrentNode.GetName().compare("rw_manipulation") != 0)
        return;
    
    std::string lStrContent;
    SimpleXMLNode lMiNode;
    for(lCurrentNode = lCurrentNode.GetFirstChild(); lCurrentNode.IsValid(); lCurrentNode = lCurrentNode.GetNextSibling())
    {
        if(lCurrentNode.IsElementNode())
        {
            if(lCurrentNode.GetName().compare("rw_node_key") == 0)
            {
                lCurrentNode.GetContent(lStrContent);
                mpTemplateNode = static_cast<DSSRWTemplateNode*>(ipContext->getNode(lStrContent));
            }
            else if(lCurrentNode.GetName().compare("mi") == 0)
            {
                lMiNode = lCurrentNode;
            }
        }
    }
    
    if (!lMiNode.IsValid())
		return;
	
	// Parse the mi xml to get root node of the expression
	mpExpression = new DSSCExpression(ipContext);
	SimpleXMLNode lRootNode = NULL;
	for (lCurrentNode = lMiNode.GetFirstChild(); lCurrentNode.IsValid(); lCurrentNode = lCurrentNode.GetNextSibling())
	{
		if (lCurrentNode.IsElementNode() && lCurrentNode.GetName().compare("exp") == 0)
		{
			// Found the exp node
			for (lCurrentNode = lCurrentNode.GetFirstChild(); lCurrentNode.IsValid(); lCurrentNode = lCurrentNode.GetNextSibling())
			{
				if (lCurrentNode.IsElementNode() && lCurrentNode.GetName().compare("nd") == 0)
				{
					// Found the root node
					lRootNode = lCurrentNode;
					break;
				}
			}
			break;
		}
	}
	
	if (!lRootNode.IsValid())
		return;
	
	DSSExpNode* lpExpNode = hBuildExpNode(lRootNode, NULL, ipContext);
	mpExpression->setRoot(lpExpNode);
	mpExpression->setExpresssionType(lpExpNode->getExpressionType());
}

DSSExpNode* DSSCManipulationAddViewFilter::hBuildExpNode(SimpleXMLNode& iXMLNode, DSSExpNode* ipParentExpNode, DSSObjectContext* ipContext)
{
	std::string lstrProp;
	iXMLNode.GetProperty("et", lstrProp);
	int et = DSSExpNode::StringToInt(lstrProp);
	iXMLNode.GetProperty("nt", lstrProp);
	int nt = DSSExpNode::StringToInt(lstrProp);
	switch (nt)
	{
		case DssNodeElementsObject:
		{
			DSSExpNodeElementsObject* lpElementsNode;
			if (ipParentExpNode)
				lpElementsNode = static_cast<DSSExpNodeElementsObject*>(ipParentExpNode->Add(DssNodeElementsObject, ""));
			else
				lpElementsNode = new DSSExpNodeElementsObject(ipContext, "", mpExpression);
			
			lpElementsNode->setExpressionType((EnumDSSExpressionType)et);
			
			DSSBaseElementsProxy* lpElements = new DSSBaseElementsProxy(ipContext);
			int type;
			
			for (SimpleXMLNode lChildNode = iXMLNode.GetFirstChild(); lChildNode.IsValid(); lChildNode = lChildNode.GetNextSibling())
			{
				if (lChildNode.IsElementNode() && lChildNode.GetName().compare("mi") == 0)
				{
					// found mi node
					for (lChildNode = lChildNode.GetFirstChild(); lChildNode.IsValid(); lChildNode = lChildNode.GetNextSibling())
					{
						if (lChildNode.IsElementNode() && lChildNode.GetName().compare("in") == 0)
						{
							// found in node
							for (SimpleXMLNode lOiNode = lChildNode.GetFirstChild(); lOiNode.IsValid(); lOiNode = lOiNode.GetNextSibling())
							{
								if (lOiNode.IsElementNode() && lOiNode.GetName().compare("oi") == 0)
								{
									// found oi node
									lOiNode.GetProperty("tp", lstrProp);
									type = DSSExpNode::StringToInt(lstrProp);
									break;
								}
							}
						}								 
						else if (lChildNode.IsElementNode() && lChildNode.GetName().compare("es") == 0)
						{
							// found es node
							for (lChildNode = lChildNode.GetFirstChild(); lChildNode.IsValid(); lChildNode = lChildNode.GetNextSibling())
							{
								if (lChildNode.IsElementNode() && lChildNode.GetName().compare("e") == 0)
								{
									// found e node
									lChildNode.GetProperty("ei", lstrProp);
									DSSBaseElementProxy* lpElement = NULL;
									switch (type) {
										case DssTypeAttribute:
											lpElement = new DSSAttributeElementProxy(ipContext);
											((DSSAttributeElementProxy*)(lpElement))->Parse(lstrProp);
											break;
										default:
											// TODO check other cases
											break;
									}
									
									if (lpElement)
										lpElements->AddElement(lpElement);;
								}
							}
							break;
						}
					}
					break;
				}
			}
			lpElementsNode->setElements(lpElements);
			return lpElementsNode;
		}
			break;
		case DssNodeConstant:
		{
			DSSExpNodeConstant* lpConstantNode;
			if (ipParentExpNode)
				lpConstantNode = static_cast<DSSExpNodeConstant*>(ipParentExpNode->Add(DssNodeConstant, ""));
			else
				lpConstantNode = new DSSExpNodeConstant(ipContext, DssNodeConstant, "", mpExpression);
			
			lpConstantNode->setExpressionType((EnumDSSExpressionType)et);
			
			for (SimpleXMLNode lChildNode = iXMLNode.GetFirstChild(); lChildNode.IsValid(); lChildNode = lChildNode.GetNextSibling())
			{
				if (lChildNode.IsElementNode())
				{
					if (lChildNode.GetName().compare("cst") == 0)
					{
						// constant's data type and value
						lChildNode.GetProperty("ddt", lstrProp);
						int ddt = DSSExpNode::StringToInt(lstrProp);
						lpConstantNode->setDataType(ddt);
						CComVariant value;
						lChildNode.GetContent(lstrProp);
						DSSExpNode::StringToVariant(lstrProp, ddt, value);
						lpConstantNode->setValue(value);
					}
				}
			}
			return lpConstantNode;
		}
			break;
		case DssNodeTime:
		{
			DSSExpNodeConstant* lpTimeNode;
			if (ipParentExpNode)
				lpTimeNode = static_cast<DSSExpNodeConstant*>(ipParentExpNode->Add(DssNodeTime, ""));
			else
				lpTimeNode = new DSSExpNodeConstant(ipContext, DssNodeTime, "", mpExpression);
			lpTimeNode->setExpressionType((EnumDSSExpressionType)et);
			
			for (SimpleXMLNode lChildNode = iXMLNode.GetFirstChild(); lChildNode.IsValid(); lChildNode = lChildNode.GetNextSibling())
			{
				if (lChildNode.IsElementNode())
				{
					if (lChildNode.GetName().compare("cst") == 0)
					{
						// constant's data type and value
						lChildNode.GetProperty("ddt", lstrProp);
						int ddt = DSSExpNode::StringToInt(lstrProp);
						lpTimeNode->setDataType(ddt);
						CComVariant value;
						lChildNode.GetContent(lstrProp);
						DSSExpNode::StringToVariant(lstrProp, ddt, value);
						lpTimeNode->setValue(value);
					}
				}
			}
			return lpTimeNode;
		}
			break;
		case DssNodeOperator:
		{
			DSSExpNodeOperator* lpOperatorNode;
			if (ipParentExpNode)
				lpOperatorNode = static_cast<DSSExpNodeOperator*>(ipParentExpNode->Add(DssNodeOperator, ""));
			else
				lpOperatorNode = new DSSExpNodeOperator(ipContext, "", mpExpression);
			
			lpOperatorNode->setExpressionType((EnumDSSExpressionType)et);
			
			for (SimpleXMLNode lChildNode = iXMLNode.GetFirstChild(); lChildNode.IsValid(); lChildNode = lChildNode.GetNextSibling())
			{
				if (lChildNode.IsElementNode())
				{
					if (lChildNode.GetName().compare("nd") == 0)
					{
						// Found a child exp node
						hBuildExpNode(lChildNode, lpOperatorNode, ipContext);
					}
					else if (lChildNode.GetName().compare("op") == 0)
					{
						// operator's function type
						lChildNode.GetProperty("fnt", lstrProp);
						int fnt = DSSExpNode::StringToInt(lstrProp);
						lpOperatorNode->setOperatorType(fnt);
					}
				}
			}
			return lpOperatorNode;
		}
			break;
		case DssNodeShortcut:
		{
			DSSExpNodeShortcut* lpShortCutNode;
			if (ipParentExpNode)
				lpShortCutNode = static_cast<DSSExpNodeShortcut*>(ipParentExpNode->Add(DssNodeShortcut, ""));
			else
				lpShortCutNode = new DSSExpNodeShortcut(ipContext, "", mpExpression);
			
			lpShortCutNode->setExpressionType((EnumDSSExpressionType)et);
			if (mpControl)
				lpShortCutNode->setTareget(mpControl->getSourceObject());
			
			return lpShortCutNode;
		}
			break;
		default:
		{
			DSSExpNode* lpNode;
			if (ipParentExpNode)
				lpNode = ipParentExpNode->Add(et, "");
			else
				lpNode = new DSSExpNode(ipContext, DssNodeReserved, "", mpExpression);
			lpNode->setExpressionType((EnumDSSExpressionType)et);
			return lpNode;
		}
			break;
	}
	return NULL;
}
#else
void DSSCManipulationAddViewFilter::GenerateAODeltaXML()
{
    TiXmlDocument lDoc;
    TiXmlElement* lpRootElement = new TiXmlElement("rw_manipulation");
    
    TiXmlElement* lpManipulationMethodElement = new TiXmlElement("rw_manipulation_method");
    TiXmlText* lpManipulationMethodText =  new TiXmlText("49");
    lpManipulationMethodElement->LinkEndChild(lpManipulationMethodText);
    
    int lTreeType = static_cast<int>(mpTemplateNode->getTreeType());
    TiXmlElement* lpTreeTypeElement = new TiXmlElement("rw_tree_type");
    TiXmlText* lpTreeTypeText = new TiXmlText(DSSExpNode::IntToString(lTreeType).c_str());
    lpTreeTypeElement->LinkEndChild(lpTreeTypeText);
    
    TiXmlElement* lpNodeKeyElement = new TiXmlElement("rw_node_key");
    TiXmlText* lpNodeKeyText = new TiXmlText(mpTemplateNode->getKey().c_str());
    lpNodeKeyElement->LinkEndChild(lpNodeKeyText);
    
    lpRootElement->LinkEndChild(lpManipulationMethodElement);
    lpRootElement->LinkEndChild(lpTreeTypeElement);
    lpRootElement->LinkEndChild(lpNodeKeyElement);
    mpExpression->getExpressionXML(lpRootElement);
	lDoc.LinkEndChild(lpRootElement); 
    
    TiXmlPrinter lPrinter;
    lPrinter.SetIndent(NULL);
    lPrinter.SetLineBreak(NULL);
    lDoc.Accept(&lPrinter);
    
    mpActionObject = new DSSAODeltaXML();
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lPrinter.Str());
}
#endif