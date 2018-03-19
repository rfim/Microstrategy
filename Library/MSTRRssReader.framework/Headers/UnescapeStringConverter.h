//
//  UnescapeStringConverter.h
//  RssFeedLib
//
//  Created by Victor Shilo on 5/13/10.
//  Copyright 2010 EastBanc Technologies. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface MSMUnescapeStringConverter : NSObject<NSXMLParserDelegate> {

}

@property (nonatomic, strong) NSMutableString* result;

- (NSString*)convert:(NSString*)string;

@end
