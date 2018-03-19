//==============================================================================================
//	FILENAME	:	DatasetImpl.h
//	AUTHOR		:	Dong Shi & Fangyao Ma
//	CREATION	:	2008-05-15
//	Copyright (C) MicroStrategy Incorporated 2008
//	All Rights Reserved
//==============================================================================================

#ifndef MsiChart_DatasetImpl_h
#define MsiChart_DatasetImpl_h

#include "../Dataset.h"
#include "DatasetHelper.h" 
#include "PDCHeader/PDCmap"
#include "PDCHeader/PDCvector"
#include "Synch/Defines/SmartPtr.h"
#include "FormatManager.h"

#ifdef _VE_CE
#else
#import "DatasetImplHelper.h"
#endif

namespace MsiChart
{
	/// Maximum number of Multi Dimensional Label supported
	const unsigned Int32 gMaxMDLabel = 32;	
	
	typedef  unsigned char DSSDataFlag;		
	typedef  std::pair<Int32, Int32>  _PositionIndex;
	typedef  std::pair<Int32, Int32>  _PositionAssociatedInfo;
	
	/**
	 *	Dataset Interface's implementation, holds data for Chart to render
	 */
	class DatasetImpl:public Dataset
	{
	public:
		/***********************************************************
		*	Constructor - for different access mode
		***********************************************************/
		/**
		 *	Constructor
		 *	@param iAccessMode Generic data or data from template
		 */
		DatasetImpl(Int32 iAccessMode);
		///	Destructor, release resource
		virtual ~DatasetImpl() throw();

		// Common option
		///	Set "Series by Column" flag
		virtual void SetSeriesByColumn(bool iSeriesByColumn);
		///	Get "Series by Column" flag
		virtual bool GetSeriesByColumn() const; 
		///	Set "Reverse Group" flag
		virtual void SetReverseGroup(bool iReverseGroup);
		///	Get "Reverse Group" flag
		virtual bool GetReverseGroup();
		///	Set "Reverse Series" flag
		virtual void SetReverseSeries(bool iRevereSeries);
		/// Set "Is For Histogram" flag
		virtual void SetForHistogram(bool iIsForHistogram);
		
		virtual float GetMemoryUsageEstimation();
	
		/**
		 *	Set the data grouping information
		 *	@param iDataGroup Number of columns will grouping to form one Series/Group
		 */
		virtual HRESULT SetDataGroup(Int32 iDataGroup, bool iIgnoreForNoSeriesOrGroup = true);

		/************************************************************
		*		Used for renderring - by SeriesID&GroupID
		************************************************************/
		/**
		 *	Return the data grouping information
		 *	@return iDataGroup Number of columns will grouping to form one Series/Group
		 */
		virtual Int32 GetDataGroup();

		/**
		 *	Get the data with specified SeriesID, GroupID & Column offset
		 *	@param	iSeriesID	Series index
		 *	@param	iGroupID	Group index
		 *	@param	iorValue	cell data
		 *	@param	iColumnOffset	column offset
		 *	@return  iorValue will point to the specified cell data with SeriesID, GroupID & Column offset
		 */
		virtual HRESULT GetData(Int32 iSeriesID,Int32 iGroupID, const double*& iorValue, Int32 iColumnOffset = 0, bool iIsForSlice = true);
        virtual HRESULT GetGMPieMaxMin(double& orMax, double& orMin, Int32 iColumnOffset, DATA_RETRIEVAL_TYPE iType = DRT_NORMAL);
        //Use the Subtotal from AE, passed from Widget.
        virtual HRESULT GetGMPieSubtotalMaxMin(std::map<std::pair<Int32, Int32>, double> iPieSubtotalMap, double& orMax, double& orMin, Int32 iColumnOffset, DATA_RETRIEVAL_TYPE iType = DRT_NORMAL);
        virtual HRESULT GetTotalData(Int32 iIndex, bool iIsGroupTotal, Int32 iSpanStartIndex, Int32 iSpanEndIndex, double& iorValue, Int32 iColumnOffset,  bool iIsUseAbsValue = false, bool iIsForSlice = true);
        virtual HRESULT GetTotalData(Int32 iIndex, bool iIsGroupTotal, const std::vector<Int32>& iPickedIndexSet, double& iorValue, Int32 iColumnOffset,  bool iIsUseAbsValue = false, bool iIsForSlice = true);
        virtual HRESULT GetPieSubtotal(std::map<std::pair<Int32, Int32>, double> iPieSubtotalMap, Int32 iGroupId, const std::vector<Int32>& iPickedIndexSet, double& iorValue, Int32 iColumnOffset,  bool iIsUseAbsValue = false, bool iIsForSlice = true);
        virtual void GetSubTotalGlobalRowAndColumnIndex(Int32 iGroupId, const std::vector<Int32>& iPickedIndexSet, Int32 iColumnOffset, Int32 &iorRowOrdinal, Int32 &iorColOrdinal);
		virtual HRESULT GetPrimaryData(Int32 iSeriesID,Int32 iGroupID, const double*& iorValue,Int32 iPrimaryIndex = 0, bool iIsForColor = false,  bool iIsForSlice = true, int iAngleIndex=0, int iSliceIndex=0);
        virtual HRESULT GetToolTipDataString(Int32 iSeriesID, Int32 iGroupID, MBase::String& orDataString, Int32 iOffset);
		/**
		 *	Get the Series count
		 *	@param	orSeriesCount	get the Series count within the dataset
		 */
		virtual HRESULT GetSeriesCount(Int32& orSeriesCount);
		
		/**
		 *	Get the Group count
		 *	@param	orGroupCount	get the Group count within the dataset
		 */
		virtual HRESULT GetGroupCount(Int32& orGroupCount);
		
		/**
		 *	Get the Series Label
		 *	@param	iSeriesID	Series index
		 *	@param	orSeriesLabel	Series label
		 *	@param	iSubstitute	 Flag indicate whether to sustitute the "\n" for "space" with the label
		 */
		virtual HRESULT GetSeriesLabel(Int32 iSeriesID,MBase::String& orSeriesLabel,bool iSubstitute = true, Int32 iColumnOffset = 0, bool iIsForSlice = true, LABEL_LEVEL_OPTION iOption = LLO_NORMAL, bool iIsEmptyAxisWithTemplateMetrc = false);
		
		/**
		 *	Get the Group Label
		 *	@param	iSeriesID	Group index
		 *	@param	orSeriesLabel	Group label
		 */
		virtual HRESULT GetGroupLabel(Int32 iGroupID,MBase::String& orGroupLabel, bool iSubstitute = true, Int32 iColumnOffset = 0, bool iIsForSlice = true, LABEL_LEVEL_OPTION iOption = LLO_NORMAL);
		
		/**
		 *	Get the group dimensional tree
		 *	@param	iopDimTree	Series index
		 */
		virtual void GetGroupDimTree(DimTree*& iopDimTree);
				
		/*
		 *Get the number format for value axis
		 */
		virtual HRESULT GetValueAxisNumberFormat(ChartAdvFormat& orFormat, bool iConsiderThresholds = true, Int32 iStartIndex = 0, Int32 iStep = 0);
        virtual HRESULT GetValueAxisNumberFormat(ChartAdvFormat& orFormat, const std::set<Int32>& irMetricSet);

		/*
		 *Get the number format for one speicifi data point.
		 */
		virtual HRESULT GetGridDataFormat(ChartAdvFormat& orFormat, Int32 iSeriesId, Int32 iGroupId, Int32 iColumnOffset, Int32 iAngleId = -3, Int32 iSliceId = -3);

		/**
		Used for From Grid, via the hosting metric Index.
		*/
		virtual HRESULT GetGridDataFormat(ChartAdvFormat& orFormat, Int32 iHostMetricIndex, bool iIsAngleMetrixIndex = false, Int32 iGroupId = 0);

		/**
		Get the threshold fill color.
		@param iSeriesId Specifies the series id.
		@param iGroupId Specifies the group id.
		@param opValue Stores the color value.
		*/
		virtual HRESULT	GetThresholdFillColor(Int32 iSeriesId, Int32 iGroupId, CComVariant* opValue);  //not used for now
		virtual THRESHOLD_FILL_STYLE GetCurrentTreshHoldFillType();

        // GM RefLine Support
        virtual std::vector<double> GetRefLineValues(Int32 iSeriesId);
        virtual void GetRefLineLable(Int32 iSeriesId, Int32 iRefLineIndex, std::string& iText, bool& orIsShowValue, double& orValue);
        virtual std::vector<double> GetRefLineValuesForBubble(Int32 iIndex, Int32 iAxisIndex);
        
        // GM TrendLine Support
        virtual std::vector<double> GetTrendLineValuesWithForecast(Int32 iSeriesId);
        virtual bool IsExpomentialBaseNearOne(Int32 iSeriesId);
        virtual std::vector<double> GetCoefficients(Int32 iSeriesId);
        
		virtual bool GetReplacedText(Int32 iSeriesId, Int32 iGroupId, Int32 iColumnOffset, MBase::String& orReplacedText, Int32& orColor);
		
		
		/***********************************************************
		*		Manual set data - interface for inner class
		***********************************************************/
		/**
		 *	Set the number of rows and columns
		 *	@param iRowNumber number of rows
		 *	@param iColumnNumber number of columns
		 */	
		virtual HRESULT SetRowAndColumn(Int32 iRowNumber,Int32 iColumnNumber);
		/**
		 *	Set the data with the specified row and column index
		 *	@param iRow	row index
		 *	@param iColumn	column index
		 *	@param	iValue value for the specified row and column index
		 */	
		virtual HRESULT SetData(Int32 iRow,Int32 iCol,double iValue);

		/**
		 *	Set the row label string
		 *	@param iRow	row index
		 *	@param ipRowLabel	label string with the specified row index
		 */
		virtual HRESULT SetRowLabel(Int32 iRow,const wchar_t* ipRowLabel);

		/**
		 *	Set the column label string
		 *	@param iColumn	column index
		 *	@param ipColumnLabel	label string with the specified column index
		 */
		virtual HRESULT SetColumnLabel(Int32 iColumn,const wchar_t* ipColumnLabel);

		virtual void Delete();
		

	//public:		
		/// This section include the methods for token manger and the refreshment
		/*
		 *[PREASSUMTION assumed]: 1. Metic is in column tree 2. Metric is the 1st template unit along the column axis
		 *For Drill Support
		 */
		virtual bool PopulateTokenManager(bool iEnableToken);				///< Enable/disable the token manager by the input parameter
		virtual TokenManager* GetTokenManager();							///< Returen the reference of the tokenmanager
		virtual bool PopulateTotalValues(std::wifstream& iFileStream);		///< Populate the total info into the table pool
		virtual bool GetTokenFlag();										///< Get the flag to see if token manager is active
		virtual bool GetRowCount(Int32& orCount);							///< Get the row of the subtree rooted by current token manger's position 
		//virtual bool GetMetricsCount(Int32& orCount);						///< Get metric element number
		virtual bool GetRowLabel(Int32 iIndex, MBase::String& orLabel);		///< Get row label for the child indexed by iIndex of the node positioned by the token manager
		virtual bool GetMetricsLabel(Int32 iIndex, MBase::String& orLabel); ///< Get the name of the metric element indexed by iIndex
		virtual bool Refresh();		
		//[End of Public Methods for TokenManager and Dataset Refreshment]
		
		virtual bool IsSubtotalAt(Int32 iRowIndex, Int32 iColIndex, bool& orIsTotal);
		virtual MetricUnitInfo* GetMetricInfo();
		virtual bool IsMetricBySeries();
		
	public:
		///This section include methods for misc uses 
		virtual bool PopulateOneTree(std::wifstream& iFileStream, bool iIsForRow = true);					 ///< Populate one tree structure from the generic input data	
		virtual HRESULT GetPeakValuesAcrossSeries(double& orMax, double& orMin, const std::vector<Int32>& irSereisSet, Int32 iColumnOffset = 0);  ///< Get the Max/Min Value across the specified sereis collection, no matter slice is used or not
		virtual HRESULT GetPeakValuesAcrossSeries(double& orMax, double& orMin, Int32 iBeginSeries, Int32 iEndSeries, Int32 iColumnOffset = 0);	  ///< overloaded version of the above one	
		//[End of Public Methods for Misc use]
        virtual void ConvertLocalAEIndexToGlobal(int& ioRowIndex, int& ioColIndex);
		
	public: 
		// This section contains Data slice stuff			
		virtual bool PopulateOneSlice(Int32 iBeinSeries, Int32 iEndSeries, Int32 iBeginGroup, Int32 iEndGroup);  ///< assume we are dealing with the simplest case
		virtual bool GetOneSliceRange(Int32& iBeinSeries, Int32& iEndSeries, Int32& iBeginGroup, Int32& iEndGroup);  ///< assume we are dealing with the simplest case
		virtual bool PopulateOneSlice(const std::vector<Int32>& irSeriesIndex, const std::vector<Int32>& irGroupIndex, SLICE_TYPE = SLICE_STRIPPED); ///< this is for the more complex ones
		virtual bool GetOneSliceRange(std::vector<Int32>& irSeriesIndex, std::vector<Int32>& irGroupIndex); ///< this is for more complex ones		
		virtual bool PopulateOneSlice(Int32 iBegin, Int32 iEnd, const std::vector<Int32>& irIndex, SLICE_TYPE iType = SLICE_MIXED_GROUP); //either series or groups are continous
		virtual bool GetSliceGroupRange(Int32& iBeginGroup, Int32& iEndGroup); //get the index for start & end group of one slice, valid for slice with continous group. 

		virtual void EnableSlices(bool iUseSlice = true);
		virtual bool IsSliced();
		virtual bool GetSliceFlag();
		virtual bool SetActiveSliceIndex(Int32 iIndex = 0);
		virtual Int32 GetActiveSliceIndex();		 
		//virtual Int32  GetSliceNumber(Int32  iGroupPerSlice = -1, Int32 iSeriesPerSlice = -1);/// Get Slice Number
		virtual bool AdjustGroupRange(Int32 iOffset);
		virtual bool AdjustGroupRange(Int32 iBeginGroup, Int32 iEndGroup);
        
        virtual bool ShuffleIndex(bool iIsOnRow, const std::vector<Int32>& iShuffledIndexList);
        virtual SHUFFLE_STATUS GetShuffledStatus();
        virtual void RestoreFromShuffle();

	public: 
		// This section relates to the dim tree info.
		virtual MBase::String GetTemplateUnitElementName(Int32 iDimLayer, Int32 iIndexInsideDim, bool iIsInRowTree = false, Int32 iFormIndex = 0, MBase::String iSeperator = L" ");
		//virtual Int32 GetTreeDepth(bool iIsInRowTree);
		virtual MBase::String GetTemplateUnitName(Int32 iDimLayer, bool iIsInRowTree = false, bool iUseAlias = false);    
		virtual Int32 GetTreeWidth(Int32 iDimLayer, bool iIsInRowTree, bool iIsActualSize = false, bool iIsSqueezeHCG = true);         ///Get the number of dim node in one dim.
		virtual DimTree* GetRowTree();
		virtual DimTree* GetColTree();

	public://for metric selection
		virtual Int32 GetMetricsCount(bool iIsAtTopLevel = false);//note, this is not the same as the TokenManager Version
		
	public: //For AE integration	
#ifndef _VE_CE
		virtual HRESULT InitFromTemplateAndGrid(DSSTemplate* ipTemplate, DSSRWGridIterator* ipGridIterator, bool iIsParseTotals);
#endif
		virtual bool IsHeaderTreeEmpty(bool iIsForGroup);	

	public:
		virtual Int32 GetThresholdIndex(Int32 iRowIndex, Int32 iColIndex);	
		virtual Int32 GetMetricIndex(Int32 iRowIndex, Int32 iColIndex);
        virtual Int32 GetMetricIndex(Int32 iGroupId, Int32 iSeriesId, Int32 iAngleIndex, Int32 iSliceIndex);
        
        //for dataset refactor
        virtual HRESULT GetPieAngleCount(Int32 iGroupId, Int32 iSeriesId, Int32& orAngleCount);
        virtual HRESULT GetPieSliceCount(Int32 iGroupId, Int32 iSeriesId, Int32 iAngleId, Int32& orSliceCount);
        virtual HRESULT GetPieSliceData(Int32 iGroupId, Int32 iSeriesId, Int32 iAngleId, Int32 iSliceId, const double*& iorValue, Int32 iColumnOffset = -1);
        virtual HRESULT GetData(Int32 iGroupId, Int32 iSeriesId, EnumGraphMatrixDataIndexType iDataIndexType, const double*& iorValue, Int32 iAngleId = -3);
        virtual HRESULT GetDataPositionPartSize(Int32& orDataPositionPartSize);
        virtual HRESULT GetDataSizeByPartSize(Int32& orDataSizeByPartSize);
        virtual HRESULT GetDataAdditionPartSize(Int32& orDataAdditionPartSize);
        virtual void SetGMDataSet(void *pDataSet = NULL);
        virtual void PrepareIterator();
        virtual int GetGMColorByIndex(Int32 iSeriesID, Int32 iGroupID, Int32 iAngleIndex = -3, Int32 iColumnOffset = -1);
        virtual void getLineMarkerColorSetting(std::vector<int>& iSetting);
        // [GM Partial Rendering Support]
        virtual bool IsGroupOnScreen(Int32 iGroupId);
        virtual bool IsSeriesOnScreen(Int32 iSeriesId);
        // [GM Bullet Chart Suppor]
        virtual void GetBandDivision(Int32 iGroupId, Int32 iVAIndex, std::vector<double>& iMetrics, std::vector<double>& iRatios);
        // [GM Combination Graph Support]
        virtual Int32 GetShapeBySeriesId(Int32 iSeriesId);
        // In Combination Graph, different shapes can stacked, so, we need to calculate stacked value in dataset, not in SeriesRender
        virtual HRESULT GetCumulatedData(Int32 iGroupId, Int32 iSeriesId, EnumGraphMatrixDataIndexType iDataIndexType, const double*& iorValue);
        virtual std::pair<int, int> getOnScreenTextRange(bool isForGroup);
        
        // [GM Metric Level Data Label Support]
        virtual bool IsDataLabelEnaled(Int32 iSeriesId, Int32 iGroupId, Int32 iAngleId = -3, Int32 iSliceId = -3);
        
        virtual std::set<int> GetSeriesInSecondaryAxis();
        
        //[GM text as shape support]
        virtual bool GetTextValue(Int32 iSeriesId, Int32 iGroupId,double& orDataValue);
        //[GM getting properties support]
        virtual Int32 FindCell(bool iFromSeries,Int32 Index,Int32 Target);
        virtual bool GetGenericFillFormatColor(RGBColor& orColor, Int32 iSeriesId ,Int32 iGroupId,Int32 iAngleId = -3 , Int32 iSliceId = -3);
        virtual bool GetGenericFillFormatType(unsigned char& orFillType,Int32 iSeriesId ,Int32 iGroupId,Int32 iAngleId = -3 , Int32 iSliceId = -3);
        virtual bool GetGenericFillFormatAlpha(unsigned char& orAlpha,Int32 iSeriesId ,Int32 iGroupId,Int32 iAngleId = -3 , Int32 iSliceId = -3);
        virtual bool GetGenericFillFormatGradient(GraphFillGradient& irFillGradient,Int32 iSeriesId ,Int32 iGroupId, Int32 iSliceId = -3);
        
        virtual bool GetTrendLineThickness(Int32& orLineThickness,Int32 iSeriesId,Int32 iGroupId);
        virtual bool GetTrendLineStyle(unsigned char& orLineStyle,Int32 iSeriesId,Int32 iGroupId);
        virtual bool GetTrendLineColor(RGBColor& orLineColor,Int32 iSeriesId,Int32 iGroupId);
        
        virtual bool GetReferenceLineThickness(Int32& orLineThickness,Int32 iSeriesId,Int32 iGroupId);
        virtual bool GetReferenceLineStyle(unsigned char& orLineStyle,Int32 iSeriesId,Int32 iGroupId);
        virtual bool GetReferenceLineColor(RGBColor& orLineColor,Int32 mSeriesId,Int32 iGroupId);
        
        virtual bool GetAxisOriginLineThickness(Int32& orLineThickness,Int32 iObjectID);
        virtual bool GetAxisOriginLineStyle(unsigned char& orLineStyle,Int32 iObjectID);
        virtual bool GetAxisOriginLineColor(RGBColor& orLineColor,Int32 iObjectID);
        
        virtual bool GetGenericLineThickness(Int32& orLineThickness,Int32 iSeriesId ,Int32 iGroupId,Int32 iAngleId = -3 , Int32 iSliceId = -3);
        virtual bool GetGenericLineStyle(unsigned char& orLineStyle,Int32 iSeriesId ,Int32 iGroupId,Int32 iAngleId = -3 , Int32 iSliceId = -3);
        virtual bool GetGenericLineColor(RGBColor& orLineColor,Int32 iSeriesId ,Int32 iGroupId,Int32 iAngleId = -3 , Int32 iSliceId = -3);
        
        virtual bool GetAxisLineThicknessFormat(Int32& orLineThickness);
        virtual bool GetAxisLineStyleFormat(unsigned char& orLineStyle);
        virtual bool GetAxisLineColorFormat(RGBColor& orLineColor);
        
        virtual bool GetAxisMajorTickLineThicknessFormat(Int32& orLineThickness,Int32 iObjectID);
        virtual bool GetAxisMajorTickLineStyleFormat(unsigned char& orLineStyle,Int32 iObjectID);
        virtual bool GetAxisMajorTickLineColorFormat(RGBColor& orLineColor,Int32 iObjectID);
        
        virtual bool GetAxisMinorTickLineThicknessFormat(Int32& orLineThickness,Int32 iObjectID);
        virtual bool GetAxisMinorTickLineStyleFormat(unsigned char& orLineStyle,Int32 iObjectID);
        virtual bool GetAxisMinorTickLineColorFormat(RGBColor& orLineColor,Int32 iObjectID);
        
        virtual bool GetReferenceLineFontColor(RGBColor& orFontColor,Int32 iSeriesId,Int32 iGroupId);
        virtual bool GetReferenceLineFontSize(Int32& orFontSize,Int32 iSeriesId,Int32 iGroupId);
        virtual bool GetReferenceLineFontStyle(unsigned char& orFontStyle,Int32 iSeriesId,Int32 iGroupId);
        virtual bool GetReferenceLineFontName(MBase::String& orFontName,Int32 iSeriesId,Int32 iGroupId);
        
        virtual bool GetDataLableFontAlignment(unsigned char& orFontAlignment);
        virtual bool GetDataLableFontSize(Int32& orFontSize);
        virtual bool GetDataLableFontStyle(unsigned char& orFontStyle);
        virtual bool GetDataLableFontName(MBase::String& orFontName);
        virtual bool GetDataLableFontBox(unsigned char& orFontBox);
        virtual bool GetDataLableFontAlpha(unsigned char& orFontAlpha);
        virtual bool GetDataLabelFontColor(RGBColor& orFontColor);
        
        virtual bool GetAxisLabelFontSize(Int32& orFontSize ,Int32 iWhichAxis);
        virtual bool GetAxisLabelFontName(MBase::String& orFontName ,Int32 iWhichAxis);
        virtual bool GetAxisLabelFontStyle(unsigned char& orFontStyle ,Int32 iWhichAxis);
        virtual bool GetAxisLabelFontRotation(unsigned char& orFontRotation ,Int32 iWhichAxis);
        virtual bool GetAxisLabelFontColor(RGBColor& orFontColor ,Int32 iWhichAxis);
        
        virtual bool GetTextAsShapeFontSize(Int32& orFontSize,Int32 iSeriesId ,Int32 iGroupId);
        virtual bool GetTextAsShapeFontColor(RGBColor& orFontColor,Int32 iSeriesId ,Int32 iGroupId);
        virtual bool GetTextAsShapeFontName(MBase::String& orFontName,Int32 iSeriesId ,Int32 iGroupId);
        virtual bool GetTextAsShapeFontStyle(unsigned char& orFontStyle,Int32 iSeriesId ,Int32 iGroupId);
        virtual bool GetTextAsShapeAlignment(unsigned char& orFontAlignment,Int32 iSeriesId ,Int32 iGroupId);
        
        virtual bool GetAxisOriginValue(double& orAxisOriginValue , Int32 iObjectId);

	private: //For AE integration, helper methods
#ifndef _VE_CE
		bool hPopulateDataCells(DSSRWGridIterator* ipGridIterator, Int32 iRowCount, Int32 iColumnCount);
		bool hPopulatePlainLabels(bool iIsRow);
		bool hPopulatePlainLabelsWithIterator(DSSRWGridIterator* ipIterator, bool iIsRow);

		bool hMoveIteratorByElement(DSSRWGridIterator* ipGridIterator, Int32 iDepth, bool iIsWithBoundaryCheck, bool iIsForRow, bool iIsForward); //To replace the NextXXXNByElement
		bool hMoveIteratorByElement(DSSRWGridIterator* ipGridIterator, Int32 iDepth, bool iIsForRow = true, bool iIsForward = true); //To replace the NextXXXNByElement
#endif
		void hGetShuffledIndex(Int32& iorRowIndex, Int32& iorColIndex);
        Int32 hGetShuffledIndexOn(bool iIsOnRow, Int32 iIndex);
        bool hIsIndexShuffled();
	private: 
		//This secton contans private helper methods
		void hTrimSpace(MBase::String& iorLabel);
        
        /* used in deciding the value axis number format, to decide how many cells per metric should be scanned.
         * @param iOnRow, use row count as the max limit or not.
         * @return the max limit to scan for one single metric.
         */
		Int32 hGetScanRange(bool iOnRow,  bool iConsiderThresholds);
        
        /* Helper function used in GetValueAxisNumberFormat.
         * @return true, means the result is finalized as a uniform form format.
         */
        bool hGetValueAxisNumberFormat(ChartAdvFormat& orFormat, bool iConsiderThresholds, Int32 iStartIndex, Int32 iStep);

		/**
		 *	Simple Data holder
		 */	
		class SimpleDataset
		{
		public:
			SimpleDataset();
			~SimpleDataset();
			void SetDimension(const Int32 iRowCount, const Int32 iColumnCount);
			void AddData(const Int32 iRowIndex, const Int32 iColumnIndex, const double* irValue);
			void GetData(const Int32 iRowIndex, const Int32 iColumnIndex, const double*& orData);
			Int32 GetRowCount();
			Int32 GetColumnCount();
			void Delete();
            Int32 GetMemoryUsageEstimation();
		private:
			Int32 mRowCount;	///< Number of Rows
			Int32 mColumnCount;	///< Number of Columns
			double** mpData;	///< ultimate structure used to hold data
		};

		
	
	//Methods and Inner Class For template, they are not used by CE for now, maybe will be resued will AE logic steps-in, --dshi
	private:
		/**
		 *	Inner class for base forms, From GraphGenImpl
		 */
		class BaseForms
		{
		public:
			std::vector<Int32> mBaseFormIDs;
			std::vector<Int32> mBaseFormTypes;
			BaseForms& operator = (BaseForms& iBaseForms)
			{
				mBaseFormIDs = iBaseForms.mBaseFormIDs;
				mBaseFormTypes = iBaseForms.mBaseFormTypes;
				return *this;
			}
		};
		// Private function for parsing template - some from GraphGenImpl.cpp
		void hGetRowColumnIndex(Int32 iSeriesID, Int32 iGroupID, Int32& orRowIndex, Int32& orColumnIndex, Int32 iColumnOffset = 0);
#ifndef _VE_CE
		void hGetRealRowAndColumnCount(Int32& orRowCount, Int32& orColumnCount, DSSRWGridIterator* ipIterator);
#endif
    //[End of Inner Class and Methods for Template]

		
	private: ///< Methods in this section are used for Tree Population from Generic Data		
		/**
		 *	Get the path to the leaf node indexed by iIndex
		 *	@param iIndex the index for the leafnode
		 *	@param orDimIndexSet the retrieved path info
		 *  @return true if we find the path to that node and false ortherwise
		 */		
		bool hMapIndexToTreeDimInfo(Int32 iIndex, DimNode* ipNode, std::vector<Int32>& orDimIndexSet);								//not used now
		bool hGetLabelPieceAlongPath(MBase::String& orFullLabel, DimTree* ipTree, const std::vector<Int32>& irPath, Int32 iFormat); //not used now
		///< Parese one dim(layer) along the tree top -> bottom, iDimIndex is the index of the dim to be parsed
		bool hPopulateOneDimFromString(const MBase::String& irString, Int32 iDimIndex, DimTree* ipTree);								
		///< Parse the element info inside one dim. iDimIndex : the index of hosting dim; iElementCount : the element number inside that dim 
		bool hParseElementString(const MBase::String& irString, Int32 iDimIndex, Int32 iElementCount, DimTree* ipTree);	
		///< Parse the info for one dim node, iDim: dim index, iIndex : node index, ipParet: parental node
		bool hPopulateOneDimNode(const MBase::String& irString, Int32 iDim, Int32 iIndex, DimNode* ipParent, DimTree* ipTree);
		///< Traverese the tree to complete the following info : offsprint count + ordinal + width + groupIndex
		bool hTraverseTreeForCompletion(DimNode* ipNode, DimTree* ipTree);
	//[End of Tree Population Methods for Generic Data Input]	
		
			
		
	private: 
		MBase::String mPageLabel;					///< Page Label - There should only 1 Page Label, so no container
		bool mShowSubtotalValue;    ///< Show Subtotal value or not
		bool mNestedLabel;			///< Nested Group Label
		bool mTrimSpace;			///< Trim leading or trailing spaces for label
		bool mIgnoreForNoSeriesOrGroup;	///< Define when data is not enough, how do we group the columns
		bool mSuppressLastLevel;	///< Suppress Last Column Level
		bool mIsConcatenateLabels;  ///< Concated Labels for DataGroup taking more than 1 columns.
		bool mIsUseZeroForNullValue;///< Replace Null data with zero
		
		Int32 mDataGroup;			///< Define how we grouping the "Columns"
		Int32 mBeginRow;			///< Current begin row
		Int32 mEndRow;				///< Current end row
		Int32 mBeginColumn;			///< Current begin Column
		Int32 mEndColumn;			///< Current end Column
        
        // [GM Partial Rendering Support]
        Int32 mOnScreenStartGroupId;
        Int32 mOnScreenEndGroupId;
		
		
	private: ///< Properties in this section are from old Dataset Component design, and they are reused in iphone Project		
		Int32 mAccessMode;							///< Access Mode : DM_TEMPLATE OR DM_GENERIC
		DimTree* mpRowTree;							///< Row tree after parsing the template
		DimTree* mpColTree;							///< Column tree after parsing the template
		std::vector<MBase::String> mRowLabels;		///< Row label string container, plain label
		std::vector<MBase::String> mColumnLabels;	///< Column label string container, plain label
		
		bool mUseManualRowLabels;
		bool mUseManualColumnLabels;
		
		Int32 mRowSize;
		Int32 mColumnSize;
	//[End of Reused & Old Properties]	
				
		
	private: ///< This Section is used to store data		
		MSynch::SmartPtr<SimpleDataset> mSimpleDatasetPtr;								  ///< the reference pointer caller would see	
		MSynch::SmartPtr<SimpleDataset> mFullDatasetPtr;								  ///< data block of the full data set 	
		std::vector<std::vector<MSynch::SmartPtr<SimpleDataset> > > mTotalCollection;	  ///< data blocks vectors at different layers (for drill, totals)	
    //[End of Data Stroage]
		
		
	private: ///< This Section is for the slicing stuff.	
		std::vector<DataPiece*> mSliceSet; ///< this is for the data slice used in IPhone Charting project.
		bool mIsUseSlice;				   ///< indicate whether user want the slice works
		bool mIsSliced;					   ///< indicate whether the data block is sliced already	
		size_t mSliceNumber;               ///< slice number
		size_t mActiveSliceIndex;		   ///< the active slice
		void hClearUpSlices();			   ///< for 1) get a new collection 2) destruction of datasetimpl
	//[End of Data Slice]	
			
	private: ///< This Section is for the shuffling stuff.
        SHUFFLE_STATUS mShuffleStatus;
        std::map<Int32,Int32> mShuffledRowIndexSet;
        std::map<Int32,Int32> mShuffledColIndexSet;
        
	private: ///< this section is for Drill Effect	
		TokenManager mTokenManager;	       ///< token manager instance
		bool mIsUseToken;				   ///< indicate if or not a token is used	
		bool hPickCorrectDataTable();      ///< used to pick the correct data table for the current token
	//[End of the Drill Support]	

	private: ///< this section is for AE integration	
		bool mHasMetrics;									///< With a metric in template  
		bool mIsMetricInRow;								///< Is Metric in row?
		std::vector<MBase::String> mMetricElements;			///< Name For Each Metric Element.

		Int32 mMetricIndexAlongAxis;						///< Metric Index along its hosting axis
		std::map<Int32, bool> mRowSubTotalsMap;				///< Row Index Is Total Or Not.
		std::map<Int32, bool> mColSubTotalsMap;	
		std::map<Int32, Int32> mRowIndexToGlobalIndex;		///< local row index --> global index (need to consider subtotal).
		std::map<Int32, Int32> mColIndexToGlobalIndex; 
		std::map<Int32, Int32> mGlobalIndexToRowIndex;		///< global row index --> local index (need to consider subtotal).
		std::map<Int32, Int32> mGlobalIndexToColIndex; 
		std::set<Int32> mTotalsInRow;						///< rows that have total from iterator
		std::set<Int32> mTotalsInCol;						///< cols that have total from iterator
		
		Int32 hMapLocalTableIndexToGlobalIndex(Int32 iIndex, bool iIsForRow);
		//[End of AE integration]	


	private: ///< Refactory into a lighter component	
#ifndef _VE_CE		
		DSSTemplate* mpTemplate;
		DSSRWGridIterator* mpGridIterator;			//< Iterator with the data table that is already sliced by group-by.
		DSSRWGridIterator* mpGlobalIterator;		//< Iterator with the whole data table. Used in Colorby support.
#endif		
		Int32 mSliceID;							//< SliceId for group-by
		Int32 mTableRowCount;
		Int32 mTableColCount;
		
		bool mIsRowHeaderEmpty;
		bool mIsColHeaderEmpty;
		bool mSeriesByColumn;				///< SeriesByColumn flag
		bool mIsDataBackedUp;				///< This option goes against the spirit to lighten Dataset, use it only for small slice please.
		bool mIsReformed;					///< Used in GM for line/Bar chart & series is by row.
		MetricUnitInfo::Ptr mMetricPtr;
#ifndef _VE_CE
		DataTableReformer::Ptr mDataTableReformerPtr;	///< Data structe to regulate reformed data table.
#endif
		DataGroupInfo mDataGroupInfo;
        std::vector<int> mAxisInfo;
        std::vector<Int32> mSeriesIndexPieces;
        std::vector<Int32> mSeriesPiecesStartIndex;
        Int32 mPieGroupDepth;
        
        Int32 mBalancedGroupCount;
        
		/**
		 Check whether the axis/header is empty for one specific data modal. For Sliced DataModal, we have only sliced iterator, but no sliced template, so we have to use iterator to get all the info we need
		 @param iIsForRow : row axis or column axis
		 @param iAccessMode: data modal type
		 @return true means axis is empty and false means opposite.
		 */
		bool hIsHeaderEmpty(bool iIsForRow = true, Int32 iAccessMode = DM_TEMPLATE);
		void hPopulateMetricInfo();

	public: ///< Refactory into a lighter component
		/** Init Function for widgets. 
		@param ipTemplate: the whole template. [We may want to drop it in future]
		@param ipGridIterator: the iterator used.
		@param iSliceID: the index of the sliced block.
		@param iIsCacheData: whether to copy data over to this class.
		*/
#ifndef _VE_CE
		virtual HRESULT InitFromTemplateSlice(DSSTemplate* ipTemplate, DSSRWGridIterator* ipGridIterator, Int32 iSliceID, bool iIsCacheData = false, bool iIncludeSubtotal = true);
#endif
		
		/** Check whether header is empty with no units. See: hIsHeaderEmpty
		*/
		virtual bool IsHeaderEmpty(bool iIsForRow = true);	
		
		/** Get Avg Value. see:GetAggregationResult
		*/
		virtual bool GetAvgValue(double& orValue, Int32 iIndex, Int32 iColumnOffset = 0, bool iIsForSeries = true, bool iIsForSlice = false);
	
		/** Set DimTree.
		@param ipTree. Tree to be set to this dataset.
		@iIsForRowTree. Whether the tree to be set is row or column.
		*/
		virtual void SetTreeInstance(DimTree* ipTree, bool iIsForRowTree);
		
		/**Used in Widgets. Get the sliced Id. Note: there is no real sliced template.
		@return the sliced id for one template.
		*/
		virtual Int32 GetAESliceId();
		
		/** Aggregation support, used by widgets. 
		@param orValue: the aggregated result.
		@param index, the index series/group.
		@param iIsForSeries, indicating whether this operation is on sereis/group
		@iAggType, the operation type
		@return, how many data points have been used in this operation.
		*/
		virtual Int32 GetAggregationResult(double& orValue, Int32 iIndex, bool iIsForSeries = true,  AGG_TYPE iAggType = AGG_TYPE_AVG); //return valide data cell.
		
		/**[obsolete, to delete with gridPlotManager] Return the width of a unit.
		 */
		//virtual	Int32 GetUnitWidth(Int32 iIndex, bool iIsInRow);
		
		/**Check the status of the CE dataset.
		 @param iIsGraph, obsolete, is used for graph rendering or not.
		 @param iIsDummyGrid: is the dataset associated with a dummy grid chart, dummy grid chart is a grid chart whose template has no metrics, used in GraphMatrix.
		 @return the status of the dataset.
		 */
		virtual DATASET_STATUS GetStatus(bool iIsGraph, bool iIsDummyGridChart);
		
		/**ColorBy support. Map an local color index to a global one.
		@return the global color index, currently it is counted row-first.	
		 */
		virtual Int32 GetColorStyleIndex(Int32 iLocalIndex);

		/**ColorBy support. This method first convert seriesid/groupid to row/col oridianl and return the corrspoinding color index for that position.
		@return the global color index, currently it is counted row-first.	
		 */
		virtual Int32 GetColorStyleIndex(Int32 iSeriesID, Int32 iGroupID, Int32 iColumnOffset = 0, bool iIsConsiderSubtotal = true);
		
        /**
         Colorby support, for GraphMatrix. This method first convert seriesid/groupid to row/col oridianl and find the corrspoinding row/col unit name for that position, and return colorMap key(string).
         */
        virtual std::string GetGMColorByKey(std::vector<std::pair<Int32, Int32> > iColorByUnitsIndex, Int32 iSeriesID, Int32 iGroupID, Int32 iColumnOffset = 0, bool iIsConsiderSubtotal = true);
        
		/**ColorBy support. This method first convert seriesid/groupid to row/col oridianl and find the corrspoinding color index for that position.
		 then return the label string for that index.
		 @return the label string for one color index, currently it is counted row-first.	
		 */
		virtual MBase::String GetColorStyleLabel(Int32 iSeriesID, Int32 iGroupID, Int32 iColumnOffset, bool iIsConsiderSubtotal = true);

		/**ColorBy support. 
		 @return the total number of color by styles.	
		 */
		virtual Int32 GetColorStyleNumber();
		
		/**ColorBy support, to set the slicing unit to back end modal.
		 @param irRowUnits, the slicing units on row axis
		 @param irColUnits, the slicing units on col axis
		 */
		virtual void SetColorSlices(std::vector<Int32>& irRowUnits, std::vector<Int32> irColumnUnits); 
		
		/**ColorBy support.Return the label string for that index. Note This one should be the local index.
		 @return the indexed label string for one color.
		 */
		virtual MBase::String GetColorStyleLabel(Int32 iLocalColorIndex);
#ifndef _VE_CE
		/**Set Global Iterator , used in color by
		@param ipIterator, the global iterator	
		 */
		virtual void SetGlobalIterator(DSSRWGridIterator* ipIterator);	
#endif		
        virtual	void GetSliceLabel(Int32 iOrdinal, bool iIsForRow, MBase::String& orCollection);  //< Data Label for GM in Pie chart, show Text
        virtual	void GetTextDataLabel(Int32 iGroupId, Int32 iSeriesId, std::string& orCollection, Int32 iAngleId = -3, Int32 iSliceId = -3);
        virtual void GetTextDataLabel(Int32 iOrdinal, std::string& orCollection);
        virtual	void GetBreakByLabel(Int32 iOrdinal, bool iIsForRow, MBase::String& orCollection);  //< Data Label for GM, show Text
        virtual	void GetDataTextLabel(Int32 iOrdinal, bool iIsForRow, MBase::String& orCollection, int iDepth);
		virtual void GetLabelCollection(Int32 iOrdinal, bool iIsForRow, std::vector<std::string>&orCollection);	
#ifndef _VE_CE
        virtual DataTableReformer* GetDataReformer() const;
#endif
        Int32 GetBalancedCount(bool iIsForGroup);
        Int32 GetMetricLabelCount();
		virtual void GetUnitNameCollection(bool iIsForRow, std::vector<std::string>&orCollection);
#ifndef _VE_CE
		virtual bool PopulateReformer(DSSRWGridIterator* ipGridIterator, DataSetReformInfo* ipReformInfo);
#endif
		virtual bool IsReformed();
        virtual const DataGroupInfo* GetDataGroupInfo(); 
        virtual void SetDatagroupInfo(const DataGroupInfo& irInfo);
        virtual const std::vector<int> GetAxisInfo();
        virtual void SetAxisInfo(const std::vector<int> &irAxisInfo);
        virtual void GetSeriesSplitVector(std::vector<Int32>& orVec);
        virtual void SetSeriesSplitVector(const std::vector<Int32>& irVec);
        virtual void SetPieGroupDetph(Int32 irDetph);
        virtual void GetPieGroupDetph(Int32 &orDepth);
        virtual Int32 GetSeriesPieceStartIndex(Int32 iIndex);
        virtual void DumpDataSet(bool iIsNaive, bool isGlobalIterator = false);

	private: 
		/** For Widgets Initiate the internal data table
		 */
		void hPopulateCETable();		
		
		/* For Widgets Get data from the internal data table.
		 */
		void hGetDataFromAETable(const Int32 iRowIndex, const Int32 iColumnIndex, double*& orData);
        void hConvertToDoubleValue4Data(double*& iorData, const void * ipData, int iSizeInByte, int iDataType, int iFlag);

		/* For Widgets Get label string from the internal data table.
		 */
		//void hGetConcatenatedLabel(bool iIsRowAxis, Int32 iOrdinal, bool iIsInclusive, std::string iSeperator, MBase::String& orText, bool iIsForColorBy = false);
		
		/**Get the element combination under certain slicing units. Assume no hierachical display units are used.
		@param irSliceUnits, slicing units.
		@param orResultLabels, the result combinations
		@param iIsForRow, is the slicing and combination is done on row.
		@return if no slicing is done on the specified axis.		
		*/
		bool hGetLabelCombination(const std::vector<Int32>& irSliceUnits, std::vector<MBase::String>& orResultLabels, bool iIsForRow);
#ifndef _VE_CE
        /** Get the string representation of one form under the template unit.
         @param ipUnit, the hosting tempalte unit.
         @param iAxisId, which axis this unit rests on.
         @param iOrdinal, the oridinal to query at.
         @param iDepth, the index of the unit
         @param iFormId, the form index
         @param iFormType, the form type.
         @param iLinkType, which part to get if it is a link form
         @return the string representation for one form
         */
		std::string hGetAttrElementPieceFromOneFrom(DSSTemplateUnit* ipUnit, Int32 iAxisId, Int32 iOrdinal, Int32 iDepth, Int32 iFormId, Int32 iFormType, LINK_TEXT_TYPE iLinkType = LINK_TEXT_DISPLAY);
		std::string hGetAttrElementPieceFromOneFromForGM(DSSTemplateUnit* ipUnit, Int32 iAxisId, Int32 iOrdinal, Int32 iDepth, Int32 iFormId, Int32 iFormType, LINK_TEXT_TYPE iLinkType = LINK_TEXT_DISPLAY);
#endif
        /**Gather all the info needed for colorby in one slice.
		1. color number 2. local color index mapped to global index.	
		*/
		void hBuildColorSliceInfo();

        // use static is not thread safe, 890344
		static double sDataCellValue;											//< We use its address to get data.
		static double sDataCellColor;											//< We use its address to get fill color for each cell.
		std::map<_PositionIndex, _PositionAssociatedInfo> mDataCellInfoPool;	//< Data cell info for easier use position -> info
		DATASET_STATUS mStatus;													//< Dataset status.
		std::vector<Int32> mRowColorByUnits;									//< ColorBy units on Row axis
		std::vector<Int32> mColColorByUnits;									//< ColorBy units on Col axis
		std::map<Int32, MBase::String> mColorByLabelMap;						//< Map: color by index to one label string.
		std::map<Int32, Int32> mColorIndexMap;									//< Map: local color Index to global color index
		Int32 mColorNumber;														//< The availible color in one slice.
		/**
		 Retrieve data label from data modal side
		 @param iIsRowAxis, specify which is the target axis row =0 col = 1
		 @param iOrdinal, position on that axis
		 @param iIsInclusive, should we concatenate all the forms inside one attribute
		 @param iSeperator, seperator.
		 @param orText, result
		 @param iOption, specify how the label piece should be picked. For tooltip.
		 */
		void hGetConcatenatedLabel(bool iIsRowAxis, Int32 iOrdinal, bool iIsInclusive, std::string iSeperator, MBase::String& orText, LABEL_LEVEL_OPTION iOption = LLO_NORMAL, bool iIsForColorBy = false);
	//[Max /Min Row Col to display starts]	
	public:
		virtual bool NextSliceToDisplay(bool iIsPrevious, bool iIsStaring = false);
		virtual Int32 CurrentSliceNumberToDisplay();
		virtual void CurrentSlicePosition(Int32& orRowIndex, Int32& orColIndex);
		virtual void SeriesGroupIndexToRowColumnIndex(Int32 iSeriesID, Int32 iGroupID, Int32& orRowIndex, Int32& orColumnIndex, Int32 iColumnOffset, Int32 iAngleId = -3, Int32 iSliceId = -3, bool iIsConsiderSubtotal = true);
        virtual void GetRowAndColumnIndexForMinMaxMetrixIndex(bool iIsForMax, std::vector<Int32> iMetricIndex, Int32& orRowIndex, Int32& orColumnIndex);
		virtual void SetFormatManager(FormatManager* ipFormatManager);
		virtual bool IsDisplaySliceInvalid();
		virtual bool GetSeriesGroupIdsFromIndex(std::vector<std::pair<Int32, Int32> >& iorSGPairs, const std::vector<Int32>& irRowIndexSet, const std::vector<Int32>& irColIndexSet);	
		virtual std::string GetLabelInDrillPath(bool iIsByRow, Int32 iIndex, const std::string& irCandiate, bool iIncludeAllForms = false);		

        //void DumpDataSet(bool iIsNaive, bool isGlobalIterator);
        bool IsEmpty();
        inline void setPropertyTreatEmptyAsEmpty(bool iValue) {mTreatEmptyAsEmpty = iValue;};
        
	private:
		bool mReverseGroup;			///< Reverse the order of group
		bool mReverseSeries;		///< Reverse the order of series
		bool mIsForHistogram;

        bool mTreatEmptyAsEmpty;    // DE32547
        
	//[Max /Min Row Col to display]	
		Int32 mTotalBlocksToDisplay;
		Int32 mCurrentDisplayBlockIndex;
		Int32 mMaxRowToDisplay;
		Int32 mMaxColToDisplay;
		Int32 mDisplayBlocksPerRow;
		Int32 mDisplayBlocksPerCol;
		Int32 mLeftoverOnCol;
		Int32 mLeftoverOnRow;
		bool mIsUseMaxGroupAsMinBound;   //reuse the max group value, but as the minimal bound.
		bool mIsDisplaySliceInvalid;
		FormatManager::Ptr mpFormatManager;
		
		bool hGetSeriesCroupRangeForOneDisplayBlock(Int32 iDisplayBlockIndex);	
		bool hPopulateDisplayBlocks(bool iIsRowHeaderEmpty, bool iIsColHeaderEmpty);
#ifndef _VE_CE
		bool hGetGradientPropertyForThreshold(DSSFormat* ipFormat, CComVariant* orValue);
#endif
		bool hIsInEmptyDataZone(Int32 iGroupID);
		bool hIsSupprestLastLevelLabel();
		bool hIsConcatedLabels();
		bool hIsUseZeroForNullValue();
		CComVariant* hGetOneGraphPropertyFromTemplate(const std::string& irPropertyName);

	private: 
		std::map<std::pair<Int32, Int32>, double> mPerfMapFormatIndexToColorValue;
		bool hGetColorValueFromFormatIndexMap(const std::pair<Int32, Int32>& iKey, double& orValue);	
		void hSetColorValueFromFormatIndexMap(const std::pair<Int32, Int32>& iKey, double orValue);
		/**
		 Get the concatenated label when the property Concatenate Label is true.
		 @parm iorLabel the final result
		 @parm iStartIndex, current label index into the data group
		 @parm iSpan, the left over which means mDataGroup - offset.
		 @parm where it is for row or for column labels.
		 */
		void hConcatenateLabel(MBase::String& iorLabel, Int32 iStartIndex, Int32 iSpan, bool iIsForRow);
		
		Int32 hRowSize()
		{
			return mUseManualRowLabels ?(Int32)mRowLabels.size() : mRowSize;
		}
		Int32 hColumnSize()
		{
			return mUseManualColumnLabels ? (Int32)mColumnLabels.size() : mColumnSize;
		}
		Int32 hGetSize(bool iIsForRow)
		{
			return iIsForRow ? hRowSize() : hColumnSize();
		}
		MBase::String hGetRowLabel(Int32 iIndex)
		{
			if (mUseManualRowLabels) {
				return mRowLabels[iIndex];
			}
			else {
				return mpRowTree->GetPlainLabels(iIndex, mSuppressLastLevel);
			}
		}
		MBase::String hGetColumnLabel(Int32 iIndex)
		{
			if (mUseManualColumnLabels) {
				return mColumnLabels[iIndex];
			}
			else {
				return mpColTree->GetPlainLabels(iIndex, mSuppressLastLevel);
			}
		}
		
		
		Int32 hCheckIndexValidity(Int32 iIndex, Int32 iIndexSetSize);
		Int32 hGetCoveredIndexByCurrentBlock(bool iIsByRow);
		/**
		 Get label, this should be the final step in label retrieve when rendering CE charts. It handls two major cases according to 
		 last parameter 1. Normal Logic in CE 2. Specail treatment for tooptip label. For(2), subcases can be divided.
		 @param iIsForRow, whether it is a row-related label or col-related label
		 @param iLabelIndex, the index into the label set, we may get it from mRowLabel or mRowTree (mColLabel or mColTree).
		 @param iColumnOffset, the index into the data group
		 @param iOption, how the label should be retrieved.
		 @param iNeedConcated, whether we should concatenate labels across data group.
		 */
		MBase::String hGetLabel(bool iIsForRow, Int32 iLabelIndex, Int32 iColumnOffset, LABEL_LEVEL_OPTION iOption, bool iNeedConcated = false);
	
//===============[thresh hold fill style]============================
	private:
		
		THRESHOLD_FILL_STYLE mCurrentThresholdStyle;
		bool hGetDrillLabelsFromIterator(bool iIsByRow, Int32 iOrdinal, const std::string& irSeperator, std::string& iorLabels, bool iIncludeAllForms = false); 
		
		double hGetThresholdBackgroundColor(Int32 iMetrixId, Int32 iThresholdId, double iDefaultColorValue);
        
		void hGetTotalPositonsFromIterator();
        bool hHasChosenControlsOnAxis(int iAxis);
    public:
        void PopulateOneMetricThresholdSuite(Int32 iRow, Int32 iCol, Int32 iMetricId, Int32 iThresholdId);
        void ResetCurrentThresholdStyle();
        bool GetMetricAndTresholdID(Int32 iSeriesId, Int32 iGroupId, Int32 iColumnOffset, Int32& orMetricId, Int32& orThresholdId);
        void* GetThresholdWithId(Int32 iMetricId, Int32 iThresholdId);
//===============for code seperation (DatasetImplHelper)==============
    public:
        int GetGlobalIteratorElementValueAt(int iAxis, int iOrdinal, int iDepth, int iBaseForm, const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag);
#ifndef _VE_CE
		DSSRWGridIterator* GetGridIterator();
#endif
        bool ShowSubtotalValue();
        void GetRowSubTotalsMap(std::map<Int32, bool> &orRowSubTotalsMap);
        void GetColSubTotalsMap(std::map<Int32, bool> &orColSubTotalsMap);
        bool IsUseZeroForNullValue();
        void AddDataToSimpleDataset(const Int32 iRowIndex, const Int32 iColumnIndex, const double* irValue);
        Int32 GetAccessMode();
        void SetCurrentThresholdSytle(THRESHOLD_FILL_STYLE iCurrentThresholdStyle);
    private:
#ifdef _VE_CE
#else
        DatasetImplHelper *mpDatasetImplHelper;
#endif
	};
    
    inline bool DatasetImpl::ShowSubtotalValue()
    {
        return mShowSubtotalValue;
    }
    inline void DatasetImpl::GetRowSubTotalsMap(std::map<Int32, bool> &orRowSubTotalsMap)
    {
        orRowSubTotalsMap = mRowSubTotalsMap;
    }
    inline void DatasetImpl::GetColSubTotalsMap(std::map<Int32, bool> &orColSubTotalsMap)
    {
        orColSubTotalsMap = mColSubTotalsMap;
    }
    inline bool DatasetImpl::IsUseZeroForNullValue()
    {
        return mIsUseZeroForNullValue;
    }
    inline void DatasetImpl::AddDataToSimpleDataset(const Int32 iRowIndex, const Int32 iColumnIndex, const double* irValue)
    {
        mSimpleDatasetPtr->AddData(iRowIndex, iColumnIndex, irValue);
    }
    inline Int32 DatasetImpl::GetAccessMode()
    {
        return mAccessMode;
    }
}

#endif



