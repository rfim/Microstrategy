//
//  ImageCacheCallback.h
//  MSTRMobile
//
//  Created by Firat Alpergin on 2/23/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

/** 	
 *	Protocol renamed from GridWidgetImageCacheCallback to ImageCacheCallback
 */

@protocol ImageCacheCallback

-(void)imageReceived:(UIImage *)image forURL:(NSString*)url withLocalURL:(NSString*)localUrl isHitCache:(BOOL)hitCache;
-(NSString *)getSubscriptionId;

@end
