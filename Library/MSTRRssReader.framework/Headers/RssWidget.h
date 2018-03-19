//
//  RssWidget.h
//  MSTRRssReader
//
//  Created by Nikolay Sazonov on 3/4/11.
//  Copyright 2011 EastBanc Technologies. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "MSIWidgetViewer.h"
#import "RssProtocols.h"
#import "RssMainController.h"


@class MSIModelData;
@class InfoWindow;
@class MSMRssConfig;
@class ViewerDataModel;
@class MSMAttrParser;


@protocol ModelDataProtocol;
@interface MSMRssWidget : MSIWidgetViewer<IWidgetProtocol, MSMRssDataDelegate, MSMRssTableViewSourceDelegate, MSMArticleDelegate, MSMRssMainController, MSMSelectedItems> {

}

@property (nonatomic, strong) id<ModelDataProtocol> mTblData;
@property (nonatomic, strong) InfoWindow * infoWindow;
@property (nonatomic, strong) MSMAttrParser * parser;
//@property (nonatomic, retain) NSString * alertMessage;
@property (nonatomic, unsafe_unretained) id<MSMRssMainController> controller;

-(bool)isPortraitMode;
//- (void)layoutOrPanelChanged;

// IWidgetProtocol
- (id)initViewer:(ViewerDataModel*)ipDataModel withCommanderDelegate:(id<MSICommanderDelegate>)_commander withProps:(NSString*)ipProps;
+ (MSTROrientation)supportedOrientation;
- (void)clearBaseView;
- (void) addViewToBaseView:(UIView*)view;
- (BOOL)supportsFitToScreen;
//

-(BOOL) isDocument;
-(NSString*) DESCIDFilterFromString:(NSString*)str;
- (void) viewSetup;
- (void) initializeSubviews;
-(NSString*) ExtractAttributeFromFormattedString:(NSString*) str;
-(void) showErrorMessage: (NSString*) err_msg;
-(void) hideActivityIndicator;
- (void) checkSelectable;
- (bool)reInitDataModels;

//
- (void)updateFrame:(CGRect)frame;

@end