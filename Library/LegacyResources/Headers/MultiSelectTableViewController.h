//
//  MultiSelectTableViewController.h
//  MSTRMobile
//
//  Created by Firat Alpergin on 4/9/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <UIKit/UIKit.h>

#define imageViewTag	1000
#define cellLabelTag	1001

#define labelIndentedRect	CGRectMake(40, 0, 480, 40)
#define labelRect			CGRectMake(15, 0, 480, 40)

@interface MultiSelectTableViewController : UITableViewController {
	@protected
		BOOL inEditMode;
		UIImage *selectedImage;
		UIImage *unselectedImage;
		NSMutableArray *selectedArray;
		BOOL shouldAnimateRows;
}

-(NSInteger)getNumberOfCellsInSection:(NSInteger)section;
-(NSInteger)getNumberOfSections;
-(NSString *)getTextForCellAtIndexPath:(NSIndexPath *)indexPath;
-(void)populateSelectedArray;
-(void)didSelectRowAtIndexPath:(NSIndexPath *)indexPath;
-(NSInteger)getCurrentSelectionCount;
-(void)updateCell:(UITableViewCell *)cell atIndexPath:(NSIndexPath *)indexPath;

@property (nonatomic, retain) UIImage *selectedImage;
@property (nonatomic, retain) UIImage *unselectedImage;
@property (nonatomic, retain) NSMutableArray *selectedArray;

@end
