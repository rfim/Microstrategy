//
//  WebServerCredentials.h
//  MSTRMobileSDK
//
//  Created by jacky xu on 11/18/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSICredentials.h"

/**
 * SubClass of Credentials.
 * Credentials used in the WebServerConfiguration as web server credentials and WebServerList as default web server credentials.
 * It will be initialized with anonymous authentication mode by default.
 * It has the same behavior as basic Credentials.
 */
@interface WebServerCredentials : MSICredentials {

}

@end
