//
//  BarcodeReaderScannerView.h
//  MicroStrategyMobile
//
//  Created by Li, Dai on 2/1/13.
//  Copyright (c) 2013 Li, Dai. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "BarcodeReaderView.h"

#import "BarcodeReaderScannerViewI.h"

@interface BarcodeReaderScannerView : BarcodeReaderView <BarcodeReaderScannerViewI>{

}

@property (nonatomic, strong) NSDate *lastScanDate;
@property (nonatomic, strong) NSString *lastScanBarcode;

- (void)playBeep;

@end



