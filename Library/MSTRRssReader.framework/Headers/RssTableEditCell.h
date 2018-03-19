//
//  RssTableEditCell.h
//  RssFeedLib
//
//  Created by Nikolay Sazonov on 5/24/10.
//  Copyright 2010 EastBanc Technologies. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "RssFeed.h"
#import "RssTableViewModel.h"

@interface RssTableEditCell : UITableViewCell {
	@private
	NSInteger feedIndex;
	UIImage * imageSelected;
	UIImage * imageDeselected;
	RssTableViewModel * model;
}

-(id)initWithModel:(RssTableViewModel *) viewModel reuseIdentifier:(NSString *)reuseIdentifier;
-(void) setFeedIndex: (NSInteger) index;
-(void) switchFeedVisibility;


@end
