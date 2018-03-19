//
//  BucketElement.h
//  CalendarUIPrototype
//
//  Created by Firat Alpergin on 4/14/11.
//  Copyright 2011 MicroStrategy Incorporated. All rights reserved.
//

#import <Foundation/Foundation.h>

@class BucketHeader;

@interface BucketElement : NSObject {
	@private
	int index;
	NSString *name;
}

@property (nonatomic, copy) NSString *name;
@property (nonatomic, assign) int index;
@property (nonatomic, strong) BucketHeader* header;

-(id)initWithName:(NSString *)newName;

@end
