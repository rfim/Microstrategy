//==============================================================================================
//	FILENAME	:	CellFmtDataTypes.h
//	AUTHOR		:	Zhongchao Yu
//	CREATION	:	09/22/2005
//	Copyright (C) MicroStrategy, Inc. 2005
//==============================================================================================

#ifndef CellFmtDataTypes_h
#define CellFmtDataTypes_h 1

// PLEASE READ BEFORE YOU DO ANY CHANGE TO THIS FILE !!!!!

/*****************************************************************************************

	zyu 10-21-2005
	
	The two files, DSSCellFormatData.idl and CellFmtDataTypes.h
	must be synched at all times !!!
	
	Yes, it is ugly. The reason that we have to so is that CellFmtData must be put into
	the Common VOB so that the Database class can support it. The same thing has
	to be put into the IDLs because CellFmtData is handed from AE to COM through VARIANTs.
	However, we can't just define these types in one place and include it in the other.
	
	There could have been better way if our MiniMIDL were smarter. We have to hardcode
	the numbers because of some restrictions of MiniMIDL. 
		

******************************************************************************************/

typedef enum EnumCellFmtDataTypes
{
	CellFmtDataTypeReserved =  0,
	CellFmtDataTypeDouble	=  1,				// Float/Double						'F'
	CellFmtDataTypeMixedDim	=  2,				// Mixture type						'*'

	CellFmtDataTypeAmount	=  3,				// Amount (typically a currency)	'W'
	CellFmtDataTypeQuantity =  4,				// Quantity (some kind of unit)		'M'
	CellFmtDataTypePrice	=  5,				// Price (Amount/Quantity)			'P'
	CellFmtDataTypeRatio	=  6,				// Ratio (no units)					'Q'
	CellFmtDataTypePercent	=  7,				// Percentage						'%'
	CellFmtDataTypeCounter	=  8,				// Counter							'C'
	CellFmtDataTypeDate		=  9,				// Date								'D'
	CellFmtDataTypeTime		= 10,				// Time								'T'
	CellFmtDataTypeUnknown	= 11,				// Else/Unknown type				'S'
	CellFmtDataTypeChar		= 12,				// Character						'A'
	CellFmtDataTypeInactive = 13,				// Inactive cell					'I'

	CellFmtDataTypeOthersStart = 300

} EnumCellFmtDataTypes;

typedef enum EnumCellFmtNumberCategory
{
    CellFmtNumberCategoryFixed = 0,
    CellFmtNumberCategoryCurrency = 1,
    CellFmtNumberCategoryDate = 2,
    CellFmtNumberCategoryTime = 3,
    CellFmtNumberCategoryPercentage = 4,
    CellFmtNumberCategoryFraction = 5,
    CellFmtNumberCategoryScientific = 6,
    CellFmtNumberCategoryCustom = 7,
    CellFmtNumberCategorySpecial = 8,
    CellFmtNumberCategoryGeneral = 9,
} EnumCellFmtNumberCategory;

typedef enum EnumCellFmtCurrencyPosition
{
	CellFmtCurrencyPositionLeft = 0,
	CellFmtCurrencyPositionRight = 1,
	CellFmtCurrencyPositionLeftWithSpace =2,
	CellFmtCurrencyPositionRightWithSpace = 3,
} EnumCellFmtCurrencyPosition;


#endif // CellFmtDataTypes_h
