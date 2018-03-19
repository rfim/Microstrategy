//==============================================================================================
//	FILENAME	:	Variant.h
//	AUTHOR		:	Yuxiao Xiao
//	CREATION	:	8/24/01
//	Copyright (C) MicroStrategy, Inc. 2001
//==============================================================================================

#ifndef _MSIVARIANT_H_
#define _MSIVARIANT_H_

#include "DataType.h"
#include "Base/Base/String.h"
#include "Base/Base/ReturnString.h"
#include "Base/Interfaces/BaseException.h"
#include "Base/Interfaces/BaseMemoryContract.h"
#include "Base/Base/Buffer.h"
#if defined(_NO_EXCEPTION)
#include <stdio.h>
#endif

namespace MDataType {

class DateTime;
class BigDecimal;
class Binary;
class CellFmtData;

class VariantException:public MBase::BaseException {};

//==============================================================================================
// Variant represents variant structure used MSI wide
//
// It's size is IMMUTABLE, you can not make bug fix which create new member!!!
//==============================================================================================

class DLL_DATATYPE_EXIM Variant 
{

public :

	// according to DssDataTypeXXX
	enum DATA_TYPE
	{	
		MSI_SHORT = 21,  
		MSI_INTEGER = 1, 
		MSI_LONG = 22, 
		MSI_FLOAT = 7, 
		MSI_DOUBLE = 6, 
		MSI_DATE = 14, 
		MSI_TIME = 15, 
		MSI_DATETIME = 16, 
		MSI_STRING = 9, 
		MSI_MBCS_STRING = 23, 
		MSI_BINARY = 11,
		MSI_BIGDECIMAL = 30,
		MSI_CELLFMTDATA = 31, 
		MSI_INVALID = -1 ,
		MSI_UNSIGNED_INTEGER = 2,	// DssDataTypeUnsigned	= 2,
		MSI_UTF8_STRING = 33

	};

	//
	// constructors.
	//
	Variant(MBase::Buffer* ipBuffer = NULL);
	Variant(short iSval, MBase::Buffer* ipBuffer = NULL);
	Variant(Int32 iIval, MBase::Buffer* ipBuffer = NULL);
	Variant(unsigned Int32 iIval, MBase::Buffer* ipBuffer = NULL);
	Variant(float iFval, MBase::Buffer* ipBuffer = NULL);
	Variant(double iDval, MBase::Buffer* ipBuffer = NULL);
	Variant(const char*	iStval, MBase::Buffer* ipBuffer = NULL);
#ifndef NO_WCHAR_SUPPORT
	Variant(const wchar_t* iStval, MBase::Buffer* ipBuffer = NULL);
#endif
	Variant(const DateTime& iDtval, MBase::Buffer* ipBuffer = NULL);
	Variant(const BigDecimal& iBDval, MBase::Buffer* ipBuffer = NULL);
	Variant(const Binary* irBinary, MBase::Buffer* ipBuffer = NULL);
	Variant(const CellFmtData &iCellFmtData, MBase::Buffer* ipBuffer = NULL);

	// yluo 11/16/06: we need a Variant that is governed by MemoryContract, but not by Buffer.
	Variant(MBase::MemoryContract* ipMemContract);
	
	// copy constructor
	Variant(const Variant& iVariant);

	// destructor
	~Variant() throw();

	// = operators
	Variant&	operator = (short iSval);
	Variant&	operator = (Int32 iIval);
	Variant&	operator = (unsigned Int32 iIval);
	Variant&	operator = (float iFval);
	Variant&	operator = (double iDval);
	Variant&	operator = (const char* iStval);
#ifndef NO_WCHAR_SUPPORT
	Variant&	operator = (const wchar_t* iStval);
#endif
	Variant&	operator = (const DateTime& irDateTime);
	Variant&	operator = (const BigDecimal& irBigDecimal);
	Variant&	operator = (const Binary* irBinary);
	Variant&	operator = (const CellFmtData& iCellFmtData);
	Variant&	operator = (const Variant& irVariant);

	//lcui: TQMS 243505	 8/25/2006
	void SetDate(const DateTime& irDateTime);

	// casting operators. Each of these methods convert the Variant type to the
	// return type indicated.

	// throws VariantException if mDateType is not numeric
	operator	short() const;
	// throws VariantException if mDateType is not numeric
	operator	Int32() const;
	// throws VariantException if mDateType is not numeric
	operator	unsigned Int32() const;
	// throws VariantException if mDateType is not numeric
	operator	float() const;
	// throws VariantException if mDateType is not numeric
	operator	double() const;

	// throws VariantException if mDateType is not MSI_DATETIME
	operator	DateTime() const;
	const DateTime* ToDateTime() const;

	// throws VariantException if mDateType is not MSI_BIGDECIMAL
	const BigDecimal* ToBigDecimal() const;

	operator CellFmtData() const;

	// throws VariantException if mDateType is not MSI_BINARY
	// For performance concern, this call returns a weak reference to the internal data. 
	// DO NOT delete or make any modification on the pointer.
	// If caller want to keep a copy, he should make a duplicate.
	const Binary* ToBinary() const;

	DATA_TYPE	GetVarType() const;

	// MSI_STRING, MSI_DATETIME, MSI_BINARY is only comparable with the exact same type
	// build types (short, long, float, double) are all comparable.
	bool IsTypeComparable(const Variant& irVariant) const;

	// return false is two varant types are incomparable
	// otherwise based on the value comparision
	bool operator == (short iSval) const;
	bool operator == (Int32 iIval) const;
	bool operator == (unsigned Int32 iIval) const;
	bool operator == (float iFval) const;
	bool operator == (double iDval) const;
	bool operator == (const char* iStval) const;
#ifndef NO_WCHAR_SUPPORT
	bool operator == (const wchar_t* iStval) const;
#endif
	bool operator == (const DateTime& iDtval) const;
	bool operator == (const BigDecimal& iBdval) const;
	bool operator == (const Binary& irBinary) const;
	bool operator == (const Variant& irVariant) const;
	bool operator == (const CellFmtData& iCellFmtData) const;

	// throws VariantException is two types are not comparable
	// otherwise based on the value comparision
	bool operator < (const Variant& irVariant);

	// Return mpStval if data type is MSI_STRING, otherwise return NULL
	const MBase::String* GetRawWString() const;

#ifndef NO_WCHAR_SUPPORT
	// if it's a Binary type, this call will always return a NULL string
	MBase::ReturnWString ToWString() const;

	// the following two methods are counterpart of ToWString, with less memory copies.
	// it's optimized for MSI_STRING, NOT recommented for other types.

	// returns the length of wstring representation of the variant.
	unsigned Int32 GetWStringLength() const;
	
	// inBufferLen is the size of opcBuffer
	// if inBufferLen is less than or equal to the length of the string, a null character is NOT appended automatically to opcBuffer 
	// if inBufferLen is greater than the length of the string, the destination string is padded with null characters up to length count. 
	void ToWString(wchar_t* opcBuffer, unsigned Int32 inBufferLen) const;
#endif

	// if it's a Binary type, this call will always return a NULL string
	MBase::ReturnString ToString() const;
	
	// the following methods are counterpart of ToString, with less memory copies.
	// it's optimized for MSI_STRING, NOT recommented for other types.
	
	// returns the length of string representation of the variant.
	unsigned Int32 GetStringLength() const;
	
	// inBufferLen is the size of opcBuffer
	// if inBufferLen is less than or equal to the length of the string, a null character is NOT appended automatically to opcBuffer
	// if inBufferLen is greater than the length of the string, the destination string is padded with null characters up to length count. 
	void ToString(char* opcBuffer, unsigned Int32 inBufferLen) const;

	void swap(Variant& irOther) throw ();

	friend void swap(Variant& irVar1, Variant& irVar2) throw ();

	// long pan 2010-04-29: TQMS 388539, specially handle bulk export result
	MBase::ReturnString ToStringVBBehavior() const;
	
private:

	void Reset() throw();
	void TakeValue(Variant& irOther) throw ();

	inline void CheckMemoryAvailability(unsigned Int64 iRequestSize)
	{
		if ( ! mpMemContract.IsNull() && mpMemContract->Alloc64(iRequestSize) != MCM_OK )
		{
#if !defined(_NO_EXCEPTION)
			throw ("Not enough memory left.");
#else
			printf("%s\n","Not enough memory left.");
			// TODO: What should we do here?
#endif
			
		}
	};

	DATA_TYPE	mDataType;

	union 
	{
		short	mSval;
		Int32		mIval;
		unsigned Int32		mUIval;
		Int32	mLval;
		float	mFval;
		double	mDval;
		DateTime*		mpDtval;	
		BigDecimal*		mpBdval;
		MBase::String*	mpStval;
		Binary*			mpBinary;
		CellFmtData*	mpCellFmtData;
		void*			mpReserved;
	};


	MBase::Buffer*		mpBuffer;

	MBase::MemoryContractPtr mpMemContract;
};

}

#endif
