//
//  ElementSelectionsController.h
//  MSTRMobile
//
//  Created by Firat Alpergin on 3/16/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "OperatorCallback.h"

@class PromptHelper, ObjectInfo, Elements, ElementListController;

@interface ElementSelectionsController : UITableViewController <OperatorCallback> {
	NSMutableArray *elementList;
	PromptHelper *promptHelper;
	ObjectInfo* promptAttribute;
	NSString* shortFilterXML;
	NSArray* suggestedAnswers;
	NSString* barcodeFormId;
	NSString* geoMapping;
	NSString* geoAttributeId;
	NSString* dataSourcesXML;
	
	PromptStepperOperatorView* addView;
	PromptStepperOperatorView* deleteView;
	ElementListController* currentController;
}

@property (nonatomic, retain) NSMutableArray *elementList;
@property (nonatomic, assign) PromptHelper *promptHelper;
@property (nonatomic, retain) ObjectInfo* promptAttribute;
@property (nonatomic, retain) NSString* shortFilterXML;
@property (nonatomic, retain) NSArray* suggestedAnswers;
@property (nonatomic, retain) ElementListController* currentController;
@property (nonatomic, copy) NSString* barcodeFormId;
@property (nonatomic, copy) NSString* geoMapping;
@property (nonatomic, copy) NSString* geoAttributeId;
@property (nonatomic, copy) NSString* dataSourcesXML;

-(void)elementEditDone:(BOOL)shouldUpdate;

@end
