/*
 *  Service.h
 *  MSTR Foundation
 *
 *  Created by Amirali Charania on 2/20/09.
 *  Copyright 2009 MicroStrategy. All rights reserved.
 *
 */

@protocol Service;
@class MSTRError;

@protocol ServiceHandler <NSObject>

-(void)onSuccess: (id)data service:(id<Service>)service;
-(void)onFailure: (MSTRError*)error service:(id<Service>)service;

@optional
-(void)onProgress:(long long)loadedSize Total:(long long)totalSize service:(id<Service>)service;
- (void) onCancel:(id<Service>)service;


@end

@protocol Service <NSObject>

-(void)start;
-(void)cancel;
-(NSInteger)state;
-(void)setHandler:(id<ServiceHandler>)_handler;
-(void)interrupt;
@end
