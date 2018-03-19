//
//  RssDataServiceTest.h
//  RssFeedLib
//
//  Created by Andrey Andreev on 4/29/10.
//  Copyright 2010 EastBanc Technologies. All rights reserved.
//

#import <SenTestingKit/SenTestingKit.h>
#import "RssDataService.h"
#import "RssCacheManager.h"
#import "RssDownloader.h"

@interface RssDataServiceTest : SenTestCase<RssDataDelegate, DownloaderDelegate> {
	
}

@end
