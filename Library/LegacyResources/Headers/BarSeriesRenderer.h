//=======================================================================
//	Filename:		BarSeriesRenderer.h
//	Date:			2008-10-06
//	Authors:		Bing Huang

//	Copyright (C) 1997-2008 MicroStrategy Incorporated
//	All rights reserved
//=======================================================================

#ifndef MsiChart_BarSeriesRenderer_h
#define MsiChart_BarSeriesRenderer_h

#include "SeriesRenderer.h"
#include "GraphCollectionObject.h"
#include "DataLabelManager.h"

namespace MsiChart
{
    /*struct BarDataLabelInfo
    {
        bool mIsDisPlayed;  //<Whether this label is already been located
        bool mIsValid;      //Is Host marker valid ?
        
        Int32 mSeriesId;
        Int32 mGroupId;
        
        Int32 mSeriesInfo;

        Rect2D mHostMarkerBound;
        
        TextObject::Ptr mLabelPtr;
        
        BarDataLabelInfo(): mSeriesId(0),mGroupId(0), mIsDisPlayed(false), mIsValid(false)
        {}
        
        Rect2D ToRectangle()
        {
            Rect2D lRect;
            mLabelPtr->GetBoundingRect(lRect);
            return lRect;
        }
    };
    
    typedef std::vector<BarDataLabelInfo> DataLables;*/
    
	///	Options related to Bar graph
	struct BarOptions
	{
		BarOptions(): mOverlap(0),mGapWidth(40),
			mRiserShape(0),mShowErrorBar(false),mHasMiddleValue(false),
			mMiddleValueSize(50), mShowSeriesLines(false),
			mColorBySeries(true){}
		Int32			mOverlap;	///< Overlap value
		Int32			mGapWidth;	///< Gap width between groups
		unsigned char	mRiserShape;	///< Riser's shape, only "Rectangle"
		bool			mShowErrorBar;	///< Flag indicates whether to show Error Bar or not
		bool			mHasMiddleValue;	///< Flag indicates whether to draw middle value in Error Bar graph
		Int32			mMiddleValueSize;	///< Middle value's size
		bool			mShowSeriesLines;	///< Whether to show series lines
		bool			mColorBySeries;		///< Color mode: by series or by category
	};

	/// RiserInfo --- SeriesInfoIndex and GroupID pair
	struct RiserInfo
	{
		RiserInfo(Int32 iGroupID,Int32 iSeriesInfoID)
			: mGroupID(iGroupID),mSeriesInfoID(iSeriesInfoID)
		{}
		Int32	mGroupID;
		Int32	mSeriesInfoID;

		virtual Int32 GetGroupIndex();
	};

	inline Int32 RiserInfo::GetGroupIndex() {return mGroupID;}
	
	struct RiserInfoWithIndex : public RiserInfo //used by Pareto chart...
	{
			
		RiserInfoWithIndex(Int32 iGroupID,Int32 iSeriesInfoID) : 
		RiserInfo(iGroupID, iSeriesInfoID)	
		{
			mGroupIndex = iGroupID;
		}	
		

		RiserInfoWithIndex(Int32 iGroupID, Int32 iSeriesInfoID, Int32 iGroupIndex) : 
		RiserInfo(iGroupID, iSeriesInfoID)	
		{
			mGroupIndex = iGroupIndex;
		}	
	

		Int32 mGroupIndex;

		virtual Int32 GetGroupIndex();
	};

	inline Int32 RiserInfoWithIndex::GetGroupIndex(){return mGroupIndex;}

	/// SeriesLine Info
    
    enum SERIES_LINEINFO_CLEARTYPE
    {
        SLC_FRONT = 0,
        SLC_BACK,
        SLC_BOTH
    };
    
	struct SeriesLineInfo
	{
		struct PointInfo
		{
			PointInfo():mpFront(NULL), mpBack(NULL){}
 
            ~PointInfo()
			{
                Clear(SLC_BOTH);
			}
            
            void Clear(SERIES_LINEINFO_CLEARTYPE iType)
            {
                if (iType == SLC_FRONT || iType == SLC_BOTH)
                { 
                    if (mpFront)
                    {
                        delete mpFront; mpFront = NULL;
                    }
                }
                
                if (iType == SLC_BACK || iType == SLC_BOTH)
                {    
                    if (mpBack)
                    {
                        delete mpBack; mpBack = NULL;
                    }
                }   
            }

			Point2D* mpFront;
			Point2D* mpBack;
		};
	
		SeriesLineInfo(Int32 iGroupCount, Int32 iSeriesCount):
			mGroupCount(iGroupCount),mSeriesCount(iSeriesCount)
		{
			
			if (iGroupCount > 0 && iSeriesCount > 0)
				mpPointInfo = new PointInfo[iSeriesCount * iGroupCount];
		}
		~SeriesLineInfo()
		{
			delete [] mpPointInfo;
		}

		void SetPoint(Int32 iGroupID, Int32 iSeriesID, const Point2D& irPoint, bool iFront)
		{
			if (iFront)
            {    
                mpPointInfo[iGroupID + iSeriesID * mGroupCount].Clear(SLC_FRONT);
				mpPointInfo[iGroupID + iSeriesID * mGroupCount].mpFront = new Point2D(irPoint);
            } 
			else
            {
                mpPointInfo[iGroupID + iSeriesID * mGroupCount].Clear(SLC_BACK);
				mpPointInfo[iGroupID + iSeriesID * mGroupCount].mpBack = new Point2D(irPoint);
            }
		}

		bool GetPoint(Int32 iGroupID, Int32 iSeriesID, Point2D& orPoint, bool iFront)
		{
			if (iFront)
			{
				if(mpPointInfo[iGroupID + iSeriesID * mGroupCount].mpFront)
                {
					orPoint = (*mpPointInfo[iGroupID + iSeriesID * mGroupCount].mpFront);
                    return true;
                }
			}
			else
			{
				if(mpPointInfo[iGroupID + iSeriesID * mGroupCount].mpBack)
                {
					orPoint = (*mpPointInfo[iGroupID + iSeriesID * mGroupCount].mpBack);
                    return true;
                }
			}
            
            return false;
		}
	
		PointInfo* mpPointInfo;

		Int32 mGroupCount;
		Int32 mSeriesCount;
	};

	///	Enumeration of Bar Rendering type, corresponding to 4 layout type
	enum EnumBarRenderType
	{
		BAR_ABSOLUTE,
		BAR_CLUSTER,
		BAR_STACKED,
		BAR_PERCENT,
	};

	/**
	 *	Render each series in a Bar shape(Error Bar included)
	 */
	class BarSeriesRenderer : public SeriesRenderer
	{
	public:
		///	Type definition
		typedef MSynch::SmartPtr<BarSeriesRenderer> Ptr;

		/**
		 *	Constructor,params are passed to base class
		 */
		BarSeriesRenderer(const TripleId& irTripleId, GraphObjectManager* ipManager,
							Dataset::Ptr iDatasetPtr, Legend::Ptr iLegendPtr);

		virtual ~BarSeriesRenderer()
		{
			if (mpSeriesLineInfo)
			{
				delete mpSeriesLineInfo;	mpSeriesLineInfo = NULL;
			}
		}

		virtual void GenerateMapAndList();

		/**
		 *	Generate graph objects according to the input EnumValueAxis
		 *	and put the objects into the input List by the caller
		 *	@param iValueAxis ValueAxis index, VA_Y1 or VA_Y2
		 *	@param iorGraphObjectList List provided by the caller
		 */
		virtual void GenerateByValueAxis(EnumValueAxis iValueAxis, std::vector<GraphObject::Ptr>& iorGraphObjectList);


		/**
		 *	Override base class' implementation, calculate bar width for later use
		 *	@param irDeviceDataArea Device data area
		 */
		void SetDeviceDataArea(const Rect2D& irDeviceDataArea);

		virtual void GetOptions(Int32 iSeriesId, Int32 iPropertyId, CComVariant* opValue) const;

		///	Add the trend line to orTrendLineObjects, called by ABLPlot
		virtual void AddTextToMapAndList(std::map<TripleId, GraphObject::Ptr>& irGraphObjectMap, std::vector<GraphObject::Ptr>& irGraphObjectList);

		///	Add the trend line to orTrendLineObjects, called by ABLPlot
		void hGenerateTrendLine();
		
		/// Provide position information for tooltip display on iPad.
		virtual bool GetDataPoint(Int32 iSeriesId, Int32 iGroupId, Point2D& orDataPoint);
        
        /**
         Reserve area for data labels.
         @param iorRect Shrink iorRect when necessary so that the data labels will not be cut at the boundary of the graph.
         */
        void ReserveAreaForDataLabels(Rect2D& iorRect);
        
        //----Data Label for Graph Matrix---//
        bool ShouldRotateDataLabel();
        void AddObjectsToDataLabelManager(DataLabelManager::Ptr iDataLabelManagerPtr);
        void AddObjectsToDLManagerForAxisOrigin(DataLabelManager::Ptr iDataLabelManagerPtr);
    protected:
		///	Load default bar graph related properties from ChartContext
		virtual void LoadProperties();

		/** Render all the items need to be rendered
		 @param iVA VA_Y1, VA_Y2 or VA_BOTH.
		 @param iIsOnlyForDataLabels If iIsOnlyForDataLabels is true, only needs to
		 generate data labels. If iIsOnlyForDataLabels is false, needs to gerenate all necessary GraphObjects including bars, series lines, data labels, trend lines, etc.
		 */
		virtual void hRenderItems(EnumValueAxis iVA, BOOL iIsOnlyForDataLabels);

		/** This is a helper method which is extracted from GenerateByValueAxis.
		 @param iValueAxis VA_Y1, VA_Y2 or VA_BOTH.
		 @param iIsOnlyForDataLabels If iIsOnlyForDataLabels is true, only needs to 
		 generate data labels. If iIsOnlyForDataLabels is false, needs to gerenate all necessary GraphObjects including bars, series lines, data labels, trend lines, etc.
		 */
		void hGenerateByValueAxis(EnumValueAxis iValueAxis, BOOL iIsOnlyForDataLabels);
        
	//private:

        ///-------------Data Label Methods------------///
        bool hShouldRotateDataLabel();
        Int32 hGetDataLabelNumber(bool iIsRotated);
        void hCreateDataLabels(std::vector<Rect2D> &iorBarsRect, std::vector<Rect2D> &iorDLRect, bool iIsRotated, bool iIsOnlyForRect = false);
        void hCreateOneDataLabelItem(const RiserInfo& irRiserInfo, std::vector<Rect2D> &iorBarsRect, std::vector<Rect2D> &iorDLRect, bool iIsRoteated, bool iIsOnlyForRect = true);
        bool hIsFitPaddingToBorder(const Rect2D &irDLRect, const Int32 &irPadding);
        bool hIsTwoRectFitPadding(const Rect2D &irDLRect, const Rect2D &irRect, const Int32 &irPadding);
        void hPutOneDataLabel(const Rect2D &irBarRect, TextObject::Ptr irDLText, double irValue, bool iIsForJudgeRotate = false, bool iIsRotated = false); // used for both verticla and horizontal bar, cal the data label's position
        void hPutOneDataLabel(const Rect2D &irHostRect, TextObject::Ptr irLDText, EnumDssGraphMatrixDataLabelPosition irLocation, bool iIsRotated);
        //TextObject::Ptr hCreateDataLabelForGraphMatrix(const double &irValue, GraphCollectionObject *ipCollection);
        void hRenderDataLabels();  //detect overlap, show data labels
        GMDataLabelVecs mDataLabes;
        
        /// Data Label~
        
		/// Get the general Bar rendering type
		void hGetRenderingType();
		
		/// For one group, calculate the series order
		void hCalSeriesOrderForOneGroup(Int32 iGroupIndex, std::vector<Int32>& orSeries, EnumValueAxis iVA); 

		
		///	Find the riser collection
		GraphCollectionObject* hFindRiserCollectionOrInsert(const RiserInfo& irInfo);

		///	Get the number of value axis that assigned
		Int32 hGetNumberOfVA() const;

		///	Calcuated the drawing order for Value Axis
		void hGetVAOrder(EnumValueAxis& orFirst, EnumValueAxis& orSecond);

		///	Get the series info ID for the specified ValueAxis index
		void hGetSeriesByVA(std::vector<Int32>& irSeries,EnumValueAxis iVA);

		///	Get the series order by rendering type for one group
		void hGetSeriesOrderByType(Int32 iGroupIndex, std::vector<Int32>& orSerie, EnumValueAxis iVA);

		///	Get the riser's start position and size
		virtual void hGetRiserStartPosAndSize(const RiserInfo& irRiserInfo, Int32& orStart, Int32& orSize);

		/**
		Used to get trend line X data.
		*/
		void hGetLogicalMiddlePosition(Int32 iSeriesIndex, Int32 iGroupId, double& orPosition) const;
	
		/// Get the index along the Category Axis for each riser
		virtual Int32 hGetIndexInColumn(const RiserInfo& irRiserInfo);

        /// Get group index for a tripleId.
        Int32 hGetGroupIndex(const RiserInfo& irRiserInfo);
        
		///	Calculate the bar width
		virtual void hCalBarWidth();
        
        //lizhang, 2016-06-21, US55868, Calculate how many series clusters in combo chart when subtype is Absolue(Cluster)
        void hCalClusterNum();
		
		//	Calculate the drawing order for 3 different kinds of render
		//	Need to consider 3 factors
		//	1. Depth direction	2. Plot Orientation  3. ValueAxis.IsReverseOrder 4. Data
		
		///	Adjust the drawing order for considering Negative value
		void hAdjustSeriesOrderForNegativeValue(Int32 iGroupIndex, std::vector<Int32> &iorVecSeries);
	
		/**	Create one riser, label and add 1 sample point, sometimes ErrorBar information
		 @param iIsOnlyForDataLabels If iIsOnlyForDataLabels is true, only needs to
		 generate data labels. If iIsOnlyForDataLabels is false, needs to gerenate all necessary GraphObjects including bars, series lines, data labels, trend lines, etc.
		 */
		void hRenderOneItem(const RiserInfo& irRiserInfo, std::vector<std::pair<Int32, TextObject::Ptr> >& orTextForGroup, BOOL iIsForReserveArea);
        bool hIsSeperateMetricForClusterBar();

        //The followings are helper functions called by hRenderOneItem
        bool hPrepareData(const RiserInfo& irRiserInfo, double& orValue);
        Rect2D hPrepareRect(const RiserInfo& irRiserInfo, double iDataValue, bool& orAboveZero);
        GraphObject::Ptr hPrepareRectangleObject(const RiserInfo& irRiserInfo, const Rect2D& irRectBar, bool iAboveZero);
        //lizhang, 2016-06-20, US55868, new shapes in combo
        void hGetSingleShapeCenter(Point2D& iCenter, const Rect2D& iRectBar, bool iAboveZero);
        GraphObject::Ptr hprepareSingleShape(const TripleId& irTripleId, const Point2D& irPoint, bool iIsLineAsDots);
        void hGetSingleShapeSize(Int32& iMarkersize, const TripleId& irTripleId, bool iIsLineAsDots);
        
        void hPrepareAnimationForOneBar(const RiserInfo& irRiserInfo, const Rect2D& irRectBar, GraphObject::Ptr iObjectPtr);
        void hPostProcessOneBar(const RiserInfo& irRiserInfo, GraphObject::Ptr iObjectPtr);
        void hPrepareErrorBar(const RiserInfo& irRiserInfo, const Rect2D& irRectBar);
        void hPrepareGMRiserLabel(const TripleId& ID, const RiserInfo& irRiserInfo, const Rect2D& irRectBar, const Rect2D& irOldRectBar, double iDataValue);
        bool hCheckBarRect(const Int32 iSeriesIndex, const Rect2D& irRect);
        
        bool hIsTickShapeForGMObject(const TripleId& iID);
        
        /** This function is to create rect for one bar, can be resued in hRenderOneItem and hGetOneItemRect.
         */
        bool hGetOneBarRect(const RiserInfo& irRiserInfo, Rect2D& iorRect, bool iIsOnlyForDataLabels = true);
        
		///	Create the label for one riser
		TextObject::Ptr hCreateRiserLabel(const RiserInfo& irRiserInfo, const Rect2D& irRefRect, GraphCollectionObject::Ptr iColPtr);

		/// Move label further in case they go beyond the chart area.
		void hCheckLabelForClipping(const RiserInfo& irRiserInfo, TextObject::Ptr iTextObjectPtr, const Point2D& irMax, const Point2D& irMin);

		///	Get the sample point of trendline one riser
		bool hGetSamplePoint(const RiserInfo& irRiserInfo, Point<double>& orSample);

		/// Get series line point
		void hGetSeriesLinePoint(const RiserInfo& irRiserInfo, const Rect2D& irRefRect, Point2D& orFront, Point2D& orBack);

		///	Generate series line for specified value axis - usually it's VA_Y1
		void hGenerateSeriesLine(EnumValueAxis iValueAxis);

		void hSmartLabelForCluster(EnumValueAxis iVA, std::vector<TextObject::Ptr>& irTextForGroup);

		bool hIsLegendMarkerAddedToCollection(Int32 iSeriesID);
		
        /** Update the bar size as per GM requirement.
         */
		bool hUpdateBarRectSize(const RiserInfo& irRiserInfo, Rect2D& iorRect, bool iIsDataPointAboveZero, double iDataValue);
		bool hUpdateBarRectSizeToTickMode(const RiserInfo& irRiserInfo, Rect2D& iorRect, bool iIsDataPointAboveZero, double iDataValue);
        
		/// Get Bar Riser width according to different scenario. (Normal, GraphMatrix<and sub size mode>)	
		double hGetBarRiserWidth(const RiserInfo& irRiserInfo);
		double hGetBarThickness(const RiserInfo& irRiserInfo);
		bool hUpdateSpecificColor(const TripleId& irId, GraphObject::Ptr lPtr);
		void hPrepareForAnimation();
        //lizhang, de38822, 20160727, reform case in combo
        bool multipleShapes();
        
        /**
         *This method would check for one textObject, if or not it has reach out the denoted area(specified internally in the logic).
         *@param iorRect, the area to be adjusted.
         *@param ipTextObject, the textObject to be tested against
         *@return True means that the adjustment is valid
         */
        bool hReserveAreaForDataLabel(Rect2D &iorRect, TextObject* ipTextObject);
		virtual void hReserveAreaForDataLabel(Rect2D &iorRect); //used by subclass.
        virtual void hClearUp();
        virtual bool hIsLabelShown();
        
		/// map which holds Series/GroupID -> Collection object pointer
		std::map<Int32, GraphCollectionObject::Ptr> mSGMapLabel;
		/// map which holds Series/GroupID -> sample points
		std::map<Int32, std::vector<Point<double> > > mSGMapTrend;
		/// HOLDS information for series line information
		SeriesLineInfo* mpSeriesLineInfo;	///< Series Line Info

		std::map<Int32, Rect2D> mMapTotalRect[2];	///< Holds information for total labels, one for each Axis

		EnumBarRenderType mBarRenderType;	///< Bar Rendering Type
		double mBarWidth;		///< Bar's width
		double mGroupSize;
		double mSpaceBetweenRiser;
		double mSpaceBetweenGroup;
		Int32 mBarColumns;		///< How many columns within one group
        //lizhang, 2016-06-21, US55868, how many series clusters in combo chart when subtype is Absolue(Cluster)
        Int32 mBarClusters; 
		BarOptions mBarOptions;		///< Internal bar options

		std::map<Int32, bool> mAddedLegendMarkerSet;	///< which legend Marker has been added to collection
        Int32 mLastSeenWidth;                           ///< used in cluster bar in GraphMatrix to record the last seen size of the bar, usually the 1st bar. 
        std::map<Int32, Int32> mBarSeriesInfoToIndex;
        bool mReformedCombo;//lizhang, 20160728, de38822, indicate whether data is reformed in combo
        
	};
}

#endif
