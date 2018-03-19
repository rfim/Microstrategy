//
//  PromptSummaryScreenCell.h
//  MSTRMobile
//
//  Created by Firat Alpergin on 2/16/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "PromptBaseCell.h"

#define ELEMENTS_TOP_PADDING 8
#define LEFT_PADDING 5

@class ElementsPrompt;

@interface PromptSummaryScreenCell : PromptBaseCell {
	NSInteger index;
	UILabel *indexLabel;
	//UILabel *requiredLabel;
	UILabel *promptDescriptionLabel;
	UILabel *promptAnswersLabel;
	UILabel *errorMessage;
	
	BOOL isDateType;
}

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier prompt:(ElementsPrompt *)elPrompt isDate:(BOOL)isDate;

@end
