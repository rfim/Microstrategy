//
//  ConsolidationHeader.h
//  MSTRMobile
//
//  Created by Amirali Charania on 2/20/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ElementHeader.h"

@class MSIConsolidation;

@interface MSIConsolidationHeader : MSIElementHeader {
}

@property (nonatomic, strong, readonly) MSIConsolidation* consolidation;
@property (nonatomic)  int formID;

@end
