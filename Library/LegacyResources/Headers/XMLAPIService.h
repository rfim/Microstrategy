//
//  XMLAPIService.h
//  iPadMain Library
//
//  Created by Ji Jin on 8/24/10.
//  Copyright 2010 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSIServiceImpl.h"
#import "MSTRHandler.h"

@class MSIProjectInfo, SAXSupport;

@interface XMLAPIService : MSIServiceImpl <MSTRHandler> {
@protected
	MSIProjectInfo* project;
	NSInteger resultFlags;
	BOOL ignoreAccessProjectError;
}


@property (nonatomic, strong) MSIProjectInfo* project;
@property (nonatomic) NSInteger resultFlags;

@property (nonatomic, assign) BOOL shouldReceiveBinaryData;

/**
 * Whether or not we should ignore privilege error, for now, whether project access errors are viewed as invalid credentials errors
 */
@property BOOL ignoreAccessProjectError;

/**
 * To be implemented by subclasses. Generating XML command
 * 
 * @return XML Command
 */
-(NSString*) generateXMLCommand;

/**
 * Call back from XML parser when start element
 * @param elementName XML attribute element name
 * @param namespaceURI Namespace URI
 * @param attributeDict XML MSIAttribute dictionary
 * @param level XML element level
 * @param support XML parser pointer
 */
-(void) doXMLStartElement:(NSString *)elementName namespaceURI:(NSString *)namespaceURI attributes:(NSDictionary *)attributeDict level:(NSInteger)level support:(SAXSupport*)support;

/**
 * Call back from XML parser when start element
 * @param elementName XML attribute element name
 * @param namespaceURI Namespace URI
 * @param level XML element level
 * @param support XML parser pointer
 */
-(void) doXMLEndElement:(NSString *)elementName namespaceURI:(NSString *)namespaceURI level:(NSInteger)level support:(SAXSupport*)support;

/**
 * Handle success response
 */
-(void) propagateSuccess;

/**
 * Send network request to submit XML command
 */
-(void) sendXMLAPITaskRequest;

@end
