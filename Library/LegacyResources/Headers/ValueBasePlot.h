//
//  ValueBasePlot.h
//  IPhoneChart
//
//  Created by dong shi on 3/26/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef IPhoneChart_ValueBasePlot_h
#define IPhoneChart_ValueBasePlot_h
#include "Plot.h"
#include "ValueAxis.h"

namespace MsiChart
{
    class ValueBasePlot: public Plot
    {
    public:
        typedef MSynch::SmartPtr<ValueBasePlot> Ptr;
        ValueBasePlot(const TripleId& irTripleId, GraphObjectManager* ipManager, const Rect2D& irDataArea);
        
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

    };
}

#endif
