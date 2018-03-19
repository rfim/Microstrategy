//
//  BarcodeReaderView.h
//  MicroStrategyMobile
//
//  Created by Li, Dai on 2/4/13.
//  Copyright (c) 2013 Li, Dai. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Enums.h"
#import "MSIAlertView.h"

@class BarcodeReaderView;
@protocol BarcodeReaderViewDelegate;

@protocol BarcodeReaderViewDelegate <NSObject>

-(void) barcodeReaderViewDidScanSuccess:(BarcodeReaderView*)barcodeReaderView barcode:(NSString*)barcode;

@end

@interface BarcodeReaderView : UIView

+ (NSInteger) iOSMainVersion;

@property(nonatomic, weak) id<BarcodeReaderViewDelegate> delegate;
@property (nonatomic) ThemeType themeType;
@property (nonatomic, copy) UIColor* accentColor;

-(void) displayMatchText:(NSString*)matchText;

-(void) reset;

-(void) enableUserInteraction:(BOOL)enabled;

@end



@interface DefaultNoMatchBarcodeHandler : NSObject <MSIAlertViewDelegate>

-(id) initWithBarcodeReaderView:(BarcodeReaderView*) barcodeReaderView;

-(void) displayAlert;

-(void) dismissAlertEnforce;

@end