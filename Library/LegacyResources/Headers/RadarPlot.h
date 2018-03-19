//==============================================================================================
//	FILENAME	:	RadarPlot.h
//	AUTHOR		:	Liang Liu
//	CREATION	:	2009-04-07
//	Copyright (C) MicroStrategy Incorporated 2009
//	All Rights Reserved
//==============================================================================================

#ifndef MsiChart_RadarPlot_h
#define MsiChart_RadarPlot_h

#include "PolarPlot.h"
#include "RadarCategoryAxis.h"

namespace MsiChart
{
	struct RadarSeriesInfo
	{
		RadarSeriesInfo()
			:  mDataIndex(0),		
			  mShowMarkers(false),
			  mMarkerSize(33),
			  mMarkerShape(DssGraphMarkerShapeRectangle)
		{
		}	
		Int32 mDataIndex;				///< Indicates the data index for this series ID in specified value axis.
		bool mShowMarkers;				///< Indicates whether to show data markers
		Int32 mMarkerSize;				///< Indicates the marker size.
		unsigned char mMarkerShape;		///< Indicates the marker shape.
	};

	///< This class is used to generate radar chart.
	class RadarPlot : public PolarPlot
	{
	public:
		typedef MSynch::SmartPtr<RadarPlot> Ptr;

		/** Constructor
		@param irTripleId,ipManager,irDeviceRect Required by base class.
		*/
		RadarPlot(const TripleId& irTripleId, GraphObjectManager* ipManager, const Rect2D& irDeviceRect);
		
		//virtual functions from GraphObjectManager
		virtual void GenerateMapAndList();
		virtual void GetOptions(Int32 iSeriesId, Int32 iPropertyId, CComVariant* opValue) const;
		//virtual void Draw();
		
		//override functions	
		//virtual void OnRectangleSelection(const Rect2D& irRect, std::vector<GraphObject::Ptr>& iorSelectedObjects) const;		
		
	private:
		virtual PolarValueAxis::Ptr hGenerateXAxis();
		/**
		Load plot level properties.
		*/
		void hLoadProperties();
		void hGenerateAreaForSeries(Int32 iSeriesIndex, std::vector<Point2D>& orSeriesPoints);
		//void hGenerateDataLineForSeries(Int32 iSeriesIndex);

		/// Generate axes.
		//void hGenerateAxes();

		/**
		Generate data markers for one series.
		@param irPoints Specifies where to draw the markers.
		@param iSeriesIndex Specifies the series index.
		*/
		//void hGenerateMarkersBySeries(const std::vector<Point2D>& irPoints, Int32 iSeriesIndex, std::vector<Rect2D>& orMarkerRects);

		/**
		Generate data lines for one series.
		@param irPolarPoints Specifies the data positions.
		@param iSeriesIndex Specifies the series index.
		*/
		//void hGenerateDataLinesBySeries(const std::vector<FPoint2D>& irPolarPoints, Int32 iSeriesIndex);

		//void hGenerateOneDataLine(const std::vector<FPoint2D>& irPolarPointSet, Int32 iSeriesIndex, GraphCollectionObject* ipDataLineSet);

		//void hGenerateDatalabels(const std::vector<Rect2D>& irRects, Int32 iSeriesIndex);
		//virtual TextObject::Ptr hCreateDataLabel(Int32 iSeriesIndex, Int32 iGroupID, GraphCollectionObject* ipCollection);
		virtual bool hGetValueString(Int32 iSeriesIndex, Int32 iGroupID, MBase::String& orValue, GraphCollectionObject* ipCollection);
		//bool hGetVAFormattedText(Int32 iSeriesIndex, Int32 iGroupID, MBase::String& orText);
		/**
		 *	Get the minimum and maximum value for value axis
		 *	@param orMin: Minimum value for this axis
		 *	@param orMax: Maximum value for this axis
		 */
		void hGetMinMaxValueForValueAxis(double& orMin, double& orMax);	

		//Point2D hGetPointByAngleAndRadius(double iAngleRad, double iRadius) const;

		void hMoveDataLabel(TextObject* ipText, const Rect2D& irRefRect);

		//void hCreateTrendLines();

		/**
		Load series level properties.
		@param iSeriesIndex The input series index. 
		*/
		void hLoadSeriesProperties(Int32 iSeriesIndex);
		
		void hParseSubGraphType();
		/**
		Get the data values.
		In stacked chart, you will get cumulated data.
		@param iSeriesIndex The index for mSeriesOptions.
		@param iGroupId The group ID.
		@param iIsXValue Indicates whether to get X value or Y value.
		@param iPreValue Indicates whether to get the previous cumulate data in Stacked layout.
		@param iDataGroup The input data group index
		@return true if successful, false if NULL in Dataset.
		*/
		virtual bool GetData(Int32 iSeriesIndex, Int32 iGroupId, bool iIsXValue, double& orValue) const;
		void hInitCumulateData(EnumValueAxis iValueAxis);		
		/**
		Get a polygon related to one series.
		@param iSeriesIndex This indicates the series index in mSeriesInfo.
		@param orPolygon This holds the output polygon.
		@return Return true if all points in orPolygon are valid.
		*/
		bool hGetSeriesPolygon(Int32 iSeriesIndex, Polygon2D& orPolygon);	
		bool hGetStackedPolygon(Int32 iSeriesIndex, Polygon2D& orPolygon);
		void hParseStackedPolygonUnits(const Polygon2D& irPolygon, std::vector<std::vector<Point2D> >& orPolygonUnits);
		//void hParseSeriesPoints(const std::vector<FPoint2D> irOuterPoints, const std::vector<FPoint2D> irInnerPoints, std::vector<std::vector<FPoint2D> >& orPointGroups) const;
		/**
		Get the data values.
		If stacked or percent, you will get cumulated data.
		@param iValueAxis The input value axis.
		@param iDataIndex The data index.
		@param iGroupId The group ID.
		@return The data value
		*/
		double hGetCumulateData(EnumValueAxis iValueAxis, Int32 iDataIndex, Int32 iGroupId) const;	
		void hGenerateTotalLabelsByAxis(EnumValueAxis iValueAxisIndex, GraphCollectionObject::Ptr iTotalCollectionPtr, 
			const std::vector<std::vector<Rect2D> >& irMarkerRectangleMatrix);
		TextObject::Ptr CreateTotalLabel(const EnumValueAxis iValueAxis, const Int32 iGroupId, GraphCollectionObject::Ptr iTotalCollectionPtr);
		const Int32 GetUpmostIndexByValueAxisAndGroup(const EnumValueAxis iValueAxis, const Int32 iGroupId) const;
		//void MoveLabel(Int32 iGroupId, TextObject::Ptr iTextObjectPtr, const Point2D& irMax, const Point2D& irMin, Int32 iInfoIndex);
		virtual void hMoveDataLabel(TextObject* ipText, const Rect2D& irRefRect, Int32 iSeriesIndex);	
		
		bool mIsDualAxis;
		bool mShowCumulate;
		bool mShowTotal;

		/// Data for Y1 and Y2, cumulate or not
		std::vector<double> mCumulateData[2];
		/// Series count for each axis
		Int32 mSeriesNumber[2];

		std::vector<RadarSeriesInfo> mRadarSeriesInfo;	///< Series information for Radar.	
	};	

	inline double RadarPlot::hGetCumulateData(EnumValueAxis iValueAxis, Int32 iDataIndex, Int32 iGroupId) const
	{
		return mCumulateData[iValueAxis][iGroupId * mSeriesNumber[iValueAxis] + iDataIndex];
	}

}
#endif
