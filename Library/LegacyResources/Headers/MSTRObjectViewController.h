//
//  MSTRObjectViewController.h
//  MSTRMobileSDK
//
//  Copyright 2011 MicroStrategy. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "MSTRVCRetriever.h"

@protocol DocumentControllerInternal;

/**
 This is the protocol for the MSTRObjectViewController to communicate with its delegate.
 */
@protocol MSTRObjectViewControllerDelegate <NSObject>

/**
 Use this method if you want to initialize MSTRView with setViewFrame, setViewOrientation, etc after getting the viewController in the method retriever:didFetchVC.
 @param viewController you want to pass to delegate, here viewController could be IPadFolderController or IPadDataController
 */
- (void)initMSTRView: (UIViewController *)viewController;

@end


/** 
 The MSTRObjectViewController class lets you view an MSTR Object which can be a MSTR Folder,report or a document.
 
 It is in essence a wrapper around MSTRVCRetriever and gives the same functionality as the MSTRVCRetriever but without the hassle of fetching and displaying.
 
 Internally this class uses the MSTRVCRetriever and when the View Controller is retrieved it replaces itself with the fetched view controller.
 */
@interface MSTRObjectViewController : UIViewController <MSTRVCRetrieverDelegate>{
	id <MSTRObjectViewControllerDelegate> __weak delegate;
	UIViewController* vc;
	MSTRVCRetriever* vcRetriever;
	id<DocumentControllerInternal>	mDataController;
	NSString* objectID;
	NSInteger type;
	NSString *reportStyle;
	MSTRVCMode mode; 
	NSNumber *namedSystemFolderID;
	NSString *projectName;
	NSString *serverName;
	
	NSString *promptsAnswerXML;
	NSString *elementsPromptAnswers;
	NSString *valuePromptAnswers;
	NSString* objectsPromptAnswers;
	
	BOOL load;
	BOOL viewLoaded;
	
	NSError* loadError;
}

/**
 Use this property to get or set the delegate of the MSTRObjectViewController.The delegate must conform to the MSTRObjectViewControllerDelegate protocol.
 */
@property (nonatomic, weak) id <MSTRObjectViewControllerDelegate> delegate;

/**
 Use this property to get or set the MSTR ID of the object you want to display
 */
@property (nonatomic, strong)  NSString* objectID;

/**
 Use this property to get or set the mode of display operation(MSTRVCMode)
 */
@property (nonatomic)  MSTRVCMode mode;

/**
 Use this property to get or set the type of the MSTRObjectViewController.The type can be (ObjectTypeDocumentDefinition,ObjectTypeReportDefinition,ObjectTypeFolder)
 */
@property (nonatomic) NSInteger type ;

/**
 Use this property to get or set a particular Report Style for a report
 */
@property (nonatomic, strong)  NSString* reportStyle;

/**
 Use this property to get or set the NamedSystemFolderID of folder you want to display.This property is readonly
 */
@property (nonatomic, strong)  NSNumber *namedSystemFolderID;

/**
 Use this property to get or set the Preferences.xml file you want to use to ini the app.
 */
@property (nonatomic, copy)  NSString *preferencesXmlFile;

/**
 Use this property to get or set the serverName.If this property is not set the first server which has a project called projectName will be selected if one exists.
 if the projectName property not set then the first server will be selected if one exists.
 */
@property (nonatomic, strong)  NSString* serverName;

/**
 Use this property to get or set the projectName.If this property is not set the first project will be selected if one exists.
 */
@property (nonatomic, strong)  NSString* projectName;

/**
 Use this property to get the View Controller object currently in focus on the MSTRObjectViewController.This property is readonly
*/
@property (nonatomic, readonly) UIViewController* vc;

/**
 Use this property to get or set the promptsAnswerXML
 */
@property (nonatomic, strong)  NSString* promptsAnswerXML;

/**
 Use this property to get or set the elementsPromptAnswers
 */
@property (nonatomic, strong)  NSString* elementsPromptAnswers;

/**
 Use this property to get or set the valuePromptAnswers
 */
@property (nonatomic, strong)  NSString* valuePromptAnswers;

/**
 Use this property to get or set the objectsPromptAnswers
 */
@property (nonatomic, strong)  NSString* objectsPromptAnswers;


/**
 Use this constructor if you want to display a folder,report or document.
 @param objectID The MSTR ID of the object you want to display
 @param type The type of the object you want to display(Currently supports only ObjectTypeDocumentDefinition,ObjectTypeReportDefinition,ObjectTypeFolder)
 @return a newly constructed MSTRObjectViewController object
 */
- (id)initWithObjectID:(NSString*)objectID  type:(NSInteger)type;

/**
 Use this constructor to display a Settings Screen or Status Screen
 @param mode The mode of display operation(MSTRVCMode).You will get an error if you use the ObjectVCMode with this constructor.Use the initWithObjectID:type: method for creating MSTR Objects.
 @return a newly constructed MSTRObjectViewController object
 */ 
- (id)initWithMode:(MSTRVCMode)mode;

/**
 Use this constructor to display a folder with its Named System Folder ID
 @param sysFolder A named system fodler ID
 @return a newly constructed MSTRObjectViewController object
 */
- (id)initWithNamedSystemFolderID:(NSNumber*)sysFolder;

/**
 Use this constructor to init app with Preferences.xml file
 @param xmlFile the xml file
 @return a newly constructed MSTRObjectViewController object
 */
- (id)initWithPreferencesXmlFile:(NSString*)xmlFile;

/**
 Use this method if you want to display a folder,report or document.
 @param objectID The MSTR ID of the object you want to display
 @param type The type of the object you want to display(Currently supports only ObjectTypeDocumentDefinition,ObjectTypeReportDefinition,ObjectTypeFolder)
*/
- (void)loadWithObjectID:(NSString*)objectID  type:(NSInteger)type;

/**
 Use this method to display a Settings Screen or Status Screen
 @param mode The mode of display operation(MSTRVCMode).You will get an error if you use the ObjectVCMode with this method.Use the loadWithObjectID:type: method for creating MSTR Objects.
 */ 
- (void)loadWithMode:(MSTRVCMode)mode;

/**
 Use this method to display a folder with its Named System Folder ID
 @param sysFolder A named system fodler ID
 */
- (void)loadWithNamedSystemFolderID:(NSNumber*)sysFolder;




#pragma mark Actions exposed for SDK users and handled by our IPadDataController if we have one

/**
 Invoke reprompt action in the data controller
 */
- (void) filterAction;

/**
 Invoke email action in the data controller
 */
- (void) emailAction;

/**
 Invoke display mode change action in the data controller
 */
- (void) modeSwitchAction;

/**
 Invoke maximize document action in the data controller
 */
- (void) maximizeAction;

/**
 Invoke list pending transaction action in the data controller
 */
- (void) transactionListAction;

/**
 Invoke back action in the data controller
 */
- (void) backAction;

/**
 Invoke refresh action in the data controller
 */
- (void) refreshAction;

/**
 Invoke logout action in the data controller
 */
- (void) logoutAction;

/**
 Check if navigation bar is suppressed
 @return If we should display navigation bar
 */
- (BOOL) isNavToolbarSuppressed;

/**
 Set if navigation bar should be suppressed
 @param suppressNavToolbar If we should hid the navigation bar
 */
- (void) setNavToolbarSuppressed: (BOOL) suppressNavToolbar;
#pragma mark -


@end // MSTRObjectViewController





/**
 This class extends UINavigation Controller to extend its functionality for setting the root view controller
 */
@interface MSTRNavigationController :UINavigationController {
	UIViewController *fakeRootViewController;
}
@end



