//
//  RWSaveAsDocumentService.h
//  MicroStrategyMobile
//
//  Created by Sun, Han on 5/8/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "XMLAPIService.h"
#import "DSSIPhoneEnums.h"

@class MSIReportDisplayInfo;

@interface RWSaveAsDocumentService : XMLAPIService{
    MSIReportDisplayInfo* document;
    NSString* folderID;
    NSString* docName;
    NSString* docDescription;
    EnumDSSXMLDocSaveAsFlags flag;
}
@property (nonatomic, strong) MSIReportDisplayInfo* document;
@property (nonatomic, copy) NSString* folderID;
@property (nonatomic, copy) NSString* docName;
@property (nonatomic, copy) NSString* docDescription;
@property (nonatomic) EnumDSSXMLDocSaveAsFlags flag;
@end
