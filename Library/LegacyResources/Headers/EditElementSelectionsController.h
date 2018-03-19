//
//  EditElementSelectionsController.h
//  MSTRMobile
//
//  Created by Firat Alpergin on 3/16/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "MultiSelectTableViewController.h"

@class ElementSelectionsController, ElementListController, MultiDeleteButton;

@interface EditElementSelectionsController : MultiSelectTableViewController {
	NSMutableArray *elementList;
	ElementSelectionsController *parent;
	ElementListController *currentController;
	
	MultiDeleteButton *deleteButton;
	UIButton *selectAllButton;
}

-(id)initWithStyle:(UITableViewStyle)style selectedElements:(NSMutableArray *)elementList;
-(void)elementSelectionCancelled;
-(void)elementSelectionDone;
-(void)setupToolbar;
-(void)updateDeleteButtonLabel:(NSInteger)newSelectionCnt;
-(void)updateSelectAllButtonLabel:(NSInteger)newSelectionCnt;

@property (nonatomic, retain) NSMutableArray *elementList;
@property (nonatomic, assign) ElementSelectionsController *parent;
@property (nonatomic, retain) ElementListController *currentController;

@end
