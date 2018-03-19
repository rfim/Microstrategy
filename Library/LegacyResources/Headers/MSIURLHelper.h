//
//  URLHelper.h
//  MSTRMobile
//
//  Created by Kevin Maurer on 12/16/09.
//  Copyright 2009 MicroStrategy. All rights reserved.
//

/** 	
 *	Class renamed from DrillHelper to URLHelper
 */

#import <Foundation/Foundation.h>
#import "MSTRHandler.h"

//DL;10/16/2012;668506;support document refresh
typedef enum _RefreshDocumentEventOption{
    RefreshDocumentEventOptionAgainstIServerCache,         //retrieve data from the latest valid Intelligence Server cache
    RefreshDocumentEventOptionAgainstWarehouse,          //re-run against the warehouse
    RefreshDocumentEventOptionWebServerSetting           //refresh accoding to web server setting (TODO: not supported yet)
} RefreshDocumentEventOption;

@class DrillService, RegularDrillService, LinkedDrillService, XMLBuilder, MSIReportDisplayInfo, MIActionSheet;

@interface MSIURLHelper : NSObject <MSTRHandler>{
@private
	
	NSString* urlString;
	int urlType;
	NSString* currentAxis;
	NSString* currentDepth;
	NSString* currentOrdinal;
	NSDictionary* elementIndices;
	DrillService* drillService;
	BOOL axisFound;
	BOOL depthFound;
	BOOL elementFound;
	BOOL inElem;
	XMLBuilder* promptAnswerBuilder;
	NSInteger defaultAnswerMode;
	NSMutableArray* answerList;
	NSMutableDictionary* attributeIDDictionary;
	NSMutableDictionary* elementInfoDictionary;
	NSString* currentIndex;
	NSArray* currentLookupValue;
	NSString* attributeToLoad;
	NSString* elementToLoad;
	NSInteger styleToLoad;
	NSString* panelName;
	NSString* panelStackName;
    NSString* panelStackKey;
    NSString* panelKey;	
    NSString* unitKey;	
    NSString* visibility;
	NSString* masterPanelState;	
	BOOL multiSelect;
	BOOL searchRequired;
	BOOL isSimple;
	NSURL* unknownTypeURL;
	NSMutableDictionary* urlParameters;
    NSMutableArray* urlTypes;
	MSIReportDisplayInfo* srcReport;
	BOOL isProjectRestore;	
	// the below two do not retain
	RegularDrillService* __weak regDrillService;
	LinkedDrillService* __weak linkedDrillService;
	
	//url is of type mstr://
	BOOL schemeIsCFBundleURLScheme;
	
	//for PDF link
	NSString * originURL;
	
	//for ActionSheet
	MIActionSheet* actionSheet;
    
    //DL;07/16/2012;342570;link drill event following by multiple manipulation events
    BOOL ignoreLinkDrillEvent;
    
    RefreshDocumentEventOption refreshDocumentEventOption;
}

-(MSIURLHelper*) initWithUrl:(NSURL*)url srcReport:(MSIReportDisplayInfo*)_srtReport;
-(MSIURLHelper*) initWithUrlString:(NSString*)urlString srcReport:(MSIReportDisplayInfo*)_srcReport;

//DL;07/16/2012;342570;link drill event following by multiple manipulation events
-(MSIURLHelper*) initWithUrlString:(NSString*)urlString srcReport:(MSIReportDisplayInfo*)_srcReport ignoreLinkDrillEvent:(BOOL)ignoreLinkDrillEvent;

-(NSInteger) urlType;
-(NSInteger) displayMode;
-(void) loadCatalogXML:(NSString*)xml;
-(DrillService*) drillService;
-(NSInteger) targetType;
-(NSString*) projectID;
- (void) updatePromptAnswerMap:(NSMutableDictionary*) attributeDict elementsPromptAnswers:(NSString*)elementsPromptAnswers;

@property (strong, readonly) NSString* urlString;
@property (nonatomic, copy) NSString* attributeToLoad;
@property (nonatomic, copy) NSString* elementToLoad;
@property NSInteger styleToLoad;
@property BOOL multiSelect;
@property BOOL searchRequired;
@property (nonatomic, strong) NSURL* unknownTypeURL;
@property BOOL isSimple;
@property (nonatomic, readonly) NSDictionary* urlParameters;
@property (nonatomic, readonly) NSArray* urlTypes;
@property BOOL schemeIsCFBundleURLScheme;
@property (nonatomic, copy) NSString* panelName;
@property (nonatomic, copy) NSString* panelStackName;
@property (nonatomic, copy) NSString* panelStackKey;
@property (nonatomic, copy) NSString* panelKey;
@property (nonatomic, copy) NSString* unitKey;
@property (nonatomic, copy) NSString* visibility;
@property (nonatomic, copy) NSString* masterPanelState;
@property (nonatomic, strong) NSString* originURL;
@property (strong, readonly) MIActionSheet* actionSheet;
@property (readonly) RefreshDocumentEventOption refreshDocumentEventOption;
@property (nonatomic, assign) BOOL adHocURLNavigation;  // this property is used for Link Drill SDK support
@property BOOL isProjectRestore;
@property int pdfMode;    // 0:default value, 1: print target document
@property (nonatomic, copy) NSString* selectorKey;
@property (nonatomic, copy) NSString* selectorName;
@property (nonatomic, copy) NSString* selectorIndice;

@end

enum _URLType
{
	URLTypeUnknown          = -1,
    URLTypeRegularDrill     = 0,
    URLTypeLinkedDrill      = 1,
    URLTypeGroupBy          = 2,
    URLTypePrompt           = 3,
    URLTypeEvent            = 4,
    URLTypeInfoWindow       = 5,
    URLTypeEmail            = 6,
    URLTypeHTTP             = 7,
    URLTypeActionSheet      = 8,
    URLTypePanelSelection   = 9,
    URLTypeUnitVisibility   = 10,
    URLTypeMasterPanel      = 11,
    URLTypeMultiEvent       = 12,
    URLTypeFile             = 13,
    URLTypeInternal         = 14,
    URLTypeRefreshDocument  = 15,
    URLTypeSwitchLayout     = 16,
    URLTypePendingTransactionQueue = 17,
    URLTypeTel              = 18,
    URLTypeSMS              = 19,
    URLTypeCustomEvent      = 20,
    URLTypeSelection        = 21
};
