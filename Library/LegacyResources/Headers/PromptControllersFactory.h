//
//  PromptControllersFactory.h
//  MicroStrategyMobile
//
//  Created by Firat Alpergin on 2/23/11.
//  Copyright 2011 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@class PromptHelper, Prompts;

@interface PromptControllersFactory : NSObject {

}

+(PromptControllersFactory *)promptControllersFactory;
-(PromptHelper *)newPromptHelper;
-(PromptHelper *)newPromptHelperWithPrompts:(Prompts *)prompts;

@end
