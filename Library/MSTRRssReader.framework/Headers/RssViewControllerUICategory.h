/*
 *  RssViewControllerUICategory.h
 *  RssFeedLib
 *
 *  Created by Nikolay Sazonov on 7/12/10.
 *  Copyright 2010 EastBanc Technologies. All rights reserved.
 *
 */

@interface RssViewController (UICategory)

-(void) switchToEditView;
-(void) switchToNormalView;
-(void) willSwitchView: (UIViewController *)vc1 withView:(UIViewController*)vc2 
			transition:(UIViewAnimationTransition) transition;
-(void) showActivityIndicator;
-(void) hideActivityIndicator;
-(void) loadFeedDetails: (RssFeed *) feed;

@end

