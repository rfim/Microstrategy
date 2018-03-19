//=======================================================================
//	Filename:		Kernel/SourceCode/IPhoneChart/PrivateSource/StepLineSeriesRenderer.h
//	Date:			2008-12-09
//	Authors:		Bing Huang

//	Copyright (C) 1997-2008 MicroStrategy Incorporated
//	All rights reserved
//=======================================================================

#ifndef MsiChart_StepLineSeriesRenderer_h
#define MsiChart_StepLineSeriesRenderer_h

#include "LineSeriesRenderer.h"

namespace MsiChart
{
	class StepLineSeriesRenderer : public LineSeriesRenderer
	{
	public:
		typedef MSynch::SmartPtr<LineSeriesRenderer> Ptr;
		
		StepLineSeriesRenderer(const TripleId& irTripleId, GraphObjectManager* ipManager, Dataset::Ptr iDatasetPtr, Legend::Ptr iLegendPtr);

		//pure virtual functions from GraphObjectManager
		//virtual void GenerateMapAndList();

		//pure virtual function from SeriesRenderer
		//virtual void GenerateByValueAxis(EnumValueAxis iValueAxis, std::vector<GraphObject::Ptr>& iorGraphObjectList);
		
		//virtual void GenerateInterpolationFrame(LayerObject* ipLayerObject);

	protected:
		/**
		Load step line property.
		@param iInfoIndex The index for mLineSeiresOptions.
		*/
		virtual void LoadStepLine(Int32 iInfoIndex);

		/**
		Get the point set for the step line series.
		@param iInfoIndex The index for mLineSeiresOptions.
		@param orPointSet The output point set.
		*/
		virtual void GetLinePointSet(Int32 iInfoIndex, std::vector<std::vector<Point2D> >& orPointSet) const;

		/**
		Get step line points for the specified series and group.
		For the step line belongs to specified series and group, there will be two points.
		The normal line point is the middle of them.
		@param iInfoIndex The index for mSeriesInfo.
		@param iGroupId The input group ID.
		@param orStartPoint The output step line start point.
		@param orEndPoint The output step line end point.
		@return true if successful, false if the data is NULL in Dataset.
		*/		
		bool GetStepLinePoint(Int32 iInfoIndex, Int32 iGroupId, Point2D& orStartPoint, Point2D& orEndPoint) const;

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
		Inherited from LineSeriesRender. To compute the points for step line series in pareto percent chart.	
		*/
		virtual bool hGetParetoLinePoints(Int32 iInfoIndex, std::vector<std::vector<Point2D> >& orPointSet) const;
	private:
		/**
		Generate step line by value axis.
		If there are no negative values, we will generate series by series.
		Else we will generate group by group.
		@param iValueAxisIndex The input value axis index.
		*/
		void hGenerateByValueAxis(EnumValueAxis iValueAxisIndex);

		/**
		Generate the side face for step line.
		@param iValueAxisIndex The input value axis index.
		@param iGroupId The input group ID.
		@param iIsReverseGroupOrder Indicates whether the group order is reversed.
		*/
		void hGenerateSideFaceForGroup(EnumValueAxis iValueAxisIndex, Int32 iGroupId, bool iIsReverseGroupOrder);	
		
		/// Get group position by SeriesInfo index and group index.
		Int32 hGetGroupPosition(Int32 iInfoIndex, float iGroupIndex) const;
		
#ifndef _VE_CE
		virtual void GenerateFrameInOnePlot(LayerObject* ipLayerObject);
#else
		virtual void GenerateFrameInOnePlot(GraphObjectAggregation* ioGraphObjectAggregation, bool iForStartFrame);
#endif
	};
}

#endif
