//
//  GridFormatting.h
//  IPhoneChart
//
//  Created by dong shi on 5/7/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#ifndef MsiFormat_BaseGridFormatting_h
#define MsiFormat_BaseGridFormatting_h

//#import "DatasetHelper.h"
#import "DSSTemplate.h"
#import "BaseFormatManager.h"
#import "DSSCustomGroup.h"
#import	"template/DSSConcreteElementProxy.h"
#import	"template/DSSBandElementProxy.h"
#import	"template/DSSSubExpressionElementProxy.h"
#import	"template/DSSFilterElementProxy.h"
#import "DSSJointConcreteElementProxy.h"

class CComVariant;
class BaseFormatManager;
class DSSFormat;
namespace MsiFormat
{
	class BaseFormatting
	{
	public:
		BaseFormatting();
		BaseFormatting(GUID& iGUID, FORMAT_CATEGORY iCellType, BaseFormatManager* ipManager);
		virtual ~BaseFormatting();
		//virtual FormatInfo::Ptr Resolve(FormatInfo::Ptr ipUpperLayerFormat);		 
		virtual void Resolve(FormatInfo::Ptr& iopUpperLayerFormat);		 

		virtual std::map<GUID, FormatInfo::Ptr>* GetPool();
		virtual std::map<GUID, FormatInfo::Ptr>* GetTitle();
		virtual std::map<GUID, FormatInfo::Ptr>* GetHeader();
		virtual std::map<GUID, FormatInfo::Ptr>* GetDataFormat();
		virtual std::map<GUID, FormatInfo::Ptr>* GetSubtotalHeader(GUID& irID);
		virtual std::map<GUID, FormatInfo::Ptr>* GetSubtotalTitle();
		virtual	std::map<GUID, FormatInfo::Ptr>* GetCornerFormat();

		void Update(GUID& irID, Int32 iCellType);
		static void MergeFormatToFormatInfo(DSSFormat* ipFormat, FormatInfo::Ptr iInfoPtr, bool iIsAtDefaultLayer);
	protected:
        /**
         * To decide if the required format type should be handled at current layer.
         * @return TRUE means the current formatting layer will take care of the input format type.
         */
		virtual bool hIsApplicalbel();

        /**
         * In some layer, we need modify the incoming format from upper layer and also check if we need to go on with the following layers' resolve
         * @param irCurrentLayerFormats, the formats retrieved at current layer.
         * @param iUpperLayerInfoPtr, the incomping format from upper layer. 
         * @return TRUE means we need to go on with the following resolve and FALSE otherwise
         */
		virtual bool hPrepareAndContinue(const std::vector<DSSFormat*>& irCurrentLayerFormats, FormatInfo::Ptr iUpperLayerInfoPtr);
		
		//Update Format info
		/** To get all the DssFormat objects controlling current layer
		*/
		virtual void hGetFormats(GUID& irID, Int32 iCellType, std::vector<DSSFormat*>& orFormat);

		/**
         * Used in Template layer when the RESOLVED format is invalid from server side, we will use this method to do client side formatting ourselves
         * @param irID, the associated GUID for the format to be resolved.
         * @param iCellType, the format type (header, data, etc).
         * @param ipInfo, the format from upper layer.
		*/
		virtual FormatInfo::Ptr hUpdateOneFormat(GUID& irID, Int32 iCellType, FormatInfo::Ptr ipInfo);

		/** Get the key for current layer (for primary cache).
		*/
		virtual void hGetFormatKey(const std::vector<DSSFormat*>& irFomatSet, FormatInfo::Ptr iUpperLayerInfoPtr, std::string& orResultKey);
		void hGetFormatIndexAsText(DSSFormat* ipFormat, std::string& orKey);

		/** Special handling to any properties at each layer should be here.
		*/
		virtual void hSpecialProperty(FormatInfo::Ptr iInfoPtr);	
		
		/** Grab all the properties from the DssFormat set and put them into the format info.
		*/
		virtual void hUpdateOneFormat(DSSFormat* ipFormat, FormatInfo::Ptr iInfoPtr, bool iIsAtDefaultLayer);
		void hUpdateOneFormat(const std::vector<DSSFormat*>& irFomatSet, FormatInfo::Ptr iInfoPtr, bool iIsAtDefaultLayer);
		
		
		//Get Format from different layer
		DSSFormat* hGetFormatFromType(DSSTemplateUnit* ipUnit,  Int32 iFormatterType);
		DSSFormat* hGetFormatFromType(DSSAxis* ipAxis,  Int32 iFormatterType);
		DSSFormat* hGetFormatFromType(DSSTemplate* ipUnit,  Int32 iFormatterType);
		DSSFormat* hGetFormatFromType(DSSMetric* ipMetric,  Int32 iFormatterType); //Just for metric
		DSSFormat* hGetFormatFromType(DSSTemplateMetric* ipTemplateMetric,  Int32 iFormatterType); //Just for templatemetric
		
	protected: //Data Members:
		GUID mGUID;
		FORMAT_CATEGORY mCellFormatCategory;
		BaseFormatManager* mpManager;
		FORMAT_LAYER_INDEX mLayerIndex;
		
		bool mIsUseResolved; //use resolved optimization from server.
		std::map<std::string, FormatInfo::Ptr> mResolvedPool; //pool that hold format resolved from top layer to us.
	};

	class TemplateFormatting: public BaseFormatting
	{
	public:	
		TemplateFormatting(GUID& iGUID, FORMAT_CATEGORY iCellType, BaseFormatManager* ipManager);
		~TemplateFormatting();
		
		std::map<GUID, FormatInfo::Ptr>* GetTitle();
		std::map<GUID, FormatInfo::Ptr>* GetHeader();
		std::map<GUID, FormatInfo::Ptr>* GetDataFormat();
		std::map<GUID, FormatInfo::Ptr>* GetCornerFormat();

	protected:
		virtual bool hIsApplicalbel();
		virtual FormatInfo::Ptr hUpdateOneFormat(GUID& irID, Int32 iCellType, FormatInfo::Ptr ipInfo);
		virtual void hGetFormats(GUID& irID, Int32 iCellType, std::vector<DSSFormat*>& orFormat);
		virtual void hSpecialProperty(FormatInfo::Ptr iInfoPtr);    //usually this will be the last layer. except when we have cell level.
		virtual bool hPrepareAndContinue(const std::vector<DSSFormat*>& irCurrentLayerFormats, FormatInfo::Ptr iUpperLayerInfoPtr);

	protected:
		std::map<GUID, FormatInfo::Ptr> mTitleFormatPool;				//Header in GUI.
		std::map<GUID, FormatInfo::Ptr> mHeaderFormatPool;				//Value in GUI.
		std::map<GUID, FormatInfo::Ptr> mMetricDataFormatPool;			//For each metric.
		std::map<GUID, FormatInfo::Ptr>	mCornerFormatPool;				//Corner Case
	};

	class SubTotalFormatting: public BaseFormatting	
	{
	public:	
		SubTotalFormatting(GUID& iGUID, FORMAT_CATEGORY iCellType, BaseFormatManager* ipManager);
		~SubTotalFormatting();
		//virtual void Resolve(FormatInfo::Ptr& ipUpperLayerFormat);		 
		
		std::map<GUID, FormatInfo::Ptr>* GetSubtotalHeader(GUID& irID);
		std::map<GUID, FormatInfo::Ptr>* GetSubtotalTitle();

		static CComVariant gCountSubTotalNumberCategory;
		static CComVariant gCountSubTotalNumberFormat;
	protected:
		virtual bool hPrepareAndContinue(const std::vector<DSSFormat*>& irCurrentLayerFormats, FormatInfo::Ptr iUpperLayerInfoPtr);
		virtual FormatInfo::Ptr hUpdateOneFormat(GUID& irID, Int32 iColIndex, FormatInfo::Ptr ipInfo);
		virtual bool hIsApplicalbel();
		void hMapToMetricID();
		virtual void hGetFormatKey(const std::vector<DSSFormat*>& irFomatSet, FormatInfo::Ptr iUpperLayerInfoPtr, std::string& orResultKey);
		virtual void hSpecialProperty(FormatInfo::Ptr iInfoPtr);	
		virtual void hGetFormats(GUID& irID, Int32 iCellType, std::vector<DSSFormat*>& orFormat);

		
	protected:	
		std::map<GUID, FormatInfo::Ptr> mSubtotalTitleFormatPool;		//Header in GUI.
		std::map<GUID, std::map<GUID, FormatInfo::Ptr> > mSubtotalHeaderFormatPoolVec;		
		
	private:
		bool mShouldChangeNumberFormat;
		bool mShouldChangeNumberCategory;
	};
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//BandingFormatting				by:Jeremiah
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	typedef enum
	{
		BG_UNDEFINED = 16,
		BG_G1,
		BG_G2,
        BG_G1_HD,  //First group but in header,
        BG_G2_HD  //2nd group but in header 
	}BANDING_GROUP;
	
	
	typedef struct _BandingPropertySet
	{
		bool _enabled;
		bool _includeHeaders;
		bool _byRows;
		Int32 _color1;
		Int32 _color2;
		bool _byHeaders;
		Int32 _group1;
		Int32 _group2;
		string _rowHeader;
		string _colHeader;
		bool _useDefaultColor1;
		bool _useDefaultColor2;
	}BandingPropertySet;
	
	static CComVariant gBandingBgColor1Variant;
	static CComVariant gBandingBgColor2Variant;
	static CComVariant gThemeBandingBgColor1Variant;
	static CComVariant gThemeBandingBgColor2Variant;
	static CComVariant gThemeBandingBgColor3Variant;
	static CComVariant gThemeBandingBgColor4Variant;

    
	class BandingFormatting: public BaseFormatting
	{					
	private:
		BandingPropertySet mBandingProperties;
		
	public:
		BandingFormatting(GUID& iGUID, FORMAT_CATEGORY iCellType, BaseFormatManager* ipManager);
		~BandingFormatting();
		virtual void Resolve(FormatInfo::Ptr& ipUpperLayerFormat);		 
				
	protected:
		bool hParseBandingStr(CComVariant * ipBandingStrVariant);
		Int32 hParseIntAt(string iSource, const char* iTag1, const char* iTag2);
		string hParseStrAt(string iSource, const char* iTag1, const char* iTag2);
		void hLoadGUID();
        CComVariant* hGetBandingResult();
        void hPopulateTheme();
	};
	//////////////////////////	CustomGroup	/////////////////////////////////////////////////
	class CustomGroupFormatting : public BaseFormatting 
	{
	public:
		CustomGroupFormatting(GUID& iGUID, FORMAT_CATEGORY iCellType, BaseFormatManager* ipManager);
		~CustomGroupFormatting();
		//virtual FormatInfo::Ptr Resolve(FormatInfo::Ptr ipUpperLayerFormat);
		static DSSFormat* hGetCustomGroupElementID(DSSBaseElementProxy * ipBaseElementProxy, bool iIsHeader);
	protected:
		FormatInfo::Ptr hFindinPool(const std::string iElementID, DSSFormat* ipFormat, bool iIsHeader);
	private:
		std::map<std::string, FormatInfo::Ptr> mCustomGroupElementPool;
		std::map<std::string, FormatInfo::Ptr> mCustomGroupMetricPool;
	};
		
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//ThresholdFormatting				by:Roy
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	class ThresholdFormatting : public BaseFormatting
	{
	public:
		ThresholdFormatting(GUID& irGUID, FORMAT_CATEGORY iCellType, BaseFormatManager* ipManager);
		~ThresholdFormatting();
		std::map<GUID, FormatInfo::Ptr>* GetDataFormat();
		
	protected:
        virtual void hUpdateOneFormat(DSSFormat* ipFormat, FormatInfo::Ptr iInfoPtr, bool iIsAtDefaultLayer);
		FormatInfo::Ptr hUpdateOneFormat(GUID& irID, Int32 iCellType, FormatInfo::Ptr ipInfo);
		virtual bool hIsApplicalbel();
		virtual void hGetFormats(GUID& irID, Int32 iCellType, std::vector<DSSFormat*>& orFormat);

	protected:
		std::map<GUID, FormatInfo::Ptr> mThresholdPool;
        std::map<std::pair<Int32, Int32>, DSSThreshold*> mPositionToThreasholdMap;
    private:
        bool hGetThresholdPoolInfo(int iMetricIndex, DSSThresholds* &opThresholds);
	};
	
	class ConsolidationFormatting: public BaseFormatting
	{
	public:
		ConsolidationFormatting(GUID& irGUID, FORMAT_CATEGORY iCellType, BaseFormatManager* ipManager);
		~ConsolidationFormatting();
		virtual void Resolve(FormatInfo::Ptr& ipUpperLayerFormat);		 
	protected:
		FormatInfo::Ptr hUpdateOneFormat(GUID& irID, Int32 iCellType, FormatInfo::Ptr ipInfo);
		virtual bool hIsApplicalbel();
		
		void hGetFormatSetFromElements(const std::vector<DSSConsolidationElement*>& irElements, FORMAT_CATEGORY iType, std::vector<DSSFormat*>& iorFormatSet);
		
		void hGetFormatKey(const std::vector<DSSFormat*>& irFomatSet, std::string& orResultKey);
		
	private:
		
		std::map<std::string, FormatInfo::Ptr> mLocalResolvedPool;
		std::map<GUID, FormatInfo::Ptr> mTitleFormatPool;
		std::map<GUID, FormatInfo::Ptr> mGridFormatPool;
	};
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Derived Element			
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	class DerivedElementFormatting: public ConsolidationFormatting
	{
	public:
		DerivedElementFormatting(GUID& irGUID, FORMAT_CATEGORY iCellType, BaseFormatManager* ipManager);
	};
	
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Composite Layer. for CG and Consolidation, stone.			
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	class CGAndConsolidationFormatting: public BaseFormatting
	{//this class would work as a decorator
	public:
		CGAndConsolidationFormatting(GUID& irGUID, FORMAT_CATEGORY iCellType, BaseFormatManager* ipManager);
		~CGAndConsolidationFormatting();
		virtual void Resolve(FormatInfo::Ptr& ipUpperLayerFormat);		 
	protected:
		FormatInfo::Ptr hUpdateOneFormat(const std::vector<FormatInfo::Ptr>& irFormatInfoSet, FormatInfo::Ptr ipInfo);
		void hGetFormatSetFromElements(std::vector<std::pair<DSSConsolidationElement*, Int32> >* ipConsolidationElementSet, 
									   std::vector<std::pair<DSSBaseElementProxy*, Int32> >* ipCGElementSet, std::vector<DSSFormat*>& iorFormatSet);
	private:
		BaseFormatting* mpCGFormatting;
		BaseFormatting* mpConsolidationFormatting;	
		CustomGroupUnitInfo* mpCGInfo;
		bool mHasCG;
		bool mHasConsolidation;
		bool mHasConsolidationOnRow;
		bool mHasConsolidationOnCol;	
		std::map <DSSFormat*, FormatInfo::Ptr> mFormatPool;		
	};
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////	
	//Cell Layer. This is the very bottom layer with lowest priority, stone.			
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	class CellLevelFormatting:public BaseFormatting
	{
	public:	
		CellLevelFormatting(GUID& irGUID, FORMAT_CATEGORY iCellType, BaseFormatManager* ipManager);
		~CellLevelFormatting();	
		virtual void Resolve(FormatInfo::Ptr& ipUpperLayerFormat);		 

	protected:
		bool hIsApplicalbel();
		std::map<GUID, FormatInfo::Ptr>* GetPool();
        
        void hGetFormats(GUID& irID, Int32 iCellType, std::vector<DSSFormat*>& orFormat);
        void hGetCellFormatKey(FormatInfo::Ptr iUpperLayerInfoPtr, std::string& orResultKey);
        
	private:	
		double hGetScaleFactor(FormatInfo::Ptr ipInfo, bool& orHasScaleFactor);

        std::string hToDecimalWString(Int32 iValue) const;

        std::map <GUID, FormatInfo::Ptr> mFormatPool;		
        
		void* mpGridCell;
		
		CComVariant mScaleFactor;
		CComVariant mNumberCategory;
		CComVariant mNumberFormatting;
	};
		

	
}
#endif
