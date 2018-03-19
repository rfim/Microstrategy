//
//  MSIWebRequest.h
//  MSTR Foundation
//
//  Created by Amirali Charania on 2/20/09.
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSINetworkRequestImpl.h"

@interface MSIWebRequest : MSINetworkRequestImpl {
	@protected
	NSInteger event;
	NSString* src;
}

@end
