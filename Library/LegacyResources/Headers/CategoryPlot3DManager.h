//====================================================================
//	Filename:		Kernel/SourceCode/ChartEngine/PrivateSource/CategoryPlot3DManager.h
//	AUTHOR		:	Bing Huang
//	CREATION	:	2009-05-19
//	Copyright (C) MicroStrategy, Inc. 2008
//==============================================================================================

#ifndef MSICHART_CATEGORYPLOT3DMANAGER_H
#define	MSICHART_CATEGORYPLOT3DMANAGER_H

#include "PlotManager.h"
#include "ValueAxis.h"
#include "CategoryAxis.h"
#include "WallManager.h"

namespace MsiChart
{
	// Enumerations and Structs
	typedef enum
	{
		ARS_AREA = 0,
		ARS_RIBBON,
		ARS_STEP
	}EnumAreaRibbonStep;

	struct TDSeriesOptions
	{
		typedef MSynch::SmartPtr<TDSeriesOptions> Ptr;

		TDSeriesOptions() : mSeriesType(DssGraph3DRiserBar), mShowDataLabels(true)
		{}

		EnumDSSGraph3DRiserType		mSeriesType;
		bool						mShowDataLabels;	
		ChartAdvFormat				mGraphNumberFormat;
	};

	/**
	CategoryPlot3DManager class is the combination of Plot and PlotManager.
	It doesn't suppose to be support chart areas.
	*/
	class CategoryPlot3DManager: public PlotManager
	{
	public:
		typedef MSynch::SmartPtr<CategoryPlot3DManager> Ptr;

		CategoryPlot3DManager(const TripleId& irTripleId, GraphObjectManager* ipManager, const Rect2D& irPlotArea);
		virtual ~CategoryPlot3DManager();

		//Re-implemention the virtual functions inherited from GraphObjectManager
		virtual void GenerateMapAndList(); 	
		virtual void GetOptions(Int32 iSeriesId, Int32 iPropertyId, VARIANT* opValue) const;

		/**
		For surface object to get the order of surfaces.
		If the chart is color by series, we will return the order for group axis.
		Else we will return the order for series axis.
		@param iIsMiddle Input parameter indicates whether it is the middle of two series/groups or not.
			true return the middle position order, false return the order of the axis points.
		@param orOrder The output order vector.
		*/
		void GetOrder(bool iIsMiddle, std::vector<Int32>& orOrder);

	protected:
		/**
		This function loads gap width, colorbyseries
		*/
		virtual void LoadProperties();

	private:
		/**
		Do initialization to some members and options
		*/
		void hInit();

		/**
		Create Wall Manager and all 3 Axes for it.
		Also set margin to Y Axis if there is floating type series in chart.
		*/
		void hCreateWallManagerAndAxes();

		/**
		Generate Collections for risers and labels.
		Pay attention to the color by series/group.
		@param orRiserCollection The output collection for risers.
		@param orLabelCollection The output collection for labels.
		*/
		void hGenerateCollections(std::vector<GraphCollectionObject::Ptr>& orRiserCollection, std::vector<GraphCollectionObject::Ptr>& orLabelCollection);

		/**
		Check all the series types, if any of them is floating type then set margin for Y axis.
		*/
		void hSetYMargin();

		/**
		Get the minimum and maximum value from dataset, which will be used to create value axis.
		@param iorMin The input and output value for minimum value.
		@param iorMax The input and output value for maximum value.
		*/
		void hGetMinMaxValue(double& iorMin, double& iorMax);

		/**
		Generate surface objects.
		There are no labels for surface chart.
		Also in surface chart we don't use the series type since we can't set series type here.
		@param iorRiserCollection The input and output collections for risers.
		*/
		void hGenerateSurface(std::vector<GraphCollectionObject::Ptr>& iorRiserCollection);

		/**
		Generate Area/Cuboid/Ribbon/Step objects.
		For connect group chart, we will disable the series type setting.
		For connect series or cuboid chart, we can do them series by series.
		@param iorRiserCollection The input and output collections for risers.
		@param iorLabelCollection The input and output collections for labels.
		*/
		void hGenerateACRS(std::vector<GraphCollectionObject::Ptr>& iorRiserCollection, std::vector<GraphCollectionObject::Ptr>& iorLabelCollection);

		/**
		This function loads series options.
		@param iSeriesId The input series ID.
		*/
		void hLoadSeriesProperties(Int32 iSeriesId);

		/**
		Get the default series type for each 3D graph type.
		Used in load the series type of each series when we get some wrong or inconsistent series type. 
		*/
		EnumDSSGraph3DRiserType hGetRiserTypeByGraphType();

		/**
		Check whether current graph type is in one of the 3 surface chart types.
		@return true if surface chart, false if not.
		*/
		bool hIsSurface();

		/**
		Check whether the surface is simple surface or side wall surfaces.
		@return true if DssGraphTypeD3_Surface chart, false if other 2 chart types.
		*/
		bool hIsSimpleSurface();

		/**
		Check whether current graph type is in one of the 3 connection group chart types.
		@return true if connection group chart, false if not.
		*/
		bool hIsGraphTypeConnectGroup();

		/**
		Check whether the specified series type is cuboid or not.
		@param iSeriesId The input series ID.
		@return true if cuboid series type, false if not.
		*/
		bool hIsSeriesCuboid(Int32 iSeriesId);

		/**
		Check whether the specified series type is floating type or not.
		Besides floating cube/sphere, Ribbon/Step charts are also floating type and should have a margin in wall.
		@param iSeriesId The input series ID.
		@return true if floating series type, false if not.
		*/
		bool hIsFloating(Int32 iSeriesId);

		/**
		Get the series type among Area/Ribbon/Step.
		For connection group chart all the series type share the same series type.
		For connection series chart series could have their own appearance.
		@param iSeriesId The input series ID, gNullSeriesID means the series type will be decided by graph type.
		@return ARS_AREA/ARS_RIBBON/ARS_STEP.
		*/
		EnumAreaRibbonStep hGetSeriesARS(Int32 iSeriesId);

		/**
		Get the specific cuboid type for specified series.
		@param iSeriesId The input series ID..
		@return CS_BOX/CS_PARAMID/CS_SPHERE/CS_OCTAGON/CS_CUT_CORNER.
		*/
		CuboidSubtype hGetCuboidType(Int32 iSeriesId);

		/**
		Get the specific cuboid type for specified series.
		@param iSeriesId The input series ID..
		@return CS_BOX/CS_PARAMID/CS_SPHERE/CS_OCTAGON/CS_CUT_CORNER.
		*/
		Int32 hGetAreaRibbonPosition(Int32 iId, bool iIsSeries);

		/**
		Calculate the drawing order for all series/groups, also the order for position in middle of series/group points.
		*/
		void hCalculateOrder();

		/**
		Calculate the drawing order for all series/groups.
		Called by hCalculateOrder().
		@param iIsSeries Indicate whether it's series or group.
		*/
		void hCalculateNormalOrder(bool iIsSeries);

		/**
		Calculate the drawing order for all middle positions of series/groups.
		Called by hCalculateOrder().
		@param iIsSeries Indicate whether it's series or group.
		*/
		void hCalculateMiddleOrder(bool iIsSeries);
		
		/**
		Generate one surface Item.
		No labels in surface chart.
		@param iId The input series ID for surface object.
		@param irPointSet The input point set for current surface item.
		@param ioRiserCollectionPtr The pointer to the riser collection object.
		*/
		void hGenerateSurfaceItem(Int32 iId, const std::vector<std::vector<DPoint3D*> >& irPointSet, GraphCollectionObject::Ptr ioRiserCollectionPtr);

		/**
		Generate one cuboid item.
		@param iSeriesId The input series ID.
		@param iGroupId The input group ID.
		@param iorRiserCollection The collections for cuboid items.
		@param iorLabelCollection The collections for labels.
		@param iIsFloating Indicate whether it's floating type cuboid or not.
		@param iCuboidSubType Indicate the cuboid substype.
		*/
		void hGenerateCuboidItem(Int32 iSeriesId, Int32 iGroupId, std::vector<GraphCollectionObject::Ptr>& iorRiserCollection, std::vector<GraphCollectionObject::Ptr>& iorLabelCollection, bool iIsFloating, CuboidSubtype iCuboidSubType);

		/**
		Generate one floating cuboid item.
		@param iSeriesId The input series ID.
		@param iGroupId The input group ID.
		@param iorRiserCollection The collections for cuboid items.
		@param iorLabelCollection The collections for labels.
		@param iCuboidSubType Indicate the cuboid substype.
		*/
		void hGenerateFloatingCuboidItem(Int32 iSeriesId, Int32 iGroupId, std::vector<GraphCollectionObject::Ptr>& iorRiserCollection, std::vector<GraphCollectionObject::Ptr>& iorLabelCollection, CuboidSubtype iCuboidSubType);

		/**
		Generate one normal cuboid item.
		@param iSeriesId The input series ID.
		@param iGroupId The input group ID.
		@param iorRiserCollection The collections for cuboid items.
		@param iorLabelCollection The collections for labels.
		@param iCuboidSubType Indicate the cuboid substype.
		*/
		void hGenerateNormalCuboidItem(Int32 iSeriesId, Int32 iGroupId, std::vector<GraphCollectionObject::Ptr>& iorRiserCollection, std::vector<GraphCollectionObject::Ptr>& iorLabelCollection, CuboidSubtype iCuboidSubType);

		/**
		Generate one Area/Ribbon item.
		@param iSeriesId The input series ID.
		@param iGroupId The input group ID.
		@param iPosition The position of current item, could be front, back, middle or front|back.
		@param iIsConnectSeries Indicate whether it's connection series or group.
		@param iIsArea Indicate whether it's area or ribbon.
		@param iorRiserCollection The collections for cuboid items.
		@param iorLabelCollection The collections for labels.
		*/
		void hGenerateAreaRibbonItem(Int32 iSeriesId, Int32 iGroupId, Int32 iPosition, bool iIsConnectSeries, bool iIsArea, std::vector<GraphCollectionObject::Ptr>& iorRiserCollection, std::vector<GraphCollectionObject::Ptr>& iorLabelCollection);
		
		/**
		Generate all step items.
		No labels for step chart.
		@param iId The input series or group ID.
		@param iIsSeries Indicate whether it's connection series or connection group.
		@param iorRiserCollection The collections for step items.
		*/
		void hGenerateStep(Int32 iId, bool iIsSeries, std::vector<GraphCollectionObject::Ptr>& iorRiserCollection, std::vector<GraphCollectionObject::Ptr>& iorLabelCollection);
		
		/**
		Generate one step item.
		No labels for step chart.
		@param iId The input series or group ID.
		@param iIndex The input index of irSegment.
		@param iIsSeries Indicate whether it's connection series or connection group.
		@param iCenterAbsolute The X/Z coordinate for this step item.
		@param irSegment All the segment coordinate for this step item.
		@param irData The data value from data set for this step item.
		@param iorRiserCollection The collections for step items.
		*/
		void hGenerateStepItem(Int32 iId, Int32 iIndex, bool iIsSeries, double iCenterAbsolute, std::vector<std::pair<double, bool> >& irSegment, std::vector<std::pair<double, Int32> >& irData, std::vector<GraphCollectionObject::Ptr>& iorRiserCollection, std::vector<GraphCollectionObject::Ptr>& iorLabelCollection);

		/**
		Generate one label object.
		@param iSeriesId The input series ID.
		@param iGroupId The input group ID.
		@param iValue The value of this label to be shown.
		@param iCollectionPtr The pointer to the collection of the label.
		@param irPoint The reference point where the label should be placed.
		*/
		void hCreateOneLabel(Int32 iSeriesId, Int32 iGroupId, double iValue, GraphCollectionObject::Ptr iCollectionPtr, const Point2D& irPoint);

		/**
		Get one pair of Area/Ribbon points.
		@param iSeriesId The input series ID.
		@param iGroupId The input group ID.
		@param iIsConnectSeries Indicate whether it's connection series or group.
		@param orStartPoint The output start point of area/ribbon.
		@param orEndPoint The output end point of area/ribbon.
		@param orStartValue The start point value from dataset, used to created label.
		@param orEndValue The end point value from dataset, used to created label.
		*/
		bool hGetAreaRibbonPoint(Int32 iSeriesId, Int32 iGroupId,  bool iIsConnectSeries, DPoint3D& orStartPoint, DPoint3D& orEndPoint, double& orStartValue, double& orEndValue);

		/**
		Get one step item segments and datas.
		@param iId The input series or group ID for this step item.
		@param iIsSeries Indicate whether it's connection series or connection group.
		@param orSegment All the segment coordinate for this step item.
		@param orData The data value from data set for this step item.
		*/
		bool hGetStepSegmentAndData(Int32 iId, bool iIsSeries, std::vector<std::pair<double, bool> >& orSegment, std::vector<std::pair<double, Int32> >& orData);
		
		/**
		Get/Allocate the surface points for all series/group.
		Use pointer here so that we can handle NULL data values.
		The orPointSet will be released in hReleaseSurfacePointSet() function.
		@param orPointSet The output point set which will be allocated in this function.
		*/
		void hGetSurfacePointSet(std::vector<std::vector<DPoint3D*> >& orPointSet);

		/**
		Release the surface points for all series/group.
		The iorPointSet is allocated in hGetSurfacePointSet() function.
		@param iorPointSet The point set to be released.
		*/
		void hReleaseSurfacePointSet(std::vector<std::vector<DPoint3D*> >& iorPointSet);

		bool hLogicalToAbsolute(const DPoint3D& irLogicalPoint, DPoint3D& orAbsolutePoint, bool iIsBottom);

	protected:
		// Data Members
		Int32					mSeriesCount;
		Int32					mGroupCount;
		SPoint3D				mCubeSize;

		double					mMaxRiserWidth;
		double					mYMargin;
		Int32					mGapWidth;
		bool					mColorBySeries;
		// Axes
		CategoryAxis::Ptr		mCategoryAxisPtr;
		CategoryAxis::Ptr		mSeriesAxisPtr;
		ValueAxis::Ptr			mValueAxisPtr;
		// Wall Manager
		WallManager::Ptr		mWallManagerPtr;

		Transformer*			mpTransformer;

		// Series Options
		std::vector<TDSeriesOptions::Ptr> mTDSeriesOptions;

		//the drawing order
		std::vector<Int32> mSeriesNormalOrder;
		std::vector<Int32> mSeriesMiddleOrder;
		std::vector<Int32> mGroupNormalOrder;
		std::vector<Int32> mGroupMiddleOrder;
	};
}

#endif