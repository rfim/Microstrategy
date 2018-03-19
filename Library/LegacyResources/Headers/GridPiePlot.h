//
//  GridPiePlot.h
//  IPhoneChart
//
//  Created by dong shi on 3/26/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef IPhoneChart_GridPiePlot_h
#define IPhoneChart_GridPiePlot_h

#include "GridBasePlot.h"
#include "PiePlotRenderer.h"
#include "CategoryAxis.h"

namespace MsiChart
{	
	class GridPiePlot : public GridBasePlot {
	public:
		
		typedef MSynch::SmartPtr<GridPiePlot> Ptr;
		
		GridPiePlot(const TripleId& irTripleId, GraphObjectManager* ipManager, const Rect2D& irPlotArea);
		virtual ~GridPiePlot();
		
		//Re-implemention the virtual functions inherited from GraphObjectManager
		virtual GraphObject::Ptr OnPointDetection(const Point2D& irCursor) const; 
		virtual void OnRectangleSelection(const Rect2D& irRect, std::vector<GraphObject::Ptr>& iorSelectedObjects) const;
		virtual void Draw();
		virtual void GenerateImageMap(ImageMap& iorImageMap); 
		virtual void GenerateMapAndList(); 	
		virtual void GetOptions(Int32 iSeriesId, Int32 iPropertyId, CComVariant* opValue) const;
		virtual Int32 GetMaxElementRadius(); 	
        
		virtual void AssignSeries(const std::vector<Int32>& irSeries);
        
	    virtual bool GetGMPiePositon(Point2D& orCood, double iXInput, double iYInput, Int32 iXSubIndex, Int32 iYSubIndex, Int32 iXSectionCount, Int32 iYSectionCount,Int32 iRadius);

        //override from GraphObjectManager
        virtual GraphObject::Ptr OnPointDetectionEx(const Point2D& irCursor, 
                                                    const std::vector<Int32>& irObjectIds, Int32& iorDistance, Int32 iRadius,
                                                    std::vector<GraphObject::Ptr>& iorGraphObjectsAround, EnumGraphObjectUsage iUsage = GOU_NORMAL_HIGHLIGHT); 
        
    private:
        PiePlotRenderer::Ptr mPiePlotRenderer;
        std::vector<Int32> mSeriesSet;
        double mMaxPieRadius;
    };
}

#endif
