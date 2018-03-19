//==============================================================================================
//	FILENAME	:	RadarCategoryAxis.h
//	AUTHOR		:	Liang Liu
//	CREATION	:	2009-03-31
//	Copyright (C) MicroStrategy Incorporated 2009
//	All Rights Reserved
//==============================================================================================
#ifndef MsiChart_RadarCategoryAxis_h
#define MsiChart_RadarCategoryAxis_h

#include "CategoryAxis.h"
#include "PolarValueAxis.h"

namespace MsiChart
{	
	/**
	Creates and maintains axis title, axis labels, axis line and grid lines. 
	This class also provide conversion from value to coordinate. This class is derived from PolarValueAxis
	instead of CategoryAxis because RadarCategoryAxis share more common logic with PolarValueAxis than 
	that with CategoryAxis.
	*/
	class RadarCategoryAxis: public PolarValueAxis
	{
	public:
		typedef MSynch::SmartPtr<RadarCategoryAxis> Ptr;
		
		/**        
        @param irTripleId,ipManager,irPolarInfo Required by base class.
		@param iDatasetPtr Pointer to Dataset.
		*/
		RadarCategoryAxis(const TripleId& irTripleId, GraphObjectManager* ipManager, 				
			const PolarInfo& irPolarInfo, Dataset::Ptr iDatasetPtr); 
		virtual ~RadarCategoryAxis();	

		/**
		Get the logical coordinates for the labels along current axis.
		*/
		virtual void GetTicks(std::vector<double>& orTicks) const;		
		virtual void ReserveLabelArea(Rect2D& iorArea, bool iIsReversed);
		
	private:			
		TextObject::Ptr hCreateGroupLabel(const TripleId& iID, const MBase::String& iStr, Point2D& orSize);

		/**
		Get logical coordinate by Group ID.
		@iGroupId Specifies a group ID or series ID.
		@return Return the logical coordinate which corresponds to iGroupId.
		*/
		double hGetLogicalCoordById(Int32 iGroupId) const;
	
		Point2D mMaxLabelSize;		
		Int32 mGroupCount; ///< Number of Groups.
		Dataset::Ptr mDatasetPtr;	///< pointer to dataset
	};	

	inline double RadarCategoryAxis::hGetLogicalCoordById(Int32 iGroupId) const
	{
		return iGroupId + 1.0;
	}
}
#endif
