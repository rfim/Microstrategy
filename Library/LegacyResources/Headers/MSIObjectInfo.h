//
//  ObjectInfo.h
//  MSTRMobile
//
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Enums.h"


#define HIERARCHY_TYPE 14
#define ATTRIBUTE_TYPE 12
#define ELEMENT_TYPE 1048576

@class SAXSupport, XMLBuilder;
@protocol MSTRHandler;

/**
 * ObjectInfo is the base class of MicroStrategy object. It contains the object information, methods to serialize and deserialize these information.
 */
@interface MSIObjectInfo : NSObject <NSCopying, NSCoding> {
	
@private
	NSString* objectID;
    
    /**
     * This member is added to support representing a named folder with an ObjectInfo, it is NOT serialized to XML,
     * however it is saved in archive (NSCoding) and copied over (NSCopying).  The default value is negative
     * one (-1) which represents invalid named ID.
     */
    NSInteger namedID;
	NSString* name;
	ObjectTypes type;
	ObjectSubTypes subType;
	NSString* displayName;
	NSString* desc;
	NSString* abbreviation;		
	NSString* iconPath;
	NSString* owner;
	BOOL isShortcut;
	NSInteger accessGranted;
	NSInteger viewMedia;
    NSInteger availableViewMedia;
    NSString* sta;
    // Parent Folder
    MSIObjectInfo* parentFolder;

    /**
     * Represents an external path of the object.  This is added to support WebDAV.
     */
    NSString* mpExternalPath;
    NSInteger mExternalObjectType;
    NSDate* mpCreationDate;
    NSDate* mpModificationDate;
    // TQMS:602916. Add this member to track the binary status of web dav file/folder.
    int mBinaryStatus;
}

/**
 * Initializer a ObjectInfo with type.
 * Subtype will default to 0. If the type doesn't have different subtypes, the subtype will also be initialized.
 * @param _type one of the ObjectTypes in enums.h
 * @return ObjectInfo
 * @see ObjectTypes
 */
-(id)initWithType:(ObjectTypes) _type;

/**
 * Initializer a ObjectInfo with type and subtype
 * @param _type one of the ObjectTypes in enums.h
 * @param _subType one of the ObjectSubTypes in enums.h
 * @return ObjectInfo
 * @see ObjectTypes
 */
-(id)initWithType:(ObjectTypes) _type subType:(ObjectSubTypes) _subType;

/**
 * Compare the object name with another object. The names are compared with numeric order, case insensitive.
 * @param object object to be compared
 * @return NSOrderedAscending if the name is smaller than the name to be compared
 *			NSOrderedSame if the two objects has the same name
 *			NSOrderedDecending if the name is larger than the name to be compared
 * @see ObjectInfo
 */
-(NSComparisonResult)compareElementName:(MSIObjectInfo*)object;

/**
 * Compare the object ID with another object. The ID are compared with numeric order, case insensitive.
 * @param object object to be compared
 * @return NSOrderedAscending if the ID is smaller than the ID to be compared
 *			NSOrderedSame if the two objects has the same ID
 *			NSOrderedDecending if the ID is larger than the ID to be compared
 * @see ObjectInfo
 */
-(NSComparisonResult)compareElementID:(MSIObjectInfo*)object;

/**
 * Read the object info from xml
 * @param elementName xml element name
 * @param attributes attributes of this element
 * @param support XML SAX parser
 */
-(void)handleXml:(NSString*)elementName attributes:(NSDictionary*)attributes  support:(SAXSupport*)support;

/**
 * Get the handler which can parse the ObjectInfo xml.
 * Subclass should implement this method to return the appropriate handler if subclass have more content in xml.
 * @return MSTRHandler xml handler
 */
-(id<MSTRHandler>)getXmlHandler;

/**
 * Read attributes of this element to object info.
 * If subclass has more attributes, it needs to overwrite this method.
 * @param elementName xml element name
 * @param attributes attributes of this element
 */
-(void)handleXmlAttributes:(NSString*)elementName attributes:(NSDictionary*)attributes;

/**
 * Load attributs of this element to object info. It has the same functionality as handleXMLAttributes.
 * @param elementName xml element name
 * @param attributes attributes of this element
 */
-(void)loadFromXML:(NSString*)elementName attributes:(NSDictionary*)attributes;

/**
 * Get the xml string of this object info
 * @return object info xml representation
 */
-(NSString*)getXml;

/**
 * Write the object info to xml format.
 * It will add tag for the object depending on the type.
 * @see buildXml:tagName
 * @param builder xml builder
 */
-(void)buildXml:(XMLBuilder*)builder;

/**
 * Write the object info to xml format.
 * It will add tag for the object.
 * @param builder xml builder
 * @param tagName xml tag name for the object
 */
-(void)buildXml:(XMLBuilder*)builder tagName:(NSString*)tagName;

/**
 * Write special info of subclass to xml.
 * Subclass should overwrite this method if it has more information.
 * @param builder xml builder
 */
-(void)buildTypeSpecificXml:(XMLBuilder*)builder;

/**
 * Static method to create a ObjectInfo
 * @param type object type
 * @return Folder if the object type is ObjectTypeFolder, otherwise, return the base class
 */
+(MSIObjectInfo*)newObject:(ObjectTypes)type;

/**
 * Get subtype from type. It only works if the type doesn't have different subtypes, or it returns 0.
 * @param type object type
 * @param object subtype
 */
+(ObjectSubTypes) subTypeFromType:(ObjectTypes)type;

/**
 * ID of this object. It's unique within a MicroStrategy project.
 */
@property (nonatomic, copy) NSString* objectID;

/**
 * ID of named folder
 */
@property (nonatomic) NSInteger namedID;

/**
 * object name
 */
@property (nonatomic, copy) NSString* name;

/**
 * object display name
 */
@property (nonatomic, copy) NSString* displayName;

/**
 * object type
 */
@property ObjectTypes type;


@property (nonatomic, copy) NSString* sta;
/**
 * object subtype
 */
@property ObjectSubTypes subType;

/**
 * Access right granted on this object for the login user
 * The value can be the combination of AccessRightFlags.
 */
@property NSInteger accessGranted;

/**
 * View mode of this object.
 */
@property NSInteger viewMedia;

@property NSInteger availableViewMedia;

/**
 * object description
 */
@property (nonatomic, copy) NSString* desc;

/**
 * object abbreviation
 */
@property (nonatomic, copy) NSString* abbreviation;

/**
 * icon path of this object
 */
@property (nonatomic, copy) NSString* iconPath;

/**
 * Owner of this object. It's the user's name who created this object.
 */
@property (nonatomic, copy) NSString* owner;

/**
 * Whether this object is a shortcut.
 */
@property (nonatomic) BOOL isShortcut;

/**
 * path to the resource of the external object
 */
@property (nonatomic, copy) NSString* externalPath;

/*
 * type of external object
 */
@property (nonatomic) NSInteger externalObjectType;

/**
 * object creation date
 */
@property (nonatomic, strong) NSDate* creationDate;

/**
 * last modification date
 */
@property (nonatomic, strong) NSDate* modificationDate;

/**
 * Binary status. For webdav folder and webdav file
 */
@property (nonatomic) int binaryStatus;

/*
 * Parent folder.
 */
@property (nonatomic, strong) MSIObjectInfo* parentFolder;

/*
 * Container ID. For nested object.
 */
@property (nonatomic, strong) NSString* containerId;

/*
 * Container type. For nested object.
 */
@property (nonatomic, strong) NSString* containerType;

/** 
 * Initializer with object information
 */
-(id)initWithContents:(NSString*)nm anID:(NSString*)did aDesc:(NSString*)d aType:(ObjectTypes)tp aSubtype:(ObjectSubTypes)subtp 
			 anAbbrev:(NSString*)abbr isShortcut:(BOOL)shortcut iconPath:(NSString*)_iconPath;
/**
 * Intializer with named object information
 */
-(id)initWithContents:(NSString*)nm namedID:(NSInteger)_namedID aDesc:(NSString*)d aType:(ObjectTypes)tp aSubtype:(ObjectSubTypes)subtp 
			 anAbbrev:(NSString*)abbr isShortcut:(BOOL)shortcut iconPath:(NSString*)_iconPath;


@end
