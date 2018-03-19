//==============================================================================================
//	FILENAME	:	DecimalFormatWrapper.h
//	AUTHOR		:	Yuxiao Xiao
//	CREATION	:	9/10/01
//	Modified by Liang Liu at May 25, 2010. 
//	Copyright (C) MicroStrategy, Inc. 2001
//==============================================================================================
#ifndef MFormat_DecimalFormatWrapper_h
#define MFormat_DecimalFormatWrapper_h

#include "Base/Base/BaseString.h"
#include "Base/Interfaces/BaseException.h"
#include "Synch/Synch/AtomicLong.h"
#include "Base/Defines/StrongPtr.h"
#include "PDCHeader/PDCmap"

#ifndef __ANDROID__
	#include <CoreFoundation/CFNumberFormatter.h>
#endif

#define MAX_SUBFORMATS 4
namespace MFormat
{
	class DecimalFormatWrapper;
	class NumberFormatImpl;
	typedef std::map<int, MBase::String> SymbolMap;
	
	class ColorText
	{
	public:
		ColorText(const WCHAR* ipcText, Int32 inColor);
		ColorText(const ColorText& irSrc);
		
		friend class DecimalFormatWrapper;
		
	private:
		ColorText& operator = (const ColorText& irSrc);
		
		MBase::String	mText;
		long			mnColor;
	};
	
	
	class CondText
	{
	public:
		CondText(const WCHAR* ipcText, Int32 iType);
		CondText(const CondText& irSrc);
		
		friend class DecimalFormatWrapper;
		
	private:
		CondText& operator = (const CondText& irSrc);
		
		MBase::String mText;
		Int32		  mType;
	};
	
	//typedef MBase::StrongPtr<DecimalFormat> DecimalFormatPtr;
	typedef MBase::StrongPtr<DecimalFormatWrapper> DecimalFormatWrapperPtr;
	
	class SubFormat
	{
	public:
		SubFormat();
		~SubFormat() throw();
		
		friend class DecimalFormatWrapper;
		friend class NumberFormatImpl;
		friend class BigDecimalFormatImpl;
		
	private:
		SubFormat& operator = (const SubFormat& irSrc);
		
		long				mColor;
		bool				mbIsTime;
#ifndef __ANDROID__
		CFNumberFormatterRef		mpFormat;
#else
		void*						mpFormat;	//jobject.
#endif
		MBase::String		mTimeFormat;
		bool				mbIsCond;
		Int32				mType;
		double				mValue;
		SymbolMap			mSymbolMap;
		Int32				mScalingFactor;
		Int32				mSymbolLen;
	};
	
	class DecimalFormatWrapper
	{
	public:
#ifndef __ANDROID__
		DecimalFormatWrapper(const WCHAR* ipcFormat, CFLocaleRef ipLocale);
#else
		DecimalFormatWrapper(const WCHAR* ipcFormat, LCID iLocale);
#endif
		
		~DecimalFormatWrapper() throw ();
		
		friend class NumberFormatImpl;	
		friend class BigDecimalFormatImpl;
	private:
		
		//DecimalFormatWrapper& operator = (const DecimalFormatWrapper& irSrc);
		void LowerWString(WCHAR* ipcWString, int iLength);
		
		long				mID;
		// There are four sub formats for positive, negative, zero and text vales.
		// If only one if present it is used for positive, negative and zero. Text is general format
		// If two is given, the first is positive and zero, the second is negative. Text is general format
		// If three, the text is again general format.
		// In addition if any sub format has a condition, the condition takes precedence. The values that don't
		// meet the condition take the format of the first sub format that can be applied.
		// IMPORTANT: As of now the text format is always ignored. It is assumed that we always get a number here
		
		bool				mbCondUsed;
		// If conditions are used and if any of the conditions apply for both positive and negative numbers,
		// then this flag is set
		// eg. [>-1], [<100], <>, "[>100]#;0" -> second format applies to positive and negative numbers
		// If we have conditions involving >0, <0, >=0 and <=0 then we know that mixed ranges do not occur in 
		// which case we set the following flag
		
		// When a number matches a condition, and if that number is negative; then if this flag is not set, 
		// when passing to ICU we do not change signs because ICU will introduce negative sign. 
		// If the flag is set we change the sign because we know this range is for negative numbers
		// and we do not want ICU to introduce a negative sign, so we format it as positve number
		
		// When a number does not fall in any condition, all above logic applies BUT in addition the sign is only
		// changed if we are using the second format (that is the format that would be used for negative
		// numbers if condition was not involved)
		bool				mbSpecialCase;
		short				mnSubFormat;
		SubFormat			mvSubFormat[MAX_SUBFORMATS];
		static MSynch::AtomicLong mCounter;
		bool				mbGeneralFormat;
#ifndef __ANDROID__		
		CFLocaleRef mpLocale;
#else
		LCID mLocale;
#endif
		
		//static MBase::StrongPtr<U_ICU_NAMESPACE::NumberFormat>	mpNeutralFormat;
	};
}

#endif // MFormat_DecimalFormatWrapper_h
