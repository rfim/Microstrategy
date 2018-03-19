//==============================================================================================
//	FILENAME	:	Surface3DObject.h
//	AUTHOR		:	Xin Liu
//	CREATION	:	2009-05-25
//	Copyright (C) MicroStrategy Incorporated 2009
//	All Rights Reserved
//==============================================================================================

#ifndef MSICHART_SURFACE3DOBJECT_H
#define MSICHART_SURFACE3DOBJECT_H

#include "Shape3DObject.h"

namespace MsiChart
{
	enum SurfaceType
	{
		ST_SURFACE,
		ST_SURFACE_WITH_SIDES,
		ST_HONEYCOMBED_SURFACE
	};

	class Surface3DObject : public Shape3DObject
	{
	public:
		typedef MSynch::SmartPtr<Surface3DObject> Ptr;

		Surface3DObject(const TripleId& irTripleId, GraphObjectManager* ipManager, 
			const std::vector<DPoint3D*>& irStartDataPoints, const std::vector<DPoint3D*>& irEndDataPoints, double iBaseValue, bool iIsLast = false,
			GraphCollectionObject* ipParentObject = NULL, bool iHasFill = true);
		~Surface3DObject();

	private:
		
		void hAddSurfaces();
		void hAddTopSurfaces();
		void hAddSideFaces();
		void hAddSections();

		bool hGetSide(Int32 iIndex, Polygon3D& orSide);
		void hGetTopSurfaces(Int32 iIndex, Polygon3D& orFirstTopSurface, DVector3D& orFirstNormalDirection, bool& orIsFirstValid
			, Polygon3D& orSecondTopSurface, DVector3D& orSecondNormalDirection, bool& orIsSecondValid);
		bool hGetSection(Int32 iIndex, Polygon3D& orSection);

		void hGetSideNormalDirection(DVector3D& orSideNormalDirection);
		bool hGetFrontNormalDirection(DVector3D& orFrontNormalDirection);
		bool hGetBackNormalDirection(DVector3D& orBackNormalDirection);

		void hCalculateFrontNormalDirection();
		void hCalculateSideNormalDirection();
		void hCalculateNonNullDataPointIndex();


		std::vector<DPoint3D*> mStartDataPoints;
		std::vector<DPoint3D*> mEndDataPoints;
		double mBaseValue;
		bool mIsLast;
		Int32 mSurfaceType;

		DVector3D mFrontNormalDirection;
		DVector3D mSideNormalDirection;
		Int32 mNonNullDataPointIndex;
	};
}


#endif