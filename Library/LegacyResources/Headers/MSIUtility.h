//
//  MSIUtility.h
//  IPadDashboardViewer
//
//  Copyright 2010 MicroStrategy, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <string>


// Add by wenzhou, 05/19/2015. For clean warning of PerformSelectorLeak
#define SuppressPerformSelectorLeakWarning(Stuff) \
do { \
_Pragma("clang diagnostic push") \
_Pragma("clang diagnostic ignored \"-Warc-performSelector-leaks\"") \
Stuff; \
_Pragma("clang diagnostic pop") \
} while (0)

@class RootViewerContainer;


/**
 A singleton with a variety of class methods for handling colors, strings, device info, etc.
 @see StringUtils, Generic
 */
@interface MSIUtility : NSObject {
	
}


/**
 * Map a string value to its corresponding RGB representation.
 * @param ipString, the string to be mapped. This one should NOT be null.
 * @param opRed, the red component value in the mapped RGB result.
 * @param opGreen, the green component value in the mapped RGB result.
 * @param opBlue, the blue component value in the mapped RGB result.
 * @return TRUE means a successful conversion and FALSE otherwise.
 */
+ (bool)stringToRGB:(NSString*)ipString withRed:(float*)opRed andGreen:(float*)opGreen andBlue:(float*)opBlue;

+ (int)hexStringToInt:(NSString*)ipString;

/**
 * Get another integer repsenting the same color from order 'BGR' to 'RGB'
 * @param input iValue, represent by integer with 'BGR' order
 * @return the color represented by integer in 'RGB' order
 */
+ (int) intFromBGRToRGB: (int)iValue;

/**
 * Returns components of a given color
 * @param input iValue, represent by integer in 'BGR' order
 * @param output opRed, red componet of color 'iValue'
 * @param output opGreen, green componet of color 'iValue'
 * @param output opBlue, blue componet of color 'iValue'
 */
+ (void) IntToRGB: (int)iValue withRed:(CGFloat*)opRed andGreen:(CGFloat*)opGreen andBlue:(CGFloat*)opBlue;

/**
 * Reture UIColor instance from a integer save in 'RGB' order
 * @param iValue, integer representing a color saved in 'RGB' order
 * @return UIColor instance.
 */
+ (UIColor*) colorFromInteger:(NSInteger)iValue;

/**
 * Please note the color info is saved as BGR instead of RGB in the grid and CE
 * Returns UIColor instance from a integer save in 'RGB' order
 * @param iValue, integer representing a color
 * @param iIsRGBReversed, if iIsRGBReversed is TRUE, indicating color is saved in 
 * order 'BGR' in iValue
 * @return UIColor instance.
 */
+ (UIColor*) colorFromInteger:(NSInteger)iValue rgbReversed:(BOOL)iIsRGBReversed;

/**
 * To layout the text under specified font type and font size.
 * @param ipString, the text to layout. 
 * @param iFontSize, to denote how big the font is.
 * @param ipFontName, to denote which font is chosen to render the font. 
 * @retutn the size (bounding rectangle) of the text.
 */
+ (CGSize)GetMeasuredTextSize:(const char*)ipString withFontSize:(float)iFontSize withFontName:(const char*)ipFontName;

/**
 * Get the root view of the view hierarchy that the specific view is in.
 *
 * @param iView the specific view of the view hierarchy; nil is allowed.
 * @return the root view of the view hierarchy that the specific view is in. If iView is nil, return nil; if iView is the root view, return itself.
 */
+ (UIView*)getRootViewer:(UIView*)iView;

/**
 * Compare two float numbers. If the difference between two numbers is inside the accuracy of float number (FLT_EPSILON), they are treated as equal.
 *
 * @param iLeft the first number to be compared.
 * @param iRight the second number to be comared.
 * @return 1 if iLeft is bigger than iRight; -1 if iLeft is smaller than iRight; 0 if equal.
 */
+ (int)compCGFloat:(CGFloat)iLeft right:(CGFloat)iRight;

/**
 * Get text width by set the string to a UILabel and fitToSize.
 * @param input ipString the NSString to measure
 * @param input iFontSize font size used to display the ipString
 * @param input ipFontName font name used to get the font which is used to display the text.
 * @return the width of text with given font and string
 */
+(CGFloat) GetTextSizeByUILabel:(NSString*)ipString withFontSize:(float)iFontSize withFontName:(NSString*)ipFontName;

/**
 * Convert R, G, B values in RGB color space to H, S, V values in HSV color space. The parameters iR, iG, iB, *opH, *opS, *opV are all in [0, 1].
 *
 * @param iR input R value in RGB color space.
 * @param iG input G value in RGB color space.
 * @param iB input B value in RGB color space.
 * @param opH output H value in HSV color space.
 * @param opS output S value in HSV color space.
 * @param opV output V value in HSV color space.
 */
+ (void)rgbToHsv:(CGFloat)iR withG:(CGFloat)iG andB:(CGFloat)iB andH:(CGFloat*)opH andS:(CGFloat*)opS andV:(CGFloat*)opV;

/**
 * Convert H, S, V values in HSV color space to R, G, B values in RGB color space. The parameters iH, iS, iV, *opR, *opG, *opB are all in [0, 1].
 *
 * @param iH input H value in HSV color space.
 * @param iS input S value in HSV color space.
 * @param iV input V value in HSV color space.
 * @param opR output R value in RGB color space.
 * @param opG output G value in RGB color space.
 * @param opB output B value in RGB color space.
 */
+(void)hsvToRgb:(CGFloat)iH withS:(CGFloat)iS andV:(CGFloat)iV andR:(CGFloat*)opR andG:(CGFloat*)opG andB:(CGFloat*)opB;

/**
 * Determine whether the given color is a bright color or not
 * @param input redComp, red componet
 * @param output greenComp, green componet
 * @param output blueComp, blue componet
 * @return TRUE if (redComp,greenComp,blueComp) is a bright color
 */
+(BOOL) colorIsBright:(int)redComp withGreen:(int)greenComp withBlue:(int)blueComp;

/**
 * Tune the given color brighter or darker
 * @param input oriColor, represent by integer with 'RGB' order
 * @param input isDarker, indicating we should tune the color darker or brighter
 * @return tuned color
 */
+(int) colorAdjust:(int)oriColor withDarker:(BOOL) isDarker;

+(int) getCalculatedGradient:(int)oriColor;

/**
 * Get components of a given color
 * @param input iValue, represent by integer with 'RGB' order
 * @param output opRed, red componet of color 'iValue'
 * @param output opGreen, green componet of color 'iValue'
 * @param output opBlue, blue componet of color 'iValue'
 */
+ (void) IntToRGB2: (int)iValue withRed:(CGFloat*)opRed andGreen:(CGFloat*)opGreen andBlue:(CGFloat*)opBlue;

/**
 * Extract R, G, B values in RGB color space from a UIColor object. The output parameters *opR, *opG, *opB are all in [0, 1].
 *
 * @param ipColor the UIColor object.
 * @param opR output R value in RGB color space.
 * @param opG output G value in RGB color space.
 * @param opB output B value in RGB color space.
 */
+(void)colorToRgb:(UIColor*)ipColor withR:(CGFloat*)opR andG:(CGFloat*)opG andB:(CGFloat*)opB;

+(NSInteger)getContrastColor:(NSInteger)iGivenColor;
+(NSInteger)darkerHex:(NSInteger)hex percentage:(CGFloat)prc;
+(NSInteger)getColorInGradientRange:(float)index startColor:(NSInteger)iStartColor endColor:(NSInteger)iEndColor;

/**
 * Get text size by set the string to a UILabel and fitToSize.
 * @param input ipString the NSString to measure
 * @param input iFontSize font size used to display the ipString
 * @param input ipFontName font name used to get the font which is used to display the text.
 * @return the size of text with given font and string
 */
+(CGSize) GetTextWidthHeightByUILabel:(NSString*)ipString withFontSize:(float)iFontSize withFontName:(NSString*)ipFontName;

/**
 * Get the value of next month, according to a given month. e.g., input month = 12, year = 2012,
 * output nextMonth = 1, nextYear = 2013
 * @param input month, given month ranging from 1 to 12
 * @param input year, given year represented by four digits
 * @param output nextMonth ranging from 1 to 12
 * @param output nextYear represented by four digits
 */
+(void) nextMonth:(int) month withYear:(int) year toNextMonth:(int*)nextMonth toYear:(int*)nextYear;

/**
 * Get the value of last month, according to a given month. e.g., input month = 12, year = 2012,
 * output lastMonth = 11, lastYear = 2012
 * @param input month, given month ranging from 1 to 12
 * @param input year, given year represented by four digits
 * @param output lastMonth ranging from 1 to 12
 * @param output lastYear represented by four digits
 */
+(void) lastMonth:(int) month withYear:(int) year toLastMonth:(int*)lastMonth toYear:(int*)lastYear;

/**
 * Get the number of days in a given month
 * @param input month, given month ranging from 1 to 12
 * @param input year, given year represented by four digits
 * @return the number of days in the given month
 */
+(int) daysOfMonth:(int)month withYear:(int) year;

/**
 * Tune the given color brighter or darker
 * @param input oriColor, represent by integer with 'RGB' order
 * @param input vFactor, indicating the percentage of current lightness,
 * if vFactor less than 1, tune oriColor darker, else brighter
 * @return tuned color
 */
+(int) colorAdjust:(int)oriColor withVFactor:(float) vFactor;

/**
 * Get file name of image by parsing the given path
 * @param input imagePath, the full path of image
 * @return image name.
 */
+(NSString*)findImageNameFromPath:(NSString*)imagePath;

/**
 * Get the current zoom scale of dashboard. The returned value is valid only after current zoom gesture (double-tapping or pinching) ends.
 *
 * @return the current zoom scale of dashboard.
 */
+(float) getZoomScale;

/**
 * Check whether the dashboard is not in 100% scale.
 *
 * @return YES if zoom scale of dashboard is not 100%, NO if it is 100% scale.
 */
+(BOOL) isDashboardZooming;

/**
 * Check whether the dashboard is being zoomed by pinching or double-tapping gesture. Note the end of double-tapping are defined as the 
 * end of zooming animation. 
 *
 * @return YES if zooming gesture is not finished, NO if zooming gesture is finished.
 */
+(BOOL) isDuringDoubleTapOrPinchZooming;

/**
 * Update the given view and its subviews to proper resolution: make the views on-screen clear and make the views off-screen blurry. 
 *
 * @param iView the specific view of the view hierarchy. If you pass nil or an NSNull object, then it will use the content view of 
 *        RootViewerContainer instead.
 */
+(void) updateZoomScale: (UIView*) iView;

/**
 * Update the resolution of the given view and its subviews to make them clear (i.e., treat them all as on-screen views). 
 *
 * Use {@link #updateScaleFactorToViews:(UIView*) allWithGivenFactor:(CGFloat)}  to update resolutions.
 *
 * @param iView the specific view of the view hierarchy; nil is not allowed.
 */
+(void) updateHighScaleFactorToViews:(UIView*) iView;

/**
 * Update the resolution of the given view and its subviews to given value. 
 *
 * @param iView the specific view of the view hierarchy; nil is not allowed.
 * @param iFactor the given scaleFactor.
 */
+(void) updateScaleFactorToViews:(UIView*) iView allWithGivenFactor:(CGFloat)iFactor;

/**
 * Get the RootViewerContainer view. RootViewerContainer view is the root view for a dashboard.
 *
 * @return the RootViewerContainer view in the view hierarchy.
 */
+(RootViewerContainer*) getRootViewerContainer;

/**
 * Calculate the distace between two CGPoints.
 *
 * @param a the first CGPoint.
 * @param b the second CGPoint. 
 * @return the distance between the two points.
 */
+ (float) distanceBetween:(CGPoint)a andPoint:(CGPoint)b; 

/**
 * Check whether the current view hierarchy is in landscape mode or not.
 *
 * @return YES if the current view hierachy is in landscape mode; Otherwise, NO.
 */
+ (BOOL)isLandscape;

/**
 * Mark a grid cell with orange triangle flag on the upper right corner
 
 * @param iCurrentContext   the CGContext instance where the drawing will happen
 * @param iRect             the anchor rectagle within which to draw the orange triangle
 */
+ (void)markIndication: (CGContextRef) iCurrentContext inRect: (CGRect) iRect;

/**
 * For grid use, whether we will refresh current window screen for each UIScrollView Move in Grid.
 * Basically, it checked whether it's iPad2 device or not.
 
 * @return true if we should refresh the grid for every scroll view movement for this device, otherwise false. The every movement is defined by delegate method scrollViewDidScroll
 */
+ (bool)supportRefreshWindowForEveryMoveInGrid;

+ (void)setRotatingStatus:(int)status;
+ (int)rotatingStatus;

//nli, for 100% of fieldgroup viewers to update frame with superview
/**
 * This method is used to update field group viewer's width, if the field group viewer's width mode is by percent
 
 * @param   the field group viewer that will be updated
 */
+ (void) updateSubFieldGroupViewers:(UIView *)ipView;

/**
 * This method is used to create a new grayed image based on the source image provided in the parameter.
 
 * @param   image the source image that is to be translated.
 * @return  the destination image that is grayed from the source image.
 */
+ (UIImage*) createGrayScaleImageFromImage:(UIImage *)image;

//hhang, support Airplay
/**
 * Set whether it is in Airplay mode now
 
 * @param _isAirplay    whether it is Airplay mode now
 */
+ (void)setIsAirplay:(bool)_isAirplay;

/**
 * Get whether it is in Airplay mode now
 
 * @return  whether it is Airplay mode now
 */
+ (bool)IsAirplay;

/**
 * Get the largest resolution for a screen instance
 
 * @param screen    the screen instance
 * @return          the largest resolution for a screen instance
 */
+ (CGRect)getBestResolution:(UIScreen *)screen;

+(float)getScaleRatio;

+(void)setScaleRatio:(float)ratio;

+(void)setSDKScaleRatio: (float)ratio;
+ (void)resetSDKScaleRatio;

//hhang, LaserPointer support
/**
 * Get whether the app will enter laser pointer mode
 
 * @return  whether the app will enter laser pointer mode
 */
+ (bool)willUseLaserPointer;

@end

/**
 * Enum for log options. This enum is not intended to be used by SDK users. 
 *
 * The element ID of this enum should be a power of 2 so that several log options can be combined like (LO_VIEW_TREE | LO_RETAIN_COUNT).
 */
typedef enum
{
	LO_NO_LOG = 0,
	LO_IMPORTANT_INFO = 1,	// Log important information.
	LO_TRACE_METHODS = 2,	// Trace methods when necessary.
	LO_VIEW_TREE = 4,		// Print the UIView tree of RootViewerContainer to console.
	LO_RETAIN_COUNT = 8,	// Log retain count when necessary.	
	LO_METRICS_INFO = 16,	// Log metrics info, such as bounds, frame, offset, etc.
    LO_TRACE_GENERAL_INFO = 32, // log general information.
}EnumLogOption;

/**
 * The methods in this category are not intended to be used by SDK users.
 */
@interface MSIUtility (Internal)
+ (EnumLogOption)getLogOption;
+ (void)printSubtree:(UIView*)ipView;
+ (void)setLandscape:(BOOL)isLandscape;
+(void)setHonestZoomScale:(float)scale;
+(float)getHonestZoomScale;
+(float)getLastHonestZoomScale;
+(void)setZoomScale:(float)scale;
+(void)setIsDuringDoubleTapOrPinchZooming:(BOOL)isDuring;
+(void)setRootViewerContainer:(RootViewerContainer*)iRootView;
+ (std::string) WToString:(const wchar_t*) ipWSource;
/**
 * Map the length in inches to pixles.
 * @param iInches, the input length in inches to be mapped.
 * @return the mapped length in pixles.
 */
+ (float)inchesToPixles:(float)iInches;
+ (float)inchesToPixles:(float)iInches scaleRatio:(float)iScaleRatio;
/**
 * Special case. Need return float without round.
 */
+ (float)inchesToPixlesWithoutRound:(float)iInches;
+ (float)inchesToPixlesWithoutRound:(float)iInches scaleRatio:(float)iScaleRatio;

/**
 * Map the length in pixles to inches.
 * @param iPixels, the input length in pixles to be mapped.
 * @return the mapped length in inches.
 */
+ (float)pixelsToInches:(float)iPixels;

/**
 * Map the length in points to pixles.
 * @param iPoints, the input length in points to be mapped.
 * @return the mapped length in pixles.
 */
//+ (float)pointsToPixles:(float)iPoints;
+ (float)pointsToPixles:(float)iPoints scaleRatio:(float)iScaleRatio;
/**
 * Special case. Need return float without round.
 */
//+ (float)pointsToPixlesWithoutRound:(float)iPoints;
+ (float)pointsToPixlesWithoutRound:(float)iPoints scaleRatio:(float)iScaleRatio;

/**
 * Get whether the device is iPad or iPhone.
 
 * @return  true when the device is iPad, false otherwise.
 */
+(BOOL)isIPad;

/**
 * For the scaling to width feature on iPhone. Get the scale ratio for the current layout.
 
 * @return  the value of the scale ratio for the current layout.
 */
//+(float)getScaleRatio;

/**
 * For the scaling to width feature on iPhone. Set the scale ratio for the current layout. It is calculated in the layout section viewer.
 
 * @param   ratio   The value of the scale ratio for the current layout.
 */
//+(void)setScaleRatio:(float)ratio;

/**
 * Get description From DataSet, used in accessibility
 * @param input dataSet
 * @return accessibility string from dataSet, read as grid
 */
+(NSString*)getDescriptionStringFromDataSet:(void*)dataSet;

/**
 * Get UISwitch frame size.
 */
+(CGSize) getUISwitchFrameSize;

/**
 * Set the BarButtonItems to one side of a navigationItem. If iOS is earlier than 5.0, then only the first
 * barButtonItem is set.
 * @param iButtonArray the BarButtonItems
 * @param iNavigationItem the navigationItem
 * @param iAtLeft whether set it at the left side
 * @param iAnimated whether we set the BarButtonItems animatedly
 */
+(void) setBarButtonItems:(NSArray*)iButtonArray toNavigationItem:(UINavigationItem*)iNavigationItem atLeft:(BOOL)iAtLeft animated:(BOOL)iAnimated;

/**
 * Get whether app is rendering any dashboard.
 
 * @return  the value of the scale ratio for the current layout.
 */
+(BOOL)isRendering;

/**
 * Set whether app is rendering a dashboard.
 
 * @param   ratio   The value of the scale ratio for the current layout.
 */
+(void)setIsRendering:(BOOL)rendering;


@end
