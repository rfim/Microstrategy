//
//  BarcodeReaderCompView.h
//  MicroStrategyMobile
//
//  Created by Li, Dai on 2/6/13.
//  Copyright (c) 2013 Li, Dai. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BarcodeReaderView.h"

@protocol BarcodeReaderCompViewDelegate <BarcodeReaderViewDelegate>

@optional

-(void) barcodeScannerDidShow;

-(void) barcodeKeypadDidShow;

@end

@interface BarcodeReaderCompView : BarcodeReaderView

@property (readonly, strong) UISegmentedControl *barcodeToggle;

-(void) viewWillAppear;

-(void) viewWillDisappear;

-(void) viewDidAppear;

-(void) repWillRotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation duration:(NSTimeInterval)duration;


@end