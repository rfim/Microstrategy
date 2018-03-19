//==============================================================================================
//	FILENAME	:	NumberFormatImpl.h
//	AUTHOR		:	Yuxiao Xiao
//	CREATION	:	9/10/01
//	Modified by Liang Liu at May 27, 2010. 
//	Copyright (C) MicroStrategy, Inc. 2001
//==============================================================================================
#ifndef MFormat_NumberFormatImpl_h
#define MFormat_NumberFormatImpl_h

#include "PDCHeader/PDCmap"

#include "Format/Format/NumberFormat.h"
#include "Base/Base/ReturnString.h"

#include "DecimalFormatWrapper.h"

namespace MFormat
{
	class NumberFormatImpl:
		public NumberFormat
	{
	friend class BigDecimalFormatImpl;
	public:

		NumberFormatImpl();

		~NumberFormatImpl() throw();

		MBase::ReturnWString DoubleToText(double ifNumber, const WCHAR* ipcFormat, LCID inLocale = 0, Int32* opColor = NULL);
		
		void Clear() throw ();

		WCHAR* DoubleToText2(double ifNumber, const WCHAR* ipcFormat, LCID inLocale = 0, Int32* opColor = NULL);
	

	private:

		NumberFormatImpl(const NumberFormatImpl& irSrc);
		NumberFormatImpl& operator = (const NumberFormatImpl& irSrc);

		void GetNumberFormat(const WCHAR* ipcFormat, LCID inLocale, DecimalFormatWrapper*& oprFormat) const;

		WCHAR* Format(double ifNumber, const WCHAR* ipcFormat, LCID inLocale = 0, Int32* opColor = NULL);

		WCHAR* FormatNumber(double ifNumber, DecimalFormatWrapper* ipFormat, LCID inLocale = 0, Int32* opColor = NULL) const;

		void GetDecimalFormatWrapper(Int32 inHandle, DecimalFormatWrapper*& oprFormat) const;

		int FindSubFormat(double InifNumber, DecimalFormatWrapper* ipFormat, bool& lChangeSign) const;

		// private member
		DecimalFormatWrapper*		mpDefaultFormatWrapper;	
		mutable DecimalFormatWrapper*		mpCurrentFormatWrapper;			
		mutable MBase::String mCurrentFormatString;
#ifndef __ANDROID__		
		CFLocaleRef mpCurrentLocale;
#else
		LCID mCurrentLocale;
#endif
	};
}

#endif // MFormat_NumberFormatImpl_h
