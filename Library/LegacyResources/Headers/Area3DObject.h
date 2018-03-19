//==============================================================================================
//	FILENAME	:	Area3DObject.h
//	AUTHOR		:	Xin Liu
//	CREATION	:	2009-05-27
//	Copyright (C) MicroStrategy Incorporated 2009
//	All Rights Reserved
//==============================================================================================

#ifndef MSICHART_AREA3DOBJECT_H
#define MSICHART_AREA3DOBJECT_H

#include "Shape3DObject.h"

namespace MsiChart
{
	enum AreaPositionType
	{
		APT_MIDDLE = 1,
		APT_FRONT = 2,
		APT_BACK = 4,		
	};

	class Area3DObject: public Shape3DObject
	{
	public:
		typedef MSynch::SmartPtr<Area3DObject> Ptr;

		Area3DObject(const TripleId& irTripleId, GraphObjectManager* ipManager, 
			const DPoint3D& irStartDataPoint, const DPoint3D& irEndDataPoint, double iBaseValue, double iAreaThickness, Int32 iAreaPositionType = APT_MIDDLE,
			GraphCollectionObject* ipParentObject = NULL, bool iHasFill = true);

		~Area3DObject();

	private:
		void hCalculateIntersectionPoint(const DPoint3D& irStartDataPoint, const DPoint3D& irEndDataPoint, DPoint3D& orIntersectionPoint);
		void hCalculateTopFacePoints(const DPoint3D& irStartDataPoint, const DPoint3D& irEndDataPoint);
		void hAddFaces();
		
		void hGetFront(Polygon3D& orFront);
		void hGetBack(Polygon3D& orBack);
		void hGetTop(Polygon3D& orTop);
		void hGetFirstTop(Polygon3D& orFirstTop);
		void hGetSecondTop(Polygon3D& orSecondTop);
		void hGetBottom(Polygon3D& orBottom);
		void hGetFirstBottom(Polygon3D& orFirstBottom);
		void hGetSecondBottom(Polygon3D& orSecondBottom);
		void hGetLeftSide(Polygon3D &orLeftSide);
		void hGetRightSide(Polygon3D &orRightSide);
		void hGetFrontDirection(DVector3D& orFrontDirection);
		void hGetRightDirection(DVector3D& orRightDirection);
		void hGetTopDirection(DVector3D& orTopDirection);
		void hGetBottomDirection(DVector3D& orBottomDirection);

		DPoint3D mStartLeftPoint;
		DPoint3D mStartRightPoint;
		DPoint3D mEndLeftPoint;
		DPoint3D mEndRightPoint;
		DPoint3D mIntersectionLeftPoint;
		DPoint3D mIntersectionRightPoint;
		double mBaseValue;
		double mAreaThickness;
		Int32 mAreaPositionType;

		bool mIsSplit;
	};
}

#endif