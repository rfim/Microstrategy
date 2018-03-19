//
//  MarkRowView.h
//  iPadMain Library
//
//  Created by Ma Xianqing on 3/9/11.
//  Copyright 2011 MicroStrategy Inc. All rights reserved.
//

#import <UIKit/UIKit.h>
@class Commander;
class DSSRWGridIterator;



//RowLevel
//The MarkRow view status is insert/delete/unset
typedef enum EnumRowStatus{
	ROW_SELECT,
	ROW_UNSET,
}EnumRowStatus;

@protocol MarkRowViewDelegate

-(void) markRow: (int) iRow withStatus: (EnumRowStatus) iStatus;

@optional
- (Commander*) getCommander;

@end



@interface MarkRowView : UIView {
	UIImage* mpImage;
    UIImageView* mpImageView;
	EnumRowStatus mStatus;
	int mRow;
	id<MarkRowViewDelegate> __weak mDelegate;
}

@property (readonly) int mRow;
@property (strong) UIImage* image;  // TQMS 512671, retain image, yzeng
//iRow is 0 based`
- (id)initWithFrame:(CGRect)frame andStatus: (EnumRowStatus) iStatus forRow: (int) iRow inGrid: (id<MarkRowViewDelegate>)ipMarkRowViewDelegate;
- (void)loadImage: (EnumRowStatus) iStatus;
//helper method, move to appropriate place
+ (void) executeMarkRowManipulation: (int) iRow withStatus: (EnumRowStatus) iStatus withCommander:(Commander*)commander gridIterator:(DSSRWGridIterator*)gridIterator;
@end
