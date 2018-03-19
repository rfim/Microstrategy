//=======================================================================
//	Filename:		CategoryPlot.h
//	Date:			2008-10-06
//	Authors:		Bing Huang

//	Copyright (C) 1997-2008 MicroStrategy Incorporated
//	All rights reserved
//=======================================================================

#ifndef MsiChart_CategoryPlot_h
#define MsiChart_CategoryPlot_h

#include "Plot.h"
#include "CategoryAxis.h"
#include "ValueAxis.h"
#include "SeriesRenderer.h"

namespace MsiChart
{
	//enumerations and structs
	typedef enum
	{
		OR_HORIZONTAL,
		OR_VERTICAL
	}EnumOrientation;

	class CategoryPlot : public Plot
	{
	public:
		typedef MSynch::SmartPtr<CategoryPlot> Ptr;

		CategoryPlot(const TripleId& irTripleId, GraphObjectManager* ipManager, const Rect2D& irDataArea);

		/**
		Create the labels.
		@param irTripleId The input triple ID.
		@param iValue The input value used for data value.
		@param ipFormat The input number formatting information.
			If ipFormat == NULL then no data value will be created.
		@param irString The input string used for data label.
			If irString == L"" then no data label will be created.
		@param iRiserCollectionPtr The input collection where the label should be in.
		@return The output pointer to text object.
		*/
		TextObject::Ptr CreateRiserLabel(const TripleId& irTripleId, double iValue, ChartAdvFormat* ipFormat, const MBase::String& irString, GraphCollectionObject::Ptr iRiserCollectionPtr);

		/**
		Initialization to plot
		*/
		virtual void Init() = 0;

		/**
		Get the min & max value of the data in this plot for specified value axis.
		It is used to set range of ValueAxis.
		@param iValueAxis The input value aixs index.
		@param iorMin The output minimum value.
		@param iorMax The output maximum value.
		@param iorAreInitialized Indicates whether iorMin and iorMax are initialized or not.
		*/
		virtual void GetMinMaxValue(EnumValueAxis iValueAxis, double& iorMin, double& iorMax, bool& iorAreInitialized) = 0;

		virtual void GetMinMaxValue(EnumValueAxis iValueAxis, double& iorMin, double& iorMax, bool& iorAreInitialized, Int32 iValueAxisDirection); //For Graphmatrix

		/**
		Add a value axis to the category plot.
		There may be one or two axis for a categtory plot when it is dual axis or bipolar chart.
		For chart areas there may be even more.
		@param iValueAxisPtr The input pointer to value axis. 
		*/
		virtual void AddValueAxis(ValueAxis::Ptr iValueAxisPtr) = 0;

		/**
		Set the category axis for the plot
		There can be only one category axis for a category plot currently.
		@param iCategoryAxisPtr The input pointer to category axis. 
		*/
		virtual void SetCategoryAxis(CategoryAxis::Ptr iCategoryAxisPtr) = 0;
        
        /**
         Reserve area for data labels.
         @param iorRect Shrink iorRect when necessary so that the data labels will not be cut at the boundary of the graph.
         */
        virtual void ReserveAreaForDataLabels(Rect2D& iorRect);
	};

}

#endif
