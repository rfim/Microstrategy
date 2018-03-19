//
//  StringUtils.h
//  MSTRFoundation
//
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>

/**
 * StringUtils provides utility functions to manipulate strings.
 */
@interface MSIStringUtils : NSObject {

}

/**
 * Encode special characters in xml string.
 * @param inputString original string
 * @return encoded string
 */
+(NSString*)xmlEncode:(NSString*)inputString;

/**
 * Get string representation of a date for display purpose
 * @param inputDate the date in milliseconds
 * @return display string with abbreviate text
 */
+(NSString*)getDateForDisplay:(NSTimeInterval)inputDate;

/**
 * Get string representation of a date for display purpose with short time string
 * @param inputDate the date in milliseconds
 * @return display string with abbreviate text
 */
+(NSString*)getDateForDisplayShortTime:(NSTimeInterval)inputDate;

/**
 * Check whether two strings contain same characters. Both strings can be nil. Case sensitive.
 * @param str1 the first string to be compared
 * @param str2 the second string to be compared
 * @return YES if two strings are equal or both are empty, otherwise NO.
 */
+(BOOL)string:(NSString*)str1 isEqualTo:(NSString*)str2;

/**
 * Check whether two strings contain different characters. Both strings can be nil. Case sensitive.
 * @param str1 the first string to be compared
 * @param str2 the second string to be compared
 * @return YES if two strings are not equal or one is empty, otherwise NO.
 */
+(BOOL)string:(NSString*)str1 isNotEqualTo:(NSString*)str2;

/**
 * Decode xml string
 * @param inputString encoded string
 * @return decoded string
 */
+(NSString*)xmlDecode:(NSString*)inputString;

/**
 * Generate a globally unique ID. The GUID has the format of xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
 * @return GUID string
 */
+(NSString*)newGUID;

/**
 * Get the parameter pairs in URL. The URL has the format of baseUrl?param1=value1&param2=value2&...
 * @param url the url to be processed
 * @return a dictionary contains the key value pair of parameters
 */
+ (NSDictionary*) getURLParameters:(NSString*)url;

/**
 * Get the parameter pairs in the URL query string. The URL query string has the format of param1=value1&param2=value2&...
 * @param query the URL query string to be process
 * @return a dictionary contains the key value pair of query parameters
 */
+ (NSDictionary*) getQueryStringParameters:(NSString*)query;

/**
 * Construct a query string by key value pair of query parameters
 * @param queryParameters key value pair of query parameters
 * @return URL query string with the format of ?param1=value1&param2=value2&...
 */
+ (NSString*) getQueryString:(NSDictionary*)queryParameters;

/**
 * Check whether a input string is empty, the string can either be nil or contain no characters
 * @param string to be checked
 * @return YES if the input string is nil or empty string, otherwise NO
 */
+ (BOOL) isEmpty:(NSString*)input;

/**
 * Check whether a string is not empty.
 * @return YES if the string is not empty, otherwise NO.
 */
+ (BOOL) isNotEmpty:(NSString*)input;


/**
 * Creates a url string from the base URL and the (name, value) pairs as parameters.
 * @param prefix base URL with the form "http://www.mstr.com"
 * @param params key value pairs of paramters dictionary
 * @return a URL string with format of prefix?param1=value1&param2=value2&...
 */
+ (NSString*) urlWithPrefix:(NSString*)prefix paramDictionary:(NSDictionary*)params;

/**
 * Converts the data to the string using UTF8Encoding. The caller is responsible for releasinge the string.
 * @param data NSData
 * @return NSString with UTF8 encoding
 */
+ (NSString*) newUTF8StringFromNSData:(NSData*) data;

/**
 * Replaces the HTML codes like &#13, &#32 and returns a new string 
 * E.g,: &#13 : newline &#32 : space
 * @param input string contains HTML codes
 * @return string without the HTML codes
 */
+ (NSString*) replaceHTMLCodes:(NSString*)input;

/**
 * Decode an encoded url string.
 * @param urlString encoded URL string
 * @param decodeSpaceChar whether to decode the '+' to ' '(space) character
 * @return decoded string
 */
+ (NSString*) decodedUrlString:(NSString*)urlString decodeSpace:(BOOL)decodeSpaceChar;

/**
 * Compares two MicroStrategy versions. Empty version always loses.
 * Version structure assumed to be major.minor.release1.release2.  Precedence is major>minor>release2>release1.
 * 	For a.b.c.d, we check a then b then d then c

 * @param fromVersion The first version
 * @param toVersion The second version
 * @return compare result, > 0 if fromVersion > toVersion, -1 if fromVersion < toVersion, 0 if equal.
 */

+ (NSInteger) compareServerVersion:(NSString*) fromVersion toVersion:(NSString*)toVersion;

/**
 * Compares two MicroStrategy iserver versions. Empty version always loses.
 * Version structure assumed to be major.minor.release1.release2.  Precedence is major>minor>release1>release2.
 * 	For a.b.c.d, we check a then b then c then d
 * @param fromVersion The first version
 * @param toVersion The second version
 * @return compare result, < 0 if fromVersion > toVersion, -1 if fromVersion < toVersion, 0 if equal.
 */
+ (NSInteger) compareIServerVersion:(NSString*) fromVersion toVersion:(NSString*)toVersion;

/**
 Compare two url strings to see if they are connecting to the same server
 @param urlString1 The first url string
 @param urlString2 The second url string
 @return YES if the two urls are connecting to the same server
 NO if not
 **/
+ (BOOL) isURLStringsConnectingToSameServer:(NSString *)urlString1 andURLString:(NSString *)urlString2;

/**
 * Replace special encoded characters in link drill URL, and return the new URL.
 * "&amp;#38;" to @"&amp;"
 * "&amp;#35;" to @"#"
 * "&amp;#59;" to @";"
 * "&amp;#62;" to @"&gt;"
 * "&amp;#60;" to @"&lt;"
 * "&amp;#34;" to @"&quot;"
 * "&amp;#32;" to @" "
 * @param inputString the source string
 * @return string after replacement
 */
+ (NSString*) replaceLinkDrillEncodedCharacters:(NSString*) inputString;

/**
 *	Kun: add inApp parameter for url string
 **/
+ (NSString*) addInAppParameterForURL:(NSString *)urlString;

@end
