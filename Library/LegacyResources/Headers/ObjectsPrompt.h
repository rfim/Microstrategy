//
//  ObjectsPrompt.h
//  MicroStrategyMobile
//
//  Created by Li, Dai on 1/3/13.
//  Copyright (c) 2013 MicroStrategy Inc. All rights reserved.
//
#import "MSIPrompt.h"

@interface ObjectsPrompt : MSIPrompt

@property (nonatomic, strong) NSArray* suggestedAnswers;
@property (nonatomic, readonly, copy) NSString* searchXML;

@end
