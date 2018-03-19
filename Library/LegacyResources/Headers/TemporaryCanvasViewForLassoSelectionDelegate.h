//
//  TemporaryCanvasViewForLassoSelectionDelegate.h
//  MicroStrategyMobile
//
//  Created by Clark Zhu on 4/12/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol TemporaryCanvasViewForLassoSelectionDelegate <NSObject>

- (void) touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event;

- (void) touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event;

- (void) touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event;

- (void) clearLassoSelection;

- (void) handleLassoSelections:(NSMutableArray *)iPointArray;

@end
