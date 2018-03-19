
namespace MsiChart
{
    class RectangleObject;
    
	class RectangleObjectHelper
	{
	public:
		RectangleObjectHelper(RectangleObject *ipRectangleObject,ChartContext::Ptr iChartContextPtr);
        
		virtual ~RectangleObjectHelper();
		void DrawWithContext(CGContextRef ipCGContext, const CGRect& irRect);
        void GenerateView();
        CGRect hPickStartAnimationFrame(void* ipBarView) const;
        
	protected:
        RectangleObject *mpRectangleObject;
        ChartContext::Ptr mChartContextPtr;
	};
}


