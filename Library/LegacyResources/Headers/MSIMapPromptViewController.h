//
//  MapPromptViewController.h
//  MicroStrategyMobile
//
//  Created by xi zhang on 7/11/11.
//  Copyright 2011 MicroStrategy Inc. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "MIPromptDetailsViewController.h"
#import "MSIMapWidgetInfo.h"
#import "MapPromptView.h"
#import <Foundation/Foundation.h>

@class PromptHelper;
@class BucketHeader;
@class MIFilter;
@class ElementsPrompt;

@interface MSIMapPromptViewController : MIPromptDetailsViewController <MSIMapPromptViewDelegate> {
@private
	PromptHelper* __weak promptHelper;
	
	BucketHeader* dummyHeader;
	// map view for prompt.
	MapPromptView* mapPromptView; 
	// widget info, support list of elements.
    MSIMapWidgetInfo * widgetInfo;
	ElementsPrompt *__weak prompt;
	NSInteger elementType;
	
	MIFilter *__weak filter;
	
	// the expected structure is: 
	// [1] the key is the element id.
	// [2] the value is a dictionary, this dictionary will contain all of the info about the particular element.
	// [2.1] key - "id", value - element id.
	// [2.2] key - "lat", value - latitude value.
	// [2.3] key - "lng", value - longitude value.
	// [2.4] key - "name", value - to be shown in the info window.
	// [2.5] key - "selected", value - initially selected or not.
	NSDictionary* elementDictionary;
}


@property(nonatomic, weak) PromptHelper *promptHelper;
@property(nonatomic, strong) NSDictionary* elementDictionary;
@property(nonatomic, weak) MIFilter *filter;
@property(nonatomic, weak) ElementsPrompt *prompt;

-(void)setSelectedElements:(NSArray *)elements;

@end
