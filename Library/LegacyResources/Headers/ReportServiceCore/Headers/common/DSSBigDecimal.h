/*
 *  DSSBigDecimal.h
 *  DashboardViewer
 *
 *  Created by Xiaolong Wang on 12/29/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef BIGDECIMAL_H
#define BIGDECIMAL_H
#import "../../Classes/common/DSSBaseString.h"
//#import "Base/Interfaces/BaseException.h" //no implementation now, 
#import "Base/Defines/ReturnPtr.h"

using namespace std;

namespace MDataType
{
	//==============================================================================================
	// Bigdecimal represents an arbitrary length integer or fixed point number
	// [it is a immutable object except that one can create an empty one using default constructor and 
	//	then assign. It can only be assigned once
	// It's size is IMMUTABLE - you can not make a bug fix which creates new member!!!]
	// NOT TRUE in this implementation as of no Buffer in use. 
	//==============================================================================================
	
	class DSSBigDecimal
	{
	public:

		typedef MBase::ReturnPtr<DSSBigDecimal> ReturnPtr;
		DSSBigDecimal();
		DSSBigDecimal(const DSSBigDecimal& irSrc);

		DSSBigDecimal(const char *ipStr);
		DSSBigDecimal(const double iValue);
#ifndef NO_WCHAR_SUPPORT
		DSSBigDecimal(const wchar_t *ipwStr);
#endif
        
        //xzeng add 09/28/2011
        int CallCompareTo(const DSSBigDecimal &irSrc) const;
		
		virtual ~DSSBigDecimal();
		DSSBigDecimal& operator = (const DSSBigDecimal& irSrc);
		DSSBigDecimal* operator + (const DSSBigDecimal& irSrc);
		bool operator < (const double data2) const;
		bool operator == (const double data2) const;

		bool operator < (const DSSBigDecimal& data2) const;
		bool operator == (const DSSBigDecimal& data2) const;
		
		void GetValue(MBase::CString &irStr);
		MBase::CString ConvertToString() const;

		
		MBase::CString GetIntegerDigits() const;
		MBase::CString GetFractionDigits() const;
		int Signum() const;
		bool IsInvalid() const; 
		int Scale() const;
		void StrToBigDecimal(const char *iStr);
        // DE33097 weilin 2016/6/22 mScale minus 2 for percentage format
        void ChangeScaleForPercentage(bool ibHasPositivePercentage, bool ibHasNegativePercentage);
        // DE33097 weilin 2016/6/22 mScale plus 2 for percentage format back
        void ChangeScaleForPercentageBack(bool ibHasPositivePercentage, bool ibHasNegativePercentage);
        // DE33097 weilin 2016/6/22 change signum
        void ChangeSignum() const;
        bool IsPositive() const;
	private:
		
		bool IsEmpty() const;
		

        
		int	CompareTo(const DSSBigDecimal &irSrc) const;
		DSSBigDecimal* CreateBigDecimal(void *iBigInt, int iScale);

		// members

		int mScale;
		
		void *mBigInt;
	};
	//class BigDecimalException:public MBase::BaseException{};
}

#endif
