//
//  DSSCManipulationValidateDerivedMetric.cpp
//  ReportServiceCore
//
//  Created by Li, Dong on 3/30/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//

#include "DSSCManipulationValidateDerivedMetric.h"
#include "DSSAODeltaXML.h"
#include "DSSExpNode.h"
#ifdef __ANDROID__
    #include "TinyXml/tinyxml.h"
#else
    #include "DSSSimpleXML.h"
#endif
#include "DSSBaseElementProxy.h"

DSSCManipulationValidateDerivedMetric::DSSCManipulationValidateDerivedMetric(std::string iFormula, DSSDataElement* ipElement)
{
    mFormula = iFormula;
    mpElement = ipElement;
    
    mType = ManipulationValidateDerivedMetric;
}

DSSCManipulationValidateDerivedMetric::DSSCManipulationValidateDerivedMetric(DSSActionObject* ipAO, DSSObjectContext* ipContext)
{
    mType = ManipulationValidateDerivedMetric;
    
    if(!ipAO->IsDeltaXML())
        return;
    
    DSSAODeltaXML* lpAO = static_cast<DSSAODeltaXML*>(ipAO);
    std::string lXMLString = lpAO->getXML();
#ifndef __ANDROID__
    hParseAOXML(lXMLString, ipContext);
#endif
}

DSSCManipulationValidateDerivedMetric::~DSSCManipulationValidateDerivedMetric()
{
    
}

int DSSCManipulationValidateDerivedMetric::Execute()
{
    GenerateAODeltaXML();
    return S_OK;
}

#ifndef __ANDROID__
void DSSCManipulationValidateDerivedMetric::GenerateAODeltaXML()
{
    SimpleXMLBuilder lBuilder;
    SimpleXMLNode lXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
    
    SimpleXMLNode lManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
    SimpleXMLNode lManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("133", true));
    lManipulationMethodXML.AddChild(lManipulationMethodXMLContent);
    
    char lDataSetID[33];
    DSSBaseElementProxy::ConvertGUIDToString(mpElement->getDatasetID(), lDataSetID);
    SimpleXMLNode lDataSetIDXML(SimpleXMLBuilder::CreateNodePtr("rw_data_set_id"));
    SimpleXMLNode lDataSetIDXMLContent(SimpleXMLBuilder::CreateNodePtr(lDataSetID, true));
    lDataSetIDXML.AddChild(lDataSetIDXMLContent);
    
    SimpleXMLNode lDataSetTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_data_set_type"));
    SimpleXMLNode lDataSetTypeXMLContent(SimpleXMLBuilder::CreateNodePtr("3", true));
    lDataSetTypeXML.AddChild(lDataSetTypeXMLContent);
    
    SimpleXMLNode lFormulaXML(SimpleXMLBuilder::CreateNodePtr("derived_metric_formula"));
    SimpleXMLNode lFormulaXMLContent(SimpleXMLBuilder::CreateNodePtr(mFormula.c_str(), true));
    lFormulaXML.AddChild(lFormulaXMLContent);
    
    lXML.AddChild(lManipulationMethodXML);
    lXML.AddChild(lDataSetIDXML);
    lXML.AddChild(lDataSetTypeXML);
    lXML.AddChild(lFormulaXML);

    SimpleXMLNode lRoot(lBuilder.GetRoot());
    lRoot.AddChild(lXML);
    
    mpActionObject = new DSSAODeltaXML(ManipulationValidateDerivedMetric);
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lBuilder.ToString(false));
}

void DSSCManipulationValidateDerivedMetric::hParseAOXML(std::string &iXMLString, DSSObjectContext *ipContext)
{
    SimpleXMLParser lParser(iXMLString);
    if(!lParser.IsValid())
        return;
    SimpleXMLNode lCurrentNode(lParser.GetRoot());
    if(!lCurrentNode.IsElementNode() || lCurrentNode.GetName().compare("rw_manipulation") != 0)
        return;
    
    std::string lStrContent;
    for(lCurrentNode = lCurrentNode.GetFirstChild(); lCurrentNode.IsValid(); lCurrentNode = lCurrentNode.GetNextSibling())
    {
        if(lCurrentNode.IsElementNode())
        {
            if(lCurrentNode.GetName().compare("rw_data_set_id") == 0)
            {
                lCurrentNode.GetContent(lStrContent);
                GUID lDataSetID;
                DSSBaseElementProxy::ConvertFromStringToGUID(lStrContent, lDataSetID);
                if(!mpElement)
                    mpElement = new DSSDataElement(lDataSetID, ipContext);
            }
            else if(lCurrentNode.GetName().compare("derived_metric_formula") == 0)
            {
                lCurrentNode.GetContent(lStrContent);
                mFormula = lStrContent;
            }
        }
    }
}
#endif