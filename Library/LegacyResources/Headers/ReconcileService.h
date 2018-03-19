//
//  ReconcileService.h
//  MSTR Foundation
//
//  Created by Amirali Charania on 2/20/09.
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSIServiceImpl.h"

#define RECONCILE_IN_PROGRESS 1
#define RECONCILE_DONE 2
#define RECONCILE_ERROR 3

@class MSIProjectConfiguration, MSIProjectInfo, MSTRMultipleError, MSIReportDisplayInfo, MSIObjectInfo, MSTRError;

@protocol ReconcileServiceHandler<ServiceHandler>

-(void)onProjectSuccess:(id)data project:(MSIProjectInfo*)project service:(id<Service>)service;
-(void)onProjectFailure:(MSTRMultipleError*)error project:(MSIProjectInfo*)project service:(id<Service>)service;
-(BOOL)onCredentialsFailure:(MSTRException*)exception project:(MSIProjectInfo*)project service:(id<Service>)service;
-(BOOL)onCertificateFailure:(MSTRException*)exception service:(id<Service>)service;

@end

//If project is given, only reconcile the given project for the given scope.  
//If both project and report are given, we will only reconcile the given report. The scope is irrelavent. 
//If neither project or report is given, we will reconcile all projects for the given scope. 
//There are three scopes: 
//1) ReconcileScopeLiveReports: to reconcile only live reports. 
//2) ReconcileScopeSubscriptions: to reconcile only the subscriptions.
//3) ReconcileScopeAll: to reconcile both live reports and subscriptions.

@interface ReconcileService : MSIServiceImpl <ServiceHandler> {
@private
	BOOL ignoreAccessProjectError;
@protected 
	NSMutableArray* networkReportList;
	MSIProjectConfiguration* __weak currentProject;
	NSMutableArray* reconcileErrors;
	NSMutableArray* projectErrors;
	MSIReportDisplayInfo* parsingCurrentReport;
	NSMutableDictionary* cacheInfoClientMap;
	NSMutableDictionary* cacheInfoServerMap;
	MSIProjectConfiguration* project;
	MSIReportDisplayInfo* report;
	NSInteger scope; //ReconcileScopeLiveReports, ReconcileScopeSubscription, ReconcileScopeAll, ReconcileScopeCustomize
	NSInteger behavior; // ReconcileDropInvalid, ReconcileKeepInvalid
	NSString* subscriptionID;
	MSTRError* currentError;
}

@property (nonatomic, strong) MSIProjectConfiguration* project;
@property (nonatomic, strong) MSIReportDisplayInfo* report;
@property (nonatomic) NSInteger scope;
@property (nonatomic) NSInteger behavior;
@property (nonatomic, strong) NSString* subscriptionID;
@property (nonatomic, strong) MSTRError* currentError;

/**
 * input ReportDisplayInfo when scope is ReconcileScopeCustomize
 *   Type definition: NSDictionary*<NSString*, NSArray*<MSIReportDisplayInfo*>>
 */
@property (nonatomic, strong) NSDictionary* customReportArrayByProjectIDDict;
@property (nonatomic) bool customReconcileSubscription;
@end
