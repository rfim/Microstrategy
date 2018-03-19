//
//  DSSCManipulationAddField.cpp
//  ReportServiceCore
//
//  Created by Li, Dong on 4/25/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//

#include "DSSCManipulationAddField.h"
#include "DSSAODeltaXML.h"
#include "DSSExpNode.h"
#ifdef __ANDROID__
    #include "TinyXml/tinyxml.h"
#else
    #include "DSSSimpleXML.h"
#endif
#include "DSSBaseElementProxy.h"

DSSCManipulationAddField::DSSCManipulationAddField(DSSRWNode* ipNode, std::string iFieldName, EnumDSSRWFieldType iFieldType, std::string iFieldText, EnumDSSObjectType iFieldObjectType, GUID iFieldObjectID, double iFieldLeft, double iFieldTop, double iFieldWidth, double iFieldHeight, std::vector<PropertyInfo*>& iProperties)
{
    mpNode = ipNode;
    mpFieldGroupNode = static_cast<DSSRWFieldGroupNode*>(mpNode);
    DSSObjectContext* lpContext = mpNode->getObjectContext();
    mpField = new DSSField(lpContext);
    mpField->setName(iFieldName);
    mpField->setType(iFieldType);
    switch (iFieldType)
    {
        case DssRWFieldShape:
        case DssRWFieldImage:
        case DssRWFieldLine:
        case DssRWFieldLabel:
        case DssRWFieldConcatenation:
        // AutoText need more parsing code?
        case DssRWFieldAutoText:
            mpField->setText(iFieldText);
        
        case DssRWFieldObjectName:
        case DssRWFieldObjectValue:
            mpField->setFieldObject(iFieldObjectType, iFieldObjectID);
        default:
            break;
    }
    mpField->setLeft(iFieldLeft);
    mpField->setTop(iFieldTop);
    mpField->setWidth(iFieldWidth);
    mpField->setHeight(iFieldHeight);
    
    mProperties = iProperties;
    
    mType = ManipulationAddField;
}

DSSCManipulationAddField::DSSCManipulationAddField(DSSActionObject* ipAO, DSSObjectContext* ipContext)
{
    mType = ManipulationAddField;
    
    if(!ipAO->IsDeltaXML())
        return;
    
    DSSAODeltaXML* lpAO = static_cast<DSSAODeltaXML*>(ipAO);
    std::string lXMLString = lpAO->getXML();
#ifndef __ANDROID__
    hParseAOXML(lXMLString, ipContext);
#endif
}

DSSCManipulationAddField::~DSSCManipulationAddField()
{
    
}

int DSSCManipulationAddField::Execute()
{
    // AddRWNode();
    
    // add the property
    if(mProperties.size())
    {
        std::vector<PropertyInfo*>::iterator lIt = mProperties.begin();
        for(; lIt!=mProperties.end(); lIt++)
        {
            // int lFmtInx = (*lIt)->mFormatIndex;
            std::string lPrptySetName = (*lIt)->mPropertySetName;
            std::string lPrptyName = (*lIt)->mPropertyName;
            CComVariant& lPrptyValue = (*lIt)->mPropertyValue;
            
            DSSFormat* lpFormat = mpField->getFormat();
            if(!lpFormat)
                lpFormat = new DSSFormat(1, &mProperties, mpField->getObjectContext());
            lpFormat->setPropertyValue(lPrptySetName, lPrptyName, lPrptyValue);
        }
    }
    mpFieldGroupNode->getFields()->Add(mpField);

    mNodesModelChanged.push_back(mpFieldGroupNode);
    std::vector<std::string> lNodes = mpFieldGroupNode->GetServerNodeKeys();
    int lSize = lNodes.size();
    for (int i = 0; i < lSize; i++)
    {
        std::string lKey = lNodes[i];
        if (mNodeMapServer.find(lKey) != mNodeMapServer.end())
            continue;
        
        DSSObjectContext* lpContext = mpFieldGroupNode->getObjectContext();
        DSSRWNode* lNode = lpContext->getNode(lKey);
        
        mNodesModelChanged.push_back(lNode);
        mNodeMapServer[lKey] = lNode;
    }
    
    GenerateAODeltaXML();
    return S_OK;
}

#ifndef __ANDROID__
void DSSCManipulationAddField::GenerateAODeltaXML()
{
    SimpleXMLBuilder lBuilder;
    
    SimpleXMLNode lXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
    
    SimpleXMLNode lManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
    SimpleXMLNode lManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("4", true));
    lManipulationMethodXML.AddChild(lManipulationMethodXMLContent);
    
    int lTreeType = static_cast<int>(mpNode->getTreeType());
    SimpleXMLNode lTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
    SimpleXMLNode lTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lTreeType).c_str(), true));
    lTreeTypeXML.AddChild(lTreeTypeXMLContent);
    
    SimpleXMLNode lNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
    SimpleXMLNode lNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpNode->getKey().c_str(), true));
    lNodeKeyXML.AddChild(lNodeKeyXMLContent);
    
    int lNodeType = static_cast<int>(mpNode->getType());
    SimpleXMLNode lNodeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_node_type"));
    SimpleXMLNode lNodeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lNodeType).c_str(), true));
    lNodeTypeXML.AddChild(lNodeTypeXMLContent);
    
    SimpleXMLNode lNodeKeyParentXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key_parent"));
    SimpleXMLNode lNodeKeyParentXMLContent(SimpleXMLBuilder::CreateNodePtr(mpNode->getParentNode()->getKey().c_str(), true));
    lNodeKeyParentXML.AddChild(lNodeKeyParentXMLContent);
    
    lXML.AddChild(lManipulationMethodXML);
    lXML.AddChild(lTreeTypeXML);
    lXML.AddChild(lNodeKeyXML);
    lXML.AddChild(lNodeTypeXML);
    lXML.AddChild(lNodeKeyParentXML);
    
    SimpleXMLNode lXML2(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));

    SimpleXMLNode lManipulationMethodXML2(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
    SimpleXMLNode lManipulationMethodXMLContent2(SimpleXMLBuilder::CreateNodePtr("11", true));
    lManipulationMethodXML2.AddChild(lManipulationMethodXMLContent2);
    
    int lTreeType2 = static_cast<int>(mpNode->getTreeType());
    SimpleXMLNode lTreeTypeXML2(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
    SimpleXMLNode lTreeTypeXMLContent2(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lTreeType2).c_str(), true));
    lTreeTypeXML2.AddChild(lTreeTypeXMLContent2);
    
    SimpleXMLNode lNodeKeyXML2(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
    SimpleXMLNode lNodeKeyXMLContent2(SimpleXMLBuilder::CreateNodePtr(mpNode->getKey().c_str(), true));
    lNodeKeyXML2.AddChild(lNodeKeyXMLContent2);
    
    SimpleXMLNode lFieldKeyXML2(SimpleXMLBuilder::CreateNodePtr("rw_field_key"));
    SimpleXMLNode lFieldKeyXMLContent2(SimpleXMLBuilder::CreateNodePtr(mpField->getKey().c_str(), true));
    lFieldKeyXML2.AddChild(lFieldKeyXMLContent2);
    
    int lFieldType2 = static_cast<int>(mpField->getType());
    SimpleXMLNode lFieldTypeXML2(SimpleXMLBuilder::CreateNodePtr("rw_field_type"));
    SimpleXMLNode lFieldTypeXMLContent2(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lFieldType2).c_str(), true));
    lFieldTypeXML2.AddChild(lFieldTypeXMLContent2);
    
    SimpleXMLNode lFieldNameXML2(SimpleXMLBuilder::CreateNodePtr("rw_field_name"));
    SimpleXMLNode lFieldNameXMLContent2(SimpleXMLBuilder::CreateNodePtr(mpField->getName().c_str(), true));
    lFieldNameXML2.AddChild(lFieldNameXMLContent2);
    
    // field value = field text?
    SimpleXMLNode lFieldValueXML2(SimpleXMLBuilder::CreateNodePtr("rw_field_value"));
    SimpleXMLNode lFieldValueXMLContent2(SimpleXMLBuilder::CreateNodePtr(mpField->getText().c_str(), true));
    lFieldValueXML2.AddChild(lFieldValueXMLContent2);
    
    lXML2.AddChild(lManipulationMethodXML2);
    lXML2.AddChild(lTreeTypeXML2);
    lXML2.AddChild(lNodeKeyXML2);
    lXML2.AddChild(lFieldKeyXML2);
    lXML2.AddChild(lFieldTypeXML2);
    lXML2.AddChild(lFieldNameXML2);
    lXML2.AddChild(lFieldValueXML2);
    
    SimpleXMLNode lXML3(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
    
    SimpleXMLNode lManipulationMethodXML3(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
    SimpleXMLNode lManipulationMethodXMLContent3(SimpleXMLBuilder::CreateNodePtr("17"));
    lManipulationMethodXML3.AddChild(lManipulationMethodXMLContent3);
    
    int lTreeType3 = static_cast<int>(mpNode->getTreeType());
    SimpleXMLNode lTreeTypeXML3(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
    SimpleXMLNode lTreeTypeXMLContent3(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lTreeType3).c_str(), true));
    lTreeTypeXML3.AddChild(lTreeTypeXMLContent3);
    
    SimpleXMLNode lNodeKeyXML3(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
    SimpleXMLNode lNodeKeyXMLContent3(SimpleXMLBuilder::CreateNodePtr(mpNode->getKey().c_str(), true));
    lNodeKeyXML3.AddChild(lNodeKeyXMLContent3);
    
    SimpleXMLNode lFieldKeyXML3(SimpleXMLBuilder::CreateNodePtr("rw_field_key"));
    SimpleXMLNode lFieldKeyXMLContent3(SimpleXMLBuilder::CreateNodePtr(mpField->getKey().c_str(), true));
    lFieldKeyXML3.AddChild(lFieldKeyXMLContent3);
    
    SimpleXMLNode lFormatTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_format_type"));
    SimpleXMLNode lFormatTypeXMLContent(SimpleXMLBuilder::CreateNodePtr("1", true));
    lFormatTypeXML.AddChild(lFormatTypeXMLContent);
    
    DSSFormat* lpFormat = new DSSFormat(1, &mProperties, mpNode->getObjectContext());
    CComVariant lFieldTop(mpField->getTop());
    CComVariant lFieldLeft(mpField->getLeft());
    CComVariant lFieldWidth(mpField->getWidth());
    CComVariant lFieldHeight(mpField->getHeight());
    lpFormat->setPropertyValue("FormattingPosition", "Top", lFieldTop);
    lpFormat->setPropertyValue("FormattingPosition", "Left", lFieldLeft);
    lpFormat->setPropertyValue("FormattingSize", "Width", lFieldWidth);
    lpFormat->setPropertyValue("FormattingSize", "Height", lFieldHeight);
    
    xmlNodePtr lpFormatNode = static_cast<xmlNodePtr> (lpFormat->GenerateXMLNode());
    SimpleXMLNode formatNode(lpFormatNode);
    
    lXML3.AddChild(lManipulationMethodXML3);
    lXML3.AddChild(lTreeTypeXML3);
    lXML3.AddChild(lNodeKeyXML3);
    lXML3.AddChild(lFieldKeyXML3);
    lXML3.AddChild(lFormatTypeXML);
    lXML3.AddChild(formatNode);
    
    SimpleXMLNode lRoot(lBuilder.GetRoot());
    lRoot.AddChild(lXML);
    lRoot.AddChild(lXML2);
    lRoot.AddChild(lXML3);
    
    mpActionObject = new DSSAODeltaXML(ManipulationAddField);
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lBuilder.ToString(false));
}

void DSSCManipulationAddField::hParseAOXML(std::string &iXMLString, DSSObjectContext *ipContext)
{
    SimpleXMLParser lParser("<rw_manipulations>" + iXMLString + "</rw_manipulations>");
	if (!lParser.IsValid())
		return;
	
	SimpleXMLNode lCurrentNode = lParser.GetRoot();
	
    std::string lTemp = lCurrentNode.GetName();
    
	if (!lCurrentNode.IsElementNode() || lCurrentNode.GetName().compare("rw_manipulations") != 0)
		return;
	
	SimpleXMLNode lRWManipulation1;
    SimpleXMLNode lRWManipulation2;
    SimpleXMLNode lRWManipulation3;
    int lIndex = 1;
    
    for (lCurrentNode = lCurrentNode.GetFirstChild(); lCurrentNode.IsValid(); lCurrentNode = lCurrentNode.GetNextSibling())
    {
        if (lCurrentNode.IsElementNode())
        {
            if (lCurrentNode.GetName().compare("rw_manipulation") == 0)
            {
                if (lIndex == 1)
                    lRWManipulation1 = lCurrentNode;
                else if (lIndex == 2)
                    lRWManipulation2 = lCurrentNode;
                else if (lIndex == 3)
                    lRWManipulation3 = lCurrentNode;
                
                lIndex++;
                
            }
        }
    }

    std::string lStrContent;
    
    for(SimpleXMLNode lCurrentNode2 = lRWManipulation2.GetFirstChild(); lCurrentNode2.IsValid(); lCurrentNode2 = lCurrentNode2.GetNextSibling())
    {
        if(lCurrentNode2.IsElementNode())
        {
            if(lCurrentNode2.GetName().compare("rw_node_key") == 0)
            {
                lCurrentNode2.GetContent(lStrContent);
                mpNode = ipContext->getNode(lStrContent);
                mpFieldGroupNode = static_cast<DSSRWFieldGroupNode*>(mpNode);
                mpParentNode = static_cast<DSSRWSectionNode*>(mpNode->getParentNode());
            }
            else if(lCurrentNode2.GetName().compare("rw_field_key") == 0)
            {
                lCurrentNode2.GetContent(lStrContent);
                DSSFields* mpFields = mpFieldGroupNode->getFields();
                mpField = mpFields->FindByKey(lStrContent);
            }
            else if(lCurrentNode2.GetName().compare("rw_field_name") == 0)
            {
                lCurrentNode2.GetContent(lStrContent);
                mpField->setName(lStrContent);
            }
            else if(lCurrentNode2.GetName().compare("rw_field_type") == 0)
            {
                lCurrentNode2.GetContent(lStrContent);
                EnumDSSRWFieldType lType = static_cast<EnumDSSRWFieldType>(DSSExpNode::StringToInt(lStrContent));
                mpField->setType(lType);
            }
            else if(lCurrentNode2.GetName().compare("rw_field_value") == 0)
            {
                lCurrentNode2.GetContent(lStrContent);
                mpField->setText(lStrContent);
            }
        }
    }
    
    SimpleXMLNode lMiNode;
    for(SimpleXMLNode lCurrentNode3 = lRWManipulation3.GetFirstChild(); lCurrentNode3.IsValid(); lCurrentNode3 = lCurrentNode3.GetNextSibling())
    {
        if(lCurrentNode3.IsElementNode())
        {
            if(lCurrentNode3.GetName().compare("rwfmt") == 0)
            {
                lMiNode = lCurrentNode3;
            }
        }
    }
    
    if (!lMiNode.IsValid())
		return;
	
    std::string lstrPropertySetName;
    std::string lstrPropertyName;
    EnumVariantType lValueType;
    CComVariant lValue;
    SimpleXMLNode lSubNode;
    for (lCurrentNode = lMiNode.GetFirstChild(); lCurrentNode.IsValid(); lCurrentNode.GetNextSibling())
    {
        if (lCurrentNode.IsElementNode())
        {
            if (lCurrentNode.GetName().compare("prs") == 0)
            {
                lCurrentNode.GetProperty("n", lstrPropertySetName);
                for (lSubNode=lCurrentNode.GetFirstChild(); lSubNode.IsValid(); lSubNode.GetNextSibling())
                {
                    if (lSubNode.IsElementNode())
                    {
                        if (lSubNode.GetName().compare("pr") == 0)
                        {
                            lSubNode.GetProperty("pri", lStrContent);
                            int lnPri = atoi(lStrContent.c_str());
                            lstrPropertyName = ipContext->getPropertyNameFromID(lstrPropertySetName, lnPri);
                            lValueType = ipContext->getPropertyTypeFromID(lstrPropertySetName, lnPri);
                            lSubNode.GetProperty("v", lStrContent);
                            DSSExpNode::StringToVariant(lStrContent, lValueType, lValue);
                            
                            if(lstrPropertyName.compare("FormattingPosition") == 0)
                            {
                                if(lStrContent.compare("Top") == 0)
                                {
                                    mpField->setTop(lValue.dblVal);
                                }
                                else if(lStrContent.compare("Left") == 0)
                                {
                                    mpField->setLeft(lValue.dblVal);
                                }
                            }
                            else if(lstrPropertyName.compare("FormattingSize") == 0)
                            {
                                if(lStrContent.compare("Width") == 0)
                                {
                                    mpField->setWidth(lValue.dblVal);
                                }
                                else if(lStrContent.compare("Height") == 0)
                                {
                                    mpField->setHeight(lValue.dblVal);
                                }
                            }
                            else
                            {
                                PropertyInfo* lPropertyInfo = new PropertyInfo(1, lstrPropertySetName, lstrPropertyName, lValue);
                                mProperties.push_back(lPropertyInfo);
                            }
                        }
                    }
                }
            }
        }
    }

}
#endif