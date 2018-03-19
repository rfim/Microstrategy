//
//  MIIDynamicTableDataIterator.h
//  MicroStrategyMobile
//
//  Created by Victor Pena on 5/4/11.
//  Copyright 2011 MicroStrategy Inc. All rights reserved.
//

@protocol MIIDynamicTableDataItem;
@protocol MIIDynamicTableDataIterator
/**
 init - initialize iterator.
 */


/**
 itemCount - returns the number of items ont the data source.
 */
- (int)itemCount;

/**
 getDataItemForIndex - returns the data item for an specific index.
 */
- (id<MIIDynamicTableDataItem>)getDataItemForIndex:(int)itemIndex;

/**
 getNewDataItemPropertiesForSelectedPropertyForType - returns the pair <type, dataItemProperties> for properties of new data item for selected property <type, dataItemProperty>.
 */
- (NSDictionary*)getNewDataItemPropertiesForSelectedPropertyForType:(NSDictionary*)propertyForType;

/**
 addDataItemForProperties - properties is a dictionary of <type, dataItemProperties>.
 */
- (void)addDataItemForProperties:(NSDictionary*)propertiesForTypes;

/**
 deleteDataItemForIndex
 */
- (void)deleteDataItemForIndex:(int)itemIndex;

/**
 reorderDataItems
 */
- (void)reorderDataItems:(NSArray*)items;


@end
