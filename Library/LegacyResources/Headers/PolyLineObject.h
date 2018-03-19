//==============================================================================================
//	FILENAME	:	PolyLineObject.h
//	AUTHOR		:	Liang Liu
//	CREATION	:	2008-7-25
//	Copyright (C) MicroStrategy Incorporated 2008
//	All Rights Reserved
//==============================================================================================
#ifndef MsiChart_PolyLineObject_h
#define MsiChart_PolyLineObject_h

#include "PDCHeader/PDCvector"
#include "GraphObject.h"

namespace MsiChart
{	
	/// Delegates Poly lines.
	class PolyLineObject: public LineObject
	{
	public:
		typedef MSynch::SmartPtr<PolyLineObject> Ptr;

		/**	Constructor.
		@param irTripleId Required by base class.
		@param ipManager Required by base class.	
		@param irPoints Specifies the points in the polyline. 
		@param ipParentObject Required by base class.		
		*/
		PolyLineObject(const TripleId& irTripleId, GraphObjectManager* ipManager, 
			const std::vector<Point2D>& irPoints, GraphCollectionObject* ipParentObject = NULL, 
			bool iHasLineFormat = true, bool iIsClosePath = false); 
		virtual ~PolyLineObject();

		// Public methods
        /**
         Extend the polyline object.
         @param irPoints, the points to be added for the original line points.
         @param iPosition, the position to insert the irPoints. When it is negative, the irPoints will be added to the end of mLinePoints
         */
        void Extend(std::vector<Point2D>& irPoints, Int32 iPosition);

		/**
		Add one point into the polyline. 
		@param irPoint Specifies the added point. 
		*/
		//void AddPoint(const Point2D& irPoint); 
		/**
		Set one point in the polyline according to the parameter iIndex 
		@param irPoint Store the point that will be passed to the polyline. 
		@param iIndex Specifies which point to set. 
		@return true = the point is set successfully. false = failed to set the point.
		*/
		//bool SetPoint(const Point2D& irPoint, unsigned Int32 iIndex);
		/**
		Get the specific point in the polyline according to the parameter iIndex 
		@param orPoint Store the point get from the polyline. 
		@param iIndex Specifies which point to get. 
		@return true = get the point successfully. false = failed to get the point.
		*/
		//bool GetPoint(Point2D& orPoint, unsigned Int32 iIndex) const; 
		
		/// Clear all the points in the polyline. 
		//void Clear(); 			

		// virtual functions from GraphObject
		virtual void Draw() const;
		virtual bool PointInObject(const Point2D& irPoint);
		virtual bool ObjectInRectangle(const Rect2D& irRect);	
		virtual void GenerateImageMap(ImageMap& iorImageMap);
		virtual void DrawOutLine();
		virtual Int32 DistanceToPoint(const Point2D& irPoint);	
        virtual std::vector<Point2D> GetLinePoints() const;
		
		/** Get polygons as the outline of current polyline.
		Sometimes, it is not very convenient to represent the outline using only one polygon. (For example, for Polyline ABC, if BA and BC have the same
		direction or the angle between them is very small, it is better to use two polygons.)
		*/
		void GetPolygons(std::vector<Polygon2D>& orPolygons) const;
		
	protected:
		//pure virtual functions from GraphObject
		virtual void GetClockwisePolygon(std::vector<Point2D>& orPolygon) const;

		void hGetOnePolygon(const std::vector<Point2D>& irLinePoints, Polygon2D& orPolygon) const;

		void SetPath() const;

		std::vector<Point2D> mLinePoints; ///< Store all the points of the poly line in device coordinates.

		bool mIsClosePath;	///< Indicates whether current polyline is a close path or not.
	};
    inline std::vector<Point2D> PolyLineObject::GetLinePoints() const
    {
        return mLinePoints;
    }
}
#endif
