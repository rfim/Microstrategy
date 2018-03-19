//==============================================================================================
//	FILENAME	:	DepthRectangleObject.h
//	AUTHOR		:	Xin Liu
//	CREATION	:	10/20/08
//	Copyright (C) MicroStrategy, Inc. 2008
//==============================================================================================

#ifndef MSICHART_DEPTHRECTANGLEOBJECT_H
#define	MSICHART_DEPTHRECTANGLEOBJECT_H

#include "RectangleObject.h"

namespace MsiChart
{
	/**
	Indicates whether the face is front face, side face or top or bottom face. Used privately for FormatFill.
	*/
	typedef enum 
	{
		FT_FRONT,
		FT_TOP_OR_BOTTOM,
		FT_SIDE,
		FT_ALL
	}FaceType;

	/**
	This class delegates rectangle object with depth or pseudo-3D cube.
	*/
	class DepthRectangleObject: public RectangleObject
	{
	public:
		typedef MSynch::SmartPtr<DepthRectangleObject> Ptr;

		/**
		Constructor
		@param irRect The rectangle area.
		@param iDepth The depth value.
		@param iAngle The depth direction.
		*/
		DepthRectangleObject(const TripleId& irTripleId, 					
						GraphObjectManager* ipManager,		// manager who create the object 				
						const Rect2D& irRect, Int32 iDepth, Int32 iAngle,
						GraphCollectionObject* ipParentObject = NULL, unsigned char iDataPointFaceType = FT_FRONT);

		virtual ~DepthRectangleObject();
		
		//virtual functions from GraphObject
		//virtual void Draw() const;
		virtual void DrawOutLine();
		virtual bool PointInObject(const Point2D& irPoint);
		virtual void GenerateImageMap(ImageMap& iorImageMap);	//reuse RectangleObject::GenerateImageMap() method
		virtual Int32 DistanceToPoint(const Point2D& irPoint);
		virtual Point2D GetDataPoint();


		//pure virtual function from ShapeObject
		virtual void GetBoundingRect(Rect2D& orBoundingRect) const; 	

		//virtual void SetProgress(double iProgress);

	protected:
		virtual void GetClockwisePolygon(std::vector<Point2D>& orPolygon) const; 	
		
		/// This is a helper method;
		virtual void DoDraw() const;
		
		virtual void SetRect(const Rect2D& irRect);
	
	private:
		/**
		This method is to calculate the top face and side face of the depth rectangle or the cube.
		*/
		void hCalcuateTopAndSideFace();

		/**
		This method is to fill the top or the bottom face of the depth rectangle or the cube.
		*/
		void hFillTop() const;

		/**
		This method is to fill the side face of the depth rectangle or the cube.
		*/
		void hFillSide()const;

		/**
		this method is to fill the front face of the depth rectangle or the cube.
		*/
		void hFillFront()const;	
	
		Rect2D mFront;				///< The front rectangle.
//		Rect2D mFinalFront;

		std::vector<Point2D> mTop;	///< The polygon delegating the top or the bottom face.
		std::vector<Point2D> mSide;	///< The polygon delegating the side face.
		
//		std::vector<Point2D> mFinalTop;	///< The polygon delegating the top or the bottom face.
//		std::vector<Point2D> mFinalSide;	///< The polygon delegating the side face.

		Int32 mDepth;				///< The depth value.
		Int32 mAngle;				///< The depth direction.
		mutable unsigned char mFaceType;	///< Indicates the face type, top face, side face or front face. It is used for FormatFill.
		unsigned char mDataPointFaceType;  ///< Decides which side is the main side used for calculate data points. E.g. when used in Gantt charts, the mMainFace should be FT_FRONT.

	};

}

#endif
