//
//  SummaryCell.h
//  MSTRMobile
//
//  Created by Amirali Charania on 10/28/09.
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface SummaryCell : UITableViewCell {
	UILabel* titleLabel;
	UILabel* valueLabel;
	NSString* title;
	NSString* value;
}

+ (CGFloat) getPreferredHeight:(NSString*)text;

@property (nonatomic, copy)  NSString* title;
@property (nonatomic, copy) NSString* value;

@end
