//
//  SecurityManager.h
//  MicroStrategyMobile
//
//  Created by Amirali Charania on 11/24/11.
//  Copyright 2011 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "SecurityHandler.h"
#import "Enums.h"

@interface SecurityManager : NSObject {
	id<SecurityHandler> securityHandler;    
}

/** Returns the singleton secuirty manager
  * @return the security manager for the application
  */
+ (SecurityManager*)securityManager;

@property (nonatomic, readonly) id<SecurityHandler> securityHandler;

@end
