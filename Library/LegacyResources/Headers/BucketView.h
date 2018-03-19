//
//  BucketView.h
//  CalendarUIPrototype
//
//  Created by Firat Alpergin on 4/14/11.
//  Copyright 2011 MicroStrategy Incorporated. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "BucketCell.h"

#define BUCKET_LINE_WIDTH 8
#define BUCKET_LABEL_PADDING 5
#define BUCKET_LABEL_TOP_PADDING 5
#define BUCKET_BUTTON_HEIGHT 40

@class BucketElement, BucketCell;

@protocol BucketCallback

-(void)elementsRemoved:(NSArray *)elements;

@end

@interface BucketView : UIView <UITableViewDelegate, UITableViewDataSource, BucketCellDelegate> {
	@private
	UITableView *bucketTableView;
	NSMutableArray *elementList;
	BOOL inDeletionMode;
	UIButton *deleteButton;
	NSMutableArray *deletionList;
}

-(BOOL)add:(BucketElement *)newElement;
-(BOOL)remove:(BucketElement *)newElement;
//-(void)cellTapped:(BucketCell *)cell;

@end
