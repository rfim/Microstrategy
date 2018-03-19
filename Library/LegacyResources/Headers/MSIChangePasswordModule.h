//
//  MSIChangePasswordModule.h
//  MicroStrategyMobile
//
//  Created by Wang, Wenyin on 3/24/14.
//  Copyright (c) 2014 MicroStrategy Inc. All rights reserved.
//

#import "MSIMobileLoginModule.h"

#define MSI_AUTHENTICATION_OLD_PASSWORD_KEY @"oldPassword"
#define MSI_AUTHENTICATION_NEW_PASSWORD_KEY @"newPassword"
#define MSI_AUTHENTICATION_CONFIRM_NEW_PASSWORD_KEY @"confirmNewPassword"

@interface MSIChangePasswordModule : MSIMobileLoginModule

@end
