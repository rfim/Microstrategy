//
//  Header.h
//  IPadMain
//
//  Created by Liang Chen on 8/12/10.
//  Copyright 2010 MicroStrategy Inc. All rights reserved.
//
#import "DataProviderProtocol.h"
#import	"DSSIPhoneEnums.h"

@class MSIDisplayInfo, MSIPropertyGroup, MSICustomGroupItem, MSIAttributeForm;
@interface MSIHeader : NSObject {
//	MSIDisplayInfo* displayInfo;
	//_HeaderTypes in Enum TODO change this to HeaderTypes enum...
	NSInteger type;
	
	id <MSIDataProviderProtocol> __weak data;
	//0-based index and is meant for non metric headers. It shows the position of the corresponding attribute in the template
	//Currently for metric headers, the value does not make sense and is set to 0 by TabularData. It should be set to metricIndex for MSIMetricHeader TODO???
	NSInteger depth;
	MSIPropertyGroup* format;
	NSMutableArray* elements;
	@private
	//EnumDSSDataType
	EnumDSSDataType dataType;
}

@property (nonatomic, strong) NSMutableArray* elements;
@property (nonatomic, strong, getter=getFormat) MSIPropertyGroup* format;
@property (readonly) NSInteger type;
@property EnumDSSDataType dataType;

/**
 Deprecated. Please use displayInfo instead. 
 **/
//- (MSIDisplayInfo*)object __MIDEPRECATED_METHOD;
- (MSIDisplayInfo*)object;

/**
 Returns the enclosing unit (e.g. MSIAttributeForm, MSIMetric) 
 **/
- (MSIDisplayInfo*)displayInfo;
- (NSInteger)depth;
- (NSInteger) baseFormID;
- (NSInteger) baseFormType;
- (MSIAttributeForm*) getAttributeForm;
- (MSICustomGroupItem*)customGroupItem;

/**
 * Return the width of the first element
 */
- (CGFloat) firstHeaderWidth:(UIFont *)_font imageWidth:(CGFloat)_imageColumnWidth;

@end
