//
//  DSSCManipulationSetGraphDefaultProperties.cpp
//  ReportServiceCore
//
//  Created by Li, Dong on 5/22/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//

#include "DSSCManipulationSetGraphDefaultProperties.h"
#include "DSSAODeltaXML.h"
#include "DSSExpNode.h"
#ifdef __ANDROID__
    #include "TinyXml/tinyxml.h"
#else
    #include "DSSSimpleXML.h"
#endif

DSSCManipulationSetGraphDefaultProperties::DSSCManipulationSetGraphDefaultProperties(DSSRWTemplateNode* ipTemplateNode, EnumGraphMajorType iGraphType, int iGraphSubtype, bool isForChangeVizType)
{
    mpTemplateNode = ipTemplateNode;
    mGraphType = iGraphType;
    mGraphSubtype = iGraphSubtype;
    mIsForChangeVizType = isForChangeVizType;
    
    mType = ManipulationSetGraphDefaultProperties;
}

DSSCManipulationSetGraphDefaultProperties::~DSSCManipulationSetGraphDefaultProperties()
{
    
}

int DSSCManipulationSetGraphDefaultProperties::Execute()
{
    mNodesModelChanged.push_back(mpTemplateNode);
    
    GenerateAODeltaXML();
    
    return S_OK;
}

void DSSCManipulationSetGraphDefaultProperties::GenerateAODeltaXML()
{
    if(!mpTemplateNode)
        return;
    
    DSSGraphProperties* lProperties = mpTemplateNode->getGraphProperties();
    if (!lProperties)
        return;
    
    SimpleXMLBuilder lBuilder;
    
    SimpleXMLNode lXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));

    SimpleXMLNode lManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
    SimpleXMLNode lManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("37", true));
    lManipulationMethodXML.AddChild(lManipulationMethodXMLContent);
    lXML.AddChild(lManipulationMethodXML);
    
    int lTreeType = static_cast<EnumDSSRWTreeType>(mpTemplateNode->getTreeType());
    SimpleXMLNode lTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
    SimpleXMLNode lTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lTreeType).c_str(), true));
    lTreeTypeXML.AddChild(lTreeTypeXMLContent);
    lXML.AddChild(lTreeTypeXML);
    
    SimpleXMLNode lNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
    SimpleXMLNode lNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpTemplateNode->getKey().c_str(), true));
    lNodeKeyXML.AddChild(lNodeKeyXMLContent);
    lXML.AddChild(lNodeKeyXML);
    
    SimpleXMLNode lGraphPropertiesXML(SimpleXMLBuilder::CreateNodePtr("gpp"));
    lGraphPropertiesXML.AddProperty("gat", DSSExpNode::IntToString(static_cast<int>(mGraphType)).c_str());
    lGraphPropertiesXML.AddProperty("git", DSSExpNode::IntToString(mGraphSubtype).c_str());
    
    CComVariant lValue;
    lValue.vt = VT_I4;
    lValue.lVal = mGraphType;
    lProperties->setPropertyValue(-3, -3, DssGraphInfoMajorType - DssGraphInfoReserved, lValue);
    
    lValue.vt = VT_I4;
    lValue.lVal = mGraphSubtype;
    lProperties->setPropertyValue(-3, -3, DssGraphInfoMinorType - DssGraphInfoReserved, lValue);
    
    lValue.vt = VT_BSTR;
    lValue.bstrVal = SysAllocString(L"0xffffff");
    lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(FillColor, Background, "0xffffff")));
    lProperties->setPropertyValue(-3, DssGraphBackground, DssGraphAreaColor, lValue);
    if (mIsForChangeVizType)
    {
        lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(GraphAreaAlpha, Background, "255")));
        lValue.vt = VT_I4;
        lValue.lVal = 255;
        lProperties->setPropertyValue(-3, DssGraphBackground, DssGraphAreaAlpha, lValue);
        lProperties->setPropertyValue(-3, DssGraphLegendArea, DssGraphAreaAlpha, lValue);
    }
   
    lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(FillColor, GraphFrame, "0xffffff")));
    lValue.vt = VT_BSTR;
    lValue.bstrVal = SysAllocString(L"0xffffff");
    lProperties->setPropertyValue(-3, DssGraphFrame, DssGraphAreaColor, lValue);
    
    lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(LineColor, GraphFrame, "0xb0b0b0")));
    lValue.bstrVal = SysAllocString(L"0xb0b0b0");
    lProperties->setPropertyValue(-3, DssGraphFrame, DssGraphLineColor, lValue);
    
    lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(FontFamily, DefaultText, "Verdana")));
    lValue.vt = VT_BSTR;
    lValue.bstrVal = SysAllocString(L"Verdana");
    lProperties->setPropertyValue(-3, DssGraphAllTexts, DssGraphFontName, lValue);
    
    lValue.vt = VT_I4;
    lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(FontStyle, DefaultText, "0")));
    lValue.lVal = 0;
    lProperties->setPropertyValue(-3, DssGraphAllTexts, DssGraphFontStyle, lValue);
    
    lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(FontSize, DefaultText, "8")));
    lValue.lVal = 8;
    lProperties->setPropertyValue(-3, DssGraphAllTexts, DssGraphFontSize, lValue);
    
    lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(FontColor, DefaultText, "0x666666")));
    lValue.vt = VT_BSTR;
    lValue.bstrVal = SysAllocString(L"0x666666");
    lProperties->setPropertyValue(-3, DssGraphAllTexts, DssGraphFontColor, lValue);
    
    lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(EnhancedSeriesEffect, Default, "0")));
    lValue.vt = VT_BOOL;
    lValue.lVal = VARIANT_FALSE;
    lProperties->setPropertyValue(-3, DssGraphTextContent, DssGraphEnhancedSeriesEffect, lValue);
    
    for(int i = 0; i < 30; i++)
    {
        lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(FillAlpha, Riser, i, "255")));
        lValue.vt = VT_I4;
        lValue.lVal = 255;
        lProperties->setPropertyValue(i, DssGraphRiser, DssGraphAreaAlpha, lValue);
        
        lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(BevelProperties, Riser, i, "0,0,0,0")));
        lValue.vt = VT_BSTR;
        lValue.bstrVal = SysAllocString(L"0,0,0,0");
        lProperties->setPropertyValue(i, DssGraphRiser, DssGraphAreaCustomBevelEffect, lValue);
        
        lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(SeriesDependentDataLineType, Default, i, "2")));
        lValue.vt = VT_BOOL;
        lValue.boolVal = VARIANT_TRUE;
        lProperties->setPropertyValue(i, DssGraphRiser, DssGraphShowSDDataLineType, lValue);
    }
    
    lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(AxisAllowFractionalGrids, AxesXBody, "0")));
    lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(AxisAllowFractionalGrids, AxesY1Body, "0")));
    lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(AxisAllowFractionalGrids, AxesY2Body, "0")));
    
    lValue.vt = VT_BSTR;
    lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(LineColor, AxesCategoryMajorLine, "0xebebeb")));
    lValue.bstrVal = SysAllocString(L"0xebebeb");
    lProperties->setPropertyValue(-3, DssGraphO1Major, DssGraphLineColor, lValue);
    
    lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(LineColor, AxesXMajorLine, "0xebebeb")));
    lValue.bstrVal = SysAllocString(L"0xebebeb");
    lProperties->setPropertyValue(-3, DssGraphX1Major, DssGraphLineColor, lValue);
    
    lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(LineColor, AxesY1MajorLine, "0xebebeb")));
    lValue.bstrVal = SysAllocString(L"0xebebeb");
    lProperties->setPropertyValue(-3, DssGraphY1Major, DssGraphLineColor, lValue);
    
    lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(LineColor, AxesY2MajorLine, "0x8f8f8f")));
    lValue.bstrVal = SysAllocString(L"0x8f8f8f");
    lProperties->setPropertyValue(-3, DssGraphY2Major, DssGraphLineColor, lValue);
    
    lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(FillColor, LegendArea, "0xebebeb")));
    lValue.bstrVal = SysAllocString(L"0xebebeb");
    lProperties->setPropertyValue(-3, DssGraphLegendArea, DssGraphAreaColor, lValue);
    
    lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(LineColor, AxesXBody, "0xb0b0b0")));
    lValue.bstrVal = SysAllocString(L"0xb0b0b0");
    lProperties->setPropertyValue(-3, DssGraphX1Body, DssGraphLineColor, lValue);
    
    lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(LineColor, AxesY1Body, "0xb0b0b0")));
    lValue.bstrVal = SysAllocString(L"0xb0b0b0");
    lProperties->setPropertyValue(-3, DssGraphY1Body, DssGraphLineColor, lValue);
    
    lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(LineColor, AxesY2Body, "0x8f8f8f")));
    lValue.bstrVal = SysAllocString(L"0x8f8f8f");
    lProperties->setPropertyValue(-3, DssGraphY2Body, DssGraphLineColor, lValue);
    
    lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(FillColor, Riser, 0, "0x1f77b4")));
    lValue.bstrVal = SysAllocString(L"0x1f77b4");
    lProperties->setPropertyValue(0, DssGraphRiser, DssGraphAreaColor, lValue);
    
    lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(FillColor, Riser, 1, "0xd62728")));
    lValue.bstrVal = SysAllocString(L"0xd62728");
    lProperties->setPropertyValue(1, DssGraphRiser, DssGraphAreaColor, lValue);
    
    lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(FillColor, Riser, 2, "0x2ca02c")));
    lValue.bstrVal = SysAllocString(L"0x2ca02c");
    lProperties->setPropertyValue(2, DssGraphRiser, DssGraphAreaColor, lValue);
    
    lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(FillColor, Riser, 3, "0xff7f0e")));
    lValue.bstrVal = SysAllocString(L"0xff7f0e");
    lProperties->setPropertyValue(3, DssGraphRiser, DssGraphAreaColor, lValue);
    
    lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(FillColor, Riser, 4, "0x9467bd")));
    lValue.bstrVal = SysAllocString(L"0x9467bd");
    lProperties->setPropertyValue(4, DssGraphRiser, DssGraphAreaColor, lValue);
    
    lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(FillColor, Riser, 5, "0xbcbd22")));
    lValue.bstrVal = SysAllocString(L"0xbcbd22");
    lProperties->setPropertyValue(5, DssGraphRiser, DssGraphAreaColor, lValue);
    
    lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(FillColor, Riser, 6, "0x17becf")));
    lValue.bstrVal = SysAllocString(L"0x17becf");
    lProperties->setPropertyValue(6, DssGraphRiser, DssGraphAreaColor, lValue);
    
    lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(FillColor, Riser, 7, "0x7f7f7f")));
    lValue.bstrVal = SysAllocString(L"0x7f7f7f");
    lProperties->setPropertyValue(7, DssGraphRiser, DssGraphAreaColor, lValue);
    
    lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(FillColor, Riser, 8, "0xaec7e8")));
    lValue.bstrVal = SysAllocString(L"0xaec7e8");
    lProperties->setPropertyValue(8, DssGraphRiser, DssGraphAreaColor, lValue);
    
    lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(FillColor, Riser, 9, "0xff9896")));
    lValue.bstrVal = SysAllocString(L"0xff9896");
    lProperties->setPropertyValue(9, DssGraphRiser, DssGraphAreaColor, lValue);
    
    lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(FillColor, Riser, 10, "0x98df8a")));
    lValue.bstrVal = SysAllocString(L"0x98df8a");
    lProperties->setPropertyValue(10, DssGraphRiser, DssGraphAreaColor, lValue);
    
    lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(FillColor, Riser, 11, "0xffbb78")));
    lValue.bstrVal = SysAllocString(L"0xffbb78");
    lProperties->setPropertyValue(11, DssGraphRiser, DssGraphAreaColor, lValue);
    
    lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(FillColor, Riser, 12, "0xc5b0d5")));
    lValue.bstrVal = SysAllocString(L"0xc5b0d5");
    lProperties->setPropertyValue(12, DssGraphRiser, DssGraphAreaColor, lValue);
    
    lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(FillColor, Riser, 13, "0xdbdb8d")));
    lValue.bstrVal = SysAllocString(L"0xdbdb8d");
    lProperties->setPropertyValue(13, DssGraphRiser, DssGraphAreaColor, lValue);
    
    lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(FillColor, Riser, 14, "0x9edae5")));
    lValue.bstrVal = SysAllocString(L"0x9edae5");
    lProperties->setPropertyValue(14, DssGraphRiser, DssGraphAreaColor, lValue);
    
    lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(FillColor, Riser, 15, "0xc7c7c7")));
    lValue.bstrVal = SysAllocString(L"0xc7c7c7");
    lProperties->setPropertyValue(15, DssGraphRiser, DssGraphAreaColor, lValue);
    
    lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(LockFontSize, Default, "1")));
    
    lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(TitleShow, Default, "false")));
    
    lValue.vt = VT_I4;
    if(mGraphType == MajorTypeScatter || mGraphType == MajorTypeBubble)
    {
        lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(LineThickness, AxesXMajorLine, "50")));
        lValue.lVal = 50;
        lProperties->setPropertyValue(-3, DssGraphX1Major, DssGraphLineWidth, lValue);
    }
    else
    {
        lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(LineThickness, AxesCategoryMajorLine, "50")));
        lValue.lVal = 50;
        lProperties->setPropertyValue(-3, DssGraphO1Major, DssGraphLineWidth, lValue);
    }
    lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(LineThickness, AxesY1MajorLine, "50")));
    lValue.lVal = 50;
    lProperties->setPropertyValue(-3, DssGraphY1Major, DssGraphLineWidth, lValue);
    
    if(mGraphType == MajorTypeScatter || mGraphType == MajorTypeBubble)
    {
        lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(SeriesDependentBubbleLineShow, Riser, "false")));
        lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(BubbleScatterUnknown1, Riser, DSSExpNode::IntToString(static_cast<int>(GraphMarkerShapeCircle)).c_str())));
        lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(BubbleScatterUnknown2, Riser, mGraphType == MajorTypeBubble ? "33" : "31")));
    }
    
    if(mGraphType == MajorTypeVerticalLine || mGraphType == MajorTypeHorizontalLine)
    {
        lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(BubbleScatterUnknown1, Riser, DSSExpNode::IntToString(static_cast<int>(GraphMarkerShapeCircle)).c_str())));
        lGraphPropertiesXML.AddChild(static_cast<xmlNodePtr>(GenerateGraphPropertyXML(BubbleScatterUnknown2, Riser, "18")));
    }
    
    lXML.AddChild(lGraphPropertiesXML);
    
    SimpleXMLNode lXML2(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
    
    SimpleXMLNode lManipulationMethodXML2(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
    SimpleXMLNode lManipulationMethodXMLContent2(SimpleXMLBuilder::CreateNodePtr("19", true));
    lManipulationMethodXML2.AddChild(lManipulationMethodXMLContent2);
    lXML2.AddChild(lManipulationMethodXML2);
    
    int lTreeType2 = static_cast<int>(mpTemplateNode->getTreeType());
    SimpleXMLNode lTreeTypeXML2(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
    SimpleXMLNode lTreeTypeXMLContent2(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lTreeType2).c_str(), true));
    lTreeTypeXML2.AddChild(lTreeTypeXMLContent2);
    lXML2.AddChild(lTreeTypeXML2);
    
    SimpleXMLNode lNodeKeyXML2(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
    SimpleXMLNode lNodeKeyXMLContent2(SimpleXMLBuilder::CreateNodePtr(mpTemplateNode->getKey().c_str(), true));
    lNodeKeyXML2.AddChild(lNodeKeyXMLContent2);
    lXML2.AddChild(lNodeKeyXML2);
    
    SimpleXMLNode lReportManipulationXML(SimpleXMLBuilder::CreateNodePtr("report_manipulation"));
    
    SimpleXMLNode lReportManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("report_manipulation_method"));
    SimpleXMLNode lReportManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("38", true));
    lReportManipulationMethodXML.AddChild(lReportManipulationMethodXMLContent);
    lReportManipulationXML.AddChild(lReportManipulationMethodXML);
    
    SimpleXMLNode lReportPropertiesXML(SimpleXMLBuilder::CreateNodePtr("properties_xml"));
    
    SimpleXMLNode lPrsXML(SimpleXMLBuilder::CreateNodePtr("prs"));
    lPrsXML.AddProperty("did", "3226ED12769B11D3B91E00C04F2233EA");
    lPrsXML.AddProperty("n", "GraphInfo");
    lPrsXML.AddProperty("dirty", "1");
    
    SimpleXMLNode lPrXML1(SimpleXMLBuilder::CreateNodePtr("pr"));
    lPrXML1.AddProperty("n", "MaxSeries");
    lPrXML1.AddProperty("dt", "3");
    lPrXML1.AddProperty("pdv", "5");
    lPrXML1.AddProperty("pri", "3");
    lPrXML1.AddProperty("pru", "0");
    lPrXML1.AddProperty("dirty", "1");
    lPrXML1.AddProperty("v", "30");
    
    SimpleXMLNode lPrXML2(SimpleXMLBuilder::CreateNodePtr("pr"));
    lPrXML2.AddProperty("n", "MaxCategories");
    lPrXML2.AddProperty("dt", "3");
    lPrXML2.AddProperty("pdv", "6");
    lPrXML2.AddProperty("pri", "4");
    lPrXML2.AddProperty("pru", "0");
    lPrXML2.AddProperty("dirty", "1");
    lPrXML2.AddProperty("v", "30");
    
    lPrsXML.AddChild(lPrXML1);
    lPrsXML.AddChild(lPrXML2);
    lReportPropertiesXML.AddChild(lPrsXML);
    lReportManipulationXML.AddChild(lReportPropertiesXML);
    lXML2.AddChild(lReportManipulationXML);
    
    SimpleXMLNode lRoot(lBuilder.GetRoot());
    lRoot.AddChild(lXML);
    lRoot.AddChild(lXML2);
    mpActionObject = new DSSAODeltaXML(ManipulationSetGraphDefaultProperties);
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lBuilder.ToString(false));
}

void* DSSCManipulationSetGraphDefaultProperties::GenerateGraphPropertyXML(EnumMFGraphAttributeID iPropertyID, EnumMFGraphObjectID iObjectID, std::string iPropertyValue)
{
    SimpleXMLNode lGraphPropertiesXML(SimpleXMLBuilder::CreateNodePtr("gp"));
    lGraphPropertiesXML.AddProperty("gaid", DSSExpNode::IntToString(static_cast<int>(iPropertyID)).c_str());
    lGraphPropertiesXML.AddProperty("goid", DSSExpNode::IntToString(static_cast<int>(iObjectID)).c_str());
    lGraphPropertiesXML.AddProperty("v", iPropertyValue.c_str());
    return lGraphPropertiesXML.GetNode();
}

void* DSSCManipulationSetGraphDefaultProperties::GenerateGraphPropertyXML(EnumMFGraphAttributeID iPropertyID, EnumMFGraphObjectID iObjectID, int iSeriesID, std::string iPropertyValue)
{
    SimpleXMLNode lGraphPropertiesXML(SimpleXMLBuilder::CreateNodePtr("gp"));
    lGraphPropertiesXML.AddProperty("gaid", DSSExpNode::IntToString(static_cast<int>(iPropertyID)).c_str());
    lGraphPropertiesXML.AddProperty("goid", DSSExpNode::IntToString(static_cast<int>(iObjectID)).c_str());
    lGraphPropertiesXML.AddProperty("gsid", DSSExpNode::IntToString(iSeriesID).c_str());
    lGraphPropertiesXML.AddProperty("v", iPropertyValue.c_str());
    return lGraphPropertiesXML.GetNode();

}