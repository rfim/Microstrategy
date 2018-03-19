//==============================================================================================
//	FILENAME	:	BigInt.h
//	AUTHOR		:	Xun Feng
//	CREATION	:	2/11/04
//	Copyright (C) MicroStrategy, Inc. 2001
//==============================================================================================
#ifndef MDataType_BigInt_h
#define MDataType_BigInt_h

#include "DataType/DataType/DataType.h"
#include "Base/Base/Buffer.h"

namespace MDataType
{
#define BigIntPlus 0
#define BigIntMinus 1
#define BigIntTime 2
#define BigIntDivide 3

	//==============================================================================================
	// BigInt represents an arbitrary length integer
	//==============================================================================================

	class BigInt
	{

	friend class BigDecimal;

	private	:
		// members
		Int32 mSignum;
		Int32 mDigits;
		Int32 mLeading0;
		char * mag;
		MBase::Buffer *mBuffer;

	private:
		// every method is private, only used by BigDecimal
		BigInt();
		~BigInt();
		BigInt(const BigInt& iBigInt, MBase::Buffer *iBuffer = 0);
				
		Int32 Signum() const;
		Int32 Precision() const;

		void GetRawForm(const char*& oArray, Int32 &oSignum, Int32& oDigits, Int32& oLeading0);

		// the input string may contain '.', ignore
		BigInt(const char *iArray, Int32 iDigits, Int32 iSignum, Int32 iExtra0, bool iKeepLeading0 = true, MBase::Buffer *iBuffer = 0);

		Int32 CompareABS(const BigInt& val, Int32 iScaleDiff) const;
		static BigInt* Calculate(Int32 iCal, const BigInt* left, const BigInt* right, Int32 iScaleDiff, MBase::Buffer *iBuffer, Int32 leftscale = 0, Int32 rightscale = 0, Int32 requestscale = 0);
		static BigInt* CalculateBuf(Int32 iCal, const BigInt* left, const BigInt* right, Int32 iScaleDiff, MBase::Buffer *iBuffer, Int32 iSignum, Int32 leftscale = 0, Int32 rightscale = 0, Int32 requestscale = 0);

		static void Plus(char *oArray, Int32& oDigits, const BigInt* left, const BigInt* right, Int32 iScaleDiff);
		static void Minus(char *oArray, Int32& oDigits, const BigInt* left, const BigInt* right, Int32 iScaleDiff);
		static void Times(char *oArray, Int32& oDigits, const BigInt* left, const BigInt* right);
		static void Divide(char *oArray, Int32& oDigits, const BigInt* left, const BigInt* right, Int32 shift);

		static int SearchMultiplyList(char* pmul[9], bool exist[9], char* dividend, Int32 iDigits_dividend, char* divisor, Int32 iDigits_divisor);
		static void GenMultiplyList(Int32 index, char* pmul[9], bool exist[9], char* divisor, Int32 iDigits);
		static void Subtract4divide(char *pdividend, Int32 dividend_length, Int32 digits_divisor, char* mul[9], Int32 result);

		BigInt* Clone(MBase::Buffer *iBuffer, Int32 iSignum) const;
			
		static BigInt* CreateZero(MBase::Buffer *iBuffer);
	};

}
#endif // MDataType_BigInt_h
