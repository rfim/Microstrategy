//====================================================================
//	Filename:		Plot.h
//	Date:			2008-05-21
//	Authors:		Bing Huang

//	Copyright (C) 1997-2008 MicroStrategy Incorporated
//	All rights reserved
//====================================================================

#ifndef MsiChart_Plot_h
#define MsiChart_Plot_h

#include "../Dataset.h"
#include "Legend.h"

#include "GraphObjectManager.h"
#include "TrendLineObject.h"
#include "DataLabelManager.h"

namespace MsiChart
{
	//enumerations and structs
	typedef enum
	{
		TLI_Y1 = 0,
		TLI_Y2 = 1,
		TLI_BOTH = 2
	}EnumTrendLineIndex;
	
	struct GraphTrendLineOptions
	{
		GraphTrendLineOptions() : mRegressionType(DssGraphRegressionNone)
		, mOrder(3), mPeriod(2), mShowEquation(false), mShowCoefficients(false)
		, mUserSeriesGraphColor(true), mUseSeasonal(false), mIsExpomentialBaseNearOne(false)
		{}
		
		/**
		 This function gets the trendline property value according to the input property id.
		 @param iPropertyId The input property id.
		 @param opValue Stores the output value of the specified property.
		 */
		void GetOptions(Int32 iPropertyId, VARIANT* opValue) const;
		
		EnumDSSGraphRegressionType		mRegressionType;
		Int32	 						mOrder;
		Int32							mPeriod;
		bool							mShowEquation;
		bool							mShowCoefficients;
		bool							mUserSeriesGraphColor;
        bool                            mUseSeasonal;
        bool                            mIsExpomentialBaseNearOne;
	};
	
	typedef enum
	{
		VA_Y1 = 0,
		VA_Y2 = 1,
		VA_BOTH = 2,
		VA_Y3,
		VA_Y4
	}EnumValueAxis;

	//pre-declaration
	class TextObject;
	class TrendLineObject;

	/*
		virtual base class for all plot classes
	*/
	class Plot : public GraphObjectManager
	{
	public:
		typedef MSynch::SmartPtr<Plot> Ptr;
		
		//ctor & dtor
		Plot(const TripleId& irTripleId, GraphObjectManager* ipManager, const Rect2D& irDataArea);
		virtual ~Plot();

		//Pure virtual functions from GraphObjectManager
		virtual void Draw();
		virtual void GenerateView();
		virtual void GenerateImageMap(ImageMap &iorImageMap);
		
		//Virtual functions from GraphObjectManager
		virtual GraphObject::Ptr OnPointDetection(const Point2D& irCursor) const; 
		virtual void OnRectangleSelection(const Rect2D& irRect, std::vector<GraphObject::Ptr>& iorSelectedObjects) const;

		virtual bool GetToolTipData(Int32 iSeriesID, Int32 iGroupID, double& orValue, double& orPercentValue, Int32 iOffset = 0);
		
		virtual bool GetGroupSpace(std::vector<Int32>& orVect, Int32 iGroupIndex); //< voice over
        virtual Int32 GetMaxElementRadius();	//< graph matrix
        virtual void RefreshValueAxis(const std::map<Int32, std::pair<double, double> >& irScaleSet, const Rect2D& irRect);
        virtual bool GetMaxElementOffsetClippedByBoundary(bool iIsOnXAxis, double& orLowOffset, double& orHighOffset);
        virtual bool GetGMPiePositon(Point2D& orCood, double iXInput, double iYInput, Int32 iXSubIndex = 0, Int32 iYSubIndex = 0, Int32 iXSectionCount = 1, Int32 iYSectionCount = 1,Int32 iRadius = 2);

		/// Draw trendlines and equations.
		virtual void DrawTrendLinesAndEquations();
		/**
		Get the data area for the plot.
		@param orDataArea The output data area.
		*/
		void GetDataArea(Rect2D& orDataArea) const;

		/**
		Set the data area for the plot.
		@param irDataArea The input data area.
		*/
		void SetDataArea(const Rect2D& irDataArea);

		/**
		Get dataset pointer.
		@return The dataset pointer.
		*/
		Dataset::Ptr GetDataset() const;

		/**
		Get legend pointer.
		@return The legend pointer.
		*/
		Legend::Ptr GetLegend() const;

		/**
		Set dataset to plot.
		@param iLegendPtr The input dataset pointer. 
		*/
		void SetDataset(Dataset::Ptr iDataSetPtr);

		/**
		Set legend to plot.
		@param iLegendPtr The input legend pointer. 
		*/
		void SetLegend(Legend::Ptr iLegendPtr);

		/**
		Add a series to the category plot
		@param iSeriesId The input series ID.
		*/
		void AddSeries(Int32 iSeriesId);


		/**
		Get the series index by series ID.
		We do a binary search in mSeriesId since it is ordered vector.
		@param iSeriesId The input series ID.
		@return The series index
		*/	
		Int32 GetSeriesIndexByID(Int32 iSeriesId) const;

		/**
		Move the labels.
		@param ipTextObject The input text object that is to be moved.
		@param irTopRight The top or right point according to the new center point.
		@param irBottomLeft The bottom or left point according to the new center point.
		@param iIsReverse Indicate whether reverse, two things will affect this: Axis reverse and data negative or positive.
		@param iIsVertical Indicate whether the chart is vertical or horizontal.
		@param iLabelLocation Indicate the label position.
		@param iSmart, whether use smart label or not.
		*/
		void MoveLabel(TextObject* ipTextObject, const Point2D& irTopRight, const Point2D& irBottomLeft, bool iIsReverse, bool iIsVertical, EnumDSSGraphDataTextPosition iLabelLocation) const;
		/*void MoveLabel(TextObject* ipTextObject, const Point2D& irTopRight, const Point2D& irBottomLeft, bool iIsReverse, bool iIsVertical, EnumDSSGraphDataTextPosition iLabelLocation, bool iSmart) const;
		bool IsLabelClipped(TextObject* ipTextObject, bool iIsVertical, EnumDSSGraphDataTextPosition iOriginalPosition, EnumDSSGraphDataTextPosition& iorBestPosition) const;*/

		/**
		As the name, add legend marker to the collection they belong to.
		@param iSeriesId The input series ID.
		@param ipRiserCollection The input collection.
		@param iIsLineCollection Indicate whether it is line collection, true for line chart.
		@param iMarkerShape Indicate the marker shape.
		@param iIsShownLine Indicate whether the lines are shown or not.
		*/
		void AddLegendMarkerToCollection(Int32 iSeriesId, GraphCollectionObject* ipRiserCollection, bool iIsLineCollection = false, bool iIsShownMarker = true, unsigned char iMarkerShape = DssGraphMarkerShapeRectangle, bool iIsShownLine = true, bool iIsLineVertical = false) const;

		/**
		 Create a trend line, add it to collection and update the format line information if necessary.
		 @param irX The input sample point X coordinate.
		 @param irY The input sample point Y coordinate.
		 @param irTripleId The input triple ID for trend line.
		 @param ipManager Indicate the manager to create the trend line.
		 @param ipParentObject Indicate the parent of the trend line.
		 @param ipXAxis Indicate the axis where we get the irX.
		 @param ipYAxis Indicate the axis where we get the irY.
		 @param ipTrendLineOptions The input trend line option.
		 @param irDataArea The input data area.
		 @param iIsVertical Indicate whether the chart is vertical or horizontal.
		 @param irDepthOffset The input depth information.
		 @param iIsY2 Indicate whether the trend line belongs to Y1 or Y2.
		 */
		void CreateTrendLine(const std::vector<double>& irX, const std::vector<double>& irY, const TripleId& irTripleId, 
							 GraphObjectManager* ipManager, GraphCollectionObject* ipParentObject, Axis* ipXAxis, Axis* ipYAxis,
							 GraphTrendLineOptions* ipTrendLineOptions, const Rect2D& irDataArea, bool iIsVertical, const Point2D& irDepthOffset, bool iIsY2 = false);
		
		/**
		 Adjust trend line equation position.
		 @param irRect The rectangle trend lines should be in.
		 @param iIndex Indicate which trend line collection we would like to adjust, Y1, Y2 or Both.
		 */
		void AdjustTrendLineEquation(Rect2D& irRect, EnumTrendLineIndex iIndex);
		
		/**
		 Draw trend lines and their equations.
		 @param irClipRect The rectangle which is used to clip the trend line.
		 @param iIsY2 Indicate whether draw trend line for Y1 or Y2.
		 */
		void DrawTrendLinesAndEquations(Rect2D& irClipRect, bool iIsY2 = false);
		
		/**
		Draw objects in mPostTrendLineObjects.
		*/
		void DrawPostTrendLineObjects();
		
		/**
		 * Generate UIViews for objects in mPostTrendLineObjects.
		 */
		void GeneratePostTrendLineViews();
		
		bool HasTrendLines() const;
        
        void AddTrendLineAnadRefLine(GraphObject::Ptr iLine);
        
        DataLabelManager::Ptr GetDataLabelManager();
        
        //kai;2016/Jul/26;DE35637;fix GM referenceline issue, change to metric to decide whether to draw
        bool GetVAMetricIndexDrawn(Int32 iValueMetricIndex);
        void SetVAMetricIndexDrawn(Int32 iValueMetricIndex);
        /*
        bool GetVAIndexDrawn(Int32 iValueAxis);
        void SetVAIndexDrawn(Int32 iValueAxis, bool iDrawn);
         */
	protected:
		/**
		 Load the trend line properties for series specified by iSeriesId.
		 @param iSeriesId The input series ID.
		 @param iorTrendLineOption The input and output trend line option.
		 */
		void LoadTrendLineProperties(Int32 iSeriesId, GraphTrendLineOptions& iorTrendLineOption) const;
			
		/**
		Load the value axis information for each series.
		@param iSeriesIndex The input series index.
		@param iIsDualAxes Indicate whether it is dual axis or not.
		@param orValueAxis The output reference to value axis variable.
		*/
		void LoadValueAxisInfo(Int32 iSeriesIndex, bool iIsDualAxes, EnumValueAxis& orValueAxis) const;

		Dataset::Ptr mDatasetPtr;
		Legend::Ptr mLegendPtr;

        std::vector<Int32> mGroupID;
		std::vector<Int32> mSeriesId;
		Rect2D mDataArea;

		//use to put trend line object.
		std::vector<TrendLineObject::Ptr> mTrendLines[2];
		
		//use to put the objects that should be draw after trend line.
		//for example, custom line and series text labels.
		std::vector<GraphObject::Ptr> mPostTrendLineObjects;	
        
        // GM Text objects manager including data label and reference line label.
        DataLabelManager::Ptr mLabelManager;
        
        // RefLineSupport
        bool mVAIndexDrawn[5];
        std::set<Int32> mVAIndexDrawnSet;

	private:
		/**
		 Update the trend line format line information.
		 Used if use the same color as series option is checked.
		 @param iTrendLinePtr The input trend line object.
		 */
		void hUpdateTrendLineFormatLine(TrendLineObject::Ptr iTrendLinePtr);
	};

	inline Dataset::Ptr Plot::GetDataset() const
	{
		return mDatasetPtr;
	}

	inline Legend::Ptr Plot::GetLegend() const
	{
		return mLegendPtr;
	}

	inline void Plot::SetLegend(Legend::Ptr iLegendPtr)
	{
		mLegendPtr = iLegendPtr;
	}
	
	inline void Plot::SetDataset(Dataset::Ptr iDataSetPtr)
	{
		mDatasetPtr = iDataSetPtr;
	}

	inline void Plot::AddSeries(Int32 iSeriesId)
	{
		mSeriesId.push_back(iSeriesId);
	}	
    
	inline bool Plot::HasTrendLines() const
	{
		return !mTrendLines[0].empty() || !mTrendLines[1].empty();
	}
    
    inline DataLabelManager::Ptr Plot::GetDataLabelManager()
    {
        if (mLabelManager.IsNull()) {
            mLabelManager = new DataLabelManager(mChartContextPtr, mpManager);
        }
        return mLabelManager;
    }
    /*
    inline bool Plot::GetVAIndexDrawn(Int32 iValueAxisIndex)
    {
        return mVAIndexDrawn[iValueAxisIndex];
    }
    
    inline void Plot::SetVAIndexDrawn(Int32 iValueAxisIndex, bool iDrawn)
    {
        mVAIndexDrawn[iValueAxisIndex] = iDrawn;
    }
     */
    
    inline bool Plot::GetVAMetricIndexDrawn(Int32 iValueMetricIndex)
    {
        return mVAIndexDrawnSet.find(iValueMetricIndex) != mVAIndexDrawnSet.end();
    }
    
    inline void Plot::SetVAMetricIndexDrawn(Int32 iValueMetricIndex)
    {
        mVAIndexDrawnSet.insert(iValueMetricIndex);
    }
    
}

#endif
