//
//  GridBasePlot.h
//  IPhoneChart
//
//  Created by dong shi on 3/26/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef IPhoneChart_GridBasePlot_h
#define IPhoneChart_GridBasePlot_h
#include "Plot.h"
#include "CategoryAxis.h"

namespace MsiChart
{
    class GridBasePlot: public Plot
    {
    public:
        typedef MSynch::SmartPtr<GridBasePlot> Ptr;
        GridBasePlot(const TripleId& irTripleId, GraphObjectManager* ipManager, const Rect2D& irDataArea);
        
 		virtual void AssignSeries(const std::vector<Int32>& irSeries);
		virtual void AssignAxis(CategoryAxis::Ptr iPtr, bool iIsY);
		virtual void AssignDataArea(const Rect2D& irDataArea, bool iIsToY);

    protected:
        bool mHasXAxis;		 //Has X axis or not, for some case we may only have one axis	
		bool mHasYAxis;		 //Has Y axis or not, for some case we may only have one axis 	
		unsigned char mMarkerShape;

        CategoryAxis::Ptr mXAxis;
		CategoryAxis::Ptr mYAxis;
        
        Rect2D mDataArea;
        
        double hGetMaxMarkerSizeInDevice();
        double hGetMaxRadiusInDeviceForPie();

    };
}



#endif
