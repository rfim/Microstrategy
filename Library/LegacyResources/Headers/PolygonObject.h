//==============================================================================================
//	FILENAME	:	PolygonObject.h
//	AUTHOR		:	Liang Liu
//	CREATION	:	2008-05-26
//	Copyright (C) MicroStrategy Incorporated 2008
//	All Rights Reserved
//==============================================================================================
#ifndef MsiChart_PolygonObject_h
#define MsiChart_PolygonObject_h

#include "PDCHeader/PDCvector"
#include "ShapeObject.h"

namespace MsiChart
{	
	/// Represents a polygon area. 
	class PolygonObject: public ShapeObject
	{
	public:
		typedef MSynch::SmartPtr<PolygonObject> Ptr;

		/** Constructor
		@param irTripleId,ipManager,ipParent Required by base class.	
		@param irPolygon Specify a polygon.
		@param ipParentObject,iHasFill Required by base class.
		@param iHasHole Indicate whether current polygon has a hole or not.
		*/
		PolygonObject(const TripleId& irTripleId, GraphObjectManager* ipManager,
			const std::vector<Point2D>& irPolygon, GraphCollectionObject* ipParentObject = NULL, 
			bool iHasFill = true, bool iHasHole = false); 
		virtual ~PolygonObject();

		/** This function sets a new polygon area for current PolygonObject.
		@param irPolygon The new polygon area.
		*/
		//void SetPolygon(const std::vector<Point2D>& irPolygon); 

		/** This function gives the polygon area of current PolygonObject.
		@param orPolygon Polygon area of current PolygonObject. 
		*/
		//void GetPolygon(std::vector<Point2D>& orPolygon) const; 		

		// Inherit from GraphObject class.
		virtual void Draw() const;		
		virtual void DrawOutLine();
		virtual bool PointInObject(const Point2D& irPoint) const;
        virtual bool PointStrictlyInObject(const Point2D& irPoint) const;
		virtual bool ObjectInRectangle(const Rect2D& irRect);		
		virtual void GenerateImageMap(ImageMap& iorImageMap);
		virtual void GetBoundingRect(Rect2D& orBoundingRect) const;
        virtual std::vector<Point2D> GetPolygon() const;

	protected:
		// Inherit from GraphObject class.
		virtual void GetClockwisePolygon(std::vector<Point2D>& orPolygon) const;
		virtual void FillBevel() const;

		//-----------Data member--------------------------
		mutable std::vector<Point2D> mDevicePolygon;	///< Represents a polygon in device coordinate. 
		bool mHasHole;
	};	
    
    inline std::vector<Point2D> PolygonObject::GetPolygon() const
    {
        return mDevicePolygon;
    }

	
}
#endif
