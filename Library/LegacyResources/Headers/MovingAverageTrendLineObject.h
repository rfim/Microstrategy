//====================================================================
//	Filename:		Kernel/SourceCode/IPhoneChart/PrivateSource/MovingAverageTrendLineObject.h
//	Date:			2008-11-10
//	Authors:		Bing Huang

//	Copyright (C) 1997-2008 MicroStrategy Incorporated
//	All rights reserved
//====================================================================

#ifndef MsiChart_MovingAverageTrendLineObject_h
#define MsiChart_MovingAverageTrendLineObject_h

#include "TrendLineObject.h"
#include "PolyLineObject.h"

namespace MsiChart
{
	//pre-declaration
	struct GraphTrendLineOptions;
	
	class MovingAverageTrendLineObject : public TrendLineObject
	{
	public:
		typedef MSynch::SmartPtr<MovingAverageTrendLineObject> Ptr;

		MovingAverageTrendLineObject(const TripleId& irTripleId, EnumDSSGraphObject iRealObjectId, GraphObjectManager* ipManager, GraphCollectionObject* ipParentObject, 
			Axis* ipXAxis, Axis* ipYAxis,
			GraphTrendLineOptions* ipTrendLineOptions, const Rect2D& irDataArea, bool iIsVertical, 
			const Point2D& irDepthOffset);

		virtual void Draw() const;
		virtual void DrawOutLine();
		virtual bool PointInObject(const Point2D& irPoint);
		virtual void GenerateImageMap(ImageMap& iorImageMap);
		virtual bool ObjectInRectangle(const Rect2D& irRect);		
		virtual void SetData(const std::vector<double>& irX, const std::vector<double>& irY);
	protected:
		virtual bool ComputeEquation(const std::vector<double>& irX, const std::vector<double>& irY);
		virtual void ComputeR(const std::vector<double>& irX, const std::vector<double>& irY);
		virtual void GetEquationString(wchar_t* iorString);
		virtual double GetY(double iX) const;
		virtual double GetSlope(double iX) const;
		virtual void GetClockwisePolygon(std::vector<Point2D>& orPolygon) const;
	private:
		//member variables
		//std::vector<Point2D> mCoordinate;
		PolyLineObject::Ptr mPolyLinePtr;
	};
}

#endif
