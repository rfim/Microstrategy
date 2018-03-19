//
//  FileManager.h
//  MicroStrategyMobile
//
//  Created by Amirali Charania on 10/31/11.
//  Copyright 2011 MicroStrategy Inc. All rights reserved.
//

#import <UIKit/UIKit.h>

@protocol FileManager

/**
 * Return a file manager singleton instance, create one if it does not exist.
 * @return a reference to the file manager singleton
 */
+(id<FileManager>)fileManager;

/**
 * Create a directory at specific path with attributes.
 * @param path the path of the directory.
 * @param createIntermediates whether to create the parent directories to the destination directory.
 * @param attributes attriutes of the directory.
 * @param error the error occurred while creating directory.
 */
-(BOOL)createDirectoryAtPath:(NSString*)path withIntermediateDirectories:(BOOL)createIntermediates
                  attributes:(NSDictionary*)attributes error:(NSError**)error;

/**
 * Returns the contents of a directory at path.
 * @param path the path of the directory.
 * @param error the error occurred while creating directory.
 * @return an array of the contents of the directory.
 */
-(NSArray*)contentsOfDirectoryAtPath:(NSString*)path error:(NSError**)error;

/**
 * Returns the sub paths of a directory.
 * @param path the path of the directory.
 * @return the sub paths of the directory.
 */
-(NSArray*)subpathsAtPath:(NSString *)path;

/**
 * APIs to tell whether a file/directory exists in the file system.
 * @param path the path of file to check.
 * @param isDirectory out-parameter to tell whether the file is a directory.
 * @return YES if the file exist; No if the file does not exist.
 */
-(BOOL)fileExistsAtPath:(NSString*)path;
-(BOOL)fileExistsAtPath:(NSString*)path isDirectory:(BOOL*)isDirectory;

/**
 * Set the attributes for a specific file.
 * @param attributes the attributes to get/set.
 * @param path the path of the file.
 * @param error out-parameter to tell whether there is any error occurred.
 * @return YES if set attributes successfully; NO otherwise.
 */
-(BOOL)setAttributes:(NSDictionary *)attributes ofItemAtPath:(NSString*)path error:(NSError **)error;

/**
 * Get the attributes for a specific file.
 * @param path the path of the file.
 * @param error out-parameter to tell whether there is any error occurred.
 * @return attributes of the file.
 */
-(NSDictionary *)attributesOfItemAtPath:(NSString *)path error:(NSError **)error;

/**
 * Read data from a specific file in an encrypted format.
 * @param path the path of the file.
 * @param error out-parameter to tell whether there is any error occurred.
 * @return data object that contains the content of file.
 **/
-(NSData*)readDataAtPath:(NSString*)path error:(NSError**)error;

/**
 * Read data from a specific file into a data buffer.
 * @param dataBuf the buffer to which the data is read.
 * @param path the path of the file.
 * @param offset the offset of the start point of reading.
 * @param length length of data to read.
 * @param error out-parameter to tell whether there is any error occurred.
 * @return the length of data returned.
 */
-(size_t)readData:(void*)dataBuf atPath:(NSString*)path offset:(long)offset
		   length:(size_t)length error:(NSError**)error;
-(size_t)readDataInBuffer:(void**)dataBuf atPath:(NSString*)path offset:(long)offset
                   length:(size_t)length error:(NSError**)error;

/**
 * Write/Append data to a file at path.
 * @param data the data to write.
 * @param path the path of the destination file.
 * @param error out-parameter to tell whether there is any error occurred.
 * @return 1 if data or path is invalid; 0 otherwize. Use error to tell whether the writing is successful.
 */
-(NSInteger)writeData:(NSData*)data atPath:(NSString*)path error:(NSError**)error;
-(NSInteger)appendData:(NSData*)data atPath:(NSString*)path error:(NSError**)error;

/**
 * Create file with provided content at path.
 * @param path the path of the destination file.
 * @param contents the data to be written into the file.
 * @param attributes the attributes of the file.
 * @return YES if the writing operation succeeds; NO otherwise.
 */
-(BOOL)createFileAtPath:(NSString*)path contents:(NSData*)contents attributes:(NSDictionary*)attributes;

/**
 * Write a dictionary into a file.
 * @param dictionary the dictionary to write.
 * @param filePath the path of the destination file.
 */
-(void)writeDictionary:(NSDictionary*)dictionary toFileAtPath:(NSString*)filePath;

/**
 * Read the content of file into a dictionary. It has to work with writeDictionary:toFileAtPath
 * @param filePath the path of the file to be read from.
 * @return a dictionary that contains the content of file.
 */
-(NSDictionary*)dictionaryAtFilePath:(NSString*)filePath;


//APIs to read/write/append data from a specific file without protection. These APIs are used to read/write/append data when device is locked. At this point, the data is not encrypted, data encryption could be done in these method if necessary.
/**
 * @param path the path of the file.
 * @param error out-parameter to indicate whether there is error occurred.
 * @param encrypt whether we need to encrypt data.
 * @return the data read from the file.
 */
-(NSData*)readDataWithNoProtectionAtPath:(NSString*)path error:(NSError**)error encrypt:(bool)encrypt;

/**
 * @param the data to write/append to the file.
 * @param path the path of the file.
 * @param error out-parameter to indicate whether there is error occurred.
 * @param encrypt whether we need to encrypt data.
 */
-(NSInteger)writeDataWithNoProtection:(NSData*)data atPath:(NSString*)path error:(NSError**)error encrypt:(bool)encrypt;
-(NSInteger)appendDataWithNoProtection:(NSData*)data atPath:(NSString*)path error:(NSError**)error encrypt:(bool)encrypt;

/**
 * Create and open an output stream for file at path.
 * @param filePath the path of the file.
 * @param append whether to append data or not.
 * @return output stream of the file.
 */
-(NSOutputStream*)newOutputStreamToFileAtPath:(NSString*)filePath append:(BOOL)append;

/**
 * Create and open an input stream for file at path.
 * @param filePath the path of the file.
 * @return input stream of the file.
 */
-(NSInputStream*)newInputStreamWithFileAtPath:(NSString*)filePath;

/**
 * APIs to copy/move a file to another destination.
 * @param path file path of the source file.
 * @param toPath file path of the destination file.
 * @param error out-parameter to tell whether there is error occurred.
 * @return status of the operation. Do not use the return value to determine whether the operation is successful, use the error.
 */
-(NSInteger)moveItemAtPath:(NSString*)path toPath:(NSString*)toPath error:(NSError**)error;
-(NSInteger)copyItemAtPath:(NSString*)path toPath:(NSString*)toPath error:(NSError**)error;

/**
 * APIs to remove file at a specific path.
 * @param path the path of the file to delete.
 * @param error out-parameter to tell whether there is error occurred.
 * @return status of the operation. Do not use the return value to determine whether the operation is successful, use the error.
 */
-(NSInteger)deleteDataAtPath:(NSString*)path error:(NSError**)error;
-(BOOL)removeItemAtPath:(NSString*)path error:(NSError**)error;

/**
 * Clear the Tmp directory.
 */
-(void)clearTmpDirectory;

/**
 * Utility APIs to get regular directories in file system sandbox.
 */
-(NSString*)documentDirectory;
-(NSString*)cachesDirectory;
-(NSString*)libraryDirectory;
-(NSString*)tempDirectory;

/**
 * Block the file operation while the device is locked.
 */
-(void)waitUntilUnlockOperationIsFinished;

/**
 * The API to tell AE whether the mapped file is supported.
 */
-(BOOL)isMappedFileSupported;

/**
 * Decrypt the content of file at path and write to the same path.
 * The file reading/writing process is done without any protection.
 * @param the path of the file.
 * @return the length of the decrypted data.
 */
-(long long)decryptDataAtPath:(NSString*)path;

/**
 * Decrypt the data downloaded from Newsstand and write to the file system. This method is only called by Newsstand feature.
 * The file writing process is done without any protection.
 *
 * @param the path of the file.
 * @param error out-parameter to tell whether there is error occurred.
 * @return the size of the decrypted data.
 */
-(size_t)incrementalDecryptSoftwareEncryptedDataAtPath:(NSString *)path error:(NSError **)error;

/**
 * Decrypt the file that will be opened in 3rd party app and save it to temperary location if necessary. 
 * The file has to be decrypted because 3rd party app could not decrypt the file encrypted by security framework.
 *
 * @param the path of the file to decrypt.
 * @return the location of the temperary file.
 */
- (NSString *) decryptedExternalFileForFileAtPath:(NSString *)encryptedFilePath;

/**
 * Remove the temperary files generated for 3rd party app.
 *
 * @param the path for the encrypted file.
 *
 */
- (void) removeDecryptedFileForFile:(NSString *)file;
@end

