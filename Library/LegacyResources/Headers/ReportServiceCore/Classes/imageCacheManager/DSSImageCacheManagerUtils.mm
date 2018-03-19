//
//  DSSImageCacheManagerUtils.mm
//  ReportServiceCore
//
//  Created by Mingjun Zhu on 6/22/10.
//  Copyright 2010 MSTR. All rights reserved.
//

#import "DSSDocumentInstance.h"
#import "DSSImageCacheManagerUtils.h"

@implementation DSSImageCacheManagerUtils

+(void) getImageNamesFromDocumentInstance:(DSSDocumentInstance*)ipDocInstance imageNames:(NSMutableArray*)iopImageNamesArray imageTimeStamps:(NSMutableArray*)iopImageTimeStampArray
{
	if (ipDocInstance == nil || 
		iopImageNamesArray == nil ||
		iopImageTimeStampArray == nil)
		return;

	DSSObjectContext* lpObjectContext = ipDocInstance->getObjectContext();
	if (lpObjectContext == nil)
		return;
	// std::map<std::string, std::string> lImagesMap = lpObjectContext->getImageMap();
	ImagePathMap* lImagesMap = lpObjectContext->getImageMap();
	ImagePathMap::iterator lIter = lImagesMap->begin();
	NSDateFormatter *df=[[NSDateFormatter alloc] init];
	[df setDateFormat:@"yyyy-MM-dd HH:mm:ss.SSS"];
	
	while (lIter != lImagesMap->end())
	{
		std::string lImageName = lIter->first;
		std::string lImageTimeStamp = lIter->second;
		{
			// mzhu 2010-06-09: testing code to retrieve missing images for document
			NSString* lNSImageName = [NSString stringWithCString:lImageName.c_str() encoding:NSASCIIStringEncoding];
			NSString* lNSImageTimeStamp = [NSString stringWithCString:lImageTimeStamp.c_str() encoding:NSASCIIStringEncoding];
			NSDate *lNSDate = [df dateFromString:lNSImageTimeStamp];
			
			[iopImageNamesArray addObject: lNSImageName];
			[iopImageTimeStampArray addObject: lNSDate];
		}					
		++lIter;
	}
	
}

/*
+(void) getImageNamesFromDocumentInstance:(NSMutableArray*)iopImageNamesArray imageTimeStamps:(NSMutableArray*)iopImageTimeStampArray
{
}
*/
@end

