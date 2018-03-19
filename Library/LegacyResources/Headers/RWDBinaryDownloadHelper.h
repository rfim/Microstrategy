//
//  RWDBinaryDownloadHelper.h
//  MicroStrategyMobile
//
//  Created by Ji Jin on 9/10/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Enums.h"
#import "DSSIPhoneEnums.h"

#define CACHEINFO_ID_CLIENT @"00000000000000000000000000000000"

@class MSIReportDisplayInfo;
@class RWDataServiceResult;
@class MSIServiceImpl;
@class IServerCacheInfo;
@class NSError;
@class MSIProjectInfo;

@interface RWDBinaryDownloadHelper : NSObject

/**
 * Check if the service is doing on demand loading from result flags
 * @param iResultFlags Result flag used
 * @param iDetailFlags Detail flag used
 * @retrun If we are using on demand loading
 */
+(BOOL) isOnDemandLoading:(NSInteger)iResultFlags detailFlags:(NSInteger)iDetailFlags;

/**
 * Check if the service is doing definition and data separate loading from result flags
 * @param iResultFlags Result flag used
 * @param iDetailFlags Detail flag used 
 * @retrun If we are using definition and data separate loading
 */
+(BOOL) isSeparatedBinaryDownloading:(NSInteger)iResultFlags detailFlags:(NSInteger)iDetailFlags;

//+(BOOL)isSeparatedImageBlock:(int)iResultFlags detailFlags:(int)iDetailFlags;
/**
 * Is cacheID the first slice binary
 * @param iCacheID cache ID
 * @retrun If the cache ID is of first slice binary
 */
+(BOOL) isCacheIDFirstSlice:(NSString*)iCacheID;

/**
 * Return the main cache ID of the document
 * @param iDocument Document or report information
 * @retrun Main cache ID
 */
+(NSString*) getMainCacheID:(MSIReportDisplayInfo*)iDocument;

/**
 * Return cache ID based on document information
 * @param iDocument Document or report information
 * @param iResultFlags Result flag used
 * @param iDetailFlags Detail flag used
 * @param iNodeKeys Node keys of the binary request
 * @param iOrientation Device orientation
 * @retrun Cache ID for the current document
 */
+(NSString*) getCacheID:(MSIReportDisplayInfo*)iDocument resultFlags:(NSInteger)iResultFlags detailFlags:(NSInteger)iDetailFlags nodeKeys:(NSString*)iNodeKeys orientation:(int)iOrientation;

/**
 * Save server cache info to the report store
 * @param iDocument Document or report information
 * @param iBinDataPath Binary data file path
 * @param iCacheID Cache ID to be saved
 * @param iOrientation Device orientation
 */
//+(void) saveCacheInfoOfBinary:(NSString*)iBinDataPath forDocument:(MSIReportDisplayInfo*)iDocument withCacheID:(NSString*)iCacheID orientation:(int)iOrientation;

/**
 * Return server cache info from the binary
 * @param iDocument Document or report information
 * @param iBinDataPath Binary data file path
 * @param iCacheID Cache ID to be saved
 * @param iOrientation Device orientation
 * @return Server cache info
 */
+(IServerCacheInfo*) getCacheInfoOfBinary:(NSString*)iBinDataPath forDocument:(MSIReportDisplayInfo*)iDocument withCacheID:(NSString*)iCacheID orientation:(int)iOrientation;
/**
 * Return true if auto refresh is enabled for saving binary cache
 * @param iDocument Document or report information
 * @param iBinDataPath Binary data file path
 * @param iCacheID Cache ID to be saved
 * @param iOrientation Device orientation
 */
+(bool) isAutoRefreshEnabledForBinary:(NSString*)iBinDataPath forDocument:(MSIReportDisplayInfo*)iDocument withCacheID:(NSString*)iCacheID orientation:(int)iOrientation;

/**
 * Return a RWDataServiceResult structure based on service information
 * @param iDocument Document or report information
 * @param iResultFlags Result flag used
 * @param iDetailFlags Detail flag used
 * @param iNodeKeys Node keys of the binary request
 * @param iOrientation Device orientation
 * @param iRequestDownloadMode Request download mode
 * @param iBinDataPath Binary data file path
 * @param iCacheID Cache ID to be saved
 * @param iOrientation Device orientation
 */
+(RWDataServiceResult*)handleBinaryData:(MSIReportDisplayInfo*)iDocument
                            resultFlags:(NSInteger)iResultFlags
                            detailFlags:(NSInteger)iDetailFlags
                               nodeKeys:(NSString*)iNodeKeys
                            orientation:(int)iOrientation
                    requestDownloadMode:(EnumBinaryDownloadMode)iDownloadMode
                                service:(MSIServiceImpl *)iService
                           isBackground:(BOOL)iBackground
                                  error:(NSError**)iError;

/**
 * Return result flag that will be used when retrieving binary
 * @param iDownloadMode Download mode
 * @param iLayoutFlag The detailflag that indicate layout
 * @param iProject The project info
 * @retrun ResultFlags to be used
 */
+(NSUInteger) getResultFlagsForDownloadMode:(EnumBinaryDownloadMode)iDownloadMode ofLayout:(EnumBinaryContextDetailFlags)iLayoutFlag inProject:(MSIProjectInfo*)iProject;

/**
 * Check if there is enough device cache for rendering the document
 *
 * @param iDocument Document/Report info
 * @param iOrientation Current device orientation
 * @return If enough cache is available
 */
+(BOOL) canDocumentBeRendered:(MSIReportDisplayInfo*)iDocument inOrientation:(UIInterfaceOrientation)iOrientation;

/**
 * Check if the request is for the all CSI binary
 *
 * @param iResultFlags Result flag used
 * @param iDetailFlags Detail flag used
 * @return the request is for the all CSI binary
 */
+(BOOL)isAllCSIBinaryRequest:(NSInteger)iResultFlags detailFlags:(NSInteger)iDetailFlags;

/**
 * Check if the request is for the all dataset binary
 *
 * @param iResultFlags Result flag used
 * @param iDetailFlags Detail flag used
 * @return the request is for the all dataset binary
 */
+(BOOL)isAllDatasetsBinaryRequest:(NSInteger)iResultFlags detailFlags:(NSInteger)iDetailFlags;

+(BOOL)isAllBinaryPartsBinaryRequest:(NSInteger)iResultFlags detailFlags:(NSInteger)iDetailFlags;

/**
 * Check if the request is for initial layout binary
 *
 * @param iResultFlags Result flag used
 * @param iDetailFlags Detail flag used
 * @return the request is for initial layout binary
 */
+(BOOL)isInitialLayoutBinaryRequest:(NSInteger)iResultFlags detailFlags:(NSInteger)iDetailFlags;

/**
 * Check if the request is for a specific layout binary
 *
 * @param iResultFlags Result flag used
 * @param iDetailFlags Detail flag used
 * @return the request is for a specific layout binary
 */
+(BOOL)isSpecificLayoutBinaryRequest:(NSInteger)iResultFlags detailFlags:(NSInteger)iDetailFlags;

+(EnumSeparateBinaryChunkType)getBinaryTypeWithResultFlags:(int)iResultFlags detailFlags:(int)iDetailFlags;
+(NSArray*)loadDatasetIDsFromDefinition:(NSString *)path isSmart:(BOOL*)isSmart datasetPromptAnswers:(NSMutableDictionary*)promptAnswers;
+(NSArray*)loadLayoutKeysFromDefinition:(NSString *)path initLayoutKey:(NSString**)initLayoutKey;
+(NSString*)loadDatasetPromptAnswerFromDefinition:(NSString*)datasetID onDocument:(MSIReportDisplayInfo*) document;

/**
 * Check if all binary caches for separable binary format is valid
 *
 * @param iDocument Document/Report info
 * @param iValidateServerCache If we need to validate server cache info is valid.
 * @param iValidateDeviceFile If we need to validate file exists. If set to NO, we will only check for binary path.
 * @return If enough cache is available
 */
+(BOOL) checkSeparableBinaryCache:(MSIReportDisplayInfo*)iDocument validateServerCache:(BOOL)iValidateServerCache validateDeviceFile:(BOOL)iValidateDeviceFile;

/**
 * Check if binary cache for full binary format is valid
 *
 * @param iDocument Document/Report info
 * @param iValidateServerCache If we need to validate server cache info is valid.
 * @param iValidateDeviceFile If we need to validate file exists. If set to NO, we will only check for binary path.
 * @return If enough cache is available
 */
+(BOOL) checkFullBinaryCache:(MSIReportDisplayInfo*)iDocument validateServerCache:(BOOL)iValidateServerCache validateDeviceFile:(BOOL)iValidateDeviceFile;

/**
 * Check if the request is for first slice (definition or first slice data) in separable format
 * @param iResultFlags Result flag used
 * @param iDetailFlags Detail flag used
 * @retrun If we are request first slice binary
 */
+(BOOL) isSeparableBinaryFirstSlice:(NSInteger)iResultFlags detailFlags:(NSInteger)iDetailFlags;

/**
 * Check if the request is for first data slice
 * @param iResultFlags Result flag used
 * @param iDetailFlags Detail flag used
 * @retrun If we are request first slice binary
 */
+(BOOL) isFirstSliceData:(NSInteger)iResultFlags detailFlags:(NSInteger)iDetailFlags;
@end
