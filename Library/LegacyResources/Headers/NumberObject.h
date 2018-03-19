//==============================================================================================
//	FILENAME	:	NumberObject.h
//	AUTHOR		:	Liang Liu
//	CREATION	:	2008-09-23
//	Copyright (C) MicroStrategy Incorporated 2008
//	All Rights Reserved
//==============================================================================================

#ifndef MsiChart_NumberObject_h
#define MsiChart_NumberObject_h

#include "TextObject.h"
#include "NumberObjectHelper.h"

namespace MsiChart
{
	/// This enumeration is from ChartCtrl.h
	typedef enum
	{
		Default = -2,
		Fixed = 0,
		Currency,
		Date,
		Time,
		Percentage,
		Fraction,
		Scientific,
		Custom,
		Special,
		General,
		FromGrid,
	} NumberCategory;

	#define CURR_SYMB_SIZE 5
	#define FORMAT_SIZE 256

	/// This struct is from ChartCtrl.h
	struct ChartAdvFormat
	{
		int		formatType;
		int		iDigits;
		int		iGrouping;
		char	szCurrency[CURR_SYMB_SIZE];
		int		iPosCurr;
		int		NegFormat;
		char	szCustomFormat[FORMAT_SIZE];
		
		ChartAdvFormat() : formatType(General),
							iDigits(0),
							iGrouping(3),
							iPosCurr(0),
							NegFormat(3)

		{
			strlcpy(szCurrency, "$", CURR_SYMB_SIZE);
			strlcpy(szCustomFormat, "General", FORMAT_SIZE);
		}
	};
    
    const static char sFixedStringTable[3][8] = {
		{"[SF:-3]"},
		{"[SF:-6]"},
		{"[SF:-9]"}
	};
	//////////////////// end ////////////
	
	/**
	This class deals with the conversion from a data value to a string based on a specified number format.
	*/
	class NumberObject: public TextObject
	{
	public:
		typedef MSynch::SmartPtr<NumberObject> Ptr;

		/**
		Constructor.
		@param irTripleId Required by base class.
		@param ipManager Required by base class.
		@param iNumber Specifies a data value which need to be converted into a string.
		@param irNumberFormat Specifies a number format.
		@param ipParentObject Required by base class.
		@param iLocale Specifies a locale. This is required by MFormat::NumberFormat::DoubleToText().
		@param iClientDrag Required by base class.
		@param iDataSetPtr When format type is FromGrid, Dataset::GetDataFormat() need to be called.
		@param iIsFinal Indicates whether the current string is the final string or not.
        @param iColumnOffset, the index offset to index the data cell
        @param iNumberFormatAsAxis, which axis format should this object derive from.
		*/
		NumberObject(const TripleId& irTripleId, 
						GraphObjectManager* ipManager,						
						double iNumber,	
						const ChartAdvFormat& irNumberFormat,
						GraphCollectionObject* ipParentObject = NULL, 						
						LCID iLocale = 0,
						bool iClientDrag = false,
						Dataset::Ptr iDatasetPtr = NULL,
						bool iIsFinal = true,					 
						Int32 iColumnOffset = 0,
						EnumDssGraphNumberFormatAsAxis iNumberFormatAsAxis = DssGraphNumberFormatAsDefaultAxis);

		virtual ~NumberObject();

		//	Pure virtual function from GraphObject
		virtual void Draw() const;

		/**
		Add another string before current number.
		@param irString The input string.	
		*/
		void AddTextBeforeNumber(const MBase::String& irString);

		/**
		Get the number of current NumberObject.
		@return Return the number of current NumberObject.
		*/
		double GetNumber() const;
        void SetNumber(double iNumber);
		
		void SetThreshHold(bool iIsThreshHold, Int32 iColor = 0);
        
        /**
         If the string representation of input number is formatted to be abbreviated.
         @return TRUE means abbreviation happens.
         */
		bool IsAbbreviated() const;
        
        /**
         Used in Graph Matrix, this method try to get a number out of its fomratted-string representation
         @param orValue, return result.
         @return TRUE means a successful recognition
         */
        bool GetNumberFromText(double& orValue);
        
        /**
         Whether this number is formatted with replaced text
         @return TRUE means a YES.
         */
        bool IsReplacedText() const;
        
        void SetCorrectDateFormatString(ChartAdvFormat& orFormat);
        
    
        Int32 GetColor() const;
        void SetColor(Int32 iColor);
        void SetPrefix(MBase::String iPrefix);
        void SetSuffix(MBase::String iSuffix);
        void GetPrefix(MBase::String& orPrefix);
        void GetSuffix(MBase::String& orSuffix);
        void SetHasBracket(bool iHasBracket);
        Int32 GetColumnOffset() const;
        //void SetTextValue(const MBase::String& irString);
        /**
         *This method set the formatted string to this numberobject
         *@param irString, the formattes string.
         */
        //NumberObjectHelper need to call this method and there is a SetText in TextObject,so I didn't change the prefix --Jin ,dong
		void hSetText(const MBase::String& irString);
        void HandleNegNumberFormat(const char* ipFormattingString, Int32 iNegFormat, LCID iLocale);
        void FilterQuotes(char* ipString, bool iDividPosAndNeg = true);
		bool FilterQuotes(std::string& iorString, bool iDividPosAndNeg = true);
        void MapFormattingString(std::string& iorStr);
        bool IsFraction(const std::string& irNumberFormat) const;
        double AdjustFormatStringWithDot(std::string& formatString, int iDotIndex);
        bool HasBracket(std::string& iorFormatStr);
        double AnalyzeSeperatorAtTail(std::string& iorNumberFormat);
        bool GetPreAndSuffix(std::string& iorFormatStr, std::string& orPrefix, std::string& orSuffix);
    private:
		/**
		Set number format.
		@param irNumberFormat Specifies a number format.
		@param iLocale Specifies a locale. This is required by MFormat::NumberFormat::DoubleToText().
		@param iDatasetPtr When format type is FromGrid, Dataset::GetDataFormat() need to be called.
        @param iNumberFormatAsAxis, which axis number format should this number goes with
		*/
		void SetNumberFormat(const ChartAdvFormat& irNumberFormat, LCID iLocale, Dataset::Ptr iDatasetPtr, EnumDssGraphNumberFormatAsAxis iNumberFormatAsAxis = DssGraphNumberFormatAsDefaultAxis);
        
        /**
         *This method set number format for time series
         @param irNumberFormat Specifies a number format.
         */
		void SetNumberFormatForTimeSeries(const ChartAdvFormat& irNumberFormat);

        /**
         *This method would be used to format the axis labels for time series and graph matrix
         @param irNumberFormat Specifies a number format.
         @param iLocale Specifies a locale. This is required by MFormat::NumberFormat::DoubleToText().
         @param iDatasetPtr When format type is FromGrid, Dataset::GetDataFormat() need to be called.
         @param iNumberFormatAsAxis, which axis format should this object derive from.
         **/
        void hFormatTimeSeriesAxisLabels(const ChartAdvFormat& irNumberFormat, LCID iLocale, Dataset::Ptr iDatasetPtr, EnumDssGraphNumberFormatAsAxis iNumberFormatAsAxis);
        void hFormatGraphMatrixAxisLabels(const ChartAdvFormat& irNumberFormat, LCID iLocale, Dataset::Ptr iDatasetPtr);
        
        /**
         *a helper function to map mTripleId to a needed tripleId for latter parse & formatting, it might change the number format accordingly
         *@param iorNumberFormat, a ChartAdvFormat object
         *@return a tripleId object
         */
        TripleId hMapTripleId(EnumDssGraphNumberFormatAsAxis iNumberFormatAsAxis, ChartAdvFormat& iorNumberFormat);
        
        /**
         *a helper function to map the format type to the one we needed for later parse and formatting.
         *@param irTripleId, a tripleId object
         *@param iorNumberFormat, a ChartAdvFormat object
         */
        void hMapFormatType(const TripleId& irTripleId, ChartAdvFormat& iorNumberFormat);
        
		/**
		@return true means no quoto left, other wise still quoto left.
		*/
		bool hGetPreAndSuffix(std::string& iorFormatStr, std::string& orPrefix, std::string& orSuffix, bool& orHasBrakcket, bool iDividPosAndNeg = true);
		
		void hParseColor(char** oppSource);
		
		std::string hConvertToFraction(double value, std::string& formatString);
	
		void hSetTextFromGrid(Dataset::Ptr iDatasetPtr, MFormat::NumberFormat* ipNumberFormat, LCID iLocale); //Handle the "From Grid" case
		void hSetNumberFormat(Dataset::Ptr iDatasetPtr, const ChartAdvFormat& irNumberFormat,  LCID iLocale);
		void hRemoveMinusFromNegExpComponet(std::string& iorNumberFormat); //used in scientific mode.
		bool replaceQuestionMarkWithHash(std::string& iorNumberFormat);	
        bool hIsCondenseAllowed(Dataset::Ptr iDatasetPtr);
        
        void hParseConditions(std::string& iorFormat);
        void hReplaceCurencySymbol(std::string& iorFormat);
        bool hHandleReplacedText(Dataset::Ptr iDatasetPtr);

		// data members
		double mNumber;	///< A data value which need to be converted into a string.
		Int32 mColor;	///< The color got from number format.
		bool mIsFinal;	///< Indicates whether current number object maintains the final string. In iPad we use this variable for the optimization of hSetText.
		bool mIsThreshHold;
		bool mIsAbbreviated;	//< Abbreviated with time series style.	
		Int32 mColumnOffset;
        bool mIsReplacedText;   //< Replaced Text from Thresholds or other specified text.
        
		bool mHasBracket;
		MBase::String mPrefix;
		MBase::String mSuffix;

		NumberObjectHelper *mpNumberObjectHelper;
	};
    inline Int32 NumberObject::GetColumnOffset() const
    {
        return mColumnOffset;
    }
	inline double NumberObject::GetNumber() const
	{
		return mNumber;
	}
	inline void NumberObject::SetThreshHold(bool iIsThreshHold, Int32 iColor)
	{
		mIsThreshHold = iIsThreshHold;
		if (mIsThreshHold)
			mColor = iColor;
	}
	inline void NumberObject::SetHasBracket(bool iHasBracket)
    {
        mHasBracket = iHasBracket;
    }
	inline bool NumberObject::IsAbbreviated() const
	{
		return mIsAbbreviated;
	}
    inline bool NumberObject::IsReplacedText() const
    {
        return mIsReplacedText;
    }
    inline Int32 NumberObject::GetColor() const
    {
        return mColor;
    }
    inline void NumberObject::SetColor(Int32 iColor)
    {
        mColor = iColor;
    }
    inline void NumberObject::SetPrefix(MBase::String iPrefix)
    {
        mPrefix = iPrefix;
    }
    inline void NumberObject::SetSuffix(MBase::String iSuffix)
    {
        mSuffix = iSuffix;
    }
    inline void NumberObject::GetPrefix(MBase::String& orPrefix)
    {
        orPrefix =  mPrefix;
    }
    inline void NumberObject::GetSuffix(MBase::String& orSuffix)
    {
        orSuffix = mSuffix;
    }
    inline void NumberObject::SetNumber(double iNumber)
    {
        mNumber = iNumber;
    }
}

#endif
