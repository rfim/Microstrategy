//====================================================================
//	Filename:		Kernel/SourceCode/IPhoneChart/PrivateSource/LinearTrendLineObject.h
//	Date:			2008-11-10
//	Authors:		Bing Huang

//	Copyright (C) 1997-2008 MicroStrategy Incorporated
//	All rights reserved
//====================================================================

#ifndef MsiChart_LinearTrendLineObject_h
#define MsiChart_LinearTrendLineObject_h

#include "TrendLineObject.h"
#include "StraightLineObject.h"

namespace MsiChart
{
	//pre-declaration
	struct GraphTrendLineOptions;
	
	class LinearTrendLineObject : public TrendLineObject
	{
	public:
		typedef MSynch::SmartPtr<LinearTrendLineObject> Ptr;

		LinearTrendLineObject(const TripleId& irTripleId, EnumDSSGraphObject iRealObjectId, GraphObjectManager* ipManager, GraphCollectionObject* ipParentObject, 
			Axis* ipXAxis, Axis* ipYAxis,
			GraphTrendLineOptions* ipTrendLineOptions, Rect2D& irDataArea, bool iIsVertical, 
			const Point2D& irDepthOffset);

		virtual void Draw() const;
		virtual void DrawOutLine();
		virtual bool PointInObject(const Point2D& irPoint);
		virtual void GenerateImageMap(ImageMap& iorImageMap);
		virtual bool ObjectInRectangle(const Rect2D& irRect);		
		virtual void SetData(std::vector<double>& irX, std::vector<double>& irY);
	protected:
		virtual bool ComputeEquation(std::vector<double>& irX, std::vector<double>& irY);
		virtual void ComputeR(std::vector<double>& irX, std::vector<double>& irY);
		virtual void GetEquationString(wchar_t* iorString);
		virtual double GetY(double iX) const;
		virtual double GetSlope(double iX) const;
	private:
		/**
		Create a straight line.
		*/
		void hCreateLine();
		
		/**
		Get the formatting string for swprintf().
		@param iIsEquation true if the string is for equation, false if for coefficients.
		@param iIsBInBoundaryOrShowEuqation If "iIsEquation" is true, it will indicate whether mB is in boundary,
			If "iIsEquation" is false, then it will indicate whether euqation is shown of not.
		@param iIsARInBoundary If "iIsEquation" is true, it will indicate whether mA is in boundary,
			If "iIsEquation" is false, then it will indicate whether mR is in boundary.
		@return The output formatting string.
		*/
		const wchar_t * hGetFormattingString(bool iIsEquation, bool iIsBInBoundaryOrShowEuqation, bool iIsARInBoundary) const;
		//member variables
		double mA;
		double mB;
		double mR;

		StraightLineObject::Ptr mLinePtr;
	};
}

#endif
