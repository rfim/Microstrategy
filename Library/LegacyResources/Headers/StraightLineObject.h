//==============================================================================================
//	FILENAME	:	StraightLineObject.h
//	AUTHOR		:	Liang Liu
//	CREATION	:	2008-7-29
//	Copyright (C) MicroStrategy Incorporated 2008
//	All Rights Reserved
//==============================================================================================
#ifndef MsiChart_StraightLineObject_h
#define MsiChart_StraightLineObject_h

#include "PDCHeader/PDCvector"
#include "GraphObject.h"

namespace MsiChart
{	
	/** Represent a straight line.
	It is obvious that PolyLineObject could provide all the functionalities that StraightLineObject support. 
	The reason that we have this standalone StraightLineObject is that PolyLineObject is only used in the line 
	chart while StraightLineObject prevails in the chart. 
	*/
	class StraightLineObject: public LineObject
	{
	public:
		typedef MSynch::SmartPtr<StraightLineObject> Ptr;

		/**	Constructor.
		@param irTripleId Required by base class.
		@param ipManager Required by base class.
		@param irStartPoint The start point of straight line.
		@param irEndPoint The end point of straight line.
		@param ipParentObject Required by base class.
		@param iIsOnlyForToolTip Indicate whether current StraightLineObject is only used for tool tip.
		*/
		StraightLineObject(const TripleId& irTripleId, GraphObjectManager* ipManager, 
			const Point2D irStartPoint, const Point2D irEndPoint, GraphCollectionObject* ipParentObject = NULL,
			bool iIsOnlyForToolTip = false); 
		virtual ~StraightLineObject();

		virtual void SetProgress(double iProgress);
		//	Public methods
		/** Set the start point of the line. 
		@param irStartPoint Specifies the start point of the line. 
		*/
		//void SetStartPoint(const Point2D& irStartPoint); 
		/** Get the start point of the line. 
		@param orStartPoint Output to store the start point of the line. 
		*/
		//void GetStartPoint(Point2D& orStartPoint) const;
		/** Set the end point of the line. 
		@param irEndPoint Specifies the end point of the line. 
		*/
		//void SetEndPoint(const Point2D& irEndPoint);	
		/** Get the end point of the line. 
		@param orEndPoint Output to store the end point of the line. 
		*/
		//void GetEndPoint(Point2D& orEndPoint) const;

		// Inherit from GraphObject
		virtual void Draw() const;
		virtual bool PointInObject(const Point2D& irPoint);	
		virtual bool ObjectInRectangle(const Rect2D& irRect);	
		virtual void DrawOutLine();
		virtual Point2D GetDataPoint();
	
	protected:
		// Inherit from GraphObject
		virtual void GetClockwisePolygon(std::vector<Point2D>& orPolygon) const;

	private:	
		Point2D mDeviceStartPoint; 	///< Start point of the straight line.
		Point2D mDeviceEndPoint;	///< End point of the straight line.

		Point2D mFinalStartPoint;
		Point2D mFinalEndPoint;

		bool mIsOnlyForToolTip;	///< Indicate whether current RectangleObject is only used for tool tip.
	};
	
}
#endif
