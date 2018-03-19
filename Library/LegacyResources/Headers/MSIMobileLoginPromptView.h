//
//  MSIMobileLoginPromptView.h
//  MicroStrategyMobile
//
//  Created by Wang, Wenyin on 1/3/14.
//  Copyright (c) 2014 MicroStrategy Inc. All rights reserved.
//

#import <UIKit/UIKit.h>

@class MSIMobileLoginPromptView;

/**
 Protocol for mobile login view controller to implement.
 */
@protocol MSIMobileLoginPromptViewDelegate<NSObject>

/**
 Called when user did input login information on view.
 @param promptView The mobile login prompt view
 @param parameters The user input information mobile login prompt view returns to the controller
 */
- (void) loginPromptView:(MSIMobileLoginPromptView *)promptView didInputAuthenticationParameters:(NSDictionary*)parameters;

/**
 Called when user cancel the login on view.
 @param promptView The mobile login prompt view
 */
-(void) loginPromptViewDidCancel:(MSIMobileLoginPromptView *)promptView;

@end

#define MSI_AUTHENTICATION_SECTION_TITLE_KEY @"sectionTitle"
#define MSI_AUTHENTICATION_SECTION_SUBTITLE_KEY @"sectionSubtitle"
#define MSI_AUTHENTICATION_PROJECT_NAME_KEY @"projectName"
#define MSI_AUTHENTICATION_SERVER_NAME_KEY @"serverName"
#define MSI_AUTHENTICATION_USER_NAME_KEY @"username"
#define MSI_AUTHENTICATION_PASSWORD_KEY @"password"
#define MSI_AUTHENTICATION_CONFIGURATION_KEY @"configuration"
#define MSI_AUTHENTICATION_PROJECT_INFO @"projectInfo"

#define MSI_AUTHENTICATION_TRUE_VALUE @"true"
#define MSI_AUTHENTICATION_FALSE_VALUE @"false"

@interface MSIMobileLoginPromptView : UIView {
}

/**
 prompt view's delegate
 */
@property (nonatomic, weak) id<MSIMobileLoginPromptViewDelegate> delegate;

/**
 prompt view's parameters, passed in by calling setupWithParameters:delegate:
 */
@property (nonatomic, strong) NSDictionary * parameters;

/**
 Init the view.
 @param promptViewParameters The parameters mobile login prompt view controller passes to the view for init
 @param delegate The view controller
 */
- (void) setupWithParameters:(NSDictionary*) promptViewParameters delegate:(id<MSIMobileLoginPromptViewDelegate>) delegate;


@end
