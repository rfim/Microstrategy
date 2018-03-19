//
//  BucketCell.h
//  CalendarUIPrototype
//
//  Created by Firat Alpergin on 4/14/11.
//  Copyright 2011 MicroStrategy Incorporated. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "BucketCellUIHelper.h"
@class BucketCell;

@protocol BucketCellDelegate <NSObject>

@optional

- (void)cellDelete:(BucketCell *)cell;
@end

@class BucketElement;

//@class MIDynamicExpandableTableViewCell;

@interface BucketCell : UITableViewCell {
	@private
	UIImageView *imageView;
    UILabel *nameLabel;
	BOOL beingTapped;
	BOOL isSelected;
	BucketElement *element;

	id<BucketCellDelegate> __weak delegate;
    BucketCellUIHelper* bucketCellUIhelper;
    
    UIImageView *dividerLine;
}

@property(nonatomic, weak) id<BucketCellDelegate> delegate;
@property(nonatomic, strong) BucketElement *element;

@property BOOL isSelected;
@property(nonatomic) UIImageView *imageView;
@property(nonatomic) UILabel *nameLabel;
@property(nonatomic) UIImageView *dividerLine;
- (id)initWithStyle:(UITableViewCellStyle)style selected:(BOOL)cellSelected element:(BucketElement *)elem UIHelper:(UIHelper*)uihelper;
-(id)initWithStyle:(UITableViewCellStyle)style selected:(BOOL)cellSelected element:(BucketElement *)elem;

@end
