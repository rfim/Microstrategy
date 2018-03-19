//==============================================================================================
//	FILENAME	:	WallManager.h
//	AUTHOR		:	Liang Liu
//	CREATION	:	2009-05-19
//	Copyright (C) MicroStrategy Incorporated 2009
//	All Rights Reserved
//==============================================================================================
#ifndef MsiChart_WallManager_h
#define MsiChart_WallManager_h

#include "PDCHeader/PDCvector"
#include "GraphObjectManager.h"
#include "CuboidObject.h"
#include "Axis.h"
#include "GraphCollectionObject.h"

namespace MsiChart
{	
	/// This class manages the walls in 3D charts. 
	class WallManager: public GraphObjectManager
	{
	public:
		typedef MSynch::SmartPtr<WallManager> Ptr;

		/** Constructor
		@param irTripleId,ipManager,ipParentObject,iHasFill Required by base class.		
		@param irPlotArea Specifies the plot area.
		*/
		WallManager(const TripleId& irTripleId, GraphObjectManager* ipManager, Rect2D& irPlotArea); 
		virtual ~WallManager();

		virtual void GenerateMapAndList();
		virtual void GetOptions(Int32 iSeriesId, Int32 iPropertyId, CComVariant* opValue) const;		

		/**
		Add wall objects (including wall, grid lines, axis labels, etc) which need to drawn at the beginning of the plot.
		@param iorGraphObjectList The pre-plot wall objects will be appended to this list.
		*/
		void AddPrePlotWallObjects(std::vector<GraphObject::Ptr>& iorGraphObjectList);
		/**
		Add wall objects which need to drawn at the end of the plot.
		@param iorGraphObjectList The post-plot wall objects will be appended to this list.
		*/
		void AddPostPlotWallObjects(std::vector<GraphObject::Ptr>& iorGraphObjectList);

		/**
		Set one axis of X, Y, Z axes.
		@param iAxisPtr Specifies one axis.
		@param iAxisDirection Specifies the axis direction.
		*/
		void SetAxis(Axis::Ptr iAxisPtr, EnumAxisDirection iAxisDirection);
		
		/**
		Get wall margin.
		@param orWallMargin Output the wall margin.
		*/
		void GetWallMargin(SPoint3D& orWallMargin) const;
		/**
		Set Y axis margin.
		@param iYMargin Specifies the minimum acceptable Y margin.
		*/
		void SetYAxisMargin(Int32 iYMargin);

		/**
		Set all axis margins.
		@param iMargin Specifies the minimum acceptable margin for all three axes.
		*/
		void SetAllAxisMargins(Int32 iMargin);

		/**
		Checks whether a point should be drawn before PrePlot walls.
		@param irAbsolutePoint Specify the input point.
		@return Return true if the point should be drawn before PrePlot walls. Otherwise, return false.
		*/
		bool IsBeforePreWall(const DPoint3D& irAbsolutePoint) const;
		/**
		Checks whether a point should be drawn after PostPlot walls.
		@param irAbsolutePoint Specify the input point.
		@return Return true if the point should be drawn after PostPlot walls. Otherwise, return false.
		*/
		bool IsAfterPostWall(const DPoint3D& irAbsolutePoint) const;

		/**
		Checks whether a point is completely hidden by walls or not.
		GenerateMapAndList() needs to be called before calling this method.
		@param irAbsolutePoint Specify the input point.
		@return Return true if the point completely hidden by walls. Otherwise, return false.
		*/
		bool IsHiddenByWalls(const DPoint3D& irAbsolutePoint) const;

	private:
		void AddWallZXObjects(std::vector<GraphObject::Ptr>& iorGraphObjectList, std::vector<GraphObject::Ptr>& iorLabelList);
		void AddWallYZObjects(std::vector<GraphObject::Ptr>& iorGraphObjectList, std::vector<GraphObject::Ptr>& iorLabelList);
		void AddWallXYObjects(std::vector<GraphObject::Ptr>& iorGraphObjectList, std::vector<GraphObject::Ptr>& iorLabelList);

		void hLoadProperties(); 
		void hTruncateSize(Int32& iorSize, Int32 iMinSize, Int32 iMaxSize);
		DVector2D hGetAxisDirection(const DPoint3D& irStartPoint, const DPoint3D& irEndPoint) const;
		double hGetMaxLabelOffset(GraphCollectionObject* ipLabelCollection, const DVector2D& irUnitOffsetDirection) const;

		CuboidObject::Ptr hGenerateWall(const TripleId& irWallId, const DPoint3D& irStartVertex, const DPoint3D& irEndVertex, GraphCollectionObject* ipWallSet);
		void hGenerateInterlacedGrid(CuboidObject* ipWall, EnumAxisDirection iAxisDirection, std::vector<GraphObject::Ptr>& iorGraphObjectList);
		void hGenerateMajorGrid(CuboidObject* ipWall, EnumAxisDirection iAxisDirection, std::vector<GraphObject::Ptr>& iorGraphObjectList);
		GraphCollectionObject* hGenerateAxisLabels(EnumAxisDirection iAxisDirection, Int32 iAxisLabelId, const DPoint3D& irRefPoint, 
			const DVector2D& irOffsetDirection, std::vector<GraphObject::Ptr>& iorLabelList);
		void hGenerateAxisTitle(EnumAxisDirection iAxisDirection, Int32 iAxisTitleId, const DPoint3D& irRefPoint, 
			const DVector2D& irOffsetDirection, double iTitleOffset, std::vector<GraphObject::Ptr>& iorLabelList);

		Axis* hGetAxis(EnumAxisDirection iAxisDirection) const;
		Int32 hGetMajorGridId(EnumAxisDirection iAxisDirection) const;

		Rect2D mPlotArea;
		SPoint3D mCubeSize;
		SPoint3D mWallThickness;
		SPoint3D mWallMargin;
		DPoint3D mEyePosition;

		CuboidObject::Ptr mWallZXPtr;	///< Floor.		
		CuboidObject::Ptr mWallYZPtr;	///< Left wall.
		CuboidObject::Ptr mWallXYPtr;	///< Right wall.

		Axis::Ptr mXAxisPtr;
		Axis::Ptr mYAxisPtr;
		Axis::Ptr mZAxisPtr;	

		///< Indicates whether Z axis needs to be drawn in reverse order. This should be false in 3D scatter chart. But it should be true
		bool mIsReverseZAxis;
		bool mLinkColors;	///< Indicates whether to link wall colors to floor color.
		bool mShowFloor;
		bool mShowLeftWall;
		bool mShowRightWall;	
		bool mShowRightYAxisLabels;	///< Special handling for right Y axis labels in WallManager. Other "show axis labels" are handled in Axis class.
		bool mShowRightYAxisTitle;	///< Special handling for right Y axis title in WallManager. Other "show axis title" are handled in Axis class.
		Int32 mGraphMajorType;		///< Keeps the graph major type.
	};	

	inline void WallManager::hTruncateSize(Int32& iorSize, Int32 iMinSize, Int32 iMaxSize)
	{
		iorSize = (iorSize < iMinSize) ? iMinSize : iorSize;
		iorSize = (iorSize > iMaxSize) ? iMaxSize : iorSize;
	}		
}
#endif
