//==============================================================================================
//	FILENAME	:	ShapeObject.h
//	AUTHOR		:	Liang Liu
//	CREATION	:	2008-05-21
//	Copyright (C) MicroStrategy Incorporated 2008
//	All Rights Reserved
//==============================================================================================
#ifndef MsiChart_ShapeObject_h
#define MsiChart_ShapeObject_h

#include "PDCHeader/PDCvector"
#include "GraphObject.h"
#include "FormatFill.h"

namespace MsiChart
{
	/**	Mainly deal with format fill operations, such as Gradient, Texture, Picture, Pattern and Bevel.
     This is the base class of RectangleObject class, PolygonObject class, etc.
     */
	class ShapeObject: public GraphObject
	{
	public:
		typedef MSynch::SmartPtr<ShapeObject> Ptr;
        
		/**	Constructor.
         @param irTripleId Required by base class.
         @param ipManager Required by base class.
         @param ipParentObject Required by base class.
         @param iHasFill Indicate whether current ShapeObject has FormatFill or not.
         @param iHasLine Indicate whether current ShapeObject has FormatLine or not.
         */
		ShapeObject(const TripleId& irTripleId, GraphObjectManager* ipManager, GraphCollectionObject* ipParentObject, bool iHasFill = true, bool iHasLine = true);
		virtual ~ShapeObject();
        
		/** This function sets a new FormatFill for current ShapeObject.
         @param iFormatFillPtr Point to a FormatFill object.
         */
		void SetFormatFill(FormatFill::Ptr iFormatFillPtr);
        
		/** This function get the FormatFill from current ShapeObject.
         @return Point to the FormatFill object.
         */
		FormatFill::Ptr GetFormatFill() const;
        
		// Inherite from GraphObject class.
		virtual void GetOptions(Int32 iPropertyId, CComVariant* opValue) const;
		
		/** Check whether Bevel effect is supported or not.
         @return Return true if Bevel effect is supported. Otherwise, return false.
         */
		virtual bool SupportBevelEffect() const;
        
		/**
         Check whether current ShapeObject is completely opaque or not.
         @return Return true if current ShapeObject is completely opaque. Otherwise, return false.
         */
		bool IsCompletelyOpaque() const;
        
		/// Clear the reference to related RectangleView.
		void ResetRectangleView();
        void ResetRectangleView(void * ipRectanlgeView);
        void* GetRectangleView();
        void SetRectangleView(void *iRectangleView);
        
		/** For grid chart rendering.
		 @param irColor Specifies color to be used rather than the default one.
		 */
		void UpdateMarkerColor(const RGBColor& irColor);
        
        bool IsRectanleViewReusable(double iPreviousZoomScale) const;
        
		/*
		 Check whether current rectangle object is opaque or not.
		 @return Return true is current rectangle object is opaque. Otherwise, return false.
		 */
		bool IsOpaque() const;
        
        /**
         * For some kind of bars, like the difference bar of lipstick, we don't apply threshold.
         */
        void SetAllowThreshold(bool iAllowThreshold);
        
        /// A pointer to FormatFill object.
        FormatFill::Ptr mFormatFillPtr;
        
	protected:
		/// This function fills current ShapeObject with the information in mFormatFill.
		virtual void ApplyFillFormat() const;
        
		/** This function fills current ShapeObject with Bevel effect which is one kind of fill effect.
         This method will be called by ApplyFillFormat().
         */
		virtual void FillBevel() const;
        
		/**
         check whether current object is a circular ShapeObject or not.
         @return Return true if current object is a circular ShapeObject. Otherwise, return false.
         */
		virtual bool IsCircularShape() const;
        
		/** Get a bounding rectangle from an existing polygon.
         @param irPolygon Input a polygon.
         @param orRect Output a rectangle.
         */
		void GetBoundingRectFromPolygon(const std::vector<Point2D>& irPolygon, Rect2D& orRect) const;
        
		/**
         Get translated polygon based on an input polygon and an offset.
         @param irPolygon Input polygon.
         @param irOffset Specifies an offset.
         @param Output a polygon which is got from translating irPolygon by irOffset.
         */
		void GetTranslatedPolygon(Polygon2D& irPolygon, const Point2D& irOffset, Polygon2D& orPolygon) const;
        
		/**
         Check whether threshold color is used for current ShapeObeject.
         @return Return true if threshold color is used for current ShapeObeject. Otherwise, return false.
         */
        //		bool UseThresholdColor() const;
        
		/**
         Check whether picture fill is used for current ShapeObeject.
         @return Return true if picture fill is used for current ShapeObeject. Otherwise, return false.
         */
		bool UsePictureFill() const;
        
		/**
         Add compensative layer for filling.
         This is useful when combining bevel sphere effect and gradient/pattern/picture/texture.
         Add this layer to make sure that the combine alpha value of non-simple layer and bevel-sphere layer is at least mAlpha.
         */
		virtual void AddCompensativeLayer() const;
		
		/**
		 Create a bitmap context. Caller is responsible to release both the context and the data.
		 The usage is like follows.
		 CGContextRef lCGContextRef = CreateARGBBitmapContext(20, 20);
		 void *lpData = CGBitmapContextGetData(lCGContextRef);
		 if (lpData != NULL)
		 {
		 // **** You have a pointer to the image data ****
		 // **** Do stuff with the data here ****
		 }
		 
		 // When finished, release the context
		 CGContextRelease(lCGContextRef);
		 // Free image data memory for the context
		 if (lpData)
		 {
         free(lpData);
		 }
		 @param iWidth,iHeight Specifies the size of the bitmap.
		 @return Return a CGContextRef if successful. Otherwise, return NULL.
		 */
#ifndef _VE_CE
		CGContextRef CreateARGBBitmapContext (int iWidth, int iHeight) const;
		
		/// Create a gradient for glow effect.
		CGGradientRef CreateGlowGradient() const;
#endif
        
        bool mAllowThreshold; ///< For some bars, like the difference bar of lipstick, we don't want to apply threshold.
		
		/// Maintains a pointer to related rectangle view if applicable.
		mutable void* mpRectangleView;
        
        
        
        /*
         // Create surface from images
         cairo_surface_t* hCreateSurfaceFromJPEG(FILE* ipFile) const;
         cairo_surface_t* hCreateSurfaceFromBMP(FILE* ipFile) const;
         cairo_surface_t* hCreateSurfaceFromGIF(FILE* ipFile) const;
         cairo_surface_t* hCreateSurfaceFromImageFile(const char* iFilename) const;
         Int32 hReadByteFromFile(FILE* iFile) const;	// Helper function
         */
	private:
		virtual void hGetFormatFromParent();
	};
    
	inline FormatFill::Ptr ShapeObject::GetFormatFill() const
	{
		return mFormatFillPtr;
	}
	inline bool ShapeObject::IsCompletelyOpaque() const
	{
		return (mFormatFillPtr.IsNull() == false) && (mFormatFillPtr->mAlpha == 255);
	}
	
	inline void ShapeObject::ResetRectangleView(void * ipRectanlgeView)
    {
        if (ipRectanlgeView == mpRectangleView)
        {
            mpRectangleView = NULL;
        }
    }
    
	inline void ShapeObject::ResetRectangleView()
	{
		mpRectangleView = NULL;
	}
    
    inline void ShapeObject::SetAllowThreshold(bool iAllowThreshold)
    {
        mAllowThreshold = iAllowThreshold;
    }
    inline void* ShapeObject::GetRectangleView()
    {
        return mpRectangleView;
    }
    inline void ShapeObject::SetRectangleView(void *iRectangleView)
    {
        mpRectangleView = iRectangleView;
    }
}
#endif
