//
//  BarCodeCB.h
//  BarCodePOC
//
//  Created by Jose Galvez Elias on 3/18/10.
//  Copyright 2010 MicroStrategy, Inc.. All rights reserved.
//

#import <Foundation/Foundation.h>

/** decoded symbol type. */
typedef enum mabarcode_type_e {
    BARCODE_UPCA,
	BARCODE_UPCE,
	BARCODE_EAN13,
	BARCODE_EAN8,
	BARCODE_QRCODE,
	BARCODE_UNKNOWN
} mabarcode_type_t;

typedef enum mabarcode_error_e {
    BARCODE_SUCCESS = 0,
	BARCODE_ERROR = -1,
	BARCODE_CANCEL = 1
} mabarcode_error_t;



@interface BarCodeCB : NSObject {
	
	NSString *code;
	mabarcode_type_t type;
	NSString *typeName;
	mabarcode_error_t errorCode;

}

@property (nonatomic, copy) NSString *typeName;
@property (nonatomic, copy) NSString *code;
@property (nonatomic, assign) mabarcode_error_t errorCode;
@property (nonatomic, assign) mabarcode_type_t type;


@end
