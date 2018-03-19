//====================================================================
//	Filename:		ValuePlot.h
//	AUTHOR		:	Fangyao Ma, Dong Shi
//	CREATION	:	02/09/09
//	Copyright (C) MicroStrategy, Inc. 2009
//====================================================================

#ifndef MsiChart_ValuePlot_h
#define MsiChart_ValuePlot_h

#include "ValueBasePlot.h"
#include "ValueAxis.h"
#include "GraphCollectionObject.h"
#include "TextObject.h"
#include "MarkerObject.h"
#include "DataLabelManager.h"
#include "PiePlotRenderer.h"

namespace MsiChart
{
	struct ScatterSeriesInfo
	{
		ScatterSeriesInfo()
			:mSeriesID(0),mMarkerSize(33),mMarkerShape(0),
			mConnectDataPoint(false), mShowSmartLabel(false), mValueAxisIndex(-1), mXValueAxisIndex(-1)
		{}

		bool  mConnectDataPoint;	/// Connect the series line
		//bool  mShowDataLabel;		/// Show data label
        
        bool  mShowSmartLabel;      /// Show smart label

		Int32 mMarkerSize;			/// Marker Size
		Int32 mMarkerShape;			/// Marker Shape
		Int32 mValueAxisIndex;		/// Value axis index - Y1 or Y2
        Int32 mXValueAxisIndex;     /// Value axis index - X1 or X2

		Int32 mSeriesID;			///	Series ID

		GraphTrendLineOptions		mTrendLineOptions;
		std::vector<bool> mIsNullData; //<Indicate for each dp, is it a null data? 
	};

	
	///[refactory] Maybe in future, we may use this also for the normal case
	struct LabelInfo
	{
		bool mIsPut;      //<Whether this label is already been 'Smarted'
		bool mIsClockWise; //< The flag indicate which direction the intuitive try-out will take in.
		bool mIsValid;    //Is Hosting marker valid : null data, etc. 

		Int32 mSeriesId;
		Int32 mGroupId;	
	
		Rect2D mHostMarker; //< The position for its hosting marker

		EnumDSSGraphDataTextPosition mSuggestedStart; //< The suggested starting position for this label
		EnumDSSGraphFontRotation mLabelRotation; //< rotation info, unsupported yet, leave for future.
		TextObject::Ptr mLabelPtr;

		LabelInfo(): mSeriesId(0), mGroupId(0), mIsClockWise(false), mIsPut(false), mSuggestedStart(DssGraphDTPositionOutMin)
			,mLabelRotation(DssGraphFontRotateNormal), mIsValid(false)
		{}
	
		bool IsMoved(EnumDSSGraphDataTextPosition iUserSetting)
		{
			return mSuggestedStart != iUserSetting;
		}
	
		Rect2D ToRectangle()
		{
			Rect2D lRect;
			mLabelPtr->GetBoundingRect(lRect);
			return lRect;
		}
	};
	
	typedef std::vector<LabelInfo>  LabelMatrix;
    
	typedef enum
	{
		VI_X = 0,
		VI_Y = 1,
		VI_Z = 2,
	} EnumValueIndex;

	 

	class ValuePlot: public ValueBasePlot
	{
	public:
		typedef MSynch::SmartPtr<ValuePlot> Ptr;
		typedef std::vector<std::vector<Point2D> > PointMatrix;
		typedef std::vector<std::vector<Rect2D> > RecMatrix;
		ValuePlot(const TripleId& irTripleId, GraphObjectManager* ipManager, const Rect2D& irDataArea);

		//pure virtual functions from GraphObjectManager
		virtual void GenerateMapAndList();
		virtual void GetOptions(Int32 iSeriesId, Int32 iPropertyId, CComVariant* opValue) const;

		//override functions
		virtual void Draw();
		virtual void GenerateView();
		virtual GraphObject::Ptr OnPointDetection(const Point2D& irCursor) const; 
		virtual void OnRectangleSelection(const Rect2D& irRect, std::vector<GraphObject::Ptr>& iorSelectedObjects) const;
	    virtual void GenerateImageMap(ImageMap& iorImageMap); 
        virtual void RefreshValueAxis(const std::map<Int32, std::pair<double, double> >& irScaleSet, const Rect2D& irRect);

		//assign the input ValueAxis pointer to proper data memebers 
		/**        
        @param iValueAxisPtr, the input value axis pointer
		@param iIndex, the input index for that value axis pointer.
		*/
		virtual void AssignValueAxis(ValueAxis::Ptr iValueAxisPtr, Int32 iIndex);

		//Populate the series information for the hosting ValuePlot object, called by ValuePlot manager
		/**        
        @param irSeriesIndex, indicate the index(their series ids)of the series held by this plot object 
		@param irVecSeriesVAIndex, indicate the managing VA index for the series held by this plot object
		*/
		virtual void AssignSeriesInfo(const std::vector<Int32>& irSeriesIndex,
								const std::vector<Int32>& irVecSeriesVAIndex);

        //Graph Matrix Padding Required.    
		virtual Int32 GetMaxElementRadius();	
        
        //Get the radius/offset that is clipped by chart boundary
        /**
         @param iIsOnXAxis, is the offset is the result clipped by X or Y axis.
         @param orLowOffset, result on the left or bottom end of X/Y axis.
         @param orHighOffset, result on the right or top end of X/Y axis.
         */
        virtual bool GetMaxElementOffsetClippedByBoundary(bool iIsOnXAxis, double& orLowOffset, double& orHighOffset);
        
        //AxisOriginLine support
        void CreateAxisOriginLine();
        void CreateAxisOriginLineLabels();
	protected:
		void LoadProperties();

		//helper function to generate collection objects for later use
		/**
		@param orLineCollection, the vector to store the created collection objects.
		*/
		void hGenerateCollection(std::vector<GraphCollectionObject::Ptr>& orLineCollection);
		
		//helper function to generate data-connection lines for one series, denote by the SeriesIndex
		/**
		@param irCoordPoints, coordination points of this single series
		@param iSeriesIndex, index of the series for this chart area
		@param ipCollection, for scatter chart, it will add polylines to collection; for bubble chart, nothing is done.
		*/
		void hCreateConnectionPolyLine(std::vector<Point2D>& irCoordPoints, Int32 iSeriesIndex, GraphCollectionObject* ipCollection);

		//helper function to generate Custome lines for Y1 Axis (, Y2 (if dual axis ) ) and X Axis held by this plot.
		void hGenerateCustomeLine();

		//helper function to generate data markers for one series specified by iSeriesIndex in one chart area.
		/**
		@param iorRecMatrix, storing the rectangle information for each genearted marker.
		@param irCoordPoints, the coordinate information of the data markers in this single series
		@param irMarkerSizes, the marker sizes of the data markers in this single series
		@param iSeriesIndex, index of the series for this chart area
		@param ipCollection, for bubble chart, it will add data markers to collection; for scatter chart, nothing is done.
		*/
		void hGenerateMarker(RecMatrix& iorRecMatrix,  std::vector<Point2D>& irCoordPoints, std::vector<Int32>& irMarkerSizes, Int32 iSeriesIndex, GraphCollectionObject* ipCollection, std::vector<GraphObject::Ptr>& ioShapeList);


        std::vector<GraphObject::Ptr > hGenerateMarkersForOneDataPoint(Int32 iSeriesIndex, Int32 iGroupId, Point2D& irCoordPoint, Int32 iMarkerSize, GraphCollectionObject* ipCollection, std::vector<GraphObject::Ptr>& ioShaperList);
		GraphCollectionObject* hPickCollectionForGM(Int32 iIndex, std::vector<GraphCollectionObject::Ptr>& irCollections); 


		//overloaded function:used for scatter chart
		/**
		@param iScatterMarkerSize, the Scatter marker size;
		*/
		//void hGenerateMarker(RecMatrix& iorRecMatrix,  std::vector<Point2D>& irCoordPoints, Int32 iScatterMarkerSize, Int32 iSeriesIndex, GraphCollectionObject* ipCollection);
		
		//helper function to generate trendlines for all the series held by this plot.
		void hCreateTrendLine();
        void hCreateGMTrendLine();
        void hCreateRefLines();
        Int32 hGetRefLineDir(Int32 iSeriesInfoIndex);
        TextObject::Ptr hCreateRefLineLable(Int32 iAxisIndex, Int32 iRefLineIndex);
        ValueAxis::Ptr hGetValueAxis(Int32 iAxisIndex);  //GM Support

		//helper function to generate the data labels for all the series held by this plot
		/**
		@param iorRecMatrix, the rectangle information for each genearted marker, it is used as a refecence rectangle for data label generation
		*/
		void hGenerateDatalabels(RecMatrix& irRecMatrix, GraphCollectionObject* ipColPtr = NULL);
        
        
        //////----------Data Label Methods---------///
        void hCreateDataLabelCollection(std::vector<GraphCollectionObject::Ptr>& iorCollections);
        DataLabelManager::Ptr hInitDataLabelManager();
        void hPrepareDataLabelPositions();
        void hInitDataLabels(const std::vector<std::vector<Rect2D> >& irMarkerMatrix, std::vector<GraphCollectionObject::Ptr>& iorCollections);
        
        //////----------Custom Min and Max---------///
        bool hIsUseCustomMinMax();
        bool hIsHostMarkerInDataAreaWithCustomMinMax(const Rect2D& irHostMarker);
	
				//////~~~~~~~~~ Smart Label Method Stuff ~~~~~~~~~~~~~~~~ /////
		//stage one : Greedy
		void hCreateSmartLabelCollection(std::vector<GraphCollectionObject::Ptr>& iorCollections);
		void hInitLabelMatrix(const std::vector<std::vector<Rect2D> >& irMarkerMatrix, std::vector<GraphCollectionObject::Ptr>& iorCollections);
		void hGetMarkerRectSet(const std::vector<std::vector<Rect2D> >&irRectMatrix, std::vector<Rect2D>& orRectSet) const;
		bool hIsFallOut(const Rect2D& irLabelRect);
		Int32 hIsOverlapped(const Rect2D& irLabelRect, std::vector<Rect2D>& irTargetSet, bool iIsTiltRect = false, bool iIsTiltTargets = false);	
		Int32 hIsOverlapped(const LabelInfo& irLabelInfo, std::vector<Rect2D>& irTargetSet, bool iIsTiltTargets = false);
		Int32 hGreedyPlacer(std::vector<Rect2D>& irMarkerRectSet, std::vector<Rect2D>& iorLabelRectSet, GraphCollectionObject::Ptr iLabelLinesPtr = NULL);
		bool hPlaceOneLabel(LabelInfo& iorLabel,  std::vector<Rect2D>& irMarkerRectSet, std::vector<Rect2D>& iorLabelRectSet, GraphCollectionObject::Ptr iLabelLinesPtr = NULL);

		//post stage one: Heuristic
		//bool hHeuristicAdjust(LabelInfo& iorLabel,  std::vector<Rect2D>& irMarkerRectSet, std::vector<Rect2D>& iorLabelRectSet, std::vector<Rect2D>& iorTiltLabelRectSet);//< for adjustment
		//Int32 hGetSuggestedStep(LabelInfo& iorLabel);

		//stage two: Grid Slicing & placement
		void hGetSuggestedSliceUnit(Rect2D& orRect);//get the unit rect	
		bool hInitGridMapOccupation(GridMap &iorGridMap); //inital occupation work		
		//bool hAllocateFreeSpace(GridMap &iorGridMap);
		bool hPlaceLeftOversIntoFreeSpace(GridMap &iorGridMap, GraphCollectionObject::Ptr iLabelLinesPtr = NULL);
		void hPlaceLeftOvers(GraphCollectionObject::Ptr iLabelLinesPtr);//place all the labels that does not stand a chance to avoid any overlapps

		//Data label line for smart label	
		bool hGenerateFeelerForOneLabel(LabelInfo& irLabelInfo, GraphCollectionObject* iopCollection, Int32 iSpace = 0); 
        
        //Get the the offset clipped out of by chart boundary
        void hGetClippedOffset(double iLowBoundary, double iHighBoundary,
                               double iRadius, const Point2D& irMarkerCenter, 
                               bool isXAxis, double& orLowOffset, double& orHighOffset);
        
        
        
	private:
		
		//Find the correct series index for the input series id. 0 will be retruned if there is no match between the input and any series ids.
		/**
		@param iSeriesID, the input series id.
		@return Return the series index info.
		*/
		Int32 hFindSeriesInfoID(Int32 iSeriesID) const;
		
		//helper function used when generating data labels, just to retrieve the data format.
		/**
		@param iSeriesInfoIndex, the input series index.
		@param iGroupID, the input group id.
		@param iVAIndex, the VA index.
		@param orText, the retrieved text.
		*/
		void hGetVAFormattedText(Int32 iSeriesInfoIndex, Int32 iGroupID, EnumValueIndex iVAIndex, MBase::String& orText, GraphCollectionObject* ipCollection = NULL);
		void hGetGridCellFormattedText(Int32 iSeriesInfoIndex, Int32 iGroupID,EnumValueIndex iVAIndex, MBase::String& orText, GraphCollectionObject* ipCollection = NULL);
		//helper fuction used when generating markers, to compute the data marker size
		/**
		@param orMarkersize, the retrieved marker size.
		@param iSeriesInfoIndex, the input series index.
		@param iGroupID, the input group id.
		@param ipCollection, the collection to hold the text.
		@Return false if the data swuite is null or the marker is offscaled for hide. Otherwise true.
		*/
		bool hGetMarkerSize(Int32& orMarkerSize, double& orMarkerSizeValue, Int32 iSeriesInfoIndex, Int32 iGroupId = -3);

		//helper fuction used to compute the maker position
		/**
		@param iSeriesInfoIndex, the input series index.
		@param iGroupID, the input group id.
		@param orPosition, the retrieved marker position.
		@Return false if the data is null or the marker is offscaled for hide. Otherwise true.
		*/
		bool hGetMarkerPositon(Int32 iSeriesInfoIndex, Int32 iGroupId, Point2D& orPosition);			

		//helper function to generate the data label for one data group (<x,y> for scatter, <x,y,z> for bubble) of this plot
		/**
		@param iSeriesInfoIndex, the input series index.
		@param iGroupID, the input group id.
		@param ipCollection, the pointer of the managing collection .
		@return The data label text of this single data group
		*/
		TextObject::Ptr hCreateDataLabel(Int32 iSeriesInfoIndex, Int32 iGroupID, GraphCollectionObject* ipCollection);
        
		//helper function to generate the data label for scatter chart, called by hCreateDataLabel
		/**
		@param iSeriesInfoIndex, the input series index.
		@param iGroupID, the input group id.
		@param ipCollection, the pointer of the managing collection .
		@return The data label text of this single data group
		*/
		TextObject::Ptr hCreateDataLabelForScatter(Int32 iSeriesInfoIndex, Int32 iGroupID, GraphCollectionObject* ipCollection);
		
		//helper function to generate the data label for bubble chart, called by hCreateDataLabel
		/**
		@param iSeriesInfoIndex, the input series index.
		@param iGroupID, the input group id.
		@param ipCollection, the pointer of the managing collection .
		@return The data label text of this single data group
		*/
		TextObject::Ptr hCreateDataLabelForBubble(Int32 iSeriesInfoIndex, Int32 iGroupID, GraphCollectionObject* ipCollection);
		
        //helper function to generate the data label for bubble and scatter chart only for Graph Matrix
		/**
         @param iSeriesInfoIndex, the input series index.
         @param iGroupID, the input group id.
         @param ipCollection, the pointer of the managing collection .
         @return The data label text of this single data group
         */
		TextObject::Ptr hCreateDataLabelForGraphMatrix(Int32 iSeriesInfoIndex, Int32 iGroupID, GraphCollectionObject* ipCollection);
        
		//helper function to move the data labels to a decent position around the its hosting data marker.
		/**
		@param ipText, the data label text.
		@param irRefRect, the rectangle indicate the position of the hosting marker
		@param iRotation, indicate the rotaton inforamtion of the data label text. 
		*/
		void hMoveDataLabel(TextObject* ipText, const Rect2D& irRefRect, EnumDSSGraphFontRotation iRotation = DssGraphFontRotateNormal, GraphCollectionObject* ipCollection = NULL); 
		void hGetFeelerLinePosition(Point2D& orVexOnMarker, Point2D& orVexOnLabel, const Rect2D& irMarkerRect, Int32 iSpace, EnumDSSGraphDataTextPosition iLocation); 		
		void hGetFeelerLinePosition(Point2D& orVexOnMarker, Point2D& orVexOnLabel, const Rect2D& irMarkerRect, const Rect2D& irLabelRect); 

		Int32 hPutDataLabel(TextObject* ipText, const Rect2D& irRefRect, EnumDSSGraphFontRotation iRotation = DssGraphFontRotateNormal, EnumDSSGraphDataTextPosition iLabelLocation = DssGraphDTPositionCenter);
		Int32 hPutDataLabel(TextObject* ipText, const Rect2D& irRefRect, const Point2D& irTargetPoint, EnumDSSGraphFontRotation iRotation = DssGraphFontRotateNormal); //used for smart label
        void hPutDataLabel(TextObject* ipText, const Rect2D& irHostMarkerBound, EnumDssGraphMatrixDataLabelPosition irLocation); //use for data label, to get the data label position

        //Max Y marker
        TripleId mMaxYMarker;

		//helper function : get the max radius for one marker if we slice the plot area by those number of markers
		/*
		@return the max radius for the unit.
		*/
		Int32 hGetMaxRadiusForUnitInDevice();
        
		/**	For GraphMatrix when Size = Automatic.
		Give the suggested maximun radius to a visible marker/riser according to different graph type.
		By default (when iGeneric is true) it will give the smaller between iDataArea.Width/lXunitCount	and iDataArea.Height/iYUnitCount
	    @param iDataArea Specifies the plot area in each chart area.
	    @param iXUnitCount Specifies the number of marker/riser along X axis.
		@param iYUnitCount Specifies the number of marker/riser along Y axis.
		@param iIsGeneric Specifies whether graphtype should be considered.
		@return the suggest value for max raidus.	
		*/
		Int32 hGetAdjustedMarkerRadius(const Rect2D& iDataArea, Int32 iXUnitCount, Int32 iYUnitCount, bool iIsGeneric = true);
		
        ValueAxis::Ptr hGetValueAxisPtrByIndex(Int32 iIndex) const;
		void hGetOneAxisOriginLable(ValueAxis::Ptr iXValueAxis ,ValueAxis::Ptr iYValueAxis , std::vector<Point2D>& orLablePointVec, std::vector<MBase::String>& orLableTextVec);
        
		Int32 mMaxUnitRadius; ///< the max radius for one marker in pixel. 
		bool mDataLabelLine; ///< show data label line or not, for bubble only
		bool mIsBubbleChart; ///< if the chart type is bubble, use it for better performance.
		bool mIsSmarted;     ///< whether we use smart label or not.
		double mMaxRadius;	 ///< max raidus.	
		bool mIsDrawBySize;  ///< whether we Draw bubble by size or not.
		

		Int32								mLabelFormat;	///< Data label format
		EnumDSSGraphDataTextPosition		mLabelLocation;	///< Data label location
		ValueAxis::Ptr mXAxisPtr;	///< X-Axis ptr
        ValueAxis::Ptr mX2AxisPtr;  ///< X2-Axis ptr
		ValueAxis::Ptr mY1AxisPtr;	///< Y1-Axis ptr
		ValueAxis::Ptr mY2AxisPtr;	///< Y2-Axis ptr
		ValueAxis::Ptr mZAxisPtr;	///< Z-Axis ptr
		std::vector<ScatterSeriesInfo> mVecSeriesInfo; ///< the vector of series information
		std::vector<Rect2D> mStandAloneMarkers; //< A set of marker with no labels shown.
		LabelMatrix mLabelMatrix;   //< label matrix for smart label
        GMDataLabelVecs mDataLabelVec;
        
        // GM TrendLine Support
        std::vector<GraphTrendLineOptions> mGMTrendLineOption;
        void LoadGMTrendLineProperties(Int32 iGroupId);
        
        PiePlotRenderer::Ptr mPieRender;

		//////~~~~~~~~~ Smart Label Member Stuff ~~~~~~~~~~~~~~~~ /////
		//member methods <for heuristic adjustment
		bool hIsOverlappedWithHLine(const Point2D& irStart, const Point2D& irEnd, const Point2D& irHStart, const Point2D& irHEnd) const;		
		bool hIsOverlappedWithVLine(const Point2D& irStart, const Point2D& irEnd, const Point2D& irVStart, const Point2D& irVEnd) const;		
		bool hIsTiltLineOverlappedWithNormalRect(const Point2D& irStart, const Point2D& irEnd, const std::vector<Point2D>& irPoints) const;
		void hGetPrecisePointsForTiltRect(const LabelInfo& irLabelInfo, std::vector<Point2D>& orPoints);
        
        
        /// the entry method to hanlde data label stuff for all cases.
        void hFactoryHandleDataLabels(std::vector<std::vector<Rect2D> >& irVecMarkerRectangle, bool isDataLabelPossible);
        
		};

		inline bool ComparisionOfScatterLabelInfo(const LabelInfo& iFirst, const LabelInfo& iSecond)
		{
			return iFirst.mHostMarker.x < iSecond.mHostMarker.x;
		}


}
#endif