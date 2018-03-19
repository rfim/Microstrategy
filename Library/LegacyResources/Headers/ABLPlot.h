//=======================================================================
//	Filename:		Kernel/SourceCode/IPhoneChart/PrivateSource/ABLPlot.h
//	Date:			2009-02-18
//	Authors:		Bing Huang

//	Copyright (C) 1997-2008 MicroStrategy Incorporated
//	All rights reserved
//=======================================================================

#ifndef MsiChart_ABLPlot_h
#define MsiChart_ABLPlot_h

#include "CategoryPlot.h"
#include "MarkerObject.h"
#include "NumberObject.h"
#include "AreaSeriesRenderer.h"
#include "BarSeriesRenderer.h"
#include "LineSeriesRenderer.h"
#include "StepLineSeriesRenderer.h"
#include "HiBarSeriesRenderer.h"
namespace MsiChart
{
	//enumerations and structs
	typedef enum
	{
		MGT_AREA,
		MGT_BAR,
		MGT_LINE,

		MGT_PARETO //FOR PARETO CHART

	}EnumMajorGraphType;

	typedef enum
	{
		LO_ABSOLUTE,
		LO_CLUSTER,
		LO_STACKED,
		LO_PERCENT
	}EnumLayout;

	

	typedef enum
	{
		GST_NULL,
		GST_BIPOLAR,
		GST_DUALAXIS
	}EnumGraphSubType;

	typedef enum
	{
		VT_ABSOLUTE,
		VT_CUMULATE
	}EnumValueType;
	
	struct ABLOptions
	{
		ABLOptions() : mGraphType(DssGraphTypeVertical_Line_Absolute)
			, mMajorGraphType(MGT_LINE), mLayout(LO_ABSOLUTE)
			, mOrientation(OR_VERTICAL), mGraphSubType(GST_NULL)
			, mUseDepth(false), mDualAxes(false), mCombinationGraph(false), mSplitDualAxes(false), mShowAxisSplit(true)
			, mDepth(200), mDepthDirection(45), mSplitAxisPosition(50)
			, mShowAsPercentage(false), mShowTotal(true), mShowCumulate(false)
			, mShowLabel(false), mShowValue(false), mCurvedLines(false), mShowLines(true)
			, mUseGlobalAutoScale(true)
			, mTextPosition(DssGraphDTPositionOutMax)
			, mTextShowType(DssGraphDataTextValue)
		{}

		EnumDSSGraphType		mGraphType;
		EnumMajorGraphType		mMajorGraphType;
		EnumLayout		  		mLayout;
		EnumOrientation			mOrientation;
		EnumGraphSubType		mGraphSubType;
		EnumValueType			mValueType;	
		bool					mUseDepth;
		bool					mDualAxes;
        bool                    mCombinationGraph;
		bool					mSplitDualAxes;
		bool					mShowAxisSplit;
		bool					mShowLabel;
		bool					mShowValue;
		bool					mShowAsPercentage;
		bool					mShowTotal;
		bool					mShowCumulate;	//true if cumulate
		bool					mCurvedLines;
		bool					mShowLines;
		bool					mUseGlobalAutoScale;
		Int32					mDepth;
		Int32					mDepthDirection;
		Int32					mSplitAxisPosition;
		EnumDSSGraphDataTextPosition		mTextPosition;
		EnumDSSGraphDataTextShowType		mTextShowType;
		ChartAdvFormat						mTotalNumberFormat;
	};

	typedef enum
	{
		ST_AREA,
		ST_BAR,
		ST_LINE,
		ST_STEPLINE,
        ST_PIE,
        ST_RING,
        ST_CIRCLE,
        ST_SQUARE,
        ST_TICK,
		ST_DEFAULT,
        ST_COMBINATION
	}EnumSeriesType;

	struct SeriesOptions
	{
		typedef MSynch::SmartPtr<SeriesOptions> Ptr;

		SeriesOptions() : mSeriesType(ST_DEFAULT), mRealSeriesType(ST_LINE), 
			mValueAxis(VA_Y1), mShowDataLabels(false) 
		{}

		EnumSeriesType				mSeriesType;   //The actual series type set by GUI.
		EnumSeriesType				mRealSeriesType;   //Decide whick seriesrender to be called for drawing this series.
		EnumValueAxis				mValueAxis;
		bool						mShowDataLabels;	
		GraphTrendLineOptions		mTrendLineOptions;
		ChartAdvFormat				mGraphNumberFormat;
	};

	//struct ParetoPercentInfo{
	//	std::vector<Int32> mGroupOrder;
	//	bool mIsShowY1Series;
	//	bool mIsShowY2Series;
	//};

	typedef enum
	{
		MGR_AREA,
		MGR_BAR,
		MGR_LINE,
		MGR_PLOT
	}EnumManager;

	//use a struct so that other class could access it easier
	struct ABLPlotContext
	{
		/*vector index n indicates series n*/
		std::vector<SeriesOptions::Ptr> mSeriesOptions;
		//common options
		ABLOptions mABLOptions;

		std::vector<ValueAxis::Ptr> mValueAxis;
		CategoryAxis::Ptr mCategoryAxisPtr;
	};

	

	class ABLPlot : public CategoryPlot
	{
	public:
		typedef MSynch::SmartPtr<ABLPlot> Ptr;

		ABLPlot(const TripleId& irTripleId, GraphObjectManager* ipManager, const Rect2D& irDataArea);

		//pure virtual functions from GraphObjectManager
		virtual void GenerateMapAndList();
		virtual void GetOptions(Int32 iSeriesId, Int32 iPropertyId, CComVariant* opValue) const;

		//override functions
		virtual void Draw();
		virtual void GenerateView();
		virtual void GenerateImageMap(ImageMap& iorImageMap); 
		virtual GraphObject::Ptr OnPointDetection(const Point2D& irCursor) const; 
		virtual void OnRectangleSelection(const Rect2D& irRect, std::vector<GraphObject::Ptr>& iorSelectedObjects) const;

		virtual bool GetToolTipData(Int32 iSeriesID, Int32 iGroupID, double& orValue, double& orPercentValue, Int32 iOffset = 0);
		
		virtual void DrawTrendLinesAndEquations();

		/**
		Add a value axis to the category plot.
		There may be one or two axis for a categtory plot when it is dual axis or bipolar chart.
		For chart areas there may be even more.
		@param iValueAxisPtr The input pointer to value axis. 
		*/
		virtual void AddValueAxis(ValueAxis::Ptr iValueAxisPtr);

        virtual void ReplaceValueAxis(ValueAxis::Ptr iValueAxisPtr, int iIndex);
        
		/**
		Set the category axis for the plot
		There can be only one category axis for a category plot currently.
		@param iCategoryAxisPtr The input pointer to category axis. 
		*/
		virtual void SetCategoryAxis(CategoryAxis::Ptr iCategoryAxisPtr);

		/**
		Get the pointer to the mABLPlotContext member.
		@return Pointer to mABLPlotContext. 
		*/
		ABLPlotContext* GetABLPlotContext();

		/**
		Get the major graph type of the chart, area, bar or line.
		@return The major graph type.
		*/
		EnumMajorGraphType GetMajorGraphType();

		/**
		Get the layout of the graph, absolute, cluster(for bar chart only), stacked or percent.
		@return The layout.
		*/
		EnumLayout GetLayout();

		/**
		Get the orientation of the graph, vertical or horizontal.
		@return The orientation.
		*/
		EnumOrientation GetOrientation();

		/**
		Get the sub graph type of the chart, dual axis, biplor or none.
		@return The sub graph type.
		*/
		EnumGraphSubType GetGraphSubType();

		/**
		Get the actual depth for each level.
		If you want to create a depth object, you should get the depth using this function,
		not from the mDepth of ABLOptions.
		@return The depth value.
		*/
		Int32 GetDepth() const;

		/**
		Get the device offset for the specified series ID.
		The offset you get is the actual coordinate value in the real chart.
		@param iSeriesIndex The input series index. 
		@param orOffsetX The output value for X direction offset. 
		@param orOffsetY The output value for Y direction offset. 
		*/
		void GetDepthOffset(Int32 iSeriesIndex, Int32& orOffsetX, Int32& orOffsetY) const;

		/**
		Get the group total value for the specified group ID.
		@param iGroupId The input group ID. 
		@return The group total value.
		*/
		double GetGroupTotals(Int32 iGroupId) const;

		/**
		Get the rectangle for specified value axis.
		@param iValueAxis The input value axis index. 
		@param orRect The output rectangle.
		*/
		void GetRectByValueAxis(EnumValueAxis iValueAxis, Rect2D& orRect) const;
		
		/**
		Get the min & max value of the data in this plot for specified value axis.
		It is used to set range of ValueAxis.
		@param iValueAxis The input value aixs index.
		@param iorMin The output minimum value.
		@param iorMax The output maximum value.
		@param iorAreInitialized Indicates whether iorMin and iorMax are initialized or not.
		*/
		virtual void GetMinMaxValue(EnumValueAxis iValueAxis, double& iorMin, double& iorMax, bool& iorAreInitialized);
		
		virtual void GetMinMaxValue(EnumValueAxis iValueAxis, double& iorMin, double& iorMax, bool& iorAreInitialized, Int32 iValueAxisDirection); //For Graphmatrix

        virtual bool ShouldRotateDataLabel();
		
		/**
		Initialize the renderes so that plot manager can call the data operations' function.
		*/
		virtual void Init();

		/**
		Check the layout.
		@return If absolute or cluster layout then return true, else false.
		*/
		bool IsAbsoluteOrCluster() const;

		/**
		Get the group drawing order for pareto (percent) chart
		@param iorGroupOrder to store the result.
		*/
		void GetGroupOrderForPareto(std::vector<Int32>& iorGroupOrder);

		/**
		Get the total value of the whole data set.
		@return the data set total value.
		*/
		double GetDatasetTotalForPareto();
		
		/**
		Get the series id for the last bar series. Used in Pareto Percent chart.
		We use it to retrive the data label from that bar series and use it as the data labels for line series
		Note: if we decide that we'll figure out the proper data labels for line series itself, we won't need this method.
		@param iLineSeriesId the series id for line series
		@return series id for the last bar series 
		*/
		Int32 MapLineSereisIdForPareto(Int32 iLineSeriesId);

		/**
		Indicate whether we draw the line series for pareto percent chart.
		@return true if we draw it, and false otherwise
		*/
		bool IsShowParetoY2Series();


		/**
		Get the group id for according to the given index into the group drawing order. Used in Pareto chart.
		Caller should gurantee that the input index falls inside the valid range of the group drawing range.
		@param iGroupDrawOrder : the sequence/index into the group drawing order 
		@return mGroupOrder[iGroupDrawOrder]
		*/
		Int32 GetGroupIndexByOrderForPareto(Int32 iGroupDrawOrder);
		
		/**
		Get the size of the group drawing set. Used in Pareto chart.
		Note, in some exterme cases, the mGroupOrder.size() < mGroupCount. But in most cases, the two are equal.
		@return the size of mGroupOrder
		*/
		Int32 GetGroupSizeForPareto() const;	
		
		/**
		Get the offset into the group drawing order for the input group-id. Used in Pareto chart.
		@param iGroupId the group id to be looked up
		@param iorOffset the result offset for that group id
		@return true if the input group id is valid(fall inside the group id set and we can find the offset for
		it) and false otherwise
		*/
		bool GetOffsetIntoGroupOrderForPareto(Int32 iGroupId, Int32& iorOffset);
        
        /**
         Reserve area for data labels.
         @param iorRect Shrink iorRect when necessary so that the data labels will not be cut at the boundary of the graph.
         */
        virtual void ReserveAreaForDataLabels(Rect2D& iorRect);
        
        //GM Combination Graph Support
        double GetCumulateData(EnumValueAxis iValueAxis, Int32 iDataIndex, Int32 iGroupId) const;

        
        void CreateAxisOriginLine();
        void CreateAxisOriginLineLabels();
	protected:
		/**
		Parse the graph type to sub types inlcluding major type, layout, orientation,
		sub type and dual axis information.
		@param iType Input Graph Type. 
		*/
		virtual void SetGraphType(EnumDSSGraphType iType);

	private:
		/**
		Initialize the depth level for each series.
		For "Cluster" layout, all series are in level 0.
		For "Absolute" layout, the level number is equal to series number, so the depth
		level for series is firstly defined by series type(Area->Bar->Line), and secondly
		by series ID.
		For "Stacked/Percent" layout, the level number is defined by the number of Renderer,
		so for each series the level is defined by series type.
		*/
		void hInitializeDepthLevel();

		/**
		Get the depth level for the specified series ID.
		@param iSeriesId The input series ID. 
		@return The depth level.
		*/
		//Int32 hGetDepthLevel(Int32 iSeriesId) const;

		/**
		Initialize the total value for each group.
		Negative value will be counted using fabs();

		For pareto chart it will always cumulate the real value as it is. 
		*/
		void hInitializeGroupTotals();

		/**
		Load plot level properties.
		*/	
		void hLoadProperties();


		/**
		Tell if this chart is Pareto or Pareto Percent chart.
		@return true if it is pareto (percent) chart, false otherwise.
		*/	
		bool hIsPareto();
		
		/**
		Tell if this chart is Pareto Percent chart.
		@return true if it is pareto percent chart, false otherwise.
		*/
		bool hIsParetoPercent();

		/**
		Assign series to the correct hosting value axis : Bar series => Y1 , Line series => Y2
		@param iIsForLineSeries: indicate if this is a pareto percent chart.
		*/
		void hLoadValueAxisInfoForPareto(Int32 iSereisIndex, bool iIsForLineSeries = false);

		/**
		Create a line series for pareto percent chart
		*/
		void hPopulateLineSereisForPareto();

		/**
		Get the max and min value among corresponding series to decide the scale for Value axis.
		Used by Pareto and Pareto Percent chart
		@param iValueAxis: indicate the hosting value axis (Y1 or Y2)
		@param iorMin: the minimal value, needed to decied the scale 
		@param iorMax: the maximul value, needed to decied the scale 
		*/
		void hGetMinMaxForParetoPercent(EnumValueAxis iValueAxis, double& iorMin, double& iorMax);
		
		/**
		Compute the total value of the whole dataset. Used by Pareto and Pareto Percent chart.
		*/
		void hSetDatasetTotal();

		/**
		Compute the drawing order of each group for pareto(percent) chart.
		*/
		void hSetGroupOrderForPareto();

		/**
		Load series level properties.
		@param iSeriesIndex The input series index. 
		*/
		void hLoadSeriesProperties(Int32 iSeriesIndex);

		/**
		Get the manager which is responsible for the property.
		@param iProperty The input property. 
		@return The GraphManagerObject enumeration.
		*/
		EnumManager hGetManagerFromProperty(EnumDSSGraphProperty iPropertyId) const;

		/**
		Judge whether two rectangle intersected and if intersected, get the intersection rectangle.
		@param irRectA The first input rectangle.
		@param irRectB The second input rectangle.
		@param orRect The intersection rectangle for irRectA and irRectB.
		@return true if intersected, false if not.
		*/
		bool hRectangleIntersection(const Rect2D& irRectA, const Rect2D& irRectB, Rect2D& orRect) const;

		/**
		Get the real series type.
		*/	
		void hGetRealSeriesType();

		SeriesRenderer::Ptr hGetRendererBySeriesID(Int32 iSeriesId);
        
        void hCalculateAxesAreas();

		/**
		Update the trend line text position and add them to map and list, collection.
		@param irTextmap The input map contain all the text object.
		*/	
		//void hUpdateTrendLineText(std::map<Int32, TextObject::Ptr>& irTextMap);

		//3 renderer in category plot
		AreaSeriesRenderer::Ptr	mAreaSeriesRendererPtr;
		BarSeriesRenderer::Ptr	mBarSeriesRendererPtr;
		LineSeriesRenderer::Ptr	mLineSeriesRendererPtr;
		StepLineSeriesRenderer::Ptr	mStepLineSeriesRendererPtr;

		//the context object
		ABLPlotContext mABLPlotContext;
		//the depth level
		std::vector<Int32> mDepthLevel;
		Int32 mUnitLevelDepth;
		

		//use in Percent layout only
		std::vector<double> mGroupTotals;
		std::vector<Rect2D> mAxesAreas;

		//used in pareto (percent) chart		
		std::vector<Int32> mGroupOrder;
		std::map<Int32, Int32> mGroupOrderMap;
		double mDatasetTotal;
		bool mShowParetoY2Series;
        
        // GM Combination Graph Support
        void hAddSeriesForGMCombinationGraph();
        void hUpdateCumulateData(EnumValueAxis iValueAxis);
        //Stacked/Percent Series can be in different sereisrender
        // So we need to calculate cumulated data in ABLPlot, not in sereisRender
        //data for Y1 and Y2, cumulate or not
		std::vector<double> mCumulateData[2];
		//series count for each axis
		Int32 mSeriesNumber[2];
	};

	//inline functions
	inline EnumMajorGraphType ABLPlot::GetMajorGraphType()
	{
		return mABLPlotContext.mABLOptions.mMajorGraphType;
	}

	inline EnumLayout ABLPlot::GetLayout()
	{
		return mABLPlotContext.mABLOptions.mLayout;
	}

	inline EnumOrientation ABLPlot::GetOrientation()
	{
		return mABLPlotContext.mABLOptions.mOrientation;
	}

	inline EnumGraphSubType ABLPlot::GetGraphSubType()
	{
		return mABLPlotContext.mABLOptions.mGraphSubType;
	}

	inline Int32 ABLPlot::GetDepth() const
	{
		return mUnitLevelDepth;
	}

	inline double ABLPlot::GetGroupTotals(Int32 iGroupId) const
	{
		return mGroupTotals[iGroupId];
	}

	inline bool ABLPlot::IsAbsoluteOrCluster() const
	{
		return (mABLPlotContext.mABLOptions.mLayout == LO_ABSOLUTE) || (mABLPlotContext.mABLOptions.mLayout == LO_CLUSTER);
	}
	
	inline Int32 ABLPlot::GetGroupSizeForPareto() const	
	{
		return (Int32)mGroupOrder.size();
	}

	inline bool SortForParetoGroup(const std::pair<size_t, double>& irFirst, const std::pair<size_t, double>& irSencond)
	{
		if (irFirst.second == irSencond.second)
			return irFirst.first > irSencond.first;
		return irFirst.second > irSencond.second;
	}

	
}

#endif
