//
//  MISQLWritebackService.h
//  MicroStrategyMobile
//
//  Created by hli on 3/9/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "MSIServiceImpl.h"

@class Prompts;
@class MSIReportDisplayInfo;

typedef enum {
    EnumPreSQLWriteback,
    EnumFFSQLWriteback
} WritebackTaskType;

@interface MISQLWritebackService : MSIServiceImpl <ServiceHandler> {
@private
    NSString *styleName;
    NSArray *sqlParameterArray;
    WritebackTaskType writebackTaskType;
    MSIReportDisplayInfo *report;
@protected    
    Prompts *promptsCache;
    BOOL acceptEmptyResult;
}

@property (nonatomic, strong) MSIReportDisplayInfo *report;
@property (nonatomic, copy) NSString *styleName;
@property (nonatomic, strong) NSArray *sqlParameterArray;
@property (nonatomic) WritebackTaskType writebackTaskType;
@property (nonatomic) BOOL acceptEmptyResult;

- (NSString *)populatePromptsAnswer;
- (void)runFFSQLWritebackService;

@end
