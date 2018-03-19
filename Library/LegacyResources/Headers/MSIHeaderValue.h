//
//  HeaderValue.h
//  IPadMain
//
//  Created by Liang Chen on 8/12/10.
//  Copyright 2010 MicroStrategy Inc. All rights reserved.
//

#import "DataProviderProtocol.h"

@interface MSIHeaderValue : NSObject {
	MSIHeader* __weak owner;
	NSString* headerValue;
	NSInteger semantics;
	NSString* rawValue;
    NSString* htmlValue;
	
	//is this in the hexCode format or webcolor format, find out ???
	//is this being assigned anywhere??? TODO CLEANUP
	NSString* cellColor;
	id <MSIDataProviderProtocol> __weak data;
	MSIPropertyGroup* format;
	NSInteger row;
	NSInteger col;
	
	NSInteger color; // when formatting values, we might have the color as well.
    
    NSString* fontName;
    BOOL isBold;
    BOOL isItalic;
    BOOL isUnderLine;
    int fontColor;
    int dataType;
    unsigned char dssDataFlag;
    //int backgroundColor;
}

/**
 The value of this cell. This string represents the formatted value.
 */
@property (weak, nonatomic, readonly) NSString* headerValue;
/**
 Indicates the semantic type of the header value
 @see BaseFormSemanticType
 */
@property (readonly) NSInteger semantics;
@property  int dataType;
@property unsigned char dssDataFlag;
/**
 The raw (unformatted) value of the cell (if present).
 */
@property (nonatomic, strong, readonly) NSString* rawValue;
@property (nonatomic, strong, readonly) NSString* htmlValue;
@property (nonatomic, copy) NSString* cellColor;
@property (nonatomic, readonly) NSInteger row;
@property (nonatomic, readonly) NSInteger col;
@property (nonatomic, strong, readonly, getter=getFormat) MSIPropertyGroup* format;
@property (nonatomic) NSInteger color;
@property (nonatomic, strong) NSString* fontName;

@property (nonatomic, readonly) BOOL isBold;
@property (nonatomic, readonly) BOOL isItalic;
@property (nonatomic, readonly) BOOL isUnderLine;
@property (nonatomic, readonly) int fontColor;
//@property (nonatomic, readonly) int backgroundColor;


- (void) setRowIndex:(NSInteger) rowIndex columnIndex:(NSInteger) colIndex;

-(MSIPropertyGroup*) getFormat;

- (NSInteger)type;

/**
 MSIHeader object corresponding to this header value.
 */
- (MSIHeader*)header;

/**
 Returns the color for this cell, as set in MSTR Web.
 @return String color code (RGB). //It seems from other places that this would return in BGR format??srini
 */
- (NSString*) colorCode;

/**
 Returns the color of this cell in the regular hex code. There won't be any # in the resuting
 string. It would be of the format @"3244af"
 **/
- (NSString*) hexColorCode;

-(UIFont*)collectFormatAndGetFormattedFont;

+ (NSString*) getHTMLLinkString:(NSString*)xmlStr;

@end
