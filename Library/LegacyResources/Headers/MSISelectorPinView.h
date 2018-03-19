//
//  SelectorPinView.h
//  MicroStrategyMobile
//
//  Created by Firat Alpergin on 6/10/11.
//  Copyright 2011 MicroStrategy Inc. All rights reserved.
//

#import <UIKit/UIKit.h>

#define PIN_TYPE_LEFT 0
#define PIN_TYPE_RIGHT 1
#define PIN_CIRCLE_SIZE 11

@class MSISelectorPinView;

@protocol SelectorPinDelegate

-(void)touchesBeganOnPin:(MSISelectorPinView *)pin;
-(void)touchesEndedOnPin:(MSISelectorPinView *)pin;

@end

@interface MSISelectorPinView : UIView {
	@private
	NSInteger pinType;
	id<SelectorPinDelegate> __weak delegate;
}

@property NSInteger pinType;
@property(nonatomic, weak) id<SelectorPinDelegate> delegate;

-(id)initWithFrame:(CGRect)frame pinType:(NSInteger)pType;

@end
