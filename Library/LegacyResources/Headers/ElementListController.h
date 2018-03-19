//
//  ElementListController.h
//  MSTRMobile
//
//  Created by Amirali Charania on 10/16/09.
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <CoreLocation/CoreLocation.h>
#import "Service.h"
#import "BarcodeScannerController.h"
#import <MapKit/MapKit.h>

#define SEARCH_BAR_DEFAULT_WIDTH 240
#define SEARCH_BAR_SEARCH_MODE_WIDTH_DIFF 60

@class PromptHelper, RoundedRectView, DataExplorerServiceResponse, ElementSelectionsController, ObjectInfo;

/**
 This is used for handling the UI of an Element Prompt.
 */
@interface ElementListController : UITableViewController <UISearchBarDelegate, CLLocationManagerDelegate, MKReverseGeocoderDelegate, BarcodeScannerControllerDelegate, KeypadInputControllerDelegate> {
	int Height;
	// for search by current location.
	CLLocationManager *locationManager;
	UIToolbar* curLocToolbar;
	UIBarButtonItem* curLocButton;
	BOOL searchByCurLoc;
	ObjectInfo* promptAttribute;
	NSString* shortFilterXML;
	UIBarButtonItem *barDoneButton;
	
	NSMutableArray* selectedElements;
	
	NSMutableDictionary* currentElementsByPage;
	NSInteger currentTotalElements;
	NSInteger currentMaxPages;
	
	NSMutableDictionary* elementsByPage;
	NSInteger totalElements;
	NSMutableArray* pageNumber;
	NSInteger maxPages;
	
	NSArray* suggestedAnswers;
	NSString* dataSourcesXML;
	
	NSInteger elementsPerPage;
	
	PromptHelper* promptHelper;
	ElementSelectionsController *parentController;
	
	UISearchBar* searchBar;
	UIToolbar *searchBarToolbar;
	NSMutableArray *searchBarItemsInSearchMode;
	NSMutableArray *searchBarItemsInDefaultMode;
	BOOL searchResults;

	NSMutableArray* currentlyLoading;
	RoundedRectView* updatingView;
	
	BOOL readdSearchControls;
	
	BOOL disableLoading;
	BOOL allowSearch;
	BOOL currentlySearching;
	
	NSIndexPath* visibleCell;
	CGPoint offset;
	
	//id<Service> service;
	NSMutableArray *currentServices;
	
	BOOL multiSelect;
	BOOL searchRequired;
	NSMutableDictionary *searchResultsCache;
	


	BOOL isInDisplaySelectedMode;
	BOOL isInDisplayNothingMode;
	NSMutableArray *selElementsCopy;
	UIImage *selectedImage;
	UIImage *unselectedImage;
	NSString *barcodeFormId;
	BOOL inBarcodeSearchMode;
	BOOL lastSearchWasBarcode;
	
	NSString *geoMapping;
	NSString *geoAttributeId;
	MKReverseGeocoder *reverseGeocoder;
	BOOL inGeoSeachMode;
	UILabel* selectionCountLabel;
	CGRect selectionCountLabelFrame;
	UITableViewCell* preSelectedCell;
	BOOL receivedResult;
	BOOL titleBarHidden;
}

@property (nonatomic, copy) NSMutableArray* selectedElements;
@property (nonatomic, retain) NSMutableDictionary* currentElementsByPage;
@property (nonatomic, retain) NSMutableDictionary* elementsByPage;
@property (nonatomic, retain) NSArray* suggestedAnswers;

@property (nonatomic, assign) PromptHelper* promptHelper;

@property (nonatomic, retain) UISearchBar* searchBar;

// for search by current location.
//@property (nonatomic, retain) UIToolbar* curLocToolbar;
//@property (nonatomic, retain) UIBarButtonItem* curLocButton;
@property (nonatomic, retain) ObjectInfo* promptAttribute;
@property (nonatomic, retain) NSString* shortFilterXML;

@property (nonatomic, retain) NSMutableArray* pageNumber;

@property (nonatomic, retain) UITableViewCell* preSelectedCell;
@property BOOL multiSelect;
@property BOOL searchRequired;

@property (nonatomic, retain) NSMutableArray *selElementsCopy;
@property (nonatomic, assign) int Height;
@property (nonatomic, assign) ElementSelectionsController *parentController;
@property (nonatomic, retain) UIImage *selectedImage;
@property (nonatomic, retain) UIImage *unselectedImage;
@property (nonatomic, copy) NSString *barcodeFormId;

@property (nonatomic, copy) NSString* geoMapping;
@property (nonatomic, copy) NSString* geoAttributeId;

@property (nonatomic, copy) NSString* dataSourcesXML;

// for search by current location.
-(void)handleSearchWithCurrentLocation:(id)sender;
-(void)handleSearchField:(id)sender;
-(void)doneClicked;
- (int) getHeight;
-(void)performSearch:(NSString *)searchText;
-(DataExplorerServiceResponse *)getSearchResultsFromCache:(NSString *)searchText;
-(void)createSelectedCopy;
-(void)hideTitleBar;
-(void)restoreTitleBar;
-(void)cancelRunningServices;

@end
