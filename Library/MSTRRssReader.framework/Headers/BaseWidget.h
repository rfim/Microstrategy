//
//  BaseWidget.h
//  MSTRRssReader
//
//  Created by Alexandr Gordienko on 16.02.11.
//  Copyright 2011 EastBanc Technologies. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "TabularDataProtocol.h"
#import "MSTRView.h"
#import "ActionDelegate.h"
#import "InfoWindow.h"

//@class MSITabularDataProtocol;

@interface BaseWidget : MSTRView  {
	id<MSITabularDataProtocol> mTblData;
	id<ActionDelegate> actionDelegate;	
	InfoWindow	*infoWindow;
	
}

@property (nonatomic, retain) id<MSITabularDataProtocol> mTblData;
@property (nonatomic, retain) id<ActionDelegate> actionDelegate;
@property (nonatomic, retain) InfoWindow	*infoWindow;

- (id)initViewer:(id<MSITabularDataProtocol>)ipDataModel frame:(CGRect)frame;
- (void) viewSetup;

@end
