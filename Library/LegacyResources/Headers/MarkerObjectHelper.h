#ifndef MsiChart_MarkerObjectHelper_h
#define MsiChart_MarkerObjectHelper_h


namespace MsiChart{
    
    class MarkerObject;
    class ChartContext;
    
    class MarkerObjectHelper
    {
    public:
        MarkerObjectHelper():mpMarkerObject(NULL),mChartContextPtr(NULL){};
        MarkerObjectHelper(MarkerObject *ipMarkerObject,ChartContext *iChartContextPtr);
        virtual ~MarkerObjectHelper();
        void GenerateView();
        void DrawWithContext(CGContextRef ipCGContext, const CGRect& irRect);
        void DrawOutLine();
    protected:
        MarkerObject *mpMarkerObject;
        ChartContext *mChartContextPtr;
    };
}
#endif