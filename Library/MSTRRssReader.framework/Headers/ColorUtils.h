//
//  ColorUtils.h
//  FishEyeIPhone
//
//  Created by Svetlana Kopylova on 5/13/10.
//  Copyright 2010 EastBanc Technologies. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>


@interface MSMColorUtils : NSObject 

//values of red, green, blue should be in range (0, 255)
+ (UIColor *) colorFromRed:(int) r green:(int) g blue:(int) b;

+(UIColor *) colorFromRGB:(int) rgbValue;

//returns color or nil if hexString cannot be parsed
+ (UIColor *) colorFromHex:(NSString *) hexString;
//return YES if in string hex number
+(BOOL) isHexInString:(NSString*)str;
+(UIColor *) colorFromStringNumber:(NSString*)str;
+(BOOL) isEqualColor:(UIColor*)leftColor withColor:(UIColor*)rightColor;
+(NSString *) hexFromInteger:(NSString *) str;
@end
