//====================================================================
//	Filename:		Kernel/SourceCode/IPhoneChart/PrivateSource/ExpTrendLineObject.h
//	Date:			2008-11-10
//	Authors:		Bing Huang

//	Copyright (C) 1997-2008 MicroStrategy Incorporated
//	All rights reserved
//====================================================================

#ifndef MsiChart_ExpTrendLineObject_h
#define MsiChart_ExpTrendLineObject_h

#include "TrendLineObject.h"

namespace MsiChart
{
	//pre-declaration
	struct GraphTrendLineOptions;
	
	class ExpTrendLineObject : public TrendLineObject
	{
	public:
		typedef MSynch::SmartPtr<ExpTrendLineObject> Ptr;

		ExpTrendLineObject(const TripleId& irTripleId, EnumDSSGraphObject iRealObjectId, GraphObjectManager* ipManager, GraphCollectionObject* ipParentObject, 
			Axis* ipXAxis, Axis* ipYAxis,
			GraphTrendLineOptions* ipTrendLineOptions, const Rect2D& irDataArea, bool iIsVertical, 
			const Point2D& irDepthOffset);

	protected:
		virtual bool ComputeEquation(const std::vector<double>& irX, const std::vector<double>& irY);
		virtual void ComputeR(const std::vector<double>& irX, const std::vector<double>& irY);
		virtual void GetEquationString(wchar_t* iorString);
		virtual double GetY(double iX) const;
		virtual double GetSlope(double iX) const;
		virtual void CreateCurveLine();
	private:
		/**
		Get the formatting string for swprintf().
		@param iIsEquation true if the string is for equation, false if for coefficients.
		@param iIsAInBoundaryOrShowEuqation If "iIsEquation" is true, it will indicate whether mA is in boundary,
			If "iIsEquation" is false, then it will indicate whether euqation is shown of not.
		@param iIsBRInBoundary If "iIsEquation" is true, it will indicate whether mB is in boundary,
			If "iIsEquation" is false, then it will indicate whether mR is in boundary.
		@return The output formatting string.
		*/
		const wchar_t * hGetFormattingString(bool iIsEquation, bool iIsAInBoundaryOrShowEuqation, bool iIsBRInBoundary) const;

		/**
		Create a bezier curve according to the start x and end x coordinate for Exp.
		@param iStartXLogical The start X coordinate in logical space.
		@param iorEndXLogical The end X coordinate in logical space, could be modified within the function.
		@param orOutArea Indicate whether the BezierCurve has gone out of data area, if it return true then we don't have to
			go forward or backward to get new trend line sample point.
		@return The newly created bezier curve.
		*/
		BezierCurve::Ptr hCreateBezierCurve(double iStartXLogical, double& iorEndXLogical, bool& orOutArea) const;

		//member variables
		double mA;
		double mB;
		double mR;
	};
}

#endif
