//
//  MSICloseSessionsService.h
//  MSTR Foundation
//
//  Created by Amirali Charania on 2/20/09.
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSIServiceImpl.h"

@interface MSICloseSessionsService : MSIServiceImpl {
	
@private
	WebServerConfiguration* webServer;
	NSMutableDictionary* webServerToSessionsMap;
	NSMutableDictionary* sessionTable;
    NSMutableDictionary* context;
}

//app will also try to remove realted session objects from session table in MSIServiceFactory.
@property (nonatomic, strong) WebServerConfiguration* webServer;//If we just want to close the sessions for a specific web server, this member should be set; otherwise, we will close all sessions
@property (nonatomic, strong) NSMutableDictionary* webServerToSessionsMap;
@property (nonatomic, strong) NSMutableDictionary* sessionTable;
@property (nonatomic, strong) NSMutableDictionary* context;

@end
