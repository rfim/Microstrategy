//==============================================================================================
//	FILENAME	:	RectangleObject.h
//	AUTHOR		:	Fangyao Ma
//	CREATION	:	2008-05-21
//	Copyright (C) MicroStrategy Incorporated 2008
//	All Rights Reserved
//==============================================================================================

#ifndef MsiChart_RectangleObject_h
#define MsiChart_RectangleObject_h 1

#include "ShapeObject.h"

#ifdef _VE_CE
#else
#include "RectangleObjectHelper.h"
#endif

#include "../Common.h"

namespace MsiChart
{
    typedef enum
    {
        EASP_CENTER = 0,
        EASP_TOP,
        EASP_BOTTOM,
        EASP_LEFT,
        EASP_RIGH,
    }EnumAnimationStartPositon;
    
    typedef enum
    {
        BAR_OPEN_NONE = -1,
        BAR_OPEN_TOP = 0,
        BAR_OPEN_BOTTOM,
        BAR_OPEN_LEFT,
        BAR_OPEN_RIGHT,
        BAR_OPEN_UNDEF,
    }EnumBarBorderOpenType;
    
	/**
	 *	Rectangle area in the graph, such as Bar, BackGround, etc.
	 */
	class RectangleObject: public ShapeObject
	{
	public:
		typedef MSynch::SmartPtr<RectangleObject> Ptr;

		/**
		 *	Constructor
		 *	@param irTripleId	TripleId
		 *	@param ipManager	Manager who create the object
		 *	@param irRect	Rectangle area occupied
		 *	@param ipParentObject	Parent Object
		 *  @param iIsOnlyForToolTip Indicate whether current RectangleObject is only used for tool tip.
		 */
		RectangleObject(const TripleId& irTripleId, 					
						GraphObjectManager* ipManager,		
						const Rect2D& irRect,
						GraphCollectionObject* ipParentObject = NULL,
						bool iIsOnlyForToolTip = false, bool iIsVirtualMarker = false, bool iIsVertical = true, double iDataPointXPos = 0.5, double iDataPointYPos = 0.5);

		virtual ~RectangleObject();

		/**
		 *	Set the destined rectangle area
		 *	@param irRect	input rectangle area size
		 */
	//	void SetRect(const Rect2D& irRect);

		/**
		 *	Get the destined rectangle area
		 *	@param orRect	output rectangle area size
		 */
	//	void GetRect(Rect2D& orRect);
		
		//virtual functions from GraphObject
		virtual void Draw() const;
		virtual void GenerateView();
        virtual void DrawInnerOutLine();   //as per spec, we need to draw 1 px inner stroke when hovered.
		virtual void DrawOutLine();
		virtual bool PointInObject(const Point2D& irPoint);
		virtual bool ObjectInRectangle(const Rect2D& irRect);
		virtual void GenerateImageMap(ImageMap& iorImageMap);
		virtual bool IsVirtual() const;

		///	Get the bounding rectangle area of the rect
		virtual void GetBoundingRect(Rect2D& orBoundingRect) const;
#ifndef _VE_CE		
		void DrawWithContext(CGContextRef ipCGContext, const CGRect& irRect);
#endif
		
		/** Set the position for data point.
		 @param iX ranges from 0 to 1, indicating the horizontal place of the data point, where iX = 0 means the data point is at the left of the object, and iX = 1 means the right.
		 @param iY ranges from 0 to 1, indicating the vertical place of the data point, where iY = 0 means the data point is at the top of the object, and iX = 1 means the bottom.
		 */
		virtual void SetDataPointPosition(double iX, double iY);
		
		virtual Point2D GetDataPoint();

		//virtual void SetProgress(double iProgress);
		virtual float GetMemoryUsageEstimation() const;
        
        //this is used only in bar chart, to decide if the bottom/left side is open for bar riser in vertical/horizontal chart.
        virtual void SetOpen(Int32 iOpenType);

        void SetAnimationStartPositon(EnumAnimationStartPositon iStart);
        
        Int32 GetSeriesInfoIndex();
        void SetSeriesInfoIndex(Int32 iSeriesInfo);
 
        bool IsOnlyForToolTip();
        bool IsAnimationEnabled();
        Rect2D GetRect();
        FormatLine::Ptr GetFormatLinePtr();
        EnumAnimationStartPositon GetStartPosition();
        float GetPreviousScale();
        
        //change protect to public
        virtual void DoDraw() const;
        virtual void SetRect(const Rect2D& irRect);
        
	protected:
		///	Get the clockwise polygon that surrounding the object
		virtual void GetClockwisePolygon(std::vector<Point2D>& orPolygon) const;
		
		//virtual void SetRect(const Rect2D& irRect);
#ifndef _VE_CE
        CGRect hPickStartAnimationFrame(void* ipBarView); //const;
#endif
        
        virtual void hGetBorder(Polygon2D& orBorder) const;

		Rect2D	mRect;		///< Rectangle area occupied
		Rect2D  mFinalRect;
        Int32 mSeriesInfo; //SeriesInfoIndex of line virtual marker!

		bool mIsOnlyForToolTip;	///< Indicate whether current RectangleObject is only used for tool tip.
        Int32 mBorderOpenType;  ///< Indicate the border open type RectangleObject

		bool mIsVirtualMarker;
		bool mIsVertical;
        
		double mDataPointXPos;
		double mDataPointYPos;
		
        EnumAnimationStartPositon mStartPostion;
        
        mutable float mPreviousScale;
#ifdef _VE_CE
#else
    private:
        RectangleObjectHelper * mpRectangleObjectHelper;
#endif
        

	};

	inline bool RectangleObject::IsVirtual() const
	{
		return mIsVirtualMarker;
	}
    
    inline Int32 RectangleObject::GetSeriesInfoIndex()
    {
        return mSeriesInfo;
    }
    
    inline void RectangleObject::SetSeriesInfoIndex(Int32 iSieresInfo)
    {
        mSeriesInfo = iSieresInfo;
    }
    //=================================================
    //method added for code seperation
    inline bool RectangleObject::IsOnlyForToolTip()
    {
        return mIsOnlyForToolTip;
    }
    inline bool RectangleObject::IsAnimationEnabled()
    {
        return mIsAnimationEnabled;
    }
    inline Rect2D RectangleObject::GetRect()
    {
        return mRect;
    }
    inline FormatLine::Ptr RectangleObject::GetFormatLinePtr()
    {
        return mFormatLinePtr;
    }
    inline EnumAnimationStartPositon RectangleObject::GetStartPosition()
    {
        return mStartPostion;
    }
    inline float RectangleObject::GetPreviousScale()
    {
        return mPreviousScale;
    }
}


#endif
