//
//  InputFieldValueDelegate.m
//  MicroStrategyMobile
//
//  Created by Srinivas Singanamalla on 3/17/11.
//  Copyright 2011 MicroStrategy Inc. All rights reserved.
//
#import "DSSIPhoneEnums.h"
#import	"EditControlProperty.h"

@protocol InputFieldValueDelegate

- (BOOL) showByDefault;
- (CGSize) getFrameSize;
- (NSString*) getFontName;
- (CGFloat) getFontSize;
- (NSTextAlignment) getTextAlignment;
- (NSString*) GetStringValue;
/**
	Raw value for number, else gets the string value
 **/
- (NSString*) getValue;

- (NSTextAlignment)getHorizontalAlignmentValue;

- (EnumDSSDataType) getDataType;
- (UIColor*) getTextColor;
- (BOOL) isTypeNumber;
- (BOOL) isNumberCell;

- (EnumControlType) getControlType;

- (ControlProperty*) getControlProperty;

- (void) inputValueDidEndEditing;

-(void) changeCellValue: (NSString*) iValue;

- (void) setNeedsDisplay;

@end
