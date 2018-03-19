//
//  BucketCellUIHelper.h
//  MicroStrategyMobile
//
//  Created by Cheng, Jun on 11/13/13.
//  Copyright (c) 2013 MicroStrategy Inc. All rights reserved.
//

#import "UIHelper.h"

@interface BucketCellUIHelper : UIHelper{
    ///imageview
    CGRect imageViewFrame;
    /// name Label
    CGRect nameLabelFrame;
    UIColor* nameLabelTextColor;
    UIColor* nameLabelBackgroundColor;
    UIFont* nameLabelFont;
    //// selectedImage
    NSString* selectedImageName;
    NSString* unseledtedImageName;
}
@property (nonatomic, readonly) CGRect imageViewFrame;
@property (nonatomic, readonly) CGRect nameLabelFrame;
@property (nonatomic, strong, readonly) UIColor* nameLabelTextColor;
@property (nonatomic, strong, readonly) UIColor* nameLabelBackgroundColor;
@property (nonatomic, strong, readonly) UIFont* nameLabelFont;
@property (nonatomic, strong, readonly) NSString* selectedImageName;
@property (nonatomic, strong, readonly) NSString* unseledtedImageName;
@end
