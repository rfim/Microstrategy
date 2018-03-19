//
//  LayoutInfo.h
//  MSTRMobile
//
//  Created by Amirali Charania on 7/28/09.
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>

@class Layout;

@protocol LayoutInfoDelegate

//- (void) updateLayoutViewWithCurrentLayout: (Layout *) layout;
- (void) uncacheLayout;


@end

@interface MSILayoutInfo : NSObject {
	
	NSMutableDictionary* orientationMap;
	NSMutableArray* layouts;
	
	//! stores the current layout
	Layout *currentLayout;
	
//	//! stores the layout for a map info window (nil if there is none set)
//	/*! 
//	 TODO: rename this to currentTargetLayout
//	 */
//	Layout *targetLayout;
	
//	//! state information set by view controller
//	Layout *lastAccessedPortraitLayout;
//	
//	//! state information set by view controller
//	Layout *lastAccessedLandscapeLayout;
	
	//! indicates whether or not the layoutInfo must be refreshed
	BOOL fresh;
	
//	//! state information set by view controller
//	NSMutableArray* accessedLayouts;
	
	NSInteger idCounter;
	NSInteger layoutBarStyle;
	NSInteger layoutBarPosition;
	//! used for map widget target layout which should be hidden.
	NSMutableDictionary* hiddenLayouts;
	
@private
	//! unused for now
	id <LayoutInfoDelegate> __weak delegate; //may want a KVO-based approach for this in the future(?)
	
}

- (id) initWithData:(NSData*)data;
- (NSArray*) layoutsForOrientation:(NSInteger)orientation;
- (Layout*) layoutForID:(NSInteger)layoutID;
- (Layout*) givenOrNextLayoutForID:(NSInteger)layoutID orientation:(NSInteger)orientation;
- (NSInteger) countForOrientation:(NSInteger)orientation;
- (NSInteger) count;
- (NSInteger) exclusiveCountForOrientation:(NSInteger)orientation;
- (Layout*) nextLayout:(Layout*)layout orientation:(NSInteger)orientation;
- (Layout*) prevLayout:(Layout*)layout orientation:(NSInteger)orientation;
- (NSInteger) indexOfLayout:(Layout*)layout orientation:(NSInteger)orientation;

- (void) addToOrientationMap:(Layout*)layout;
- (void) addToHiddenLayouts:(Layout*)layout;
- (void) addDummyLayoutForNoLayout;
- (void) setPreferredLayouts;

//- (void)updateLayoutID:(NSInteger)layoutID;

// used to get the map widget (hidden) target layout.
- (Layout*) targetLayoutForKey:(NSString*)layoutKey; 
// used to get the number of all visible layouts.
- (NSInteger) visibleLayoutCount; 

//! sets the argument as currentLayout if it's not already the currentLayout
/*!
 Future plans are to make this call the delegate with a notification that the currentLayout has been set
// */
//- (void) updateLayoutID: (NSInteger) layoutID;

//! updates currentLayout to the lastAccessedPortraitLayout or lastAccessedLandscapeLayout dependent on the current orientation
//- (void) updateLayoutForOrientation: (NSInteger) orientation;


@property (nonatomic, weak) id <LayoutInfoDelegate> delegate; //assign to prevent circular references

@property (nonatomic, strong) Layout *currentLayout;
//@property (nonatomic, retain) Layout *targetLayout;

//@property (nonatomic, assign) Layout *lastAccessedPortraitLayout;
//@property (nonatomic, assign) Layout *lastAccessedLandscapeLayout;
//@property (nonatomic, assign) NSMutableArray *accessedLayouts;


@property NSInteger layoutBarStyle;
@property NSInteger layoutBarPosition;
@property (strong, readonly) NSMutableArray* layouts;

@property (nonatomic) BOOL fresh;

@end
