//==============================================================================================
//	FILENAME	:	FormatFont.h
//	AUTHOR		:	Fangyao Ma
//	CREATION	:	2008-09-28
//	Copyright (C) MicroStrategy Incorporated 2008
//	All Rights Reserved
//==============================================================================================
#ifndef MsiChart_FormatFont_h
#define MsiChart_FormatFont_h

#include "PDCHeader/PDCvector"
#include "Base/Base/BaseString.h"
#include "Synch/Defines/SmartPtr.h"
#include "CEModuleMain.h"
#include "RGBColor.h"
#include "Point2D.h"
#ifndef _VE_CE
#include "CoreText/CTFont.h"
#include "CoreText/CoreText.h"
#include "CoreFoundation/CFAttributedString.h"
#endif

class CComVariant;

namespace MsiChart
{
	class FontKey;
	///	Enumeration for TextObject's Box Style
	typedef enum 
	{
		BS_NOFRAME, 
		BS_SINGLE_LINE_FRAME, 
		BS_DOUBLE_LINE_FRAME, 
		BS_EXTRUDE,
		BS_BEVELED_FRAME, 
		BS_REVERSE_BEVELED_FRAME
	}EnumBoxStyle;

	///	Enumeration for Font Alignment
	typedef enum 
	{
		FA_LEFT, 
		FA_CENTER,
		FA_RIGHT, 
	}EnumFontAlignment;

	///	Enumeration for Font Style
	typedef enum 
	{
		FS_NORMAL		=	0,
		FS_BOLD,
		FS_ITALIC,
		FS_BOTH,	
		FS_UNDERLINE	=	0x04,
		FS_STRIKETHROUGH =	0x08,
        FS_FILLSTROKE = 0x0c,
	}EnumFontStyle;

	///	Enumeration for Font Rotation, use the same value as EnumDSSGraphFontRotation
	typedef enum 
	{
		FR_NORMAL = 0,	// Use the same number as EnumDSSGraphFontRotation
		FR_45 = 5,
		FR_90 = 2,
		FR_180 = 3,
		FR_270 = 4,
		FR_315 = 6,
	}EnumFontRotation;

    typedef enum{
        EnumClassMemberFontAlignment = 0,
        EnumClassMemberFontStyle,
        EnumClassMemberFontRotation,
        EnumClassMemberFontBox,
        EnumClassMemberFontColor,
        EnumClassMemberFontAlpha,
        EnumClassMemberFontName,
        EnumClassMemberFontSize
    }EnumFormatFontMembers;
    
	class ChartContext;

	/**
	 *	Font formatting class
	 */
	class FormatFont
	{
	public:
		typedef MSynch::SmartPtr<FormatFont> Ptr;
		/**
		 *	Constructor
		 */
		FormatFont():mFontAlignment(FA_CENTER),
						mFontStyle(FS_NORMAL),
						mFontRotation(FR_NORMAL),
						mFontBox(BS_NOFRAME),
						mFontColor(0,0,0),
                        mStrokeColor(255,255,255),
						mFontAlpha(255),
						mFontName(L"Arial"),
						mFontSize(15),
                        mClassMemberMap(0)
		{
        }
		
		~FormatFont();
	
		/**	
		*   Apply the font format information
		*   @param ipChartContext Pointer to ChartContext.
		*   @param iIsOnlyForExtent Indicate whether this method is called only for getting extent or not.
		*/
		void ApplyFontFormat(ChartContext* ipChartContext, bool iIsOnlyForExtent);

		/**
		 *	Get the font height with the font name
		 *	@param orLineHeight: return the recommended line height of the font
		 */
		void GetFontExtent(Int32& orLineHeight);

		/**
		 *	Get the width height with the font name and specified text
		 *	@param irString: input text string
		 *	@param orWidth: the width of the text string
		 *	@param orHeight: the height of the text string
		 */
		void GetStringExtent(const MBase::String& irString,
							Int32& orWidth,Int32& orHeight);

		/**
		 *	Get text extent with the font name and specified text
		 *	@param irString: input text string
		 *	@param orWidth: the width of the text string
		 *	@param orHeight: the height of the text string
		 */
		void GetStringExtent(const MBase::String& irString, float& orWidth, float& orHeight);

		double GetRadian() const;

		bool GetOptions(Int32 iPropertyId, CComVariant* opValue) const;

		/**
		Check whether current rotation is vertical or not.
		@return Return true if current rotation is vertical. Otherwise, return false.
		*/
		bool IsVertical() const;
		/**
		Check whether current rotation is horizontal or not.
		@return Return true if current rotation is horizontal. Otherwise, return false.
		*/
		bool IsHorizontal() const;
        inline void SetResolvedInDataSet(const bool iFlag ,const EnumFormatFontMembers iMember);
        inline bool IsResolvedInDataSet(const EnumFormatFontMembers iMember) const;
		FormatFont* Clone() const;

#ifndef _VE_CE
		//void DrawAtPosition(CGContextRef iCGContextRef, const char* ipString, float iX, float iY) const;
		
		CTLineRef CreateCTLine(const MBase::String& irString);
		
		//void DrawLine(CTLineRef iStringRef, CTFontRef iFontRef, CGContextRef iContextRef);
		
		static CTFontRef CreateFont(CFStringRef iFamilyName, CGFloat iSize, CTFontSymbolicTraits iTraits, CTFontSymbolicTraits iTraitMasks);
		static void* CreateUIFont(const char* iFontName, CGFloat iSize, bool iIsBold, bool iIsItalic);

		//static CTFontRef CreateFont(CTFontDescriptorRef iFontDescriptor, CGFloat iSize);
#endif
		unsigned char		mFontAlignment;		///< EnumFontAlignment: 3 options
		unsigned char		mFontStyle;			///< EnumFontStyle: 4 options
		unsigned char		mFontRotation;		///< EnumFontRotation: 5 options
		unsigned char		mFontBox;			///< EnumBoxStyle: 5 options
		RGBColor			mFontColor;			///< Font Color
        RGBColor            mStrokeColor;       ///< Text Stroke Color
		unsigned char		mFontAlpha;			///< Font Alpha [0,255]
		MBase::String		mFontName;			///< Font name	
		Int32				mFontSize;			///< Font size
		Int32               mClassMemberMap;
		//void* mFontRef;	
	};

	inline bool FormatFont::IsVertical() const
	{
		return mFontRotation == FR_90 || mFontRotation == FR_270;
	}

	inline bool FormatFont::IsHorizontal() const
	{
		return mFontRotation == FR_NORMAL || mFontRotation == FR_180;
	}
    inline void FormatFont::SetResolvedInDataSet(const bool iFlag ,const EnumFormatFontMembers iMember)
    {
        mClassMemberMap ^= iFlag << iMember;
    }
    inline bool FormatFont::IsResolvedInDataSet(const EnumFormatFontMembers iMember) const
    {
        return (mClassMemberMap & 1 << iMember) != 0;
	}
}
#endif
