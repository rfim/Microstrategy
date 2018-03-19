//
//  GraphMatrixTrendLineObject.h
//  IPhoneChart
//
//  Created by Li, En on 7/16/14.
//
//

#ifndef MsiChart_GraphMatrixTrendLineObject_h
#define MsiChart_GraphMatrixTrendLineObject_h


#include "TrendLineObject.h"

namespace MsiChart
{
	//pre-declaration
	struct GraphTrendLineOptions;
	
	class GraphMatrixTrendLineObject : public TrendLineObject
	{
	public:
		typedef MSynch::SmartPtr<GraphMatrixTrendLineObject> Ptr;
        
        GraphMatrixTrendLineObject(const TripleId& irTripleId, GraphObjectManager* ipManager, GraphCollectionObject* ipParentObject,
                                         Axis* ipXAxis, Axis* ipYAxis, std::vector<double> iDataValues, std::vector<double> iCoefficients,
                                   GraphTrendLineOptions* ipTrendLineOptions, const Rect2D& irDataArea, bool iIsVertical);
        virtual void Draw() const;
        
	protected:
		virtual bool ComputeEquation(const std::vector<double>& irX, const std::vector<double>& irY);
		virtual void ComputeR(const std::vector<double>& irX, const std::vector<double>& irY);
		virtual void GetEquationString(wchar_t* iorString);
		virtual double GetY(double iX) const;
		virtual double GetSlope(double iX) const;

	private:
        
        void hCreateTrendLine();
        
        bool hCalcValues(std::vector<double> iDataValues);
        
		/**
         Create a bezier curve according to the start x and end x coordinate for Exp.
         @param iStartXLogical The start X coordinate in logical space.
         @param iorEndXLogical The end X coordinate in logical space, could be modified within the function.
         @param orOutArea Indicate whether the BezierCurve has gone out of data area, if it return true then we don't have to
         go forward or backward to get new trend line sample point.
         @return The newly created bezier curve.
         */
		BezierCurve::Ptr hCreateBezierCurve(double iStartXLogical, double iStartYLogical, double iEndXLogical, double iEndYLogical, bool& orOutArea) const;
        
        
        // GM Trendline Support
        std::vector<double> mDataValues;   // length equals 2 * n, n pairs of (x, y)
        std::vector<double > mCoefficients;
        GraphObject::Ptr mDeformedTrendLine;
        bool mDeformed;
        
	};
}

#endif