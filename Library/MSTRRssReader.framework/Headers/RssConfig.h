//
//  RssConfig.h
//  MSTRRssReader
//
//  Created by Alexandr Gordienko on 02.09.10.
//  Copyright 2010 EastBanc Technologies. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIColor.h>


@interface MSMRssConfig : NSObject {
}
/*
 note default thread attribute is automatic,
 I am not sure if RSS really need it.modify it in
 future for performance issue
 */
@property (strong) UIColor * backgroundColorTitle;
@property (strong) UIColor * backgroundColorArticle;
@property (strong) UIColor * fontColorArticle;
@property (strong) UIColor* backgroundColorRollover;
@property (strong) UIColor* fontColorRollover;
@property (strong) UIColor* rssReaderTitleColor;
@property (strong) UIColor* generalBackgroundColor;
@property (strong) UIColor* fontColorTitle;
@property (strong) UIColor* readTitleColor;

/*
 since NSString may actually a NSMutalbleString 
 use copy 
 */
@property (copy) NSMutableArray *feedsConfigs;
@property (copy) NSString * reportID;
@property (copy) NSString* bgColorArticle;
@property (copy) NSString* fColorArticle;

@property (assign) CGRect rect;
@property (assign) NSInteger reportType;

-( id )init;
-(void)restoreToDefaultVisualSettings;

@end
