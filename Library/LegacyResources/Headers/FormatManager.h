/*
 *  FormatManager.h
 *  IPhoneChart
 *
 *  Created by dong shi on 3/23/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef MsiChart_FormatManager_h
#define MsiChart_FormatManager_h
#ifndef _VE_CE
#import "DSSTemplate.h"
#import "DSSCellFmtData.h"
#import "Enums.h"
#include "FormatManagerHelper.h"
#else
#include "FormatManagerWINHelper.h"
#endif
#include "DatasetHelper.h"
#include "../Common.h"
#include "FormatInfo.h"
#include <set>

class CComVariant;
namespace MsiChart
{

#ifdef _VE_CE    
	typedef enum _ThemeType{
		THEMETYPE_UNDEFINED = -1,
		THEMETYPE_BLACK = 0,
		THEMETYPE_WHITE
	}ThemeType;
#endif

	typedef enum
	{
		GA_INVALID,
		GA_EXTRACOLUMN,
		GA_ROW,
		GA_COLUMN,
		GA_DATA,
		GA_DATA_NOMETRIC, //with no metric defined on template
		GA_NON_DATA, //ITEM 1-3 + 5.
		GA_NO_OWNER_COL_VALUE,
		GA_NO_OWNER_ROW_VALUE,
		GA_EMPTY_ZONE,
	}GRID_AREA;

	typedef enum
	{
		DssAxisHideAll		= 0x00,
		DssAxisShow			= 0x01,
		DssAxisShowLabels	= 0x02,
		DssAxisShowMajor	= 0x04,
		DssAxisShowMinor	= 0x08,
		DssAxisShowSuper	= 0x10,
		DssAxisShowAll		= 0x1F,
	}EnumAxisShowProperty;
    
    
    typedef enum {
        FRT_NORMAL = -1,
        FRT_GRAPH = 0,
        FRT_GRID = 1,
        FRT_OTHER = 2, //< TO BE ADDED.
    }EnumFormatRenderType;
	
	static const int MaxWebMajorGraphTypes = 25;

	static const int GraphTypeMappingTable[MaxWebMajorGraphTypes][2] =
	{
		DssGraphTypeVertical_Area_Absolute, 7,
		DssGraphTypeHorizontal_Area_Absolute, 7,
		DssGraphTypeVertical_Bar_Side_by_Side, 7,
		DssGraphTypeHorizontal_Bar_Side_by_Side, 7,
		DssGraphTypeVertical_Line_Absolute, 7,
		DssGraphTypeHorizontal_Line_Absolute, 7,
		DssGraphTypePies, 6,
		DssGraphTypeX_Y_Scatter, 4,
		DssGraphTypePolar_Coordinate_Chart, 2,
		DssGraphTypeRadar_Line, 4,
		DssGraphTypeBubble, 2,
		DssGraphTypeHiLo_Stock_Chart, 6,
		DssGraphTypeVertical_Histogram, 2,
		DssGraphTypeD3_Riser_Bars, 4,
		DssGraphTypeD3_Floating_Cubes, 2,
		DssGraphTypeD3_Connect_Group_Areas, 3,
		DssGraphTypeD3_Connect_Series_Areas, 3,
		DssGraphTypeD3_Surface, 3,
		DssGraphTypeD3_X_Y_Z_Scatter_Chart, 2,
		DssGraphTypeGauge,	1,
		DssGraphTypeFunnel,	1,
		DssGraphTypePareto,	2,
		DssGraphTypeBoxPlot, 1,
		DssGraphTypeGantt_Chart, 1,
		DssGraphTypeBarArea, 6
	};

	//struct: Graph Property Key
	struct GraphPropertyKey
	{
		Int32 mPropertyId;
		Int32 mObjectId;
		Int32 mSeriesId;
		Int32 mGroupId;
	
		GraphPropertyKey(Int32 iPropertyId, Int32 iObjectId, Int32 iSeriesId, Int32 iGroupId)
			: mPropertyId(iPropertyId), mObjectId(iObjectId), mSeriesId(iSeriesId), mGroupId(iGroupId)
		{}
		
		GraphPropertyKey(Int32 iPropertyId, TripleId& iTripleId)		
			: mPropertyId(iPropertyId), mObjectId(iTripleId.mObjectId), 
			mSeriesId(iTripleId.mSeriesId), mGroupId(iTripleId.mGroupId)
		{}
					
		bool operator==(GraphPropertyKey& lRight)
		{
			if (this == &lRight)
				return true;
			bool lIsIdentical = (mPropertyId == lRight.mPropertyId) && (mObjectId == lRight.mObjectId) &&
								(mSeriesId == lRight.mSeriesId) /*&& (mGroupId == lRight.mGroupId)*/;	
			return lIsIdentical;					
		}	
		
		bool BelongTo(GraphPropertyKey& lRight) const
		{
			if (mSeriesId == -3 && lRight.mSeriesId != -3)	
				return false;
			return (mPropertyId == lRight.mPropertyId) && (mObjectId == lRight.mObjectId) && (lRight.mSeriesId == -3);
		}
	};
	
	struct KeyCompare
	{
		bool operator()(const GraphPropertyKey& lLeft, const GraphPropertyKey& lRight) const
		{
			if (lLeft.mPropertyId < lRight.mPropertyId)
				return true;
			if (lLeft.mPropertyId > lRight.mPropertyId)	
				return false;
				
								
			if (lLeft.mObjectId < lRight.mObjectId)	
				return true;
			if (lLeft.mObjectId > lRight.mObjectId)	
				return false;

			return (lLeft.mSeriesId < lRight.mSeriesId);
		}
	};

	//class Format Manager:
	class ChartContext;
	class BaseFormatting;
	struct ChartAdvFormat;
	class CHARTENGINE_API FormatManager
	{
	public:
	//Ptr
		typedef FormatManager* Ptr; //to be commented
		//typedef std::vector<std::vector<Int32> > SubTotalMatrix;
		typedef std::map<Int32, Int32> SubTotalMap;
		typedef std::map<GraphPropertyKey, CComVariant, KeyCompare> GraphPropertyKeyMap;
	//Ctor	& Dtor
		FormatManager();
#ifndef _VE_CE
		FormatManager(DSSTemplate* ipTemplate, DSSRWGridIterator* ipIterator);
		//FormatManager(void* ipGridIterator);
#else
		FormatManager(ICDSSTemplate* ipTemplate, ICDSSRWIterator* ipIterator);
		FormatManager(ICDSSRWIterator* ipGridIterator);
#endif
		~FormatManager();	

	//Get Format
		CComVariant* GetFormat(Int32 iPropertyID);								//< this is for property from template & Graph Properties
		CComVariant* GetFormat(Int32 iPropertyID, Int32 iRowIndex, Int32 iColIndex);			//< this is for cell level.(For Grid. caller gets a property)
		FormatInfo::Ptr GetFormat(Int32 iRowIndex, Int32 iColIndex);			//< this is for cell level.(For Grid, caller gets a whole block of properties)
		FormatInfo::Ptr GetFormat(Int32 iRowIndex, Int32 iColIndex, void* ipFormatInfo); //< this is used to populate an out-sourced formatInfo.
#ifndef _VE_CE			
		FormatInfo::Ptr GetFormat(GUID& iGUID, FORMAT_CATEGORY iCellType);		 
		FormatInfo::Ptr GetFormat(void* ipGridCell, bool iIsPassToGridCell = false);
		FormatInfo::Ptr GetMetricFormat(GUID& iGUID,bool iIsHeader, void* ipFormatInfo = NULL);		//< Get Format for one metric Header	
#endif
	//Get FontName	
		MBase::String CheckFontName(MBase::String& irName); //for graph

	//Set Inherit Graph Properties	
		void SetUseGraphProperties(bool iIsUseGraphProperties);

	//Get Graph Property
		bool GetGraphProperty(Int32 iPropertyId, Int32 iObjectId, Int32 iSeriesId, Int32 iGroupId, CComVariant* opValue);
	//Get Column Width, for grid, should only be called when it is in fixed mode	
		Int32 GetColumnWidth(Int32 iColumnIndex);
		Int32 GetRowHeight();
	//Get Stauts
		bool IsValid();
#ifndef _VE_CE
	//Get Template
		DSSTemplate* GetTemplate();
        
    //Get Iterator    
        DSSRWGridIterator* GetIterator();
#endif        
	//Get Dataset : Dataset should only be used to help to map (row col) to some object key. 
		Dataset::Ptr GetDataset();

	//Set Dataset
		void SetDataset(Dataset::Ptr ipDatasetPtr);
#ifndef _VE_CE	

	//Mapping the index passed by CE into the data area
		GRID_AREA MapToDataCellIndex(Int32& orX, Int32& orY, bool iIsPersistent = false); 
		
	//Get Current Index Passed by CE
		void GetCurrentIndex(Int32& orRowIndex, Int32& orColIndex);
		
	//Get template Metrics Info	
		MetricUnitInfo* GetMetricInfo();

	//Get TemplateUnit
		DSSTemplateUnit* GetTemplateUnit(GUID& irID);
	
	//Map CurrentIndex to MetricID, needed by templatelayer for subtotal	
	//	bool MapToMetricIDForSubtotal(GUID& orID);
	
	//Map Row/col to GUID & CellType at one specific layer	
		bool MapToGUIDAndType(GUID& orID, FORMAT_CATEGORY& orCellType, FORMAT_LAYER_INDEX iLayerIndex = FLI_TEMPLATE);
	
	//Map Name to Unit Index: need by Banding layer		
		Int32 MapNameToUnitIndex(std::string iName, bool iIsInRow);		

		bool GetConsolidationElementAndCellType(std::vector<DSSConsolidationElement*>& orElement, FORMAT_CATEGORY& orCellType, FORMAT_LAYER_INDEX iLayerIndex);
#endif
    //Set/Get  Theme type
        void SetThemeType(ThemeType iType);
        ThemeType GetThemeType() const;

    //Get/Set isGrid    
        EnumFormatRenderType GetRenderType() const;
        void SetRenderType(EnumFormatRenderType iType);
#ifndef _VE_CE        
	//Get single GraphProperty
		Int32 GetGraphSubType();
		Int32 GetMaxColumns();
		Int32 GetMaxRows();
		Int32 GetHeight();
		Int32 GetWidth();
		Int32 GetSeriesCount();
		bool GetSeriesByColumn();
		bool GetSubTotalsEnabled();
#endif		
	//Variant to Return		
		static CComVariant sVariant;
#ifndef _VE_CE	
	//Get format info ptr								
		FormatInfo::Ptr GetFormatInfo();	
#endif
		
	//Set Graph Properties (with high priority)
		bool SetMasterGraphProperty(GraphPropertyKey& irKey, CComVariant* ipProperty);

#ifndef _VE_CE
	//CustomGroup
		bool MaptoCustomGroupElementAndCellType(FORMAT_CATEGORY& orCellType, std::vector<DSSBaseElementProxy*>& orElementVector);

	//Get Numberformat information For One Cell	
		bool GetChartAdvFormatForOneCell(Int32 iDataCellRowIndex, Int32 iDataCellColIndex, ChartAdvFormat& orChartAdvFormat, bool iConsiderThresholds);	
	
	//Is NumberFormat Only to retrieve.	
		bool IsNumberFormatExclusive();
		
	//Is Subtotal Of Count Style	
		bool SubtotalIsCount();
#endif	
	//Set graph type.	
		void SetGraphType(short iGraphType);

	//Get Span from Tempalte Edge to the data point
		Int32 DistanceToDataOrigion(bool iIsAlongRow);	
#ifndef _VE_CE		
	//Cell Level Format
		bool GetGUIDForCellLevelData(GUID& iorGUID, const void* ipRawData);
	
	//Get/Set current GridCell
		void* GetCurrentGridCell();
        void SetCurrentGridCell(void* ipCurrentGridCell);
		const void* GetCurrentCellData();
        
    // Has banding or not.    
        bool HasBanding()const;
        
	/*
	 * Convert the coordinate used in the CellView (that is row number column number) to 
	 *         the coordinate used in the Data Mode (that is axis, depth, ordinal) and the cell type
	 * for header
	 * Return true for success convertion, false for failure the the oAxis, oDepth, oOrdinal are meanless, set to -1;
	 */
		bool CoordinateConvertion(int iRow, int iCol, int& oAxis, int& oDepth, int& oOrdinal, FORMAT_CATEGORY& oCellType);
		bool GetCoordinateIntoDataArea(int& iorIndex, bool iIsForRow);	
		
	//Get totoal count of DssFormat
		Int32 GetDssFormatCount();

	//Reset layer stack
        void ResetLayers();

        //Memory Estimation
        /**
         *This method would return the memory usage estimation of formatManager.
         *@return the estimated memory usage.
         */
        Int32 GetMemoryUsageEstimation();
#endif        
    //Get / Set Properties
        bool IsShowRowHeader();
        void SetShowRowHeader(bool iShowRowHeader);
        
        bool IsShowColHeader();
        void SetShowColHeader(bool iShowColHeader);
        
        bool IsColHeaderEmpty();
        bool IsRowHeaderEmpty();
        bool HasDerivedElementOnRow();
		bool HasDerivedElementOnCol();
        bool HasExtraColumn();
        bool HasCustomGroup();
        bool HasMetrics();
        bool HasConsolidationOnRow();
        bool HasConsolidationOnColumn();
        bool HasCellLevelFormat();
        void GetRegularDrillableUnit(std::set<std::pair<Int32, Int32> > &orRegularDrillableUnits);
        void GetLinkedDrillableUnit(std::set<std::pair<Int32, Int32> > &orLinkedDrillableUnits);
        Int32 GetAbsColDataGridOrigin();
        Int32 GetAbsRowDataGridOrigin();
        void GetRowIndexToUnitIndex(std::map<Int32, Int32> &orRowIndexToUnitIndex);
        void GetUnitIndexToRowStartOrdinal(std::map<Int32, Int32> &orUnitIndexToRowStartOrdinal);
        Int32 GetColHeaderStartIndex();
        Int32 GetColSubtotalMatrixSize();
        Int32 GetRowSubtotalMatrixSize();
#ifndef _VE_CE
        void AddFormattingLayer(BaseFormatting *ipFormattingLayer);
     //from private to public for code re-organization
        void GetLinkedDrillableUnitSet();
		void GetRegularDrillableUnitSet();
        MBase::GUID GetMetricIDFromColRowIndex(Int32 iRowIndex, Int32 iColIndex);
	protected:
		//Index -> other Info mapping
		bool hMapIndexToCellTypeAndGUID(Int32 iRowIndex, Int32 iColIndex, FORMAT_CATEGORY& orCellType, GUID& orID);
		bool hMapIndexToCellTypeAndGUIDWithNoMetric(Int32 iRowIndex, Int32 iColIndex, FORMAT_CATEGORY& orCellType, GUID& orID);
		//for banding property
		Int32 hBuildBandingBuckets(Int32 iUnitElementIndex, bool iIsInRow);
		Int32 hBucketToBandingGroup(Int32 iOrdinal);
		Int32 hMapUnitIndexToOrdianlOnAxis(Int32 iUnitIndex, bool iIsInRow);
		bool hMapIndexToBandingGroup(Int32 iRowOrdinal, Int32 iColOrdinal, GUID& iorInfoGUID, FORMAT_CATEGORY& orCellType);
		//bading
		//custom group
		bool hIsBaseElementLegal(DSSBaseElementProxy* ipBaseElementProxy);
		bool hIsInEmptyZone(Int32 iRowIndex, Int32 iColIndex);
		void hGetSubtotalMatrix(bool iIsForRow, SubTotalMap& orSubtotalMatrix);
		
        /**
         *This method would populate several internal maps.
         *@param isForRow, true means the population is done for row-related structures
         */
        void hGetIndexMap(bool iIsForRow);
        
        void hCheckLayerSignals(bool iIsForRow);

        /**
         *For a given template unit, this method returns how many row/columns it takes.
         *@param ipUnit, the unit to be checked.
         *@param iAxisId, the id of the hosting axis.
         *@param iUnitIndex, the unit index of ipUnit into its hosting axis.
         *@return the row/columns ipUnit takes.
         */
		Int32 hGetUnitStep(DSSTemplateUnit* ipUnit, Int32 iAxisId, Int32 iUnitIndex);
        
		MBase::GUID hGetUnitIDFromColRowIndex(Int32 iIndex, bool iIsForRow);
	
		bool hParseSpecialColumn(Int32 iColIndex, FORMAT_CATEGORY& orCellType, GUID& orID);
		bool hParseRowSection(Int32 iRowIndex, Int32 iColIndex, FORMAT_CATEGORY& orCellType, GUID& orID);
		bool hParseColSection(Int32 iRowIndex, Int32 iColIndex, FORMAT_CATEGORY& orCellType, GUID& orID);
		bool hParseDataArea(Int32 iRowIndex, Int32 iColIndex, FORMAT_CATEGORY& orCellType, GUID& orID);	
		bool hParseEmptyZone(Int32 iRowIndex, Int32 iColIndex, FORMAT_CATEGORY& orCellType, GUID& orID);
		
 		bool hGetConsolidationElementFromRow(Int32 iRowIndex, Int32 iColIndex, std::vector<DSSConsolidationElement*>& orElement, FORMAT_CATEGORY& orCellType);
		bool hGetConsolidationElementFromColumn(Int32 iRowIndex, Int32 iColIndex, std::vector<DSSConsolidationElement*>& orElement, FORMAT_CATEGORY& orCellType);
		bool hGetConsolidationElementFromDataArea(Int32 iRowIndex, Int32 iColIndex, std::vector<DSSConsolidationElement*>& orElement, FORMAT_CATEGORY& orCellType);
#endif

		//Set ExtraColumn
		bool hSetExtraCloumnFlag();
		//Calculate Origin
		void hSetDataGridOrigin();		

	protected:
		FormatInfo::Ptr mFormatInfoPtr;	

	private:
		void hDumpTotalNumberFormat();    //this is for [testing].

	private:
#ifdef _VE_CE
		void hInit(ICDSSTemplate* ipTemplate, ICDSSRWIterator* ipIterator); //init formatmanger
#else
		void hInit(DSSTemplate* ipTemplate, DSSRWGridIterator* ipIterator); //init formatmanger
		void hInitLayers(GUID& irID, FORMAT_CATEGORY& irCellType);				  //Init formatting for each layer	
		void hRegisterOneLayer(FORMAT_LAYER_INDEX iLayerIndex, GUID& irID, FORMAT_CATEGORY& irCellType);			  //Push one layer into the layer stack.
#endif
		void hClearLayers();

		void hMapByProperty(Int32& iSeriesId, Int32& iObjectId, Int32& iPropertyId);
		void hConvertFromRGBtoBGR(Int32& inVal);
		void hSetDrillable(void* ipGridCell, FormatInfo::Ptr iFormatPtr);
#ifndef _VE_CE
		bool hHasLinkedDrill(DSSTemplateUnit* ipTemplatUnit);
#endif
		
		Int32 hGetAxisAutoFitProperty(Int32 inObjectId);
		Int32 hGetAxisVisibilityProperty(Int32 inObjectId);
		Int32 hGetAxisZeroProperty(Int32 inObjectId);
		Int32 hGetAxisScaleProperty(Int32 inObjectId);
		Int32 hGetAxisLogProperty(Int32 inObjectId);
		Int32 hGetAxisGridLineProperty(Int32 inObjectId);
		Int32 hGetAxisGridLineProperty2(Int32 inObjectId);
		Int32 hGetAxisScaleEndProperty(Int32 inObjectId);
		Int32 hGetAxisAdvFormatTextProperty(Int32 inObjectId);
		Int32 hGetAxisDirectionProperty(Int32 inObjectId);	
		Int32 hGetAxisCustomLine(Int32 inObjectId);
		Int32 hGetAxisOffScaleProperty(Int32 inObjectId);
		Int32 hGetAxisLabelLocationProperty(Int32 inObjectId);		
		Int32 hGetAxisStaggerProperty(Int32 inObjectId);
		Int32 hGetAxisScaleFreqProperty(Int32 inObjectId);
		
		void hMapWebGraphTypeToAgent(Int32 iMajorType, Int32 iMinorType, Int32 &oType);
		
	private:	
		Dataset* mpDataset;										//global dataset 
		Dataset* mpSlicedDataset;								//sliced one by setting group by unit.
#ifdef _VE_CE
		ICDSSTemplate* mpTemplate;
		ICDSSRWIterator* mpIterator;
		ICDSSGraphStyle* mpGraphStyle;
#else
		DSSTemplate* mpTemplate;
		DSSRWGridIterator* mpIterator;
#endif		

		FormatManagerHelper* mpFormatManagerHelper;
		bool mHasExtraColumn;									//Special handling.
		bool mIsValid;	
		bool mIsRowHeaderEmpty;
		bool mIsColHeaderEmpty;
		bool mHasMetrics;
		bool mIsShowRowHeader;
		bool mIsShowColHeader;
		bool mIsLayersInited;
		bool mIsNumberformatExclusive;                          //< used only in graph axis formatting.
        bool mParseThresholds;                                  //< This is used when parsing graph axis auto format.
        bool mAbsoluteCoordinates;                              //< Whether or not the pass-in row/col index is already absolute coordinates that have taken hidden headers into consideration.
        
		//For Layers initialization
		bool mHasCustomGroup;
		bool mHasConsolidationOnRow;
		bool mHasConsolidationOnColumn;
		bool mHasDerivedElementOnRow;
		bool mHasDerivedElementOnCol;
        
		bool mHasCellLevelFormat;		//Cell Level Format	
		void* mpCurrentGridCell;
		const void* mpCurrentCellData;
        ThemeType mThemeType;
		Int32 mRowHeaderStartIndex;		//along row
		Int32 mColHeaderStartIndex;		//along column
		
		Int32 mRowHeaderWidth;			//span on column
		Int32 mColHeaderHeight;			//span on row

		Int32 mCurrentRowIndex;
		Int32 mCurrentColIndex;
		
		Int32 mRowDataGridOrigin;		//real index in real template of data (0,0)
		Int32 mColDataGridOrigin;

		Int32 mAbsRowDataGridOrigin;	//index of data(0, 0) into complete tempalte.
		Int32 mAbsColDataGridOrigin; 
	
		Int32 mDssFormatTotal;			//Total Number of DssFormat object defined in modal.
	
		std::map<Int32, Int32> mRowIndexToUnitIndex; //Grid Row Header's index --> templateUnitIndex.
		std::map<Int32, Int32> mColIndexToUnitIndex; //Grid Col Header's index --> tempalteUnitIndex.
	
		std::map<Int32, Int32> mRowIndexToRowDepthIndex;	//Grid Row Header's index --> depthIndex, to call lawrence's API 
		std::map<Int32, Int32> mColIndexToColDepthIndex;	//Grid Col Header's index --> depthIndex, to call lawrence's API
	
		std::map<Int32, GUID> mRowUnitIndexToID; //Grid templateUnitIndex  --> templateUnit ID
		std::map<Int32, GUID> mColUnitIndexToID;
		
		std::map<Int32, Int32> mUnitIndexToRowStartOrdinal; //unit index --> start ordinal for this unit
		std::map<Int32, Int32> mUnitIndexToColStartOrdinal;
		
		std::map<Int32, GUID> mOrdinalToMetricID;//Absolute oridianl ==> Metric Id		
#ifndef _VE_CE
		std::map<GUID, DSSTemplateUnit*> mIDToUnit; //GUID -> templateUnit.
#endif

		GraphPropertyKeyMap mMasterGraphPropertyPool;
		
		std::vector<Int32> mBandingBuckets; //to cache the banding range for group1/group2 [a1, a2, a3] ==>[a1, a2) is in G1 [a2 a3) is in G2.
        
        Int32 mUniformElementsSpan; //if all the elements takes euqal span in "by unit" case of banding, this value will be meaningful, otherwise -1.
		
		SubTotalMap mRowSubtotalMatrix;  //RowIndex -> UnitDepth
		SubTotalMap mColSubtotalMatrix;  //ColIndex -> UnitDepth.
		
		std::map<Int32, EnumDSSMetricType> mRowOrdianlToSubtotalType;  //Row data ordinal to subtotal type
		std::map<Int32, EnumDSSMetricType> mColOrdianlToSubtotalType;   //Col data ordinal to subtotal type	
		
		std::set<Int32> mDrillableColumns;
		
		std::vector<BaseFormatting*> mFormattingLayers;
				
		GUID mAssociatedGUID; //< the Key to maps, under currenct subtotal pool.

		MetricUnitInfo* mpInfo;	//< metric information
        std::map<std::string, int> mAxisInfoMap[2];//< unit name to index
#ifndef _VE_CE        
		CustomGroupUnitInfo* mpCustomGroupInfo; //custom group infomation
		
		DSSGraphProperties* mpGraphPropertyPool;  //< grpah properties
		
		FORMAT_LAYER_INDEX mLayerToParse;	//< at which layer we are resolving/merging/caching.	
#endif
		
		Int32 mGraphType;
	
		std::string mKey; //for debugging.
	
        EnumFormatRenderType mRenderType;
        
		bool mIsUseGraphProperties;
		
		GRID_AREA mCurrentCellPosition;
#ifndef _VE_CE		
		std::vector<std::pair<DSSBaseElementProxy*, Int32> > mAssociatedCGElementSetOnCol;
		std::vector<std::pair<DSSBaseElementProxy*, Int32> > mAssociatedCGElementSetOnRow;
		std::vector<std::pair<DSSConsolidationElement*, Int32> > mAssociatedConsolElementSetOnRow; 
		std::vector<std::pair<DSSConsolidationElement*, Int32> > mAssociatedConsolElementSetOnCol;
#endif
		std::set<std::pair<Int32, Int32> > mRegularDrillableUnits;	//store the units that support regular drill
		std::set<std::pair<Int32, Int32> > mLinkedDrillableUnits;	//store the units that support linked drill
		bool mIsLinkedDrillableUnitSetReady;
		bool mIsRegularDrillableUnitSetReady;
	public:
#ifndef _VE_CE
		//cell level formatting.
		bool GetConsolidationStatus(bool iIsOnRow);
		CustomGroupUnitInfo* GetCGInfo();
		GRID_AREA GetCurrentCellPosition();
		std::vector<std::pair<DSSConsolidationElement*, Int32> >* GetAssociateConsolidationElementSet(bool iIsForRow);
		std::vector<std::pair<DSSBaseElementProxy*, Int32> >* GetAssociateCustomGroupElementSet(bool iIsForRow);
#endif		
	};
	
	//Set Graph Type
	inline void FormatManager::SetGraphType(short iGraphType)
	{
		mGraphType = iGraphType;
	}

#ifndef _VE_CE
	//Get Current Raw data  -- cell format
	inline const void* FormatManager::GetCurrentCellData()
	{
		return mpCurrentCellData;
	}
	
	//Get Current cell -- cell format
	inline 	void* FormatManager::GetCurrentGridCell()
	{
		return mpCurrentGridCell;
	}
    inline void FormatManager::SetCurrentGridCell(void* ipCurrentGridCell)
    {
        mpCurrentGridCell = ipCurrentGridCell;
    }
#endif
	//Get Stauts
	inline bool FormatManager::IsValid()
	{
		return mIsValid;
	}
#ifndef _VE_CE	
	//Get Template
	inline DSSTemplate* FormatManager::GetTemplate()
	{
		return mpTemplate;
	}
	
    //Get Iterator
    inline DSSRWGridIterator* FormatManager::GetIterator()
    {
        return mpIterator;
    }
    
	//Get dataset [to delete]
	inline Dataset::Ptr FormatManager::GetDataset()
	{
		return mpDataset;
	}
	
	//Set sliced dataset 	
	inline void FormatManager::SetDataset(Dataset::Ptr ipDatasetPtr)
	{
		mpDataset = ipDatasetPtr;
	}
	
	//Get current input index.
	inline void FormatManager::GetCurrentIndex(Int32& orRowIndex, Int32& orColIndex)
	{
		orRowIndex = mCurrentRowIndex;
		orColIndex = mCurrentColIndex;
	}
	
	//Get metric related info
	inline MetricUnitInfo* FormatManager::GetMetricInfo()
	{
		if (mpInfo && mpInfo->mIsValid)
			return mpInfo;
		else 
			return NULL;
	}

	//Get the total number of DssFormat.
	inline Int32 FormatManager::GetDssFormatCount()
	{
		return mDssFormatTotal;
	}
#endif    
    //Get / Set theme
    inline void FormatManager::SetThemeType(ThemeType iType)
    {
        mThemeType = iType;
    }
    
    inline ThemeType FormatManager::GetThemeType() const
    {
        return mThemeType;
    }
    
    //Is used for grid formatting.
	inline void FormatManager::SetRenderType(EnumFormatRenderType iType)
    {
        mRenderType = iType;
    }
    
    inline EnumFormatRenderType FormatManager::GetRenderType() const
    {
        return mRenderType;
    }

#ifndef _VE_CE
    CHARTENGINE_API FormatManager::Ptr CreateFormatManager(DSSRWGridIterator* ipIterator);
#endif

    inline bool FormatManager::IsShowRowHeader()
    {
        return mIsShowRowHeader;
    }
    inline void FormatManager::SetShowRowHeader(bool iShowRowHeader)
    {
        mIsShowRowHeader = iShowRowHeader;
    }
    inline bool FormatManager::IsShowColHeader()
    {
        return mIsShowColHeader;
    }
    inline void FormatManager::SetShowColHeader(bool iShowColHeader)
    {
        mIsShowColHeader = iShowColHeader;
    }
    inline bool FormatManager::IsColHeaderEmpty()
    {
        return mIsColHeaderEmpty;
    }
    inline bool FormatManager::IsRowHeaderEmpty()
    {
        return mIsRowHeaderEmpty;
    }
    inline bool FormatManager::HasExtraColumn()
    {
        return mHasExtraColumn;
    }
    inline void FormatManager::GetRegularDrillableUnit(std::set<std::pair<Int32, Int32> > &orRegularDrillableUnits)
    {
        orRegularDrillableUnits = mRegularDrillableUnits;
    }
    inline void FormatManager::GetLinkedDrillableUnit(std::set<std::pair<Int32, Int32> > &orLinkedDrillableUnits)
    {
        orLinkedDrillableUnits = mLinkedDrillableUnits;
    }
    inline Int32 FormatManager::GetAbsColDataGridOrigin()
    {
        return mAbsColDataGridOrigin;
    }
    inline Int32 FormatManager::GetAbsRowDataGridOrigin()
    {
        return mAbsRowDataGridOrigin;
    }
    inline bool FormatManager::HasMetrics()
    {
        return mHasMetrics;
    }
    inline void FormatManager::GetRowIndexToUnitIndex(std::map<Int32, Int32> &orRowIndexToUnitIndex)
    {
        orRowIndexToUnitIndex = mRowIndexToUnitIndex;
    }
    inline void FormatManager::GetUnitIndexToRowStartOrdinal(std::map<Int32, Int32> &orUnitIndexToRowStartOrdinal)
    {
        orUnitIndexToRowStartOrdinal = mUnitIndexToRowStartOrdinal;
    }
    inline Int32 FormatManager::GetColHeaderStartIndex()
    {
        return mColHeaderStartIndex;
    }
    
    inline Int32 FormatManager::GetColSubtotalMatrixSize()
    {
        return (Int32)mColSubtotalMatrix.size();
    }
    inline Int32 FormatManager::GetRowSubtotalMatrixSize()
    {
        return (Int32)mRowSubtotalMatrix.size();
    }
    inline bool FormatManager::HasDerivedElementOnRow()
    {
        return mHasDerivedElementOnRow;
    }
    inline bool FormatManager::HasDerivedElementOnCol()
    {
        return mHasDerivedElementOnCol;
    }
    inline bool FormatManager::HasCustomGroup()
    {
        return mHasCustomGroup;
    }
    inline bool FormatManager::HasConsolidationOnRow()
    {
        return mHasConsolidationOnRow;
    }
    inline bool FormatManager::HasConsolidationOnColumn()
    {
        return mHasConsolidationOnColumn;
    }
    inline bool FormatManager::HasCellLevelFormat()
    {
        return mHasCellLevelFormat;
    }
#ifndef _VE_CE
    inline void FormatManager::AddFormattingLayer(BaseFormatting *ipFormattingLayer)
    {
        if (ipFormattingLayer != NULL) {
            mFormattingLayers.push_back(ipFormattingLayer);
        }
    }
#endif
}

#endif

