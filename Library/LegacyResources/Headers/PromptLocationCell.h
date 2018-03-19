//
//  PromptLocationCell.h
//  MSTRMobile
//
//  Created by Firat Alpergin on 4/25/10.
//  Copyright 2010 MicroStrategy Incorporated. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "PromptBaseCell.h"
#import <CoreLocation/CoreLocation.h>
#import <Generic.h>

#define BUTTON_WIDTH 28
#define BUTTON_HEIGHT 28

@class ConstantPrompt, PromptSummaryScreen;

@interface PromptLocationCell : PromptBaseCell <CLLocationManagerDelegate> {
	CLLocationManager *locationManager;
	UILabel *locationLabel;
	PromptSummaryScreen *parent;
	
	UIButton *locationButton;
	UIImage *selImgLight;
	UIImage *selImgDark;
}

@property (nonatomic, assign) PromptSummaryScreen *parent;

+(PromptLocationCell *)newCellForPrompt:(ConstantPrompt *)_prompt;

@end
