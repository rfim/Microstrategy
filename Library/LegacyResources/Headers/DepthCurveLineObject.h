//=====================================================================================
//	Filename:		Kernel/SourceCode/IPhoneChart/PrivateSource/                                                      .h
//	Date:			2008-12-08
//	Authors:		Bing Huang

//	Copyright (C) 1997-2008 MicroStrategy Incorporated
//	All rights reserved
//=====================================================================================

#ifndef MsiChart_DepthCurveLineObject_h
#define MsiChart_DepthCurveLineObject_h

#include "DepthLineObject.h"

namespace MsiChart
{
	class DepthCurveLineObject : public DepthLineObject
	{
	public:
		typedef MSynch::SmartPtr<DepthCurveLineObject> Ptr;

		DepthCurveLineObject(const TripleId& irTripleId, 					
			GraphObjectManager* ipManager, GraphCollectionObject* ipParentObject, 
			const std::vector<Point2D>& irPoints, Int32 iDepth, Int32 iAngle, bool iIsVertical, bool iUseDarkerColor = false);
		DepthCurveLineObject(const TripleId& irTripleId, 					
			GraphObjectManager* ipManager, GraphCollectionObject* ipParentObject, 
			CurveLineObject::Ptr iCurveLinePtr, CurveLineObject::Ptr iCopyOriginalPtr, CurveLineObject::Ptr iCopyOffsetPtr,
			Int32 iDepth, Int32 iAngle, bool iIsVertical, bool iUseDarkerColor = false);
		virtual ~DepthCurveLineObject();

		//virtual functions from GraphObject
		virtual void Draw() const;
		virtual void DrawOutLine();
		virtual bool PointInObject(const Point2D& irPoint);
		virtual bool ObjectInRectangle(const Rect2D& irRect); 
		virtual void GenerateImageMap(ImageMap& iorImageMap);

		//pure virtual function from ShapeObject
		virtual void GetBoundingRect(Rect2D& orBoundingRect) const; 
		virtual bool SupportBevelEffect() const;

		//derived from DepthLineObject class
		virtual bool IsBefore(DepthLineObject::Ptr iDepthLinePtr) const;
		virtual void SplitAtTangentPoints(std::vector<DepthLineObject::Ptr>& iorDepthLines);
		virtual void SplitAtIntersections(DepthLineObject::Ptr iDepthLinePtr, std::vector<DepthLineObject::Ptr>& iorDepthLines);
		virtual void RestoreDepthLine();		

	protected:
		virtual void GetClockwisePolygon(std::vector<Point2D>& orPolygon) const;
		
	private:
		/**
		Create depth line objects for each curve line in irCurveLines, and put then into iorDepthLines.
		This fucntion is called by SplitAtTangentPoints() or SplitAtIntersections(), so we will also update the 
		mOffsetCurvePtr member according to mOriginalCurvePtr in the end.
		@param irCurveLines The input curve lines.
		@param iorDepthLines The output vector for depth line.
		*/
		void hCreateDepthLineObjects(std::vector<CurveLineObject::Ptr>& irCurveLines, std::vector<DepthLineObject::Ptr>& iorDepthLines);

		/**
		Create cairo path for Draw() method.
		If It is out line, we will draw out line for the whole series without regard to splitting.
		@param iIsLinePath Indicates whether it is line path or not.
		@param iIsOutLine Indicates whether it is out line path.
		*/
		void hSetPath(bool iIsLinePath, bool iIsOutLine) const;

		CurveLineObject::Ptr mOriginalCurvePtr;
		CurveLineObject::Ptr mOffsetCurvePtr;
		CurveLineObject::Ptr mCopyOriginalCurvePtr;
		CurveLineObject::Ptr mCopyOffsetCurvePtr;
		Point2D mOffset;
	};
}

#endif