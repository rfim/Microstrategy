//
//  MSICertificateCreationViewController.h
//  MicroStrategyMobile
//
//  Created by Wang, Wenyin on 3/7/14.
//  Copyright (c) 2014 MicroStrategy Inc. All rights reserved.
//

#import "MSIMobileLoginPromptViewController.h"
#import "CertificateLoginFieldsService.h"

#define CERT_CREATION_NOT_INITIALIZED 0
#define CERT_CREATION_RETRIEVING_LOGIN_FIELDS 1
#define CERT_CREATION_WAITING_FOR_USER_INPUT 2
#define CERT_CREATION_GETTING_CERTIFICATE 3
#define CERT_CREATION_FINISHED 4

#define CERTIFICATE_PROMPT_WAITING 0
#define CERTIFICATE_PROMPT_ANSWERED 1
#define CERTIFICATE_PROMPT_CANCELLED 2
#define CERTIFICATE_PROMPT_ERROR 3

#define MSI_AUTHENTICATION_CERTIFICATE_SERVER_KEY @"certificateServer"
#define MSI_AUTHENTICATION_FIELD_KEY @"field"
#define MSI_AUTHENTICATION_PASS_KEY @"pass"

@interface MSICertificateCreationViewController : MSIMobileLoginPromptViewController
@end
