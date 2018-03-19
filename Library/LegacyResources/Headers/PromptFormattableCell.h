//
//  PromptFormattableCell.h
//  MSTRMobile
//
//  Created by Firat Alpergin on 3/24/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "PromptBaseCell.h"

@interface PromptFormattableCell : PromptBaseCell {
	NSString *positiveFormat;
	NSString *negativeFormat;
	UIColor *negativeColor;
}

@property (nonatomic, copy) NSString *positiveFormat;
@property (nonatomic, copy) NSString *negativeFormat;
@property (nonatomic, retain) UIColor *negativeColor;

-(void)setFormattingValuesFromProperties:(NSMutableDictionary *)promptProperties;
-(void)replaceCurrencySymbolsFromProperties:(NSMutableDictionary *)promptProperties;
-(NSString *)formatValue:(NSNumber*)value;
-(void)setNegativeColor;

@end
