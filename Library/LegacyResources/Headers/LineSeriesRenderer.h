//=======================================================================
//	Filename:		LineSeriesRenderer.h
//	Date:			2008-10-06
//	Authors:		Bing Huang

//	Copyright (C) 1997-2008 MicroStrategy Incorporated
//	All rights reserved
//=======================================================================

#ifndef MsiChart_LineSeriesRenderer_h
#define MsiChart_LineSeriesRenderer_h

#include "SeriesRenderer.h"
#include "DataLabelManager.h"
#ifdef _VE_CE
#include "GraphObjectAggregation.h"
#endif

namespace MsiChart
{
	struct LineSeriesOptions
	{
		typedef MSynch::SmartPtr<LineSeriesOptions> Ptr;

		LineSeriesOptions() : mShowMarkers(true),
			mMarkerShape(0), mShowStepLine(false), mMarkerSize(50)
		{}
		
		bool			mShowStepLine;
		bool			mShowMarkers;
		unsigned char	mMarkerShape;
		Int32			mMarkerSize;
	};

	class ABLPlot;
	struct ABLPlotContext;

	typedef std::pair<Point2D, Point2D> LinePair;

	class LineSeriesRenderer : public SeriesRenderer
	{
	public:
		typedef MSynch::SmartPtr<LineSeriesRenderer> Ptr;

		LineSeriesRenderer(const TripleId& irTripleId, GraphObjectManager* ipManager, Dataset::Ptr iDatasetPtr, Legend::Ptr iLegendPtr);

		//pure virtual functions from GraphObjectManager
		virtual void GenerateMapAndList();
		virtual void GetOptions(Int32 iSeriesId, Int32 iPropertyId, CComVariant* opValue) const;

		//pure virtual function from SeriesRenderer
		virtual void GenerateByValueAxis(EnumValueAxis iValueAxis, std::vector<GraphObject::Ptr>& iorGraphObjectList);
#ifndef _VE_CE
		virtual void GenerateInterpolationFrame(double iProgress, LayerObject* ipLayerObject);
#else
		virtual void GenerateInterpolationFrame(double iProgress);
#endif
		virtual bool GetToolTipData(Int32 iSeriesID, Int32 iGroupID, double& orValue, double& orPercentValue, Int32 iOffset = 0);
		
		//override from GraphObjectManager
		virtual GraphObject::Ptr OnPointDetectionEx(const Point2D& irCursor, 
													const std::vector<Int32>& irObjectIds, Int32& iorDistance, Int32 iRadius,
													std::vector<GraphObject::Ptr>& iorGraphObjectsAround, EnumGraphObjectUsage iUsage = GOU_NORMAL_HIGHLIGHT); 
		virtual void GenerateVirtualMarker();
        
        bool GetData(Int32 iInfoIndex, Int32 iGroupId, double& orValue, bool iPreValue = false, Int32 iDataGroup = 0) const;
        void AddObjectsToDataLabelManager(DataLabelManager::Ptr iDataLabelManagerPtr);
        void hCreateDataMarkerForDataLabel(bool iOnlyForDisplayedDataLable = true);
		
	protected:
		/**
		Load series level properties.
		@param iInfoIndex The index for mLineSeiresOptions.
		*/
		void LoadSeriesProperties(Int32 iInfoIndex);

		/**
		Load step line property.
		@param iInfoIndex The index for mLineSeiresOptions.
		*/
		virtual void LoadStepLine(Int32 iInfoIndex);
				
		/**
		Generate map and list for the specified series.
		Used when layout is Absolute or when there is no depth.
		It is also used when stacked/percent chart with depth but all series are step lines.
		@param iInfoIndex The index for mSeriesInfo.
		*/
		virtual void GenerateMapAndListForSeries(Int32 iInfoIndex);

		/**
		Generate map and list for the specified value axis and group.
		Used when layout is Stacked/Percent with depth.
		It is also used by GenerateByValueAxis() function.
		@param iValueAxisIndex The input value axis index.
		@param iGroupId The input group ID, iGroupId == -1 if DepthCurveLine.	
		*/
		virtual void GenerateByValueAxisAndGroup(EnumValueAxis iValueAxisIndex, Int32 iGroupId);

		/**
		Get the point set for line or step line.
		@param iInfoIndex The index for mSeriesInfo.
		@param orPointSet The output point set.
		*/
		virtual void GetLinePointSet(Int32 iInfoIndex, std::vector<std::vector<Point2D> >& orPointSet) const;

		/**
		Get line point for the specified series and group.
		This point could be interpolation point for animation.
		@param iInfoIndex The index for mSeriesInfo.
		@param iGroupId The input group Id for normal line, or group order index for pareto line. 
		@param orPoint The output point.
		@return true if successful, false if the data is NULL in Dataset.
		*/		
		bool GetLinePoint(Int32 iInfoIndex, Int32 iGroupIndex, Point2D& orPoint) const;
		
		/**
		 Get final line point for the specified series and group after animation.
		 This point is the exact point that the value in dataset correspond to.
		 @param iInfoIndex The index for mSeriesInfo.
		 @param iGroupIndex The input group Id for normal line, or group order index for pareto line. 
		 @param orPoint The output point.
		 @param iIsOldRenderer Indicates whether current LineSeriesRenderer is an old renderer or not.
		 @return true if successful, false if the data is NULL in Dataset.
		 */
		bool GetFinalLinePoint(Int32 iInfoIndex, Int32 iGroupIndex, Point2D& orLinePoint, bool iIsOldRenderer) const;

		/**
		Generate depth line object for the specified series and group.
		Used by hGenerateByValueAxisAndGroup() function.
		Because in hGenerateByValueAxisAndGroup() function we generate by groups, each line
		object will only contain two point, that is a LinePair.
		@param iInfoIndex The index for mSeriesInfo.
		@param iGroupId The input group ID.
		@param irLinePair The input point pair.
		*/
		virtual void GenerateDepthLine(Int32 iInfoIndex, Int32 iGroupId, LinePair& irLinePair);

		/**
		Generate Line Collections for all series which belong to current renderer.
		It will also add legend marker to the corresponding collection.		
		*/
		void GenerateLineCollections();
		
		/**
		 Generate marker Collections for all series which belong to current renderer if markers need to be shown.			
		 */
		void GenerateMarkerCollections();

		/**
		Create markers for specified series.
		@param iInfoIndex The index for mSeriesInfo.
		*/
		void CreateSeriesMarkers(Int32 iInfoIndex);

		/**
		Create labels for all series.
		*/
		void CreateAllSeriesLabels();
        
		/**
		Create marker object(not legend marker) for the specified series.
		We will add the marker to map but not list because we will draw the marker after 
		all the lines are drawn.
		We will add it to list after the line object is added.
		@param iInfoIndex The index for mSeriesInfo.
		@param iGroupId The input group ID.
		@param iMarkerCollectionPtr The input marker collection object.
		@param irCenterPoint The center point where we should put the marker center to.
		*/	
		void CreateMarker(Int32 iInfoIndex, Int32 iGroupId, GraphCollectionObject::Ptr iMarkerCollectionPtr, Point2D& irCenterPoint);
        
        //For Graph Matrix, we create virtual marker in method OnPointDetectionEx() when tooltip trigled.
        void CreateVirtualMarker(Int32 iInfoIndex, Int32 iGroupId, GraphCollectionObject::Ptr iMarkerCollectionPtr, Point2D& irCenterPoint);
		
		/**
		Create trend line object for the specified series.
		@param iInfoIndex The index for mSeriesInfo.
		*/	
		//void CreateTrendLines(Int32 iInfoIndex);

		/**
		Get the two boudary points for marker.
		They may be top/bottom or left/right.
		@param iInfoIndex The index for mSeriesInfo.
		@param iGroupId The input group ID.
		@param orMinPoint The minimum value point.
		@param orMaxPoint The maximum value point.
        @return false means the internal data point is invalid and true means it is OK.
        */
		bool GetMinMaxPoint(Int32 iInfoIndex, Int32 iGroupId, Point2D& orMinPoint, Point2D& orMaxPoint) const;

		/**
		Get the line point pair for specified series and group.
		For LineSeriesRenderer, it will return points for iGrouId-1 and iGroupId.
		For StepLineSeriesRenderer, it will return points for iGroupId
		@param iInfoIndex The index for mSeriesInfo.
		@param iGroupId The input group ID.
		@param orStartPoint The start point.
		@param orEndPoint The end point.
		@return If NULL data, return false, else true.
		*/
		virtual bool GetPointPair(Int32 iInfoIndex, Int32 iGroupId, Point2D& orStartPoint, Point2D& orEndPoint) const;

		/**
		Handle negative case in stacked depth chart for both line and step line.
		Called by GenerateByValueAxisAndGroup().
		@param iValueAxisIndex The input value axis index.
		@param iGroupId The input group ID, iGroupId == -1 if DepthCurveLine.	
		@param iIsStepLine Whether it is steplinerenderer
		*/
		void GenerateOrderedDepthLine(EnumValueAxis iValueAxisIndex, Int32 iGroupId, bool iIsStepLine);
						
		/**
		Get the points(x, y) for line series of Pareto Percent Chart. 
		@param iInfoIndex The index for mSeriesInfo.
		@param orPointSet Used to store the results. This is to keep the interface consistency with GetLinePointSet
		*/
		virtual bool hGetParetoLinePoints(Int32 iInfoIndex, std::vector<std::vector<Point2D> >& orPointSet) const;
		
		/**
		Get the points(x, y) for line series of Pareto Percent Chart(overloaded version).
		@param iInfoIndex The index for mSeriesInfo.
		@param orPointSet Used to store the results. 
		*/
		//virtual bool hGetParetoLinePoints(Int32 iInfoIndex, std::vector<Point2D>& orPointSet) const;

		/**
		Get the positon of one specified point for line series of pareto chart. This function also consider offscale.
		@param orPoint The result coordination.
		@return false if the data for that point is null or it's offscaled and hiden.
		*/
		virtual bool hGetFinalParetoLinePoint(Int32 iInfoIndex,  Int32 iOffsetFromGroupOrder, Point2D& orPointSet) const;
		
		/**
		Get the positon of one specified point for line series of pareto chart. This function also consider offscale.
		@param orPoint The result coordination.
		@param iIsFinal Indicate whether final point after animation is needed. 
		@return false if the data for that point is null or it's offscaled and hiden.

		*/
		virtual bool hGetParetoLinePointWithOffset(Int32 iInfoIndex, Int32 iGroupId, Point2D& orPoint, bool iIsFinal) const;
		
		/**
		Get the positon of current group id into the group drawing order for line series of pareto chart
		@param irGroupOrder The group drawing order
		@param iGroupId The target group id.
		@return the positon of current group id into the group drawing order
		*/
		Int32 hGetOffsetFromGroupOrder(const std::vector<Int32>& irGroupOrder, Int32 iGroupId) const;
		
		/**
		Get the cumulated value 'till' one specified group. is iIsUseOffset is true, than iGroupOffset is how many group to go
		otherwise it will be the groupid we search for
		@param iGroupOffsetOrId The number of gruops before the wanted one or the groupid
		@param iIsUseOffset flag to tell if the 1st parameter is offset or groupid
		@return the cumulated value for a specificed pareto line point wihtout being divided by dataset total
		*/
		double hGetCumulatedYValueForPareto(Int32 iGroupOffsetOrId, bool iIsUseOffset = true) const; 
		
		/**
		Get the values for line series of Pareto Percent Chart.mSeriesPoints[lCurrentIndex]
		@param iInfoIndex The index for mSeriesInfo.
		@param orValues Used to store the results. 
		@return true if we get the values successfully and false otherwise.
		*/
		bool hGetParetoLineValues(Int32 iInfoIndex, std::vector<double>& orValues) const;

		/**
		Create Label for line series of Pareto Percent Chart.
		@param iTextCollectionPtr collection to hold the data labels.
		*/
		void hCreateRiserLabelForPareto(GraphCollectionObject::Ptr iTextCollectionPtr);
		
		/**
		Create trend line for line series of Pareto Percent Chart.
		*/
		void hCreateLineSeriesTrendlinesForPareto();

		/**
		Get the coordination info  for line series trend line in Pareto Percent Chart.
		@param iorX the X input needed by trendline.
		@param iorY the Y input needed by trendline. 
		*/
		bool hGetParetoTrendLineCoordination(std::vector<double>& iorX, std::vector<double>& iorY) const;
		
		/**
		 Check whether current frame is the last frame in animation;
		 */
		bool hIsLastFrame() const;
		
		/**
		 Check whether current frame is the first frame in animation;
		 */
		bool hIsFirstFrame() const;
		
		/// Check whether current series type is pareto percent.
		bool hIsParetoPercent() const;

		/**
		 Used for graph matrix when group count is one. Each seires will have one data point drawn instead of return directly.
		 @param iSeriesID, the sieres id.
		 */
		void hDrawStandAlonePoint(Int32 iSeriesID);
		/**
		 Create the line object when the graph type is of normal line(non-curve, non-step). It also handles graph matrix case.
		 @param iorAlreadyInMapAndList flag for the special case where each segment should be a depth line with different fill. In such case this flag will be true, meaning that the line segment(s) have/has already been put in map and list
		 @return the created line object for normal case. 
		 */
		GraphObject::Ptr hGetLineObject(Int32 iInfoIndex, Int32 iGroupId, const std::vector<std::vector<Point2D> >& irLinePointSet, bool& iorAlreadyInMapAndList);

        /**
         Create Polyline object with step gradient fill, now, for line with sizeby and color, we handle is as one polygon, not several connect polygon.
         */
        GraphObject::Ptr hGetPolyGonObjectWithStepGradientFill(const TripleId& iLineId, const std::vector<std::vector<Point2D> >& irLinePointSet, bool& iorAlreadyInMapAndList, Int32 iDepthValue, bool iIsTrapezoid, Int32 iSeriesInfo = 0);
        
        /**
         Check polyline sizeby & colorby mode, this decide the shape and format of line
         */
        void hCheckPolylineSizeAndColorMode(Int32& iorDepthValue, bool& iorIsLineWithFill, bool& iorIsTrapezoid, bool& iorIsUnified, Int32 iSeriesIndex = -3);
        
        /**
         Create single shape object, called in method hGetLineObject
         */
        GraphObject::Ptr hGetSingleShapeObject(Int32 iInfoIndex, Int32 iGroupId, const std::vector<std::vector<Point2D> >& irLinePointSet, bool& iorAlreadyInMapAndList);
        /**
         Create single dot object, called in method hGetLineObject
         */
        GraphObject::Ptr hGetSinglePointOrIsolatedPoint(Int32 iInfoIndex, Int32 iGroupId, const std::vector<std::vector<Point2D> >& irLinePointSet, bool& iorAlreadyInMapAndList);
        /**
         Judge series shape type is single data point without line connect, such as square, circle, ring, pie, tick and so on.
         */
        bool hIsSeriesAsSingleDataPointShapeType(Int32 iInfoIndex);
        
        /**
         Create VirtualMarker for Tooltip in GM, Rectangle object with width 4.
         */
         void CreateVirtualMarkerForGMLineWithSizeby(const TripleId& irDataPointId, const Point2D &irBasePoint, Int32 iLineWidth, Rect2D &orPolygon);
        
        
        /** 
         * To get a section of points (also a polygon) from the base polygon object. now each section polygon contains 4 points.
         * @param iSectionIndex, the index of the retrieved polygon.
         * @param irBasePolyGon, the base polygon to be divided.
         * @param orPolygon, the polygon to hold the result.
         */
		void hGetSectionFromBasePolygon(Int32 iSectionIndex, const std::vector<Point2D>& irBasePolyGon, std::vector<Point2D>& orPolygon);
		
        /** 
         * This one is to use the first 2 points of irPonts to build a trapezoid.
         * @param irLineId, triple Id for this section.
         * @param irPoints, points of this section.
         * @param iLineWidth, the base edge length.
         * @param orPolygon, the result trapezoid
         */
        void hGetTrapezoidFromPoints(const TripleId& irLineId, std::vector<Point2D>& irPoints, Int32 iLineWidth, std::vector<Point2D>& orPolygon);

		/**
         * Used to build a Trapezoid in GM, it will call  overloaded version of hGetAccompanyPoints
         @param iMaxSize, the maxSize for the base edge in trapezoid.
         @param iSizeRatio, the ratio to shrink the max edge.
         */
        void hGetAccompanyPoints(const Point2D& irBasePoint, double iMaxSize, double iSizeRatio, Point2D& orPoint1, Point2D& orPoint2);

        /**
         * Used to build a line with fill effect in GM, it will call hGetAccompanyPoints
         */
        void hGetBoundPointsForLineWithFill(std::vector<Point2D>& irPolyLine, float iLineWidth, std::vector<Point2D>& orPolygon);
        
		/////////////////      E N D     ///////////////// 
		

        ////////// MicroChart DataMarker /////////////
        //void CreateMarkerForEngeDataPoint(Int32 iInfoIndex, Int32 iGroupId, GraphCollectionObject::Ptr iMarkerCollectionPtr, Point2D& irCenterPoint)
        
        ////////// Data Label Methods//////////
        /**
         Create labels for all series.
         */
        void CreateAllSeriesDataLabels();
        DataLabelManager::Ptr hInitDataLabelManager();
        bool hCalculateDLBasePointAndOffset(GMDataLabelInfo &iorDataLabel);
        void hPrepareDataLabelPositions();
       
        EnumDssGraphMatrixDataLabelHostMarkerShape hGetDataLabelHostMarkerShape(Int32 iInfoIndex);
        int hGetDLIndexInVec(Int32 iSeriesId, Int32 iGroupId);
        void hGetDLCenterOffset(Int32 iInfoIndex, Int32 iGroupIndex, Int32& iorXCenterOffset,Int32& iorYCenterOffset);
        Int32 hGetCorrectLineObjectGroupIdInSeriesPoints(Int32 iInfoIndex, Int32 irGroupId);
        //void CreateMarkerForDataLabel(Int32 iInfoIndex, Int32 iGroupId, GraphCollectionObject::Ptr iMarkerCollectionPtr, Point2D& irCenterPoint);
        
        
		/**
		 Maintains a vector of GraphCollectionObject. 
		 Each GraphCollectionObject is related to a series in LineSeriesRenderer. mLineCollections is initialized
		 in hGenerateLineCollections(). After initialized, the size of mLineCollections equals to mSeriesInfo.size().
		 */
		std::vector<GraphCollectionObject::Ptr> mLineCollections;
		
		/**
		 Maintains a vector of GraphCollectionObject. 
		 Each GraphCollectionObject is related to a series in LineSeriesRenderer. mMarkerCollections is initialized
		 in hGenerateMarkerCollections(). After initialized, the size of mLineCollections equals to mSeriesInfo.size().
		 */
		std::vector<GraphCollectionObject::Ptr> mMarkerCollections;
		
		mutable std::vector<std::vector<Point2D> > mVecSeriesPoints; ///< Maintains a vector of series points.

		std::vector<LineSeriesOptions::Ptr> mLineSeriesOptions;
        bool mIsShowDataMarkerForEdgePoint;
		Int32 mDepth;
		bool mInitialized;
		bool mIsByValueAxis;
        bool mSupportDiscretePoints;                                ///< Whether or not discrete points should be drawn.
        std::vector<std::vector<bool> > mIsVirtualMarkerCreated;
        
        bool mIsVirtualMarkerAllCreated;
        
        // Li en : GM Combination Graph, we need to draw tick in lineseriesrender
        void hCalTickWidth();
        double hGetTickThickness(Int32 iInfoIndex, Int32 iGroupId);
        double hGetTickRiserWidth(Int32 iInfoIndex, Int32 iGroupId);
        bool hPrepareTickRect(Rect2D &iorRect, const Point2D& iCenter, Int32 iInfoIndex, Int32 iGroupId);
        GraphObject::Ptr hDrawSingleTickObject(Int32 iInfoIndex, Int32 iGroupId, Point2D iCenter);
        Int32 mTickGapWidth;
        Int32 mTickWidth;
        Int32 mSpaceBetweenGroup;
        
	private:
#ifndef _VE_CE
		virtual void GenerateFrameInOnePlot(LayerObject* ipLayerObject);
#else
		virtual void GenerateFrameInOnePlot(GraphObjectAggregation* ioGraphObjectAggregation, bool iForStartFrame);
#endif
		
		std::vector<std::map<Int32, Int32> >mVecGroupIdMapPerSeries; //<in one specific series, there could be null point, each section of contineous points will make one map.
		void hPopulateGroupIdMapAtSeries(Int32 iInfoIndex);
		void hGetGroupIdMapAt(Int32 iInfoIndex);
        void hCreateLineMarkers();
        
        GMDataLabelVecs mDataLabels;
        std::vector<GraphObject::Ptr > mSinglePointObjects;
        std::vector<GraphObject::Ptr > mPolygonObjects;
        std::vector<GraphObject::Ptr > mPolyLineObjects;
        
        // DE36136, get groupIDMap for one series, and don't modify current series's groupIDMap in mVecGroupIdMapPerSeries.
        std::vector<std::map<Int32, Int32> > hGetGroupIdMapAtSeries(Int32 iInfoIndex);
	};
}

#endif
