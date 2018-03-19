//
//  MWNetworkLayerDelegate.h
//  MicroStrategyMobile
//
//  Created by Hongchao Zhang on 12/10/15.
//  Copyright © 2015 MicroStrategy Inc. All rights reserved.
//

#ifndef MWNetworkLayerDelegate_h
#define MWNetworkLayerDelegate_h

@class MSIMWAnnotation;

@protocol MWNetworkLayerDelegate <NSObject>
- (NSInteger)getLayerIndex;
- (BOOL)currentLayerIsShown;
- (void)highlightAffinityEnd:(MSIMWAnnotation *)annotation selected:(BOOL)selected;

@end


#endif /* MWNetworkLayerDelegate_h */
