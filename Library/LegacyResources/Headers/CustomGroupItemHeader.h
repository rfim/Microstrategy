//
//  CustomGroupItemHeader.h
//  MSTRMobile
//
//  Created by Amirali Charania on 2/22/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ElementHeader.h"

@class MSICustomGroup, MSICustomGroupItem;

@interface MSICustomGroupItemHeader : MSIElementHeader {

}

- (MSICustomGroupItem*)customGroupItem;
@property (nonatomic, strong, readonly) MSICustomGroup* customGroup;

@end
