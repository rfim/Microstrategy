//
//  RWTaskRequest.h
//  MSTRMobile
//
//  Created by Kevin Maurer on 6/24/09.
//  Copyright 2009 MicroStrategy. All rights reserved.
//

/** 	
 *	Class renamed from RWDTaskRequest to RWTaskRequest
 */

#import <Foundation/Foundation.h>
#import "MessageBasedTaskRequest.h"

@interface RWTaskRequest : MessageBasedTaskRequest {

@private
	
	NSString* groupByKeys;
	NSInteger layoutIndex;
	NSString* nodeKey;
	BOOL includeLayout;
	BOOL includeGroupBy;
	BOOL includeExportData;
    bool useDefaultExportSettings;
	BOOL includeInfo;
	NSString* desiredGroupByKeys;
	NSInteger hasGroupBy;
	NSString* gridKey;
	NSInteger rebuildForPDF;
	BOOL supportsJson;
	
	//incremental fetch starting row number 1-based 
	NSInteger gridIfPosition;
	
	//incremental fetch setting: max rows per page for the grid on a page.
	NSInteger rowsPerPage;
	
	// The visualization style to use with the layout (if known)...
	NSString* visualizationStyle;
	
	//screenwidth we send. helpful for microapp mode, to get the content to fit the width of the screen
	NSInteger screenWidth;

}

@property (nonatomic, copy) NSString* groupByKeys;
@property (nonatomic, copy) NSString* desiredGroupByKeys;
@property (nonatomic, copy) NSString* nodeKey;
@property NSInteger screenWidth;
@property NSInteger layoutIndex;
@property BOOL includeLayout;
@property BOOL includeGroupBy;
@property BOOL includeExportData;
@property (nonatomic, assign) bool useDefaultExportSettings;
@property BOOL includeInfo;
@property NSInteger gridIfPosition;
@property NSInteger rowsPerPage;
@property (nonatomic, copy) NSString* visualizationStyle;
@property NSInteger hasGroupBy;
@property (nonatomic,copy) NSString* gridKey;
@property (nonatomic) NSInteger rebuildForPDF;
@property BOOL supportsJson;

@end

