//==============================================================================================
//	FILENAME	:	TextObject.h
//	AUTHOR		:	Fangyao Ma
//	CREATION	:	2008-06-06
//	Copyright (C) MicroStrategy Incorporated 2008
//	All Rights Reserved
//==============================================================================================

#ifndef MsiChart_TextObject_h
#define MsiChart_TextObject_h 1

#include "RectangleObject.h"
#include "FormatFont.h"

namespace MsiChart
{
	const Int32 XOffset = 2;	///< Offset used for explode the rectangle area
	const Int32 YOffset = 2;	///< Offset used for explode the rectangle area

	///	Vertical Alignment
	enum EnumVerticalAlign
	{
		VERTICAL_TOP,
		VERTICAL_MIDDLE,
		VERTICAL_BOTTOM,
		VERTICAL_NONE,
	};

	///	Anchor point of 8 angle points and 1 center point of one rectangle area
	enum EnumAnchorPoint
	{
		TOP_LEFT,
		TOP_MIDDLE,
		TOP_RIGHT,
		LEFT_MIDDLE,
		CENTER,
		RIGHT_MIDDLE,
		BOTTOM_LEFT,
		BOTTOM_MIDDLE,
		BOTTOM_RIGHT,
	};
	
	typedef enum EnumTextBorderStyle
	{
		TBS_NONE = 0,
		TBS_SOLID,
		TBS_DASH,
		TBS_DOT,
		TBS_DOUBLE,
	}EnumTextBorderStyle;

	struct TextExtent
	{
		/*TextExtent(const cairo_text_extents_t& iCairoTextExtent)
			: width(iCairoTextExtent.width)
			, x_bearing(iCairoTextExtent.x_bearing)
			, x_advance(iCairoTextExtent.x_advance)
		{}*/
		TextExtent(float iWidth)
		: width(iWidth)		
		, x_bearing(0)
		, x_advance(iWidth)
		{}
		TextExtent()
			: width(0.0)
			, x_bearing(0.0)
			, x_advance(0.0)
		{}

		double width;
		double x_bearing;
		double x_advance;
	};
	
   /** 
	*	This class is used for word wrapping feature. 
	*	It holds a list of text sections.
	*/
	class SubTextHolder
	{
	public:
		friend class TextObject;
		/**
		This function clears the current list of words.
		*/
		void Clear();

		/**	
		This function parses the input text string. 
		Firstly, the text string is divided into text sections according to the separating character, iSeparator. Usually the separating character is a 
		return character, so the input text will be divided into a list of string lines. The function notes down the positions of these return characters
		in mInitVecPartition.
		Then each string line will be further divided into a list of words, and the lists of words will be pushed into mVecStr sequentially.
		@param irText The input text.
		@param iSeparator The separating character.
		*/
		void ParseText(const MBase::String& irText, const wchar_t iSeparator, Int32 iWordLengthLimit = -1);

		/**	
		This function calculates the height and width of each word, and put them down in mVecSize.
		@param iChartContextPtr The chart context.
		@param ipDeviceFont The font info.
		*/
		void UpdateText(ChartContext::Ptr iChartContextPtr,FormatFont* ipDeviceFont);

		/**	
		This function calculates the most appropriate partition of the text according to the input width, using simple greedy search here. It puts the 
		partition info in mVecPartition.
		@param iDestWidth The required width.
		*/
		void PartitionBy(Int32 iDestWidth, bool iIsStrigent = true);

		/**
		This function gets partition info.
		@param orVecIndex The output indexes indicating the partition.
		*/
		void GetPartitionRange(std::vector<Int32>& orVecIndex) const;

		/**
		Get the maximum height and width of the text sections.
		*/
		Int32 GetMaxWidth() const;

		/**
		Get line count.
		@return The line count
		*/
		Int32 GetLineCount() const;

		/**
		Set line count.
		@param iLineCount Specifies the line count.
		*/
		void SetLineCount(Int32 iLineCount);
		/**
		Get the width of one line.
		@param iLineIndex Specifies the line index.
		@return The width of the line
		*/
		double GetOneLineWidth(Int32 iLineIndex) const;

		/**
		Get the x bearing of one line.
		@param iLineIndex Specifies the line index.
		@return The x bearing of the line.
		*/
		double GetOneLineXBearing(Int32 iLineIndex) const;

		/**
		Get the string of one line.
		@param iLineIndex Specifies the line index.
		@return The string text of the line.
		*/
		MBase::ReturnWString GetOneLineString(Int32 iLineIndex) const;

		/**
		Truncate the word string.
		@param irString The word string to truncate.
		@param iWordLengthLimit The word length limit.
		@return The truncated word string.
		*/
		MBase::ReturnWString TruncateWord(const MBase::String& irString, Int32 iWordLengthLimit) const;

        /**
         Return the space width
         */
        Int32 GetSpaceWidth() const;
	private:
		std::vector<MBase::String> mVecStr;		///< Stores the list of words.
		std::vector<Int32>	mInitVecPartition;	///< Stores the position info of return character of the text.
		std::vector<Int32>	mVecPartition;		///< Stores the partition info of the text so that we can generate each text section.
		std::vector<TextExtent> mVecSize;			///< Stores the size info of each word.
		double mSpaceWidth;
		MBase::String mText;
		double mMaxWordWidth;
	};

	/**
	 *	Represnt text string in the graph with multiline feature
	 */
	class TextObject: public ShapeObject
	{
	public:
		typedef MSynch::SmartPtr<TextObject> Ptr;

		/**	
		This constructor is for Title, Subtitle and Footnote.
		*/
		TextObject(const TripleId& irTripleId, 		
						GraphObjectManager* ipManager,											
						Rect2D iRect,
						GraphCollectionObject* ipParentObject = NULL, 
						bool iClientDrag = false);
		
		/**
		This constructor is for those texts with the close bounding rectangle, such as data labels, axis labels, etc.
		*/
		TextObject(const TripleId& irTripleId,
						GraphObjectManager* ipManager,
						GraphCollectionObject* ipParentObject = NULL);
		
		virtual ~TextObject();

		//	virtual functions from GraphObject
		virtual void DrawOutLine();
		virtual bool PointInObject(const Point2D& irPoint);		
		virtual bool ObjectInRectangle(const Rect2D& irRect);
		virtual void Draw() const;
		virtual void GetOptions(Int32 iPropertyId, CComVariant* opValue) const;
		virtual void GenerateImageMap(ImageMap& iorImageMap);

		//	pure virtual function from ShapeObject
		virtual void GetBoundingRect(Rect2D& orBoundingRect) const; 	

		//	Public method
		/**
		This function gets the bounding rectangle of the text before its rotation.
		@param orBoundingRect The output bounding rectangle.
		*/
		void GetBoundingRectNoRotation(Rect2D& orBoundingRect) const;

		/**
		This function sets the text content of the text object.
		@param irText The input text content.
		*/
		void SetText(const MBase::String& irText, Int32 iWordLengthLimit = -1);

		/**
		This function gets the text content of the text object.
		@param orText The output text content.
		@param iIsOriginalText, text could be modified by this class, this parameter indicates whether the original text should be returned. 
		*/
		void GetText(MBase::String& orText, bool iIsOriginalText = false);

		/**
		This function sets the bounding rectangle of the text object. However, if it is not large enough to hold the text content, the text object will adjust
		it automatically.
		@param irRect The input retangle area.
		*/
		void SetRect(const Rect2D& irRect);
		
		void GetRect(Rect2D& orRect) const;

		/**
		This function gets the bounding retangle of the text object.
		@param orRect The output retangle area.
		*/
	//	void GetRect(Rect2D& orRect);

		/**
		This function returns the font info of the text object.
		@return The FormatFont which contains all the font info of the text object.
		*/
		FormatFont::Ptr GetFormatFont();

		/**
		This function moves the text object.
		@param iDestX The x coordinate of the destinate position.
		@param iDestY The y coordinate of the destinate position.
		@param iAnchor Indicates the anchor point which makes it much easier to use.
		*/
		void MoveTo(Int32 iDestX, Int32 iDestY, Int32 iAnchor = TOP_LEFT);	

		/**
		This function provides higher precision than MoveTo() for the case when anchor point is the center of the TextObject.
		@param iDestX The x coordinate of the destinate position.
		@param iDestY The y coordinate of the destinate position.
		*/
		void MoveToCenter(float iDestX, float iDestY);

		/**
		This function sets a width limit, and the text object will do the word wrapping according to the limit.
		@param iWidthLimit The input width limit.
		@param iIsStringent True means the final text width will be exactly equal to iWidthLimit, false means the text width will adjust automatically, but will not exceed iWidthLimit.
		*/
		void SetTextWidthLimit(Int32 iWidthLimit, bool iIsStringent = true);

		/**
		This function sets whether the text object is shown or not.
		*/
		void SetIsShown(bool iIsShown);

		/**
		This function gets whether the text object is shown or not.
		*/
		bool GetIsShown();

		/**
		This function calcultes the device rect of text object.
		*/
		void CalculateDeviceRect() const;

		TextObject::Ptr Clone() const;

		/**
		Get transformed rectangle for overlapping detection with consideration of rotation.
		This transformed rectangle can be used to check whether two TextObjects with the same rotation have overlap or not.
		@param orRect Output the transformed rectangle.
		*/
		void GetTransformedRect(Rect2D& orRect) const;
		/**
		Check whether current TextObject has overlap with another TextObject which has the same rotation.
		@param ipTextObejct Specifies another TextObject.
		@Param iLabelSpan, Span should be resverd beforehand for each two adjacent labels
		@Param iLabelExpandDirection, which direction the span is on  
		@return Return true if current TextObject has overlap with the input TextObject.
		*/
		bool HasOverlapForSameRotation(TextObject* ipTextObject, Int32 iLabelSpan = 0, REC_EXP_DIRECTION iSpanDirection = REC_EXP_INVALID) const;

		/**
		This function gets the line count of the text object.
		*/
		Int32 GetLineCount() const;

		/**
		This function gets the height of just one line.
		*/
		Int32 GetOneLineHeight() const;

		/**
		This fucntion restores the width and height of the text object.
		*/
		void RestoreWidthAndHeight();

		/**
		This function sets the line count of the text object.
		*/
		void SetLineCount(Int32 iLineCount);
		
		/**
		Get the length of mText;
		*/
		Int32 GetStringLength() const;
		
		void SetBorderStyleAndColor(EnumTextBorderStyle iStyle, Int32 iColor, Int32 iDirection);
		
		/**
		 Specifies whether to use larger truncate limit
		 */
		void EnableLargerTruncationLimit(bool iUseLargerTruncationLimit);
	
		bool IsTruncated() const;
		
		bool MaybeTruncated() const;
		
		bool IsTooCloseTo(TextObject* ipTextObject, double iDistance, Int32 iAnchorStyle);
		void SetLineCountEnforced(bool iIsEnforeced = false);
		
		Int32 GetWidthHeightMargin(bool iIsWidth);
		

	protected:	
		virtual void GetClockwisePolygon(std::vector<Point2D>& orPolygon) const;
		
		/**	
		This fucntions handles the underline and strikethrough effect of the text object.
		*/
		void ApplyUnderlineorStrikeThroughFormat() const;

		/**
		This function draws the text frame if any.
		*/
		void hFillTextFrame(const Rect2D& irTextFrameRect) const;

		/**	
		This function formats a single line text or a text section.
		@param irStartPoint The left top point of the text string.
		@param irFontExt The input font extents.
		@param iLineIndex The line index.
		*/
		void hFormatSingleLineText(Point2D& irStartPoint, Int32 iLineIndex) const;

		/**	
		This function gets device polygon for detection and imagemap.
		@param orDevicePolygon The output clockwise polygon of the text object.
		*/
		void hGetDevicePolygon(std::vector<Point2D>& orDevicePolygon) const;

		void hGetDevicePolygon(const Rect2D irRect, std::vector<Point2D>& orDevicePolygon) const;
		
		/**
		This function gets radian from enumeration.
		@return The rotation angle.
		*/
		double hGetRadian() const;	

		/**
		This function truncates the text section if it goes beyond the mDeviceRect range.
		@param iLineIndex The line index.
		@param orStr The output text section.
		@param orTextWidth The width of the output string.
		@param orTextXBearing The x bearing of the output string.
		*/
		void hTruncate(Int32 iLineIndex, MBase::String& orStr, double& orTextWidth, double& orTextXBearing) const;

		/**
		This function calculates the layout parameters mWidthEdgeMargin, mHeightEdgeMargin, and mTextLineSpace according to the font size
		*/
		void hCalculateParas();	

		FormatFont::Ptr		mFormatFontPtr;	///< Stores the font info of the text object.
		MBase::String		mText;			///< The text content.
		MBase::String		mFullText;		///< The untouched content
		
		mutable Rect2D		mRect;			///< The rectangle area enclosing the text.
		mutable SubTextHolder	mSubTextHolder;	///< To handle word wrapping.
		bool mClientDrag;
		bool mIsShown;

		Int32	mWidthEdgeMargin;
		Int32	mHeightEdgeMargin;
		Int32	mTextLineSpace;
		Int32	mDoubleLineMargin;		
		Int32	mFontHeight;
		mutable bool	mIsRectCalculated;
		Int32	mTextWidthLimit;
		bool	mIsStringent;
		bool	mUseLargerTruncationLimit;	///< Default is true. 
		Int32	mLineCount;
		bool	mIsLineCountEnforced;		///< Line count is set and should not be changed.
		mutable bool    mIsTruncated;				///< If it is truncated.
		EnumTextBorderStyle mBorderStyle[4];//left right top bottom
		int mBorderColor[4];//left right top bottom
		


	//	EnumVerticalAlign	mVerticalAlign;	///< Vertical Alignment

	private:		
		virtual void hGetFormatFromParent();

        void hAdjustRect() const;
        
		/**
		This function initialize the FormatFont of the text object according to the triple id.
		*/
		void hInitFormat();
		
	};

	
	inline void TextObject::SetIsShown(bool iIsShown)
	{
		mIsShown = iIsShown;
	}

	inline bool TextObject::GetIsShown()
	{
		return mIsShown;
	}

	inline void TextObject::MoveToCenter(float iDestX, float iDestY)
	{	
		mRect.x = static_cast<Int32>(iDestX - mRect.width / 2.0f + 0.5f);
		mRect.y = static_cast<Int32>(iDestY - mRect.height / 2.0f + 0.5f);		
	}
	
	inline Int32 TextObject::GetStringLength() const
	{
		return (Int32)mText.length();
	}
	
	inline void TextObject::GetRect(Rect2D& orRect) const
	{
		orRect = mRect;
	}
	
	inline void TextObject::SetBorderStyleAndColor(EnumTextBorderStyle iStyle, Int32 iColor, Int32 iDirection)
	{
		_ASSERT(iDirection < 4);
		mBorderStyle[iDirection] = iStyle;
		mBorderColor[iDirection] = iColor;
	}
	
	inline void TextObject::EnableLargerTruncationLimit(bool iUseLargerTruncationLimit)
	{
		mUseLargerTruncationLimit = iUseLargerTruncationLimit;
	}

	inline bool TextObject::IsTruncated() const
	{
		return mIsTruncated;
	}

	inline Int32 TextObject::GetWidthHeightMargin(bool iIsWidth)
	{
		return iIsWidth ? mWidthEdgeMargin : mHeightEdgeMargin;
	}
	
}

#endif
