//==============================================================================================
//	FILENAME	:	NumberFormat.h
//	AUTHOR		:	Yuxiao Xiao
//	CREATION	:	10/26/01
//	Modified by Liang Liu at May 27, 2010. 
//	Copyright (C) MicroStrategy, Inc. 2001
//==============================================================================================
#ifndef MFormat_NumberFormat_h
#define MFormat_NumberFormat_h

#include "PDCHeader/PDCwtypes.h"

#include "Base/Base/ReturnString.h"
#include "Synch/Defines/SmartPtr.h"
#include "Base/Interfaces/BaseException.h"

namespace MFormat
{
	typedef enum 
	{
		NONCOLOR = 0x010101,
		BLACK	= 0x0,
		RED		= 0xFF,
		GREEN	= 0x8000,
		YELLOW  = 0xFFFF,
		BLUE	= 0xFF0000,
		MAGENTA = 0xFF00FF,
		CYAN	= 0xFFFF00,
		WHITE	= 0xFFFFFF
	} COLOR;

	class NumberFormat
	{
	public:

		// The following is a description of format strings. 
		// A format string is a string that specifies how a number should be formatted.  
		// It consists of two sub-patterns separated by a semi-colon.  
		// The first sub-pattern is for positive numbers. The second (optional) sub-pattern is used for negative numbers. 
		// Here are the special characters used in the parts of the sub-pattern, with notes on their usage. 	

		// Symbol	Meaning
		// 0	A digit, shows up as a zero if it is zero.
		// #	A digit, suppressed if zero.
		// .	Placeholder for the decimal separator.
		// ,	Placeholder for the grouping separator.
		// E	Separates mantissa and exponent for exponential formats.
		// ;	Separates formats.
		// -	Default negative prefix.
		// %	Multiply by 100 and show as percentage.
		// X	Any other character can be used in the prefix or suffix.
		// '	Used to quote special characters in a prefix or suffix.

		// These are the colors supported in the negative sub-pattern. 
		// The numeric value corresponds to the output value returned by methods on IDSSTextConvertor which return a color value.  These values correspond to RGB values. 
		// VB and possibly other programming environments recognize these values.
		// Color	String	Numeric Value
		// Black	[BLACK]	0x0
		// Red	[RED]	0xFF
		// Green	[GREEN]	0xFF00
		// Yellow	[YELLOW]	0xFFFF
		// Blue	[BLUE]	0xFF0000
		// Magenta	[MAGENTA]	0xFF00FF
		// Cyan	[CYAN]	0xFFFF00
		// White	[WHITE]	0xFFFFFF
		
		// convert double value to a string
		// if ipcFormat is NULL, use the default format
		// throws NumberFormatException if formatting fails
		virtual MBase::ReturnWString DoubleToText(double ifNumber, const WCHAR* ipcFormat, LCID inLocale = 0, Int32* opColor = NULL) = 0;



		// virtual destructor is needed. cannot be protect.
		virtual ~NumberFormat()
		{
		};

		virtual WCHAR* DoubleToText2(double ifNumber, const WCHAR* ipcFormat, LCID inLocale = 0, Int32* opColor = NULL) = 0;	
	};	
}

#endif // MFormat_NumberFormat_h
