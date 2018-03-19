//
//  AbstractTestCase.h
//  RssFeedLib
//
//  Created by Nikolay Sazonov on 7/7/10.
//  Copyright 2010 EastBanc Technologies. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <SenTestingKit/SenTestingKit.h>
#import <UIKit/UIKit.h>
#import "RssCoreDataService.h";

#define REPORT_ID @"C8B0EDED411688DF7F59289B1D09F756"
#define REPORT_TYPE 55

@interface AbstractTestCase : SenTestCase {
	
	@protected 
	RssCoreDataService * dataService;

}
@end

@interface RssCoreDataService (TestCategory)

- (void) clearManagedContext;

@end


