//
//  MSIMWAnnotationView.h
//  MSTRMobile
//
//  Created by xi zhang on 3/20/10.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <MapKit/MapKit.h>
#import "ImageCacheCallback.h"
@class MSIMWBaseView;

@class RoundedRectView;

@interface MSIMWAnnotationView : MKAnnotationView  <ImageCacheCallback> {
	
	MSIMWBaseView * __weak mainView;
	BOOL mHalfRangeHighlight;
	RoundedRectView* labelContainerView;
    BOOL cancelSelect;
    UILabel* clusterLabel;
    UILabel* debugLabel;
    
@protected
	BOOL modeSwitched;
	NSString* mAccLabel;
    NSString* imageURL;
    BOOL shouldAnimate;
    
@private
    NSString *highResImgURL;
}

-(id)initWithAnnotation:(id <MKAnnotation>)annotation reuseIdentifier:(NSString *)reuseIdentifier;
-(void)createImageWithImagePath:(NSString*)imagePath;
-(void)showSingleAnnotationWithImagePath:(NSString*)imagePath;
-(void)prepareRender;
-(void)updateLabelWithClusterInfo:(NSInteger)numOfAnnotations;
-(BOOL)touchInside:(CGPoint)point;
-(void)updateImage;
-(void)cancelImageRequestAndService;

-(void)addDebugInfo:(NSString*)debugInfo;
-(void)removeDebugInfo;
-(void)displayText;

- (NSString *)getKeyForColor:(UIColor *)color;

@property(nonatomic, weak) MSIMWBaseView * mainView;
@property(nonatomic, assign) BOOL mHalfRangeHighlight;
@property(nonatomic, copy) NSString* imageURL;
@property(nonatomic, assign) BOOL cancelSelect;
@property(nonatomic, assign) BOOL shouldAnimate;
@property(nonatomic, copy) NSString *highResImgURL;
@property(nonatomic, assign) BOOL modeSwithched;

@end
