//
//  CategoryPiePlot.h
//  IPhoneChart
//
//  Created by dong shi on 2/27/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef IPhoneChart_CategoryPiePlot_h
#define IPhoneChart_CategoryPiePlot_h

#include "ValueAxis.h"
#include "CategoryAxis.h"
#include "CategoryPlot.h"
#include "PiePlotRenderer.h"

using namespace MsiChart;

class CategoryPiePlot : public CategoryPlot
{
public:
    typedef MSynch::SmartPtr<CategoryPiePlot> Ptr;
    
    CategoryPiePlot(const TripleId& irTripleId, GraphObjectManager* ipManager, const Rect2D& irDataArea);
    ~CategoryPiePlot();
    
    //pure virtual functions from GraphObjectManager
    virtual void GenerateMapAndList();
    virtual void GetOptions(Int32 iSeriesId, Int32 iPropertyId, CComVariant* opValue) const;
    
    //override functions
    virtual void Draw();
    virtual GraphObject::Ptr OnPointDetection(const Point2D& irCursor) const; 
    virtual void OnRectangleSelection(const Rect2D& irRect, std::vector<GraphObject::Ptr>& iorSelectedObjects) const;
    virtual void GenerateImageMap(ImageMap& iorImageMap); 
    //virtual void DrawTrendLinesAndEquations();
    
    /**
     Add a value axis to the category plot.
     There may be one or two axis for a categtory plot when it is dual axis or bipolar chart.
     For chart areas there may be even more.
     @param iValueAxisPtr The input pointer to value axis. 
     */
    virtual void AddValueAxis(ValueAxis::Ptr iValueAxisPtr);
    
    /**
     Set the category axis for the plot
     There can be only one category axis for a category plot currently.
     @param iCategoryAxisPtr The input pointer to category axis. 
     */
    virtual void SetCategoryAxis(CategoryAxis::Ptr iCategoryAxisPtr);
    
    virtual void Init();
    
    virtual void GetMinMaxValue(EnumValueAxis iValueAxis, double& iorMin, double& iorMax, bool& iorAreInitialized);
	virtual void GetMinMaxValue(EnumValueAxis iValueAxis, double& iorMin, double& iorMax, bool& iorAreInitialized, Int32 iValueAxisDirection);

    virtual bool GetGMPiePositon(Point2D& orCood, double iXInput, double iYInput, Int32 iXSubIndex = 0, Int32 iYSubIndex = 0, Int32 iXSectionCount = 1, Int32 iYSectionCount = 1, Int32 iRadius = 2);
    
    virtual void SetGroupCountForGMPie(Int32 iGroupCount);

    
    //override from GraphObjectManager
    virtual GraphObject::Ptr OnPointDetectionEx(const Point2D& irCursor, 
                                                const std::vector<Int32>& irObjectIds, Int32& iorDistance, Int32 iRadius,
                                                std::vector<GraphObject::Ptr>& iorGraphObjectsAround, EnumGraphObjectUsage iUsage = GOU_NORMAL_HIGHLIGHT); 
    
private:    
    bool mIsVertical;
    PiePlotRenderer::Ptr mPiePlotRenderer;
    CategoryAxis::Ptr mCategoryAxisPtr;
    ValueAxis::Ptr mValueAxisPtr;

};



#endif
