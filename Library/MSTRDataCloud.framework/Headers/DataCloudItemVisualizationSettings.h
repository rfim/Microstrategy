//
//  ItemVisualizationSettings.h
//  DataCloudIPhone
//
//  Created by Svetlana Kopylova on 4/28/10.
//  Copyright 2010 EastBanc Technologies. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

/**
 * Visualization settings for displaying data cloud items
 */
@interface DataCloudItemVisualizationSettings : NSObject {
    
    UIColor *color1;
    UIColor *color2;
    UIColor *backgroundColor;
    UIColor *borderColor;
    UIFont  *font;
}

@property(nonatomic, strong) UIColor *color1;
@property(nonatomic, strong) UIColor *color2;
@property(nonatomic, strong) UIColor *backgroundColor;
@property(nonatomic, strong) UIColor *borderColor;
@property(nonatomic, strong) UIFont  *font;

- (id) init;

@end
