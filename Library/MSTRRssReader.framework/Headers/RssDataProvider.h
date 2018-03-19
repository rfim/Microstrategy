//
//  RssDataProvider.h
//  RssFeedLib
//
//  Created by Nikolay Sazonov on 6/7/10.
//  Copyright 2010 EastBanc Technologies. All rights reserved.
//

#import <CoreData/CoreData.h>


@interface RssDataProvider : NSObject {
	
	NSPersistentStoreCoordinator * _coordinator;
	NSManagedObjectContext * managedObjectContext;

}

@property (nonatomic, assign) NSPersistentStoreCoordinator * persistentCoordinator;
@property (nonatomic, retain, readonly) NSManagedObjectContext *managedObjectContext;

- (id) initWithCoordinator:(NSPersistentStoreCoordinator *) coordinator;

- (void) save;

- (void) deleteManagedObject: (NSManagedObject *) object;

@end
