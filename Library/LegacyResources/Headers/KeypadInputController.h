//
//  KeypadInputController.h
//  BarCodePOC
//
//  Created by Jose Galvez Elias on 4/15/10.
//  Copyright 2010 MicroStrategy, Inc. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "BarCodeCB.h"

@class KeypadInputController;

@protocol KeypadInputControllerDelegate <UINavigationControllerDelegate>
- (void) keypadInputController:(KeypadInputController *)self didfinishScanning:(BarCodeCB *)result;
@end

@interface KeypadInputController : UIViewController {
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
	IBOutlet UIButton *clearButton;
	IBOutlet UIButton *enterButton;
	IBOutlet UIButton *enterButtonKeypad;
	IBOutlet UIButton *switchButton;
	IBOutlet UIButton *cancelScan;
	IBOutlet UILabel *textInput;
	IBOutlet UIToolbar *toolBar;
	UIButton *cancelButton;
	IBOutlet UIButton *deleteButton;
	BOOL showCancel;
	id<KeypadInputControllerDelegate> __weak delegate;
	NSString *initialValue;
}

@property(nonatomic,weak) id<KeypadInputControllerDelegate> delegate;
@property(nonatomic, copy) NSString *initialValue;
@property(nonatomic) BOOL showCancel;

- (IBAction) keypadPressed: (id) sender;
- (IBAction) keypadReleased: (id) sender;
- (IBAction) doneScanning: (id) sender;
- (IBAction) cancelScan:(id) sender;

@end
