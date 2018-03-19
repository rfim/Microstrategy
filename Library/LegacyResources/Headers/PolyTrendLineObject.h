//====================================================================
//	Filename:		Kernel/SourceCode/IPhoneChart/PrivateSource/PolyTrendLineObject.h
//	Date:			2009-3-12
//	Authors:		Bing Huang

//	Copyright (C) 1997-2008 MicroStrategy Incorporated
//	All rights reserved
//====================================================================

#ifndef MsiChart_PolyTrendLineObject_h
#define MsiChart_PolyTrendLineObject_h

#include "TrendLineObject.h"


namespace MsiChart
{
	//pre-declaration
	struct GraphTrendLineOptions;
	
	class PolyTrendLineObject : public TrendLineObject
	{
	public:
		typedef MSynch::SmartPtr<PolyTrendLineObject> Ptr;

		PolyTrendLineObject(const TripleId& irTripleId, EnumDSSGraphObject iRealObjectId, GraphObjectManager* ipManager, GraphCollectionObject* ipParentObject, 
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
		@param iIsFirstOrShowEuqation If "iIsEquation" is true, it will indicate whether it is the first parameter in the equation
			If "iIsEquation" is false, then it will indicate whether euqation is shown of not.
		@param iIsInBoundary Whether the value is in boundary or not (The boundary is 0.01).
		@return The output formatting string.
		*/
		const wchar_t * hGetFormattingString(bool iIsEquation, bool iIsFirstOrShowEuqation, bool iIsInBoundary) const;

		
		//different from the order in TrendLineOption
		Int32 mOrder;
		double mR;
		std::vector<double> mParameter;
	};
}

#endif
