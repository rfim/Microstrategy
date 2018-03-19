//
//  CustomTableView.h
//  MSTRMobile
//
//  Created by Amirali Charania on 10/23/09.
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface CustomTableView : UITableView {
	BOOL overlayAdded;
	BOOL searchTableViewAdded;
	BOOL cancelClicked;
}

@property BOOL cancelClicked;

@end
