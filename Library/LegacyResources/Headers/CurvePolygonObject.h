//==============================================================================================
//	FILENAME	:	CurvePolygonObject.h
//	AUTHOR		:	Liang Liu
//	CREATION	:	2008-12-8
//	Copyright (C) MicroStrategy Incorporated 2008
//	All Rights Reserved
//==============================================================================================
#ifndef MsiChart_CurvePolygonObject_h
#define MsiChart_CurvePolygonObject_h

#include "CurveLineObject.h"
#include "ShapeObject.h"

namespace MsiChart
{
	/**
	This class delegates curve polygon object.
	*/
	class CurvePolygonObject: public ShapeObject
	{
	public:
		typedef MSynch::SmartPtr<CurvePolygonObject> Ptr;

		/**
		Constructor
		@param irTripleId,ipManager,ipParentObject Used by base class.
		@param irPoints The data point of CurvePolygonObject.		
		*/
		CurvePolygonObject(const TripleId& irTripleId, 					
						GraphObjectManager* ipManager,		
						const std::vector<Point2D>& irPoints, 	
						bool iIsVertical,
						GraphCollectionObject* ipParentObject = NULL);
		virtual ~CurvePolygonObject();		

		// Inherit from GraphObject class.
		virtual void Draw() const;		
		virtual void DrawOutLine();
		virtual bool PointInObject(const Point2D& irPoint);
		virtual bool ObjectInRectangle(const Rect2D& irRect);		
		virtual void GenerateImageMap(ImageMap& iorImageMap);
		virtual void GetBoundingRect(Rect2D& orBoundingRect) const;
		virtual bool SupportBevelEffect() const;

	protected:
		// Inherit from GraphObject class.
		virtual void GetClockwisePolygon(std::vector<Point2D>& orPolygon) const;
	private:	
		/**
		This function sets the cairo path for current curve polygon.
		@param iIsLinePath Indicates whether current path is a line path or fill path.
		*/
		void hSetPath(bool iIsLinePath) const;

		CurveLineObject::Ptr mMainCurveEdgePtr;
		CurveLineObject::Ptr mSecondaryCurveEdgePtr;
		bool mIsVertical;
	};
}
#endif