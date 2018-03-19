//==============================================================================================
//	FILENAME	:	CircleLineObject.h
//	AUTHOR		:	Liang Liu
//	CREATION	:	2009-3-16
//	Copyright (C) MicroStrategy Incorporated 2009
//	All Rights Reserved
//==============================================================================================
#ifndef MsiChart_CircleLineObject_h
#define MsiChart_CircleLineObject_h

#include "GraphObject.h"

namespace MsiChart
{	
	/// Delegates circle lines.
	class CircleLineObject: public LineObject
	{
	public:
		typedef MSynch::SmartPtr<CircleLineObject> Ptr;

		/**	Constructor.
		@param irTripleId Required by base class.
		@param ipManager Required by base class.	
		@param ipParentObject Required by base class.		
		*/
		CircleLineObject(const TripleId& irTripleId, GraphObjectManager* ipManager, const Point2D& irCenter,
			float iRadius, GraphCollectionObject* ipParentObject = NULL); 
		virtual ~CircleLineObject();
	
		// virtual functions from GraphObject
		virtual void Draw() const;
		virtual bool PointInObject(const Point2D& irPoint);
		virtual bool ObjectInRectangle(const Rect2D& irRect);	
		virtual void GenerateImageMap(ImageMap& iorImageMap);	
		virtual void DrawOutLine();
		
	protected:
		//pure virtual functions from GraphObject
		virtual void GetClockwisePolygon(std::vector<Point2D>& orPolygon) const;

		Point2D mCenter;
		float mRadius;	
	};
	
}
#endif
