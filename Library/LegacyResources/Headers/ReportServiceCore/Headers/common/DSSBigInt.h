/*
 *  DSSBigInt.h
 *  ReportServiceCore
 *
 *  Created by xiaoyan yu on 8/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef BIGINT_H
#define BIGINT_H


namespace MDataType
{
	class DSSBigInt
	{
		friend class DSSBigDecimal;
	private:
		DSSBigInt();
		DSSBigInt(const char* iArray, int iDigits, int iSignum, int iExtra0);
		DSSBigInt(const DSSBigInt& iBigInt);
		~DSSBigInt();
		DSSBigInt* Clone(int iSignum) const;
		int Signum() const;
		
		void GetRawForm(const char*& orpArray, int &orSignum, int &orDigits);
		int CompareABS(const DSSBigInt& irVal, int iScaleDiff) const;
		static DSSBigInt* Calculate(int iCal, const DSSBigInt* left, const DSSBigInt * right, int iScaleDiff);
		static DSSBigInt* Plus(const DSSBigInt* left, const DSSBigInt* right,int iScaleDiff);
		static DSSBigInt* Minus(const DSSBigInt* left, const DSSBigInt* right,int iScaleDiff);
		static DSSBigInt* CreateZero();
		
		// members
		int mSignum;
		int mDigits;
		char * mNum;
		
	};
}

#endif
