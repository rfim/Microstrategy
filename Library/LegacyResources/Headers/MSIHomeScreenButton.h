//
//  MSIHomeScreenButton.h
//  MSTRMobile
//
//  Created by Liu Shu on 4/30/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSISerializable.h"

@class MSIHomeScreenButtonActionSettings, Caption, Description, Icon;

@interface MSIHomeScreenButton : NSObject <MSISerializable> {
@private
	MSIHomeScreenButtonActionSettings* actionSettings;
	Caption* caption;
	Description* description;
	Icon* iconSettings;
}

- (id)initWithActionSettings:(MSIHomeScreenButtonActionSettings*)actionSettings caption:(Caption*)caption
				 description:(Description*)description iconSettings:(Icon*)iconSettings;

@property (nonatomic, strong, readonly) MSIHomeScreenButtonActionSettings* actionSettings;
@property (nonatomic, strong, readonly) Caption* caption;
@property (nonatomic, strong, readonly) Description* description;
@property (nonatomic, strong, readonly) Icon* iconSettings;

@end
