//
//  DSSCManipulationHelper.h
//  ReportServiceCore
//
//  Created by Zeng Jinliang on 7/18/14.
//  Copyright (c) 2014 MicroStrategy, Inc. All rights reserved.
//  Discription: Used to extract the same logic in multiple manipulations

#ifndef CManipulation_Helper_H
#define CManipulation_Helper_H

#include "DSSObjectContext.h"

class DSSRWControl;
class DSSRWNode;
class DSSBaseElementsProxy;
class DSSCExpression;
class DSSMetric;
class DSSExpNode;
class DSSTemplate;
class DSSTemplateUnit;

class DSSCManipulationHelper
{
public:
    typedef std::map<std::string, DSSRWNode*> NodeMapType;
public:
    // used to sync multiple control's selection while their target is the same DDIC
    // caller need release the new NodeAndControls in ioSelectionUPdateNodeMap while it's node type is template node
    static int syncSelectionByTargetControl(DSSRWControl* ipTargetControl, const std::string& iSourceNodeKey, const std::string& iUserTargetKey, NodeAndControlsMap& ioSelectionUpdateNodeMap);
    
    // set target node's groupByUnit's selection info
    typedef enum EnumTargetType
    {
        DssTargetTypeReserved = 0,
        DssNodeTarget = 1,
        DssControlTarget = 2,
        DssPlaceHolderTarget = 3
    } EnumTargetType;
    static int setTargetGroupByUnit(DSSRWNode* ipTargetNode, DSSRWControl* ipSrcControl, DSSCExpression* ipExpression, DSSBaseElementsProxy* ipElements, EnumTargetType iTargetType = DssNodeTarget);
    // set target control's groupByUnit's selection info
    static int setTargetGroupByUnit(DSSRWControl* ipTargetControl, DSSRWControl* ipSrcControl, DSSCExpression* ipExpression, DSSBaseElementsProxy* ipElements);

    // add placeHolderMap to node map server
    static int addPlaceHolderMapToNodeMapServer(DSSRWControl* ipControl, std::map<std::string, DSSRWNode*>& ioNodeMapServer, std::vector<DSSRWNode*>& ioModelChangedNodes, NodeAndControlsMap& ioSelectionUpdateNodeMap, bool iExcludeSourceNode = false);
    
    // There is a property of control which set Action For Direct Target Selectors. Check it and do the action for targets.
    static int updateSelectionOfAffectedControl(DSSRWControl* ipControl, NodeMapType& ioNodeMapServer, std::vector<DSSRWControl*>& ioControlsModelChanged, std::vector<DSSRWNode*>& ioModelChangedNodes, NodeAndControlsMap& ioSelectionUpdateNodeMap, bool isRecursive = false);

    // set target's groupByUnit's selection info and record the node changed info list
    static int updateTargetGroupByUnit(DSSRWNode* ipTargetNode, DSSRWControl* ipSrcControl, NodeMapType& ioNodeMapServer, std::vector<DSSRWNode*>& ioModelChangedNodes, NodeAndControlsMap& ioSelectionUpdateNodeMap, DSSCExpression* ipExpression, DSSBaseElementsProxy* ipElements, EnumTargetType iTargetType = DssNodeTarget);
    static int updateTargetGroupByUnit(DSSRWControl* ipTargetControl, DSSRWControl* ipSrcControl, std::vector<DSSRWControl*>& ioControlsModelChanged, std::vector<DSSRWNode*>& ioModelChangedNodes, NodeAndControlsMap& ioSelectionUpdateNodeMap, DSSCExpression* ipExpression, DSSBaseElementsProxy* ipElements);

    // push subtree into nodeModelChanged except ipExcpNode
    static int populateSubtreeIntoNodeModelChanged(DSSRWNode* ipNode, DSSRWNode* ipExcpNode, std::vector<DSSRWNode*>& ioModelChangedNodes);
    
    /// check dynamic metric, it's used to sort manipulation
    // check does there exist dynamic in template
    static int checkDynamicMetricInVT(DSSTemplate* ipTemplate, bool& opHasDynamicMetric, bool& opSubsetNeeded);
    // check whether a metric is dynamic-dimty
    static int isMetricDynamic(DSSMetric* ipMetric, bool& opDynamic, bool& opSubsetNeeded);
    // check whether a metric is break by/Sort by in DropZone
    static int checkDynamicBreakBySortByInNode(DSSExpNode* ipRoot, bool& opDynamic);
    
    static bool replaceUnitOnTemplate(DSSTemplate* iTemplate, GUID iReplaceAttributeID, DSSTemplateUnit* newAttribute);
private:
    static int syncSelectionByTargetControlRecursively(DSSRWControl* ipTargetControl, const std::string& iSourceNodeKey, const std::string& iUserTargetKey, NodeAndControlsMap& ioSelectionUpdateNodeMap, std::vector<std::string>& ioVisitList, bool isRecursive = true);
};

#endif /* defined(CManipulation_Helper_H) */
