//==============================================================================================
//	FILENAME	:	GeneralCurvePolygonObject.h
//	AUTHOR		:	Liang Liu
//	CREATION	:	2009-04-15
//	Copyright (C) MicroStrategy Incorporated 2009
//	All Rights Reserved
//==============================================================================================
#ifndef MsiChart_GeneralCurvePolygonObject_h
#define MsiChart_GeneralCurvePolygonObject_h

#include "CurveLineObject.h"
#include "ShapeObject.h"

namespace MsiChart
{
	/**
	This class delegates generate curve polygon object.
	*/
	class GeneralCurvePolygonObject: public ShapeObject
	{
	public:
		typedef MSynch::SmartPtr<GeneralCurvePolygonObject> Ptr;

		/**
		Constructor for a generate curve polygon which do not has a hole.
		@param irTripleId,ipManager,ipParentObject Used by base class.
		@param iBorderLinePtr The border line of GeneralCurvePolygonObject.		
		*/
		GeneralCurvePolygonObject(const TripleId& irTripleId, 					
						GraphObjectManager* ipManager,		
						CurveLineObject::Ptr iBorderLinePtr,
						GraphCollectionObject* ipParentObject, 
						bool iHas360Degrees = true,
						const Point2D& irCenter = Point2D());
		/**
		Constructor for a generate curve polygon which do has a hole.
		@param irTripleId,ipManager,ipParentObject Used by base class.
		@param iBorderLinePtr The border line of GeneralCurvePolygonObject.		
		*/
		GeneralCurvePolygonObject(const TripleId& irTripleId, 					
						GraphObjectManager* ipManager,		
						CurveLineObject::Ptr iFirstBorderLinePtr,
						CurveLineObject::Ptr iSecondBorderLinePtr,
						GraphCollectionObject* ipParentObject, 
						bool iHas360Degrees);
		virtual ~GeneralCurvePolygonObject();		

		// Inherit from GraphObject class.
		virtual void Draw() const;		
		virtual void DrawOutLine();
		virtual bool PointInObject(const Point2D& irPoint);
		virtual bool ObjectInRectangle(const Rect2D& irRect);		
		virtual void GenerateImageMap(ImageMap& iorImageMap);
		virtual void GetBoundingRect(Rect2D& orBoundingRect) const;
		virtual bool SupportBevelEffect() const;

		/**
		Get an approximate polygon of current GeneralCurvePolygonObject.
		@param orPolygon Output an approximate polygon
		*/
		void GetApproximatePolygon(Polygon2D& orPolygon) const;

	protected:
		// Inherit from GraphObject class.
		virtual void GetClockwisePolygon(std::vector<Point2D>& orPolygon) const;
	private:	
		/**
		This function sets the cairo path for current curve polygon.
		@param iIsLinePath Indicates whether current path is a line path or fill path.
		*/
		void hSetPath(bool iIsLinePath) const;

		CurveLineObject::Ptr mFirstBorderLinePtr;
		CurveLineObject::Ptr mSecondBorderLinePtr;
		bool mHasInnerLine;
		bool mHas360Degrees;
		Point2D mCenter;
	};
}
#endif