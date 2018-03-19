//=====================================================================================
//	Filename:		Kernel/SourceCode/ChartEngine/PrivateSource/FunnelObject.h
//	Date:			2009-04-07
//	Authors:		Bing Huang

//	Copyright (C) 1997-2008 MicroStrategy Incorporated
//	All rights reserved
//=====================================================================================

#ifndef MsiChart_FunnelObject_h
#define MsiChart_FunnelObject_h

#include "ShapeObject.h"
#include "CurveLineObject.h"

namespace MsiChart
{
	//related structs and enums
	struct FunnelInfo
	{
		FunnelInfo(double iScale, Point2D& irTopCenter, Point2D& irBottomCenter, float iTopRadius, float iBottomRadius, 
		Int32 iHeight, bool iIs2DFunnle);

		double mScale;
		
		Point2D mTopCenter;//use Int32 here to avoid overflow for Y coordinate
		Point2D mBottomCenter;
		float mTopRadiusA;
		float mBottomRadiusA;
		float mTopRadiusB;
		float mBottomRadiusB;
		Int32 mHeight;
		//the flag is false when there is tilt only, not related to width
		bool mIs2DFunnel;
	};

	class FunnelObject : public ShapeObject
	{
	public:
		typedef MSynch::SmartPtr<FunnelObject> Ptr;

		FunnelObject(const TripleId& irTripleId, GraphObjectManager* ipManager, GraphCollectionObject* ipParentObject, FunnelInfo& irFunnelInfo);

		//virtual functions from GraphObject
		virtual void Draw() const;
		virtual void DrawOutLine();
		virtual bool PointInObject(const Point2D& irPoint);
		virtual bool ObjectInRectangle(const Rect2D& irRect);
		virtual void GenerateImageMap(ImageMap& iorImageMap);
		virtual void GetOptions(Int32 iPropertyId, CComVariant* opValue) const;
		virtual void GetBoundingRect(Rect2D& orBoundingRect) const;
		virtual Point2D GetDataPoint();

		//virtual function from ShapeObject
		virtual bool SupportBevelEffect() const;

		//helper method for label & leader line position
		Point2D GetMiddlePoint() const;
		Int32 GetMiddleWidth() const;

	protected:
		//pure virtual functions from ShapeObject
		virtual void GetClockwisePolygon(std::vector<Point2D>& orPolygon) const; 
		virtual bool IsCircularShape() const;

	private:
		
		void hDraw2DFunnel() const;
		void hDraw3DFunnel(bool iIsOutline) const;
		void hDrawSurface(bool iIsInnerSurface, bool iIsLinePath) const;
		void hDrawTop() const;
		void hFillInnerSurface() const;
		bool hPointInEllipse(const Point2D& irPoint, const Point<Int32>& irCenter, Int32 iRadiusA, Int32 iRadiusB);
		
		void hCreateCurves();
		void hCreateOneCurve(CurveLineObject::Ptr iCurvePtr, bool iIsTop, bool iInnerAngle, Point2D iOffset);
		void hCreate2DPolygon();
		void hCreate3DPolygon();

		bool hIsPointInPolygon(const Point2D& irPoint) const;
		bool hIsPointInFunnelEllipse(const Point2D& irPoint, bool iIsTop, bool iIsHeight, bool iIsUpPart);

		void hGenerateImageMap(bool iIsInnerSurface, ImageMap& iorImageMap);

		FunnelInfo mFunnelInfo;

		std::vector<Point2D> mPolygon;
		CurveLineObject::Ptr mTopInnerPtr;
		CurveLineObject::Ptr mTopOuterPtr;
		CurveLineObject::Ptr mBottomInnerPtr;
		CurveLineObject::Ptr mBottomOuterPtr;

		CurveLineObject::Ptr mWholeEllipsePtr;

		double mTopTheta;
		double mBottomTheta;
		bool mBottomInsideTop;
		
	};
}

#endif
