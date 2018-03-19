/*
 *  FormatInfo.h
 *  IPhoneChart
 *
 *  Created by dong shi on 5/19/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MSIFORMAT_FORMATINFO_H
#define	MSIFORMAT_FORMATINFO_H
#include "Base/Base/BaseString.h"
#include "map"
#include "vector"
#include "PropertyPackage.h"
class CComVariant;

namespace MsiFormat
{
	const Int32 gKeyBandingIndexOffset = 3;
	const Int32 gKeySubtotalIndexOffset = 2;
	const Int32 gKeySpecialSection = 3; //including closing '\0' 
	
	typedef enum
	{
		FONT_NORMAL = 0,
		FONT_BOLD = 1,
		FONT_ITALIC = 2,
		FONT_BOLDITALIC =3,
	}FONTSTYLE;
	
    typedef enum
    {
        EXTR_BEGIN = -1,
        EXTR_SCALEDFACTOR = 0,
        EXTR_SCALEDNUMBERFORMAT = 1,
        EXTR_SCALEDNUMBERCATEGORY = 2,
        EXTR_END,
    }EXTRA_FMT_PROPERTIES;
    
    typedef enum
    {
        EXTR_IDX_SCALEDFACTOR = 0x01,
        EXTR_IDX_SCALEDNUMBERFORMAT = 0x02,
        EXTR_IDX_SCALEDNUMBERCATEGORY = 0x04,
    }EXTRA_IDX_PROPERTIES;
    
	//class: Format Info
	//typedef std::map<_GridProperty, CComVariant*> GridPropertyMap;
	typedef std::vector<CComVariant*> GridPropertyMap;
	typedef std::map<MBase::String, std::vector<MBase::String> > FontBuckets;  //FontFamily --> FontStyle
	typedef std::map<MBase::String, std::vector<std::string> > FontBuckets_N; //N for narrow character.
	
	class FormatInfo
	{
	public:
		typedef FormatInfo* Ptr;
		
		FormatInfo();
		~FormatInfo();
		
		static void Initialize();
		
		void Clear();
		void Copy(FormatInfo::Ptr iInfo);
		void Merge(FormatInfo::Ptr iInfo);
		void Merge(FormatInfo::Ptr iMajor, FormatInfo::Ptr iMinor);
		
		bool GetBoolValue(Int32 iPropertyId);
		bool GetBoolValue(_GridProperty iPropertyId);
		Int32 GetIntValue(Int32 iPropertyId);
		Int32 GetIntValue(_GridProperty iPropertyId);
		double GetDoubleValue(Int32 iPropertyId);
		double GetDoubleValue(_GridProperty iPropertyId);
		MBase::String GetStringValue(Int32 iPropertyId);
		MBase::String GetStringValue(_GridProperty iPropertyId);
        
		std::string GetCStringValue(Int32 iPropertyId);
		std::string GetCStringValue(_GridProperty iPropertyId);
        
        bool GetCStringValue(std::string&  orValue, _GridProperty iPropertyId); //this one is customized for grid rendering, it only supports two properties :_numberFormat _currencySymbol
        void Update();
        
		bool Find(_GridProperty iPropertyId, bool iIncludeAllMaps = false);
		
        void DebugOneProperty(Int32 iPropertyId);
        
        
		//getter
		CComVariant* GetProperty(Int32 iPropertyId);
		CComVariant* GetProperty(_GridProperty iPropertyId);

		
		void GetFontName(std::string& orResult);
		void GetFontName(MBase::String& iFontFamily, int iStyle, std::string& orResult);

		MBase::String GetFontName();
		MBase::String GetFontName(MBase::String& iFontFamily, FONTSTYLE iStyle);
		
		bool IsUseDefault(Int32 iPropertyId);
		bool IsUseDefault(_GridProperty iPropertyId);
		
		//setter
		void SetProperty(Int32 iPropertyId, CComVariant* ipAddress);
		void SetProperty(_GridProperty iPropertyId, CComVariant* ipAddress);
		
		//Special handling for drillable
		void SetDrillable(bool iIsDrillable);
		bool GetDrillable();

		//handle key
		void GetKey(std::string& orKey);
		void SetKey(const std::string& irKey);
		void InitKey(Int32 iMaxIndex);
		
        //for easy debug
        void PrintOneProperty(Int32 iPropertyId);
        void PrintOneProperty(_GridProperty iPropertyId);
        
        void UpdateExtraProperties(const std::vector<CComVariant>& irExtraProperties);	
        void UpdateScaledProperties(int iPropertyUnion = 0x07);
        
        bool HasParsedCellFormat() const;
        void SetParsedCellFormat(bool iHasParsedCellFmt);
        
        //memory estimation
        Int32 GetMemoryUsageEstimation();
        
    public://dark theme support.
        bool mHasThreshold; //used in grid, if the corresponding grid has a threshold defined.
	//peformance
        bool mFontBold;
        bool mFontItalic;
        
    public://static members
		static std::map<MBase::String, Int32> msFontFamily;
		static std::map<CComVariant*, bool> msDefaultAddressPool;
		static FontBuckets msFontBuckets;
		static FontBuckets_N msFontBuckets_N;
		static PropertyPackage mPropertyPackage;
		static CComVariant msDefaultFontFamily;	
        
	private://private methods
		void hInit();
		CComVariant* hGetSupportFontFamily();
		double hConvertBSTRToDouble(CComVariant* ipVariant, int iType);	
        void hDumpVariant(CComVariant* ipVariant);
        
	private://private data members	
		GridPropertyMap mPropertyMap;
		bool mIsDrillable;
		bool mHasParsedCellFormat; //has cell format and it is already parsed.
		//key stuff.
		std::string mKey;  //impl1. use string fully
		Int32 mKeyLen;
        
        //for performance.
        std::string mCurrencySymbol;
        std::string mNumberFormatString;
        bool mHasCurrencySymbolDefined;
        bool mHasNumberFormatStringDefined;
        
    private:
        std::vector<CComVariant*> mExtraProperties; //For scaled factor related.
	};
	typedef std::map<GUID, FormatInfo::Ptr>* FormatPoolPtr;
	typedef std::map<GUID, FormatInfo::Ptr>  FormatPool;
	
	inline bool FormatInfo::GetDrillable() {return mIsDrillable;}
	inline void FormatInfo::SetDrillable(bool iIsDrillable = true)	{mIsDrillable = iIsDrillable;}
	
	inline void FormatInfo::GetKey(std::string& orKey){orKey = mKey;}
	inline void FormatInfo::SetKey(const std::string& irKey){mKey = irKey;}
    
    inline bool FormatInfo::HasParsedCellFormat() const { return mHasParsedCellFormat;}
    inline void FormatInfo::SetParsedCellFormat(bool iHasParsedCellFmt) {mHasParsedCellFormat = iHasParsedCellFmt;}

}

#endif