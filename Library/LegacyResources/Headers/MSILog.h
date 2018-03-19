//
//  MSILog.h
//  MSTRMobile
//
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>

@class MSILogger;


/**
 @see MSILogger
 */
@interface MSILog : NSObject {

}

+(MSILogger *)getLogger;

@end

#define SIMPLE_DEBUG_LOG(DEBUD_MSG, ...)\
if ([[MSILog getLogger] isLoggable:Debug]) { \
    [[MSILog getLogger] logWithLevel:Debug \
    className:NSStringFromClass([self class]) methodName:NSStringFromSelector(_cmd) \
    message:[NSString stringWithFormat:(@"{{%s at %s:%d thread=#%p self=%p}}" DEBUD_MSG), \
    __PRETTY_FUNCTION__, __FILE__, __LINE__, [NSThread currentThread], self, ## __VA_ARGS__]]; \
}
