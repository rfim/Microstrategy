//==============================================================================================
//	FILENAME	:	DepthCurvePolygonObject.h
//	AUTHOR		:	Liang Liu
//	CREATION	:	2008-12-9
//	Copyright (C) MicroStrategy Incorporated 2008
//	All Rights Reserved
//==============================================================================================
#ifndef MsiChart_DepthCurvePolygonObject_h
#define MsiChart_DepthCurvePolygonObject_h

#include "CurvePolygonObject.h"
#include "DepthPolygonObject.h"
#include "DepthCurveLineObject.h"
#include "DepthLineObject.h"

namespace MsiChart
{
	/**
	This class delegates curve polygon object with depth.
	*/
	class DepthCurvePolygonObject: public DepthPolygonObject
	{
	public:
		typedef MSynch::SmartPtr<DepthCurvePolygonObject> Ptr;

		/**
		Constructor
		@param irTripleId,ipManager,ipParentObject Used by base class
		@param irPoints The data points of the curve polygon.
		@param iDepth The depth value.
		@param iAngle The depth direction.
		@param iShowFace should be one of the combinations of SF_FRONT, SF_SIDE and SF_HIDDEN_SIDE, which determines whether to show the front face, the side face or the hidden side face.
		*/
		DepthCurvePolygonObject(const TripleId& irTripleId, 					
						GraphObjectManager* ipManager,		
						const std::vector<Point2D>& irPoints, 
						Int32 iDepth, Int32 iAngle,
						GraphCollectionObject* ipParentObject = NULL, 
						bool iIsVertical = true,
						Int32 iShowFace = SF_FRONT | SF_SIDE | SF_HIDDEN_SIDE);
		virtual ~DepthCurvePolygonObject();
		
		//virtual functions from GraphObject
		virtual void Draw() const;
		virtual void DrawOutLine();
		virtual bool ObjectInRectangle(const Rect2D& irRect);
		virtual bool PointInObject(const Point2D& irPoint);
		virtual void GenerateImageMap(ImageMap& iorImageMap);	
		virtual bool SupportBevelEffect() const;

	protected:
		virtual void GetClockwisePolygon(std::vector<Point2D>& orPolygon) const; 
	
	private:
		/**
		This method is to calculate all the top and bottom faces.
		@param irPoints The input polygon.
		*/
		void hCalculateTopAndBottomFaces(const std::vector<Point2D>& irPoints);	

		CurvePolygonObject::Ptr mFrontCurvePolygonPtr;
		bool mIsShownFrontCurvePolygon;
	};	
}
#endif
