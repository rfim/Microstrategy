//
//  BrushingHighlightData.h
//  MicroStrategyMobile
//
//  Created by Zhang, Lina on 9/3/13.
//  Copyright (c) 2013 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "DSSRWGridIterator.h"

class DSSRWGridIterator;

@interface BrushingHighlightData : NSObject
{
}
@property (nonatomic, assign) DSSRWGridIterator* mpGridIterator;
@property (nonatomic, assign) BOOL isToShowHighlight;
@property (nonatomic, assign) BOOL isMetricCellHighlight;
@property (nonatomic, strong) NSMutableArray *selectedData;
@property (nonatomic, readonly) int numberOfSelectedAttributes;


- (id)initWithIterator:(DSSRWGridIterator*)gridIterator;

- (BOOL)isDataElementSelected:(std::vector<DSSBaseElementProxy *>)headerElements;
- (BOOL)isCommonAttributeID:(GUID)attrID;
//- (void)addAttrID:(GUID)attrID;
//- (void)addDataElement:(NSMutableArray *)headerElements;
//- (void)clearDataElements;
- (void)updateSelectedAttrs:(std::vector<GUID>)selectedIDs;
- (void)updateSelectedElements:(NSMutableArray *)setOfElements;
- (std::vector<GUID>) getSelectionAttributeIDS;
@end

@interface BrushingHighlightCoordinate : NSObject
{
}

@property (nonatomic, assign) GUID attributeID;
@property (nonatomic, assign) NSInteger axisType;  //0 for row, 1 for column
@property (nonatomic, assign) NSInteger ordinal;
@property (nonatomic, assign) NSInteger depth;

@end