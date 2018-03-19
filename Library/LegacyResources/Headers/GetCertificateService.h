//
//  GetCertificateService.h
//  MSTRMobileSDK
//
//  Created by Kevin Maurer on 12/8/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSIServiceImpl.h"

@class MSIProjectInfo;

@interface GetCertificateService : MSIServiceImpl {

	NSDictionary* parameters;
	NSString* pkcs12Password;
	NSString *certificateServerURL;
}

@property (nonatomic, strong) NSDictionary* parameters;
@property (nonatomic, copy) NSString* pkcs12Password;
@property (nonatomic, strong) NSString *certificateServerURL;
@end
