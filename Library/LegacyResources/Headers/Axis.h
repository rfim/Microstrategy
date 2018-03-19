//==============================================================================================
//	FILENAME	:	Axis.h
//	AUTHOR		:	Liang Liu
//	CREATION	:	2008-05-15
//	Copyright (C) MicroStrategy Incorporated 2008
//	All Rights Reserved
//==============================================================================================
#ifndef MsiChart_Axis_h
#define MsiChart_Axis_h

#include "GraphObjectManager.h"
#include "TextObject.h"

namespace MsiChart
{
	//---------Axis Related enumerations-----------------------------------
	/// Enum for axis label location
	enum EnumAxisLabelsLocation
	{
		ALL_LEFT, 
		ALL_RIGHT, 
		ALL_LEFT_RIGHT,
		ALL_BOTTOM,
		ALL_TOP,
		ALL_BOTTOM_TOP
	}; 
	
	/// Enum for axis direction.
	enum EnumAxisDirection  
	{ 
		AD_X,	///< Horizontal, or X direction in 3D chart.
		AD_Y,	///< Vertical, or Y direction in 3D chart.
		AD_Z	///< Z direction in 3D chart.
	}; 	

	/// Enum for axis index in gAxisObjects.
	enum EnumAxisIndex
	{
		AI_O1,
		AI_O2,
		AI_X1,
		AI_Y1,
		AI_Y2,
		AI_Y3,
		AI_Y4,
		AI_Z
	}; 
	
	///Enum for GraphObject type in Axis.
	enum EnumGraphObjectType
	{
		GOT_AXIS_TITLE,
		GOT_AXIS_LABEL,
		GOT_AXIS_LINE,		
		GOT_MAJOR_GRID,
		GOT_MINOR_GRID,
		GOT_INTERLACED_GRID
	};

	//---------Axis Related structures----------------------------------
	/// General axis properties.
	struct GraphAxisGeneral
	{  
		GraphAxisGeneral(unsigned char mAxisLabelsLocation, unsigned char iAxisDirection);
		/// Indicates on which side the axis labels should be shown. Should be a value in EnumAxisLabelsLocation. 
		unsigned char mAxisLabelsLocation;	
		/// Indicates whether the axis labels should be shown in reverse order (e.g. from bottom to top, or from right to left). 
		bool mAxisDrawInReverse;
		/// Indicate whether the axis line should be shown or not.
		bool mShowAxisLine;
		/// Indicate whether the axis labels should be shown or not.
		bool mShowAxisLabels; 
		/// Indicate whether the axis title should be shown or not.
		bool mShowAxisTitle;
		/** Indicates the direction of current axis. 
		Should be a value in EnumAxisDirection. In 2D graph, only AD_X (horizontal) and AD_Y (vertical) are used. 
		AD_Z is only used in 3D graph. In 3D graph, AD_X and AD_Y may not necessary to be horizontal or vertical. 
		*/		
		unsigned char mAxisDirection;
		/// Indicate whether current axis has interlaced grids. 
		bool mUseInterlacedGrids;
        
        bool mIsValid;
	}; 

	/// Grid line properties.
	struct GraphAxisGrid
	{
		GraphAxisGrid(bool iShowGridLines, unsigned char iGridStyle);
		/// Indicates whether grid lines should be shown or not. 
		bool mShowGridLines;
		/// Indicates which style of grid lines should be shown. Should be a value in EnumGridStyle. 
		unsigned char mGridStyle; 
		/// Indicates whether manual grid interval should be used or not.
		bool mUseManualGrid;
		/// Indicates the manual grid interval.
		double mGridInterval;
	}; 	

	/// This struct is used to get the object ID of labels, major/minor grid lines or axis title from axis index.
	struct AxisObject
	{	
		Int32 mLabelId;		///< The object ID of axis label
		Int32 mMajorId;		///< The object ID of Major gridline
		Int32 mMinorId;		///< The object ID of Minor gridline
		Int32 mTitleId;		///< The object ID of axis title
		Int32 mCustomLineId;///< The object ID of custom line
	};

	/// Store the object ID of labels, major grid lines, minor grid lines or axis title.
	extern const AxisObject gAxisObjects[];
	const Int32 gLabelMargin = 4;
	const Int32 gMaxLabelLength = 42;

	/** Deals with some common options and operations of axis. 
	Generally, there are following GraphObjects in the GraphObjectList or GraphObjectMap of Axis:
	1. Axis title.
	2. Axis line.
	3. A collection of Axis labels.
	4. A collection of grid lines.
	This class is the base class of ValueAxis and CategoryAxis. 
	*/
	class Axis: public GraphObjectManager
	{
	public:
		typedef MSynch::SmartPtr<Axis> Ptr;
		/** Constructor
		@param irTriple Identifies the object. 
		@param ipManager Pointer to the GraphObjectManager who created current object. 
		@param iAxisLabelsLocation Indicates where to show axis labels. Should be a value in EnumAxisLabelsLocation. 
		@param iAxisDirection Indicates the direction of current axis. Should be a value in EnumAxisDirection. 
		*/
		Axis(const TripleId& irTripleId, GraphObjectManager* ipManager, unsigned char iAxisLabelsLocation, 
			unsigned char iAxisDirection); 
		virtual ~Axis();

		/** This function sets the data area of current axis. 
		Please make sure that this function is called before calling GenerateMapAndList().
		This function sets the data area of current axis. 
		@param irDeviceDataArea Reference to the data area of current axis. 
		*/
		void SetDeviceDataArea(const Rect2D& irDeviceDataArea); 	
		
		/** Get device data area.
		@param orDeviceDataArea Output the rectangle of device data area
		*/
		void GetDeviceDataArea(Rect2D& orDeviceDataArea); 	
		

		//---------Override functions-------------------------------------
		virtual void GetOptions(Int32 iSeriesId, Int32 iPropertyId, CComVariant* opValue) const;	
		virtual GraphObject::Ptr OnPointDetection(const Point2D& irCursor) const; 

		/** Check whether the axis labels should be shown in reverse order.
		@return Return true if the axis labels should be shown in reverse order. Otherwise, return false.
		*/
		bool IsReverseOrder();
		/** Set depth value and depth direction. If this method is called, mUseDepth with be set to true.
		@param iDepth Depth value.
		@param iDepthDirection Depth direction.
		*/
		void SetDepthAndDirection(Int32 iDepth, Int32 iDepthDirection);	
		
		/**
		Generate axis line
		@param iorGraphObjectList The axis line will be put into iorGraphObjectList.
		*/
		void GenerateAxisLine(std::vector<GraphObject::Ptr>& iorGraphObjectList);
		/**
		This method is supposed to be called by PlotManager. This method will generate Major and Minor grid lines.
		@param irDataArea Specify the data area of current chart area.
		@param AreaId Specify the ID of current chart area.
		@param iorGraphObjectList The grid lines will be put into iorGraphObjectList.
		*/		
		void GenerateGridLines(const Rect2D& irDataArea, Int32 iAreaId, std::vector<GraphObject::Ptr>& iorGraphObjectList);
		void GenerateGridLines();								//This is used by ipad Graph matrix to generate ticks.

		/**
		This method is supposed to be called by PlotManager. This method will generate interlaced grid interval.
		@param irDataArea Specify the data area of current chart area.
		@param AreaId Specify the ID of current chart area.
		@param iorGraphObjectList If depth is not used, the interlaced grids will be put into iorGraphObjectList.
		*/		
		void GenerateInterlacedGrids(const Rect2D& irDataArea, Int32 iAreaId, std::vector<GraphObject::Ptr>& iorGraphObjectList);	
        virtual void GetInterlacedPositions(std::vector<double>& orPositions);

		/**
		 *	return logical coordinate for trendline
		 *	@param iAbsoluteCoord coordinate along the axis
		 *	@return logical coordinate
		 */
		virtual double GetLogicalCoord(Int32 iAbsoluteCoord) = 0;
		
		/**
		 *	return logical coordinate for trendline
		 *	@param iLogicalCoord coordinate
		 *	@return absolute coordinate
		 */
		virtual float GetAbsoluteCoord(double iLogicalCoord) = 0;	

		/**
		Get min tick and max tick of current axis in logical coordinate.
		@param orMin Min tick.
		@param orMax Max tick.
		*/
		virtual void GetMinMaxLogicalCoord(double& orMin, double& orMax) const = 0;

		/**
		Get the slope factor (d(AbosluteVariable)/d(LogicalVariable)) at a speicified logical coordinate.
		@param iLogicalCoord Specified a logical coordinate.
		*/
		virtual double GetSlopeFactor(double iLogicalCoord) const = 0;

		/**
		Check whether current ValueAxis use logarithmic scale or not.
		@return Return true if current ValueAxis use logarithmic scale.
		*/
		virtual bool IsLogarithmic() const;


		virtual double GetPositionAtTick(double iTickValue) = 0;

		/**
		Check whether to show major grid lines or not.
		*/
		bool IsMajorGridShown() const;
		/**
		Check whether to show interlaced grids or not.
		*/
		bool IsInterlacedShown() const;
		/**
		Check whether to show axis labels or not.
		*/
		bool IsLabelsShown() const;
		/**
		Check whether to show axis title or not.
		*/
		bool IsTitleShown() const;

		/** Generate title and reserve title area
		@param iorTitleArea This area will be adjusted by subtracting title area.
		*/
		void ReserveTitleArea(Rect2D& iorTitleArea); 

		/**
		 Set axis information for 3D chart.
		 @param iAxisLength Specifies the length of current axis.
		 @param iAxisMargin Specifies the margin of current axis.
		 */
		void SetAxisInfoFor3D(Int32 iAxisLength, Int32 iAxisMargin);
		/**
		 Get axis length for 3D chart.
		 @return Return the length of current axis.
		 */
		Int32 GetAxisLengthFor3D() const;
		
		/**
		 *	Get 3D absolute coordinate from logical coordinate
		 *	@param iLogicalCoord Specifies a logical coordinate.
		 *	@return 3D absolute coordinate along current axis.
		 */
		double ValueToPositionFor3D(double iLogicalCoord) const;
		
		/**
		 Generate labels of current axis in 3D chart.
		 @param irGridPoints Specifies the grid positions to indicate where the labels should be drawn.
		 @param irUnitOffsetDirection Specifies the direction of the labels relative to the grid positions.
		 @param ipLabelCollection Specifies the label collection object.
		 @param iorGraphObjectList The labels will be appended to this list.
		 */
		virtual void GenerateLabelsFor3D(const std::vector<Point2D>& irGridPoints, const DVector2D& irUnitOffsetDirection,
										 GraphCollectionObject* ipLabelCollectioin, std::vector<GraphObject::Ptr>& iorGraphObjectList) = 0;
		
		/**
		 Generate title of current axis in 3D chart.
		 @param iObjectId Specifies the Object ID for the axis title.
		 @param irRefPoint Specifies reference point to indicate where the labels should be drawn.
		 @param irUnitOffsetDirection Specifies the direction of the labels relative to irRefPoint.
		 @param iorGraphObjectList The title will be appended to this list.
		 */
		void GenerateTitleFor3D(Int32 iObjectId, Point2D& irRefPoint, const DVector2D& irUnitOffsetDirection, std::vector<GraphObject::Ptr>& iorGraphObjectList);
		
		/**
		 Get the grid line positions of current axis in 3D chart.
		 @param orGridLinePositions Output the grid line positions.
		 @param iGraphObjectType Indicates which graph object (label, grid line or interlaced grid) are the grid positions used for.
		 This is useful for category axis.
		 */
		virtual void GetGridLinePositionsFor3D(std::vector<double>& orGridLinePositions, EnumGraphObjectType iGraphObjectType) const = 0;		




		virtual void TrimAxisPadding(Rect2D& iorRect, bool iUseFixedMargin = true);

		/**
		Get the object ID for the axis title of current axis in 3D chart.
		@param iAxisTitleId 
		@return Return the object ID of current axis title in 3D chart.
		*/
		Int32 GetAxisTitleLocationId(Int32 iAxisTitleId) const;	

		/**
		Get the object ID of interlaced grids for current axis.		
		@return Return the object ID of interlaced grids for current axis.
		*/
		Int32 GetInterlacedGridId() const;

		/**
		Init one of the following GraphCollectionObject:
			GraphCollectionObject* mpMajorGridSet;
			GraphCollectionObject* mpMinorGridSet;
			GraphCollectionObject* mpInterlacedGridSet;
			GraphCollectionObject* mpAxisTitleSet;
			GraphCollectionObject* mpAxisLabelSet;
			GraphCollectionObject* mpAxisLineSet;	
		@param irTripleId Specifies a TripleId for GraphCollectionObject.
		@param iGraphObjectType Specifies a graph object type.
		*/
		void InitObjectSet(const TripleId& irTripleId, EnumGraphObjectType iGraphObjectType);

		/**
		Get one of the following GraphCollectionObject:
			GraphCollectionObject* mpMajorGridSet;
			GraphCollectionObject* mpMinorGridSet;
			GraphCollectionObject* mpInterlacedGridSet;
			GraphCollectionObject* mpAxisTitleSet;
			GraphCollectionObject* mpAxisLabelSet;
			GraphCollectionObject* mpAxisLineSet;			
		@param iGraphObjectType Specifies a graph object type.
		@return Return a GraphCollectonObject according to iGraphObjectType;
		*/
		GraphCollectionObject* GetObjectSet(EnumGraphObjectType iGraphObjectType) const;	
		
		/**
		 Handles zooming event.
		 @param iOldStart, iOldEnd Specifies an old range.
		 @param iNewStart, iNewEnd Specifies a new range.	
		 */
		virtual void OnZooming(Int32 iOldStart, Int32 iOldEnd, Int32 iNewStart, Int32 iNewEnd) = 0;	
		
		/**
		 Only used by uniform axis in GraphMatrix.
		 Draw axis title and axis labels.
		 */
		void DrawTitleAndLabels();
		
				/**
		 Denote whether this axis is associated with a range legend
		 */
		void SetIsAxisForRangeLegend(bool iIsForRangeLegend);

		/**
		Get the interval space, for category axis, the group size; for value axis the space between 2 ticks.
		*/
		double GetIntervalSpace();
		
		/** To layout the axis in "Fit to Content" mode.
		@return the total length/height of this axis.
		@iIsXAxis, x-axis or y-axis to layout, if it is y axis, no need to consider iIsRotated, since the labels will always be horizontally placed.
		@iIsRotated, if the labels should be rotated, useful when it is x-axis.
		@orMaxLabelSpace, the max space taken by one single label, if it is rotated, this is a fixed value.
		*/
		virtual Int32 LayoutAxis(bool iIsRotated, Int32& orMaxLabelSpace) = 0;	

		//return the axis label rotation information.
		Int32 GetLabelRotation();
		
		/** Get the padding between the pass-out plotarea and the label text
		*/
		virtual Int32 GetUniformAxisExtraPadding();
		
        /** Whether this axis is a uniform axis. Setter/Getter.
         */    
        bool IsUniformAxis() const;
        void SetAsUniformAxis(bool iUniformAxis);
	protected:		
		/**
		Create axis line. This is a helper method of GenerateAxisLine().
		*/
		virtual void CreateAxisLine();
		/** Get the index in gAxisObjects from Axis ID.
		@return Axis index in gAxisObject[].
		*/
		Int32 GetIndex() const;			
		
		/**
		This method is used to reserve title area according to a reference rectangle.
		@param iorTitleArea Indicates the reference rectangle of axis title.
		@param iIsReversed If this parameter is false, iorTitleArea will be adjusted by substracting title area.
		Otherwise, it will be adjusted by adding title area.
		*/
		void ReserveTitle(Rect2D& iorTitleArea, bool iIsReversed);
		/**		
		Axis title is created in ReserveTitleArea(). But the data area may be changed later.
		The location of axis title need to be updated using this method.
		@param irTitleArea Indicates the reference rectangle of axis title.
		*/
		void UpdateTitleLocation(const Rect2D& irTitleArea);		

		/**
		This method is used to reserve depth axis line area.
		When depth is used, the data area need to be shrinked to hold the depth axis line.
		@param iorDeviceDataArea Indicates the reference rectangle for data area.
		@param iIsReversed If this parameter is false, iorDeviceDataArea will be adjusted by substracting 
		depth axis line area. Otherwise, it will be adjusted by adding depth axis line area.
		@param iBothDirection Indicate whehter to reserve depth line at both directions.
		*/
		void ReserveDepthAxisLine(Rect2D& iorDeviceDataArea, bool iIsReversed = false, bool iBothDirections = false) const;

		/**
		This is a helper method to update axis label location.
		@param iLabelLocation Should be a value in EnumAxisLabelsLocation.
		For convience, ALL_LEFT_RIGHT is treated as ALL_LEFT, and ALL_BOTTOM_TOP is treated as ALL_BOTTOM. 
		@param orOffset Output the offset of label location with consideration of depth.
		*/
		void GetLabelOffset(Int32 iLabelLocation, Point2D& orOffset) const;

		/** Check whether the axis labels need to be shown on both sides or not.
		@return Return true if the axis labels need to be shown on both sides.
		*/
		bool HasLabelsOnBothSides() const;		
		/**
		Check whether a depth axis line needs to be drawn according to a given label location. 
		@iLabelLocation Should be a value in EnumAxisLabelsLocation. 
		For convience, ALL_LEFT_RIGHT is treated as ALL_LEFT, and ALL_BOTTOM_TOP is treated as ALL_BOTTOM. 
		@return Return true if a depth axis line needs to be drawn according to iLabelLocation. Otherwise, return false.
		*/
		bool HasDepthAxisLine(Int32 iLabelLocation) const;
		/**
		Check whether current axis is vertical or not.
		@return Return true if mAxisDirection is AD_Y. Otherwise, return false.
		*/
		bool IsVerticalAxis() const;

		/**
		Get one grid line.
		@param iLocation Should be a value in EnumAxisLabelsLocation. 
		For convience, when HasTwoGridLineParts() is true, ALL_LEFT_RIGHT is treated as ALL_LEFT, 
		and ALL_BOTTOM_TOP is treated as ALL_BOTTOM. 
		@param iGridStyle Specifies the style of current grid line.
		@param iTickLenth Specifies the tick length when iGridStyle is DssGraphGridInnerTicks or DssGraphGridOuterTicks.
		@param irLeftOrBottom,irRightOrTop Specifies the intersections of current grid line with axis lines 
		assuming there is no depth.
		@param orPolyLine Output the grid line.
		*/
		void GetOneGridLine(Int32 iLocation, Int32 iGridStyle, Int32 iTickLength, 
			const Point2D& irLeftOrBottom, const Point2D& irRightOrTop, std::vector<Point2D>& orPolyLine);	
		/**
		Generate one grid line and put it into grid line collection and mGraphObjectList.
		@param irTriple Specifies the TripleId of current grid line.
		@param irPolyLine Specifies the grid line.
		@param ipGridLineSet Points to a GraphCollectionObject which indicates the grid line collection.
		*/
		void GenerateOneLineObject(const TripleId& irTripleId, const std::vector<Point2D>& irPolyLine, GraphCollectionObject* ipGridLineSet);

		/**
		Check whether current depth direction is to the right.
		@return Return true if current depth direction is to the right. Otherwise, return false.
		*/
		bool IsRightDirection() const;
		/**
		Check whether current depth direction is to the left.
		@return Return true if current depth direction is to the left. Otherwise, return false.
		*/
		bool IsLeftDirection() const;
		/**
		Check whether current depth direction is upwards.
		@return Return true if current depth direction is upwards. Otherwise, return false.
		*/
		bool IsUpDirection() const;
		/**
		Check whether current depth direction is downwards.
		@return Return true if current depth direction is downwards. Otherwise, return false.
		*/
		bool IsDownDirection() const;
		/**
		Check whether a given style of grid lines has outer ticks.
		@iGridStyle Specifies a style of grid lines.
		@return Return true if iGridStyle has outer ticks. Otherwise, return false.
		*/
		bool HasOuterTicks(Int32 iGridStyle) const;	
		/**
		Check whether a given style of grid lines will have two grid line parts when showing labels on both sides.
		@iGridStyle Specifies a style of grid lines.
		@return Return true if iGridStyle has two grid line parts when showing labels on both sides.
		Otherwise, return false.
		*/
		bool HasTwoGridLineParts(Int32 iGridStyle) const;

		/**
		This method trims the input margins for the space reserved by depth axis line.
		This method is useful for X and Category axis.
		@param iorLowMargin,iorHighMargin Specifies the margins which need to be trimed.
		*/
		void TrimMarginsForDepthAxis(Int32& iorLowMargin, Int32& iorHighMargin) const;
		/**
		Get depth margins generated by depth axis line.
		@param iorLowMargin,iorHighMargin Output the depth margins.
		*/
		void GetDepthMargin(Int32& orLowMargin, Int32& orHighMargin) const;
		/**
		Move one axis label to an appropriate position.
		@param ipLabel Specifies a label.
		@param irGridPoint Specifies a corresonding grid point for this label.
		@param irUnitOffsetDirection This is a unit vector which indicates the offset direction from irGridPoint.
		@param iLabelMargin Specifies the margin between the label and irGridPoint.
		*/
		void MoveLabel(TextObject* ipLabel, const Point2D& irGridPoint, const DVector2D& irUnitOffsetDirection, Int32 iLabelMargin = gLabelMargin) const;
		/**
		Move title for manual layout.
		@param ipAxisTitle Specifies the axis title which needs to be moved.
		*/
		void MoveTitleForManual(TextObject* ipAxisTitle) const;
		/**
		Move graph objects from mManagerGraphObjectList to iorGraphObjectList.
		@param iorGraphObjectList Manager's graphObject list.
		*/
		void MoveGraphObjectList(std::vector<GraphObject::Ptr>& iorGraphObjectList);	
		
		/**
		Load related properties of Axis from ChartContext.
		*/
		void LoadProperties();		
		
		
		
		//---------Data members----------------------------------
		GraphAxisGeneral mAxisInfo;	///< General axis properties.
		GraphAxisGrid mMajorGrid;	///< Properties of major grid lines.
		GraphAxisGrid mMinorGrid;	///< Properties of minor grid lines.
		Rect2D mDeviceDataArea;	///< Indicate the data area of current axis. 
		Int32 mTickLength;	///< Tick Length

		bool mUseDepth;		///< Indicate whether to use depth or not.
		Int32 mDepth;		///< Indicate the depth value.
		Int32 mDepthDirection;	///< Indicate the depth direction.
		Point2D mDepthOffset;	///< Indicate the depth offset.
		MBase::String mAxisTitle;	///< Axis title string		
		double mLowTrim;
		double mHighTrim;

		///< When depth is not used, the grid lines need to be put into manager's graph object list.
		std::vector<GraphObject::Ptr> mManagerGraphObjectList;		

		bool mIsAxisFor3D;
		Int32 mAxisLengthFor3D;
		Int32 mAxisMarginFor3D;
		
		GraphCollectionObject* mpMajorGridSet;
		GraphCollectionObject* mpMinorGridSet;
		GraphCollectionObject* mpInterlacedGridSet;
		GraphCollectionObject* mpAxisTitleSet;
		GraphCollectionObject* mpAxisLabelSet;
		GraphCollectionObject* mpAxisLineSet;	

		bool mIsForRangeLegend;        ///< Is this axis is used by a range legend.
        bool mIsUniformAxis;
	private:
		/**
		This method is supposed to be called by PlotManager. This method will generate Major, Minor grid lines 
		and interlaced grid interval.
		@param irDataArea Specify the data area of current chart area.
		@param iUseDepth Indicates whether depth is used or not for current chart area.
		@param iorGraphObjectList If depth is not used, the interlaced grids will be put into iorGraphObjectList.
		@param iIsGridLine If this is true, grid lines will be gererated. Otherwise, interlaced grids will be generated.
		*/		
		void hGenerateGrids(const Rect2D& irDataArea, std::vector<GraphObject::Ptr>& iorGraphObjectList, bool iIsGridLine);	
		/**
		This is a helper method	for hGenerateGrids().
		@param iIsGridLine If this is true, grid lines will be gererated. Otherwise, interlaced grids will be generated.
		*/
		virtual void hGenerateGridsByArea(bool iIsGridLine) = 0;
		
		/**
		This is a helper method	for GenerateGridsByArea().
		@param iorGrid mMajorGrid or mMinorGrid.
		*/
		void hMapGridOptions(GraphAxisGrid& iorGrid);
		/**
		Generate one axis line and add it to an axis collection if necessary.
		This method is used when there is not depth.
		@iLabelLocation Should be a value in ALL_BOTTOM, ALL_TOP, ALL_LEFT and ALL_RIGHT.
		Otherwise, this parameter should be NULL.
		*/
		void hGenerateOneAxisLine(Int32 iLabelLocation);
		/**
		Generate depth axis line.		
		*/
		void hGenerateDepthAxisLine();		
		/**
		Get the start point and end point of an axis line acoording to a given label location.
		@iLabelLocation Should be a value in ALL_BOTTOM, ALL_TOP, ALL_LEFT and ALL_RIGHT.
		@orStart,orEnd Output the start point and end point of an axis line.
		*/
		void hGetAxisLine(Int32 iLabelLocation, Point2D& orStart, Point2D& orEnd) const;	

		/**
		Get a grid line when depth is used. This is a helper method for GetOneGridLine().
		@param iLocation Should be a value in EnumAxisLabelsLocation. 
		For convience, when HasTwoGridLineParts() is true, ALL_LEFT_RIGHT is treated as ALL_LEFT, 
		and ALL_BOTTOM_TOP is treated as ALL_BOTTOM. 
		@param iGridStyle Specifies the style of current grid line.
		@param iTickLenth Specifies the tick length when iGridStyle is DssGraphGridInnerTicks or DssGraphGridOuterTicks.
		@param irLeftOrBottom,irRightOrTop Specifies the intersections of current grid line with axis lines 
		assuming there is no depth.
		@param orPolyLine Output the grid line.
		*/
		void hGetOneDepthGridLine(Int32 iLocation, Int32 iGridStyle, Int32 iTickLength, 
			const Point2D& irLeftOrBottom, const Point2D& irRightOrTop, std::vector<Point2D>& orPolyLine);
		/**
		This is a helper method for GenerateGridsByArea().
		*/
		void hUseInnerDataArea();		
	};

	inline void Axis::SetDeviceDataArea(const Rect2D& irDeviceDataArea)
	{
		mDeviceDataArea = irDeviceDataArea;
	}	
	
	inline void Axis::GetDeviceDataArea(Rect2D& orDeviceDataArea)
	{
		orDeviceDataArea = mDeviceDataArea;
	}

	inline bool Axis::IsReverseOrder()
	{
		return mAxisInfo.mAxisDrawInReverse;
	}	

	inline bool Axis::HasLabelsOnBothSides() const
	{
		return (mAxisInfo.mAxisLabelsLocation == ALL_BOTTOM_TOP || mAxisInfo.mAxisLabelsLocation == ALL_LEFT_RIGHT) && !(mChartContextPtr->IsTimeSeriesChart());
	}	

	inline bool Axis::IsVerticalAxis() const
	{
		return mAxisInfo.mAxisDirection == AD_Y;
	}

	inline bool Axis::IsRightDirection() const
	{
		return mDepthDirection < 90 || mDepthDirection > 270;
	}
	inline bool Axis::IsLeftDirection() const
	{
		return mDepthDirection > 90 && mDepthDirection < 270;
	}
	inline bool Axis::IsUpDirection() const
	{
		return mDepthDirection < 180;
	}
	inline bool Axis::IsDownDirection() const
	{
		return mDepthDirection > 180;
	}
	inline bool Axis::HasOuterTicks(Int32 iGridStyle) const
	{
		return iGridStyle == DssGraphGridOuterTicks || 
			iGridStyle == DssGraphGridBoth ||
			iGridStyle == DssGraphGridSpanningTicks;
	}
	inline bool Axis::HasTwoGridLineParts(Int32 iGridStyle) const
	{
		return HasLabelsOnBothSides() && (iGridStyle == DssGraphGridInnerTicks || 
			iGridStyle == DssGraphGridOuterTicks ||	iGridStyle == DssGraphGridSpanningTicks);
	}
	inline bool Axis::IsLogarithmic() const
	{
		return false;
	}
	inline bool Axis::IsMajorGridShown() const
	{
		return mMajorGrid.mShowGridLines;
	}
	inline bool Axis::IsInterlacedShown() const
	{
		return mAxisInfo.mUseInterlacedGrids;
	}
	inline bool Axis::IsLabelsShown() const
	{
		return mAxisInfo.mShowAxisLabels;
	}	
	inline bool Axis::IsTitleShown() const
	{
		return mAxisInfo.mShowAxisTitle;
	}
	inline void Axis::SetAxisInfoFor3D(Int32 iAxisLength, Int32 iAxisMargin)
	{
		mAxisLengthFor3D = iAxisLength;
		mAxisMarginFor3D = iAxisMargin;
	}
	inline Int32 Axis::GetAxisLengthFor3D() const
	{
		return mAxisLengthFor3D;
	}	
    inline bool Axis::IsUniformAxis() const
    {
        _ASSERTE(mChartContextPtr->IsGraphMatrix());
        return mIsUniformAxis;
    }
    inline void Axis::SetAsUniformAxis(bool iUniformAxis)
    {
        mIsUniformAxis = iUniformAxis;
    }
}
#endif
