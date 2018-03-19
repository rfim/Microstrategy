//
//  UnsupportedPrompt.h
//  MSTRMobile
//
//  Created by Ilia Baskin on 4/27/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSIPrompt.h"

@class XMLBuilder;

@interface UnsupportedPrompt : MSIPrompt {

@private
	NSString* answerXml;
	BOOL inPa;
	XMLBuilder* myBuilder;
}

@end
