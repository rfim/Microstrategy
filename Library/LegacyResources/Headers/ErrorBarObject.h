//=============================================================================
//	Filename:		ErrorBarObject.h
//	Date:			2010-11-12
//	Authors:		Zhenhua Sheng

//	Copyright (C) 1997-2010 MicroStrategy Incorporated
//	All rights reserved

//Implementation of ErrorBarObject class
//=============================================================================


#ifndef MsiChart_ErrorBarObject_h
#define MsiChart_ErrorBarObject_h

#include "StdAfx.h"
#include "PDCHeader/PDCalgorithm"
#include "PDCHeader/PDCcmath"
#include "ShapeObject.h"
#include "FormatFill.h"

namespace MsiChart
{
	/**
	 * The error bars in bar graph
	 *
	 *                 ** (AP)
	 *                 **
	 *				
	 *           <- BarWidth->
	 *    TSP    **************  TEP
	 *                 ** TMP
	 *                 **
	 *                 **
	 *                 ** MD
	 *                 **
	 *                 **
	 *                 **
	 *                 ** BMP
	 *    BSP    **************  BEP
	 */
	class ErrorBarObject: public ShapeObject
	{
	public:
		typedef MSynch::SmartPtr<ErrorBarObject> Ptr;

		/**
		 *	Constructor
		 *	@param irTripleId	TripleId
		 *	@param ipManager	Manager who create the object 				
		 *	@param iTopMidPoint	TMP
		 *	@param iBaseMidPoint	BMP
		 *  @param iAcholPoint AP.
		 *  @param isVertical the orientation of error bar, used in animation
		 *  @param hasMidValue whether to display the achol point
		 */
		ErrorBarObject(const MsiChart::TripleId &irTripleId, 
						GraphObjectManager* ipManager,
						Point2D& iTopMidPoint,
						Point2D& iBaseMidPoint,
						Int32 iBarWidth,
						Point2D& iAcholPoint,
						bool isVertical,
					    bool hasMidValue);
		/**
		 * Destructor
		 */
		virtual ~ErrorBarObject();
		
		/**
		 * draw a frame during the animation
		 */
		virtual void Draw() const;
		
		virtual bool ObjectInRectangle(const Rect2D& irRect);

		virtual void GetClockwisePolygon(std::vector<Point2D>& orPolygon) const;
		
		/**
		 * Set the frame index and prepare six points for the next drawing process
		 */
		virtual void SetProgress(double iProgress);

	private:
		// the points in the current frame
		Point2D mTopStartPoint;
		Point2D mTopEndPoint;
		Point2D mBaseStartPoint;
		Point2D mBaseEndPoint;
		Point2D mTopMidPoint;
		Point2D mBaseMidPoint;
		Point2D mAcholPoint;

		// the points in the final frame
		Point2D mFinalTopStartPoint;
		Point2D mFinalTopEndPoint;
		Point2D mFinalBaseStartPoint;
		Point2D mFinalBaseEndPoint;
		Point2D mFinalTopMidPoint;
		Point2D mFinalBaseMidPoint;
		Point2D mFinalAcholPoint;
		
		// the bar width which is the same in top and base line
		Int32 mBarWidth;
		// the orientation of error bar
		bool mIsVertical;
		// whether to draw the achol point
		bool mHasMidValue;
	};
}

#endif