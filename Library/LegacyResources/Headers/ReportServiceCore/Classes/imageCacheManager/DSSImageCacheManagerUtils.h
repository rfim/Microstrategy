//
//  DSSImageCacheManagerUtils.h
//  ReportServiceCore
//
//  mzhu: this util class is added for .mm code to import from.  .m code cannot 
//  import "DSSDocumentInstance.h" directly because it is C++ code
//
//  Created by Mingjun Zhu on 6/22/10.
//  Copyright 2010 MSTR. All rights reserved.
//


#ifdef __OBJC__
#import <Foundation/Foundation.h>
#endif

@interface DSSImageCacheManagerUtils : NSObject {
	
}

#import "DSSDocumentInstance.h"

+(void) getImageNamesFromDocumentInstance:(DSSDocumentInstance*)ipDocInstance imageNames:(NSMutableArray*)iopImageNamesArray imageTimeStamps:(NSMutableArray*)iopImageTimeStampArray;
// +(void) getImageNamesFromDocumentInstance:(NSMutableArray*)iopImageNamesArray imageTimeStamps:(NSMutableArray*)iopImageTimeStampArray;

@end

