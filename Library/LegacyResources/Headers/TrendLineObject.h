//====================================================================
//	Filename:		Kernel/SourceCode/IPhoneChart/PrivateSource/TrendLineObject.h
//	Date:			2008-11-04
//	Authors:		Bing Huang

//	Copyright (C) 1997-2008 MicroStrategy Incorporated
//	All rights reserved
//====================================================================

#ifndef MsiChart_TrendLineObject_h
#define MsiChart_TrendLineObject_h

#include "GraphObject.h"
#include "TextObject.h"
#include "CurveLineObject.h"

namespace MsiChart
{
	//extern variable
	extern const Int32 gMaxTrendLineTextLength;

	//pre-declaration
	class CategoryPlot;
	class Axis;
	
	struct GraphTrendLineOptions;
	
	class TrendLineObject : public GraphObject
	{
	public:
		typedef MSynch::SmartPtr<TrendLineObject> Ptr;

		TrendLineObject(const TripleId& irTripleId, EnumDSSGraphObject iRealObjectId, GraphObjectManager* ipManager, GraphCollectionObject* ipParentObject, 
			Axis* ipXAxis, Axis* ipYAxis,
			GraphTrendLineOptions* ipTrendLineOptions, const Rect2D& irDataArea, bool iIsVertical, 
			const Point2D& irDepthOffset);

		//virtual functions from GraphObject
		virtual void Draw() const;
		virtual void DrawOutLine();
		virtual bool PointInObject(const Point2D& irPoint);
		virtual void GenerateImageMap(ImageMap& iorImageMap);
		virtual bool ObjectInRectangle(const Rect2D& irRect);
				
		/**
		Set the two data array to the trendlineobject.
		In this function trendline object will compute the equation as well as R.
		MovingAverage trendline object will not compute R.
		@param irX The values from category axis.
		@param irY The values from value axis.
		*/
		virtual void SetData(const std::vector<double>& irX, const std::vector<double>& irY);

		/**
		Get the real object ID of the trend line object.
		We use this real object ID to identify the trend line object.
		The object ID in triple ID is used to get the format line information.
		@return mRealObjectId.
		*/
		const EnumDSSGraphObject GetRealObjectId() const;

		/**
		Get the text equation.
		@return The mTextPtr member.
		*/
		TextObject::Ptr GetEquationText();

	protected:
		/**
		Compute the trend line equation.
		@param irX The values from category axis.
		@param irY The values from value axis.
		*/
		virtual bool ComputeEquation(const std::vector<double>& irX, const std::vector<double>& irY) = 0;
		
		/**
		Compute the R(simulation degree)
		@param irX The values from category axis.
		@param irY The values from value axis.
		*/
		virtual void ComputeR(const std::vector<double>& irX, const std::vector<double>& irY) = 0;

		/**
		Get the string of the equation text object.
		*/
		virtual void GetEquationString(wchar_t* iorString) = 0;

		/**
		Get corresponding Y value from the input X value according to trend line equation.
		@param iX The input X value.
		@return The Y value.
		*/
		virtual double GetY(double iX) const = 0;

		/**
		Get the slope for specified X.
		@param iX The input X value.
		@return The slope.
		*/
		virtual double GetSlope(double iX) const = 0;

		/**
		Create equation labels according to trend line options.
		*/
		virtual void CreateEquationText();
		
		/**
		Create curve line object to simulate the trend line.
		*/
		virtual void CreateCurveLine();

		/**
		Create curve line for Polar and Radar chart.
		*/
		void CreatePolarCurveLine();

		/**
		Check whether current graph type is one of Polar and Radar.
		@return Return true if current graph type is one of Polar and Radar. Otherwise, return false.
		*/
		bool IsPolarTrendLine() const;

		/**
		Use the gaussian elimination to solve the linear equation as A*X = Y.
		The A is a n*n matrix.
		The X and Y is a n*1 vector.
		Users are responsible for the correctness and validation of the data.
		Remark: In our model, the A are guaranteed to be full-order matrix, that is |A| != 0, we could get a unique
			solution in all case because the X are all different.
			So we don't have to get a return value here.
		@param iA The input A, store the data row by row.
		@param iorXY The input for Y and output for X.
		@param iSize Indicating the n.
		*/
		void GaussElimination(std::vector<double>& irA, std::vector<double>& iorXY, const Int32 iSize);

		/**
		Indicate whether the trend line is in Value Aixs Y1 or not.
		@return true if Y2, false if Y1.
		*/
		bool IsAxisY2();

		/**
		Create a bezier curve according to the start x and end x coordinate.
		@param iStartXLogical The start X coordinate in logical space.
		@param iEndXLogical The end X coordinate in logical space.
		@return The newly created bezier curve.
		*/
		BezierCurve::Ptr CreateBezierCurve(double iStartXLogical, double iEndXLogical) const;

		/**
		Used in Draw(), DrawOutline(), PointInObject().
		MovingAverageTrendLineObject class should override this function.
		*/
		virtual void GetClockwisePolygon(std::vector<Point2D>& orPolygon) const;

		/**
		Check if a double value is 0.
		@param irValue The input value.
		@return If zero return true, else false.
		*/
		bool IsZero(double& irValue) const;

		/**
		Check whether a number is in the boundary for output or not.
		The boundary is 0.01.
		When it is in 0.01, it will be displayed as Ae+10, etc.
		Else it will be displayed normally.
		@param irNumber The input number.
		@return true if in boundary, false if not
		*/
		bool IsInBoundary(double& irNumber) const;

		/**
		This is a helper method for CreateCurveLine().
		@param iStartXLogical,iEndXLogical Specifies a range.
		@param iStep Specifies a step.
		*/
		void AddToLogarithmAxisCurve(double iStartXLogical, double iEndXLogical, double iStep);

		/**
		This is a helper method for CreatePolarCurveLine().
		@param iStartXLogical,iEndXLogical Specifies a range.
		@param iStep Specifies a step.
		*/
		void AddToPolarCurve(double iStartXLogical, double iEndXLogical, double iStep);

		//member variables
		Axis* mpXAxis;
		Axis* mpYAxis;
		GraphTrendLineOptions* mpTrendLineOptions;
		Rect2D mDataArea;
		Point2D mDepthOffset;
		bool mIsVertical;
		TextObject::Ptr mTextPtr;
		EnumDSSGraphObject mRealObjectId;
		CurveLineObject::Ptr mCurveLinePtr;
		double mMinXLogical;
		double mMaxXLogical;
		double mMinYLogical;
		double mMaxYLogical;        

	};

	inline const EnumDSSGraphObject TrendLineObject::GetRealObjectId() const
	{
		return mRealObjectId;
	}
}

#endif
