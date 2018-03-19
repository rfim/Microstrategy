//
//  CAScreenshotLayer.h
//  MicroStrategyMobile
//
//  Created by Liang Chen on 8/24/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>

@protocol CAScreenshotLayerDelegate <NSObject>

- (UIImage*) getScreenshotImage;

@end

@interface CAScreenshotLayer : CALayer{
}
@property (nonatomic, weak) id<CAScreenshotLayerDelegate> mDelegate;
@end
