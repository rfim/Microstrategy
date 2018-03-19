//
//  MSIAdHocSelectorSearch.h
//  MicroStrategyMobile
//
//  Created by Liu, Xin on 12/31/13.
//  Copyright (c) 2013 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@class MSIProjectInfo;

/**
 Selector Search types
 */

typedef enum
{
	MSIAdHocSelectorSearchExpressionTypeStartsWith = 0,
	MSIAdHocSelectorSearchExpressionTypeContains,
} MSIAdHocSelectorSearchExpressionType;

/**
 * The MSIAdHocSelectorSearch protocol provides methods for ad hoc selector search, typically the callback methods when the search is finished. Please make sure the target is a valid alive object since the methods may be called after async process. Set the delegate to "nil" before releasing the ad hoc search instance.
 * The searched results are stored in an array and passed to the delegate.
 */

@protocol MSIAdHocSelectorSearchDelegate <NSObject>

/**
 * Use this method to pass the searched results to the delegate. The method is called when the search is finished and the target is the delegate.
 * @param results is the searched results
 * @param pageIndex is the page index of the search results
 * @param isSucceed indicates search is successful or not
 */
- (void)adHocSearchDidFinish:(NSArray *)results pageIndex:(NSInteger)pageIndex succeed:(BOOL)isSucceed;

@end

@interface MSIAdHocSelectorSearch : NSObject

@property (nonatomic) NSInteger maxResultsPerSearch;
@property (nonatomic, weak) id<MSIAdHocSelectorSearchDelegate> delegate;
@property (nonatomic, readonly) NSInteger currentPageIndex;
@property (nonatomic, readonly) NSInteger resultCount;

- (id) initWithProjectInfo:(MSIProjectInfo*)projectInfo andAttributeID:(NSString*)attributeID;

- (void) performSearchWithString:(NSString*)searchString;

- (void) performSearchWithString:(NSString*)searchString andSearchExpressionType:(MSIAdHocSelectorSearchExpressionType)searchExpressionType;

- (void) enableSearch:(BOOL)enable;

- (void) retrievePreviousResults;

- (void) retrieveNextResults;

@end
