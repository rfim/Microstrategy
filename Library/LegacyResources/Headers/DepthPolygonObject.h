//==============================================================================================
//	FILENAME	:	DepthPolygonObject.h
//	AUTHOR		:	Xin Liu
//	CREATION	:	10/20/08
//	Copyright (C) MicroStrategy, Inc. 2008
//==============================================================================================

#ifndef MSICHART_DEPTHPOLYGONOBJECT_H
#define	MSICHART_DEPTHPOLYGONOBJECT_H

#include "PolygonObject.h"
#include "DepthRectangleObject.h"
#include "DepthLineObject.h"

namespace MsiChart
{
	/**
	Indicates whether some face is shown.
	*/
	enum ShowFace
	{
		SF_FRONT = 1,
		SF_SIDE = 2,
		SF_HIDDEN_SIDE = 4
	};

	/**
	This class delegates polygon object with depth.
	*/
	class DepthPolygonObject: public PolygonObject
	{
	public:
		typedef MSynch::SmartPtr<DepthPolygonObject> Ptr;

		/**
		Constructor
		@param irPoints The polygon area.
		@param iDepth The depth value.
		@param iAngle The depth direction.
		@param iShowFace should be one of the combinations of SF_FRONT, SF_SIDE and SF_HIDDEN_SIDE, which determines whether to show the front face, the side face or the hidden side face.
		*/
		DepthPolygonObject(const TripleId& irTripleId, 					
						GraphObjectManager* ipManager,		
						const std::vector<Point2D>& irPoints, 
						Int32 iDepth, Int32 iAngle,
						GraphCollectionObject* ipParentObject = NULL, 
						bool iIsVertical = true,
						Int32 iShowFace = SF_FRONT | SF_SIDE | SF_HIDDEN_SIDE);
		virtual ~DepthPolygonObject();
		
		//virtual functions from GraphObject
		virtual void Draw() const;
		//virtual void DrawOutLine();	Just re-use PolygonObject::DrawOutLine() method
		virtual bool PointInObject(const Point2D& irPoint);
		virtual bool ObjectInRectangle(const Rect2D& irRect);
		virtual void GenerateImageMap(ImageMap& iorImageMap);	

		/**
		Get top depth line.
		@return Return top depth line.
		*/
		DepthLineObject::Ptr GetTopDepthLine() const;
		/**
		Get bottom depth line.
		@return Return bottom depth line.
		*/
		DepthLineObject::Ptr GetBottomDepthLine() const;
		/**
		Get hidden side face.
		@return Return hidden side face.
		*/
		GraphObject::Ptr GetHiddenSideFace() const;
		/**
		Get side face.
		@return Return side face.
		*/
		GraphObject::Ptr GetSideFace() const;

	protected:
		virtual void GetClockwisePolygon(std::vector<Point2D>& orPolygon) const; 

		/**
		This method is to calculate the side face and the hidden side face.
		@param irPoints The input polygon.
		*/
		void CalculateSideFace(const std::vector<Point2D>& irPoints);
		
		Int32 mDepth;						///< The depth value.
		Int32 mAngle;						///< The depth direction.
		bool mIsVertical;					///< Indicates whether the depth polygon is vertical or horizontal.
		bool mIsShownSideFace;				///< Indicates whether the side face is shown or not.
		bool mIsShownFrontFace;				///< Indicates whether the front face is shown or not.
		bool mIsShownHiddenSideFace;		///< Indicates whether the hidden side face is shown or not.
		bool mIsDepthCurvePolygon;			///< Indicates whether current DepthPolygonObject is a DepthCurvePolygonObject.
		
		std::vector<DepthLineObject::Ptr> mTopDepthLines;	///< Top depth Line.
		std::vector<DepthLineObject::Ptr> mBottomDepthLines;///< Bottom depth Line.
		GraphObject::Ptr mSideFacePtr;						///< Side face.
		GraphObject::Ptr mHiddenSideFacePtr;				///< Hidden side face.

	private:		
		/**
		This method is to calculate all the top and bottom faces.
		@param irPoints The input polygon.
		*/
		void hCalculateTopAndBottomFaces(const std::vector<Point2D>& irPoints);

		/**
		This method is to fill all the top and bottom faces.
		*/
		void hDrawTopAndBottomFaces() const;	

		/**
		This method is to calculate the drawing order of each section of the depth polygon according to mIsVertical and the depth direction (mAngle).
		*/
		bool hIsReverseOrder() const;	
	};

	inline bool DepthPolygonObject::hIsReverseOrder() const
	{
		return (mIsVertical && ((mAngle >= 0 && mAngle <= 90) || (mAngle >= 270 && mAngle <= 360)))
			||(!mIsVertical && (mAngle >= 180 && mAngle <= 360));
	}
	inline DepthLineObject::Ptr DepthPolygonObject::GetTopDepthLine() const
	{
		_ASSERTE(mTopDepthLines.size() == 1);
		return mTopDepthLines[0];
	}
	inline DepthLineObject::Ptr DepthPolygonObject::GetBottomDepthLine() const
	{
		_ASSERTE(mTopDepthLines.size() == 1);
		return mBottomDepthLines[0];
	}
	inline GraphObject::Ptr DepthPolygonObject::GetHiddenSideFace() const
	{
		return mHiddenSideFacePtr;
	}
	inline GraphObject::Ptr DepthPolygonObject::GetSideFace() const
	{
		return mSideFacePtr;
	}
}

#endif