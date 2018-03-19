//==============================================================================================
//	FILENAME	:	CategoryAxis.h
//	AUTHOR		:	Fangyao Ma, Liang Liu
//	CREATION	:	2008-05-15
//	Copyright (C) MicroStrategy Incorporated 2008
//	All Rights Reserved
//==============================================================================================
#ifndef MsiChart_CategoryAxis_h
#define MsiChart_CategoryAxis_h
#include "Axis.h"
#include "../Dataset.h"
#include "FormatFont.h"
#include "GraphCollectionObject.h"
#include "TextObject.h"

namespace MsiChart
{
	//Info to construct a categoryaxis
	struct CategoryAxisInfo
	{
		TripleId mId;
		unsigned char mAxisLabelsLocation; 
		unsigned char mAxisDirection;	
		
		CategoryAxisInfo()
		{
			mId = TripleId(DssGraphO1Body, 0, -3);
			mAxisDirection = AD_X;
			mAxisLabelsLocation = ALL_BOTTOM;
		}
		
		void ToX(bool iIsSeriesAxis = false)
		{
			Int32 lObjId = iIsSeriesAxis ? DssGraphO2Body : DssGraphO1Body;
			mId = TripleId(lObjId, 0, -3);
			mAxisDirection = AD_X;
			mAxisLabelsLocation = ALL_BOTTOM;
		}
		
		void ToY(bool iIsSeriesAxis = false)
		{
			Int32 lObjId = iIsSeriesAxis ? DssGraphO2Body : DssGraphO1Body;
			mId = TripleId(lObjId, 0, -3); //maybe some other ID?
			mAxisDirection = AD_Y;
			mAxisLabelsLocation = ALL_LEFT;
		}

	};

	
	typedef enum
	{
		OID_LABEL = 0,
		OID_MAJOR_TICKS = 1,
		OID_MINOR_TICKS = 2
	}EnumCategoryAxisOidItem;
	
	
	///	Enumeration for Category Label Style
	enum EnumCategoryLabelStyle
	{
		CLS_AUTOMATIC = 0,
		CLS_NORMAL,		// ONE LINE
		CLS_STAGGER,	// STAGGERED
		CLS_MANUAL,		// MANUAL SKIPED
		CLS_NO_STAGGER, // NOT IN THE STAGGER MODE, THERE IS USED IN IPAD GRAPHMATRIX.
	};

	struct CategoryAxisOption
	{		
		CategoryAxisOption(): mLabelStyle(CLS_AUTOMATIC),
								mManualSkip(0)
		{}
					
		unsigned char mLabelStyle;	///< See EnumCategoryLabelStyle
		unsigned int mManualSkip;	///< Manual skip count
	};

	/**
	 *	Category Axis which has no numeric data
	 */
	class CategoryAxis:public Axis
	{
	public:

		///	Type definition
		typedef	MSynch::SmartPtr<CategoryAxis>	Ptr;

		/**
		 *	Constructor and Destructor
		 *	@param	iTripleId TripleId
		 *	@param  ipManager pointer to the manager
		 *	@param	iAxisLabelsLocation labels' location
		 *	@param  iAxisDirection Axis's direction AD_X: horizontal, AD_Y: vertical
		 */
		CategoryAxis(const TripleId& iTripleId,  
						GraphObjectManager* ipManager,
						unsigned char iAxisLabelsLocation = ALL_BOTTOM, 
						unsigned char iAxisDirection = AD_X); 
		/**
		 *	Destructor
		 */
		virtual ~CategoryAxis();
		
		/**
		 *	Adjust the device data area, trim space for title.
		 *	@param iorDeviceDataArea Data area will be trimed by CategoryAxis
		 */
		void AdjustTitleArea(Rect2D& iorDeviceDataArea);
		/**
		Adjust device data area, trim space for label, ticks, etc.
		@param iorDataArea Data area will be trimed by CategoryAxis.
		@param iLowMargin Specifies the existing margin on the low end. Here, "Low" means bottom or left.
		@param iHighMargin Specifies the existing margin on the high end. Here, "High" means top or right.
		@param iAreFixedMargins Specifies whether the margins can be changed or not.
		*/
		void AdjustDataArea(Rect2D& iorDataArea, Int32 iLowMargin = 1000, Int32 iHighMargin = 1000, bool iAreFixedMargins = false); 
		/**
		Adjust device data area, trim space for label, ticks, etc.
		@param iorDeviceDataArea Data area will be trimed by CategoryAxis.
		@param iLabelStyle The label style which we tries to apply.
		@param iForceUse Indicate whether to ignore iLabelStyle or not. If iForceUse is true, ignore iLabelStyle.
		@param iLowMargin Specifies the existing margin on the low end. Here, "Low" means bottom or left.
		@param iHighMargin Specifies the existing margin on the high end. Here, "High" means top or right.
		*/
		//bool AdjustDeviceDataArea(Rect2D& iorDeviceDataArea, EnumCategoryLabelStyle iLabelStyle, bool iForceUse = false, Int32 iLowMargin = 10, Int32 iHighMargin = 10); 
		virtual void GenerateMapAndList(); 
		virtual void GetOptions(Int32 iSeriesId, Int32 iPropertyId, CComVariant* opValue) const;


		//	Method used by renderer
		/**
		 *	Set the data set pointer
		 *	@iDatasetPtr Pointer to Dataset
		 */
		void SetDataset(Dataset::Ptr iDatasetPtr);
		/**
		 *	return the group middle position for the specified group index
		 *	@param iGroupIndex Group index
		 *	@return group middle position along the axis
		 */
		Int32 GetGroupMiddlePos(float iGroupIndex) const;
        Int32 GetGroupMiddlePos(float iGroupIndex, Int32 iGroupCount) const;
        Int32 GetSubGroupMiddlePos(float iGroupIndex, Int32 iSubIndex, Int32 iSectionCount) const;
        double GetPreciseGroupMiddlePos(float iGroupIndex) const;
        
        /**
        //
         *Special case of category-pie plot,need to set the GroupCount
         */
        void SetGroupCount(Int32 iGroupCount);
        Int32 GetGroupCount() const;
        bool IsMetricsOnlyOnAxis() const;
        
		/**
		 *	return the group start position for the specified group index
		 *	@param iGroupIndex Group index
		 *	@return group start position along the axis
		 */
		double GetGroupStartPos(double iGroupIndex) const;
        double GetGroupStartPos(double iGroupIndex, Int32 iGroupCount) const;

		/**
		 *	return the group size
		 *	@return group size
		 */
		double GetGroupSize() const;

		/** 
		 *	Set the "Value Axis Cross Between CategoryAxis" flag
		 *	@param iAxisCross: VA cross CA
		 */
		void SetAxisCross(bool iAxisCross);

		/** 
		 *	Get the "Value Axis Cross Between CategoryAxis" flag
		 *	@return VA cross CA or not
		 */
		bool GetAxisCross();

		/**
		Get logical coordinate by Group ID (for Category Axis) or Series ID (for Series Axis).
		@iGroupId Specifies a group ID or series ID.
		@return Return the logical coordinate which corresponds to iGroupId.
		*/
		double GetLogicalCoordById(Int32 iGroupId) const;		

		virtual double GetLogicalCoord(Int32 iAbsoluteCoord);	
		virtual float GetAbsoluteCoord(double iLogicalCoord);
		virtual void GetMinMaxLogicalCoord(double& orMin, double& orMax) const;
		virtual double GetSlopeFactor(double iLogicalCoord) const;		
		virtual void GenerateLabelsFor3D(const std::vector<Point2D>& irGridPoints, const DVector2D& irUnitOffsetDirection,  
										 GraphCollectionObject* ipLabelCollectioin, std::vector<GraphObject::Ptr>& iorGraphObjectList);
		virtual void GetGridLinePositionsFor3D(std::vector<double>& orGridLinePositions, EnumGraphObjectType iGraphObjectType) const;
		
		virtual void OnZooming(Int32 iOldStart, Int32 iOldEnd, Int32 iNewStart, Int32 iNewEnd);
		virtual double GetPositionAtTick(double iTickValue);
 		virtual double GetIntervalSpace();
		virtual Int32 LayoutAxis(bool iIsRotated, Int32& orMaxLabelSpace);	

		/**
		 Handles zooming by applying a new group range.
		 @param iStartGroup,iEndGroup Specifies a new group range.
		 */
		void OnZoomingByGroupRange(double iStartGroup, double iEndGroup);
		
		/**
		 Handles Swiping along category axis out of zoom state.		
		 */
		virtual void OnSwipingOutOfZoom();
		
		/// Restore from zoom state.
		void OnZoomRestore();
		
		/**
		Set group order for Pareto chart.
		@irGroupOrder Indicates the group order in Pareto chart.
		*/
		void SetGroupOrderForPareto(const std::vector<Int32>& irGroupOrder);

		/**
		Generate grid lines and interlaced grids on middle wall in BiPolar chart with depth.
		@param irY2DataArea Indicates the data area for Y2 axis.
		@param iorGraphObjectList The generated graph objects will be appended to this list.
		*/
		void GenerateGridsOnMiddleWall(const Rect2D& irY2DataArea, std::vector<GraphObject::Ptr>& iorGraphObjectList);
		
		/**
		 Get group label by group ID.
		 @iGroupId Specifies a group ID in dataset slice context.
		 @orGroupLabel Output a group label.
		 @param iSeparator Specifies the separator between adjacent elements. 
		 Only useful when tree structure is available in dataset.
		 @param iIsAbbreviated Indicate whether the group label should be abbreviated. 
		 For non-date labels, ignore this parameter  and output full format label.
		 Only useful when tree structure is available in dataset.
		 */
		void GetGroupLabel(Int32 iGroupId, MBase::String& orGroupLabel, const wchar_t iSeparator, bool iIsAbbreviated) const;
		
		/** Get total group count in Dataset. Used by time series chart.
		 @return Return total group count in Dataset.		 
		 */
		int GetTotalGroupCount();
		
		
		virtual void TrimAxisPadding(Rect2D& iorRect, bool iUseFixedMargin = true);
		virtual Int32 GetUniformAxisExtraPadding();
        virtual void GetInterlacedPositions(std::vector<double>& orPositions);


	protected:
		/**
		 *	Load default properties from ChartContext
		 */
		void LoadProperties();		
		Int32 mGroupCount; ///< Number of Groups, used to generate labels.
        mutable Int32 mActualGroupCount; ///< The group count used by Plot to render objects, used in GM widget.
		Dataset::Ptr mDatasetPtr;	///< pointer to dataset
	
	private:
		virtual void hGenerateGridsByArea(bool iIsGridLine);
		
		//	Interlaced grid
		void hGenerateInterlacedSection();
		void hGetInterlacedSectionPolygon(Int32 iGroupID, std::vector<Point2D>& orPolygon);
		void hGetInterlacedSectionPoints(Int32 iGroupID, Point2D& orStart, Point2D& orEnd) const;

			
		//	Axis Line & Grid Line related
		void hGetMajorPoints(std::vector<Point2D>& orVecStart,std::vector<Point2D>& orVecEnd);
		void hGetMinorPoints(std::vector<Point2D>& orVecStart,std::vector<Point2D>& orVecEnd);
		void hGenerateMajorGrid();
		void hGenerateMinorGrid();
		void hGenerateGridLine(Int32 iObjectID, Int32 iGroupIndex, Int32 iGridStyle, 
											const Point2D& irFirst, const Point2D& irSecond,
												GraphCollectionObject* ipCollection);

        double hGetGroupMiddleOffset() const;
        double hEvaluateBarSizeForBudgetOverlapBar(double iLength) const;
        double hGetPaddingInsideBudgetingGroup(Int32 iGroupIndex, bool iIsLeftPadding, Int32& orStatus) const;

		Int32 hGetAxisLength() const;
		bool hInsideHorizontalRange(Int32 iHorizontal);
		bool hInsideVerticalRange(Int32 iVertical);

		//	Title&Label related
		bool hOnFirstPlane(EnumAxisLabelsLocation iLocation);
		TextObject::Ptr hCreateGroupLabel(const TripleId& irTripleId, const MBase::String& iStr,  float iGroupInterval, 
			bool iNeedMove, std::map<TripleId, TextObject::Ptr>& iorLabelMap, Int32 iMaxLimit = 1000);
		TextObject::Ptr hCreateGroupLabelFor3D(Int32 iGroupId, GraphCollectionObject* ipCollection, std::map<TripleId, TextObject::Ptr>& iorLabelMap,
											   const Point2D& irGridPoint, const DVector2D& irUnitOffsetDirection);
		
		void hGetLabelLocation(std::vector<EnumAxisLabelsLocation>& orLabelLocation);
		EnumAxisLabelsLocation hGetSingleLabelLocation(Int32 iIndex);		
		Int32 hGetOneSideLabelSize(Int32 iLowMargin, Int32 iHighMargin, bool iAreFixedMargins, Int32 iStartIndex = 0);
		Int32 hGetOneSideLabelSizeForTimeSeries(Int32 iLowMargin, Int32 iHighMargin, bool iAreFixedMargins);
		void hGenerateLabels(bool iIsGlobalLayout);
		void hAdjustSpace(EnumAxisLabelsLocation iLocation,Rect2D& iorDeviceDataArea, Int32 iSpace);
		void hReserveLabelSpace(Int32 iLowMargin, Int32 iHighMargin, bool iAreFixedMargins);
		bool hNeedTickLength(EnumAxisLabelsLocation iLocation);
		
		void hAdjustTextPosition();
		void hAdjustTextPositionEx();
		Int32 hGetLabelOffset(EnumAxisLabelsLocation iLocation, Int32 iOffset, DVector2D& orUnitOffsetDirection);
		Int32 hGetEstimatedInterval(const Rect2D& irFirstRect, std::map<TripleId, TextObject::Ptr>& iorLabelMap);
		Int32 hGetEstimatedIntervalFor3D(const Rect2D& irFirstRect, const std::vector<Point2D>& irGridPoints, const DVector2D& irUnitOffsetDirection, 
										 GraphCollectionObject* ipCollection, std::map<TripleId, TextObject::Ptr>& iorLabelMap);
						
		//	Dimensional labels & bracket
		void hGenerateBracket();
		bool hGetBracketLine(EnumAxisLabelsLocation iLocation,
										Rect2D& irRef, Rect2D& irChild,  bool iLeftTop,
										std::vector<Point2D>& iorPoint);
		GraphObject::Ptr hGetObjectFromTextCollection(const TripleId& iID, EnumAxisLabelsLocation iLocation);
		void hSetLabelWidthLimit(TextObject* ipLabel, float iGroupInterval, Int32 iMaxLimit = 1000);
		void hHideLabelsForFixedMargin(bool iAreFixedMargins, Int32 iLowMargin, Int32 iHighMargin, TextObject* ipFirstLabel, TextObject* ipLastLabel) const;
		void hGetExtraMarginDemand(Int32& iorLowMargin, Int32& iorHighMargin, const Rect2D& irDataArea, const Rect2D& irFirstRect, const Rect2D& irLastRect) const;
		void hCalculateLabelMargins(Int32& iorLowMargin, Int32& iorHighMargin, Int32 iLowLabelMargin, Int32 iHighLabelMargin) const;
		void hApplyMarginsOnDataArea(Int32 iLowMargin, Int32 iHighMargin);
		void hMoveLabel(const TripleId& iID, TextObject* ipLabel) const;
		//void hMoveLabelFor3D(TextObject::Ptr iLabelPtr, const Point2D& irGridPoint, const DVector2D& irUnitOffsetDirection) const;
		bool hHasOverlapWithStagger(Int32 iLowMargin, Int32 iHighMargin, bool iAreFixedMargins, const Rect2D& irBackupDataArea,
			std::vector<TextObject::Ptr>& iorVecLabels, Int32& iorMaxSpace1, Int32& iorMaxSpace2, std::map<TripleId, TextObject::Ptr>& iorLabelMap, 
			const Rect2D& irFirstRect, Int32 iLowLabelMargin, Int32 iHighLabelMargin);
		bool hOverlapWithInterval(Int32 iInterval, Int32 iStartIndex, Int32& iorMaxSpace, std::vector<TextObject::Ptr>& iorVecLabels, std::map<TripleId, TextObject::Ptr>& iorLabelMap);		
		bool hOverlapWithIntervalFor3D(GraphCollectionObject* ipCollection, Int32 iInterval, std::vector<TextObject::Ptr>& iorVecLabels,
									   const std::vector<Point2D>& irGridPoints, const DVector2D& irUnitOffsetDirection, std::map<TripleId, TextObject::Ptr>& iorLabelMap);

        void hGetAdjustedTitleArea(Rect2D& orRect);
		bool hIsSeriesAxis() const;
		Int32 hGetGroupIdByIndex(Int32 iGroupIndex) const;
		void hGetGroupLabel(Int32 iGroupIndex, MBase::String& orGroupLabel) const;
        LABEL_LEVEL_OPTION hRetrievalOptionForLabels(bool iIsGroupLabel, Int32& iIndex) const;
		void hGenerateMajorGridsOnMiddleWall();
		void hGenerateMinorGridsOnMiddleWall();
		void hGenerateInterlacedGridsOnMiddleWall();
		void hGenerateGridLinesOnMiddleWall(const std::vector<Point2D>& irGridPoints, GraphCollectionObject* ipGridLineSet);
		
		double hGetCurrentImageOffsetX() const;
		void hGetScrollViewLabelsFromChartContext();
		void hAddLabelToChartContext(TextObject* ipLabel) const;
		Int32 hLayoutDateLabels(Int32 iFirstShownGlobalGroup, Int32 iLastShownGlobalGroup, bool iIsGlobalLayout);
		Int32 hLayoutNonDateLabels(Int32 iFirstShownGlobalGroup, Int32 iLastShownGlobalGroup, bool iIsGlobalLayout);
		bool hUseLabelLevelRestriction() const;
		bool hIsLabelSetFound(const std::vector<Int32>& iCandidateGroups, Int32& iorLabelLevel, bool iIsGlobalLayout);
		bool hCheckOverlapForDateLabels(const std::vector<Int32>& iCandidateGroups, std::vector<TextObject::Ptr>& orVecLabels, bool iIsGlobalLayout);
		bool hCheckOverlapForNonDateLabels(const std::vector<Int32>& iCandidateGroups, std::vector<TextObject::Ptr>& orVecLabels, bool iIsGlobalLayout);
		void hGetGroupLabel(Int32 iGroupId, MBase::String& orGroupLabel, const wchar_t iSeparator, bool iIsAbbreviated) const;
		Int32 hGetLabelHeight();
		void hPrintLabelTree(DimNode* ipNode);
		void hCalculateGroupPosForDimLabels();
		Int32 hGetFirstShownGroupId();
        Int32 hGetLastShownGroupId();
		
        /*
         *Helper funciton to get the group middle pos for the time series chart.
         *@param iGroupIndex, the index for the speific group
         */
        double hGetTSGroupMiddlePos(float iGroupIndex) const;
        
		//Map from mTripleId plus the input parameter to a specific objectId of Label/Major/Minor Ticks
		//Now it only supports for O1/O2 Axis
		Int32 hGetObjectIdForLabelorTicks(EnumCategoryAxisOidItem iItemType) const;
		Int32 hGetPaddingOnSide() const;
		TM_LABEL_PART hTMGetLabelPartIndex(const MBase::String& irPart) const;
		Int32 mLevelSpace;		///< indicates the interval space between each level
		std::vector<Int32> mLevelSize;	///< indicates each level's size(width/height)
		std::vector<Int32> mShownGroupLabelIndices;///< Stores the group indices for the shown labels.
		std::vector<Int32> mGroupOrder;	///< Maintains the group order for pareto chart.
		
		std::vector<TextObject::Ptr> mLabelList;	///< holds all the labels for the axis		
		Int32 mLabelSubIndex;	///< indicates the label index

		MsiChart::CategoryAxisOption mCategoryOption;	///< CategoryAxis options
	
		bool mShowDimLabel;	///< Flag indicate whether to show dimensional labels
		bool mShowBracket;	///< Flag indicate whether to draw brackets between each layer
		bool mAxisCross;	///< Flag indicate whether value axis and category axis cross 	
		bool mIsDateLabel;	///< Indicates whether current category axis labels are date labels.
		bool mUseCustomizedLabelAreaHeight;		
		Int32 mCustomizedLabelAreaHeight;
		
		bool mUseManualScale;	///< Indicates whether manual scale is used.
		Int32 mBeginGroup;	///< Indicates the start group index in data set.
		double mLogicalMin;	///< Indicates the logical coordinate of axis start point.
		double mLogicalMax; ///< Indicates the logical coordinate of axis end point.	
		
		double mGroupSize;		
		Int32 mLabelInterval;	///< The inteval used between labels.
			
		bool mIsFilterMetricForSeriesLabel; ///< Indicate Whether we should filter the metric part when get labels. This is used in GraphMatrix.
		bool mIsFilterMetricForGroupLabel;	
		
		Int32 mLabelObjectId;				///< ObjectId for labels, this is associated with primary/secondary axis.

		mutable Int32 mPaddingOnSide;				///< The padding at the two ends. used in stacked bar chart of graph matrix.
        mutable Int32 mShiftOffset;                 ///< The offset to consider when deciding the group mid pos, etc.
        mutable std::pair<double, double> mLastEvaluatedBarSizeInGroup;   ///< To record the last evaluation results of the bar size, <GroupSize, BarSize>, used in overlap bar chart
        
    };

	inline bool CategoryAxis::hIsSeriesAxis() const
	{
		return mTripleId.mObjectId == DssGraphO2Body;
	}
	inline double CategoryAxis::GetLogicalCoordById(Int32 iGroupId) const
	{
		return iGroupId + 1.0;
	}
	inline Int32 CategoryAxis::hGetGroupIdByIndex(Int32 iGroupIndex) const
	{
		_ASSERTE(mGroupOrder.empty() || iGroupIndex < mGroupOrder.size());
		return mGroupOrder.empty() ? iGroupIndex : mGroupOrder[iGroupIndex];
	}

}

#endif
