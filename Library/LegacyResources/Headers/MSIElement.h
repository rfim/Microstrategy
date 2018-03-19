//
//  Element.h
//  MSTRMobile
//
//  Created by Amirali Charania on 10/16/09.
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSISelectorItemProtocol.h"

@class XMLBuilder;	
@interface MSIElement : NSObject <NSCopying, NSCoding, MSISelectorItemProtocol> {
	NSString* name;
	NSString* elementID;
    NSString* formID;
	NSInteger type;
	NSInteger arity;
    NSString* weight;
}
-(void)loadFromXML:(NSString*)elementName attributes:(NSDictionary*)attributes;
-(void)buildXml:(XMLBuilder*)builder;
-(NSComparisonResult)compareElementName:(MSIElement*)element;
-(NSComparisonResult)compareElementID:(MSIElement*)element;
@property (nonatomic, copy) NSString* name;
@property (nonatomic, copy) NSString* elementID;
@property (nonatomic, copy) NSString* formID;
@property (nonatomic, copy) NSString* weight;
@property NSInteger type;

@end

