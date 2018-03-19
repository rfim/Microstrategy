//
//  PromptBarcodeCell.h
//  MSTRMobile
//
//  Created by Firat Alpergin on 3/18/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "BarcodeScannerController.h"
#import "PromptBaseCell.h"
#import "KeypadInputController.h"

#define IMAGE_WIDTH 31
#define BARCODE_MIDDLE_PADDING 5

//@class PromptSummaryScreen;
@class MIPromptSummaryScreenSV;

@interface PromptBarcodeCell : PromptBaseCell <BarcodeScannerControllerDelegate, KeypadInputControllerDelegate> {
	UILabel *valueLabel;
	UIImageView *barcodeImageView;
	
	//PromptSummaryScreen *parent;
	//MIPromptSummaryScreen *parent;
	MIPromptSummaryScreenSV *parent;
}

//@property (nonatomic, assign) PromptSummaryScreen *parent;
@property (nonatomic, assign) MIPromptSummaryScreenSV *parent;

+(PromptBarcodeCell*)newCellForPrompt:(Prompt*)_prompt;
-(void)displayCamera;

@end
