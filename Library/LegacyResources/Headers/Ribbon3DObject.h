//==============================================================================================
//	FILENAME	:	Ribbon3DObject.h
//	AUTHOR		:	Xin Liu
//	CREATION	:	2009-05-27
//	Copyright (C) MicroStrategy Incorporated 2009
//	All Rights Reserved
//==============================================================================================

#ifndef MSICHART_RIBBON3DOBJECT_H
#define MSICHART_RIBBON3DOBJECT_H

#include "Shape3DObject.h"

namespace MsiChart
{
	enum RibbonPositionType
	{
		RPT_MIDDLE = 1,
		RPT_FRONT = 2,
		RPT_BACK = 4,		
	};

	class Ribbon3DObject: public Shape3DObject
	{
	public:
		typedef MSynch::SmartPtr<Ribbon3DObject> Ptr;

		Ribbon3DObject(const TripleId& irTripleId, GraphObjectManager* ipManager, 
			const DPoint3D& irStartDataPoint, const DPoint3D& irEndDataPoint, double iRibbonThickness, double iRibbonHeight, Int32 iRibbonPositionType = RPT_MIDDLE,
			GraphCollectionObject* ipParentObject = NULL, bool iHasFill = true);

		~Ribbon3DObject();

	private:
		void hAddFaces();

		void hGetFront(Polygon3D& orFront);
		void hGetBack(Polygon3D& orBack);
		void hGetTop(Polygon3D& orTop);
		void hGetBottom(Polygon3D& orBottom);
		void hGetLeftSide(Polygon3D& orLeftSide);
		void hGetRightSide(Polygon3D& orRightSide);

		void hGetFrontDirection(DVector3D& orFrontDirection);
		void hGetRightDirection(DVector3D& orRightDirection);
		void hGetTopDirection(DVector3D& orTopDirection);

		DPoint3D hGetTopLeftStartPoint();
		DPoint3D hGetTopRightStartPoint();
		DPoint3D hGetTopLeftEndPoint();
		DPoint3D hGetTopRightEndPoint();
		DPoint3D hGetBottomLeftStartPoint();
		DPoint3D hGetBottomRightStartPoint();
		DPoint3D hGetBottomLeftEndPoint();
		DPoint3D hGetBottomRightEndPoint();

		DVector3D hGetThicknessVector();
		DVector3D hGetHeightVector();

		DPoint3D mStartDataPoint;
		DPoint3D mEndDataPoint;
		double mThickness;
		double mHeight;
		Int32 mRibbonPositionType;
	};

}

#endif