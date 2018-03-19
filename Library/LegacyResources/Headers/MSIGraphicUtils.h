//
//  MSIGraphicUtils.h
//  MSTRMobile
//
//  Created by Amirali Charania on 4/26/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface MSIGraphicUtils : NSObject {

}

+(CGPathRef)newRoundedRectPathForBounds:(CGRect)rrect radius:(CGFloat)radius strokeWidth:(CGFloat)strokeWidth;

/**
 Returns the UIColor object pertaining to the colorCode, for some reason if the colorCode cannot be decoded, 
 returns a supplied defColor
 The input colorCode could be
 a.	@"-1"		(transparent)
 b.	@"af345a" (hexcode)
 c.	@"af345a:0.5" (hexcode:alpha float value) The alpha value between 0 and 1.
 */
+(UIColor*)colorForColorCode:(NSString*)colorCode defaultColor:(UIColor *)defColor; 

/**
	Returns the UIColor object. 
	The input colorCode could be
	a.	@"-1"		(transparent)
	b.	@"af345a" (hexcode)
	c.	@"af345a:0.5" (hexcode:alpha float value) The alpha value between 0 and 1.
	If the input is @"-1", returns a [UIColor clearColor]. If the input is anything else returns a nil.
 **/
+(UIColor*)colorForColorCode:(NSString*)colorCode;

/**
	Returns a hex color code. Converts the input web color code and returns a corresponding hex color code.
	Returns an empty string if the webColor is not valid.
 **/
+ (NSString*) mstrWebColorTohexColorCode:(NSString*)webColor;

/**
 Returns the new image of the view. It is responsibility of the caller to release the view.
 This is useful if you need a snapshot of the current view while showing the transition effects.
 **/
+ (UIView*) newImageViewOfView:(UIView*)view;

+ (UIImage*)imageOfView:(UIView*)view;

/**
 Returns color components array from UIColor object.
 Returns a nil if the color is nil, else a 4 element array.
 [0] red, [1] blue, [2] green and [3] alpha.
 The r, g, b are from 0.0 and 1.0 (inclusive)
 **/
+ (const CGFloat*) getColorComponentsFromUIColor:(UIColor*) color;

/**
 Returns the unicode symbol string for the given code.
 */
+(NSString*) getQuickSymbolUnicode:(NSString*) symbol;

// The conversion between UIColor object and 10based Color RGB representation
+(UIColor*)getColorForColorDecimal:(NSInteger)colorDecimal defaultColor:(UIColor*)defColor;
+(NSInteger)getColorDecimalForColor:(UIColor*)color;
+(UIImageView*)imageViewWithImage:(UIImage*)image constrainedByframe:(CGRect)frame;
+(CGSize)imageSizeWithAspectRatioForImageSize:(CGSize)imageSize inFrame:(CGSize)frame;

@end
