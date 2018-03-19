/*
 *  GridChartPlot.h
 *  IPhoneChart
 *
 *  Created by dong shi on 12/3/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef GRIDPLOT_H
#define GRIDPLOT_H

#include "GridBasePlot.h"
#include "CategoryAxis.h"
#include "MarkerObject.h"
#include "PiePlotRenderer.h"

namespace MsiChart
{	
    
    struct GridDataLabelInfo
    {
        bool mIsDisPlayed;  //<Whether this label is already been located
        bool mIsValid;      //Is Host marker valid ?
        
        Int32 mSeriesId;
        Int32 mGroupId;
        
        int mXIndex;
        int mYIndex;
        
        Rect2D mHostMarkerBound;
    
        TextObject::Ptr mLabelPtr;
        
        GridDataLabelInfo(): mSeriesId(0),mGroupId(0), mIsDisPlayed(false), mIsValid(false)
        {}
        
        Rect2D ToRectangle()
        {
            Rect2D lRect;
            mLabelPtr->GetBoundingRect(lRect);
            return lRect;
        }
    };
    
    typedef std::vector<GridDataLabelInfo> GridDataLables;
       
    
	class GridPlot : public GridBasePlot {
	public:
		
		typedef MSynch::SmartPtr<GridPlot> Ptr;
		
		GridPlot(const TripleId& irTripleId, GraphObjectManager* ipManager, const Rect2D& irPlotArea);
		virtual ~GridPlot();
		
		//Re-implemention the virtual functions inherited from GraphObjectManager
		virtual GraphObject::Ptr OnPointDetection(const Point2D& irCursor) const; 
		virtual void OnRectangleSelection(const Rect2D& irRect, std::vector<GraphObject::Ptr>& iorSelectedObjects) const;
		virtual void Draw();
		virtual void GenerateImageMap(ImageMap& iorImageMap); 
		virtual void GenerateMapAndList(); 	
		virtual void GetOptions(Int32 iSeriesId, Int32 iPropertyId, CComVariant* opValue) const;
		virtual Int32 GetMaxElementRadius(); 	

		virtual void AssignSeries(const std::vector<Int32>& irSeries);

		static const double msMinVisibleRadius;

	protected:
		
		void hLoadProperties();
		double hGetSize(Int32 iX, Int32 iY);
        double hGetGMSizeValue(Int32 iX, Int32 iY);
        std::vector<GraphObject::Ptr > hGetMarkersForOneDataPoint(Int32 iX, Int32 iY);
        bool hNeedToDrawDataPoint(Int32 iX, Int32 iY);
        Point2D hGetPieCenter(Int32 iGroupId, Int32 iSeriesId, Int32 iAngleIndex);
        bool hGetGMPiePositon(Point2D& orCood, double iXInput, double iYInput, Int32 iXSubIndex, Int32 iYSubIndex, Int32 iXSectionCount, Int32 iYSectionCount);
		RGBColor hGetColor(Int32 iX, Int32 iY);	
		Point2D hGetPosition(Int32 iX, Int32 iY);
		Rect2D hGetMarkerRect(Int32 iX, Int32 iY);
		
		//void hShrinkMarker(MarkerObject::Ptr iMarkerPtr);
		void hGetArity();
		double hGetValueAt(Int32 iX, Int32 iY, Int32 iOffset = 0);
		const double* hGetValueAddress(Int32 iX, Int32 iY, Int32 iOffset = 0);
		bool hIsEmptyCell(Int32 iX, Int32 iY, Int32 iOffset = 0);		//Whether one cell is empty cell in template.
		Int32 hGetColorInGradientRange(double iIndex, Int32 iStartColor, Int32 iEndColor);
	
		void hPopulateMarkerCollections();
		GraphCollectionObject* hGetCollectionAsParentFor(Int32 iX, Int32 iY);
		void hAddLegnedMarkerToCollectionFor(Int32 iX, Int32 iY, GraphCollectionObject* ipCollection);
		
		Rect2D hGetAdjustAutoFitRect(bool iIsInit); //To Support case when max size is specified.
		Int32 hGetRectHLPosition(bool iIsForX, bool iIsForHighEnd, Int32 iIndex, const Point2D& irCenter);
        
        
        //------------Data Labels Method--------------//
        void hInitDataLabels();
        void hPlaceDataLableWithoutOverlap();
        void hPutDataLabel(int iIndex);
        int hGetDataLabelIndexInVec(int iX,int iY);
        TextObject::Ptr hCreateDataLabelForGraphMatrix(int iX, int iY);
        void hValueToFormattedText(const double &irValue, MBase::String& orText, Int32 iSeriesId, Int32 iGroupId, GraphCollectionObject *ipCollection);
        Rect2D hGetMarkerBoundingRect(Int32 iX, Int32 iY);
        Rect2D hAddPaddingBoundToDataLabel(Rect2D & iTextRect);
        bool hIsFitPaddingToBorder(const Rect2D& iTextRect);


	private:

		Int32 mXArity;
		Int32 mYArity;
		Int32 mColorIndex;	//< index of the color metric
		Int32 mSizeIndex;	//< index of the size metric
		Int32 mDataGroup;	//< metric number, actually
		
		bool mIsSeriesByCol;
		bool mIsAutoFitMode; //In scatter mode, each marker varies in size.
		bool mIsColorUniform;//By x, By y and ByBoth	
		bool mIsColorBy;	 //ColorBymode.	
        
        Int32 mDataLabelTextMode;

		double mMaxDataVal;
		double mMinDataVal;
		
		double mMaxColorVal;
		double mMinColorVal;
		
		double mMaxMarkerSize;
		double mMinMarkerSize;
		
		Int32 mPreXCoord;		//To compute the boundary of rectangle markers.																
		Int32 mPreYCoord;																										   
		Int32 mUniformColorMode; //by X ? by Y ? uniform for all?	
		
		RGBColor mUniformColor; //used for uniform color case
		RGBColor mPeakColor;	//used for non-uniform case, upper bound.
		RGBColor mLowColor;		//used for non-uniform case, lower bound.
		
		std::vector<Int32> mSeriesSet;
		std::vector<RGBColor> mColorsAlongAxis; //For color by row by col.
		
		std::vector<GraphCollectionObject::Ptr> mMarkerCollectionSet; //This is only used when color is by x or by y.
		
		Rect2D mAutoFitMarkerRect;
		Rect2D mAdjustMarkerRect; //used for rectangel marker when max size is specified.
        
        GridDataLables mDataLabelsVec;
        std::map<Int32, Int32> mLabelPositionFinder;               //data label index -> data label position in mDataLabelsVec
        
        PiePlotRenderer::Ptr mPieRender;
        std::map<TripleId, Point2D> mPieCentersPool;
	};
    
    inline bool CompareOfDateLabelInfo(const GridDataLabelInfo& first, const GridDataLabelInfo& second)
    {
        if(first.mYIndex == second.mYIndex)
            return first.mXIndex < second.mXIndex;
        return first.mYIndex < second.mYIndex;
    }

}

#endif