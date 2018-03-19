//=======================================================================
//	Filename:		Kernel/SourceCode/IPhoneChart/PrivateSource/AreaSeriesRenderer.h
//	Date:			2008-10-06
//	Authors:		Liang Liu, Bing Huang

//	Copyright (C) 1997-2008 MicroStrategy Incorporated
//	All rights reserved
//=======================================================================

#ifndef MsiChart_AreaSeriesRenderer_h
#define MsiChart_AreaSeriesRenderer_h

#include "SeriesRenderer.h"
#include "Vector2D.h"
#include "DataLabelManager.h"

namespace MsiChart
{
#ifdef _VE_CE
	class GraphObjectAggregation;
#endif
	struct ABLPlotContext;
	/**
	This class is used to generated area risers and area labels.
	If there are null values in the dataset, there may be more than one polygons for one series.
	*/
	class AreaSeriesRenderer : public SeriesRenderer
	{
	public:
		typedef MSynch::SmartPtr<AreaSeriesRenderer> Ptr;

		/**	Constructor.
		All the parameters are required by base class.
		*/
		AreaSeriesRenderer(const TripleId& irTripleId, GraphObjectManager* ipManager, Dataset::Ptr iDatasetPtr, Legend::Ptr iLegendPtr);

		virtual void GenerateMapAndList();
		virtual void GenerateByValueAxis(EnumValueAxis iValueAxis, std::vector<GraphObject::Ptr>& iorGraphObjectList);
		virtual void GenerateImageMap(ImageMap& iorImageMap);

#ifdef _VE_CE
		virtual void GenerateInterpolationFrame(double iProgress, GraphObjectAggregation* ipLayerObject);
#else
		virtual void GenerateInterpolationFrame(double iProgress, LayerObject* ipLayerObject);
#endif
        //override function.
        virtual GraphObject::Ptr OnPointDetectionEx(const Point2D& irCursor,
                                                    const std::vector<Int32>& irObjectIds, Int32& iorDistance, Int32 iRadius,
                                                    std::vector<GraphObject::Ptr>& iorGraphObjectsAround, EnumGraphObjectUsage iUsage = GOU_NORMAL_HIGHLIGHT);
        virtual void AddObjectsToDataLabelManager(DataLabelManager::Ptr iDataLabelManagerPtr);
        void hCreateDataMarkerForDataLabel(bool iOnlyForDisplayedDataLable = true);
	private:
		/**
		Create area collections based on one or both value axis and add them to map (use to hold area and legend marker).
		@param iValueAxisIndex Specifies a value axis.
		If iValueAxisIndex is VA_BOTH, generate area collections for both value axis.
		*/
		void hGenerateAreaCollections(EnumValueAxis iValueAxisIndex);
		/**
		Generate data labels and put them into mTextCollection.
		*/
		void hGenerateDataLabels();
		/**
		Generate data labels for one series and put them into mTextCollection.
		This is a helper method of hGenerateDataLabels().
		@param iSeriesIndex This indicates the series index in mSeriesInfo.
		*/
		void hGenerateLabelsForSeries(Int32 iSeriesIndex);
		/**
		This method is used to generate naive side faces of a depth polygon.
		This method is useful for Stacked or Percent layout with depth.
		@param iSeriesIndex This indicates the series index in mSeriesInfo.
		@param irPolygon Specifies a polygon related to the side faces.
		@param iGroupId The risers related to (iGroupId - 1, iGroupId) will be generated.
		*/
		void hGenerateNaiveSideFaces(Int32 iSeriesIndex, const Polygon2D& irPolygon, Int32 iGroupId);
		/**
		Generate risers which are related to one or both value axis and put them into mGraphObjectList and mAreaCollection.
		This is a helper method for GenerateByValueAxis().
		@param iValueAxisIndex Specify a value axis.
		If iValueAxisIndex is VA_BOTH, generate area collections for both value axis.
		*/
		void hGenerateRisers(EnumValueAxis iValueAxisIndex);
		/**
		This method is used to generate side faces of a depth polygon.
		This method is useful for Stacked or Percent layout with depth.
		@param iValueAxisIndex Specify a value axis.
		@param iGroupId The risers related to (iGroupId - 1, iGroupId) will be generated.
		*/
		void hGenerateSideFaces(EnumValueAxis iValueAxisIndex, Int32 iGroupId);
		/**
		This method is used to generate tricky side faces of a depth polygon.
		This method is useful for Stacked or Percent layout with depth.
		@param iValueAxisIndex Specify a value axis.
		@param iGroupId The riser depth lines related to (iGroupId - 1, iGroupId) will be generated.
		*/
		void hGenerateTrickySideFaces(EnumValueAxis iValueAxisIndex, Int32 iGroupId);
		/**
		Gererate total label by value axis.
		@param iValueAxisIndex Specify a value axis.
		@param iTotalCollectionPtr This is used for sharing format and batch selection.
		*/
		void hGenerateTotalLabelsByAxis(EnumValueAxis iValueAxisIndex, GraphCollectionObject::Ptr iTotalCollectionPtr);
		/**
		Generate risers for one series and put them into mGraphObjectList and mAreaCollection
		@param iSeriesIndex This indicates the series index in mSeriesInfo.
		@param iUseDepth This parameter indicates whether to generate DepthPolygonObject or PolygonObject.
		*/
		void hGenerateRisersForSeries(Int32 iSeriesIndex, bool iUseDepth);
		/**
		This is a helper method for hGenerateRisersForSeries().
		@param iSeriesIndex This indicates the series index in mSeriesInfo.
		@param irPolygon Specifies one polygon.
		@param iUseDepth This parameter indicates whether to generate DepthPolygonObject or PolygonObject.
		*/
		void hGenerateOnePolygon(Int32 iSeriesIndex, const Polygon2D& irPolygon, bool iUseDepth);
		/**
		Get a polygon related to one series.
		@param iSeriesIndex This indicates the series index in mSeriesInfo.
		@param orPolygon This holds the output polygon.
        @return TRUE means this polygon goes across the baseline of valueaxis and FALSE means otherwise.
        */
		bool hGetSeriesPolygon(Int32 iSeriesIndex, Polygon2D& orPolygon);

		void hAddVirtualMarkersToImageMap(EnumValueAxis iValueAxisIndex, ImageMap& iorImageMap);

		void hAddSeriesVirtualMarkersToImageMap(Int32 iSeriesIndex, ImageMap& iorImageMap);

		void hInitSeriesPolygons();

		void hParsePolygon(const Polygon2D& irPolygon, std::vector<std::vector<Point2D> >& orPolygonUnits, std::vector<std::vector<Point2D> >& orSeriesPolygonBounder, int iSeriesIndex) const;

		bool hGetPolygonUnit(Int32 iSeriesIndex, Int32 iGroupId, Polygon2D& orPolygonUnit) const;

		void hGenerateVirtualMarkers(EnumValueAxis iValueAxisIndex);
		void hGenerateVirtualMarkersBySeries(Int32 iSeriesIndex);
		void GetAreaPointByProgress(int iSeriesIndex, int iPointIndex, double iProgress, Point2D& orAreaPoint);

		//GraphMatrix Stuff
        /** This is a wrapping helper method to hGenerateOnePolygon. Besides the normal logic in hGenerateOnePolygon, it also takes care of
         cases specific to GraphMatrix.
         */
		void hGenerateOneRiser(Int32 iSeriesIndex, Int32 iGroupIndex, const Polygon2D& irPolygon, bool iUseDepth);
        GraphObject::Ptr hGenerateOnePolygon(const TripleId& irAreaId, const Polygon2D& irPolygon, bool iUseDepth, GraphCollectionObject* ipSet);
        void hSplitPolygon(const Polygon2D& irPolyGon, std::vector<Polygon2D>& orSplitPolyGonSet, Int32 iSplitSpan) const;

        void hPrepareAnimation();

        /** To get the correct series index, in stacked area with polygon crossng over VA baseline, we disrupt the normal drawing order.
         */
        Int32 hGetSeriesPolygonIndex(Int32 iSeriesIndex);
        //End of GM
        /**
		Maintains a vector of GraphCollectionObject.
		Each GraphCollectionObject is related to a series in AreaSeriesRenderer. mAreaCollection is initialized
		in hGenerateAreaCollections(). After initialized, the size of mAreaCollection equals to mSeriesInfo.size().
		*/
		std::vector<GraphCollectionObject::Ptr> mAreaCollection;

		std::vector<std::vector<Point2D> > mSeriesPolygons; ///< Maintains a vector of series polygons.

		std::vector<std::vector<Point2D> > mFinalSeriesPolygons;

        std::vector<Int32> mSeriesDrawingOrder;     ///< The order to draw the series.
        std::set<Int32> mTransitionalSet;           ///< Index of series where there are both negative and positive values.

		bool mIsAbsolute;	///< Indicates whether current area chart is absolute or not.
		mutable bool mIsPolygonSplit; ///< whether or not polygon are split (due to coloring, used in graph matrix).
		bool mSupportAnimation;	///< Indicate whether animaiton is supported for area series.

		std::vector<std::vector<EnumValueAxis> > mAxisGenInfo;

        mutable std::vector<std::pair<Int32, Int32> > mGroupRangeList;
        void hPopulateGroupRangeListPerSeries(const std::vector<std::vector<Point2D> >& irPolygonUnits) const;
        void hPopulateGroupIdMapPerSeries(const std::vector<Point2D>& irPolygonUnit) const;

        void hSplitGroupRangeListPerSeries(Int32 iSplitSpan) const;
        Int32 hGetStartGroupIndexForPolygonAt(Int32 iPolygonIndex, Int32 iCanidate);
        bool hIsAtHighSideToBase(bool iIsVertical, Int32 iBasePosition, bool iIsStringent, const Point2D& irPoint) const;
        void hUpdatePolygonSize(Int32& iBasePosition, Int32& iValuePosition, double iValue, double iBaseValue);
        void hPushDownBaseSide(Polygon2D& iorPolygon, bool iIsVertical) const;
        // Move the entire polygon by iOffset pixels. if iOffset is :
        // Positive : Move Up(Vertical), Move Right(Horizontal)
        // Negative : Move Down(Vertical), Move Left(Horizontal)
        void hMovePolygon(Polygon2D& iorPolygon, Int32 iOffset, bool iIsVertical) const;
        bool hUpdateBaseSide(Polygon2D& iorPolygon, ValueAxis* iValueAxisPtr, bool iIsVertical);

		//Generate one polygonline object, to draw as the stroke of the areas
		void hGenerateOnePolylineBounderForDataLabel(Int32 iSeriesIndex, Int32 iGroupIndex, const Polygon2D& irPolygon, bool iUseDepth);
        void hGenerateStorkeBounderForOneRiser(Int32 iSeriesIndex, Int32 iGroupIndex, const Polygon2D& irPolygon, bool iUseDepth);
		GraphObject::Ptr hGenerateOnePolygonBounder(const TripleId& irAreaId, const Polygon2D& irPolygon, Int32 iSeriesIndex);

        TripleId hGetBelowMarkerObjectID(const TripleId& lMarkerId, const Point2D& irCursor);

        //-------------Data Label for GM------------//
        GMDataLabelVecs mDataLabelsVec;
        std::vector<GraphObject::Ptr> mAreaBoundForDL;
        std::vector<GraphObject::Ptr > mSinglePointObjects;
        void CreateAllDataLabels();
        DataLabelManager::Ptr hInitDataLabelManager();
        
        bool hCalculateDLBasePointAndOffset(GMDataLabelInfo &iorDataLabel);
        void hPrepareDataLabelPositions();
        int hGetDLIndexInVec(Int32 iSeriesId, Int32 iGroupId);
        void hCreateAreaMarkers();
        //Int32 hGetCorrectLineObjectGroupIdInSeriesPoints(Int32 iInfoIndex, Int32 irGroupId);
	};

}

#endif