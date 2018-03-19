//
//  MSIPreferencesStore.h
//  MSTR Foundation
//
//  Created by Amirali Charania on 2/20/09.
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSIDevicePreferences.h"

#define PreferencesStoreDidUpdateNotification @"PreferencesStoreDidUpdateNotification"


/**
 * MSIPreferencesStore manages the device preferences. It is a singleton class.
 * It is used to access the device preferences, create a device preferences copy for modifying, and save the updated device preference to a file.
 */
@interface MSIPreferencesStore : NSObject {
	MSIDevicePreferences * masterPreferences;
}

/**
 * The device preferences in the application. This object is readonly.
 * If you need to modify device preferences, use preferencesCopy first.
 
 @see preferencesCopy
 */
@property (nonatomic, readonly) MSIDevicePreferences * masterPreferences;

/**
 * Singleton method to get the MSIPreferencesStore.
 * @return a singleton instance of the MSIPreferencesStore
 */
+(MSIPreferencesStore *)preferencesStore;

/**
 * Copy the device preferences
 * @return a new copy the application's device preferences. This device preferences can be updated and save back to file.
 */
-(MSIDevicePreferences *)preferencesCopy;

/**
 * Apply the Configuration XML which comes from Mobile Server
 * This API would configure the app and connection with an XML
 * @param configXML the config xml data
 */
-(BOOL)applyConfigXML:(NSData *)configXML;

/**
 * Stores the given preferences to disk. Does nothing if the given prefs are nil or marked read-only.
 * This method will trigger PreferencesStoreDidUpdateNotification
 * @param copyPreferences the device preferences copy to be saved
 * @throws MSTRException (PREFERENCES_WRITE_FAIL) if the write has an error.
 */
- (void) storePreferences: (MSIDevicePreferences *) copyPreferences;

/**
 * Reset the preferences to an default preference. 
 * This method will trigger PreferencesStoreDidUpdateNotification
 * @throws MSTRException (PREFERENCES_WRITE_FAIL) if the write has an error.
 */
- (void) resetPreferences;

/**
 * This method is deprecated. Please use resetPreferences or storePreferences instead.
 * Update the application's device preferences to another copy of device preferences.
 * It will release the original device preferences and reference to the new device preferences.
 * @param copyPreferences the new device preferences
 */
-(void)updatePreferences:(MSIDevicePreferences *)copyPreferences __attribute__((deprecated));

+ (void)clearPreferencesStore;

+ (void)stopPreferencesStoreOperations;

+ (void)startPreferencesStoreOperations;

+ (void)deletePreferences;

+ (MSIDevicePreferences *) newDevicePreferencesForConfiguration:(NSData *) xmlData;

+ (BOOL)exists;

@end
