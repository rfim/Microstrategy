//
//  SummaryController.h
//  MSTRMobile
//
//  Created by Amirali Charania on 10/28/09.
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Service.h"

@class ProjectConfiguration, ObjectInfo, ElementListController, TaskGroup, MSIReportDisplayInfo;

@interface SummaryController : UITableViewController {
	// new
	MSIReportDisplayInfo* report;
	
	// old
	id<Service> service;
	NSMutableArray* browsePathObjects;
	ElementListController* elementListController;
	TaskGroup *taskGroup;
	UILabel* noPersonalizationLabel;
}

// new
@property (nonatomic, retain) MSIReportDisplayInfo* report;

// old
@property (nonatomic, assign) id<Service> service;
@property (nonatomic, retain) NSMutableArray* browsePathObjects;
@property (nonatomic, retain) TaskGroup *taskGroup;

- (id)initWithStyle:(UITableViewStyle)style;
- (ProjectConfiguration*)project;
- (void) storePersonalization:(ObjectInfo*)attribute elements:(NSMutableArray*)elements;

@end
