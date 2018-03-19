//
//  SendPerfLog.h
//  MicroStrategyMobile
//
//  Created by Administrator on 9/14/15.
//  Copyright © 2015 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <objc/runtime.h>
#import "IPadDataControllerImpl.h"

@interface SendPerfLog : NSObject

+ (SendPerfLog *)getInstance;

@end
