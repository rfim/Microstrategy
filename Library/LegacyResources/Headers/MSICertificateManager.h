//
//  MSICertificateManager.h
//  MicroStrategyMobile
//
//  Created by Liu Shu on 8/30/11.
//  Copyright 2011 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@class CertificateInfo;

@protocol AuthenticationCodePromptCallback <NSObject>

- (void)authenticationCodePromptDismissed;

@end

@interface MSICertificateManager : NSObject {
@private
	id<AuthenticationCodePromptCallback> __unsafe_unretained callback;
}

@property BOOL isCertificateValid;

+ (MSICertificateManager *) getInstance;

@property (nonatomic, unsafe_unretained) id<AuthenticationCodePromptCallback> callback;
@property BOOL certificateFetchedUsingURL;

/*
 Persistent the certificate and the identity
 In CertificateCreationViewController, the GetCertificateService will download the certificate data and call this method to parse the info
 */
-(BOOL)loadCertificate:(NSData*)data pkcs12Password:(NSString*)pkcs12Password;

/*
 The path where to store the certificate info xml file
 */
- (NSString*) certificateInfoPath;

/*
 Fetch the certificate info which includes the expire time and issued by
 In AdvancedSettingsController, we need parse the identity info by parsing the xml file from local storage
 */
- (CertificateInfo*) getCertificateInfo;

/*
 Query the ceritificate identity
 In AdvancedSettingsController, we will query the certificate identity to check whether the certificate exists
 In MSINetworkRequestImpl, we will query the certificate identity to answer the certificate challenge
 */
-(SecIdentityRef) getCertificateIdentity;

/*
 Query the client ceritificate chain
 In MSINetworkRequestImpl, we will query the certificate chain to answer the certificate challenge
 @return nil if no one exists
 */
- (NSArray*) getClientCertificateChain;

/*
 Deletes all credentials from the application's keychain, also delete the certificate info xml file
 Not using return value because we don't care if it fails
 */
- (void) clearCertificates;

/*
 Check whether or not the certificate has expired
*/
- (BOOL) isCertificateExpired;

/*
 Load the embedded client certificates
 */
- (void) loadEmbeddedClientCertificate;

- (BOOL) isUsingEmbeddedCertificate;

- (void)persistCertificate;

@property (nonatomic) BOOL shouldPinPublicKey;

//- (BOOL) shouldPerformConnectionSecurityMeasuresForURLConnection: (NSURLConnection *) connection;
- (BOOL) shouldPerformConnectionSecurityMeasuresForURLRequest: (NSURLRequest *) request;

//- (BOOL) evaluateServerTrust: (SecTrustRef) serverTrust forURLConnection:(NSURLConnection *)connection;
- (BOOL) evaluateServerTrust: (SecTrustRef) serverTrust forURLRequest:(NSURLRequest *)request;

- (BOOL) shouldWaitForClientAuthentication;

- (BOOL) isCertificateCleared;
@end
