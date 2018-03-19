//==============================================================================================
//	FILENAME	:	CellFmtData.h
//	AUTHOR		:	Zhongchao Yu
//	CREATION	:	09/22/2005
//	Copyright (C) MicroStrategy, Inc. 2005
//==============================================================================================
#ifndef MDataType_CellFmtData_h
#define MDataType_CellFmtData_h

#include "Base/Base/GUID.h"
#include "Base/Base/String.h"
#include "Base/Base/ReturnString.h"
#include "Base/Base/Language.h"

#include "DataType/DataType/DataType.h"
#include "DataType/DataType/CellFmtDataTypes.h"
#include "PDCHeader/PDCmap"


namespace MDataType
{
	class DLL_DATATYPE_EXIM CellFmtStruct
	{
	public:
		CellFmtStruct();
		~CellFmtStruct() throw();

		bool operator == (const CellFmtStruct& iCellFmtStruct) const;

		EnumCellFmtNumberCategory mCategory; // Format category
		MBase::String mFormatString;  // Store a format string as specified in Cell Level Formatting document 
		long mBackColor; // Background color
		long mForeColor; // Color in which cell value is displayed	

		// Only SAP
		// MWKZ field in CellData table
		// amount		'W',
		// quantity		'M',
		// price		'P',
		// ratio		'Q',
		// percent		'%',
		// counter		'C',
		// float		'F',
		// date			'D',
		// time			'T',
		// else			'S',
		// mixed_dim	'*',  // Only 2004s
		// char			'A',
		// inactive		'I',
		MBase::String mMWKZ; // MWKZ field for the cell

		short mPosition; //0: Left, 1: Right, 2: Left-space, 3:Right-space
		MBase::String mCurrency; // Currency

		bool bFlipSign; 
		short mScalingFactor; // 0 for no scaling, 1 for 10's, 2 for 100's, 3 for 1000's
		short mPrecision; // Number of digits after decimal place

		//Only MSAS
		MBase::String mFontName; // Font name
		short mFontSize; // Font size
		MBase::LanguageID mLanguageID; // Language
	};

	typedef CellFmtStruct * CellFmtStructPtr;

	//==============================================================================================
	// CellFmtData is a concrete class represent the Cell Formatting Data. Besides values, it
	// also contains types and format objects. 

	// Operations on CellFmtData assume "return by value" as opposed to the "return by pointer"
	// in the favor of simplicity. All compilers but CC on Solaris support "Named Return Value 
	// Return Value Optimization" by default. Thus the overhead is mitigated.
	//==============================================================================================

	class DLL_DATATYPE_EXIM CellFmtData 
	{
	public:
		// Constructors
		CellFmtData(); // default constructor
		CellFmtData(double iDouble, EnumCellFmtDataTypes iType, const GUID& iFmtID);

		// Copy constructor  (for NRVO)
		CellFmtData(const CellFmtData& iCellFmtData);

		// Getters
		double GetValue() const;
		CellFmtStructPtr GetCellFmtStructPtr() const; 
		EnumCellFmtDataTypes GetType() const;
		GUID GetFmtID() const;

		// Setters
		void PutValue(double iDouble);
		void PutCellFmtStructPtr(CellFmtStructPtr iPtr);
		void PutType(EnumCellFmtDataTypes iType);
		void PutFmtID(const GUID& iFmtID); 

		bool operator==(const CellFmtData& iCellFmtData) const;

		// The following is only useful for ordering purpose.
		bool operator<(const CellFmtData& iCellFmtData) const;

		// For serialization
		// Caller is responsible to allocate memory for ipBuffer
		Int32 SaveObj(unsigned Int32 inSize, void* ipBuffer, Int32 mode) const;
		Int32 LoadObj(unsigned Int32 inSize, void* ipBuffer);

	private:
	
		double mValue;
		CellFmtStructPtr mCellFmtStructPtr;
		EnumCellFmtDataTypes mType;
		GUID	mFmtID;
	};

} // MDataType
#endif // MDataType_CellFmtData_h
