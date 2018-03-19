//====================================================================
//	Filename:		Kernel/SourceCode/IPhoneChart/PrivateSource/LogTrendLineObject.h
//	Date:			2008-11-10
//	Authors:		Bing Huang

//	Copyright (C) 1997-2008 MicroStrategy Incorporated
//	All rights reserved
//====================================================================

#ifndef MsiChart_LogTrendLineObject_h
#define MsiChart_LogTrendLineObject_h

#include "TrendLineObject.h"

namespace MsiChart
{
	//pre-declaration
	struct GraphTrendLineOptions;
	
	class LogTrendLineObject : public TrendLineObject
	{
	public:
		typedef MSynch::SmartPtr<LogTrendLineObject> Ptr;

		LogTrendLineObject(const TripleId& irTripleId, EnumDSSGraphObject iRealObjectId, GraphObjectManager* ipManager, GraphCollectionObject* ipParentObject, 
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
	};
}

#endif
