//
//  ImageInfo.h
//  MSTRMobileSDK
//
//  Created by Srinivas Singanamalla on 1/11/11.
//  Copyright 2011 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface ImageInfo : NSObject {
	@private
	NSString* url;
	NSString* key;	
}

@property (nonatomic, readonly) NSString* url;
@property (nonatomic, readonly) NSString* key;

- (id) initWithUrl:(NSString*)_url key:(NSString*)_key;

@end
