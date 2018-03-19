//
//  EditControlProperty.h
//  iPadMain Library
//
//  Created by Ma Xianqing on 3/11/11.
//  Copyright 2011 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSTRHandler.h"
#import "DSSIPhoneEnums.h"

typedef enum EnumControlType
{
    CT_NOT_CONTROL   = 0,
    CT_TEXTBOX       = 1,
    CT_SWITCH        = 2,
    CT_LIST          = 3,
    CT_SLIDER        = 4,
    CT_DATE_PICKER   = 5,
    CT_TIME_PICKER   = 6,
    CT_TOGGLE        = 7,
    CT_TEXTAREA      = 8,
    CT_SIGNATURE     = 9,
    CT_STARRATING    = 10,
    CT_STEPPER       = 12,
    CT_BARCODE       = 13,
    CT_LIKERTSCALE   = 14,
    CT_PARSING_ERROR,
}EnumControlType;

typedef enum _EnumValidateMethod {
	ValidateMethodNone = 0,
	ValidateMethodPhoneNumber,
	ValidateMethodEmailAddress,
	ValidateMethodUSZipCode,
	ValidateMethodSocialSecurityNumber,
	ValidateMethodRegularExpression
} EnumValidateMethod;

// Control Property
@interface ControlProperty : NSObject
{
	int mIsPassword;
    
	NSString* maxValue;
	NSString* minValue;
    
    int maxlen;
    int minlen;
    
	int mIncludeTime;
	
	//property for Time Picker and Slider
	NSString* interval;
	
	//property for Switch and Slider
	int mShowByDefault;
	NSString* valueList;
	
	//property for Slider
	int mInputValue;
	
	int mShowIcon;
	int mShowPreview;
	int mPreviewLength;
	double mControlWidth;
    
	int mWidthMode;
	
	NSString* mUnsetString;
	int mLabelDisplayWidth;
}

+ (ControlProperty*) controlPropertyWithFormatString:(NSString *)formatString DataType:(EnumDSSDataType)dataType;
+ (ControlProperty*) controlPropertyWithControlType:(EnumControlType)controlType;

+ (int) instanceCount;

- (void) readAttributesFromDictionary:(NSDictionary*) dictionary;

@property (nonatomic) EnumControlType controlType;

@property (nonatomic) int mIsPassword;

@property (nonatomic, strong) NSString* maxValue;
@property (nonatomic, strong) NSString* minValue;

@property (nonatomic) int maxlen;
@property (nonatomic) int minlen;

@property (nonatomic, strong) NSString* interval;
@property (nonatomic) int mShowByDefault;
@property (nonatomic, strong) NSString* valueList;
@property (nonatomic) int mInputValue;
@property (nonatomic) int mShowIcon;
@property (nonatomic) int mShowPreview;
@property (nonatomic) int mPreviewLength;
@property (nonatomic) double mControlWidth;
@property (nonatomic) int mWidthMode;
@property (nonatomic) int mIncludeTime;
@property (nonatomic, strong) NSString* mUnsetString;
@property (nonatomic) int mLabelDisplayWidth;

@end

// Signature Control Property
@interface ControlPropertySignature : ControlProperty {
@private
	bool showline;
}

@property (nonatomic) bool showline;

@end

// Text Field Control Property
@interface ControlPropertyTextField : ControlProperty {
@private
	bool limitMin;
	bool limitMax;
	int validateMethod;
	NSString* regularExp;
}

@property (nonatomic) bool limitMin;
@property (nonatomic) bool limitMax;
@property (nonatomic) int validateMethod;
@property (nonatomic, strong) NSString* regularExp;

@end

// Likert Scale Control Property
@interface ControlPropertyLikertScale : ControlProperty {
@private
    NSString* minLabel;
    NSString* maxLabel;
}

@property (nonatomic, strong) NSString* minLable;
@property (nonatomic, strong) NSString* maxLable;

@end

// Stepper Control Property
@interface ControlPropertyStepper : ControlProperty {
@private
//    NSString* minvalue;
//    NSString* maxvalue;
//    NSString* intervalValue;
}

//@property (nonatomic, retain) NSString* minvalue;
//@property (nonatomic, retain) NSString* maxvalue;
//@property (nonatomic, retain) NSString* intervalValue;

@end

// Barcode Control Property
@interface ControlPropertyBarcode : ControlProperty {
@private
    bool matchValue;
}

@property (nonatomic) bool matchValue;

@end

// Star rating Control Property
@interface ControlPropertyStarRating : ControlProperty {
@private
    
    int displayStyle;
}

@property (nonatomic) int displayStyle;

@end

// Switch Control Property
@interface ControlPropertySwitch : ControlProperty {
@private
    bool isCheckBox;
}

@property (nonatomic) bool isCheckBox;

@end

// List Control Property
@interface ControlPropertyList : ControlProperty {
@private
    NSString *unsetValue;
    int displayStyle;
    int itemsPerRow;
}
@property (nonatomic, strong) NSString* unsetValue;
@property (nonatomic) int displayStyle;
@property (nonatomic) int itemsPerRow;
@end


@protocol ControlPropertyDelegate
- (BOOL)isNumberCell;
- (BOOL)isTextCell;
- (BOOL)isDateCell;
- (BOOL)isTimeCell;
- (BOOL)isDateTimeCell;

@optional
- (void)setPropertyType: (EnumControlType) iType andValue: (ControlProperty*) iValue;
@end



@interface EditControlProperty : NSObject <MSTRHandler>{
	EnumControlType mControlType;
	ControlProperty* mControlProperty;
}

@property (nonatomic) EnumControlType mControlType;
@property (nonatomic, strong) ControlProperty* mControlProperty;

-(bool) ParseFormatString: (NSString *) iFormatString forValue: (id<ControlPropertyDelegate>) ipValue;

/**
 Creates an EditControlProperty.
 The caller should release this created object
 **/
+ (EditControlProperty*) createEditControlProperty:(id<ControlPropertyDelegate>) ipValue parseFormatString:(NSString*)nsString;
@end
