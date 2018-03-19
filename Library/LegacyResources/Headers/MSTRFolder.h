//
//  MSTRFolder.h
//
//	This simple class encapsulates a single folder in metadata.
//
//  Copyright 2010 MicroStrategy, Incorporated. All rights reserved.
//

#import "MSIObjectInfo.h"

@interface MSTRFolder : MSIObjectInfo <NSCoding> {
@private
	bool isLoaded;
    bool isDirty;
	NSInteger namedFolder;
	NSArray* elements;
}

// Properties...
@property (nonatomic, readonly) NSInteger namedFolder;
@property (nonatomic, readonly) bool isLoaded;
@property (nonatomic, strong, readonly) NSArray* elements;

@property (nonatomic) bool isDirty;

// Initializers...
-(id)initWithContents:(NSString*)name anID:(NSString*)dssid aDesc:(NSString*)desc aType:(NSInteger)tp aSubtype:(NSInteger)subtp aNamedFolderID:(NSInteger)folderName isShortcut:(BOOL)shortcut;

// Methods to set the contents of the folder...
-(void)setChildren:(NSArray*)children;

@end
