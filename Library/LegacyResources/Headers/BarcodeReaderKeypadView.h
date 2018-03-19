//
//  BarcodeReaderKeypadView.h
//  MicroStrategyMobile
//
//  Created by Dai Li on 2/7/13.
//  Copyright (c) 2013 Li, Dai. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "BarcodeReaderView.h"


@interface BarcodeReaderKeypadView: BarcodeReaderView{
	IBOutlet UIButton *keypad1;
	IBOutlet UIButton *keypad2;
	IBOutlet UIButton *keypad3;
	IBOutlet UIButton *keypad4;
	IBOutlet UIButton *keypad5;
	IBOutlet UIButton *keypad6;
	IBOutlet UIButton *keypad7;
	IBOutlet UIButton *keypad8;
	IBOutlet UIButton *keypad9;
	IBOutlet UIButton *keypad0;
	IBOutlet UIButton *enterButton;
	IBOutlet UIView *textBottomView;
	IBOutlet UILabel *textInput;
    IBOutlet UILabel *matchTextView;
	IBOutlet UIButton *deleteButton;
	IBOutlet UIView *keypadBottomView;
	IBOutlet UIView *view;
}

- (IBAction) keypadPressed: (id) sender;
- (IBAction) keypadReleased: (id) sender;
- (IBAction) doneScanning: (id) sender;

@end

