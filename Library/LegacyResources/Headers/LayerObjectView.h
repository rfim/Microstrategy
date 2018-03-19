//
//  LayerObjectView.h
//  IPhoneChart
//
//  Created by Liu Liang on 4/28/10.
//  Copyright 2010 MSTR. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface LayerObjectView : UIView {
	void* mpGraphObjectManager;
	void* mpLayerObject;
	double mInitialTime;
	double mProgress;
	double mDelayRef;
}

@property (nonatomic, assign) void* mpGraphObjectManager;
@property (nonatomic, assign) void* mpLayerObject;

-(void) detach;

@end
