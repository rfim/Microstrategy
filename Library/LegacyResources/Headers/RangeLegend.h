/*
 *  RangeLegend.h
 *  IPhoneChart
 *
 *  Created by dong shi on 12/11/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef MSICHART_RANGELEGEND_H
#define	MSICHART_RANGELEGEND_H

#include "Legend.h"
#include "ValueAxis.h" 
#include "RectangleObject.h"
namespace MsiChart
{
	
	struct RangeLengdOptions
	{
		double mMaxAxisVal;
		double mMinAxisVal;
		bool mIsHorizontal;
		bool mIsAxisOnStrongSide; //Strong means : left(when legend is vertical) or top(when legend is horizontal).	
		GraphFillGradient mFillOptions;
		
		RangeLengdOptions()
		{
			mMaxAxisVal = 50000.0;
			mMinAxisVal = 0.0;
			mIsHorizontal = true;
			mIsAxisOnStrongSide = true;
		}
	};
	
	
	class RangeLegend: public Legend
	{
		
	public:
		typedef MSynch::SmartPtr<RangeLegend> Ptr;
		
		RangeLegend(const TripleId& irTripleId, GraphObjectManager* ipManager, Dataset::Ptr iDatasetPtr, Rect2D iArea);
		~RangeLegend();
		
		virtual void Draw();
		virtual void GenerateView();
		virtual void GenerateMapAndList();
		virtual void GetOptions(Int32 iPropertyId, CComVariant* opValue) const;
		virtual void GetOptions(Int32 iSeriesId, Int32 iPropertyId, CComVariant* opValue) const;
	
		
		
		
		
	private:
		/**
		 This function loads all the properties related to Legend.
		 */
		void hLoadProperties();

	private:	
		RangeLengdOptions mOptions;
		ValueAxis::Ptr mAxisPtr;
		
		bool hIsValid();		
		bool hIsVertical();
		Rect2D hLayout(Rect2D& iorRect);
		ValueAxis::Ptr hGetAxis(Rect2D& iorRect, Rect2D& iorRectForColorPanel);
		RectangleObject::Ptr hGetColorPanel(const Rect2D& irRect);
	};
	
}

#endif

