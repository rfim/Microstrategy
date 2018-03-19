//
//  SAXSupport.h
//  MSTR Foundation
//
//  Created by Amirali Charania on 2/23/09.
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSTRHandler.h"

@class Stack;

@interface SAXSupport : NSObject <NSXMLParserDelegate>{
@private 
	NSMutableString* buffer;
	Stack* handlerStack;
	Stack* levelStack;
	id<MSTRHandler> currentHandler;
	NSInteger currentLevel;
	NSXMLParser *parser;
	BOOL logEnabled;	//error log control
}

@property (nonatomic, copy) NSMutableString* buffer;
@property (nonatomic, strong) id<MSTRHandler> currentHandler;
@property BOOL logEnabled;

-(SAXSupport*)initWithData:(NSData*)data handler:(id<MSTRHandler>) aHandler;
-(void)start;
-(void)pushContentHandler:(id<MSTRHandler>)handler;
+(NSInteger)intValue:(NSDictionary*)attributes attName:(NSString*)attName;

@end

