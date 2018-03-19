//
//  SendPerfLog.m
//  MicroStrategyMobile
//
//  Created by Ju Wenting on 9/14/15.
//  Copyright © 2015 MicroStrategy Inc. All rights reserved.
//

#import "SendPerfLog.h"
#import "IPadDataController.h"
#import "MSIAppDelegate.h"
#import "MSIAppDelegateUtils.h"
#import "MSIWidgetViewer.h"
#import "MSTRCustomizations.h"
#import "GraphViewer.h"


#define TEST_URL @"mstr://?url=http%3A%2F%2F10.197.32.131%3A8080%2FMicroStrategyMobile%2Fservlet%2FtaskProc%3FtaskId%3DgetMobileConfiguration%26taskEnv%3Dxml%26taskContentType%3Dxmlanf%26configurationID%3D9baab8fd-d419-44f8-afae-51a9620113f5&authMode=1&dt=2"


#pragma mark - 
#pragma mark UIApplication (SendPerfLog)

@interface UIApplication (SendPerfLog)

@end


@implementation UIApplication (SendPerfLog)

+ (void)load{
    [SendPerfLog getInstance];
}

@end


#pragma mark - 
#pragma mark MSTRCustomizations (SendPerfLog)
@interface  MSTRCustomizations (SendPerfLog)

- (NSString *)hacked_applicationInitializationConfigurationURL;

@end

@implementation MSTRCustomizations (SendPerfLog)

- (NSString *)hacked_applicationInitializationConfigurationURL{
    return TEST_URL;
}

@end


#pragma mark MSIWidgetViewer (SendPerfLog)
@interface MSIWidgetViewer (SendPerfLog)

- (NSString* )hacked_accessibilityLabel;

@end

@implementation MSIWidgetViewer (SendPerfLog)

- (NSString *)hacked_accessibilityLabel{
    if (mAccessibilityLabel == nil) {
        NSString* prefix=@"This is a ";
        NSString* className = NSStringFromClass([self class]);
        prefix = [prefix stringByAppendingFormat:@"%@ Widget.",className];
        
        [self setMAccessibilityLabel: prefix];
    }
    
    return mAccessibilityLabel;
}

@end

#pragma make GraphViwer (SendPerfLog)

@interface GraphViewer (SendPerfLog)

- (NSArray *)hacked_getAccessibilityChildren;

@end

@implementation GraphViewer (SendPerfLog)

- (NSArray *)hacked_getAccessibilityChildren{
    if ( accessibilityChildren != nil  && ([accessibilityChildren count] > 0))
    {
        return accessibilityChildren;
    }
    
    NSString *tittle = @"This is a Graph";
    
    [accessibilityChildren addObject:tittle];
    
    return accessibilityChildren;
}

@end

static SendPerfLog *instance;

@implementation SendPerfLog


+ (SendPerfLog *)getInstance{
    @synchronized([SendPerfLog class]) {
        if (instance == nil) {
            instance = [[SendPerfLog alloc]init];
            [SendPerfLog exchangeMethod];
        }
        return instance;
    }
}

+ (void)exchangeMethod{
    Method orig_method, target_method;
    

    orig_method = class_getInstanceMethod([MSIWidgetViewer class], @selector(accessibilityLabel));
    target_method = class_getInstanceMethod([MSIWidgetViewer class], @selector(hacked_accessibilityLabel));
    method_exchangeImplementations(orig_method, target_method);
    
    orig_method = class_getInstanceMethod([MSTRCustomizations class], @selector(applicationInitializationConfigurationURL));
    target_method = class_getInstanceMethod([MSTRCustomizations class], @selector(hacked_applicationInitializationConfigurationURL));
    method_exchangeImplementations(orig_method, target_method);
    
    orig_method = class_getInstanceMethod([GraphViewer class], @selector(getAccessibilityChildren));
    target_method = class_getInstanceMethod([GraphViewer class], @selector(hacked_getAccessibilityChildren));
    method_exchangeImplementations(orig_method, target_method);
    
    
    
}

@end
