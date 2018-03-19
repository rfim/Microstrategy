//==============================================================================================
//	FILENAME	:	BorderObject.h
//	AUTHOR		:	Xin Liu
//	CREATION	:	3/05/09
//	Copyright (C) MicroStrategy, Inc. 2009
//==============================================================================================

#ifndef MSICHART_BORDEROBJECT_H
#define	MSICHART_BORDEROBJECT_H

#include "ShapeObject.h"

namespace MsiChart
{
	enum BorderSection
	{
		BS_MAIN_CURVE,
		BS_MIN_CORNER,
		BS_MAX_CORNER,
		BS_MIN_LINE,
		BS_MAX_LINE,
		BS_BASE_CURVE,
	};

	enum BorderStyleSection
	{
		BSS_3D_OUTER_RING,
		BSS_3D_INNER_RING,

		BSS_METALLIC_OUTER_RING,
		BSS_METALLIC_MIDDLE_RING,
		BSS_METALLIC_INNER_RING,
	};

	enum ClippedBorderShapeType
	{
		CBST_PANEL,
		CBST_SECTOR,
		CBST_ARCH_WITH_BASE,
		CBST_ARCH
	};

	struct ClippedBorderShapeInfo
	{
		typedef MSynch::SmartPtr<ClippedBorderShapeInfo> Ptr;

		ClippedBorderShapeInfo()
			: mCornerRadius(0)
			, mBaseRadius(0)
			, mBorderSection(BS_MAIN_CURVE)
			, mClippedBorderShape(CBST_SECTOR)
		{}

		float	mStartAngle;						///< The start angle.
		float	mStopAngle;							///< The stop angle.
		float	mAngularDistance;					///< The angle between start and end.
		Int32	mCornerRadius;
		Int32	mBaseRadius;
		Point<double> mMinCornerCenter;
		Point<double> mMaxCornerCenter;
		BorderSection mBorderSection;
		ClippedBorderShapeType mClippedBorderShape;
	};
	/**
	This class delegates gauge borders only used in Gauge chart.
	*/
	class BorderObject: public ShapeObject
	{
	public:
		typedef MSynch::SmartPtr<BorderObject> Ptr;

		/**
		Constructor
		@param irRect The rectangle area.
		@param iDepth The depth value.
		@param iAngle The depth direction.
		*/
		BorderObject(const TripleId& irTripleId, 					
						GraphObjectManager* ipManager,		// manager who create the object 				
						const Point2D& irCenter, 
						float iStartAngle, 
						float iStopAngle,
						Int32 iRadius,
						Int32 iThickness,
						bool iIsClipped = false,
						Int32 iCornerRadius = 0,
						Int32 iBaseRadius = 0,
						EnumDSSGraphGaugeBorderStyle iBorderStyle = DssGraphGaugeBorderStyleSimple,
						GraphCollectionObject* ipParentObject = NULL);

		virtual ~BorderObject();
		
		//virtual functions from GraphObject
		virtual void Draw() const;
		virtual bool ObjectInRectangle(const Rect2D& irRect);
		virtual void DrawOutLine();
		virtual bool PointInObject(const Point2D& irPoint);
		virtual void GenerateImageMap(ImageMap& iorImageMap);

		//pure virtual function from ShapeObject
		virtual void GetBoundingRect(Rect2D& orBoundingRect) const; 
		virtual bool SupportBevelEffect() const;
		virtual bool IsCircularShape() const;

		Int32 GetInnerRadius();
	
	protected:
		//virtual functions from ShapeObject
		virtual void GetClockwisePolygon(std::vector<Point2D>& orPolygon) const; 
		virtual void FillBevel() const;

	private:
		void hCalculateCornerCenters(Point2D iCenter) const;
		void hCalculateClippedBorderShapeType();
		void hSetBorderPath(Point<double> iCenter, double iThickness = 0.0, bool iIsClockwise = true) const;
		void hDrawInnerAndOuterPathAsWhole(Point<double> iCenter, double iOuterRadius, double iInnerRadius) const;
		void hDrawBorderPath(Point<double> iCenter, double iThickness = 0.0, bool iIsClockwise = true) const;

		void hFillDonutRingSector(const Point<double>& irCenter, double iOuterRadius, double iInnerRadius) const;
		void hFillDonutRect(const Point<double>& irStartPoint, const Point<double>& irEndPoint) const;
	//	void hSetSectorPath(const Point<double>& irCenter, double iOuterRadius, double iInnerRadius, double iStartAngle, double iEndAngle);
		void hDrawAndFillClippedBorder() const;
		void hDrawAndFillClippedBorderDonut() const;
		void hDrawAndFillNormalBorder() const;
		void hDrawAndFillBorder3D() const;
		void hDrawAndFillBorderMetallic() const;
		void hDrawAndFillEmbossedEdge() const;

		Point2D hGetOffsetCenterForEmbossedEdge() const;

		void hFillBevelEffect() const;
		void hFillDonutEffect() const;
		void hFillBorderStyleEffect() const;
		void hFillBorderStyleEffect3D() const;
		void hFillBorderStyleEffectMetallic() const;

		void hCalculateMediumRadiusOfMetallic(double& orOuterMediumRadius, double& orInnerMediumRadius) const;

		Point<double> hGetIntersectionOfMainCurveAndMinCorner(double iThickness) const;
		Point<double> hGetIntersectionOfMainCurveAndMaxCorner(double iThickness) const;
		Point<double> hGetIntersectionOfMinCornerAndMinSideLine(double iThickness) const;
		Point<double> hGetIntersectionOfMaxCornerAndMaxSideLine(double iThickness) const;
		Point<double> hGetIntersectionOfMinSideLineAndBaseCurve(double iThickness) const;
		Point<double> hGetIntersectionOfMaxSideLineAndBaseCurve(double iThickness) const;
		double hGetMinSideBaseAngle(double iThickness) const;
		double hGetMaxSideBaseAngle(double iThickness) const;
		double hGetCornerAngularDistance() const;

		bool hPointInClippedBorder(const Point<double>& irPoint, double iThickness) const;
		bool hPointInCircle(const Point<double>& irPoint, const Point<double>& irCenter, double iRadius) const;
		bool hPointInSector(const Point<double>& irPoint, const Point<double>& irCenter, double iRadius, double iStartAngle, double iEndAngle) const;

		//The method definition is similar PieSliceObject::GetSectorImageMap(), need code refactory in the future.
		void hGenerateSectorImageMap(const Point2D& irCenter, const Int32 iOuterRadius, const Int32 iInnerRadius, 
									const Int32 iStartAngle, const Int32 iEndAngle, ImageMap& iorImageMap) const;
		void hGenerateApproximatePolylineOfCircle(const Point2D& irCenter, const Int32 iRadius, 
									const Int32 iStartAngle, const Int32 iEndAngle, std::vector<Point2D>& orPolyline) const;
		void hGenerateClippedBorderImageMap(ImageMap& iorImageMap) const;
		
		//data members
		EnumDSSGraphGaugeBorderStyle mBorderStyle;	///< The border style.
		Point2D	mCenter;							///< The cneter point of the border.
		Int32	mRadius;
		Int32	mThickness;
		bool	mIsClipped;
		bool	mIsSolid;
		ClippedBorderShapeInfo::Ptr	mBorderInfoPtr;
		mutable BorderStyleSection	mBorderStyleSection;
	};

	inline Int32 BorderObject::GetInnerRadius()
	{
		return mRadius - mThickness;
	}
}

#endif