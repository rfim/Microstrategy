//
//  FileUtils.h
//  MSTR Foundation
//
//  Created by Amirali Charania on 2/24/09.
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>

#define SOFTWARE_ENCRYPTION_PREFIX @"MSTR_SE_"

// if you don't want use encryption for reading/writing file, just comment out the following define "ENCRYPTION_ENABLED".
#define ENCRYPTION_LOGGING_ENABLED NO

@interface FileUtils : NSObject {
	
}

+(NSString*)documentDirectory;
+(NSString*)cachesDirectory;
+(NSString*)libraryDirectory;
+(NSString*)tempDirectory;
+(NSData*)readDataAtPath:(NSString*)path error:(NSError**)error;
+(size_t)readData:(void*)dataBuf atPath:(NSString*)path offset:(long)offset 
		   length:(size_t)length error:(NSError**)error;
+(size_t)readDataInBuffer:(void**)dataBuf atPath:(NSString*)path offset:(long)offset
                   length:(size_t)length error:(NSError**)error;
+(NSInteger)writeData:(NSData*)data atPath:(NSString*)path error:(NSError**)error;
+(NSInteger)deleteDataAtPath:(NSString*)path error:(NSError**)error;
+(NSInteger)appendData:(NSData*)data atPath:(NSString*)path error:(NSError**)error;
+(void)clearTmpDirectory;
+(BOOL)setAttributes:(NSDictionary *)attributes ofItemAtPath:(NSString*)path error:(NSError **)error;
+(NSDictionary *)attributesOfItemAtPath:(NSString *)path error:(NSError **)error;
+(BOOL)fileExistsAtPath:(NSString*)path;
+(BOOL)fileExistsAtPath:(NSString*)path isDirectory:(BOOL*)isDirectory;
+(NSArray*)contentsOfDirectoryAtPath:(NSString*)path error:(NSError**)error;
+(BOOL)removeItemAtPath:(NSString*)path error:(NSError**)error;
+(BOOL)createDirectoryAtPath:(NSString*)path withIntermediateDirectories:(BOOL)createIntermediates 
				  attributes:(NSDictionary*)attributes error:(NSError**)error;
+(BOOL)createFileAtPath:(NSString*)path contents:(NSData*)contents attributes:(NSDictionary*)attributes;
+(NSInteger)moveItemAtPath:(NSString*)path toPath:(NSString*)toPath error:(NSError**)error;
+(NSInteger)copyItemAtPath:(NSString*)path toPath:(NSString*)toPath error:(NSError**)error;
+(void)waitUntilUnlockOperationIsFinished;
+(NSOutputStream*)newOutputStreamToFileAtPath:(NSString*)filePath append:(BOOL)append;
+(NSInputStream*)newInputStreamWithFileAtPath:(NSString*)filePath;
+(void)writeDictionary:(NSDictionary*)dictionary toFileAtPath:(NSString*)filePath;
+(NSDictionary*)dictionaryAtFilePath:(NSString*)filePath;
+(NSString *)mimeTypeForFileAtPath:(NSString *)path;
+(NSArray*)subpathsAtPath:(NSString *)path;
+(BOOL)isMappedFileSupported;
+(NSData*)readDataWithNoProtectionAtPath:(NSString*)path error:(NSError**)error encrypt:(bool)encrypt;
+(NSInteger)writeDataWithNoProtection:(NSData*)data atPath:(NSString*)path error:(NSError**)error encrypt:(bool)encrypt;
+(NSInteger)appendDataWithNoProtection:(NSData*)data atPath:(NSString*)path error:(NSError**)error encrypt:(bool)encrypt;
+(long long)decryptDataAtPath:(NSString*)path;
+(void)stopAllFileOperations;
+(void)startAllFileOperations;
+(size_t)incrementalDecryptSoftwareEncryptedDataAtPath:(NSString*)path;
+(NSString *) decryptedExternalFileForFileAtPath:(NSString *)encryptedFilePath;
+(void) removeDecryptedFileForFile:(NSString *)file;
@end



