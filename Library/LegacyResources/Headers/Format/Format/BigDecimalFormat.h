//==============================================================================================
//	FILENAME	:	BigDecimalFormat.h
//	AUTHOR		:	Xun Feng
//	CREATION	:	3/4/04
//  Modified by Stone @ 2010-7-28
//	Copyright (C) MicroStrategy, Inc. 2001
//==============================================================================================
#ifndef MFormat_BigDecimalFormat_h
#define MFormat_BigDecimalFormat_h

//#include "Format/Format/Format.h"

#include "PDCHeader/PDCwtypes.h"

#include "Base/Base/ReturnString.h"
#include "Synch/Defines/SmartPtr.h"
//#include "Base/Interfaces/BaseException.h"

//#include "Format/Format/BigDecimal.h"
#import "common/DSSBigDecimal.h"
namespace MFormat
{
	class BigDecimalFormat
	{
	public:
		// convert BigDecimal value to a string
		// if ipcFormat is NULL, use the default format
		// throws NumberFormatException if formatting fails
		virtual MBase::ReturnWString BigDecimalToText(const	MDataType::DSSBigDecimal& inNumber, const WCHAR* ipcFormat, LCID inLocale = 0) = 0;

		// convert BigDecimal value to a string
		// inHandle is got from GetTextConversionHandle
		// if inHandle is not found, the default format handle will be used
		// throws NumberFormatException if formatting fails
		virtual MBase::ReturnWString BigDecimalToText(const	MDataType::DSSBigDecimal& inNumber, Int32 inHandle, LCID inLocale = 0) const = 0;

		// covert a string to long value
		// throws NumberFormatException if conversion fails
		virtual MDataType::DSSBigDecimal::ReturnPtr TextToBigDecimal(const WCHAR* ipcText, LCID inLocale = 0) = 0;

		// virtual destructor is needed. cannot be protect.
		virtual ~BigDecimalFormat()
		{
		};
	};

	//////////////
	// Factory Functions
	BigDecimalFormat* CreateBigDecimalFormat();

	/*class BigDecimalFormatException:	//	public MBase::BaseException
	{
	};*/

	//typedef MBase::StrongPtr<BigDecimalFormat, MBase::DeleteOperatorGeneric<BigDecimalFormat> > BigDecimalFormatPtr;
}

#endif // MFormat_BigDecimalFormat_h
