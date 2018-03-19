//==============================================================================================
//	FILENAME	:	Transformer.h
//	AUTHOR		:	Xin Liu
//	CREATION	:	2008-05-18
//	Copyright (C) MicroStrategy Incorporated 2009
//	All Rights Reserved
//==============================================================================================

#ifndef MSICHART_TRANSFORMER_H
#define MSICHART_TRANSFORMER_H

#include "Synch/Defines/SmartPtr.h"
#include "ChartContext.h"
#include "Point2D.h"
#include "Point3D.h"
#include "Vector3D.h"


namespace MsiChart
{
	const Int32 gDefaultCubeSize = 1000;

	struct ProjectionInfo
	{
		ProjectionInfo()
			: mDistanceToEye(0.0), mProjectionPaneSize(0.0)
			, mProjectionPaneXOffset(0), mProjectionPaneYOffset(0)
			, mZoomIn(1.0)
		{}
		double mDistanceToEye;
		double mProjectionPaneSize;
		Int32 mProjectionPaneXOffset;
		Int32 mProjectionPaneYOffset;
		double mZoomIn;
	};

	struct ViewInfo
	{
		ViewInfo()
			:DistanceBetweenEyeAndLookAt(2.5 * sqrt(3.0) * gDefaultCubeSize)
			, mRotationX(0), mRotationY(0), mRotationZ(0)
			, mMove(0, 0, 0)
		{}

		double DistanceBetweenEyeAndLookAt;
		Int32 mRotationX;
		Int32 mRotationY;
		Int32 mRotationZ;
		DVector3D mMove;
	};

	class Transformer
	{
	public:
		typedef MSynch::SmartPtr<Transformer> Ptr;

		Transformer(ChartContext* ipChartContext);

		bool Transform(const DPoint3D& irPoint3D, Point2D& orPoint2D) const;
		bool Transform(const DPoint3D& irPoint3D, FPoint2D& orPoint2D) const;

		double DistanceToEye(const DPoint3D& irPoint3D) const;
		void GetEyePosition(DPoint3D& orEyePosition) const;
	//	bool IsVisible(const DVector3D& irPlaneNormal) const;
		bool IsVisible(const DVector3D& irPlaneNormal, const DPoint3D& irPoint3D) const;
		void SetLightDirection(const DVector3D& irVector3D);
		void GetLightDirection(DVector3D& orVector3D) const;
		void GetCubeSize(SPoint3D& orWallSize) const;
		void GetWallThickness(SPoint3D& orWallThickness) const;

		void GetOptions(Int32 iPropertyId, VARIANT* opValue) const;
		void Update();

		void SetProjectionArea(const Rect2D& irProjectionArea);
	private:
		void hInitialize();
		void hLoadProperties();
		void hLoadWallProperties();
		void hLoadProjectionProperties();
		void hLoadTranformProperties();

		void hTruncateSize(Int32& iorSize, Int32 iMinSize, Int32 iMaxSize);

		void hViewTransformation();
		void hPerspectiveProjection();
		void hProjectionPlaneToCairo(double& iorX, double& iorY) const;

		ChartContext* mpChartContext;

		double mMatrix[16];
		DPoint3D mEyePosition;
		DPoint3D mLookAtPosition;
		DVector3D mUp;

		// Wall properties
		SPoint3D mCubeSize;
		SPoint3D mWallThickness;

		ProjectionInfo mProjectionInfo;
		ViewInfo mViewInfo;
		double mProjectionPlaneDistance;

		DVector3D mLightDirection;
		Rect2D mProjectionArea;
		bool mAdjustProjectionPlane;
	};

	inline void Transformer::hTruncateSize(Int32& iorSize, Int32 iMinSize, Int32 iMaxSize)
	{
		iorSize = (iorSize < iMinSize) ? iMinSize : iorSize;
		iorSize = (iorSize > iMaxSize) ? iMaxSize : iorSize;
	}
}

#endif