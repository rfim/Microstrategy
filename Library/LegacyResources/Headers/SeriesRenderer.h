//=======================================================================
//	Filename:		SeriesRenderer.h
//	Date:			2008-10-06
//	Authors:		Bing Huang, Liang Liu

//	Copyright (C) 1997-2008 MicroStrategy Incorporated
//	All rights reserved
//=======================================================================

#ifndef MsiChart_SeriesRenderer_h
#define MsiChart_SeriesRenderer_h

#include "PDCHeader/PDCset"

#include "GraphCollectionObject.h"
#include "GraphObjectManager.h"
#include "Legend.h"
#include "TrendLineObject.h"
#include "Plot.h"
#include "Vector2D.h"
#include "TextObject.h"
#include "PiePlotRenderer.h"

namespace MsiChart
{
	//enumerations and structs
		
	/**
	This struct is to map series ID to a value axis.
	The mDataIndex is the data index for this series ID in specified value axis.
	*/
	struct SeriesInfo
	{
		SeriesInfo(Int32 iSeriesId, Int32 iSeriesIndex, Int32 iDataIndex, EnumValueAxis iValueAxis)
			: mSeriesId(iSeriesId),
			mSeriesIndex(iSeriesIndex),
			mDataIndex(iDataIndex),
			mValueAxis(iValueAxis)
		{}

		Int32 mSeriesId;
		Int32 mSeriesIndex;
		Int32 mDataIndex;
		EnumValueAxis mValueAxis;
		
	};
	
	//pre-declarations
	class ABLPlot;
	struct ABLPlotContext;

	class SeriesRenderer : public GraphObjectManager
	{
	public:
		typedef MSynch::SmartPtr<SeriesRenderer> Ptr;

		SeriesRenderer(const TripleId& irTripleId, GraphObjectManager* ipManager, Dataset::Ptr iDatasetPtr, Legend::Ptr iLegendPtr);
		
		virtual void GetOptions(Int32 iSeriesId, Int32 iPropertyId, CComVariant* opValue) const;

		virtual bool GetToolTipData(Int32 iSeriesID, Int32 iGroupID, double& orValue, double& orPercentValue, Int32 iOffset = 0);

		/**
		Generate map and list for series belong to specified value axis and add 
		these object to specified graph object list.
		This function is used to handle the split dual axes with depth option, in this options, we should
		generate map and list value axis by value axis.
		@param iValueAxis The input value aixs index.
		@param iorGraphObjectList The input and output list.
		*/
		virtual void GenerateByValueAxis(EnumValueAxis iValueAxis, std::vector<GraphObject::Ptr>& iorGraphObjectList) = 0;

		/**
		Add a series to the renderer.
		@param iSeriesId The input series ID.
		@param iSeriesIndex The input series index.
		@param iInfoIndex The input data index.
		@param iValueAxis The input value aixs index.
		*/
		void AddSeries(Int32 iSeriesId, Int32 iSeriesIndex, Int32 iDataIndex, EnumValueAxis iValueAxis);
		
		/**
		Update the data for the stacked and percent layout for specified value axis.
		Also it will initialized "mHasNegativeValue.
		@param iValueAxis The input value aixs index.
		*/
		void UpdateCumulateData(EnumValueAxis iValueAxis);
		void GetCumulateData(EnumValueAxis iValueAxis, std::vector<double>& iorCumulatedValue);
		
		/**
		Add a base value to all the datas which belong to specified value axis.
		@param iValueAxis The input value aixs index.
		@param iBaseValue The input base value to be added to other data.
		*/
		void AddBaseToCumulateData(EnumValueAxis iValueAxis, double iBaseValue);

		/**
		Get the min & max value of the data in this renderer for specified value axis.
		It is used to set range of ValueAxis.
		@param iValueAxis The input value aixs index.
		@param iorMin The output minimum value.
		@param iorMax The output maximum value.
		@param iorAreInitialized Indicates whether iorMin and iorMax are initialized or not.
		*/
		void GetMinMaxValue(EnumValueAxis iValueAxis, double& iorMin, double& iorMax, bool& iorAreInitialized, Int32 iValueAxisDirection = 1);

		/**
		Set the data area to renderer.
		This area information could be used by trendline objects to draw itself.
		@param irDeviceDataArea The input data area.
		*/
		virtual void SetDeviceDataArea(const Rect2D& irDeviceDataArea); 

		/**
		Add the text object to ABLPlot's list and the collection to map.
		Since the labels should be shown on top, we should add them to map and list
		after all the series are created.
		It is called by ABLPlot class.
		@param iorGraphObjectMap The input and output map, it should be ABLPlot's.
		@param iorGraphObjectList The input and output list, it should be ABLPlot's.
		*/
		virtual void AddTextToMapAndList(std::map<TripleId, GraphObject::Ptr>& iorGraphObjectMap, std::vector<GraphObject::Ptr>& iorGraphObjectList);

		/**
		Move a label to the actual position.
		@param iGroupId The input group ID.
		@param iTextObjectPtr The pointer to the text object.
		@param irMax Indicates the max reference point. 
		@param irMin Indicates the min reference point. 	
		@param iInfoIndex The index for mSeriesInfo.
		*/
		void MoveLabel(Int32 iGroupId, TextObject::Ptr iTextObjectPtr, const Point2D& irMax, const Point2D& irMin, Int32 iInfoIndex);
		
		virtual void UpdateGroupStackInfoWithBase(double& orBaseValue,double& orOffsetValue,EnumValueAxis iValueIndex,Int32 iSeriseID,Int32 iGroupID,double iAxisOriginValue);
        virtual void AddObjectsToDataLabelManager(DataLabelManager::Ptr iDataLabelManagerPtr);
        
        void GetReferenceLineData(int iSeriesID, int iGroupID, std::string &orRefLineType, int& orMetricIndex, std::string &orValue);
        void hCreateDataMarkerForDataLabel(bool iOnlyForDisplayedDataLable = true);
	protected:
		/**
		Get the index in SeriesInfo for specified series ID.
		@param iSeriesId The input Series ID.
		@return The index in mSeriesInfo, -1 if not in this renderer.
		*/
		Int32 GetIndexFromSeriesId(Int32 iSeriesId) const;

		/**
		Get trend line point for the specified series and group.
		This point is the exact point that the value in dataset correspond to.
		@param iInfoIndex The index for mSeriesInfo.
		@param iGroupId The input group ID.
		@param orPoint The output point.
		@return true if successful, false if the data is NULL in Dataset.
		*/		
		bool GetTrendlLinePoint(Int32 iInfoIndex, Int32 iGroupId, Point<double>& orPoint) const;

		/**
		Get the data values.
		If stacked or percent, you will get cumulated data.
		@param iInfoIndex The index for mSeriesInfo.
		@param iGroupId The group ID.
		@param iPreValue Indicates whether to get the previous cumulate data in Stacked/Percent layout.
		@param iDataGroup The input data group index
		@return true if successful, false if NULL in Dataset.
		*/
		bool GetData(Int32 iInfoIndex, Int32 iGroupId, double& orValue, bool iPreValue = false, Int32 iDataGroup = 0) const;

        /**
         * To get two accompany points from the base points with sepcified distance, the accompany points should be aligned to base point either in X or Y axis.
         * @param iIsVertical, if it is true, than the distance between accompany point and base point are along Y axis, otherwise it is along X axis.
         * @param irBasePoint, the base point.
         * @param lOffset, The distance between accompany point and base point.
         * @param orLowPoint, the accompany point on the low side.
         * @param orHighPoint, the accompany point on the high side.
         */
        void hGetAccompanyPoints(bool iIsVertical, const Point2D& irBasePoint, double lOffset, Point2D& orLowPoint, Point2D& orHighPoint);
        
		/**
		Create trend line objects for all series, used by LineSeriesRenderer and AreaSeriesRenderer.
		*/
		void CreateAllSeriesTrendLines();
        
        void CreateRefLines();

        TextObject::Ptr hCreateRefLineLable(Int32 iSeriesInfo, Int32 iRefLineIndex);
        
        /**
         Create trend line objects for GM.
         */
		void CreateGMSeriesTrendLines();
        std::vector<Point2D> hProcessPointsForTrendline(const std::vector<Point2D>& irPoints);
        bool hCalcTrendLinePoint(Point2D& iorPoint, Int32 iInfoIndex, Int32 iGroupId, double lValue);
        
		/**
		Create trend line for specified series and only add it to mTrendLineObjects vector.
		We will not add trend line object to map and list of the renderer because it will be drawn
		after all area/bar/line are drawn.
		We add trend line object to category plot's map and list later using AddTrendLineToVector().
		@param iInfoIndex The index for mSeriesInfo.
		@param irCategory The vector values corresponding to category axis.
		@param irValue The vector values corresponding to value axis.
		*/
		void CreateTrendLine(Int32 iInfoIndex, std::vector<double>& irCategory, std::vector<double>& irValue);

		/**
		Add legend marker to riser collection.
		@param iSeriesIndex The series index in mSeriesInfo.
		@param ipRiserCollection The pointer to a given riser collection.
		@param iIsLineCollection If the ipRiserCollection is a line collection.
		@param iIsShownMarker Indicate whether shown marker or not
		@param iMarkerShape The input marker shape for legend marker
		@param iIsShownLine The legend line show status.
		*/
		void AddLegendMarkerToCollection(Int32 iSeriesIndex, GraphCollectionObject* ipRiserCollection, bool iIsLineCollection = false, bool iIsShownMarker = true, unsigned char iMarkerShape = 1, bool iIsShownLine = true);

		/**
		This method is useful for Stacked or Percent layout with depth.
		It checks whether the drawing order of each series need to be reversed.
		Here, "the drawing order" means the drawing order of each series in BarSeriesRenderer with 
		other conditions remains the same.
		@param iValueAxisIndex ValueAxis index.
		@return true means the drawing order is from small Series ID to large Series ID.
		false means the drawing order is from large Series ID to small Series ID.
		*/
		bool IsReverseSeriesOrder(EnumValueAxis iValueAxisIndex);

		/**
		This method is useful for Stacked or Percent layout with depth.
		It checks whether the drawing order of each group need to be reversed.
		@return true means the drawing order is from small Group ID to large Group ID.
		false means the drawing order is from large Group ID to small Group ID.
		*/
		bool IsReverseGroupOrder();

		/**
		This method is useful for Stacked or Percent layout with depth in LineSeriesRenderer and AreaSeriesRenderer. 
		It checks whether current series should be drawn before drawing the next series. 
		Here, "the next series" means the next series according to default order.
		Here, "default order" means the drawing order of each series in BarSeriesRenderer with other conditions remains the same.
		@param iSideDireciton For AreaSeriesRenderer, it means the direction of the common edge between current series 
		and the next series. For LineSeriesRenderer, it means the segment direction of the next series.
		@param iGroupId This is used to get cumulate data. If the data value is smaller then base value, the drawing
		order need to be switched.
		@return true means current series should be drawn before the next series. false means the next series should be drawn
		before current series.
		*/
		bool PriorToNextSeries(const TVector2D<Int32>& iSideDirection, Int32 iGroupId);


		/**
		Create label for a given index in SeriesInfo and GroupID.
		@param iInfoIndex The index for mSeriesInfo.
		@param iGroupId The group ID.
		@param iTextCollectionPtr The text collection object.
		@return The label being created.
		*/
		TextObject::Ptr CreateRiserLabel(const Int32 iInfoIndex, const Int32 iGroupId, GraphCollectionObject::Ptr iTextCollectionPtr);

        
        /////////////////////////Data Label and Custom min/max Method for Graph Matrix
        void CreateMarkerForDataLabel(Int32 iInfoIndex, Int32 iGroupId, GraphCollectionObject::Ptr iMarkerCollectionPtr, Point2D& irCenterPoint, bool isAreaChart);
        TextObject::Ptr hCreateDataLabelForGraphMatrix(const Int32 iSeriesId, const Int32 iGroupId,const double &irValue, GraphCollectionObject *ipCollection);
        bool hIsUseCustomMinMax(const EnumValueAxis iValueAxis);
        bool hIsDataLabelInDataArea(const Rect2D& iDLRect);
        
		/**
		Create total label.
		@param iValueAxis The index for mSeriesInfo.
		@param iGroupId The group ID.
		@param iTotalCollectionPtr The pointer to the text collection object.
		@return The label being created.
		*/
		TextObject::Ptr CreateTotalLabel(const EnumValueAxis iValueAxis, const Int32 iGroupId, GraphCollectionObject::Ptr iTotalCollectionPtr);

		/**
		Get the upmost series by value axis.
		This function is used to get the position of total labels.
		@param iValueAxis The index of value axis.
		@param iGroupId The input group ID.
		@return The index in mSeriesInfo.
		*/
		const Int32 GetUpmostIndexByValueAxisAndGroup(const EnumValueAxis iValueAxis, const Int32 iGroupId) const;
		
		Rect2D mDeviceDataArea;
		ABLPlot* mpABLPlot;
		Int32 mGroupCount;
		Int32 mSeriesCount;
		Dataset::Ptr mDatasetPtr;
		Legend::Ptr mLegendPtr;
		std::vector<SeriesInfo> mSeriesInfo;
		//data for Y1 and Y2, cumulate or not
		std::vector<double> mCumulateData[2];
		//series count for each axis
		Int32 mSeriesNumber[2];
		//for text collection
		std::vector<GraphCollectionObject::Ptr> mTextCollection;
		std::vector<TrendLineObject::Ptr> mTrendLineObjects;
		std::vector<TextObject::Ptr> mTrendLineTextObjects;

		// The following data members are added for access convenience. These data members are used frequently in renderers.
		ABLPlotContext* mpABLPlotContext;
		bool mIsVertical;
		bool mUseDepth;
		//Indicate whether the renderer has negative value
		//Tt will be initialized in UpdateCumulateData() since it is used in Stacked/Percent Depth Chart only
		bool mHasNegativeValue;
		Int32 mDepth;
		Int32 mDepthDirection;
		Int32 mStartLevel;			

        //For Graph Matrix.
        mutable std::map<Int32, Int32> mGroupIdMapPerSeries;				 //<the current map for current continus section.	
		Int32 hGetCorrectGroupIdInSeriesPoints(Int32 iIndex, Int32 iCandidate) const;
		/**
         Used in graph matrix, called by hDrawStandAlonePoint
         @param irTripleId, tripleId for the single dot.
         @param irPoint, the position of the dot
         @param iIsLineAsDots, true means we will draw dots instead of lines. false means there is only one single point or there are isolated points in the line series. 
         @return the dot.
         */
		GraphObject::Ptr hDrawSingleShape(const TripleId& irTripleId, const Point2D& irPoint, bool iIsLineAsDots = false);
        
        // GM Combination Support
        Int32 hConvertCEShapeToGMShape(Int32 iSeriesType);
        
        std::vector<GraphObject::Ptr > hDrawSingleDataPoint(const TripleId& irTripleId, Int32 iSeriesInfo, const Point2D& irPoint, bool iIsLineAsDots = false);
        Point2D hGetPieCenter(Int32 iGroupId, Int32 iSeriesId, Int32 iAngleIndex);
        bool hGetGMPiePositon(Point2D& orCood, Int32 iInfoIndex, double iXInput, double iYInput, Int32 iXSubIndex, Int32 iYSubIndex, Int32 iXSectionCount, Int32 iYSectionCount);
        Int32 hInitMaxPieRadius();
        std::map<TripleId, Point2D> mPieCentersPool;
    
        PiePlotRenderer::Ptr mPieRender;

        
	private:
		/**
		Get the data values.
		If stacked or percent, you will get cumulated data.
		@param iValueAxis The input value axis.
		@param iDataIndex The data index.
		@param iGroupId The group ID.
		@return The data value
		*/
		double hGetCumulateData(EnumValueAxis iValueAxis, Int32 iDataIndex, Int32 iGroupId) const;
		/**
		Read trend line formatline information from legend marker.
		@param iTrendLinePtr The pointer to trendline object.
		*/
		//void hUpdateTrendLineFormatLine(TrendLineObject::Ptr iTrendLinePtr);
	};
}

#endif
