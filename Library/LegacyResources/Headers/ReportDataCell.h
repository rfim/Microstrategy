//
//  ReportDataCell.h
//  iPadMain Library
//
//  Created by Liang Chen on 12/2/10.
//  Copyright 2010 MicroStrategy Inc. All rights reserved.
//


//@class ReportDataHeader;
@class MSIHeader;
@interface MSIReportDataCell : NSObject {
	
	id displayObject;
//	ReportDataHeader* header;
	MSIHeader *header;


}
@property (nonatomic, strong, readonly) id displayObject;
//@property (nonatomic, readonly) ReportDataHeader* header;
@property (nonatomic, strong, readonly) MSIHeader *header;

- (id) initWithData:(id)_displayObject header:(MSIHeader*)_header;
- (NSString*) displayValue;
@end
