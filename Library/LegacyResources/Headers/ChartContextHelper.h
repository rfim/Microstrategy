#ifndef Msi_ChartContextHelper
#define Msi_ChartContextHelper

#include "Rect2D.h"

namespace MsiChart {

    class ChartContext;
    
    class ChartContextHelper{
    public:
        ChartContextHelper(ChartContext* ipChartContext):mpChartContext(ipChartContext){};
        virtual ~ChartContextHelper(){mpChartContext = NULL;};
        virtual void RefreshGraphMatrixContext();
        virtual void VDFontSize(Int32& iorSize) const;
        virtual bool GetCurrentGraphFrame(Rect2D& iorRect) const;
        virtual bool IsBackgroundBright(bool iIsRecursive) const;
        virtual void AnimationDidFinished();
        virtual void UpdateZoomScale(void* ipView);
        virtual void UpdateHighScaleFactorToViews();
        virtual void UpdateHighScaleFactorToViews(void* ipView);
        virtual double GetZoomScale(bool iIsHonest);
        virtual bool IsSubView(void* ipChildView, void* ipBaseView, bool iIsRecursive);
        virtual bool IsAnimationPossible(double iZoomScale);
        virtual bool IsAnimationEnabled() const;
    protected:
        ChartContext*   mpChartContext;
    };
}

#endif