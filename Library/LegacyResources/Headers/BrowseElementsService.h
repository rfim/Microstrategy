//
//  BrowseElementsService.h
//  MSTRMobile
//
//  Created by Ilia Baskin on 3/8/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSIServiceImpl.h"

@class MSIProjectInfo;
/**
 The BrowseElementsService will find all the elements matching the given criteria.
 
	@param	handler	id<MSTRHandler>	object to receive the blocks as an NSArray of Elemetns, or handle the error.
	@param	async	Set to YES for asynchronous execution, NO for synchronous.
	@param	attributeId	the object ID of the attribute (e.g. Category or Region) on the mobile server
	@param	project	The ProjectInfo for the project to browse. One way it can be found is via MSIPreferencesStore -> MSIDevicePreferences -> WebServerList -> WebServerConfiguration -> ProjectList -> ProjectConfiguration -> ProjectInfo
	@param	blockBegin	1-based index of the items to return. This is useful for retrieving incrementally.
	@param	blockCount	Number of items to return. Use -1 to retrieve all matching Elements.
	@param	searchPattern	String that is matched against the displayName. A nil or empty string matches all. You can use SQL wildcards "%" and "_" to match a pattern. If a displayName contains the pattern, it matches. E.g. "Electronics" matches "tron" and it matches "E%"
	@param	filterXML	To Be Documented.
	@param	shortFilterXML	To Be Documented.
	@param	totalSize	(read-only) The number of items that matched, even if blockCount was set to fewer.
 
	@return an array of Element objects.
 
	@see	The PromptTypesSample project for an example of usage.
 */
@interface BrowseElementsService : MSIServiceImpl<ServiceHandler> {
@private
	MSIProjectInfo* project;
	MSTRError* error;
	NSString* attributeId;
	NSString* filterXML;
	NSString* shortFilterXML;
	NSInteger blockBegin;
	NSInteger blockCount;
	NSMutableArray* blocks;	
	NSInteger totalSize;
	NSString* searchPattern;
	NSString* searchFormId; //if we are searching on a particular form
	NSString* targetAttributeId;
	NSString* dataSourcesXML;
    NSString* datasetId;
    NSString* cubeinsId;
    NSString* ctlKey;
	NSString* messageID;
}
@property (nonatomic, copy) NSString* datasetId;
@property (nonatomic, copy) NSString* cubeinsId;
@property (nonatomic, strong) MSIProjectInfo* project;
@property (nonatomic, copy) NSString* attributeId;
@property (nonatomic, copy) NSString* filterXML;
@property (nonatomic, copy) NSString* shortFilterXML;
@property (nonatomic, copy) NSString* searchPattern;
@property NSInteger blockBegin;
@property NSInteger blockCount;
@property (nonatomic, copy) NSString* searchFormId;
@property (nonatomic, copy) NSString* targetAttributeId;
@property (nonatomic, copy) NSString* dataSourcesXML;
@property (nonatomic, copy) NSString* ctlKey;
@property (nonatomic, copy) NSString* messageID;
@property (nonatomic, weak) MSIReportDisplayInfo* document;

@end
