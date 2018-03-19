//==============================================================================================
//	FILENAME	:	FormatLine.h
//	AUTHOR		:	Xin Liu
//	CREATION	:	2008-09-26
//	Copyright (C) MicroStrategy Incorporated 2008
//	All Rights Reserved
//==============================================================================================
#ifndef MsiChart_FormatLine_h
#define MsiChart_FormatLine_h

#include "StdAfx.h"
#include "PDCHeader/PDCvector"
#include "Base/Base/BaseString.h"
#include "Synch/Defines/SmartPtr.h"
#include "RGBColor.h"
#include "Point2D.h"
#include <map>

namespace MsiChart
{
	//////////////////////// GraphForamtLine ///////////////////////

	/**
	The line style.
	*/
	typedef enum 
	{
		LS_SOLID, 
		LS_DASH, 
		LS_DOT, 
		LS_DOT_DASH, 
		LS_DASH_DOT_DOT, 
		LS_MEDIUM_DASH, 
		LS_SHORT_DASH, 
		LS_LONGEST_DASH, 
		LS_LONG_DOT, 
		LS_DOT_DOT_DOT, 
		LS_DASH_DASH_DOT, 
		LS_DASH_DASH_DOT_DOT, 
		LS_LONG_DASH_DOT, 
		LS_LONG_DASH_DOT_DOT, 
		LS_LONG_DASH_DASH, 
		LS_LONG_DASH_DASH_DASH
	}EnumLineStyle;

    typedef enum{
        EnumClassMemberLineStyle = 0,
        EnumClassMemberLineAlpha,
        EnumClassMemberLineThickness,
        EnumClassMemberLineThicknessPt,
        EnumClassMemberLineColor
    }EnumFormatLineMembers;
    
	class ChartContext;

	/**
	This class manages all the line format info of a GraphObject.
	*/
	class FormatLine
	{
	public:
		typedef MSynch::SmartPtr<FormatLine> Ptr;
		FormatLine();

		/**
		Constructor.
		@param iLineStyle Specifies the line style.
		@param iTransparency Specifies the transparency of the line.
		@param iLineThickness Specifies the line thickness or line width.
		@param iLineColor Specifies the line color.
		*/
		FormatLine(unsigned char iLineStyle, unsigned char iTransparency, 
			Int32 iLineThickness, const RGBColor& iLineColor);	

		/**
		This function gets the line property value according to the input property id.
		@param iPropertyId The input property id.
		@param opValue Stores the output value of the specified property.
		@return True means the output value is valid while false means the opposite.
		*/
		bool GetOptions(Int32 iPropertyId, CComVariant* opValue) const;

		/**
		This function calls the cairo API to set all the line properties.
		*/
		void ApplyLineFormat(ChartContext* ipChartContext) const;

		FormatLine::Ptr Clone() const;
        inline void SetResolvedInDataSet(const bool iFlag ,const EnumFormatLineMembers iMember);
        inline bool IsResolvedInDataSet(const EnumFormatLineMembers iMember) const;
	public:
		unsigned char	mLineStyle;		///< The line style.
		unsigned char	mAlpha;	///< The transparency of the line.
		Int32			mLineThickness;	///< The line thickness or line width in pixel.
		Int32			mLineThicknessPt;	/// The line thickness in Pt multiplied by 100.
		RGBColor		mLineColor;		///< The line color
        Int32           mClassMemberMap;

	};
    void FormatLine::SetResolvedInDataSet(const bool iFlag ,const EnumFormatLineMembers iMember)
    {
        mClassMemberMap ^= iFlag << iMember;
    }
    bool FormatLine::IsResolvedInDataSet(const EnumFormatLineMembers iMember) const
    {
        return (mClassMemberMap & 1 << iMember) != 0;
    }
	extern const Int32 gHoverStrokeThickness;

	extern const RGBColor gBlack;
	extern const RGBColor gWhite;	

	extern const FormatLine gOutlineWhite;
	extern const FormatLine gOutlineWhite2;
	extern const FormatLine gOutlineBlack;
	extern const FormatLine gLeaderLine;
	extern const FormatLine gStrokeForDarkBackground;
	extern const FormatLine gStrokeForLightBackground;
    extern const FormatLine gBudgetingHoverOutLine;

	extern const FormatLine gOutlineHoverWhite;
    extern const FormatLine gInnerOutlineHoverBlack;
    extern const FormatLine gSubtleStrokeForOverlaidInnerPies;
}
#endif
