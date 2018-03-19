//
//  DSSImageCacheManager.mm
//  ReportServiceCore
//
//  Created by Mingjun Zhu on 5/27/10.
//  Copyright 2010 MSTR. All rights reserved.
//

#import "DSSDocumentInstance.h"
#import "Utils/Common/FileUtils.h"
#import "DSSImageCacheManager.h"
#import "DSSImageCacheManagerUtils.h"
#import "Utils/Common/MSIGeneric.h"

@implementation DSSImageCacheManager

static NSDate* gsPlaceHolderImageTime = nil; // this one should be refreshed everytime app is restarted.
// static NSInteger gs10yearsInSeconds = 3600 * 24 * 366 * 10; // 10 years in seconds
static NSInteger gPlaceHolderImageYear = 2036;

static NSString* gCachePath = nil;
static NSString* gLocalImageHomePath = nil;
static NSString* gHttpImagesHomePath = nil;
static NSString* gAbsoluteImagesHomePath = nil;

-(void) dealloc 
{
    gCachePath = nil;
    gLocalImageHomePath = nil;
    gHttpImagesHomePath = nil;
    gAbsoluteImagesHomePath = nil;
}

+(BOOL) isHTTPImage:(NSString *)ipPath
{
	// if it starts with "http://", then it's http image
	if ([ipPath length] <= 0)
		return false;
	/*
	 NSString* lpPathPrefix = [ipPath substringToIndex:7];
	 NSComparisonResult lCompareResult = [lpPathPrefix caseInsensitiveCompare:@"http://"];
	 if (lCompareResult == NSOrderedSame)
	 {
	 return true;
	 }
	 */
	
	NSRange lRange = [ipPath rangeOfString:@"http://" options:NSCaseInsensitiveSearch];
	if (lRange.location != NSNotFound)
	{
		return true;
	}

	lRange = [ipPath rangeOfString:@"https://" options:NSCaseInsensitiveSearch];
	if (lRange.location != NSNotFound)
	{
		return true;
	}

	lRange = [ipPath rangeOfString:@"ftp://" options:NSCaseInsensitiveSearch];
	if (lRange.location != NSNotFound)
	{
		return true;
	}
	return false;
}

+(BOOL) isAbsolutePath:(NSString *)ipPath
{
	if ([ipPath length] <= 0)
		return FALSE;
	
	NSInteger pathLength = [ipPath length];
	// mzhu: 2010-06-25: "\\" is absolute path
	unichar lFirstChar = [ipPath characterAtIndex:0];
	if (lFirstChar == '/')
	{
		// "/" is absolute path
		return TRUE;
	}
	
	if (pathLength > 1)
	{
		unichar lSecondChar = [ipPath characterAtIndex:1];
		if (lFirstChar == '\\' && lSecondChar == '\\')
		{
			// "\\" is absolute path
			return TRUE;
		}
		
		if (pathLength > 2)
		{
			unichar lThirdChar = [ipPath characterAtIndex:2];
			if (lSecondChar == ':')
			{
				if (lThirdChar == '\\' || lThirdChar == '/')
				{
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

+ (unichar) getValidFileNameChar:(unichar) ipChar
{
	// mzhu: file name character reference is taken from:
	// http://en.wikipedia.org/wiki/Filename
	switch (ipChar)
	{
		case '*':
		case '\\':
		case '/':
		case ':':
		case '?':
		case '\"':
		case '<':
		case '>':
		case '|':
		case '[':
		case ']':
			return '_';
		default:
			return ipChar;
	}
	
	return ipChar;
}

//kai,6/20/2014 use regular expression
+ (NSString*) convertUrlToFileName:(NSString*) ipUrlPath
{
    //    NSInteger urlPathLength = [ipUrlPath length];
    //	NSMutableString* newFileName = [[NSMutableString alloc] initWithCapacity:urlPathLength];
    //
    //	for (NSInteger index = 0; index < urlPathLength; ++index)
    //	{
    //		[newFileName appendFormat:@"%c", [DSSImageCacheManager getValidFileNameChar:[ipUrlPath characterAtIndex:index]]];
    //	}
    //	return [newFileName autorelease];
//    ipUrlPath = @"*A\SDA\\FFAS/FDSFDS:?ADSFAF<\">|[]_abcasdede\\u8bf7请";
    NSError* error = nil;
    NSString* pattern = @"\\*|\\\\|\\/|\\:|\\?|\\\"|\\<|\\>|\\||\\[|\\]";
    NSRegularExpression* regularExp = [NSRegularExpression regularExpressionWithPattern:pattern options:0 error:&error];
    NSString* modifiedString = [regularExp stringByReplacingMatchesInString:ipUrlPath options:0
                                                                      range:NSMakeRange(0, [ipUrlPath length]) withTemplate:@"\\_"];
    if(error) NSLog(@"imageCache convertUrl errorCode: %ld", (long)[error code]);
    return modifiedString;
}

+ (NSString*) convertWindowsMacFileName:(NSString*) ipFileName
{
	NSString* newFileName = @"";
    newFileName = [ipFileName stringByReplacingOccurrencesOfString:@"\\" withString:@"/"];
	
    /*
     NSInteger fileLength = [ipFileName length];
     
     for (NSInteger index = 0; index < fileLength; ++index)
     {
     unichar lChar = [ipFileName characterAtIndex:index];
     unichar lReplaceChar = lChar;
     if (lChar == '\\')
     {
     lReplaceChar = '/';
     }
     newFileName = [newFileName stringByAppendingFormat:@"%c", lReplaceChar];
     }
     */
	return newFileName;
}

//static NSString* gHttpImagesHomePath = nil;
//static NSString* gAbsoluteImagesHomePath = nil;


+(NSString*) getCacheFolderPath{
    
    if(!gCachePath)
    {
        gCachePath = [[NSString alloc] initWithString:[FileUtils cachesDirectory]];
    }
    
    return gCachePath;
}

+(NSString*)getLocalImagesHomePath {
    
    if(!gLocalImageHomePath)
    {
        NSString* imagesPath = [FileUtils cachesDirectory];

        NSString* imagesDir = [imagesPath stringByAppendingPathComponent:@"ImageCacheStore"];
	
        //gLocalImageHomePath = [NSString stringWithString:imagesDir];
        gLocalImageHomePath = [[NSString alloc] initWithString:imagesDir];
    }
    
    BOOL exist = [FileUtils fileExistsAtPath:gLocalImageHomePath];
	if(!exist){
		[FileUtils createDirectoryAtPath:gLocalImageHomePath withIntermediateDirectories:YES attributes:nil error:nil];
	}
	return gLocalImageHomePath;	
}

+(NSString*)getHttpImagesHomePath {
    
    if(!gHttpImagesHomePath)
    {
        NSString* imagesPath = [FileUtils cachesDirectory];
        
        NSString* imagesDir = [imagesPath stringByAppendingPathComponent:@"ImageCacheStore/httpImages"];
        
        gHttpImagesHomePath = [[NSString alloc] initWithString:imagesDir];
	}
    BOOL exist = [FileUtils fileExistsAtPath:gHttpImagesHomePath];
    if(!exist){
        [FileUtils createDirectoryAtPath:gHttpImagesHomePath withIntermediateDirectories:YES attributes:nil error:nil];
    }
	return gHttpImagesHomePath;
}

+(NSString*)getAbsoluteImagesHomePath {
    if(!gAbsoluteImagesHomePath)
    {
        NSString* imagesPath = [FileUtils cachesDirectory];

        NSString* imagesDir = [imagesPath stringByAppendingPathComponent:@"ImageCacheStore/absoluteImages"];
	
        gAbsoluteImagesHomePath = [[NSString alloc] initWithString:imagesDir];
	}
    bool exist = [FileUtils fileExistsAtPath:gAbsoluteImagesHomePath];
	if(!exist){
		[FileUtils createDirectoryAtPath:gAbsoluteImagesHomePath withIntermediateDirectories:YES attributes:nil error:nil];
	}
	return gAbsoluteImagesHomePath;	
}

+(NSString*) getLocalImagePath:(NSString*) ipPath
{
	NSString* lImageHomePath;
	NSString *lReplacedPath;
	if ([DSSImageCacheManager isAbsolutePath:ipPath])
	{
		lImageHomePath = [DSSImageCacheManager getAbsoluteImagesHomePath];
		lReplacedPath = [DSSImageCacheManager convertUrlToFileName:ipPath];
	}
	else
	{
		lImageHomePath = [DSSImageCacheManager getLocalImagesHomePath];
        NSString* lReplacedPathInter = [DSSImageCacheManager convertWindowsMacFileName:ipPath];
        lReplacedPath = [DSSImageCacheManager simplifyPath:lReplacedPathInter];
    }
	
	NSString* lFinalImagePath = [lImageHomePath stringByAppendingPathComponent:lReplacedPath];
	return lFinalImagePath;
}

//+ (void) test
//{
//    NSString* b0 = @"./..";
//    NSString* b01 = @"";
//    NSString* b02 = @".";
//    NSString* b03 = @"./";
//    NSString* b04 = @"../../../../../../";
//    
//    NSString* b1 = [NSString stringWithFormat:@"%@%c%@",@"hello" ,'\0', @"//x/./../../../"];
//    
//    NSString* b2 = @"./a/x/./../";
//    
//    NSString* b3 = @".../../../../x/.//./";
//    
//    NSString* b4 = @".//..//../../X/.//";
//    
//    NSString* b5 = @"..d/.1/2./a..//x//../..请客..a..a../.../.a.a.a.a.a";
//
//    NSArray* arrary = [NSArray arrayWithObjects:b0,b01,b02,b03,b04,b1,b2,b3,b4,b5, nil];
//    for(int i=0; i<[arrary count]; i++)
//    {
//        NSString* buf = [DSSImageCacheManager getLocalImagePath:[arrary objectAtIndex:i]];
//        NSLog(@"%@ --> %@", [arrary objectAtIndex:i],buf);
//    }
//}

//TQMS 900471, in case we can't delete some cache in imageCache parent folder, simplify path by delete '..', however, if '..' appears at the begining, keep it
//'../../../' -> '/RJBD/RJBD/RJBD/'
//'./XX/../' -> '.'

#define RANDOM_STRING @"/RJBD/"
+ (NSString*) simplifyPath:(NSString*) ipPath
{
    //only simplify when '..' appears
    if([ipPath rangeOfString:@".."].length == 0){
        return ipPath;
    }
    NSUInteger urlLength = [ipPath length];
    unichar* src = new unichar[urlLength + 1];
    unichar* dest = new unichar[urlLength + 2];
    [ipPath getCharacters:src range:NSMakeRange(0, urlLength)];
    src[urlLength] = '\0';
    
    NSString* result = [DSSImageCacheManager simplifyPath:src withDest:dest];
    
    delete[] src;
    delete[] dest;
    
//    if([result length] > 2 && [[result substringWithRange:NSMakeRange([result length ]-3, 3)] isEqualToString:@"/.."])
//    {
//        result = [result stringByAppendingString:@"/"];
//    }//can't happen because image file ends with its file extension
    NSString* resultTmp = [result stringByReplacingOccurrencesOfString:@"/../" withString:RANDOM_STRING];
    return [resultTmp stringByReplacingOccurrencesOfString:@"/../" withString:RANDOM_STRING];
}

+ (NSString*) simplifyPath:(unichar*) src withDest:(unichar*) dest
{
    int		toklen;
	unichar		c, pre1;
	unichar     *start, *slash, *walk, *out;
	int	    pre;
    
    //determine length of short
    int bit = sizeof(short) * 8;
    
	if (src == NULL || dest == NULL)
		return nil;
    
	walk = src;
	start = dest;
	out = dest;
	slash = dest;
    
	while (*walk == ' ') {
		walk++;
	}
	pre1 = *(walk++);
	c = *(walk++);
	pre = pre1;
	if (pre1 != '/') {
		pre = ('/' << bit) | pre1;
		*(out++) = '/';
	}
	*(out++) = pre1;
	if (pre1 == '\0') {
		return nil;
	}
	while (1) {
		if (c == '/' || c == '\0') {
			toklen = out - slash;
            // "/../"
			if (toklen == 3 && pre == (('.' << bit) | '.')) {
				out = slash;
                //It's first occurence of "/..", which we should keep it as well
                if (out == start){
                    if(*(out) == '/' && *(out+1) == '.' && *(out+2) == '.'){
                        out += 3;
                        if(c == '\0')
                            out--;
                    }
                }
                //before slash is "/..", which we should keep it
                else if(out - start > 2 && *(out-3) == '/' && *(out-2) == '.' && *(out-1) == '.'){
                    //do nothing
                    out += 3;
                } else if (out > start) {
                        out--;
                        while (out > start && *out != '/') {
                            out--;
                        }
                    }
                if (c == '\0')
                    out++;
			}// "//" or "/./"
            else if (toklen == 1 || pre == (('/' << bit) | '.')) {
				out = slash;
                if (c == '\0')
                    out++;
			}
			slash = out;
		}
		if (c == '\0')
			break;
		pre1 = c;
		pre = (pre << bit) | pre1;
		c = *walk;
		*out = pre1;
		out++;
		walk++;
	}
	*out = '\0';
	return [NSString stringWithCharacters:dest length:out-dest];
}


+(NSData*) getLocalUIImageByPath:(NSString*) ipPath
{
	return [DSSImageCacheManager getLocalUIImageByPath:ipPath autoRelease:YES];
}

+(NSData*) getLocalUIImageByPath:(NSString*) ipPath autoRelease:(BOOL) isAutoReleased
{
    NSString *lReplacedPath = [DSSImageCacheManager convertWindowsMacFileName:ipPath];
	NSString* lFinalImagePath = [DSSImageCacheManager getLocalImagePath:lReplacedPath];
	//Change to use FileUtils, TQMS 453446 yzeng
	NSError* error = nil;
	
	// mzhu 2011-03-11: TQMS 455114, check if image file date is in future compare to gsPlaceHolderImageTime
	[DSSImageCacheManager checkPlaceHolderImage:lFinalImagePath];
    
    NSData* data = nil;
    if(isAutoReleased)
        data = [FileUtils readDataAtPath:lFinalImagePath error:&error];
    else
    {
        size_t size = [[[FileUtils attributesOfItemAtPath:lFinalImagePath error:&error] objectForKey:NSFileSize] integerValue];
        void* buffer = malloc(size);
        [FileUtils readDataInBuffer:&buffer atPath:lFinalImagePath offset:0 length:size error:&error];
        
        if(buffer)
        {
            data = [[NSData alloc] initWithBytes:buffer length:(NSInteger)size];
            
            //delete [] static_cast<char*>(buffer);
            //DL;04/04/2013;734029;replace delete with free to fix memory corruption
            free(buffer);
        }
        
        
        
    }
    
	if (error) {
		return nil;
	} else {
		return data;
	}
}

+(UIImage*) getLocalUIImageFromDisk:(NSString*) ipPath
{
	NSString *lReplacedPath = [DSSImageCacheManager convertWindowsMacFileName:ipPath];
	NSString* lFinalImagePath = [DSSImageCacheManager getLocalImagePath:lReplacedPath];
	
	// mzhu 2011-03-11: TQMS 455114, check if image file date is in future compare to gsPlaceHolderImageTime
	[DSSImageCacheManager checkPlaceHolderImage:lFinalImagePath];
    
    return [UIImage imageWithData:[FileUtils readDataAtPath:lFinalImagePath error:nil]];
}

+ (NSString*) getHttpImagePath:(NSString*) ipUrlPath
{
	NSString* newUrlPath = [DSSImageCacheManager convertUrlToFileName:ipUrlPath];
	
	NSString* lImageHomePath = [self getHttpImagesHomePath];
	NSString* lFinalImagePath = [lImageHomePath stringByAppendingPathComponent:newUrlPath];
	
	return lFinalImagePath;
}

+ (NSData*) getHttpUIImageByPath:(NSString*) ipUrlPath
{
	return [DSSImageCacheManager getHttpUIImageByPath:ipUrlPath autoRelease:YES];
}

+ (NSData*) getHttpUIImageByPath:(NSString*) ipUrlPath autoRelease:(BOOL) isAutoReleased
{
    //for dynamic links(supposely with ? in the url, do not use cache for now KNG 10\16\12
    
    //kng, warning to be removed, MSIAppContext.h cannot be included
    
    // Since we store the info after "?" in the saveHttpUIImage:(NSData*) ipImageData forPath:(NSString*) ipUrlPath forTimeStamp:(NSString*)ipTimeStamp, it is reasonable to use the cache even the url contains "?". Comment out this if-judgement to enhance the performance and fix DE22339 by always using the cache. haoli, 12\9\15
//    if([MSIGeneric networkIsReachable] && ([ipUrlPath rangeOfString:@"?"].location != NSNotFound)) {
//        return nil;
//    }
    NSString* lFinalImagePath = [self getHttpImagePath:ipUrlPath];
	//Change to use FileUtils, TQMS 453446 yzeng
	NSError* error = nil;
	
	// mzhu 2011-03-11: TQMS 455114, check if image file date is in future compare to gsPlaceHolderImageTime
	[DSSImageCacheManager checkPlaceHolderImage:lFinalImagePath];
    
	//NSData* data = [FileUtils readDataAtPath:lFinalImagePath error:&error];
    
    NSData* data = nil;
    if(isAutoReleased)
        data = [FileUtils readDataAtPath:lFinalImagePath error:&error];
    else
    {
        size_t size = [[[FileUtils attributesOfItemAtPath:lFinalImagePath error:&error] objectForKey:NSFileSize] integerValue];
        void* buffer = malloc(size);
        [FileUtils readDataInBuffer:&buffer atPath:lFinalImagePath offset:0 length:size error:&error];
        
        if(buffer)
        {
            data = [[NSData alloc] initWithBytes:buffer length:(NSInteger)size];
            
            //delete [] static_cast<char*>(buffer);
            //DL;04/04/2013;734029;replace delete with free to fix memory corruption
            free(buffer);
        }
    }
    
    
	if (error) {
		return nil;
	} else {
		return data;
	}
}

+ (UIImage*) getHttpUIImageFromDisk:(NSString*) ipUrlPath
{
    
// refactor this method since the logic is same with + (NSData*) getHttpUIImageByPath:(NSString*) ipUrlPath. Re-use the code. haoli, 12\9\15
    
    return [UIImage imageWithData:[DSSImageCacheManager getHttpUIImageByPath:ipUrlPath]];
/*
    //for dynamic links(supposely with ? in the url, do not use cache for now KNG 10\16\12
    //TBD kng, warning to be removed, MSIAppContext.h cannot be included
    if([MSIGeneric networkIsReachable] && ([ipUrlPath rangeOfString:@"?"].location != NSNotFound)) {
        return nil;
    }
	NSString* lFinalImagePath = [self getHttpImagePath:ipUrlPath];
	
	// mzhu 2011-03-11: TQMS 455114, check if image file date is in future compare to gsPlaceHolderImageTime
	[DSSImageCacheManager checkPlaceHolderImage:lFinalImagePath];
    
    return [UIImage imageWithData:[FileUtils readDataAtPath:lFinalImagePath error:nil]];
 */
}

+ (void) saveHttpUIImage:(NSData*) ipImageData forPath:(NSString*) ipUrlPath forTimeStamp:(NSString*)ipTimeStamp
{
	NSString* newUrlPath = [DSSImageCacheManager convertUrlToFileName:ipUrlPath];
	NSString* lImageHomePath = [self getHttpImagesHomePath];
	NSString* lFinalImagePath = [lImageHomePath stringByAppendingPathComponent:newUrlPath];
	//Change to use FileUtils, TQMS 453446
	NSError* error = nil;
	[FileUtils writeData:ipImageData atPath:lFinalImagePath error:&error];	
	[self setFileTimeStamp:ipTimeStamp forPath:lFinalImagePath];
}

+ (void) saveLocalUIImage:(NSData*) ipImageData forPath:(NSString*) ipPath forTimeStamp:(NSString*)ipTimeStamp
{
	// mzhu TQMS 491451, input ipPath could be NULL but we should not remove the ImageCacheStore folder
	if ([ipPath length] <= 0)
		return;

	NSString *lReplacedPath = [DSSImageCacheManager convertWindowsMacFileName:ipPath];
	NSString* lFinalImagePath = [DSSImageCacheManager getLocalImagePath:lReplacedPath];
	
	[FileUtils createDirectoryAtPath:lFinalImagePath withIntermediateDirectories:YES attributes:nil error:nil];
	[FileUtils removeItemAtPath:lFinalImagePath error:nil];
	
	//Change to use FileUtils, TQMS 453446
	NSError* error = nil;
	[FileUtils writeData:ipImageData atPath:lFinalImagePath error:&error];		
	[self setFileTimeStamp:ipTimeStamp forPath:lFinalImagePath];
}

// mzhu 2011-03-11: TQMS 455114: add support to save a place holder image.  The requirements are:
// 1) create a place holder (dummy image file) so that the next request to get this image can return the dummy image quick
// 2) when app get restarted, and someone asks for this image again, we should remove this image so it has another chance to download the actual image
//
// The idea is:
// 1) when first time called, we record that time stamp
// 2) create the image file with  time stamp in future
// 3) when someone asks for this image file, if the time stamp is way in the future, and the recorded time stamp in this class is not set, then remove it
//		this is to achieve the purpose of retrieving the actual image again.

+ (void) saveHttpPlaceHolderImage:(NSData*) ipImageData forPath:(NSString*) ipUrlPath
{
	if (gsPlaceHolderImageTime == nil)
	{
		[self initPlaceHolderTime];
	}
	
	NSDateFormatter *lpFormat = [[NSDateFormatter alloc] init];
	[lpFormat setDateFormat:@"yyyy-MM-dd HH:mm:ss.SSS"];	
	NSString* lpTimeStamp = [lpFormat stringFromDate:gsPlaceHolderImageTime];
	
	[ self saveHttpUIImage:ipImageData forPath:(NSString*) ipUrlPath forTimeStamp:lpTimeStamp];
}

+ (void) saveLocalPlaceHolderImage:(NSData*) ipImageData forPath:(NSString*) ipUrlPath
{
	if (gsPlaceHolderImageTime == nil)
	{
		[self initPlaceHolderTime];
	}
	
	NSDateFormatter *lpFormat = [[NSDateFormatter alloc] init];
	[lpFormat setDateFormat:@"yyyy-MM-dd HH:mm:ss.SSS"];	
	NSString* lpTimeStamp = [lpFormat stringFromDate:gsPlaceHolderImageTime];
	
	[ self saveLocalUIImage:ipImageData forPath:(NSString*) ipUrlPath forTimeStamp:lpTimeStamp];
}

+ (void) checkPlaceHolderImage:(NSString*) ipFinalImagePath
{
	// mzhu 2011-03-11: TQMS 455114, check image file date against gsPlaceHolderImageTime
	NSError* error = nil;
	//NSDictionary* lAttributes = [FileUtils getAttributesOfItemAtPath:ipFinalImagePath error:&error];
	
	NSDictionary* lAttributes = [FileUtils attributesOfItemAtPath:ipFinalImagePath error:&error];
	
	if (lAttributes == nil)
	{
		return; // mzhu: this means local file system doesn't have it.
	}
	
	NSDate* lLocalImageDate = [lAttributes objectForKey:NSFileModificationDate];
	if (lLocalImageDate == nil)
	{
		return;
	}
	
	NSTimeInterval lLocalImageDateInterval = [lLocalImageDate timeIntervalSinceReferenceDate];
	
	NSDateFormatter *lpDateFormatter = [[NSDateFormatter alloc] init];
	[lpDateFormatter setDateFormat:@"yyyy"];
	NSInteger lLocalImageYear = [[lpDateFormatter stringFromDate:lLocalImageDate] intValue];
	
	if (lLocalImageYear < gPlaceHolderImageYear)
	{
		// mzhu: Discussed with Yi Du.  Because place holder image time stamp is in year 2036, but
		// the image time is much older than that, so this is NOT a place holder image, we should not delete it
		return;
	}
	
	if (gsPlaceHolderImageTime == nil)
	{
		[self initPlaceHolderTime];
	}
	// mzhu: set future time (in year 2036) to gsPlaceHolderImageTime, if it's different from lLocalImageDate, then it means 
	// app has been restarted and we should remove it
	NSTimeInterval lTimeInterval = [gsPlaceHolderImageTime timeIntervalSinceReferenceDate];
	
	if (lTimeInterval != lLocalImageDateInterval)
	{
		// remove image file place holder
		// mzhu: for place holder images used without App restart, we should not get into this block, this way we achieved the purpose
		// of improving image retrieve performance
		[FileUtils deleteDataAtPath:ipFinalImagePath error:&error];
	}
}

+ (void) initPlaceHolderTime
{
	// mzhu: first time accessed
	
	NSDate *lNow = [NSDate date];
	
	NSCalendar *lpCalendar = [MSIGeneric getCalendar];
	NSDateComponents *lpDayComponents = [lpCalendar components:(NSCalendarUnitSecond | NSCalendarUnitMinute | NSCalendarUnitHour | NSCalendarUnitDay | NSCalendarUnitMonth | NSCalendarUnitYear) fromDate:lNow];
	NSInteger lSecond = [lpDayComponents second];
	NSInteger lMinute = [lpDayComponents minute];
	NSInteger lHour = [lpDayComponents hour];
	NSInteger lDay = [lpDayComponents day];
	NSInteger lMonth = [lpDayComponents month];
	NSInteger lYear = gPlaceHolderImageYear;
	
	// now build a NSDate object for yourDate using these components
	NSDateComponents *lpFutureDateComponents = [[NSDateComponents alloc] init];
	[lpFutureDateComponents setSecond:lSecond];
	[lpFutureDateComponents setMinute:lMinute]; 
	[lpFutureDateComponents setHour:lHour];
	[lpFutureDateComponents setDay:lDay]; 
	[lpFutureDateComponents setMonth:lMonth]; 
	[lpFutureDateComponents setYear:lYear];
	gsPlaceHolderImageTime = [lpCalendar dateFromComponents:lpFutureDateComponents];
}

/*
 + (void) releaseAppStartTime
 {
 [gsPlaceHolderImageTime release];
 gsPlaceHolderImageTime = nil;
 }
 */
+ (void) setFileTimeStamp:(NSString*)ipTimeStamp  forPath:(NSString*) ipPath
{
	if ([ipTimeStamp length] <= 0)
		return;
	NSDateFormatter *df=[[NSDateFormatter alloc] init];
	[df setDateFormat:@"yyyy-MM-dd HH:mm:ss.SSS"];
	//[df setTimeZone:[NSTimeZone timeZoneForSecondsFromGMT:0]];
	NSDate *serverFileTimeStamp = [df dateFromString:ipTimeStamp];
    
	//Change to use FileUtils, TQMS 453446
	NSError *error = nil;
	[FileUtils setAttributes:[NSDictionary dictionaryWithObject:serverFileTimeStamp forKey:NSFileModificationDate]  ofItemAtPath:ipPath error:&error];
}

+(BOOL) isDifferentFromLocalFile:(NSString*) ipImagePath forTimeStamp:(NSDate*) ipImageTimeStamp
{
	BOOL lbIsHttpImage = [self isHTTPImage: ipImagePath];
	NSString* lFinalImagePath = NULL;
	if (lbIsHttpImage)
	{
		NSString* newUrlPath = [DSSImageCacheManager convertUrlToFileName:ipImagePath];
		NSString* lImageHomePath = [self getHttpImagesHomePath];
		lFinalImagePath = [lImageHomePath stringByAppendingPathComponent:newUrlPath];
	}
	else
	{
		NSString* lImageHomePath = [self getLocalImagesHomePath];
		lFinalImagePath = [lImageHomePath stringByAppendingPathComponent:ipImagePath];
	}
	
	NSError *error = nil;
	//Change to use FileUtils, TQMS 453446
	//NSDictionary* lAttributes = [FileUtils getAttributesOfItemAtPath:lFinalImagePath error:&error];
	
	NSDictionary* lAttributes = [FileUtils attributesOfItemAtPath:lFinalImagePath error:&error];
	
	if (lAttributes == nil)
	{
		return true; // mzhu: this means local file system doesn't have it.
	}
	
	NSDate* lLocalImageDate = [lAttributes objectForKey:NSFileModificationDate];
	if (lLocalImageDate == nil)
	{
		return true;
	}
	
	NSComparisonResult compareResult = [ipImageTimeStamp compare:lLocalImageDate];
	if (compareResult != NSOrderedSame)
	{
		return true;
	}
	return false;
}

+(void) getImageNamesFromDocumentData:(NSString*)ipDataPath imageNames:(NSMutableArray*)iopImageNamesArray imageTimeStamps:(NSMutableArray*)iopImageTimeStampArray
{
	DSSDocumentInstance* lpDoc = new DSSDocumentInstance();
	
	//NSUInteger length = [ipData length];
	NSUInteger length;
	if (ipDataPath) {
        NSDictionary *attrs = [FileUtils attributesOfItemAtPath:ipDataPath error: NULL];
		length = [attrs fileSize];
	}
	
	NSData* data = nil;
	unsigned char * lpBuffer = (unsigned char*)[data bytes];
	lpDoc->LoadImageSkipOthersFromBinary(lpBuffer, length, ((std::string)[ipDataPath UTF8String]).c_str());
	
	[DSSImageCacheManagerUtils getImageNamesFromDocumentInstance:lpDoc imageNames:iopImageNamesArray imageTimeStamps:iopImageTimeStampArray];
	
	delete lpDoc;
}

+ (void) compareLocalImages: (NSMutableArray*) ipPathsArray forTimeStamp: (NSMutableArray*) ipTimeStampArray missingImages: (NSMutableArray*)opMissingImagesArray
{
	NSUInteger lCount = [ipPathsArray count];
	if ([ipPathsArray count] != [ipTimeStampArray count] || lCount <= 0)
		return;
	NSString* lImagePath = nil;
	NSDate* lTimeStamp = nil;
	BOOL lbIsDifferentFromLocalFile = false;
	
	for (int index = 0; index < lCount; ++index)
	{
		lImagePath = [ipPathsArray objectAtIndex:index];
		lTimeStamp = [ipTimeStampArray objectAtIndex:index];
		lbIsDifferentFromLocalFile = [DSSImageCacheManager isDifferentFromLocalFile:lImagePath forTimeStamp: lTimeStamp];
		if (lbIsDifferentFromLocalFile == true)
		{
			[opMissingImagesArray addObject:lImagePath];
		}
	}	
}

@end

