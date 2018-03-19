#ifndef Msi_ChartContextWINHelper
#define Msi_ChartContextWINHelper

namespace MsiChart {
    class ChartContextWINHelper: public ChartContextHelper{
    public:
        ChartContextWINHelper(ChartContext* ipChartContext):ChartContextHelper(ipChartContext){};
        ~ChartContextWINHelper(){};
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
    };
}

#endif