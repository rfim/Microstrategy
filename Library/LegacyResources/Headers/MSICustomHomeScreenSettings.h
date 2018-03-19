//
//  MSICustomHomeScreenSettings.h
//  MSTRMobile
//
//  Created by Liu Shu on 4/30/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSISerializable.h"

@class MSIHomeScreenFormatSettings, MSIHomeScreenButtonList;

@interface MSICustomHomeScreenSettings : NSObject <MSISerializable> {
@private
	MSIHomeScreenFormatSettings* formatSettings;
	MSIHomeScreenButtonList* buttons;
}

- (id)initWithFormatSettings:(MSIHomeScreenFormatSettings*)format buttonList:(MSIHomeScreenButtonList*)buttons;

@property (nonatomic, readonly) MSIHomeScreenFormatSettings* formatSettings;
@property (nonatomic, readonly) MSIHomeScreenButtonList* buttons;

@end
