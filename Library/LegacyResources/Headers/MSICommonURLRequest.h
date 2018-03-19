//
//  MSICommonURLRequest.h
//  MicroStrategyMobile
//
//  Created by Liu Shu on 4/10/11.
//  Copyright 2011 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSINetworkRequestImpl.h"

@interface MSICommonURLRequest : MSINetworkRequestImpl {
@private
	NSString *url;
	NSInteger authMode;
	NSString *login;
	NSString *password;
}

@property (nonatomic, copy) NSString *url;
@property NSInteger authMode;
@property (nonatomic, copy) NSString *login;
@property (nonatomic, copy) NSString *password;

@end
