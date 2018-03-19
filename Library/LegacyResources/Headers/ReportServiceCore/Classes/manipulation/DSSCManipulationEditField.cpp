//
//  DSSCManipulationEditField.cpp
//  ReportServiceCore
//
//  Created by Zhu, Wei on 12/26/12.
//
//

#include "DSSCManipulationEditField.h"

#include "DSSAODeltaXML.h"
#include "DSSExpNode.h"
#ifdef __ANDROID__
#include "TinyXml/tinyxml.h"
#else
#include "DSSSimpleXML.h"
#endif
#include "DSSBaseElementProxy.h"

DSSCManipulationEditField::DSSCManipulationEditField(DSSRWNode* ipNode, std::string iFieldName, EnumDSSRWFieldType iFieldType, std::string& iFieldKey, std::string& iText)
{
    mpNode = ipNode;
    EnumDSSRWNodeType lNodeType = mpNode->getType();
    if (lNodeType != DssRWNodeFieldGroup)
        _ASSERT(false);
    
    mpFields = static_cast<DSSRWFieldGroupNode*>(mpNode)->getFields();
    mFieldKey = iFieldKey;
    mpField = mpFields->FindByKey(iFieldKey);
    mFieldType = iFieldType;
    mFieldName = iFieldName;
    mText = iText;
    
    mIsFromActionObject = false;
    
    mType = ManipulationEditField;
}

DSSCManipulationEditField::DSSCManipulationEditField(DSSActionObject* ipAO, DSSObjectContext* ipContext)
{
    mType = ManipulationEditField;
    
    if(!ipAO->IsDeltaXML())
        return;
    
    DSSAODeltaXML* lpAO = static_cast<DSSAODeltaXML*>(ipAO);
    std::string lXMLString = lpAO->getXML();
#ifndef __ANDROID__
    hParseAOXML(lXMLString, ipContext);
#endif
    
    mIsFromActionObject = true;
}

DSSCManipulationEditField::~DSSCManipulationEditField()
{
    
}

bool DSSCManipulationEditField::EditField()
{
    EnumDSSRWFieldType lFieldType = mpField->getType();
    bool lChanged = false;
    
    if (lFieldType != mFieldType)
    {
        lChanged = true;
        mpField->setType(mFieldType);
    }
    std::string lFieldName = mpField->getName();
    if (lFieldName != mFieldName)
    {
        lChanged = true;
        mpField->setName(mFieldName);
    }
    std::string lText = mpField->getText();
    if (lText != mText)
    {
        lChanged = true;
        mpField->setText(mText);
    }
    
    return S_OK;
    // TODO - do we need to support other field types beside EnumDSSRWFieldType.DssRWFieldLabel?
}

int DSSCManipulationEditField::Execute()
{
    if (!mpFields || !mpField)
        return S_FALSE;
    
    bool lChanged = EditField();
    
    if (lChanged)
        mNodesModelChanged.push_back(mpNode);
    
    if (!mIsFromActionObject)
        GenerateAODeltaXML();
    return S_OK;
}

#ifndef __ANDROID__
/*
 <rw_manipulation>
 <rw_manipulation_method>13</rw_manipulation_method>
 <rw_tree_type>1</rw_tree_type>
 <rw_node_key>F169</rw_node_key>
 <rw_field_key>F170</rw_field_key>
 <rw_field_type>6</rw_field_type>
 <rw_field_name>VisualizationPanel</rw_field_name>
 <rw_field_value>test</rw_field_value>
 </rw_manipulation>
 */
void DSSCManipulationEditField::GenerateAODeltaXML()
{
    SimpleXMLBuilder lBuilder;
    
    SimpleXMLNode lXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
    
    SimpleXMLNode lManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
    SimpleXMLNode lManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("13", true));
    lManipulationMethodXML.AddChild(lManipulationMethodXMLContent);
    
    int lTreeType = static_cast<int>(mpNode->getTreeType());
    SimpleXMLNode lTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
    SimpleXMLNode lTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lTreeType).c_str(), true));
    lTreeTypeXML.AddChild(lTreeTypeXMLContent);
    
    SimpleXMLNode lNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
    SimpleXMLNode lNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpNode->getKey().c_str(), true));
    lNodeKeyXML.AddChild(lNodeKeyXMLContent);
    
    SimpleXMLNode lFieldKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_field_key"));
    SimpleXMLNode lFieldKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpField->getKey().c_str(), true));
    lFieldKeyXML.AddChild(lFieldKeyXMLContent);
    
    int lFieldType2 = static_cast<int>(mpField->getType());
    SimpleXMLNode lFieldTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_field_type"));
    SimpleXMLNode lFieldTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lFieldType2).c_str(), true));
    lFieldTypeXML.AddChild(lFieldTypeXMLContent);
    
    SimpleXMLNode lFieldNameXML(SimpleXMLBuilder::CreateNodePtr("rw_field_name"));
    SimpleXMLNode lFieldNameXMLContent(SimpleXMLBuilder::CreateNodePtr(mpField->getName().c_str(), true));
    lFieldNameXML.AddChild(lFieldNameXMLContent);
    
    // field value = field text?
    SimpleXMLNode lFieldValueXML(SimpleXMLBuilder::CreateNodePtr("rw_field_value"));
    SimpleXMLNode lFieldValueXMLContent(SimpleXMLBuilder::CreateNodePtr(mpField->getText().c_str(), true));
    lFieldValueXML.AddChild(lFieldValueXMLContent);
    
    lXML.AddChild(lManipulationMethodXML);
    lXML.AddChild(lTreeTypeXML);
    lXML.AddChild(lNodeKeyXML);
    lXML.AddChild(lFieldKeyXML);
    lXML.AddChild(lFieldTypeXML);
    lXML.AddChild(lFieldNameXML);
    lXML.AddChild(lFieldValueXML);
    

    SimpleXMLNode lRoot(lBuilder.GetRoot());
    lRoot.AddChild(lXML);
    
    std::cout << lBuilder.ToString(false);
    mpActionObject = new DSSAODeltaXML(ManipulationEditField);
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lBuilder.ToString(false));
}

void DSSCManipulationEditField::hParseAOXML(std::string &iXMLString, DSSObjectContext *ipContext)
{
    SimpleXMLParser lParser(iXMLString);
	if (!lParser.IsValid())
		return;
	
	SimpleXMLNode lCurrentNode = lParser.GetRoot();
	
    std::string lTemp = lCurrentNode.GetName();
    
	if (!lCurrentNode.IsElementNode() || lCurrentNode.GetName().compare("rw_manipulation") != 0)
		return;
	    
    std::string lStrContent;
    
    for(SimpleXMLNode lCurrentNode2 = lCurrentNode.GetFirstChild(); lCurrentNode2.IsValid(); lCurrentNode2 = lCurrentNode2.GetNextSibling())
    {
        if(lCurrentNode2.IsElementNode())
        {
            if(lCurrentNode2.GetName().compare("rw_node_key") == 0)
            {
                lCurrentNode2.GetContent(lStrContent);
                mpNode = ipContext->getNode(lStrContent);
                //mpFieldGroupNode = static_cast<DSSRWFieldGroupNode*>(mpNode);
                mpParentNode = static_cast<DSSRWSectionNode*>(mpNode->getParentNode());
            }
            else if(lCurrentNode2.GetName().compare("rw_field_key") == 0)
            {
                lCurrentNode2.GetContent(lStrContent);
                mFieldKey = lStrContent;
                DSSFields* mpFields = static_cast<DSSRWFieldGroupNode*>(mpNode)->getFields();
                mpField = mpFields->FindByKey(lStrContent);
            }
            else if(lCurrentNode2.GetName().compare("rw_field_name") == 0)
            {
                lCurrentNode2.GetContent(lStrContent);
                mFieldName = lStrContent;
            }
            else if(lCurrentNode2.GetName().compare("rw_field_type") == 0)
            {
                lCurrentNode2.GetContent(lStrContent);
                EnumDSSRWFieldType lType = static_cast<EnumDSSRWFieldType>(DSSExpNode::StringToInt(lStrContent));
                mFieldType = lType;
            }
            else if(lCurrentNode2.GetName().compare("rw_field_value") == 0)
            {
                lCurrentNode2.GetContent(lStrContent);
                mText = lStrContent;
            }
        }
    }
}
#endif