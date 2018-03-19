//
//  ImageRequestImpl.h
//  MSTRMobile
//
//  Created by Firat Alpergin on 2/23/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSINetworkRequestImpl.h"

@class MSIProjectInfo;

@interface ImageRequestImpl : MSINetworkRequestImpl {
	NSString *imageURL;
	MSIProjectInfo *projectInfo;  
}

@property (nonatomic, copy) NSString *imageURL;
@property (nonatomic, strong) MSIProjectInfo *projectInfo;

@end
