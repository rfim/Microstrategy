//
//  Dimty.h
//  MSTRMobile
//
//  Created by Ilia Baskin on 2/16/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>

@class SAXSupport, XMLBuilder;

@interface Dimty : NSObject <NSCopying>{
	NSString* dimtyXml;
	XMLBuilder* builder;
	BOOL isNonEmpty;	
}

@property BOOL isNonEmpty;

-(void) handleXml:(NSString*)elementName attributes:(NSDictionary*)attributeDict support:(SAXSupport*)support;
-(void)buildXml:(XMLBuilder*)builder;

@end
