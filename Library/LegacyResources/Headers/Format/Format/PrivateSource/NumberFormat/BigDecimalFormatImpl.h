//==============================================================================================
//	FILENAME	:	BigDecimalFormatImpl.h
//	AUTHOR		:	Xun Feng
//	CREATION	:	3/4/04
//	Copyright (C) MicroStrategy, Inc. 2001
//==============================================================================================
#ifndef MFormat_BigDecimalFormatImpl_h
#define MFormat_BigDecimalFormatImpl_h

#include "PDCHeader/PDCmap"

#include "Format/Format/BigDecimalFormat.h"
#include "Synch/Synch/CriticalSection.h"
#include "Base/Base/ReturnString.h"
#include "DecimalFormatWrapper.h"

namespace MFormat
{
	class NumberFormat;
	typedef MBase::String UnicodeString;
	
	class BigDecimalFormatImpl:	public BigDecimalFormat
	{
	public:

		BigDecimalFormatImpl();

		~BigDecimalFormatImpl() throw();

		MBase::ReturnWString BigDecimalToText(const MDataType::DSSBigDecimal& inNumber, const WCHAR* ipcFormat, LCID inLocale = 0);
		MBase::ReturnWString BigDecimalToText(const	MDataType::DSSBigDecimal& inNumber, Int32 inHandle, LCID inLocale = 0) const;
		MDataType::DSSBigDecimal::ReturnPtr TextToBigDecimal(const WCHAR* ipcText, LCID inLocale = 0);
	
	private:
		MBase::ReturnWString Format(const MDataType::DSSBigDecimal& ifNumber, const WCHAR* ipcFormat, LCID inLocale = 0) const;
		MBase::ReturnWString FormatNumber(const MDataType::DSSBigDecimal& ifNumber, DecimalFormatWrapper* ipFormat, LCID inLocale = 0) const;
		void GetNumberFormat(const WCHAR* ipcFormat, LCID inLocale, DecimalFormatWrapper*& oprFormat) const;

		static void AppendIntString(MBase::String& ioStr, std::basic_string<char>& iInt, int iFillSize, TCHAR iGroupChar, int iGroupSize);
		static void AppendFractionString(MBase::String& ioStr, std::basic_string<char>& iFraction, int iMin, int iMax);

		int FindSubFormat(const MDataType::DSSBigDecimal& ifNumber, DecimalFormatWrapper* ipFormat, bool& lChangeSign) const;
		
		NumberFormat* hCreateNumberFormat() const;
		
		//temporarily commented out.
		static void AppendUnicodeString(MBase::String& ioStr, UnicodeString& iUS);
		
		
		DecimalFormatWrapperPtr			mpDefaultFormatWrapper;
		MFormat::NumberFormat* mpNumberFormat; 
	};
}

#endif // MFormat_BigDecimalFormatImpl_h
