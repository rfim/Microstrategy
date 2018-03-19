//
//  RssButtonView.h
//  MSTRRssReader
//
//  Created by Baoming Tian on 11/22/11.
//  Copyright (c) 2011 EastBanc Technologies. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface MSMRssButtonView : UIView{

}
- (void)setImage:(UIImage *)image;
- (void)setTitle:(NSString*)title;
/*
 for click 
 @discussion: why not sub-class UIControl? search through the 
 internet can't find a solution
 */
- (void)addClickTarget:(id)target action:(SEL)action;
@end
