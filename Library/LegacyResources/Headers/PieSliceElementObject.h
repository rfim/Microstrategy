//=====================================================================================
//	Filename:		PieSliceElementObject.h
//	Date:			2008-05-22
//	Authors:		Bing Huang

//	Copyright (C) 1997-2008 MicroStrategy Incorporated
//	All rights reserved
//=====================================================================================

#ifndef MsiChart_PieSliceElementObject_h
#define MsiChart_PieSliceElementObject_h

#include "ShapeObject.h"

namespace MsiChart
{
	class PieSliceElementObject : public ShapeObject
	{
	public:
		typedef MSynch::SmartPtr<PieSliceElementObject> Ptr;

		PieSliceElementObject(const TripleId& irTripleId, GraphObjectManager* ipManager,
			bool iIsFirstElement, Int32 iStartAngle, Int32 iEndAngle, 
			PieSliceObject* ipParentSlice, GraphCollectionObject* ipParentObject = NULL, bool iIsShown = true);
		
		//virtual functions from GraphObject
		virtual void Draw() const;
		virtual void DrawOutLine();
		virtual bool PointInObject(const Point2D& irPoint);
		virtual bool ObjectInRectangle(const Rect2D& irRect);
		virtual void GenerateImageMap(ImageMap& iorImageMap);
		virtual void GetOptions(Int32 iPropertyId, CComVariant* opValue) const;
		virtual Int32 DistanceToPoint(const Point2D& irPoint);

	protected:
		//pure virtual functions from ShapeObject
		virtual void GetClockwisePolygon(std::vector<Point2D>& orPolygon) const;
		virtual bool IsCircularShape() const;

	private:		
		/**
		Draw ring pie element slice.
		*/
		void hDrawRingPieSlice() const;

		/**
		Draw ring pie middle part, including cylinder and side rectangle.
		*/
		void hDrawRingPieMiddle() const;

		/**
		Draw ring pie top sector.
		*/
		void hDrawRingPieTop() const;

		/**
		Draw ring pie side rectangle.
		*/
		void hDrawRingPieMiddleRectangle() const;

		/**
		Draw ring pie cylinder, only draw the part that can be seen.
		*/
		void hDrawRingPieMiddleCylinder() const;

		/**
		Draw cylinder according to the angle information.
		@param iIsRing Indicates whether it's a ring pie.
		@param iStartAngle The start angle of the cylinder.
		@param iEndAngle The end angle of the cylinder.
		*/
		void hDrawCylinder(bool iIsRing, Int32 iStartAngle, Int32 iEndAngle) const;

		/**
		Draw normal pie slice.
		*/			
		void hDrawNormalPieSlice() const;

		/**
		Draw normal pie middle part, including cylinder and side rectangle.
		*/
		void hDrawNormalPieMiddle() const;

		/**
		Draw normal pie top sector.
		*/
		void hDrawNormalPieTop() const;

		/**
		Draw normal pie side rectangle.
		@param iRectangleAngle The input anlge.
		*/
		void hDrawNormalPieMiddleRectangle() const;

		/**
		Draw normal pie cylinder, only draw the part that can be seen.
		*/
		void hDrawNormalPieMiddleCylinder() const;	

		/**
		Judge whether a point hit the ring pie element slice.
		@param irPoint The input point.
		@return true if hit, false if out of slice.
		*/
		bool hPointInRingPieSlice(const Point2D& irPoint) const;

		/**
		Judge whether a point hit the normal pie element slice.
		@param irPoint The input point.
		@return true if hit, false if out of slice.
		*/
		bool hPointInNormalPieSlice(const Point2D& irPoint) const;

		/**
		Check whether an included angle has an intersection with the element slice angle.
		@param irAngle The input included angle.
		@return true if intersect, false if not.
		*/
		bool hGetIntersectionConvertedAngleToSliceAngle(const Angle& irAngle) const;

		//data members
		bool mIsFirstElement;
		bool mIsShown;
		Int32 mStartAngle;
		Int32 mEndAngle;
		PieSliceObject* mpParentSlice;
	};
}

#endif
