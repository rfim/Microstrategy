//
//  MSIReportDisplayInfo.h
//  MSTR Foundation
//
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSTRHandler.h"
#import "Enums.h"


// XML tags
#define REPORT_ID @"id"
#define REPORT_TYPE @"tp"
#define REPORT_NAME @"n"
#define REPORT_SUB_TYPE @"stp"
#define REPORT_DESCRIPTION @"des"
#define REPORT_SUBSCRIPTION_ID @"sub_id"
#define REPORT_SUBSCRIPTION_NAME @"sub_name"
#define REPORT_TARGET_NAME @"target_name"
#define REPORT_PROJECT_ID @"pid"
#define REPORT_INBOX_MESSAGE_TIME @"imt"
#define REPORT_INBOX_COMPLETION_TIME @"rct"
#define REPORT_MESSAGE_ID @"mid"
#define REPORT_STATE_ID @"stid"
#define REPORT_HAS_WRITE_ACCESS @"acg"
#define REPORT_MOBILE_VIEW @"mv"
#define REPORT_LAST_TIME_READ @"lrt"
#define REPORT_REFRESHED @"ref"
#define REPORT_INITIAL_PAGE_INFORMATION @"ipinfo"
#define REPORT_INITIAL_PAGE_ENTRY @"ipe"
#define REPORT_INITIAL_PAGE_NAME @"ipn"
#define REPORT_INITIAL_PAGE_DATA_VALUE @"ipdv"
#define REPORT_INITIAL_PAGE_IS_NUMERIC @"ipisn"
#define REPORT_TARGET_MODIFICATION_TIME @"target_mod_time"
#define REPORT_READ @"r"
#define REPORT_ZOOM @"z"
#define REPORT_X_OFFSET @"x"
#define REPORT_Y_OFFSET @"y"
#define REPORT_LAST_REFRESH_TIMED_OUT @"lrto"
#define PROMPT_ORIGIN @"or"
#define PROMPT_ELEMENT @"e"
#define REPORT_ABBREVIATION @"abbr"
#define REPORT_VIEWMEDIA @"dvm"
#define REPORT_AVAILABLEVIEWMEDIA @"avm"
#define REPORT_ICON_PATH @"icp"
#define SAVED_ANSWER_MAP @"sam"
#define CURRENT_ANSWER_MAP @"cam"
#define MESSAGE_ID_MATCHES_PROMPT @"mamp"
#define ANSWERS_SAVED_TO_MD @"asmd"
#define IS_PERSONALIZED @"isp"
#define REPORT_IS_SUB_EDITABLE @"isSubEditable"
#define IS_PROMPT_EDITABLE @"is_prompt_editable"
#define IS_PROMPT_IN_PROMPT @"is_prompt_in_prompt"
#define REPORT_IS_UNSUBSCRIBABLE @"is_sub_u"
#define PROMPT_QUESTION_CACHE_ID @"promptQuestion"
#define ISERVER_CACHEINFOMAP_CACHE_ID @"iserverCacheInfo"
#define DELTAMAP_CACHE_ID @"deltaMapCache"
#define IS_ALERT @"is_alert"
#define IS_PROMPT_QUESTION_VALID @"is_promptQuestion_Valid"

#define GRID_TYPE 1
#define GRAPH_TYPE 2
#define RWD_TYPE 3

#define PORTRAIT_LAYOUT_ID @"portraitLayoutID"
#define LANDSCAPE_LAYOUT_ID @"landscapeLayoutID"

#define REPORT_FULLY_CACHED @"fullyCached"
#define FULLY_CACHED_TIME @"fcTime"
#define CACHED_BYTES @"cBytes"
#define LAST_LOADED_SLICE @"llSl"
#define LOADING_LAYOUT_ID @"llID"

typedef enum _EnumPrivateAuthenticationMode{
    EnumNoAuthentication = 0,
    EnumPrivateLocalAuthentication = 1,
    EnumPrivateKeychainAuthentication = 2,
    EnumPrivateLocalAuthenticationSharePrint = 3,
    EnumPrivateKeychainAuthenticationSharePrint = 4,
} EnumPrivateAuthenticationMode;

@class MSIProjectInfo, XMLBuilder, MSIObjectInfo, SAXStringHandler, IServerCacheInfo, IServerCacheInfoMap, Prompts, MSTRError;

/**
 * This class is the base class for all documents and reports information
 * It is a representation of a document/report.
 * @see ObjectInfo
 */
@interface MSIReportDisplayInfo : NSObject <MSTRHandler, NSCoding>{
@protected
	NSString* reportName;
	NSString* subscriptionName;
	NSString* subscriptionID;
	NSString* parentSubscriptionID;
	NSString* projectID;
//	NSString* savedPromptAnswers;		//not used
	NSString* currentPromptAnswers;
	NSString* promptInPromptAnswers;
	NSTimeInterval inboxMessageTime;
	NSString* baseMessageID;
	NSInteger stateID;
	NSInteger hasWriteAccess;
	NSInteger mobileView;
	MSIProjectInfo* project;
	NSInteger refreshed;
	NSTimeInterval lastTimeRead;
	NSMutableDictionary* initialPageInfo;
	NSMutableArray* tmpPageInfo;
	NSString* tmpID;
	BOOL tmpIsNumeric;
	NSInteger read;
	CGFloat zoom;
	CGFloat xOffset;
	CGFloat yOffset;
	BOOL lastRefreshTimedOut;
	NSTimeInterval targetModificationTime;
	NSString* currentMessageID; // not persisted by design
	BOOL isAlert;

	//not persisted in cache used for passing the current stateID for a report which has been manipulated
	//only stateID is persisted
	// if the report has not been manipulated then this value is the same as stateID
	NSInteger currentStateIDForManipulatedReport;

	BOOL messageIDMatchesPrompt;
	BOOL answersSavedToMD;
	//BOOL hideCurrentCacheSetID;
	SAXStringHandler* currentAnswerGatherer;	//not used
	BOOL isPersonalized;
	BOOL isEditable;
	BOOL isPromptEditable; 
	BOOL isUnsubscribable;
	BOOL forPreload;  //Indicates that this report is used for cache preloading
	//Indicates that report cache is fully populated
	BOOL fullyCached;
	NSTimeInterval fullyCachedTime;
	NSInteger bytesLoaded;
	BOOL reprompted;
	NSData* repromptQuestionXML;
	BOOL isPromptInPrompt;
	NSInteger loadedSliceCount;
	NSInteger loadingLayoutID;

	//To support different forms of prompt answers
	NSString* promptsAnswerXML;
	NSString* elementsPromptAnswers;
	NSString* valuePromptAnswers;
	NSString* objectsPromptAnswers;
    NSDictionary* datasetPromptAnswers;
	MSIObjectInfo* mainTarget;
	MSIObjectInfo* alertTarget;
	
@private
	int initialLayout;
	BOOL isParsingAlertTarget;
	
}

/**
 * Use this property to get or set the report name
 */
@property (nonatomic, copy) NSString* reportName;

/**
 * Use this property to get or set the subscription name
 */
@property (nonatomic, copy) NSString* subscriptionName;

/**
 * Use this property to get or set the report target name from inbox
 */
@property (nonatomic, copy) NSString* targetName;

/**
 * Use this property to get or set the report/document ID (object ID)
 */
@property (nonatomic, copy) NSString* reportID;

/**
 * Use this property to get or set the subscription ID
 */
@property (nonatomic, copy) NSString* subscriptionID;

/**
 * Use this property to get or set the parent subscription ID
 */
@property (nonatomic, copy) NSString* parentSubscriptionID;

/**
 * Use this property to get or set the report type (object type)
 */
@property NSInteger reportType;

/**
 * Use this property to get or set the report subtype (object subtype)
 */
@property NSInteger reportSubType;

/**
 * Use this property to get or set the object description
 */
@property (nonatomic, copy) NSString* description;

/**
 * Use this property to get or set the project ID
 */
@property (nonatomic, copy) NSString* projectID;

/**
 *
 */
@property (nonatomic, strong) MSIObjectInfo* parentFolder;

/**
 * Use this property to get or set the inbox message time
 */
@property NSTimeInterval inboxMessageTime;

/**
 * Use this property to get or set the base message ID from inbox
 */
@property (nonatomic, copy) NSString* baseMessageID;

/**
 * Use this property to get or set the state ID
 */
@property NSInteger stateID;

/**
 * Use this property to get or set if the report is writable
 */
@property NSInteger hasWriteAccess;

/**
 * Use this property to get or set the mobile view
 */
@property NSInteger mobileView;

/**
 * Use this property to get or set the project info
 */
@property (nonatomic, strong) MSIProjectInfo* project;

/**
 * Use this property to get or set the initial pageby info
 */
@property (nonatomic, strong) NSMutableDictionary* initialPageInfo;

/**
 * Use this property to get or set refresh status
 */
@property NSInteger refreshed;

/**
 * Use this property to get or set the last time read
 */
@property NSTimeInterval lastTimeRead;

/**
 * Use this property to get or set read flag
 */
@property NSInteger read;

/**
 * Use this property to get or set zoom value
 */
@property CGFloat zoom;

/**
 * Use this property to get or set xOffset value
 */
@property CGFloat xOffset;

/**
 * Use this property to get or set yOffset value
 */
@property CGFloat yOffset;

/**
 * Use this property to get or set if last refresh timed out
 */
@property BOOL lastRefreshTimedOut;

/**
 * Use this property to get or set target modification time
 */
@property NSTimeInterval targetModificationTime;

/**
 * Use this property to get or set abbreviation
 */
@property (nonatomic, copy) NSString *abbreviation;

/**
 * Use this property to get or set icon path
 */
@property (nonatomic, copy) NSString *iconPath;

/**
 * Use this property to get or set current prompt answer
 */
@property (nonatomic, copy) NSString* currentPromptAnswers;

/**
 * Use this property to get or set prompt in prompt answer
 */
@property (nonatomic, copy) NSString* promptInPromptAnswers;

/**
 * Use this property to get or set prompt question in XML
 */
@property (nonatomic, copy) NSData* cachedPromptsQuestionXML;

/**
 * Use this property to get or set prompt answer in XML
 */
@property (nonatomic, copy) NSString* promptsAnswerXML;

/**
 * Use this property to get or set elements prompt answers
 */
@property (nonatomic, copy) NSString* elementsPromptAnswers;

/**
 * Use this property to get or set value prompt answers
 */
@property (nonatomic, copy) NSString* valuePromptAnswers;

/**
 * Use this property to get or set object prompt answers
 */
@property (nonatomic, copy) NSString* objectsPromptAnswers;

/**
 * Use this property to get or set dataset prompt answers
 */
@property (nonatomic, copy) NSDictionary* datasetPromptAnswers;

/**
 * Use this property to get or set mainTarget objectinfo
 */
@property (nonatomic, strong) MSIObjectInfo* mainTarget;

/**
 * Use this property to get or set alertTarget objectinfo
 */
@property (nonatomic, strong) MSIObjectInfo* alertTarget;

/**
 * Use this property to get or set if messageID matches prompt
 */
@property BOOL messageIDMatchesPrompt;

/**
 * Use this property to get or set if prompt answer has been saved
 */
@property BOOL answersSavedToMD;

/**
 * Use this property to get or set is personalized
 */
@property BOOL isPersonalized;

/**
 * Use this property to get or set isEditable
 */
@property BOOL isEditable;

/**
 * Use this property to get or set current state ID for manipulated report
 */
@property NSInteger currentStateIDForManipulatedReport;

/**
 * Use this property to get or set is report for preload
 */
@property BOOL forPreload;

/**
 * Use this property to get or set is prompt editable
 */
@property BOOL isPromptEditable;

/**
 * Use this property to get or set is unsubscribable
 */
@property BOOL isUnsubscribable;

/**
 * Use this property to get or set fully cached value
 */
@property BOOL fullyCached;

/**
 * Use this property to get or set fully cached time
 */
@property NSTimeInterval fullyCachedTime;

/**
 * Use this property to get or set bytes loaded
 */
@property NSInteger bytesLoaded;

/**
 * Use this property to get or set reprompted
 */
@property BOOL reprompted;

/**
 * Use this property to get or set reprompt question in XML
 */
@property (nonatomic, copy) NSData* repromptQuestionXML;

/**
 * Use this property to get or set isPromptInPrompt;
 */
@property BOOL isPromptInPrompt;

/**
 * Use this property to get or set loading layoutID
 */
@property NSInteger loadingLayoutID;

/**
 * Use this property to get or set loaded slice count
 */
@property NSInteger loadedSliceCount;

/**
 * Use this property to get or set viewMedia value
 */
@property NSInteger viewMedia;

@property NSInteger availableViewMedia;

/**
 * Use this property to get or set isAlert
 */
@property BOOL isAlert;

@property BOOL promptQuestionValid;
/*
 * Serialize
 *
 * @return Data from serialized XML
 */
-(NSData*)_serialize;

/*
 * Build XML representation for the report display info
 *
 * @param builder XMLBuilder object
 */
-(void)_buildXMLBody:(XMLBuilder*)builder;

/*
 * Return Report type
 *
 * @return Report type
 */
-(NSInteger)type;

/*
 * Set initial pageby elements for ID
 *
 * @param identifier Pageby lookup ID
 * @values Pageby elemetns array
 * @return Visualization style of the report
 */
-(void) setInitialPageForID:(NSString*) identifier values:(NSArray*) values;

/*
 * Get initial pageby elements for ID
 *
 * @param identifier Pageby lookup ID
 * @return Initial pageby elements array
 */
-(NSArray*) getInitialPageForID:(NSString*) identifier;

/*
 * Return report cache set ID (current prompt answer)
 *
 * @return Report cache set ID
 */
-(NSString*) cacheSetID;

/*
 * Get current message ID
 *
 * @return Current message ID
 */
-(NSString*) currentMessageID;

/*
 * Set current message ID
 *
 * @param currMessageID Current message ID
 */
-(void)setCurrentMessageID:(NSString*)currMessageID;

/*
 * Get prompt question XML
 *
 * @return Prompt question XML
 */
-(NSData*) promptQuestionXML;

/*
 * Set prompt question XML
 *
 * @param promptQuestionXML Prompt question XML
 */
-(void) setPromptQuestionXML:(NSData*)promptQuestionXML;

/*
 * Add iserver cache info
 *
 * @param isci Iserver cache info
 * @param cacheID Cache ID to be added
 * @param background If the action is triggered by background downloading
 */
-(void)addIServerCacheInfo:(IServerCacheInfo *)isci withCacheID:(NSString*)cacheID isBackground:(BOOL)background;

/*
 * Return report's iserver cacheinfo map
 *
 * @return Iserver cacheinfo map of the report
 */
-(IServerCacheInfoMap*)getIServerCacheInfoMap;

/*
 * Remove Iserver cache info
 *
 * @param promptAnswerXML Prompt answer XML
 */
-(void)removeIServerCacheInfo:(NSString*)promptAnswerXML;
-(void)removeIServerCacheInfo:(NSString*)promptAnswer withCacheInfoID:(NSString*)cacheInfoID;

/*
 * Return current message ID
 *
 * @return Current message ID
 */
-(NSString*) _currentMessageIDInternal;

/*
 * Valiate prompts answer
 *
 * @param prompts Prompts in the report
 * @param anError Error if validation failed
 * @return If all prompts answers are valid
 */
-(BOOL)validatePrompts:(Prompts*)prompts anError:(MSTRError**)anError;

/**
 * Sets the currentStateIDForManipulation to stateID. 
 * The currentStateIDForManipulation is syched at the start when the report is to be shown
 */
- (void) syncManipulationStateId;

/*
 * Return visualization style from report's abbreviation
 *
 * @param abbreviation Report's abbreviation
 * @return Visualization style of the report
 */
+ (NSString*)visualizationStyle:(NSString*)abbreviation;

/*
 * Compare report name with input instance
 * 
 * @param item Another MSIReportDisplayInfo to be compared
 * @return comparison result
 */
-(NSComparisonResult)compareReportName:(MSIReportDisplayInfo*)item;

/*
 * Get XML representation of certain layout and groupby elements
 * 
 * @param layoutID Layout ID
 * @param groupByElements Groupby Elements
 * @return data from XML using UTF8 encoding
 */
-(NSData*)infoXMLForLayout:(NSInteger)layoutID grouByElements:(NSArray*)groupByElements;

/*
 * Set initial pageby elements
 * 
 * @param pageByElements Array of pageby elements
 * @param pageByUnits Array of pageby units
 */
- (void) setInitialPageByElements:(NSArray*)pageByElements andPageByUnits:(NSArray*) pageByUnits ;

/*
 * @deprecated 
 * use setInitialPageForID:values: instead
 */
- (void) setInitialLayout:(int)layoutID inPortraitMode:(BOOL) isPortrait;

/*
 * Get binary downloadMode accroding to server version
 * 
 * @return EnumBinaryDownloadMode type
 */
-(EnumBinaryDownloadMode)getBinaryDownloadMode;

/*
 * Get private property from the view media
 *
 * @return if doucment /report is private (need touch ID authentication)
 */
-(EnumPrivateAuthenticationMode)isPrivateReport;
-(EnumPrivateAuthenticationMode)isAuthPrintShare;

/*
 * Get the path of the exported excel file for the document
 *
 * @return the path of the exported excel file
 */
- (NSString *)getExcelFilePathAndCreateFolders:(NSString *)pagebyKey;

/*
 * Load prompt question xml from cache file into cachedPromptsQuestionXML property
 */
-(void) loadPromptQuestionXML;

/*
 * Clear the document prompt question cache file
 */
- (void)clearPromptQuestionCache;

@end
