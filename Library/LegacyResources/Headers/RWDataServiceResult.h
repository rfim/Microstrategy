//
//  RWDataServiceResult.h
//  MSTRMobile
//
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>

@class PageByInfo;
@class MSIReportDisplayInfo;

/**
 * This is to abstract binary data for document, we need to associate a cacheID with it
 * 
 */
@interface RWBinaryDataSlice : NSObject
{
	NSString *  dataPath;
    NSString*   cacheID;
}

@property (nonatomic, readonly) NSString* dataPath;
@property (nonatomic, readonly) NSString* cacheID;

- (RWBinaryDataSlice*)initWithDataPath:(NSString*)iDataPath andCacheID:(NSString*)iCacheID;

@end

@interface RWDataServiceResult : NSObject 
{
	@private
	NSData* layoutXML;
	NSData* groupByXML;
	// Renamed from pdfData to contentData
	NSData* contentData;
	NSString* contentDataPath;
	// Renamed from currentGroupByData to reportInfoXML
	NSData* reportInfoXML;
	NSDictionary* images;
	
	NSString *reportName; //RWName, DataModel conformance
	BOOL hasPageBy; //hasGroupBy, DataModel conformance
	PageByInfo* pageByInfo;
	BOOL needApplyToDI; // if the data needs to be applied to Document Instance
	BOOL needReDoManipulation;	
	BOOL needRenderDoc;
	
	BOOL renderDirectly;
	NSString* requestedLayoutKey;
    MSIReportDisplayInfo* drilledReportInfo;
    NSArray* separatedBinaries;
    BOOL needEvaluateViews;
}

/**
 * Initialization Method
 *
 * @param layoutXML layout XML
 * @param groupByXML grouby XML
 * @param pdfData PDF data
 * @param currentGroupBy current groupby data
 * @return New instance created
 */
- (RWDataServiceResult *) initWithLayout:(NSData*)layoutXML 
								 groupBy:(NSData*)groupByXML 
									data:(NSData*)pdfData 
						  currentGroupBy:(NSData*)currentGroupBy;

/**
 * Initialization Method
 *
 * @param layoutXML layout XML
 * @param groupByXML grouby XML
 * @param pdfData PDF data
 * @param currentGroupBy current groupby data
 * @return New instance created
 */
- (RWDataServiceResult *) initWithLayout2:(NSData*)layoutXML 
								  groupBy:(NSData*)groupByXML 
									 data:(NSData*)pdfData 
						   currentGroupBy:(id)currentGroupBy;

/**
 * Initialization Method
 *
 * @param layoutXML layout XML
 * @param pageByInfo pageby info
 * @param pdfData PDF data
 * @param currentGroupBy current groupby data
 * @return New instance created
 */
- (RWDataServiceResult *) initWithLayout:(NSData*)layoutXML 
				 			  pageByInfo:(PageByInfo*)pageByInfo 
									data:(NSData*)pdfData 
						  currentGroupBy:(NSData*)currentGroupBy;

/**
 * Initialization Method
 *
 * @param layoutXML layout XML
 * @param groupByXML grouby XML
 * @param binData binary data
 * @param binDataPath binary data path
 * @param pImages preview images
 * @return New instance created
 */
- (RWDataServiceResult *) initWithLayout2:(NSData*)layoutXML 
								  groupBy:(NSData*)groupByXML 
									 data:(NSData*)binData
								 dataPath:(NSString*)binDataPath
								   images:(id)pImages;

/**
 * Initialization Method for linked drill results
 *
 * @param iReportInfo drill target report/document
 * @param iBinDataPath binary data path
 * @return New instance created
 */
- (RWDataServiceResult *) initWithDrilledReport:(MSIReportDisplayInfo*)iReportInfo
                                     binaryPath:(NSString*)iBinDataPath;

/**
 * Initialization Method
 *
 * @param iBinDataPath binary data path
 * @param iBinaryArray An array of binaries
 * @param iBinData binary data
 * @param iImages preview images
 * @return New instance created
 */
- (RWDataServiceResult *) initWithBinaryPath:(NSString*)iBinDataPath
                                 binaryArray:(NSArray*)iBinaryArray
                                  binaryData:(NSData*)iBinData
                                      images:(id)iImages;

/**
 * Set report name
 *
 * @param reportName report name
 */
- (void) setReportName: (NSString *) reportName;

/**
 * Set true if it has PageBy.
 *
 * @param hasPageBy if has pageby
 */
- (void) setHasPageBy: (BOOL) hasPageBy;

/**
 * Set NeedApplyToDI
 *
 * @param value Use true if we need to apply data to document instance.
 */
- (void) setNeedApplyToDI: (BOOL) value;

/**
 * Set NeedReDoManipulation 
 *
 * @param value If we need to redo manipulation after data is passed in.
 */
- (void) setNeedReDoManipulation: (BOOL) value;

/**
 * Set NeedRenderDoc
 *
 * @param value If we need render the document after data is passed in.
 */
- (void) setNeedRenderDoc: (BOOL) value;

/**
 * Set NeedEvaluateViews
 *
 * @param value If we need evaluate views after applying datasets and before rendering.
 */
- (void) setNeedEvaluateViews: (BOOL) value;

/**
 * Release content data
 */
- (void) releaseContentData;

/**
 * is CSI binary result
 */
- (BOOL) isCSIResult;

@property (nonatomic, readonly) NSData		 *layoutXML;
@property (nonatomic, readonly) NSData		 *groupByXML;
@property (nonatomic, readonly) NSData		 *contentData;
@property (nonatomic, readonly) NSString	 *contentDataPath;
@property (nonatomic, readonly) NSData		 *reportInfoXML;
@property (nonatomic, readonly) NSDictionary *images;
@property (nonatomic, readonly) PageByInfo   *pageByInfo;
@property (nonatomic, readonly) BOOL needApplyToDI;
@property (nonatomic, readonly) BOOL needReDoManipulation;
@property (nonatomic, readonly) BOOL needRenderDoc;
@property (nonatomic, strong) NSString* requestedLayoutKey;
@property (nonatomic, strong) MSIReportDisplayInfo* drilledReportInfo;
@property (nonatomic, readonly) NSArray* separatedBinaries;
@property (nonatomic, readonly) BOOL needEvaluateViews;

/**
 * This property may return either PageByInfo or NSData containing page by XML
 **/
@property (weak, nonatomic, readonly) id          groupByData;

//DataModel
//! stores document name
/*!
 Doesn't get populated from data, but rather by RWDataController. This is because XML parsing is not the responsibility of RWDataServiceResult.
 */
@property (weak, nonatomic, readonly)		NSString  *reportName; 

//! indicates whether this RWD has group-by or not
/*!
 Not populated from the data; this is due to the fact that groupByXML is not always fetched for better performance.
 */
@property (nonatomic, readonly)		BOOL	  hasPageBy;

@property (nonatomic) BOOL renderDirectly;

@end
