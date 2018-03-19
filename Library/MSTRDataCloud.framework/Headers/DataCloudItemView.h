//
//  DataCloudItemView.h
//  DataCloudIPhone
//
//  Created by Svetlana Kopylova on 4/28/10.
//  Copyright 2010 EastBanc Technologies. All rights reserved.
//

#import <UIKit/UIKit.h>

@class DataCloudItem;

/**
   DataCloudItemView implements representation of DataCloudItem element
*/
@interface DataCloudItemView : UILabel {
	// item contains data to draw
	DataCloudItem *item;
	int descender;
	BOOL selected;
    BOOL hasGlow;
	int interval;
	int leading;
}

@property (nonatomic, strong) DataCloudItem *item;
@property int descender;
@property BOOL selected;
@property BOOL hasGlow;
@property int interval;
@property int leading;

/**
 Draws element on screen based on item's size and coordinate.
 */
-(void) setFrame:(CGRect) rect;
-(BOOL) selected;
-(void) setSelected:(BOOL) value;

@end
