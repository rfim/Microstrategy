//
//  InitializeApplicationService.h
//  MSTRMobile
//
//  Created by Liu Shu on 4/28/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSIServiceImpl.h"

@class MSTRError;

@interface InitializeApplicationService : MSIServiceImpl {
	NSString *xmlURL;
	NSInteger authMode;
	NSString *login;
	NSString *password;
	MSTRError *currentError;
	BOOL isSuccessful;
}

@property (nonatomic, copy) NSString *xmlURL;
@property NSInteger authMode;
@property (nonatomic, copy) NSString *login;
@property (nonatomic, copy) NSString *password;
@property (nonatomic) BOOL cancelAuthenticationChallengeForClientCertificateMode;
@property (nonatomic) BOOL ignoreIdentityForClientCertificateMode;

@end