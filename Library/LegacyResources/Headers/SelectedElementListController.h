//
//  SelectedElementListController.h
//  MSTRMobile
//
//  Created by Amirali Charania on 10/16/09.
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <UIKit/UIKit.h>

@class PromptHelper, ObjectInfo;

@interface SelectedElementListController : UITableViewController {
	ObjectInfo* promptAttribute;
	PromptHelper* promptHelper;
	NSMutableArray* selectedElements;
	BOOL store;
	BOOL elementListScreenDisplayed;
	UILabel* noPersonalizationLabel;
}

@property (nonatomic, retain) ObjectInfo* promptAttribute;
@property (nonatomic, assign) PromptHelper* promptHelper;
@property (nonatomic, retain) NSMutableArray* selectedElements; 

@end
