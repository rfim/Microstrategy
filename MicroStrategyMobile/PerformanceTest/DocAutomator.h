//
//  DocAutomator.h
//  MicroStrategyMobile
//
//  Created by Wenting on 11/7/14.
//  Copyright (c) 2014 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "FolderItemsListView.h"
#import "FolderViewDataSource.h"
#import "FolderController.h"
#import "FolderModel.h"
#import "FolderItem.h"
#import "FolderViewConstants.h"
#import "UIColor+Getters.h"
#import "MediaManager.h"
#import "PDFDocumentController.h"
#import "AudioPlayer.h"
#import "MSIGeneric.h"
#import "FolderItemWebFile.h"
#import "MSIPreferencesStore.h"
#import "BooleanPreference.h"
#import "MSIDevicePreferences.h"
#import "MIPromptSummaryScreen.h"
#import "PromptHelper.h"
#import "Prompts.h"

@class IPadDataControllerImpl;


@interface DocAutomator : NSObject


@property (nonatomic,readwrite) bool working;
@property (retain, nonatomic, readwrite)FolderItemsListView* folderItemsListView;
@property (retain, nonatomic, readwrite)UITableView* tableView;
@property (retain, nonatomic, readwrite)NSIndexPath* path;
@property (nonatomic, readwrite) bool entering;
@property (nonatomic, readwrite) bool closing;
@property (nonatomic, readwrite) bool backing;
@property (nonatomic, readwrite)NSString *docName;


+ (DocAutomator*)getInstance;

- (id) init;

- (void) start;

- (void) didEnteredFolderItem:(NSIndexPath*)indexPath withFolderItemsListView:(FolderItemsListView*) folderView tableView:(UITableView*)tableView;

- (void) folderBackToUpperLevel;

- (void) backFolderDone;

- (void) executeDocument:(NSIndexPath *)indexPath;

- (void) closeButtonActionDone;

- (void) setIPadDataControllerImpl:(IPadDataControllerImpl*) controller;

- (void)setPDFDocumentController: (PDFDocumentController *)controller;

- (void) setMediaWebController: (MediaWebController *)controller;

- (void)setPromptSummaryScreen: (MIPromptSummaryScreen *)promptSummaryScreen;

+ (void) exchangeMethods;

- (NSIndexPath *)generateValidIndexPath: (int)row;

- (void)folderBackToUpperLevel;

@end
