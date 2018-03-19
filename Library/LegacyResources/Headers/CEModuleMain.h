//==============================================================================================
//	FILENAME	:	CEModuleMain.h
//	AUTHOR		:	Fangyao Ma
//	CREATION	:	2008-05-15
//	Copyright (C) MicroStrategy Incorporated 2008
//	All Rights Reserved
//==============================================================================================
#ifndef MsiChart_CEModuleMain_h
#define MsiChart_CEModuleMain_h 

#include "Base/Base/BaseString.h"
#include "Point2D.h"
#ifndef _VE_CE
#include "Format/Format/PrivateSource/NumberFormat/NumberFormatImpl.h"
#include "CoreText/CoreText.h"
#else
#include "VisualizationEngine/ChartEngine/Chart/ChartEngine.h"
#include "Format/Format/NumberFormat.h"
#include <map>
#include "VisualizationEngine/VECore/VECommon/VEDefine.h"
#endif
#include <set> 
//#include "UIKit/UIFont.h"
//#include "Foundation/NSNumberFormatter.h"

//class NSNumberFormatter;
//class UIFont;

namespace MsiChart
{	
	typedef enum 
	{	
		WT_NONSUPPORTED_GRAPH_TYPE = 1,
		WT_INSUFFICIENT_DATA_TO_PLOT		
	}EnumWarningType;
    
    typedef enum
    {
        WT_GRAPH = 0,
        WT_GRAPH_MATRIX,
        //ADD for other widget.
        //If we add widget type here, we need to update mWidgetTypeNumber
        //we also need to modify the method GetWidgetType() in chartcontext!
        //we use the mWidgetTypeNumber to init the mpNumberFormat
        
    }EnumWidgetType;
	
	typedef enum 
	{	
        /*
         *IF you wish to change anything here, make sure you insert to the right category, or contact Stone if you are unsure.
         */
        ELST_UNDEFINED = 0,                 //< reserved
        ELST_WARN_GRAPH_TYPE,               //< warning starts
        ELST_WARN_DATA_INSUFFICIENCY,       
        ELST_GENERAL_SERIES,                //< general starts
        ELST_GENERAL_CATEGORY,
        ELST_GENERAL_VALUE,
        ELST_GENERAL_METRICS,
        ELST_STOCK_HIGH,                    //< stock starts
        ELST_STOCK_LOW,
        ELST_STOCK_OPEN,
        ELST_STOCK_CLOSE,
        ELST_HISTOGRAM_START,               //< histograme starts
        ELST_HISTOGRAM_END,
        ELST_HISTOGRAM_RANGE,
        ELST_HISTOGRAM_COUNT,
        ELST_BUDGET_ACTUAL,                //< hichart starts.
        ELST_BUDGET_TARGET,
        ELST_BUDGET_DIFFERENCE,
        ELST_BUDGET_ADDITIONAL,
	}EnumLocalizedStringType;
	
	/**
	 *	Module's initialization entry
	 */
	class CEModuleMain
	{
	public:
		CEModuleMain();
		~CEModuleMain();

		/**
		Convert angle degree to radian
		@param iDegree angle's value in degree
        @return angle's value in radian
		*/
		float DegreeToRadian(Int32 iDegree) const;
		
		/**
		Convert angle's value from radian to degree(0 - 360)
		@param iRadian angle's value in radian
        @return angle's value in degree(0 - 360)
		*/
		Int32 RadianToDegree(double iRadian) const;

		/**
		whether line AB and CD are intersecting
		in: A,B,C,D points
		out: oIntersection indicates the intersection point if existed the only point, that is the reture value is 1.
		@return -1 if they are the same line, 0 if parallel
		1 if segment intersection, 2 if line intersection
		reference: http://geometryalgorithms.com/Archive/algorithm_0104/algorithm_01048.htm#Line%20Intersections
		*/
		Int32 LineIntersection(const FPoint2D& irA, const FPoint2D& irB, const FPoint2D& irC, const FPoint2D& irD, FPoint2D& orIntersection, bool iExclude = false) const;
		
		/**
		Given the depth and the angle, get the point in the angular bisector.
		In this case, for the angle ABC, we get a point in its angular bisector, which has 
		an distance "irDepth" from both the edge BA and BC.
		@param irA The input point A.
		@param irB The input point B.
		@param irC The input point C.
		@param irDepth The input distance.
		@param orBisectorPoint The output point.
		*/
		void GetAngularBisectorIntersectionPoint(const Point2D& irA, const Point2D& irB, const Point2D& irC, float irDepth, Point2D& orBisectorPoint) const;
		void GetAngularBisectorIntersectionPoint(const Point2D& irA, const Point2D& irB, const Point2D& irC, float irDepth, FPoint2D& orBisectorPoint) const;

		/**
		Get the bounding polygon of a given poly line.
		@param irPolyLine Specifies a poly line.
		@param iLineWidth Specifies a line width.
		@param orPolygon Output the bounding polygon.
		*/
		void GetPolygonFromPolyLine(std::vector<Point2D>& irPolyLine, float iLineWidth, std::vector<Point2D>& orPolygon) const;	
		void GetPolygonFromPolyLine(std::vector<Point2D>& irPolyLine, float iLineWidth, std::vector<FPoint2D>& orPolygon) const;	
		
		/* Image the polygon as a circle, shift the first iShiftSize points to the end.
         */
        void ShiftPolygonBy(std::vector<Point2D>& orPolygon, Int32 iShiftSize) const;
        
		Int32 GetColorInGradientRange(double iIndex, Int32 iStartColor, Int32 iEndColor);
		void MakePolygonClockwise(std::vector<Point2D>& iorPolygon) const;

		bool IsIPad();
		bool IsSystemVersionAtOrNewerThan(double iVersionNumber);
		bool IsFontSupported(MBase::String& irFontFamilyName);
		
		/// Check whether the current device is a recent device such as iPhone4 and iPad2.
		bool IsRecentDevice() const;
		
		void* GetNumberFormatter();
		
		MFormat::NumberFormat* GetNumberFormat(EnumWidgetType iWidgetType);
        
        void UpdateNumberFormat(EnumWidgetType iWidgetType = WT_GRAPH_MATRIX);
        bool IsNeedUpdate();
        void SetIsNeedUpdate(const bool iIsNeedUpdate);

		Int32 DecimalToInt(const wchar_t* ipStr) const;
		Int32 DecimalToInt(const char* ipStr) const;
		MBase::String ToDecimalWString(Int32 iValue) const;
		MBase::String ToWString(const std::string& iStr);	
		void DoubleToString(double iValue, MBase::String& orString) const;	
#ifndef _VE_CE
		CTFontRef GetFont(MBase::String& irFontFamily, Int32 iFontSize, bool iIsBold, bool iIsItalic);		
		void* GetUIFont(const char* ipFontName, Int32 iFontSize, bool iIsBold, bool iIsItalic);
#endif
		
		MBase::ReturnWString GetLocalWarningString(int iWarningType) const;
		MBase::ReturnString GetLocalizedWarningString(int iWarningType) const; //overloaded version for easy use.
		MBase::ReturnString GetLocalizedString(int iStringType) const;
		
		MBase::ReturnString GetLocalizedSeries() const;
		MBase::ReturnString GetLocalizedCategory() const;
		MBase::ReturnString GetLocalizedValue() const;
		
        MBase::ReturnString GetConcatenatedStringFromCollection(const std::vector<std::string>& irCollection, const std::string& irSeperator ) const;
        
		//Get localized names for the "High", "Low", "Open", and "Close" for HiLo_Stock charts
		MBase::ReturnString GetLocalizedStockNames(int i) const;
		
		//Get localized names for the "Range Start", "Range End", "Bucket Range", and "Bucket Count" for Histogram charts
		MBase::ReturnString GetLocalizedHistogramNames(int i) const;

        void ReplaceText(std::string& orString, const std::string& irOld, const std::string& irNew);
        
		void InitializeDefaultFont();
#ifndef _VE_CE
		const CTFontRef& GetDefaultFont();
#endif

        Int32 AdjustColorValueByBrightness(Int32 iOriginalValue, double iAddupBrightness);

#ifdef _VE_CE
		CHARTENGINE_API void SetTextMeasureFunc(GetTextSize getTextSizeFunc);
#endif
        
		float GetMemoryUsageEstimation();
		
		const float mTolerance;
		const float mFloatPi;
		const Int32 mDefaultGraphWidth;
		const Int32 mDefaultGraphHeight;
		const Int32 mRiserCountLimit;
		const Int32 mGlowWidth;
		static const Int32 mRadialSize = 256;
		std::map<std::string, Int32> mFontNameMap;
#ifndef _VE_CE
		std::map<int, CTFontRef> mFontMap;
#endif
		std::map<int, void*> mUIFontMap;
		 Int32 mMemoryEstimatedCount;
		float mRadialAlpha[mRadialSize+1][mRadialSize+1];
		
#ifndef _VE_CE
		pthread_mutex_t mMutex;
#endif
#ifdef _VE_CE
		GetTextSize mGetTextSizeFunc;
#endif
	private:
		// the global number formatter
		void* mpNumberFormatter;
        
        // whether need update the formatter
        bool mIsNeedUpdate;
#ifndef _VE_CE		
		MFormat::NumberFormatImpl** mpNumberFormat;
#else
		MFormat::NumberFormat** mpNumberFormat;
#endif
        short mWidgetTypeNumber;
#ifndef _VE_CE
		CTFontRef mDefaultFontRef;
#endif

		bool mIsDefaultFontInitialized;
		
		std::set<MBase::String> mSupportedFontFamily;
	};

#ifdef _VE_CE
	CHARTENGINE_API extern CEModuleMain gModuleMain;
#else
    extern CEModuleMain gModuleMain;
#endif
	inline float CEModuleMain::DegreeToRadian(Int32 iDegree) const
	{
		return iDegree * mFloatPi / 180;
	}

	inline Int32 CEModuleMain::RadianToDegree(double iRadian) const
	{
		return static_cast<Int32>(iRadian * 180 / mFloatPi + (iRadian >= 0 ? 0.5 : -0.5));
	}
	
	inline void* CEModuleMain::GetNumberFormatter()
	{
		return mpNumberFormatter;
	}
    
    inline bool CEModuleMain::IsNeedUpdate()
    {
        return mIsNeedUpdate;
    }
    
    

}
#endif
