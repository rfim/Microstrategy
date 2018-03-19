//
//  CustomGroupHeader.h
//  MSTRMobile
//
//  Created by Amirali Charania on 2/20/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ElementHeader.h"

@class MSICustomGroup, MSICustomGroupItem;

@interface MSICustomGroupHeader : MSIElementHeader {

}

@property (nonatomic, strong, readonly) MSICustomGroup* customGroup;

- (MSICustomGroupItem*)customGroupItem;

@end
