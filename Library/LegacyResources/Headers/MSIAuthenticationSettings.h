//
//  MSIAuthenticationSettings.h
//  MicroStrategyMobile
//
//  Created by Wang, Wenyin on 5/7/14.
//  Copyright (c) 2014 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>


typedef enum _AuthenticationItemType{
	AuthenticationItemTypeTextField = 0,
    AuthenticationItemTypePasswordTextField = 1,
    AuthenticationItemTypeList = 2
}AuthenticationItemType;

@interface MSIAuthenticationSettings : NSObject

+ (MSIAuthenticationSettings *) sharedMSIAuthenticationSettings;

- (id) init;

- (void) dealloc;

////Settings

- (NSDictionary*) serverAuthenticationSettings;

- (NSDictionary*) projectAuthenticationSettings;

- (NSDictionary*) certificateServerAuthenticationSettings;

- (NSDictionary*) changePasswordSettings;

////Configuration, elements of Settings

- (NSDictionary*) defaultServerAuthenticationConfiguration;

- (NSDictionary*) defaultProjectAuthenticationConfiguration;

- (NSDictionary*) tutorialProjectAuthenticationConfiguration;

- (NSDictionary*) defaultCertificateServerAuthenticationConfiguration;

- (NSDictionary*) defaultChangePasswordConfiguration;

////Parameters, elements of Configuration

+ (NSString *) portraitBackgroundImage:(NSDictionary *) configuration;

+ (NSString *) landscapeBackgroundImage:(NSDictionary *) configuration;

+ (NSString *) backgroundImage:(NSDictionary *) configuration;

+ (NSString *) iPhone4BackgroundImage:(NSDictionary *) configuration;

+ (NSDictionary *) titleParameters:(NSDictionary *) configuration;
+ (NSString *) titleText:(NSDictionary *) configuration;
+ (NSString *) titleFontName:(NSDictionary *) configuration;
+ (UIColor *) titleFontColor:(NSDictionary *) configuration;

+ (NSDictionary *) headerParameters:(NSDictionary *) configuration;
+ (NSString *) headerText:(NSDictionary *) configuration;
+ (NSString *) headerFontName:(NSDictionary *) configuration;
+ (UIColor *) headerFontColor:(NSDictionary *) configuration;

+ (NSDictionary *) loginButtonParameters:(NSDictionary *) configuration;
+ (NSString *) loginButtonTitle:(NSDictionary *) configuration;
+ (NSString *) loginButtonFontName:(NSDictionary *) configuration;
+ (UIColor *) loginButtonFontColor:(NSDictionary *) configuration;
+ (UIColor *) loginButtonHighlightedFontColor:(NSDictionary *) configuration;
+ (UIColor *) loginButtonBackgroundColor:(NSDictionary *) configuration;
+ (UIColor *) loginButtonHighlightedBackgroundColor:(NSDictionary *) configuration;

+ (NSDictionary *) cancelButtonParameters:(NSDictionary *) configuration;
+ (NSString *) cancelButtonTitle:(NSDictionary *) configuration;
+ (NSString *) cancelButtonFontName:(NSDictionary *) configuration;
+ (UIColor *) cancelButtonFontColor:(NSDictionary *) configuration;
+ (UIColor *) cancelButtonHighlightedFontColor:(NSDictionary *) configuration;
+ (UIColor *) cancelButtonBackgroundColor:(NSDictionary *) configuration;
+ (UIColor *) cancelButtonHighlightedBackgroundColor:(NSDictionary *) configuration;

+ (NSDictionary *) footerParameters:(NSDictionary *) configuration;
+ (NSString *) footerText:(NSDictionary *) configuration;
+ (NSString *) footerFontName:(NSDictionary *) configuration;
+ (UIColor *) footerFontColor:(NSDictionary *) configuration;

+ (NSDictionary *) forgetPasswordParameters:(NSDictionary *) configuration;
+ (NSString *) forgetPasswordText:(NSDictionary *) configuration;
+ (NSString *) forgetPasswordFontName:(NSDictionary *) configuration;
+ (UIColor *) forgetPasswordFontColor:(NSDictionary *) configuration;
+ (NSString *) forgetPasswordLink:(NSDictionary *) configuration;

+ (NSArray *) authenticationParameters:(NSDictionary *) configuration;

////Authentication Item

+ (NSString *) authenticationItemKey:(NSDictionary *) authenticationItem;

+ (AuthenticationItemType) authenticationItemType:(NSDictionary *) authenticationItem;

+ (NSDictionary *) authenticationItemCfg:(NSDictionary *) authenticationItem;
+ (NSString *) authenticationItemLabel:(NSDictionary *) authenticationItem;
+ (NSString *) authenticationItemPlaceholderText:(NSDictionary *) authenticationItem;
+ (NSString *) authenticationItemDefaultValue:(NSDictionary *) authenticationItem;
+ (NSArray *) authenticationItemSelections:(NSDictionary *) authenticationItem;
+ (NSInteger) authenticationItemDefaultSelection:(NSDictionary *) authenticationItem;

+ (NSString *)localizedString:(NSString *) string;

@end
