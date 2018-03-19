//
//  DisplayInfo.h
//  IPadMain
//
//  Created by Liang Chen on 8/12/10.
//  Copyright 2010 MicroStrategy Inc. All rights reserved.
//


@interface MSIDisplayInfo : NSObject{

}

@property (readonly) NSInteger type;
@property (nonatomic, strong, readonly) NSString* displayInfoID;
@property (readonly) NSInteger subType;
@property (nonatomic, strong, readonly) NSString* name;
@property (nonatomic, strong, readonly) NSString* description;
@property (nonatomic, copy) NSString* path;
@property NSInteger accessGranted;
@property (nonatomic, copy) NSString* displayName;
@property (readonly) NSInteger drillType;
@property (readonly) NSInteger linkDrillHeaderIndex;
@property BOOL editable;

@end
