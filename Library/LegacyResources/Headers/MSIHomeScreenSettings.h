//
//  HomeScreenSettings.h
//  MSTRMobile
//
//  Created by Liu Shu on 4/30/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSIPreference.h"

@class MSIHomeScreenEntryPointResultSet, MSIHomeScreenFolder, MSICustomHomeScreenSettings;

@interface MSIHomeScreenSettings : NSObject <MSIPreference> {
@private
	NSString* prefID;
	__weak MSIDevicePreferences * parent;
	NSInteger homeScreenType;
	MSIHomeScreenEntryPointResultSet* resultSet;
	MSIHomeScreenFolder* folder;
	MSICustomHomeScreenSettings * customSettings;
}

@property (nonatomic, copy) NSString* prefID;
@property (readonly) NSInteger homeScreenType;
@property (nonatomic, strong, readonly) MSIHomeScreenEntryPointResultSet* resultSet;
@property (nonatomic, strong, readonly) MSIHomeScreenFolder* folder;
@property (nonatomic, strong, readonly) MSICustomHomeScreenSettings * customSettings;

-(void)setParent:(MSIDevicePreferences *)devicePreference;
-(NSMutableArray*) getSubscriptionReports;
-(void)setHomeScreenTypeToDefault;

@end