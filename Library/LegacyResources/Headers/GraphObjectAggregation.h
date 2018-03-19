//=============================================================================
//	Filename:		GraphObjectAggregation.h
//	Date:			2010-11-25
//	Authors:		Zhenhua Sheng

//	Copyright (C) 1997-2010 MicroStrategy Incorporated
//	All rights reserved

//Implementation of GraphObjectAggregation class
//=============================================================================


#ifndef MsiChart_GraphObjectAggregation_h
#define MsiChart_GraphObjectAggregation_h

#include "StdAfx.h"
#include "PDCHeader/PDCalgorithm"
#include "PDCHeader/PDCcmath"
#include "GraphObject.h"

namespace MsiChart {
	class GraphObjectAggregation : public GraphObject {
	public:
		typedef MSynch::SmartPtr<GraphObjectAggregation> Ptr;
		
		/**	Constructor.
		 @param irTripleId This is used by base class.(GraphObject)
		 @param ipManager This is used by base class.
		 */
		GraphObjectAggregation(const TripleId& irTripleId, GraphObjectManager* ipManager); 
		virtual ~GraphObjectAggregation();
		
		/** This function will add a GraphObject into current LayerObject. 
		 @param iGraphObjectPtr Pointer to the GraphObject which is ready to be added into current LayerObject.		
		 */
		void AddGraphObject(GraphObject::Ptr iGraphObjectPtr, bool iToFirstFrame = false);
		
		/**	Clear mGraphObjectList.		
		 */
		void Clear(bool iClearStartFrame = true, bool iClearEndFrame = true);
		
		//---------Override functions-------------------------------------		
		virtual void GenerateImageMap(ImageMap& iorImageMap);
		virtual void Draw() const;		
		virtual bool IsLayerObject() const;
		virtual bool ObjectInRectangle(const Rect2D& irRect);
		virtual void GetClockwisePolygon(std::vector<Point2D>& orPolygon) const; 
		
		GraphObject::Ptr OnPointDetectionEx(const Point2D& irCursor, 
			const std::vector<Int32>& irObjectIds, Int32& iorDistance, Int32 iRadius,
			std::vector<GraphObject::Ptr>& iorGraphObjectsAround, EnumGraphObjectUsage iUsage = GOU_NORMAL_HIGHLIGHT) const;

		virtual void SetProgress(double iProgress);

		/// Check whether the current LayerObject is empty.
		bool IsEmpty() const;	

	protected:	
		// Draw the objects in mGraphObjectList;
		void DoDraw() const;			
		
		/// A collection of GraphObjects which share the same format properties. 
		std::vector<GraphObject::Ptr> mGraphObjectList;	
		std::vector<GraphObject::Ptr> mStartFrameGraphObjectList;
	};

	inline bool GraphObjectAggregation::IsLayerObject() const
	{
		return true;
	}
}


#endif