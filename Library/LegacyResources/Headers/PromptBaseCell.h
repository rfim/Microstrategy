//
//  PromptBaseCell.h
//  MSTRMobile
//
//  Created by Firat Alpergin on 2/25/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <UIKit/UIKit.h>

#define TOP_PADDING 12
#define LEFT_PADDING 5
#define RIGHT_PADDING 5
#define ERROR_LABEL_HEIGHT 25

@class Prompt;

/**
 This is used by the UITableView of the PromptSummaryScreen, where each prompt has a cell in the table.
 @see PromptFormattableCell, the subclass.
 */
@interface PromptBaseCell : UITableViewCell {
	Prompt *prompt;
	
	UILabel *titleLabel;
	UILabel *errorLabel;
	UILabel *requiredLabel;
}

-(void)populate;

/**
 Does nothing. Subclasses need to provide the view content that is appropriate for that type of prompt.
*/
- (void) addContent;

/**
 Adds the UILabel "This prompt is required." to the cell
 @return the right edge of the UILabel
 */
- (NSInteger) addRequiredLabel;


/**
 Added a UILabel with an error message with red text from prompt.errorDetails.errorMsg
 */
- (void) addErrorMessage;

-(NSInteger)rightMargin;
-(void)didRotateFromInterfaceOrientation;

@property (nonatomic, retain) Prompt *prompt;

@end
