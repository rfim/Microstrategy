//
//  ViewerDataModel.h
//  IPadDashboardViewer
//
//  Created by bhuang on 3/24/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef enum EnumViewerDataType
{
	VDT_ITERATOR = 0,
	VDT_FIELDGROUPCELL,
	VDT_FIELDCELL
}EnumViewerDataType;

@class MSIDocumentContext;
@interface ViewerDataModel : NSObject {
    
    NSString* mpViewerID;
	void* mpViewerData;
	EnumViewerDataType mViewerDataType;
	void* mpField;	
	MSIDocumentContext* mpDocumentContext;
    CGFloat mScaleRatio;
}

/**
 * Add viewer id for viewer status restore.
 * The ID should follow G(Groupby section index)_(Node key), e.g., G4_K45.
 */
@property (nonatomic, copy)   NSString* mpViewerID;
@property (nonatomic) void* mpViewerData;
@property (nonatomic) EnumViewerDataType mViewerDataType;
@property (nonatomic) void* mpField;
@property (nonatomic, strong) MSIDocumentContext* mpDocumentContext;
@property (nonatomic) CGFloat mScaleRatio;
@property (nonatomic, copy) NSString *selectedVisStyle;
@property (nonatomic, copy) NSString *selectedVisClass;
@property (nonatomic, copy) NSString *selectedVisDropzone;
@property (nonatomic, copy) NSString *selectedVisEditormodel;
@property (nonatomic) void* mpNodeFormat;
@property (nonatomic) void* mpTitleFormat;

- (id)initWithData:(void*)ipViewerData andType:(EnumViewerDataType)iViewerDataType;
- (id)initWithData:(void*)ipViewerData andType:(EnumViewerDataType)iViewerDataType andField:(void*)ipField;
- (id)initWithData:(void*)ipViewerData andType:(EnumViewerDataType)iViewerDataType andContext:(MSIDocumentContext*)ipDocumentContext;

@end


