//
//  MWShapeConfig.h
//  MicroStrategyMobile
//
//  Created by Zhang, Xi (Jason) on 1/9/14.
//  Copyright (c) 2014 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(int, GEO_ROLE) {
    GEO_ROLE_NONE = 0,
    GEO_ROLE_CITY = 1,
    GEO_ROLE_STATE = 2,
    GEO_ROLE_COUNTRY = 3,
    GEO_ROLE_LOCATION = 4,
    GEO_ROLE_LATITUDE = 5,
    GEO_ROLE_LONGITUDE = 6,
    GEO_ROLE_OTHER = 7,
    GEO_ROLE_ZIPCODE = 8,
    GEO_ROLE_COUNTY = 9
};

@interface MWShapeConfig : NSObject <NSCoding> {
    
    @private
    
    NSMutableArray* roles;
    NSMutableArray* configs;
    NSString* parentElement;

    // search type info.
    NSMutableDictionary* searchTypeByShapeType;
    
    // tiles.
    NSMutableDictionary* tileToShapeIdsByShapeType;
    
    // name-id lookup. name from grid, id format - an array of shape ids.
    NSMutableDictionary* nameToIdLookupByShapeType;
    NSMutableDictionary* idToNameLookupByShapeType;
    
    // hierarchy info. id format - shapeId, it is an id to id lookup.
    NSMutableDictionary* childToParentIdLookupByShapeType;

    // for example, 1003:1004:1005
    NSMutableArray* shapeTypeHierarchy;
    
    // for KML case.
    NSString* dataSourceKey;
    
    NSMutableDictionary* nameConversionRules;
    NSMutableString* ruleKey;
    NSString* ruleShapeId;
    NSString* ruleRole;
    NSString* ruleSfv;
    NSString* ruleValue;
    NSMutableSet* shapeNamesInRules;
    
    // for performance improvement.
    BOOL ignoreCaseCheck;
    NSString* currentShapeType;
    NSDictionary* currentNameToIdLookup;
}

- (id)initWithXML:(NSString*)xml;

- (BOOL)isCustomShapeForConfigId:(NSString*)configId;
- (NSString*)roleForConfigId:(NSString*)configId;
- (NSString*)configIdForRole:(NSString*)role;
- (NSString*)parentRoleForRole:(NSString*)role;
- (NSString*)shapeTypeForRole:(NSString*)role;
- (NSString*)searchTypeForRole:(NSString*)role;

- (BOOL)isValidShapeLookupForm:(int)geoRoleId;
- (BOOL)isNoneGeoRoleId:(int)geoRoleId;

- (NSString*)parentRoleForConfigId:(NSString*)configId;
- (NSString*)shapeTypeForConfigId:(NSString*)configId;
- (NSString*)searchTypeForConfigId:(NSString*)configId;

- (BOOL)isLookupAvailableForShapeTypes:(NSArray*)shapeTypes
                           searchTypes:(NSArray*)searchTypes;

- (BOOL)isTileAvailableForShapeType:(NSString*)shapeType;
- (void)populateLookupsWithResult:(NSDictionary*)result
                     forShapeType:(NSString*)shapeType
                      searchTypes:(NSDictionary*)searchTypes;

- (void)setPerformanceVariablesForShapeType:(NSString*)shapeType;

- (NSString*)convertShapeName:(NSString*)shapeName
                 withConfigid:(NSString*)configId
                   andGeoRole:(NSString*)geoRole
                       andSfv:(NSString*)sfv;

- (NSString*)shapeIdForShapeName:(NSString*)shapeName
                  withParentName:(NSString*)parentName
                   andParentType:(NSString*)parentType;

@property (nonatomic, readonly) NSMutableDictionary* tileToShapeIdsByShapeType;
@property (nonatomic, readonly) NSString* dataSourceKey;

@end
