//
//  PropertyGroup.h
//  IPadMain
//
//  Created by dong shi on 8/10/10.
//  Copyright 2010 MicroStrategy Inc. All rights reserved.
//

/*
 * this class is an adaptor to formatInfo class
 */


#import <Foundation/Foundation.h>

@interface MSIPropertyGroup : NSObject {

}


@property(nonatomic, readonly) bool IsShared;
@property(nonatomic, readwrite) bool IsFormatPopulated;


- (NSString*)propertyByPropertySetID:(NSInteger)propertySetID propertyID:(NSInteger)propertyID;
- (void*) FormatInfoPtr;
- (void) setFormatInfoPtr:(void*)ipFormatInfoPtr;
- (id) initWithFlag:(bool) iIsShared;
+ (NSString*)propertyKeyForPropertySetID:(NSInteger)propertySetID propertyID:(NSInteger)propertyID;
+ (NSString*)propertyKeyForPropertySetName:(NSString*)propertySetName propertyName:(NSString*)propertyName;

@end
