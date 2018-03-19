//
//  AnnotationContext.h
//  MicroStrategyMobile
//
//  Created by Han Sun on 12/12/11.
//  Copyright (c) 2011 MicroStrategy Inc. All rights reserved.

#import <Foundation/Foundation.h>

// The maximum number of comment box on iPad and iPhone. Defined in AnnotationCanvasViewer.mm
extern const int ANNOTATION_MAXIMUM_COMMENT_IPAD;
extern const int ANNOTATION_MAXIMUM_COMMENT_IPHONE;

// Represent the current annotation mode. yzeng
typedef enum AnnotationMode {
    AnnotationModeUnspecified = -1,// initial value
	AnnotationModeDraw = 0,        // draw mode
	AnnotationModeComment = 1,     // comment mode
    AnnotationModeLockDrawing = 2, // a mode that user can move his finger on the screen but will not trigger drawing.
} AnnotationMode;

// Represent the current chose shape. yzeng
typedef enum AnnotationShape {
    AnnotationShapeUnspecified = -1,            // initial value
	AnnotationShapeAdHocStroke = 0,             // free drawing
    AnnotationShapeHandWritingOval = 1,        // a fancy hand-writing oval
    AnnotationShapeOval = 2,                    // oval
    AnnotationShapeRectangle = 3,               // rectangle
    AnnotationShapeRoundedCornerRectangle = 4,  // straight line
    AnnotationShapeStraightLine = 5,
} AnnotationShape;

// Represent the stroke style. yzeng
typedef enum AnnotationStrokeStyle {
	AnnotationStrokeStyleNoAngle = 0,            // a stroke always with the same width
    AnnotationStrokeStyleAngled = 1,             // a stroke considering angle like a pen
} AnnotationStrokeStyle;

// Represent the current used spec. This is just a temporary enum, and should not used in product. yzeng
typedef enum AnnotationSpec {
	AnnotationSpecPM = 0, 
	AnnotationSpecNingning = 1,
    AnnotationSpecFinal = 2,
} AnnotationSpec;

@interface AnnotationContext : NSObject <NSCopying, NSCoding>{
    AnnotationMode mAnnotationMode;
    
    // For draw (shape)
    AnnotationShape mAnnotationShape;
    UIColor* mStrokeColor;
    CGFloat mStrokeWidth;
    AnnotationStrokeStyle mStrokeStyle;
    
    // For comment box
    UIColor* mTextColor;
    CGFloat mFontSize;
    NSString* mFontName;
    UIColor* mCommentBoxBackgroundColor;
    
    AnnotationSpec mAnnotationSpec;
}

@property (nonatomic) AnnotationMode annotationMode;
@property (nonatomic) AnnotationShape annotationShape;
@property (nonatomic, strong) UIColor* strokeColor;
@property (nonatomic) CGFloat strokeWidth;
@property (nonatomic) AnnotationStrokeStyle strokeStyle;
@property (nonatomic, strong) UIColor* textColor;
@property (nonatomic) CGFloat fontSize;
@property (nonatomic, strong) NSString* fontName;
@property (nonatomic, strong) UIColor* commentBoxBackgroundColor;
@property (nonatomic) AnnotationSpec annotationSpec;

+ (UIColor*)getItemsTintColor;
@end
