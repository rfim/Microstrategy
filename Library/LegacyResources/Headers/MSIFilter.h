//
//  Filter.h
//  MSTRMobile
//
//  Created by Ilia Baskin on 3/3/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSIObjectInfo.h"

@class MSIExpression;
@class PromptInstances;
@interface MSIFilter : MSIObjectInfo {
	MSIExpression* expression;
    PromptInstances* promptInstance;
}

@property (nonatomic, strong) MSIExpression* expression;
@property (nonatomic, strong) PromptInstances* promptInstance;
@end
