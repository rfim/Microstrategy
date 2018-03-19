//==============================================================================================
//	FILENAME	:	MarkerObject.h
//	AUTHOR		:	Xin Liu
//	CREATION	:	5/22/08
//	Copyright (C) MicroStrategy, Inc. 2008
//==============================================================================================

#ifndef MSICHART_MARKEROBJECT_H
#define	MSICHART_MARKEROBJECT_H

#include "ShapeObject.h"

#ifdef _VE_CE
#else
#import "MarkerObjectHelper.h"
#endif

namespace MsiChart
{
	const Int32 gDefaultMarkerSize = 31;	///< The default dialog marker size.
	const Int32 gLocalMarkerSize = 100;		///< The size of the local area.
//	const Int32 gMaxMarkerSize = 200;		

	/**
	Indicates the marker shape.
	*/
	enum MarkerShape
	{
		MS_Rectangle, 
		MS_XShape, 
		MS_PlusSymbolthin, 
		MS_PlusSymbolthick, 
		MS_Circle, 
		MS_Diamond, 
		MS_Triangle, 
		MS_Square, 
		MS_Star,
		MS_Area,
		MS_Star45
	};

	/** This class delegates all kinds of markers.
	MarkerObject prevails in the chart, such as the markers in the line chart, the legend markers, etc.
	*/
	class MarkerObject: public ShapeObject
	{
	public:
		typedef MSynch::SmartPtr<MarkerObject> Ptr;

		/**Constructor
		@param iMarkerPosition The marker position is defined as the coordinate of the center point of the marker.
		@param iMarkerShape The marker shape. The default shape is rectangle.
		@param iIsIsometric Indicates whether the bounding rectangle of the marker keeps a squre shape or changes according to the image size.
		@param iMarkerSize The marker size.
		@param iIsDialogSize Indicates the unit of the input iMarkerSize. We have two kind of marker size. One is called dialog size, which is the value shown
		in the GUI dialog to the client, and the other one is the size in image pixel.
		*/
		MarkerObject(const TripleId& irTripleId, GraphObjectManager* ipManager, const Point2D& iMarkerPosition, 
			unsigned char iMarkerShape = DssGraphMarkerShapeRectangle, bool iIsIsometric = true, 
			Int32 iMarkerSize = gDefaultMarkerSize, GraphCollectionObject* ipParentObject = NULL, bool iIsDialogSize = true);
		virtual ~MarkerObject();
		
		//inherite from GraphObject class
		virtual void Draw() const;
		virtual void GenerateView();
        virtual void DrawInnerOutLine();
		virtual void DrawOutLine();
		virtual bool PointInObject(const Point2D& irPoint);
		virtual bool ObjectInRectangle(const Rect2D& irRect);
		virtual void GetOptions(Int32 iPropertyId, CComVariant* opValue) const;
		virtual void GenerateImageMap(ImageMap& iorImageMap);

		//inherite from ShapeObject class
		virtual void GetBoundingRect(Rect2D& orBoundingRect) const;
		virtual bool SupportBevelEffect() const;

		/**
		 * prepare the next frame to draw
		 */
		virtual void SetProgress(double iProgress);
		
		virtual float GetMemoryUsageEstimation() const;
		
		/**
		This function returns the marker size in pixel.
		@return The marker size in pixel.
		*/
		Int32 GetMarkerSize() const;

		/**
		This function set the marker size in pixel.
		@param iMarkerSize The input marker size.
		*/
		void SetMarkerSize(Int32 iMarkerSize);

		/**
		This function returns the marker position.
		@return The marker position.
		*/
		const Point2D& GetMarkerPosition() const;
        void SetMarkerPosition(Point2D iMarkerPosition);
		/**
		This function sets the marker shape
		*/
		void SetMakerShape(unsigned char iMarkerShape);
        void GetMarkerShape(unsigned char &orMarkerShape);
        
		/**
		This function sets whether the marker is shown or not
		*/
		void SetIsShown(bool iIsShown);
        bool IsShown();
        float GetPreviousScale();
        void GetPolygon(std::vector<Point2D> &orPolygon);
        void SetPolygon(std::vector<Point2D> iPolygon);
        /**
         This function calculates the polygon that defines the marker object.
         */
		void GetClockwisePolygon();

        void DoDraw() const;
        /**
         This function sets the cairo path for the marker.
         @param iIsLinePath Indicates whether current path is a line path or fill path.
         */
		void SetMarkerPath(bool iIsLinePath) const;
        void ApplyOutlineFormat() const;
#ifndef _VE_CE
        CGGradientRef CreateGlowGradient() const;
		/** For series animation.
		 @param ipCGContext Specifies a core graphics context.
		 @param irRect Speicfies drawing area.
		 */
		void DrawWithContext(CGContextRef ipCGContext, const CGRect& irRect);
#endif
	protected:
		//inherite from ShapeObject class
		virtual void GetClockwisePolygon(std::vector<Point2D>& orPolygon) const;
		virtual bool IsCircularShape() const;
		virtual void FillBevel() const;

	private:
		/**
		This function gets a list of points that defines a marker shape in local coordinate.
		@param iMarkerShape The marker shape.
		@param opPointNumber The number of points.
		@param opPoints Ths list of points.
		*/
		void hGetMarkerShape(unsigned char iMarkerShape, Int32* opPointNumber, const Int32** opPoints) const;

		/**
		This function converts the pixel size to dialog size.
		@param iMarkerSize The input pixel size.
		*/
		Int32 hConvertToDialogSize(Int32 iMarkerSize) const;

		/**
		This function converts the dialog size to pixel size.
		@param iDialogSize The input dialog size.
		*/
		Int32 hConvertToMarkerSize(Int32 iDialogSize) const;
        
        bool hSetMarkerInnerPath(bool iIsLinePath) const;

		/**
		This fucntion gets the local marker shape according to the marker type defined in CDSSGraphStype.idl
		*/
		unsigned char hConvertToLocalMarkerShape(EnumDSSGraphMarkerShape iGraphMarkerShape);

		Int32 mMarkerSize;				///< The marker size in pixel.
		Int32 mFinalMarkerSize;
		unsigned char mMarkerShape;		///< The marker shape.
		Point2D mMarkerPosition;		///< The marker position, which is the coordinate of the center point of the marker.
		Point2D mFinalMarkerPosition;
		bool mIsIsometric;				///< Indicates whether the bounding rectangle of the marker keeps a squre shape or changes according to the image size.
		std::vector<Point2D> mPolygon;	///< The polygon that defines the marker object.
        std::vector<Point<float> > mFloatPolygon;	///< The polygon that defines the marker object with float cooridates to be precise.
		bool mIsShown;					///< Indicates the marker object is shown or not.
        mutable float mPreviousScale;   ///< For Zooming, previous scale
#ifdef _VE_CE
#else
        MarkerObjectHelper *mpMarkerObjectHelper;
#endif
	};
    inline void MarkerObject::GetMarkerShape(unsigned char &orMarkerShape)
    {
        orMarkerShape = mMarkerShape;
    }
    inline void MarkerObject::GetPolygon(std::vector<Point2D> &orPolygon)
    {
        orPolygon = mPolygon;
    }
    inline void MarkerObject::SetPolygon(std::vector<Point2D> iPolygon)
    {
        mPolygon = iPolygon;
    }
    inline float MarkerObject::GetPreviousScale()
    {
        return mPreviousScale;
    }
    inline bool MarkerObject::IsShown()
    {
        return mIsShown;
    }
	inline Int32 MarkerObject::hConvertToDialogSize(Int32 iMarkerSize) const
	{
		return iMarkerSize * 2000 / std::min(mChartContextPtr->GetGraphHeight(), mChartContextPtr->GetGraphWidth());
	}

	inline Int32 MarkerObject::hConvertToMarkerSize(Int32 iDialogSize) const
	{
		return mChartContextPtr->DialogSizeToPixelSize(iDialogSize);
	}

	inline Int32 MarkerObject::GetMarkerSize() const
	{
		return mMarkerSize;
	}

	inline const Point2D& MarkerObject::GetMarkerPosition() const
	{
		return mMarkerPosition;
	}
    inline void MarkerObject::SetMarkerPosition(Point2D iMarkerPosition)
    {
        mMarkerPosition = iMarkerPosition;
    }
    
	inline void MarkerObject::SetIsShown(bool iIsShown)
	{
		mIsShown = iIsShown;
	}
#ifndef _VE_CE
    inline CGGradientRef MarkerObject::CreateGlowGradient() const
    {
        return ShapeObject::CreateGlowGradient();
    }
#endif
    inline void MarkerObject::ApplyOutlineFormat() const
    {
        GraphObject::ApplyOutlineFormat();
    }
}

#endif
