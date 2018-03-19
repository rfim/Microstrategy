//
//  RefreshDocumentService.h
//  MicroStrategyMobile
//
//  Created by Dai Li on 10/5/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "XMLAPIService.h"

@class MSIReportDisplayInfo;

typedef enum _RefreshDocumentOption{
    RefreshDocumentOptionAgainstIServerCache,          //retrieve data from the latest valid Intelligence Server cache
    RefreshDocumentOptionAgainstWarehouse,      //re-run against the warehouse
} RefreshDocumentOption;


/**
 * This service refreshes document (with its' message ID in working set) against data warehouse or iServer cache.
 * This service maintains mainpulations performed on the document, which is different from document execution.
 * This service is an asychronizd service, please use <code>IPadPollStatusService</code> to query its' execution status.
 * To save&replay client manipulations, please save client manipulations with <code>RWManipulationAndBinaryDataService</code>
 *
 */
@interface RefreshDocumentService : XMLAPIService {
	MSIReportDisplayInfo* document;
}

/**
 * document to be refreshed. 
 * This is required property and cannot be nil. Document's currentMessageID is also required and cannot be nil.
 */
@property (nonatomic, strong) MSIReportDisplayInfo* document;

/**
 * refresh option. default value is RefreshDocumentOptionAgainstCache if this value is not provided
 */
@property (nonatomic) RefreshDocumentOption refreshDocumentOption;

@end