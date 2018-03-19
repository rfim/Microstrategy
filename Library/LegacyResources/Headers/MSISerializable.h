/*
 *  XMLSerializer.h
 *  MSTR Foundation
 *
 *  Created by Amirali Charania on 3/9/09.
 *  Copyright 2009 MicroStrategy. All rights reserved.
 *
 */
#import "MSTRHandler.h"

@class SAXSupport, XMLBuilder;

@protocol MSISerializable

-(void)loadFromXML:(id<MSTRHandler>)handler elementName:(NSString*)elementName attributes:(NSDictionary*)attributes support:(SAXSupport*)support context:(NSObject*)context;
-(void)buildXML:(XMLBuilder*)builder;

@end
