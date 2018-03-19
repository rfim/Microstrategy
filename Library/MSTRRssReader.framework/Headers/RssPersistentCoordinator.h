//
//  RssPersistentCoordinator.h
//  RssFeedLib
//
//  Created by Nikolay Sazonov on 6/7/10.
//  Copyright 2010 EastBanc Technologies. All rights reserved.
//

#import <CoreData/CoreData.h>

@interface RssPersistentCoordinator : NSObject {
	
@private
	NSManagedObjectModel *managedObjectModel;
	NSPersistentStoreCoordinator *persistentStoreCoordinator;

}

@property (nonatomic, retain, readonly) NSPersistentStoreCoordinator *persistentStoreCoordinator;
@property (nonatomic, retain, readonly) NSManagedObjectModel *managedObjectModel;

+ (RssPersistentCoordinator *)sharedInstance;
+ (NSString *)applicationDocumentsDirectory;
@end
