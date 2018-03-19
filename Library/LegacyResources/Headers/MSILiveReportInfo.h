//
//  LiveReportInfo.h
//
//  Copyright 2010 MicroStrategy, Incorporated. All rights reserved.
//

#import "MSIReportDisplayInfo.h"
#import "MSIObjectInfo.h"
#import "MSIProjectInfo.h"
#import "ReportInfo.h"

/**
 * This class is used to represent a report (or document) that is executed "live" from the folder browser UI.
 * It is a subclass of MSIReportDisplayInfo which represents all reports (or documents)
 *
 */
@interface MSILiveReportInfo : MSIReportDisplayInfo {
@private
	BOOL isPrompted;
	BOOL answersSavedToMessage;
	BOOL openPromptsRemain;
	BOOL isIncompleteAnswerSet;
	BOOL promptsPreviouslyAnswered;
	BOOL wasDataDisplayed;
	BOOL isLinkedDrillWithAnsweredPrompts;
}

// Properties...
@property BOOL isPrompted;
@property BOOL answersSavedToMessage;
@property BOOL isIncompleteAnswerSet;
@property (nonatomic, readonly) BOOL openPromptsRemain;
@property (nonatomic, readonly) BOOL promptsPreviouslyAnswered;
@property BOOL wasDataDisplayed;
@property BOOL isLinkedDrillWithAnsweredPrompts;

/**
 * Initialize object
 * 
 * @param objectinfo ObjectInfo for the report
 * @param projectInfo Project information
 */
-(id)initWithObjectInfo:(MSIObjectInfo*) objectInfo AndProject:(MSIProjectInfo*)projectInfo;

/**
 * Callback after prompt answer saved to report info
 * 
 * @param messageStatus Message status
 * @param newMessageID Message ID
 */
-(void)promptAnswersSaved:(NSInteger)messageStatus newMessageID:(NSString*)newMessageID;

/**
 * Callback after prompt answer cleared
 * 
 */
-(void)promptAnswersCleared;

/**
 * Ensures that we already have answers for any UnsupportedPrompt's, since we are not able to present those.
 *
 * @param prompts Prompts to be validated
 * @param anError Return error
 * @return YES if all Prompts are supported or answered.
 */
- (BOOL) validatePrompts: (Prompts*) prompts anError: (MSTRError**) anError;

/**
 * Update object by new object info
 * 
 * @param objectInfo New object info
 * @return If object has changed
 */
-(BOOL)updateFields:(MSIObjectInfo*)objectInfo;

/**
 * Check if the cache root ID passed is for a live report info
 * 
 * @param cacheRootID Cache root ID to check
 * @return If this ID is for a live report info
 */
+(BOOL)isValidCacheRootID:(NSString*)cacheRootID;

/**
 * Generate cache root ID based on object ID
 *
 * @param dssid Object ID
 * @return Generated cache root ID
 */
+(NSString*)getCacheRootIDForObject:(NSString *)dssid;


/**
 * Get cache ID in cache set based on server cache ID
 *
 * @param server cache ID
 * @param cache set ID
 * @return cache ID
 */
-(NSString*)getCahceIDFor:(NSString*)serverCacheID inSet:(NSString*)cacheSetID;

@end
