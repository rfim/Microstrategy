//
//  DSSDataConverter.h
//  DashboardViewer
//
//  Created by Xiaolong Wang on 2/19/10.
//  Copyright 2010 MicroStrategy, Inc.. All rights reserved.
//


#ifdef __OBJC__
#import <Foundation/Foundation.h>
#endif

@interface DSSDataConverter : NSObject {

}
+ (NSString*) stringValue4Data:(const void *)ipData Size:(int)iSizeInByte Type:(int)iDataType Flag:(int)iFlag;
+ (NSString*) stringValue4Data:(const void *)ipData Size:(int)iSizeInByte Type:(int)iDataType Flag:(int)iFlag NumericForDateTime:(BOOL) iUseNumeric;


@end
