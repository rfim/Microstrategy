//==============================================================================================
//	FILENAME	:	DepthLineObject.h
//	AUTHOR		:	Xin Liu
//	CREATION	:	10/20/08
//	Copyright (C) MicroStrategy, Inc. 2008
//==============================================================================================

#ifndef MSICHART_DEPTHLINEOBJECT_H
#define	MSICHART_DEPTHLINEOBJECT_H

#include "ShapeObject.h"

namespace MsiChart
{
	/**
	This class delegates line object with depth.
	*/
	class DepthLineObject: public ShapeObject
	{
	public:
		typedef MSynch::SmartPtr<DepthLineObject> Ptr;

		/**
		Constructor for generate depth line.
		@param irTripleId,ipManager,ipParentObject Used by base class.
		@param irPoints The polyline.
		@param iDepth The depth value.
		@param iAngle The depth direction.
		*/
		DepthLineObject(const TripleId& irTripleId, GraphObjectManager* ipManager,const std::vector<Point2D>& irPoints, 
			Int32 iDepth, Int32 iAngle,	GraphCollectionObject* ipParentObject, bool iIsVertical = true, 
			bool iUseDarkerColor = false);

		DepthLineObject(const TripleId& irTripleId, GraphObjectManager* ipManager,		
			const Point2D& irStartPoint, const Point2D& irEndPoint, Int32 iDepth, Int32 iAngle,
			GraphCollectionObject* ipParentObject, bool iIsVertical, bool iUseDarkerColor = false);

		DepthLineObject(const FPoint2D& irStartPoint, const FPoint2D& irEndPoint, DepthLineObject* ipDepthLine);

		virtual ~DepthLineObject();
		
		//virtual functions from GraphObject
		virtual void Draw() const;
		virtual void DrawOutLine();
		virtual bool PointInObject(const Point2D& irPoint);
		virtual bool ObjectInRectangle(const Rect2D& irRect); 
		virtual void GenerateImageMap(ImageMap& iorImageMap);

		//pure virtual function from ShapeObject
		virtual void GetBoundingRect(Rect2D& orBoundingRect) const; 	

		/**	
		Check whether current depth line need to be drawn before another depth line. 		
		@iDepthLinePtr Specifies another depth Line.
		@return Return true if current depth line need to be drawn before the input depth line.
		*/
		virtual bool IsBefore(DepthLineObject::Ptr iDepthLinePtr) const;
		
		/**
		Split current depth line at tangent points. For this method, current class need to do nothing because 
		straight depth lines do not need to be split at tangent points. This method needs to be implemented by 
		derived class.
		@param iorDepthLines After splitting, the current depth lines will become the first piece of the split 
		depth lines. The other split depth lines will be put into iorDepthLines.
		*/
		virtual void SplitAtTangentPoints(std::vector<DepthLineObject::Ptr>& iorDepthLines);
		
		/**
		Split current depth line at intersection points with another depth line.
		@param iDepthLinePtr Specifies another depth line.
		@param iorDepthLines After splitting, the current depth lines will become the first piece of the split 
		depth lines. The other split depth lines will be put into iorDepthLines.
		*/
		virtual void SplitAtIntersections(DepthLineObject::Ptr iDepthLinePtr, std::vector<DepthLineObject::Ptr>& iorDepthLines);

		/**
		Restore current depth line after it is split. 
		*/
		virtual void RestoreDepthLine();

		/** This method is mainly used for debugging. 
		Each depth line can have one index to identify itself.
		@param i Specifies an index for current depth line.
		*/
		void SetIndex(Int32 i);
		/** This method is mainly used for debugging. 
		Each depth line can have one index to identify itself.
		@return Return the index for current depth line.
		*/
		Int32 GetIndex();
	
	protected:
		virtual void GetClockwisePolygon(std::vector<Point2D>& orPolygon) const; 		

		/**
		This method is to calculate all the sections of the depth polyline according to input polyline.
		@param irPoints The input polyline.
		*/
		void hCalculateSections(const std::vector<Point2D>& irPoints);	
		/**
		This method is to fill one section of the depth polyline.
		@param iSectionIndex The section index.
		*/
		void hFillSection(size_t iSectionIndex) const;

		/**
		Get polygon of straight depth line.
		@param Specifies a depth.
		@param orPolygon Output polygon.
		*/
		void hGetPolygonByDepth(Int32 iDepth, std::vector<FPoint2D>& orPolygon) const;
	
		std::vector<Polygon2D> mSections;	///< A list of polygons all the sections of the depth polyline.		
		std::vector<FPoint2D> mPolygon;		///< A polygon for straight depth line.
		FPoint2D mStartPoint;				///< Indicates the start point of straight depth line.
		FPoint2D mEndPoint;					///< Indicates the end point of straight depth line.
		Int32 mDepth;						///< The depth value.
		Int32 mAngle;						///< The depth direction.
		mutable size_t mSectionIndex;		///< The index of the polyline sections, which is used for FormatFill.
		bool mIsVertical;					///< Indicates whether the depth polyline is vertical or horizontal.
		bool mUseDarkerColor;				///< Indicates whether darker color should be used or not.
		bool mIsStraightDepthLine;			///< Indicates whether current depth line is a straight depth line;
		Int32 mIndex;						///< Identify current depth line. For debugging.		

	};
	inline void DepthLineObject::SetIndex(Int32 i)
	{
		mIndex = i;
	}
	inline Int32 DepthLineObject::GetIndex()
	{
		return mIndex;
	}
}

#endif