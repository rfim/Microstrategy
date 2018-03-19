//
//  MSTRVCRetriever.h
//  MSTRMobileSDK
//
//  Copyright 2010 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Service.h"
//#import "MSTRError.h"
//#import "ObjectInfoService.h"

@class MSTRError;
@class ObjectInfoService;

/** 
 MSTRVCMode is used by the MSTRVCRetriever to know exactly what kind of view controller is being asked for.
*/
typedef enum {
	/**This mode implies an object such as a folder, report or document.*/
	ObjectVCMode,
	
	/**This mode implies the settings screen.*/	
	SettingsVCMode,
	
	/**This mode implies the status screen.*/	
	StatusVCMode
} MSTRVCMode;


@class MSTRVCRetriever;

/** 
	This is the protocol for the MSTRVCRetriever to communicate with its delegate.
 */
@protocol MSTRVCRetrieverDelegate <NSObject>

/** 
 This method is called when the VCRetriever has fetched a VC
 */
- (void)retriever:(MSTRVCRetriever *)retriever didFetchVC:(UIViewController *)viewController;

/** 
 This method is called when the VCRetriever encountered an Error while fetching the VC
 */
- (void)retriever:(MSTRVCRetriever *)retriever didFailWithError:(MSTRError *)error;

@end



/** 
	The MSTRVCRetriever Class lets you retrieve (fetch) a ViewController.
 
	The View controller you want to fetch may be a MicroStrategy Folder, Report, Document, Settings Screen or Status Screen.
 
	You can use any of the contstructors (init methods) to create an MSTRVCRetriever object, set its delegate and then call its start method.The MSTRVCRetriever object will call its delegate when a view controller is retrieved successfully or in case of an error.
 
	The delegate must conform to the MSTRVCRetrieverDelegate protocol.
	 @see MSTRVCRetrieverDelegate
*/
@interface MSTRVCRetriever : NSObject<ServiceHandler>{
	
	id <MSTRVCRetrieverDelegate> __weak delegate;
	NSString *objectID;
    
    //FIXME: dali, 804648, 11/17/2011: objectInfoService is now a local variable, please contact Derek(Dai Li) if you want to use it.
    ObjectInfoService *objectInfoService;
    NSInteger type;
	MSTRVCMode mode; 
	NSString *reportStyle;
	NSNumber *namedSystemFolderID;
	NSString *projectName;
	NSString *serverName;
	
	NSString *promptsAnswerXML;
	NSString *elementsPromptAnswers;
	NSString *valuePromptAnswers;
	NSString* objectsPromptAnswers;

	NSArray* initialPageByElements;
	NSArray* initialPageByUnits;
	NSInteger initialLayout;
	Boolean portraitMode;
	
	NSString* messageID;
}


/**
 Use this property to get or set the delegate of the MSTRVCRetriever.The delegate must conform to the MSTRVCRetrieverDelegate protocol.
 @see MSTRVCRetrieverDelegate
 */
@property (nonatomic, weak) id <MSTRVCRetrieverDelegate> delegate;

/**
 Use this property to get the ObjectID of the VCRetriever. This property is readonly.
 */
@property (nonatomic, strong, readonly)  NSString* objectID;

/**
 Use this property to get the Mode(MSTRVCMode) of the VCRetriever. This property is readonly.
 */
@property (nonatomic, readonly)  MSTRVCMode mode;

/**
 Use this property to get the type of the Object of the VCRetriever. This property is readonly.
 */
@property (nonatomic, readonly) NSInteger type;

/**
 Use this property to get the NamedSystemFolderID of the VCRetriever. This property is readonly.
 */
@property (nonatomic, strong, readonly)  NSNumber *namedSystemFolderID;

/**
 Use this property to get or set the serverName. If this property is not set, the first server which has a project called projectName will be selected if one exists.
 if the projectName property not set, then the first server will be selected if one exists.
 */
@property (nonatomic, strong)  NSString* serverName;

/**
 Use this property to get or set the projectName. If this property is not set the first project will be selected if one exists 
 in the server matching serverName.
 */
@property (nonatomic, strong)  NSString* projectName;

/**
 Use this property to get or set a particular Report Style when you are fetching a ViewController for a report.
 */
@property (nonatomic, strong)  NSString* reportStyle;

/**
 Use this property to get or set the promptsAnswerXML.
 */
@property (nonatomic, strong)  NSString* promptsAnswerXML;

/**
 Use this property to get or set the elementsPromptAnswers.
 */
@property (nonatomic, strong)  NSString* elementsPromptAnswers;

/**
 Use this property to get or set the valuePromptAnswers.
 */
@property (nonatomic, strong)  NSString* valuePromptAnswers;

/**
 Use this property to get or set the objectsPromptAnswers.
 */
@property (nonatomic, strong)  NSString* objectsPromptAnswers;

/**
 Use this property to get or set the message ID
 */
@property (nonatomic, strong, readonly)  NSString* messageID;

/**
 Call this method when you want to start the fetching operation.
 */
- (void) start;

/**
 Use this constructor to fetch a folder, report or document.
 @param objectID	The MSTR ID of the object you want to fetch
 @param type		The type of the object you want to fetch. Currently supports only ObjectTypeDocumentDefinition,ObjectTypeReportDefinition,ObjectTypeFolder.
 @return			a newly constructed MSTRVCRetriever object
 */
- (id)initWithObjectID:(NSString*)objectID  type:(NSInteger)type;

/**
 Use this constructor if you want to fetch a report or document and has a messageID.
 @param objectID The MSTR ID of the object you want to fetch
 @param type The type of the object you want to fetch(Currently supports only ObjectTypeDocumentDefinition,ObjectTypeReportDefinition,ObjectTypeFolder)
 @param msgID The message ID of this document
 @return a newly constructed MSTRVCRetriever object
 */
- (id)initWithDocumentID:(NSString*)objectID  type:(NSInteger)type messageID:(NSString*)msgID;
/**
 Use this constructor if you want to fetch a report or document and has a messageID.
 @param objectID The MSTR ID of the object you want to fetch
 @param type The type of the object you want to fetch(Currently supports only ObjectTypeDocumentDefinition,ObjectTypeReportDefinition,ObjectTypeFolder)
 @param msgID The message ID of this document
 @param projectName The project name of the document
 @return a newly constructed MSTRVCRetriever object
 */
- (id)initWithDocumentID:(NSString*)objectID  type:(NSInteger)type messageID:(NSString*)msgID projectName:(NSString*)projName;
/**
 Use this constructor to fetch a Settings Screen or Status Screen
 @param mode The mode of fetch operation(MSTRVCMode). You will get an error if you use the ObjectVCMode with this constructor.Use the initWithObjectID:type: method for creating MSTR Objects.
 @return a newly constructed MSTRVCRetriever object
*/ 
- (id)initWithMode:(MSTRVCMode)mode;

/**
 Use this constructor to fetch a folder with its Named System Folder ID
 @param sysFolder A named system fodler ID
 @return a newly constructed MSTRVCRetriever object
 */
- (id)initWithNamedSystemFolderID:(NSNumber*)sysFolder;

/**
 Use this contstructor to create a VCRetriever Object from a URL String. The URL String parameters must follow the MSTR URL API
 @param urlString		The url string that follows the MSTR URL API Conventions.
 @param parseErrorRef	This error reference will be set in case there is an error.
 @return				a newly constructed MSTRVCRetriever object or nil in case of a parsing Error.
 */
- (id)initWithURLString:(NSString*)urlString  errorRef:(NSError**)parseErrorRef;

/**
 Use this method to set intialPageByElements and PageByUnits
 @param pageByElements pageby elements
 @param pageByUnits pageby units
 */
- (void) setInitialPageByElements:(NSArray*)pageByElements andPageByUnits:(NSArray*) pageByUnits;

/**
 Use this method to set initial layout ID and orientation
 @param layoutID Layout ID
 @param isPortrait is portrait orientation
 */
- (void) setInitialLayout:(int)layoutID inPortraitMode:(BOOL)isPortrait;

@end

