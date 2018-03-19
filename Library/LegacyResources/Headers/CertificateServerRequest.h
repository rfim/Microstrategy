//
//  CertificateServerRequest.h
//  MSTRMobileSDK
//
//  Created by Kevin Maurer on 12/8/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSINetworkRequestImpl.h"
#import "MSTRHandler.h"

@class AuthenticationFormRequest;

@interface CertificateServerRequest : MSINetworkRequestImpl <MSTRHandler, NetworkResponseHandler>{
	NSString* action;
	NSString* certificateServerURL;
	AuthenticationFormRequest* liveAuthRequest;
}

@property (nonatomic, copy) NSString* action;
@property (nonatomic, strong) NSString *certificateServerURL;

-(void) addParameterValue:(NSString*)value forKey:(NSString*)key;

@end
