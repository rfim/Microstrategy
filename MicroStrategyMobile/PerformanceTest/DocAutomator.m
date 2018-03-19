//
//  DocAutomator.m
//  MicroStrategyMobile
//
//  Created by Wenting Ju on 11/7/14.
//  Copyright (c) 2014 MicroStrategy Inc. All rights reserved.
//
//  This class is used by EA Team to run stability test.
//  It can run all of the documents in your mobile configuration one by one. Please run build with debug mode on simulator.
//
//
//  How To Use
//  1. Add DocAutomator.h and DocAutomator.m to target: MicroStrategyMobileSDK
//  2. Change some parameters according to your test environment
//     EXEC_WAIT_TIME
//     SAVE_PATH (Location on Mac. Document screen shot will be saved in this location.)
//     startLevel
//  3. Run on simulator (Please use debug mode)
//  4. Tap on the folder which you want to start with.
//  Test will help execute all of the documents under this configuration one by one.
//  If you find some problems, feel free to contact with Wenting Ju.


#import "DocAutomator.h"
#import <objc/runtime.h>
#import "SAXSupport.h"
#import "MIPromptCellFactory.h"
#import "MIDynamicTableView.h"
#import "MIPromptCellFactoryUIHelper.h"
#import "MediaExternalOpenController.h"
#import "IPadDataControllerImpl.h"
#import "MSTRFileManager.h"
#import "FileUtils.h"


//Use EXEC_WAIT_TIME to define the pause time when executing a document.
#define EXEC_WAIT_TIME 20

//Use DISABLE_ALERT_VIEW to disable Alert View. If you want APP to pop up Alert, please undocument it.
#define DISABLE_ALERT_VIEW

//Document screen shot saved location.
#define SAVE_PATH @"/Users/Automation/Automation/AutomationResult/"
#define DEVICETEST YES

//the following array specifies where to start
static NSInteger startLevel[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
static bool startLevelCheck[20] = {NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO};
static DocAutomator* instance = nil;
static NSInteger pathLevel[20];
static int currentLevel;



#pragma mark -
#pragma mark UIApplication (DocAutomator)

@interface UIApplication (DocAutomator)
@end

@implementation UIApplication (DocAutomator)

+ (void)load
{
    [DocAutomator getInstance];
}

@end


#pragma mark -
#pragma mark NSObject (DocAutomator)

@interface NSObject (DocAutomator)
- (void) hacked_initializeView;
@property(nonatomic,retain) UIView *view;

@end


@implementation NSObject (DocAutomator)

- (void) hacked_initializeView{
    UIView* v = [[UIView alloc] initWithFrame:CGRectZero];
    self.view = v;
    self.view.clipsToBounds = YES;
    self.view.backgroundColor = [UIColor blackColor];
    self.view.autoresizingMask = (UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight);
    [[DocAutomator getInstance] setIPadDataControllerImpl:self];
    NSLog(@"lalalala");
}


@end


#pragma mark -
#pragma mark UIAlertView (DocAutomator)

@interface UIAlertView (DocAutomator)

- (void)hacked_show;

@end

@implementation UIAlertView (DocAutomator)

- (void)hacked_show{
    NSLog(@"Warning! An alert view pops up! The error message is: %@", self.message);
}

@end

#pragma mark -
#pragma mark MIPromptSummaryScreen (DocAutomator)

@interface MIPromptSummaryScreen (DocAutomator)

- (id) initForAuto: (Prompts *)prompts helper: (PromptHelper* )helper;

@end

@implementation MIPromptSummaryScreen (DocAutomator)

- (id) initForAuto:(Prompts *)prompts helper:(PromptHelper *)helper{
    self = [super init];
    if(self) {
        mScrollview.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
        promptList = prompts;
        promptHelper = helper;
        MIPromptCellFactory* _cellfactory = [[MIPromptCellFactory alloc] init];
        MIPromptCellFactoryUIHelper *_uihelper = [[MIPromptCellFactoryUIHelper alloc]initWithThemeType:helper.themetype];
        _cellfactory.uiHelper = _uihelper;
        self.cellFactory = _cellfactory;
        [(MIPromptCellFactory*)self.cellFactory setPromptHelper:helper];
        self.dataIterator = prompts;
        
        saveOnExit = NO;
        mHighlighted_num = -1; ///  -1 means no highlighted
        themetype = helper.themetype;
        seperatorLine = [[UIImageView alloc]init];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector (keyboardDidHide:) name: UIKeyboardDidHideNotification object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector (keyboardDidShow:) name: UIKeyboardDidShowNotification object:nil];
        didRotate = NO;
        enableButtons = YES;
        [[DocAutomator getInstance]setPromptSummaryScreen:self];
    }
    
    return self;

}

@end

#pragma mark -
#pragma mark PDFDocumentController (DocAutomator)

@interface PDFDocumentController (DocAutomator)

- (void)hacked_viewWillAppear:(BOOL)animated;

@end

@implementation PDFDocumentController (DocAutomator)


- (void)hacked_viewWillAppear:(BOOL)animated{
    
    [[DocAutomator getInstance] setPDFDocumentController:self];
    [self hacked_viewWillAppear:animated];

}

@end

#pragma mark -
#pragma mark MediaWebController (DocAutomator)



@interface MediaWebController (DocAutomator)

- (void)hacked_viewDidAppear:(BOOL)animated;

@end

@implementation MediaWebController (DocAutomator)


- (void)hacked_viewDidAppear:(BOOL)animated{
    NSLog(@"hacked view did appear");
    [self hacked_viewDidAppear:animated];
    [[DocAutomator getInstance] setMediaWebController:self];
}

@end

#pragma mark -
#pragma mark MediaExternalOpenController (DocAutomator)

@interface MediaExternalOpenController (DocAutomator)

- (id)initWithURLForAuto:(NSString *)url withThemeHelper:(DocumentThemeHelper*) thmHelper;

@end

@implementation MediaExternalOpenController (DocAutomator)

- (id)initWithURLForAuto:(NSString *)url withThemeHelper:(DocumentThemeHelper *)thmHelper{
    if (self = [super initWithUrl:url]) {
        baseGradientView = nil;
        docController = nil;
        themeHelper = thmHelper;
    }
    [self setNavigationBarWithThemeHelper:thmHelper];
    [[DocAutomator getInstance]setMediaWebController:self];
    return self;
}

@end


#pragma mark -
#pragma mark FolderItemsListView (DocAutomator)

@interface FolderItemsListView (DocAutomator)

- (void) hacked_tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath;
- (void) tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath;
- (void) hacked_reloadData;

@property (nonatomic, retain)UITableView* tableView;
@end

@implementation FolderItemsListView (DocAutomator)

- (void) hacked_tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath{
    
    NSLog(@"Hacked_Select Folder Item");
    if (!startLevelCheck[currentLevel]) {
        pathLevel[currentLevel] = startLevel[currentLevel];
        startLevelCheck[currentLevel] = YES;
    }
    
    FolderItem* folderItem = [self.dataSource folderItemAtIndexPath:indexPath];
    [DocAutomator getInstance].docName = [folderItem getName];
    
    while ([folderItem isShortcut] || [folderItem isSecured]){
        int row = indexPath.row + 1;
        pathLevel[currentLevel]++;
        indexPath = [[DocAutomator getInstance]generateValidIndexPath:pathLevel[currentLevel]];
        NSLog(@"%i, %i", indexPath.section, indexPath.row);
        
        if (!indexPath) {
            break;
        }else
            folderItem = [self.dataSource folderItemAtIndexPath:indexPath];
    }

    if (!indexPath) {
        [[DocAutomator getInstance]folderBackToUpperLevel];
    }else{
        NSLog(@"%i, %i", indexPath.section, indexPath.row);

        folderItem = [self.dataSource folderItemAtIndexPath:indexPath];

        
        [DocAutomator getInstance].path = indexPath;
        [DocAutomator getInstance].tableView = tableView;
        [DocAutomator getInstance].folderItemsListView = self;
        
        if ([folderItem hasChildItems]) {

            [DocAutomator getInstance].entering = YES;
            
            if (tableView.isEditing) {
                [self.delegate folderView:self selectFolderItemForEditingAtIndexPath:indexPath];
            } else {
                [self.delegate folderView:self selectFolderItemAtIndexPath:indexPath];
            }
            
        }else
            [[DocAutomator getInstance]executeDocument:indexPath];
    }
    
}

- (void)hacked_reloadData{
    
    NSLog(@"reloadData");
    
    [self hacked_reloadData];

    if ([DocAutomator getInstance].entering) {
        NSLog(@"Go To Folder");
        [DocAutomator getInstance].entering = NO;
        [[DocAutomator getInstance] didEnteredFolderItem:[DocAutomator getInstance].path withFolderItemsListView: [DocAutomator getInstance].folderItemsListView tableView:[DocAutomator getInstance].tableView];
    }else if ([DocAutomator getInstance].closing){
        [DocAutomator getInstance].closing = NO;
        [[DocAutomator getInstance] closeButtonActionDone];
    }else if ([DocAutomator getInstance].backing){
        NSLog(@"Back from folder");
        [DocAutomator getInstance].backing = NO;
        [[DocAutomator getInstance] backFolderDone];
    }

}
@end


typedef enum { CloseButtonRWDButton = 0, ClosePDFButton, CloseMediaButton, UnsupportButton} CloseButtonType;

@interface DocAutomator(){
    

    IPadDataControllerImpl* privateIPadDataController;
    PDFDocumentController* privatePDFDocumentController;
    MediaWebController* privateMediaWebController;
    CloseButtonType closeButtonType;
    MIPromptSummaryScreen *privatePromptSummaryScreen;
    BOOL promptEnable;
    
    dispatch_queue_t launchQueue;
    dispatch_queue_t launchQueue2;
}

@end



@implementation DocAutomator

+ (DocAutomator* )getInstance{
    @synchronized ([DocAutomator class]){
        if (instance == nil) {
            instance = [[DocAutomator alloc]init];
            [DocAutomator exchangeMethods];
        }
        return instance;
    }

}

- (id) init{
    @synchronized(self)
    {
        self = [super init];
        _working = false;
        launchQueue = dispatch_queue_create("launch queue", NULL);
        launchQueue2 = dispatch_queue_create("launch queue2", NULL);
        _entering = NO;
        _closing = NO;
        _backing = NO;
        return self;
    }
}


- (void) start
{
    _working = true;
    currentLevel = 0;
    pathLevel[0] = 0;
}

+ (void)exchangeMethods{
 
    Method orig_method, target_method;
    
    orig_method = class_getInstanceMethod(NSClassFromString(@"IPadDataControllerImpl"), @selector(initializeView));
    target_method = class_getInstanceMethod(NSClassFromString(@"NSObject"), @selector(hacked_initializeView));
    method_exchangeImplementations(orig_method, target_method);

    orig_method = class_getInstanceMethod([FolderItemsListView class], @selector(tableView: didSelectRowAtIndexPath:));
    target_method = class_getInstanceMethod([FolderItemsListView class], @selector(hacked_tableView:didSelectRowAtIndexPath:));
    method_exchangeImplementations(orig_method, target_method);
    
    orig_method = class_getInstanceMethod([FolderItemsListView class], @selector(reloadData));
    target_method = class_getInstanceMethod([FolderItemsListView class], @selector(hacked_reloadData));
    method_exchangeImplementations(orig_method, target_method);
    
    orig_method = class_getInstanceMethod([PDFDocumentController class], @selector(viewWillAppear:));
    target_method = class_getInstanceMethod([PDFDocumentController class], @selector(hacked_viewWillAppear:));
    method_exchangeImplementations(orig_method, target_method);
    
    orig_method = class_getInstanceMethod([MediaWebController class], @selector(viewDidAppear:));
    target_method = class_getInstanceMethod([MediaWebController class], @selector(hacked_viewDidAppear:));
    method_exchangeImplementations(orig_method, target_method);
    
    orig_method = class_getInstanceMethod([MIPromptSummaryScreen class], @selector(init:helper:));
    target_method = class_getInstanceMethod([MIPromptSummaryScreen class], @selector(initForAuto:helper:));
    method_exchangeImplementations(orig_method, target_method);
    
    orig_method = class_getInstanceMethod([MediaExternalOpenController class], @selector(initWithUrl:withThemeHelper:));
    target_method = class_getInstanceMethod([MediaExternalOpenController class], @selector(initWithURLForAuto:withThemeHelper:));
    method_exchangeImplementations(orig_method, target_method);

    
#ifdef DISABLE_ALERT_VIEW
    orig_method = class_getInstanceMethod([UIAlertView class], @selector(show));
    target_method = class_getInstanceMethod([UIAlertView class], @selector(hacked_show));
    method_exchangeImplementations(orig_method, target_method);
#endif
    
}

- (void)setIPadDataControllerImpl:(IPadDataControllerImpl *)controller{
    NSLog(@"set IPad Data Controller Impl");
    privateIPadDataController = controller;
    closeButtonType = CloseButtonRWDButton;
}

- (void)setPDFDocumentController: (PDFDocumentController *)controller{
    NSLog(@"set PDF Document Controller");
    privatePDFDocumentController = controller;
    closeButtonType = ClosePDFButton;
}

- (void)setMediaWebController:(MediaWebController *)controller{
    NSLog(@"set MediaWebController");
    privateMediaWebController = controller;
    closeButtonType = CloseMediaButton;
}

- (void)setPromptSummaryScreen: (MIPromptSummaryScreen *)promptSummaryScreen{
    NSLog(@"Set Prompt Summary Screen");
    privatePromptSummaryScreen = promptSummaryScreen;
    promptEnable = YES;
}


- (void) didEnteredFolderItem:(NSIndexPath *)indexPath withFolderItemsListView:(FolderItemsListView *)folderView tableView:(UITableView *)tableView
{
    if (indexPath) {
        [NSThread sleepForTimeInterval:1];
        pathLevel[currentLevel] = indexPath.row;
        
        currentLevel++;
        
        
        if (!startLevelCheck[currentLevel])
        {
            startLevelCheck[currentLevel] = YES;
            pathLevel[currentLevel] = startLevel[currentLevel];
        }
        else
        {
            pathLevel[currentLevel] = 0;
        }
        NSLog(@"enter next, %d", currentLevel);
        
        NSLog(@"%d,%d,%d,%d,%d,%d",pathLevel[0], pathLevel[1], pathLevel[2], pathLevel[3], pathLevel[4], pathLevel[5]);
        NSIndexPath *nextIndexPath = [self generateValidIndexPath:pathLevel[currentLevel]];
        
        if (nextIndexPath) {
            [folderView tableView:tableView didSelectRowAtIndexPath:nextIndexPath];
        }else{
            dispatch_async(dispatch_get_main_queue(), ^{
                [[DocAutomator getInstance] folderBackToUpperLevel];
            });
        }

    }
    
}

- (void)executeDocument:(NSIndexPath *)indexPath{
    
    NSLog(@"Execute Document!");
    
    dispatch_async(launchQueue, ^{
        
        [NSThread sleepForTimeInterval:1];
        dispatch_async(dispatch_get_main_queue(), ^{
            if (_tableView.isEditing) {
                [_folderItemsListView.delegate folderView:_folderItemsListView selectFolderItemForEditingAtIndexPath:indexPath];
            } else {
                [_folderItemsListView.delegate folderView:_folderItemsListView selectFolderItemAtIndexPath:indexPath];
            }
        });
        
        [NSThread sleepForTimeInterval:EXEC_WAIT_TIME];
        
        NSLog(@"Prompt Enable is %hhd", promptEnable);
        if (promptEnable) {
            dispatch_async(dispatch_get_main_queue(), ^{
                UIBarButtonItem *buttonItem = privatePromptSummaryScreen.navigationItem.rightBarButtonItem;
                NSLog(@"Apply Button: %@", buttonItem);
                [privatePromptSummaryScreen answerPromptsFromSender:buttonItem];
            });

            [NSThread sleepForTimeInterval:EXEC_WAIT_TIME];
        }
        promptEnable = NO;
        
        if (DEVICETEST) {
            NSString *fileName = @"AutoDetails.txt";
            NSString *tempPath = [NSTemporaryDirectory() stringByAppendingPathComponent:fileName];
            
            if ([FileUtils fileExistsAtPath:tempPath]) {
                
                [FileUtils appendData:[self docAddressData] atPath:tempPath error:nil];
              //  [FileUtils writeData:[self docAddressData] atPath:tempPath error:nil];
                
            }else{
                [FileUtils createFileAtPath:tempPath contents:[self docAddressData] attributes:nil];
            }
            
            
        }else{

            UIImage *image = [DocAutomator imageWithView:[UIApplication sharedApplication].keyWindow];
            [DocAutomator saveImage:image toFilePath:[self getFileName]];
        }
        dispatch_async(dispatch_get_main_queue(), ^{
            _closing = YES;
            NSLog(@"Will to close view");
            if (closeButtonType == CloseButtonRWDButton) {
                [privateIPadDataController performSelectorOnMainThread:@selector(handleCloseBarButton) withObject:nil waitUntilDone:YES];
                closeButtonType = UnsupportButton;
            }else if (closeButtonType == ClosePDFButton){
                [privatePDFDocumentController performSelectorOnMainThread:@selector(returnToParent) withObject:nil waitUntilDone:YES];
                closeButtonType = UnsupportButton;
            }else if (closeButtonType == CloseMediaButton){
                NSLog(@"Close Web view");
                
               // [privateMediaWebController.navigationController performSelectorOnMainThread:@selector(popViewControllerAnimated:) withObject:YES waitUntilDone:YES];
                [privateMediaWebController.navigationController popViewControllerAnimated:YES];
                closeButtonType = UnsupportButton;
            }else{
                [[DocAutomator getInstance]closeButtonActionDone];
            }

        });
    });
    
}

- (void)closeButtonActionDone{
    
    dispatch_async(launchQueue2, ^{
        [NSThread sleepForTimeInterval:1];
        pathLevel[currentLevel]++;
        NSLog(@"pos: %d, %ld", currentLevel, (long)pathLevel[currentLevel]);
        
        NSIndexPath *nextIndexPath = [self generateValidIndexPath:pathLevel[currentLevel]];
        
        if (nextIndexPath) {
            dispatch_async(dispatch_get_main_queue(), ^{
                //[_folderItemsListView hacked_tableView:_tableView didSelectRowAtIndexPath:nextIndexPath];
                [_folderItemsListView tableView:_tableView didSelectRowAtIndexPath:nextIndexPath];
            });
        }else{
            NSLog(@"not valid");
            dispatch_async(dispatch_get_main_queue(), ^{
                [self folderBackToUpperLevel];
            });
        }
    });
}

- (void)folderBackToUpperLevel{
    
    _backing = YES;
    int upperLevel = currentLevel - 1;
    NSLog(@"The Upper Level is %i", upperLevel);
    dispatch_async(dispatch_get_main_queue(), ^{
        [NSThread sleepForTimeInterval:1];
        [_folderItemsListView.delegate folderView:_folderItemsListView selectFolderItemOnFolderPathAtIndex:upperLevel];
    });

}

- (void)backFolderDone{
    pathLevel[currentLevel] = 0;
    currentLevel--;
    if (currentLevel == -1)
        currentLevel = 0;
    NSLog(@"back folder, %d", currentLevel);
    pathLevel[currentLevel]++;
    NSLog(@"pos: %d, %d", currentLevel, pathLevel[currentLevel]);
    
    NSIndexPath *nextIndexPath = [self generateValidIndexPath:pathLevel[currentLevel]];
    
    NSLog(@"input %d,%d", nextIndexPath.section, nextIndexPath.row);
    
    if (nextIndexPath) {
        [_folderItemsListView tableView:_tableView didSelectRowAtIndexPath:nextIndexPath];
    }else{
        NSLog(@"not valid");
        dispatch_async(dispatch_get_main_queue(), ^{
            [self folderBackToUpperLevel];
        });
    }
}

- (NSIndexPath *)generateValidIndexPath: (int)row{
    
    NSMutableArray *tableCellArray = [[NSMutableArray alloc]init];

    
    int sectionNumberOfCurrentTable = _tableView.numberOfSections;
    NSInteger rowCount[5];
   // NSLog(@"Section Count is %i", sectionNumberOfCurrentTable);
    
    for (int i = 0; i < sectionNumberOfCurrentTable; i++) {
        rowCount[i] = [_tableView numberOfRowsInSection:i];
     //   NSLog(@"The row number of section %i is %i", i, rowCount[i]);
        
        for (int j = 0; j < rowCount[i]; j++) {
            NSIndexPath *path = [NSIndexPath indexPathForRow:j inSection:i];
            [tableCellArray addObject:path];
        }
    }
    
  //  NSLog(@"Row count of current table is %i", [tableCellArray count]);
    
    if (row < [tableCellArray count]) {
        return [tableCellArray objectAtIndex:row];
    }else if (currentLevel == 0){
        NSIndexPath *path = [NSIndexPath indexPathForRow:0 inSection:0];
        return path;
    }else
        return nil;

}

+ (UIImage *)imageWithView:(UIView *)view {
    [view setNeedsDisplay];
    UIGraphicsBeginImageContextWithOptions(view.frame.size, NO, [[UIScreen mainScreen] scale]);
    CALayer *layer = view.layer;
    CGContextRef context = UIGraphicsGetCurrentContext();
    [layer renderInContext:context];
    UIImage *viewImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    return viewImage;
}

- (NSString *) getFileName{
    NSString *s = [NSString stringWithFormat:@"%@", SAVE_PATH];
    if (currentLevel ==0)
    {
        return [s stringByAppendingString: [NSString stringWithFormat:@"%ld", (long)pathLevel[0]]];
    }
    
    for (int i=0; i< currentLevel; i++)
    {
        s =  [s stringByAppendingString:[NSString stringWithFormat:@"%ld_", (long)pathLevel[i]]];
    }
    
    s = [s stringByAppendingString:[NSString stringWithFormat:@"%ld.png", (long)pathLevel[currentLevel]]];
    
    
    return  s;
}

- (NSData *)docAddressData{
    
    NSString *s = [NSString stringWithFormat:@"%d_", pathLevel[0]];
    
    if (currentLevel > 0) {
        for (int i = 1; i<= currentLevel; i++)
        {
            s =  [s stringByAppendingString:[NSString stringWithFormat:@"%ld_", (long)pathLevel[i]]];
        }
    }
    
    s = [s stringByAppendingString:[NSString stringWithFormat:@"%@ \n", _docName]];
    
    NSData *data = [s dataUsingEncoding:NSUTF8StringEncoding];
    return data;
    
    
}


+ (void)saveImage:(UIImage*)image toFilePath:(NSString*)filePath{
    BOOL saved = [UIImagePNGRepresentation(image) writeToFile:filePath atomically:YES];
    if (!saved) NSLog(@"Unable to save image to %@", filePath);
}



@end
