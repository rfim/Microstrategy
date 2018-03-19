//
//  RssHeaderInSection.h
//  RssFeedLib
//
//  Created by Victor Shilo on 5/7/10.
//  Copyright 2010 EastBanc Technologies. All rights reserved.
//

#import <UIKit/UIKit.h>

@protocol MSMRssHeaderInSectionDelegate

-(void) rssHeaderInSectionTapped:(id)sender;

@end

@protocol MSMRssMainController;

@interface MSMRssHeaderInSection : UIView {
 
}

@property (nonatomic, strong) NSString * title;
@property (nonatomic, strong) UILabel * titleLabel;

- (id)initWithFrame:(CGRect)frame title:(NSString *)newTitle tag:(NSInteger) newTag delegate:(id<MSMRssHeaderInSectionDelegate>)newDelegate mainController:(id<MSMRssMainController>)rssMainController;

-(void)collapse;

-(void)expand;

@end


