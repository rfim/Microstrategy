//
//  ValuePiePlot.h
//  IPhoneChart
//
//  Created by dong shi on 3/26/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef IPhoneChart_ValuePiePlot_h
#define IPhoneChart_ValuePiePlot_h

#include "ValueAxis.h"
#include "ValueBasePlot.h"
#include "PiePlotRenderer.h"

using namespace MsiChart;

namespace MsiChart
{
    class ValuePiePlot : public ValueBasePlot
    {
    public:
        typedef MSynch::SmartPtr<ValuePiePlot> Ptr;
        
        ValuePiePlot(const TripleId& irTripleId, GraphObjectManager* ipManager, const Rect2D& irDataArea);
        virtual ~ValuePiePlot();
        
        //pure virtual functions from GraphObjectManager
        virtual void GenerateMapAndList();
        virtual void GetOptions(Int32 iSeriesId, Int32 iPropertyId, CComVariant* opValue) const;
        
        //override functions
        virtual void Draw();
        virtual GraphObject::Ptr OnPointDetection(const Point2D& irCursor) const; 
        virtual void OnRectangleSelection(const Rect2D& irRect, std::vector<GraphObject::Ptr>& iorSelectedObjects) const;
        virtual void GenerateImageMap(ImageMap& iorImageMap); 
        //virtual void DrawTrendLinesAndEquations();
        
        
        virtual bool GetGMPiePositon(Point2D& orCood, double iXInput, double iYInput, Int32 iXSubIndex = 0, Int32 iYSubIndex = 0, Int32 iXSectionCount = 1, Int32 iYSectionCount = 1, Int32 iRadius = 2);
        
        
        //override from GraphObjectManager
        virtual GraphObject::Ptr OnPointDetectionEx(const Point2D& irCursor, 
                                                    const std::vector<Int32>& irObjectIds, Int32& iorDistance, Int32 iRadius,
                                                    std::vector<GraphObject::Ptr>& iorGraphObjectsAround, EnumGraphObjectUsage iUsage = GOU_NORMAL_HIGHLIGHT); 
        
        //assign the input ValueAxis pointer to proper data memebers 
        /**        
         @param iValueAxisPtr, the input value axis pointer
         @param iIndex, the input index for that value axis pointer.
         */
        virtual void AssignValueAxis(ValueAxis::Ptr iValueAxisPtr, Int32 iIndex);
        
        //Populate the series information for the hosting ValuePlot object, called by ValuePlot manager
        /**        
         @param irSeriesIndex, indicate the index(their series ids)of the series held by this plot object 
         @param irVecSeriesVAIndex, indicate the managing VA index for the series held by this plot object
         */
        virtual void AssignSeriesInfo(const std::vector<Int32>& irSeriesIndex,
                              const std::vector<Int32>& irVecSeriesVAIndex);
        
        virtual void AssignGroupsInfo(const std::vector<Int32>& irGroupIndex,
                                      const std::vector<Int32>& irVecGroupVAIndex);
        
        
        //Graph Matrix Padding Required.    
        virtual Int32 GetMaxElementRadius();	
        
        //Get the radius/offset that is clipped by chart boundary
        /**
         @param iIsOnXAxis, is the offset is the result clipped by X or Y axis.
         @param orLowOffset, result on the left or bottom end of X/Y axis.
         @param orHighOffset, result on the right or top end of X/Y axis.
         */
        //virtual bool GetMaxElementOffsetClippedByBoundary(bool iIsOnXAxis, double& orLowOffset, double& orHighOffset);


    private:    
        PiePlotRenderer::Ptr mPiePlotRenderer;
        ValueAxis::Ptr mXAxisPtr;	///< X-Axis ptr
        ValueAxis::Ptr mY1AxisPtr;	///< Y1-Axis ptr
        ValueAxis::Ptr mY2AxisPtr;	///< Y2-Axis ptr
        ValueAxis::Ptr mZAxisPtr;	///< Z-Axis ptr
        std::vector<std::pair<Int32, Int32> > mVecSeriesInfo; ///< the vector of series information

        
    };
}




#endif
