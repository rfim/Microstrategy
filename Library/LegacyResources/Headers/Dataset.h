//==============================================================================================
//	FILENAME	:	Dataset.h
//	AUTHOR		:	Dong Shi & Fangyao Ma
//	CREATION	:	2008-05-15
//	Copyright (C) MicroStrategy Incorporated 2008
//	All Rights Reserved
//==============================================================================================
#ifndef MsiChart_Dataset_h
#define MsiChart_Dataset_h

#include "Base/Base/BaseString.h" 
#include "Base/Defines/BasicTypes.h"
#include "Synch/Defines/SmartPtr.h"
#include "PDCHeader/PDCvector"
#include "PDCHeader/PDCset"
#include "PDCHeader/PDCmap"
#include "PDCHeader/PDCfstream"
#ifdef _VE_CE
#include "VisualizationEngine/ChartEngine/Chart/ChartEngine.h"
#include "VisualizationEngine/VECore/VEWidgets/GraphMatrix/DataModel/GMCEDataProvider.h"
#else
#define CHARTENGINE_API
#endif

/*
#ifndef  _Msi_Variant
#define _Msi_Variant
#endif
*/
#ifndef _VE_CE
// AE Integration
#import "DSSRWGridIterator.h"
#import "DSSTemplate.h"

class DSSRWGridIterator;
class DSSTemplateUnit;
class DSSTemplate;
#endif

//extern const MBase::String  gAttrFormsSeperator;

namespace MsiChart
{
#ifndef _VE_CE
	typedef  unsigned char DSSDataFlag;		
#endif
	class DimTree; // forward declaration
	struct ChartAdvFormat;
	class DataPiece;
	class TokenManager;
	struct MetricUnitInfo;
	
	enum EnumDataMode	// 3 access mode
	{
		DM_GENERIC = 1, //< Generic data.
		DM_TEMPLATE,	//< Data from template, with duplication.
		DM_AE_ADAPTER,	//< Wrapper of AE data table.
		DM_AE_ADAPTER_REFORMED //< Reformed Table.
	};

	typedef enum
	{	
		AGG_TYPE_AVG,
		AGG_TYPE_SUM,
		AGG_TYPE_MAX,
		AGG_TYPE_MIN,
		AGG_TYPE_CNT
	}AGG_TYPE;
	
    typedef enum{
        DRT_NORMAL = 0,
        DRT_PERCENT,         //ordinary value, but in percentage.   
        DRT_PERCENT_STACKED, //stacked value, in percentage.
        DRT_STACKED,         //stacked value
    }DATA_RETRIEVAL_TYPE;
	
	typedef enum
	{
		SLICE_STEPWISE = 0,		 //Closely overlaped, used in collection.
		SLICE_GROUPADJACENT = 1, //< adjust to others, used in collection
		SLICE_INDEPENDENT = 2,   //< With no overlap with others, used in collection
		SLICE_STRIPPED =3,		 //< DE-ADJACENT one
		SLICE_SPECIFY = 4,		 //< USER SET THE WINDOW SIZE AND POSITION
		SLICE_MIXED_GROUP = 5,	 //< Group is continous but not the series
		SLICE_MIXED_SERIES = 6,  //< Series is continous but not the group
	}SLICE_TYPE; 

    typedef enum
    {
        SHUFFLE_NONE = 0,
        SHUFFLE_ROWONLY = 0x01,
        SHUFFLE_COLONLY = 0x10,
        SHUFFLE_BOTH = 0x11,
    }SHUFFLE_STATUS;
    
    
	typedef enum
	{
		DTST_INVALID,
		DTST_OKAY,
		DTST_DISABLED
	}DATASET_STATUS;


	typedef enum
	{
	  THRESHOLD_FILL_INVALID = -1,
	  THRESHOLD_FILL_SOLID = 0,
	  THRESHOLD_FILL_PATTERN = 1,
	  THRESHOLD_FILL_GRADIENT = 2,
	  THRESHOLD_FILL_TRANSPARENT = 3,
	}THRESHOLD_FILL_STYLE;
 
	typedef enum
	{
		LLO_NORMAL = 0,		//normal case, use old chart engine logic
		LLO_COMMON = 1,		//Label piece from root node to the last but one.
		LLO_SPECIFIC = 2,	//Bottom level piece.
        LLO_LINK = 3,       //To Get the link part (always the lowest part), if it is indeed a linkable label 
	}LABEL_LEVEL_OPTION;
 
    /**
     Index For total
     */
    typedef enum
    {
        GMDI_X,
        GMDI_Y,
        GMDI_SIZE_BY,
        GMDI_SLICE_Total,
        GMDI_ADDITIONAL,
        GMDI_VALID_METRIC
    }EnumGraphMatrixDataIndexType;
    
    typedef enum
    {
        GMPPT_GRID,
        GMPPT_CATEGORY,
        GMPPT_VALUE,
    }EnumGraphMatrixPiePlotType;
 
    struct DataGroupInfo
    {
        Int32 mDataPartEndIndex;
        Int32 mDataAdditionStartIndex;
        Int32 mDataAngleSetStartIndex;
        Int32 mTotalSize;
        
        DataGroupInfo();
    };
    
	struct DataSetReformInfo	//< Used for iPad Graph Matrix in Dataset Reform.
	{
        bool mIsMetricGrouped;  //< If the same metric needs to be grooped.
		bool mIsDrawByRow;		//< Draw row by row or col by col.
		bool mIsShuffled;		//< Does Suffle actually happend here.
		bool mHasConsolidatedMetrics;  //< To pass info to dataset, whether to draw multiple data metrics in one single chart.
		
		Int32 mSizeIndex;		//< primary size index. for non-bubble chart.
		Int32 mColorIndex;		//< primary color index.
		Int32 mDataGroup;		//< original data group size without addition metrics
        Int32 mBalancedGroupCount;  //< cluster bar, empty dropzone with "MSIMetric" key, show metric labels. The Number of Metrics to be shown.
		
		std::vector<Int32> mDataMetricIndexSet;			//< column indexs of those metric used as data metric, this should go along with the rendered sequence.
		std::vector<Int32> mAdditionalMetricIndexSet;	//< column indexs of those metric used as addtional metric
		std::vector<Int32> mShuffledIndexSet;			//< the shuffled index by inserting addtional metrics to each data group.
		
		bool mHasGroupbyMetrics;			//< group by metrics or not.
		bool mIsDataNeeded;					//< is data part needed in data set population?
		Int32 mSelectedMetricNumber;		//< number of selected metrics. 

		bool Shuffle();				//< compute mShuffledIndex @return ture if shuffle actually happens.
		DataSetReformInfo();
        
        //For pie chart
        bool mIsForPieChart;
        bool mIsNeedToBreakCol;     //< for horizatial-category pie, need to break columns into groups.
        bool mIsNeedToCombineCol;  //< for category-pie, sometime need to combine groups to one group, several groups.
        bool mIsNeedToCombineSlice;  //< for horizontal-category pie, has slice, need to combine the slice to Row.
        Int32 mGroupNumForCombine;  //< number of groups to combine together : for multiple X/Y metric in one group, combine to row.
        Int32 mSliceNumForCombine;  //< number of slice to combile together : For horizontal categorypie, slice on the column, combine to row.
        Int32 mColBreakDepth;       //< columns'break starts from which depth.
        Int32 mGroupNumAfterBreak;   //< group number after col break.

        
	private:
		bool mInit;
	};


	
	/*
	 *	Dataset interface
	 */
	class FormatManager;
    class RGBColor;
    struct GraphFillGradient;
	class Dataset
	{
	public:
		// Type definition
		typedef Dataset* Ptr;	// Since Dataset needs to be passed in from other module, we use raw pointer here for simplicity.

		// Common option
		virtual void SetSeriesByColumn(bool iSeriesByColumn) = 0;
		virtual bool GetSeriesByColumn() const = 0; 
		virtual void SetReverseGroup(bool iReverseGroup) = 0;
		virtual bool GetReverseGroup() = 0;
		virtual void SetReverseSeries(bool iRevereSeries) = 0;
		virtual void SetForHistogram(bool iIsForHistogram) = 0;
		
		virtual float GetMemoryUsageEstimation() = 0;
		
		// DataSliceWindow
		virtual bool GetSliceGroupRange(Int32& iBeginGroup, Int32& iEndGroup) = 0;										///get the index for start & end group of one slice, valid for slice with continous group. 
		virtual bool PopulateOneSlice(Int32 iBeinSeries, Int32 iEndSeries, Int32 iBeginGroup, Int32 iEndGroup) = 0;		///< assume we are dealing with the simplest case
		virtual bool GetOneSliceRange(Int32& iBeinSeries, Int32& iEndSeries, Int32& iBeginGroup, Int32& iEndGroup) = 0; ///< assume we are dealing with the simplest case
		virtual bool PopulateOneSlice(const std::vector<Int32>& irSeriesIndex, const std::vector<Int32>& irGroupIndex, SLICE_TYPE iType = SLICE_STRIPPED) = 0; ///< this is for the more complex ones
		virtual bool GetOneSliceRange(std::vector<Int32>& irSeriesIndex, std::vector<Int32>& irGroupIndex) = 0;			///< this is for more complex ones
		
        
        //Data Shuffle Window
        virtual bool ShuffleIndex(bool iIsOnRow, const std::vector<Int32>& iShuffledIndexList) = 0;
        virtual SHUFFLE_STATUS GetShuffledStatus() = 0;
        virtual void RestoreFromShuffle() = 0;
        
		/*Populate one slice window, in which the groups are continous or the series are continuous
		 *@param iBegin : star index for the continuous axis
		 *@param iEnd : end index for the continuous axis
		 *@param irIndex: the index set for the uncontinuous axis
		 *@param iType: denote which axis is continuous.
		 */
		virtual bool PopulateOneSlice(Int32 iBegin, Int32 iEnd, const std::vector<Int32>& irIndex, SLICE_TYPE iType = SLICE_MIXED_GROUP) = 0; 		
		virtual bool GetSliceFlag() = 0;
		virtual void EnableSlices(bool iUseSlice = true) = 0;
		virtual bool IsSliced() = 0;
		virtual bool SetActiveSliceIndex(Int32 iIndex = 0) = 0;
		virtual Int32 GetActiveSliceIndex() = 0;
		virtual bool AdjustGroupRange(Int32 iOffset) = 0;
		virtual bool AdjustGroupRange(Int32 iBeginGroup, Int32 iEndGroup) = 0;
			
		//Data Group
		virtual HRESULT SetDataGroup(Int32 iDataGroup, bool iIgnoreForNoSeriesOrGroup = true) = 0;
	
		// Used for rendering - by SeriesID&GroupID
		virtual Int32 GetDataGroup() = 0;
		virtual HRESULT GetData(Int32 iSeriesID, Int32 iGroupID, const double*& iorValue, Int32 iColumnOffset = 0, bool iIsForSlice = true) = 0;
		virtual HRESULT GetTotalData(Int32 iIndex, bool iIsGroupTotal, Int32 iSpanStartIndex, Int32 iSpanEndIndex, double& iorValue, Int32 iColumnOffset,  bool iIsUseAbsValue = false, bool iIsForSlice = true) = 0;
        virtual HRESULT GetTotalData(Int32 iIndex, bool iIsGroupTotal, const std::vector<Int32>& iPickedIndexSet, double& iorValue, Int32 iColumnOffset,  bool iIsUseAbsValue = false, bool iIsForSlice = true) = 0;
        virtual HRESULT GetToolTipDataString(Int32 iSeriesID, Int32 iGroupID, MBase::String& orDataString, Int32 iOffset) = 0;
        virtual HRESULT GetPieSubtotal(std::map<std::pair<Int32, Int32>, double> iPieSubtotalMap, Int32 iGroupId, const std::vector<Int32>& iPickedIndexSet, double& iorValue, Int32 iColumnOffset,  bool iIsUseAbsValue = false, bool iIsForSlice = true) = 0;
        virtual void GetSubTotalGlobalRowAndColumnIndex(Int32 iGroupId, const std::vector<Int32>& iPickedIndexSet, Int32 iColumnOffset, Int32 &iorRowOrdinal, Int32 &iorColOrdinal) = 0;
		virtual HRESULT GetPrimaryData(Int32 iSeriesID,Int32 iGroupID, const double*& iorValue,Int32 iPrimaryIndex = 0, bool iIsForColor = false,  bool iIsForSlice = true, int iAngleIndex = 0, int iSliceIndex = 0) = 0;
        virtual HRESULT GetGMPieMaxMin(double& orMax, double& orMin, Int32 iColumnOffset, DATA_RETRIEVAL_TYPE iType = DRT_NORMAL) = 0;
        virtual HRESULT GetGMPieSubtotalMaxMin(std::map<std::pair<Int32, Int32>, double> iPieSubtotalMap, double& orMax, double& orMin, Int32 iColumnOffset, DATA_RETRIEVAL_TYPE iType = DRT_NORMAL) = 0;
        
		virtual HRESULT GetSeriesCount(Int32& orSeriesCount) = 0;
		virtual HRESULT GetGroupCount(Int32& orGroupCount) = 0;
        
        //Graph Matrix Pie Support, Data Info from widget level
        virtual HRESULT GetPieAngleCount(Int32 iGroupId, Int32 iSeriesId, Int32& orAngleCount) = 0;
        virtual HRESULT GetPieSliceCount(Int32 iGroupId, Int32 iSeriesId, Int32 iAngleId, Int32& orSliceCount) = 0;
        virtual HRESULT GetPieSliceData(Int32 iGroupId, Int32 iSeriesId, Int32 iAngleId, Int32 iSliceId, const double*& iorValue, Int32 iColumnOffset = -1) = 0;
        virtual HRESULT GetData(Int32 iGroupId, Int32 iSeriesId, EnumGraphMatrixDataIndexType iDataIndexType, const double*& iorValue, Int32 iAngleId = -3) = 0;
        virtual HRESULT GetDataPositionPartSize(Int32& orDataPositionPartSize) = 0;
        virtual HRESULT GetDataSizeByPartSize(Int32& orDataSizeByPartSize) = 0;
        virtual HRESULT GetDataAdditionPartSize(Int32& orDataAdditionPartSize) = 0;
        
		virtual HRESULT GetSeriesLabel(Int32 iSeriesID,MBase::String& orSeriesLabel, bool iSubstitute = true, Int32 iColumnOffset = 0, bool iIsForSlice = true, LABEL_LEVEL_OPTION iOption = LLO_NORMAL, bool iIsForEmptyAxisWithTemplateMetrc = false) = 0;
		virtual HRESULT GetGroupLabel(Int32 iGroupID, MBase::String& orGroupLabel, bool iSubstitute = true, Int32 iColumnOffset = 0, bool iIsForSlice = true, LABEL_LEVEL_OPTION iOption = LLO_NORMAL) = 0;
		virtual HRESULT GetPeakValuesAcrossSeries(double& orMax, double& orMin, const std::vector<Int32>& irSereisSet, Int32 iColumnOffset = 0) = 0;
		virtual HRESULT GetPeakValuesAcrossSeries(double& orMax, double& orMin, Int32 iBeginSeries, Int32 iEndSeries, Int32 iColumnOffset = 0) = 0;		
		
		// For dimensional label - return the group's Dimensional Tree
		virtual void GetGroupDimTree(DimTree*& iopDimTree) = 0;
		virtual MBase::String GetTemplateUnitElementName(Int32 iDimLayer, Int32 iIndexInsideDim, bool iIsInRowTree = false, Int32 iFormIndex = 0, MBase::String iSeperator = L" ") = 0;
		virtual MBase::String GetTemplateUnitName(Int32 iDimLayer, bool iIsInRowTree = false, bool iUseAlias = false) = 0;    
		virtual Int32 GetTreeWidth(Int32 iDimLayer, bool iIsInRowTree, bool iIsActualSize = false, bool iIsSqueezeHCG = true) = 0;         ///Get the number of dim node in one dim.
		//virtual Int32 GetTreeDepth(bool iIsInRowTree) = 0;
		virtual DimTree* GetRowTree() = 0;
		virtual DimTree* GetColTree() = 0;

		// Get data format for ValueAxis /each data point /thresh hold color
        /**
         *Get the automatic number format for value axis. It is either the shared format by all choosen metric or general if any two metric's format are different.
         *@param orFormat, the parsed format
         *@param iConsiderThresholds, consider threshold or not
         *@param iStartIndex, where to start the scan
         *@param iStep    
         */
		virtual HRESULT GetValueAxisNumberFormat(ChartAdvFormat& orFormat, bool iConsiderThresholds = true, Int32 iStartIndex = 0, Int32 iStep = 0) = 0;
		virtual HRESULT GetValueAxisNumberFormat(ChartAdvFormat& orFormat, const std::set<Int32>& irMetricSet) = 0;
		virtual HRESULT GetGridDataFormat(ChartAdvFormat& orFormat, Int32 iSeriesId, Int32 iGroupId, Int32 iColumnOffset, Int32 iAngleId = -3, Int32 iSliceId = -3) = 0;
		virtual HRESULT GetGridDataFormat(ChartAdvFormat& orFormat, Int32 iHostMetricIndex, bool iIsAngleMetricIndex = false, Int32 iGroupId = 0) = 0;

		virtual HRESULT	GetThresholdFillColor(Int32 iSeriesId, Int32 iGroupId, CComVariant* opValue) = 0;
		virtual THRESHOLD_FILL_STYLE GetCurrentTreshHoldFillType() = 0;
		virtual bool GetReplacedText(Int32 iSeriesId, Int32 iGroupId, Int32 iColumnOffset,MBase::String& orReplacedText, Int32& orColor) = 0;
        
        // GM TrendLine Support
        virtual std::vector<double> GetTrendLineValuesWithForecast(Int32 iSeriesId) = 0;
        virtual std::vector<double> GetCoefficients(Int32 iSeriesId) = 0;
        virtual bool IsExpomentialBaseNearOne(Int32 iSeriesId) = 0;
        
        // GM RefLine Support
        virtual std::vector<double> GetRefLineValues(Int32 iSeriesId) = 0;
        virtual std::vector<double> GetRefLineValuesForBubble(Int32 iIndex, Int32 iAxisIndex) = 0;
        virtual void GetRefLineLable(Int32 iSeriesId, Int32 iRefLineIndex, std::string& iText, bool& orIsShowValue, double& orValue) = 0;
        
		// Manual set data
		virtual HRESULT SetRowAndColumn(Int32 iRowNumber, Int32 iColumnNumber) = 0;
		virtual HRESULT SetData(Int32 iRow, Int32 iCol, double iValue) = 0;
		virtual HRESULT SetRowLabel(Int32 iRow, const wchar_t* ipRowLabel) = 0;
		virtual HRESULT SetColumnLabel(Int32 iColumn, const wchar_t* ipColumnLabel) = 0;

        //Text as shape support
        virtual bool GetTextValue(Int32 iSeriesId, Int32 iGroupId,double& orDataValue) = 0;
        
        //Getting Properties Support ============================================================================
        /*
         *find the 1st cell that has (or has not) individual formatting from either series or group.
         *@param Target  0:generic  1:individual
         */
        virtual Int32 FindCell(bool iFromSeries,Int32 Index,Int32 Target) = 0;
        virtual bool GetGenericFillFormatColor(RGBColor& orColor, Int32 iSeriesId ,Int32 iGroupId,Int32 iAngleId = -3 , Int32 iSliceId = -3) = 0;
        virtual bool GetGenericFillFormatType(unsigned char& orFillType,Int32 iSeriesId ,Int32 iGroupId,Int32 iAngleId = -3 , Int32 iSliceId = -3) = 0;
        virtual bool GetGenericFillFormatAlpha(unsigned char& orAlpha,Int32 iSeriesId ,Int32 iGroupId,Int32 iAngleId = -3 , Int32 iSliceId = -3) = 0;
        virtual bool GetGenericFillFormatGradient(GraphFillGradient& irFillGradient,Int32 iSeriesId ,Int32 iGroupId ,Int32 iSliceId = -3) = 0;
        
        virtual bool GetTrendLineThickness(Int32& orLineThickness,Int32 iSeriesId,Int32 iGroupId) = 0;
        virtual bool GetTrendLineStyle(unsigned char& orLineStyle,Int32 iSeriesId,Int32 iGroupId) = 0;
        virtual bool GetTrendLineColor(RGBColor& orLineColor,Int32 iSeriesId,Int32 iGroupId) = 0;

        virtual bool GetReferenceLineThickness(Int32& orLineThickness,Int32 iSeriesId,Int32 iGroupId) = 0;
        virtual bool GetReferenceLineStyle(unsigned char& orLineStyle,Int32 iSeriesId,Int32 iGroupId) = 0;
        virtual bool GetReferenceLineColor(RGBColor& orLineColor,Int32 mSeriesId,Int32 iGroupId) = 0;
        
        virtual bool GetAxisOriginLineThickness(Int32& orLineThickness,Int32 iObjectID) = 0;
        virtual bool GetAxisOriginLineStyle(unsigned char& orLineStyle,Int32 iObjectID) = 0;
        virtual bool GetAxisOriginLineColor(RGBColor& orLineColor,Int32 iObjectID) = 0;
        
        virtual bool GetGenericLineThickness(Int32& orLineThickness,Int32 iSeriesId ,Int32 iGroupId,Int32 iAngleId = -3 , Int32 iSliceId = -3) = 0;
        virtual bool GetGenericLineStyle(unsigned char& orLineStyle,Int32 iSeriesId ,Int32 iGroupId,Int32 iAngleId = -3 , Int32 iSliceId = -3) = 0;
        virtual bool GetGenericLineColor(RGBColor& orLineColor,Int32 iSeriesId ,Int32 iGroupId,Int32 iAngleId = -3 , Int32 iSliceId = -3) = 0;
        
        virtual bool GetAxisLineThicknessFormat(Int32& orLineThickness) = 0;
        virtual bool GetAxisLineStyleFormat(unsigned char& orLineStyle) = 0;
        virtual bool GetAxisLineColorFormat(RGBColor& orLineColor) = 0;
        
        virtual bool GetAxisMajorTickLineThicknessFormat(Int32& orLineThickness,Int32 iObjectID) = 0;
        virtual bool GetAxisMajorTickLineStyleFormat(unsigned char& orLineStyle,Int32 iObjectID) = 0;
        virtual bool GetAxisMajorTickLineColorFormat(RGBColor& orLineColor,Int32 iObjectID) = 0;
        
        virtual bool GetAxisMinorTickLineThicknessFormat(Int32& orLineThickness,Int32 iObjectID) = 0;
        virtual bool GetAxisMinorTickLineStyleFormat(unsigned char& orLineStyle,Int32 iObjectID) = 0;
        virtual bool GetAxisMinorTickLineColorFormat(RGBColor& orLineColor,Int32 iObjectID) = 0;
        
        virtual bool GetReferenceLineFontColor(RGBColor& orFontColor,Int32 iSeriesId,Int32 iGroupId) = 0;
        virtual bool GetReferenceLineFontSize(Int32& orFontSize,Int32 iSeriesId,Int32 iGroupId) = 0;
        virtual bool GetReferenceLineFontStyle(unsigned char& orFontStyle,Int32 iSeriesId,Int32 iGroupId) = 0;
        virtual bool GetReferenceLineFontName(MBase::String& orFontName,Int32 iSeriesId,Int32 iGroupId) = 0;
        
        virtual bool GetDataLableFontAlignment(unsigned char& orFontAlignment) = 0;
        virtual bool GetDataLableFontSize(Int32& orFontSize) = 0;
        virtual bool GetDataLableFontStyle(unsigned char& orFontStyle) = 0;
        virtual bool GetDataLableFontName(MBase::String& orFontName) = 0;
        virtual bool GetDataLableFontBox(unsigned char& orFontBox) = 0;
        virtual bool GetDataLableFontAlpha(unsigned char& orFontAlpha) = 0;
        virtual bool GetDataLabelFontColor(RGBColor& orFontColor) = 0;
        
        virtual bool GetAxisLabelFontSize(Int32& orFontSize ,Int32 iWhichAxis) = 0;
        virtual bool GetAxisLabelFontName(MBase::String& orFontName ,Int32 iWhichAxis) = 0;
        virtual bool GetAxisLabelFontStyle(unsigned char& orFontStyle ,Int32 iWhichAxis) = 0;
        virtual bool GetAxisLabelFontRotation(unsigned char& orFontRotation ,Int32 iWhichAxis) = 0;
        virtual bool GetAxisLabelFontColor(RGBColor& orFontColor ,Int32 iWhichAxis) = 0;
        
        virtual bool GetTextAsShapeFontSize(Int32& orFontSize,Int32 iSeriesId ,Int32 iGroupId) = 0;
        virtual bool GetTextAsShapeFontColor(RGBColor& orFontColor,Int32 iSeriesId ,Int32 iGroupId) = 0;
        virtual bool GetTextAsShapeFontName(MBase::String& orFontName,Int32 iSeriesId ,Int32 iGroupId) = 0;
        virtual bool GetTextAsShapeFontStyle(unsigned char& orFontStyle,Int32 iSeriesId ,Int32 iGroupId) = 0;
        virtual bool GetTextAsShapeAlignment(unsigned char& orFontAlignment,Int32 iSeriesId ,Int32 iGroupId) = 0;
        
        //@param iObjectId :DSSGraphUserLine1 - 4
        virtual bool GetAxisOriginValue(double& orAxisOriginValue , Int32 iObjectId) = 0;
#ifndef _VE_CE
        //============================================================================
		//AE Integration
        /**
         *This mehoud is used to initilization all the data members of the dataset instance.
         *This is used for OOTB graph rendering.
         *@param ipTemplate, the passed-in template
         *@param ipGridIterator, the passed-in iterator
         *@param iIsParseTotals, whether to consider totals.
         *@return the status of the dataset.
         */
		virtual HRESULT InitFromTemplateAndGrid(DSSTemplate* ipTemplate, DSSRWGridIterator* ipGridIterator, bool iIsParseTotals) = 0;

        /**
         *This mehoud is used to initilization all the data members of the dataset instance.
         *This is used for widget rendering.
         *@param ipTemplate, the passed-in template
         *@param ipGridIterator, the passed-in iterator
         *@param iSliceID, the slice block index.
         *@param iIsCacheData, whether to cache the data cells.
         *@return the status of the dataset.
         */
		virtual HRESULT InitFromTemplateSlice(DSSTemplate* ipTemplate, DSSRWGridIterator* ipGridIterator, Int32 iSliceID, bool iIsCacheData = false, bool iIncludeSubtotal = true) = 0;
#endif        
        /**
         *This method would check if the tree instance is empty.
         *@param iIsForGroup, ture means group tree to check out.
         *@return true means empty.
         */
		virtual bool IsHeaderTreeEmpty(bool iIsForGroup) = 0;	
		
		/**
		This function deletes the Dataset instance.
		*/
		virtual void Delete() = 0;
		
		//Populate one tree for client use with generic data input.
		//This method should be used with  extrem carefulness
		/*We use this method to boot up Row tree and Column tree
		*/	
		virtual bool PopulateOneTree(std::wifstream& iFileStream, bool iIsForRow = true) = 0;
        virtual void ConvertLocalAEIndexToGlobal(int& ioRowIndex, int& ioColIndex) = 0;
		
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////
        //                                                                                            //
        //                                                                                            //
        //                                                                                            //
        //                          Warning : this section is obsolete.                                 //
        //                                                                                            //
        //                                                                                            //
        //                                                                                            //
        ////////////////////////////////////////////////////////////////////////////////////////////////

    public://[token manager]
        
        //Drill Support with preassumption that
		//In Colmun tree, we put metric at the top level
		virtual bool PopulateTokenManager(bool iEnableToken) = 0;
		virtual bool GetTokenFlag() = 0;
		virtual TokenManager* GetTokenManager() = 0;
		virtual bool PopulateTotalValues(std::wifstream& iFileStream) = 0;	
		virtual bool Refresh() = 0;
		virtual bool GetRowCount(Int32& orCount) = 0; //to delete
		//virtual bool GetMetricsCount(Int32& orCount) = 0; //to delete
		virtual bool GetRowLabel(Int32 iIndex, MBase::String& orLabel) = 0; //to delete
		virtual bool GetMetricsLabel(Int32 iIndex, MBase::String& orLabel) = 0; //to delete

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
		/**
         *This method would return one metricUnitInfo object which is used to provide metric related information.
         *@return the MetricUnitInfo object
         */
        virtual MetricUnitInfo* GetMetricInfo() = 0;
		
        /**
         *This method would determine if the metric is on the series dimension or not.
         *@return true means metric is on the series dimension
         */
        virtual bool IsMetricBySeries() = 0;
		
		/**For Graph Drill to see if a certail graph object is total.
		 @param iRowIndex : RowIndex to the queried object.
		 @param iColIndex : ColIndex id to the queries object.
		 @param orIsTotal : the output result
		 @return true means the the result is valid and false otherwise.
		 */
		virtual bool IsSubtotalAt(Int32 iRowIndex, Int32 iColIndex, bool& orIsTotal) = 0;
        
        /**Whether or not there is valid data values in this dataset.
         @return true means no valid data value and false means otherwise.
         */
        virtual bool IsEmpty() = 0;
		
		//MSIMetric Selection Support
		virtual Int32 GetMetricsCount(bool iIsAtTopLevel = false) = 0;//note, this is not the same as the TokenManager Version
				
		//Get Label Path for Graph Drill Path		
		virtual std::string GetLabelInDrillPath(bool iIsByRow, Int32 iIndex, const std::string& irCandiate, bool iIncludeAllForms = false) = 0;		
public:		
		//Refactory into ligher component
        virtual bool IsHeaderEmpty(bool iIsForRow = true) = 0;
		virtual bool GetAvgValue(double& orValue, Int32 iIndex, Int32 iColumnOffset = 0, bool iIsForSeries = true, bool iIsForSlice = false) = 0;
		virtual void SetTreeInstance(DimTree* ipTree, bool iIsForRowTree) = 0;
		virtual Int32 GetThresholdIndex(Int32 iRowIndex, Int32 iColIndex) = 0;	
		virtual Int32 GetMetricIndex(Int32 iRowIndex, Int32 iColIndex) = 0;
        virtual Int32 GetMetricIndex(Int32 iGroupId, Int32 iSeriesId, Int32 iAngleIndex, Int32 iSliceIndex) = 0;
		virtual Int32 GetAESliceId() = 0;
		virtual Int32 GetAggregationResult(double& orValue, Int32 iIndex, bool iIsForSeries = true,  AGG_TYPE iAggType = AGG_TYPE_AVG) = 0; //return valide data cell.
	
		//virtual Int32 GetUnitWidth(Int32 iIndex, bool iIsInRow)	= 0;
		
		//[status check up]
        /**
         *Get the currrent status of this dataset.
         *@param isGraph, whether this dataset is used for graph rendering
         *@param isDummyGrid, whether this dataset is used for grid rendering, this one is obsolete
         *@return the status.
         **/
        virtual DATASET_STATUS GetStatus(bool iIsGraph, bool iIsDummyGridChart = false) = 0;
        
        /**
         *This method would get a chunck of data from the whole dataset by slicing it with the max row/col setting, and use it as the next slice for rendering.
         *@param iIsPrevious, true means previous slice and false means next
         *@param iIsStaring, true means the 1st slice and it will overrite the iIsPrvious setting, false means not the first slice.
         *@return true means a successful slicing.
         */
		virtual bool NextSliceToDisplay(bool iIsPrevious, bool iIsStaring = false) = 0;
        
        /**
         *This method would return the current slice index.
         *@return the current slice index
         **/
		virtual Int32 CurrentSliceNumberToDisplay() = 0;

        /**
         *This method would check if current slice is valid
         *@return true means INVALID
         */
		virtual bool IsDisplaySliceInvalid() = 0;
        
        /**
         *This method would return the starting postion of current slice into the whole data table.
         *@param orRowIndex is the row coordinate
         *@param orColIndex is the column coordinate
         **/
		virtual void CurrentSlicePosition(Int32& orRowIndex, Int32& orColIndex) = 0;

        /**
         *Set formatmanager instance to dataset.
         *@param ipFormatManager, the passed-in formatManager instance.
         */
		virtual void SetFormatManager(FormatManager* ipFormatManager) = 0;

		
        virtual void SeriesGroupIndexToRowColumnIndex(Int32 iSeriesID, Int32 iGroupID, Int32& orRowIndex, Int32& orColumnIndex, Int32 iColumnOffset, Int32 iAngleId = -3, Int32 iSliceId = -3, bool iIsConsiderSubtotal = true) = 0;
        virtual void GetRowAndColumnIndexForMinMaxMetrixIndex(bool iIsForMax, std::vector<Int32> iMetricIndex, Int32& orRowIndex, Int32& orColumnIndex) = 0;
        
        
		virtual bool GetSeriesGroupIdsFromIndex(std::vector<std::pair<Int32, Int32> >& iorSGPairs, const std::vector<Int32>& irRowIndexSet, const std::vector<Int32>& irColIndexSet) = 0;

		virtual Int32 GetColorStyleIndex(Int32 iLocalIndex) = 0;
		virtual Int32 GetColorStyleIndex(Int32 iSeriesID, Int32 iGroupID, Int32 iColumnOffset = 0, bool iIsConsiderSubtotal = true) = 0;
        virtual std::string GetGMColorByKey(std::vector<std::pair<Int32, Int32> > iColorByUnitsIndex, Int32 iSeriesID, Int32 iGroupID, Int32 iColumnOffset = 0, bool iIsConsiderSubtotal = true) = 0;
        virtual int GetGMColorByIndex(Int32 iSeriesID, Int32 iGroupID, Int32 iAngleIndex = -3, Int32 iColumnOffset = 0) = 0;
        virtual void getLineMarkerColorSetting(std::vector<int>& iSetting) = 0;
		virtual MBase::String GetColorStyleLabel(Int32 iSeriesID, Int32 iGroupID, Int32 iColumnOffset, bool iIsConsiderSubtotal = true)  = 0;
		virtual MBase::String GetColorStyleLabel(Int32 iLocalColorIndex)  = 0;
		virtual Int32 GetColorStyleNumber() = 0;
		virtual void SetColorSlices(std::vector<Int32>& irRowUnits, std::vector<Int32> irColumnUnits) = 0; 
#ifndef _VE_CE
		virtual void SetGlobalIterator(DSSRWGridIterator* ipIterator) = 0;	
#endif
		virtual	void GetLabelCollection(Int32 iOrdinal, bool iIsForRow, std::vector<std::string>&orCollection) = 0;
        virtual	void GetDataTextLabel(Int32 iOrdinal, bool iIsForRow, MBase::String& orCollection, int iDepth) = 0;
        virtual	void GetBreakByLabel(Int32 iOrdinal, bool iIsForRow, MBase::String& orCollection) = 0;  //< Data Label for GM, show Text
        /**
         *This method would return all the alias name of the tempalte unit along the denoted axis.
         *@param isForRow, if it is on row axis,
         *@param orCollection, the results, an array of unit name.
         *@param iParseTemplateMetrics, how to handle tempalte metrics.
         */
		virtual void GetUnitNameCollection(bool iIsForRow, std::vector<std::string>&orCollection) = 0;
        virtual	void GetSliceLabel(Int32 iOrdinal, bool iIsForRow, MBase::String& orCollection) = 0;  //< Data Label for GM in Pie chart, show Text
        virtual	void GetTextDataLabel(Int32 iGroupId, Int32 iSeriesId, std::string& orCollection, Int32 iAngleId = -3, Int32 iSliceId = -3) = 0;
        virtual void GetTextDataLabel(Int32 iOrdinal, std::string& orCollection) = 0;
		
        // [GM Bullet Chart Suppor]
        virtual void GetBandDivision(Int32 iGroupId, Int32 iVAIndex, std::vector<double>& iMetrics, std::vector<double>& iRatios) = 0;
        
        // [GM Partial Rendering Support]
        virtual bool IsGroupOnScreen(Int32 iGroupId) = 0;
        virtual bool IsSeriesOnScreen(Int32 iSeriesId) = 0;
        
        // [GM Combination Graph Support]
        // This method to get the shape of each series, if not combination graph, all series have same shape!
        virtual Int32 GetShapeBySeriesId(Int32 iSeriesId) = 0;
        
        // [GM Metric Level Data Label Support]
        virtual bool IsDataLabelEnaled(Int32 iSeriesId, Int32 iGroupId, Int32 iAngleId = -3, Int32 iSliceId = -3) = 0;
        
        // [GM Dual Axis Graph Support]
        // This method is used to get the series index belong to secondary value axis.
        virtual std::set<int> GetSeriesInSecondaryAxis() = 0;
        
        virtual HRESULT GetCumulatedData(Int32 iGroupId, Int32 iSeriesId, EnumGraphMatrixDataIndexType iDataIndexType, const double*& iorValue) = 0;
		virtual std::pair<int, int> getOnScreenTextRange(bool isForGroup) = 0;
		//[DataSet Reform]
#ifndef _VE_CE
		virtual bool PopulateReformer(DSSRWGridIterator* ipGridIterator, DataSetReformInfo* ipReformInfo) = 0;
#endif
		virtual bool IsReformed() = 0;
        virtual Int32 GetMetricLabelCount() = 0;
        virtual Int32 GetBalancedCount(bool iIsForGroup) = 0;
        virtual const DataGroupInfo* GetDataGroupInfo() = 0; 
        virtual Int32 GetSeriesPieceStartIndex(Int32 iIndex) = 0;
        virtual void SetDatagroupInfo(const DataGroupInfo& irInfo) = 0;
        virtual const std::vector<int> GetAxisInfo() = 0;
        virtual void SetAxisInfo(const std::vector<int> &irAxisInfo) = 0;
        virtual void GetSeriesSplitVector(std::vector<Int32>& orVec) = 0;
        virtual void SetSeriesSplitVector(const std::vector<Int32>& irVec) = 0;
        virtual void SetPieGroupDetph(Int32 irDetph) = 0;
        virtual void GetPieGroupDetph(Int32 &orDepth) = 0;
        virtual void DumpDataSet(bool iIsNaive, bool isGlobalIterator = false) = 0;
        virtual void SetGMDataSet(void *pDataSet = NULL) = 0;
        virtual void PrepareIterator() = 0;
        //virtual void DumpDataSet(bool iIsNaive, bool isGlobalIterator) = 0;
	};

	// Export Function
	CHARTENGINE_API Dataset::Ptr CreateDataset(Int32 iDataType);
    
	
    
#ifndef _VE_CE	
	/*
	 * 3 input parameters stands for the template (to get the MSIHeader Info), the GridIterator(to get data cell), 
	 * the flag to denote whether this dataset needs to get layered subtotal info (to support MiniGrid) 
	 */
	CHARTENGINE_API Dataset::Ptr CreateDataset(DSSTemplate* ipTemplate, DSSRWGridIterator* ipGridIterator, bool iIsParseTotals = true);                   
	
	CHARTENGINE_API Dataset::Ptr CreateDataset(DSSTemplate* ipTemplate, DSSRWGridIterator* ipGridIterator, bool iIsParseTotals, FormatManager* ipFormatManager);                   

	/*
	* New workflow, make dataset a slice into AE datatable
	*
	*/
	CHARTENGINE_API Dataset::Ptr CreateDataset(DSSTemplate* ipTemplate, DSSRWGridIterator* ipGridIterator, Int32 iIntSliceID, bool iIsCacheData = false, bool iIncludeSubtotal = true);
	CHARTENGINE_API Dataset::Ptr CreateDataset(DSSTemplate* ipTemplate, DSSRWGridIterator* ipGridIterator, Int32 iIntSliceID, DataSetReformInfo* ipReformInfo, bool iIsCacheData = false);
    CHARTENGINE_API Dataset::Ptr CreateDatasetForGraphMatrix(void* iGraphMatrixDataSet);
#else
    CHARTENGINE_API Dataset::Ptr CreateDatasetForGraphMatrix(MVE::GMCEDataProvider::Ptr iGraphMatrixDataSet);
#endif
};

#endif

