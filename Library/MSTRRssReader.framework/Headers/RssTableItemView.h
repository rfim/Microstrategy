//
//  RssTableItemView.h
//  RssFeedLib
//
//  Created by Nikolay Sazonov on 6/16/10.
//  Copyright 2010 EastBanc Technologies. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@class RssItem;
@class MSMRssDateUtils;

@protocol MSMRssMainController;

@interface MSMRssTableItemView : UIView {
	
}

@property (nonatomic, getter=isHighlighted) BOOL highlighted;
@property (nonatomic, strong) RssItem * item;
@property (nonatomic, getter=isEditing) BOOL editing;
- (id)initWithFrame:(CGRect)frame mainController:(id<MSMRssMainController>)rssMainController;
@end
