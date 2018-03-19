//
//  DataCloudAttributeElement.h
//  DataCloudIPhone
//
//  Created by Svetlana Kopylova on 4/29/10.
//  Copyright 2010 EastBanc Technologies. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface DataCloudAttribute : NSObject {
    NSString *name;
    NSString *value;
    NSString *attrElementID;
}
-(NSString*) getValueForAccessibility;

@property(nonatomic, strong) NSString *name;
@property(nonatomic, strong) NSString *value;
@property(nonatomic, strong) NSString *attrElementID;
@end
