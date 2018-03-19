//
//  MSICustomizableAuthenticationPromptView.h
//  MicroStrategyMobile
//
//  Created by Wang, Wenyin on 5/4/14.
//  Copyright (c) 2014 MicroStrategy Inc. All rights reserved.
//

#import "MSIMobileLoginPromptView.h"

@interface MSICustomizableAuthenticationPromptView : MSIMobileLoginPromptView

@property (nonatomic, strong) UILabel* sectionTitleLabel;
@property (nonatomic, strong) UILabel* sectionSubtitleLabel;
@property (nonatomic, strong) UILabel* footerLabel;
@property (nonatomic, strong) UILabel* forgetPasswordLabel;
@property (nonatomic, strong) UIButton* loginButton;
@property (nonatomic, strong) UIButton* cancelButton;
@property (nonatomic, strong) UITableView *tableView;

- (void)setFrame:(CGRect)_frame;

@end
