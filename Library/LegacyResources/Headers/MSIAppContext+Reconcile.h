//
//  MSIAppContext+Reconcile.h
//  MicroStrategyMobile
//
//  Created by Liu, Shu on 8/30/13.
//  Copyright (c) 2013 MicroStrategy Inc. All rights reserved.
//

#import "MSIAppContext.h"

@class MSTRMultipleError;
@class MSIReportDisplayInfo;

@protocol ReconcileListener

- (void)handleReconcileProjectSuccess:(MSIProjectInfo*)project;
- (void)handleReconcileProjectFailure:(MSTRMultipleError*)error;
- (void)handleReconcileSuccess:(id)data;
- (void)handleReconcileFailure:(MSTRError*)error;
- (BOOL)handleAuthenticationFailure:(MSIProjectInfo*)project;
- (BOOL)handleCertificateFailure;
- (void)handleReconcileStart;

@end

@interface MSIAppContext (Reconcile) <UIAccelerometerDelegate>

/**
 * Default suppress network error alerts
 **/

// reconcile
- (void)reconcile;
- (void)reconcileWithDoesSuppressNetworkErrorAlert:(BOOL)suppressNetworkErrorAlerts;
- (void)reconcile:(NSInteger)scope;
- (void)reconcile:(NSInteger)scope suppressNetworkErrorAlerts:(BOOL)suppressNetworkErrorAlerts;

// reconcile project
- (void)reconcileProject:(MSIProjectConfiguration*)project;
- (void)reconcileProject:(MSIProjectConfiguration*)project suppressNetworkErrorAlerts:(BOOL)suppressNetworkErrorAlerts;
- (void)reconcileProject:(MSIProjectConfiguration*)project scope:(NSInteger)scope;
- (void)reconcileProject:(MSIProjectConfiguration*)project scope:(NSInteger)scope suppressNetworkErrorAlerts:(BOOL)suppressNetworkErrorAlerts;

// reconcile report
- (void)reconcileReport:(MSIProjectConfiguration*)project report:(MSIReportDisplayInfo*)report;
- (void)reconcileReport:(MSIProjectConfiguration*)project report:(MSIReportDisplayInfo*)report suppressNetworkErrorAlerts:(BOOL)suppressNetworkErrorAlerts;

// reconcile notificate subscription
- (void)reconcileNotificationSubscription:(NSString*)subscriptionID;
- (void)reconcileNotificationSubscription:(NSString*)subscriptionID suppressNetworkErrorAlerts:(BOOL)suppressNetworkErrorAlerts;

// reconcile listener
- (id<ReconcileListener>)getReconcileListener;
- (void)setReconcileListener:(id<ReconcileListener>)_reconcileListener withShakeEnabled:(BOOL)shakeEnabled;
- (void)removeReconcileListener;

// setup periodic reconcile
- (void)setupPeriodicReconcile;

// cancel reconcile
- (void)cancelReconcile;

@end
