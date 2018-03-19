//
//  DSSXTabViewHelper.h
//  ReportServiceCore
//
//  Created by He Tengfei on 11/13/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//


#ifndef DSSXTABVIEWHELPER_H
#define DSSXTABVIEWHELPER_H

#include "stddef.h"

#import <map>

class DSSViewDataSet;
class DSSTemplate;
class DSSXTabView;
class DSSRWGridModel;

class DSSXTabDispInfo;
class DSSThresholds;
class DSSRTTemplateInfo;

class DSSTabularIndex;
class DSSXTabBitVector;

class DSSMaskInfos;
class DSSSliceID;
class DSSCube;
class DSSFunctionServer;
class DSSTemplateMetrics;
class MCESchema;
class DSSTabularData;

class DSSXTabViewHelper
{
public:
	
	
	static int InitXTabView(DSSViewDataSet *ipViewDataSet, 
                                                   DSSTemplate *ipRuntimeTemplate,
                                                   DSSXTabView **opView,
                                                   DSSRWGridModel *ipGridModel,
                            bool iDoPreXTab = true, DSSMaskInfos *ipMaskInfo = NULL, bool ibHideZero = false, bool ibHideNull = false, MCESchema* ipSchema = NULL);//fali, 815802, add parameter to support NullOrZero
    
	static int ExtractDispInfo(DSSTemplate *pTemplate, 
                                                     DSSViewDataSet *pViewDataSet, 
                                                     DSSXTabDispInfo *pDispInfo);
    
    static int InitXTabViewForFieldGroup(DSSViewDataSet *ipViewDataSet, /*DSSRTTemplateInfo *ipTemplateInfo,*/ DSSXTabView **opView, bool iDoPreXTab = true);
    
    static int ExtractDispInfoForFieldGroup(/*DSSRTTemplateInfo *ipTemplateInfo, */DSSViewDataSet *ipViewDataSet, DSSXTabDispInfo *pDispInfo);
    
    static int RefreshThresholdSlices(DSSViewDataSet *ipViewDataSet,
                                      DSSTemplate *ipRuntimeTemplate,
                                      DSSXTabView *ipView);
    
    //fali, 815802, calculate Hide NullorZero
    static int CalculateHideNullorHideZero(DSSSliceID &irSliceID, bool bHideZero, bool bHideNull,
                                           MCESchema* ipSchema, DSSSliceID &orFilterSliceID, DSSCube* opTargetCube);

    static int CalculateMaskInfoOfMainIndex(DSSViewDataSet* ipViewDataSet,DSSTemplateMetrics *ipTemplateMetrics,
                                                        bool bHideZero, bool bHideNull, MCESchema* ipSchema, DSSMaskInfos* opMaskInfos, DSSCube* opTargetCube);
    
    static int CheckMissing(DSSSliceID &irSliceID, bool &bMissingorNotExisted);
    
private:
    static int hReplaceWithRecursiveUnit(DSSTabularData* ipTabularData, DSSXTabDispInfo* ipDispInfo, vector<int>& irvTotalUnits, int* opLevel);

};

#endif