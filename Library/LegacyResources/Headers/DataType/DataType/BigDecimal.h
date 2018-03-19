//==============================================================================================
//	FILENAME	:	BigDecimal.h
//	AUTHOR		:	Xun Feng
//	CREATION	:	2/11/04
//	Copyright (C) MicroStrategy, Inc. 2001
//==============================================================================================
#ifndef MDataType_BigDecimal_h
#define MDataType_BigDecimal_h

#include "PDCHeader/PDCwtypes.h"

#include "DataType/DataType/DataType.h"

#include "Base/Base/String.h"
#include "Base/Base/ReturnString.h"
#include "Base/Interfaces/BaseException.h"

namespace MDataType
{
	//==============================================================================================
	// Bigdecimal represents an arbitrary length integer or fixed point number
	// it is a immutable object except that one can create an empty one using default constructor and 
	//	then assign. It can only be assigned once
	// It's size is IMMUTABLE - you can not make a bug fix which creates new member!!!
	//==============================================================================================

	class DLL_DATATYPE_EXIM BigDecimal
	{

	private:
		// members
		Int32 mScale;
		MBase::Buffer *mBuffer;
		void * mBigInt;	// use void to avoid exposing BigInteger to outside

	public:

		typedef MBase::StrongPtr<BigDecimal> Ptr;
		typedef MBase::ReturnPtr<BigDecimal> ReturnPtr;
		
		BigDecimal(MBase::Buffer *iBuffer = 0);

		BigDecimal(const char *iStr, MBase::Buffer *iBuffer = 0);
#ifndef NO_WCHAR_SUPPORT
		BigDecimal(const wchar_t* iStr, MBase::Buffer *iBuffer = 0);
#endif
		BigDecimal(int val, MBase::Buffer *iBuffer = 0);

		BigDecimal(double val, MBase::Buffer *iBuffer = 0);

		BigDecimal(const BigDecimal& iRight, MBase::Buffer *iBuffer = 0);

		~BigDecimal();

		// comparison
		bool operator==(const BigDecimal& right) const;

		BigDecimal& operator=(const BigDecimal& right);

		bool operator> (const BigDecimal& right) const;
		bool operator< (const BigDecimal& right) const;


		// arithmatic calculations
		ReturnPtr operator+(const BigDecimal& right) const;
		   //  Returns a BigDecimal whose value is (this + right), and whose scale is max(this.scale(), right.scale()). 

		ReturnPtr operator-(const BigDecimal& right) const;
		   //  Returns a BigDecimal whose value is (this - right), and whose scale is max(this.scale(), right.scale()). 

		ReturnPtr operator*(const BigDecimal& right) const;
		   //  Returns a BigDecimal whose value is (this * right), and whose scale is (this.scale() + right.scale()). 

		ReturnPtr operator/(const BigDecimal& right) const;
		   //   Returns a BigDecimal whose value is (this / right), and whose scale is this.scale()

		ReturnPtr Divide(const BigDecimal& right, Int32 req_cale = -1) const;
		   //   Returns a BigDecimal whose value is (left / right), and whose scale is req_scale or this.scale() if req_scope is -1
#ifndef NO_WCHAR_SUPPORT
		MBase::ReturnWString ToWString() const;

		MBase::ReturnWString ToWString(LCID inLocale) const;
#endif
		MBase::ReturnString ToString() const;

		// Comparison
		Int32 CompareTo(const BigDecimal& val) const;
    		// Compares this BigDecimal with the specified BigDecimal.
			// -1 means this is smaller than val, 0 means equal, 1 means greater

		// property that describes the number
		Int32 Scale() const;
    		// Returns the scale of this BigDecimal. 

		Int32 Signum() const; 
			// Returns the signum function of this BigDecimal.

		Int32 Precision() const;
#ifndef NO_WCHAR_SUPPORT
		MBase::ReturnWString MantissaToWString() const;
			// Returns the string representation of this BigDecimal's mantissa part.
#endif
		bool IsInvalid() const;

		MBase::ReturnString GetIntegerDigits() const;

		MBase::ReturnString GetFractionDigits() const;

	private:
		void _StrToBigDecimal(const char *iStr);

		void _CreateInvalidBigDecimal();

		const char * _FindChr(const char *iStr, const char *iEnd, char ch);

		bool _CheckDigits(const char* iStr, const char* iEnd, bool & oAllZero, char ch = '\0');

		bool IsEmpty() const;

		static BigDecimal * CreateBigDecimal(void *iBigInt, Int32 iScale, MBase::Buffer *iBuffer);
	};

class BigDecimalException:public MBase::BaseException {};

}
#endif // MDataType_BigDecimal_h
