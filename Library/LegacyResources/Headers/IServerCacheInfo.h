//
//  IServerCacheInfo.h
//  MSTRMobile
//
//  Created by Glenn Boysko on 4/16/10.
//  Copyright 2010 MicroStrategy, Incorporated. All rights reserved.
//

#import <Foundation/Foundation.h>


@class XMLBuilder;

@interface IServerCacheInfo : NSObject {
@private
	// Used only to track report store information during reconciliation
	NSString* cacheRootID;
	NSString* promptAnswers;
	NSString* reportID;
	
	// Used for all cases...
	NSString* cID;
	long long lastUpdate;
	NSInteger type;
	long long expireTime;
	NSInteger status;
	
	// Only to record server-side cache info...
	NSInteger errorNumber;
}

// Properties...
@property (nonatomic, readonly, copy) NSString* cID;
@property (nonatomic, readonly) long long lastUpdate;
@property (nonatomic, readonly) NSInteger type;
@property (nonatomic, readonly) NSInteger errorNumber;
@property (nonatomic, copy) NSString* cacheRootID;
@property (nonatomic, copy) NSString* promptAnswers;
@property (nonatomic, copy) NSString* reportID;
@property (nonatomic, readonly) long long expireTime;
@property NSInteger status;

// Initializers...
-(id)initWithDict:(NSDictionary*)dict;
-(id)initWithString:(NSString*)str;
-(void)buildXML:(XMLBuilder*)builder;
-(id)initWithData:(NSString*)cid type:(int)iType lastUpdate:(long long)updateTime expireTime:(long long)expire;

// Instance methods...
-(NSString*)serialize;

@end
