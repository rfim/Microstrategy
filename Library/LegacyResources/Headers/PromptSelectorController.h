//
//  PromptSelectorController.h
//  MicroStrategyMobile
//
//  Created by Firat Alpergin on 6/24/11.
//  Copyright 2011 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@class BucketElement;

@protocol PromptSelectorObserver

-(void)elementsUpdated:(NSDictionary *)elementsByHeader;

@end


@interface PromptSelectorController : NSObject {
	@private
	NSMutableArray *observers;
	NSMutableDictionary* elementsByHeader;
}
@property (nonatomic)  NSMutableArray *observers;

-(void)updateElements:(NSMutableDictionary *)_elementsByHeader observer:(id<PromptSelectorObserver>)observer;
-(void)addSelectorObserver:(id<PromptSelectorObserver>)observer;
-(void)removeSelectorObserver:(id<PromptSelectorObserver>)observer;

@end
