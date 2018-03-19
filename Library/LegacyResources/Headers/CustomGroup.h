//
//  CustomGroup.h
//  MSTRMobile
//
//  Created by Amirali Charania on 2/20/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSIDisplayInfo.h"

@class MSICustomGroupItem;

@interface MSICustomGroup : MSIDisplayInfo {

}

- (MSICustomGroupItem*)customGroupItem;
- (int) customGroupItemCount;

@end
