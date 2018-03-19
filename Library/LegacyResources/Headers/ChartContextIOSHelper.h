//
//  ChartContextIOSHelper.h
//  IPhoneChart
//
//  Created by Jin, Dong on 24/9/14.
//
//

#ifndef IPhoneChart_ChartContextIOSHelper_h
#define IPhoneChart_ChartContextIOSHelper_h

#import "ChartContextHelper.h"
namespace MsiChart {
    
    class ChartContext;
    
    class ChartContextIOSHelper:public ChartContextHelper
    {
    public:
        ChartContextIOSHelper(ChartContext* ipChartContext):ChartContextHelper(ipChartContext){};
        virtual ~ChartContextIOSHelper(){};
        void RefreshGraphMatrixContext();
        void VDFontSize(Int32& iorSize) const;
        bool GetCurrentGraphFrame(Rect2D& iorRect) const;
        bool IsBackgroundBright(bool iIsRecursive) const;
        void AnimationDidFinished();
        void UpdateZoomScale(void* ipView);
        void UpdateHighScaleFactorToViews();
        void UpdateHighScaleFactorToViews(void* ipView);
        double GetZoomScale(bool iIsHonest);
        bool IsSubView(void* ipChildView, void* ipBaseView, bool iIsRecursive);
        bool IsAnimationPossible(double iZoomScale);
        bool IsAnimationEnabled() const;
        bool IsBackgourndDark();
    };
}

#endif
