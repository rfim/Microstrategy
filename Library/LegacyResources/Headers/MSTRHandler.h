/*
 *  MSTRHandler.h
 *  MSTR Foundation
 *
 *  Created by Amirali Charania on 2/23/09.
 *  Copyright 2009 MicroStrategy. All rights reserved.
 *
 */

@class SAXSupport;

@protocol MSTRHandler

-(void)doStartElement:(NSString *)elementName namespaceURI:(NSString *)namespaceURI attributes:(NSDictionary *)attributeDict level:(NSInteger)level support:(SAXSupport*)support;
-(void)doEndElement:(NSString *)elementName namespaceURI:(NSString *)namespaceURI level:(NSInteger)level support:(SAXSupport*)support;

@end
