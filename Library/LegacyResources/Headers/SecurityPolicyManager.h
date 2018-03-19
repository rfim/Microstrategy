//
//  SecurityPolicyManager.h
//  MicroStrategyMobile
//
//  Created by Charania, Amirali on 11/12/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Enums.h"

@class SecurityPolicy;

@protocol SecurityPolicyManagerDelegate

- (void)authorizationSuccess;
- (void)authorizationFailure;
- (BOOL)isAuthorized;

@end

@protocol SecurityPolicyManager <NSObject>

- (void)initialize;
- (void)initializeWithSecurityPolicy:(SecurityPolicy*)securityPolicy;
- (void)authorize;
- (void)setSecurityPolicyManagerDelegate:(id<SecurityPolicyManagerDelegate>)delegate;
- (BOOL)fileNeedsOnlySoftwareEncryption:(NSString*)fileName;
- (EncryptionMode)encryptionMode;
- (SecurityPolicy*)securityPolicy;
- (BOOL)shouldHideScreenContentsOnBackground;
- (BOOL)shouldUnauthorizeOnBackground;
- (BOOL)shouldBeAauthorized;
- (BOOL)isMappedFileSupported;
- (BOOL)webViewShouldLoadDataUsingURL;
- (void)clearKey;
- (void)clearKeyChain;
- (void)deleteKey;
- (void)changePassword;
- (BOOL)isConfidenfidentialProjectSupported;
- (NSOutputStream*)newOutputStreamToFileAtPath:(NSString*)filePath append:(BOOL)append;
- (NSInputStream*)newInputStreamWithFileAtPath:(NSString*)filePath;
- (BOOL)shouldThrowExceptionIfAccessedBeforeAuhtorization;
- (BOOL)shouldUnauthorizeOnDeviceLock;
- (BOOL)shouldHideCoverScreen;
- (BOOL)shouldPresentMailComposerFromTopModalView;
- (void) clearViewController;
@end

