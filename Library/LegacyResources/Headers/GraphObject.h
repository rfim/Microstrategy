//===========================================================================
//	Filename:		GraphObject.h
//	Date:			2008-05-19
//	Authors:		Bing Huang

//	Copyright (C) 1997-2008 MicroStrategy Incorporated
//	All rights reserved
//===========================================================================

#ifndef MsiChart_GraphObject_h
#define MsiChart_GraphObject_h

#include "Base/Defines/BasicTypes.h"
#include "Synch/Defines/SmartPtr.h"
#include "PDCHeader/PDCstring"

#include "../Common.h"
#include "ImageMap.h"
#include "ChartContext.h"
#include "FormatLine.h"

namespace MsiChart
{
	class GraphObjectManager;
	class GraphCollectionObject;
	typedef std::vector<Point2D> Polygon2D;
	
	typedef enum
	{
		GOU_NORMAL_HIGHLIGHT = 0,
		GOU_GM_HOVER = 1,
		GOU_GM_SELECTED = 2
	}EnumGraphObjectUsage;
	
	/**
     Virtual Base class for all Graph Object.
     */
	class GraphObject
	{
	public:
		//smart ptr
		typedef MSynch::SmartPtr<GraphObject> Ptr;
        
		//ctor & dtor
		GraphObject(const TripleId& irTripleId, GraphObjectManager* ipManager, GraphCollectionObject* ipParentObject, bool iHasLine = true);
		virtual ~GraphObject();
        
		/**
         Get the triple ID of this graph object.
         @param orTripleId The output triple ID.
         */
		void GetTripleId(TripleId& orTripleId) const;
		
		/**
		 Get the object ID of this graph object.
		 @return return the object ID of current GraphObject.
		 */
		Int32 GetObjectId() const;
		
		/**
		 Get the series ID of this graph object.
		 @return return the series ID of current GraphObject.
		 */
		Int32 GetSeriesId() const;
		/**
         Draw the graph object.
         */
		virtual void Draw() const = 0;
		
		/**
		 * Generate UIViews for drawing.
		 * The UIViews generation is seperated from Draw() and put here to optimize the workflow and
		 * support Memory Governor's work.
		 */
		virtual void GenerateView();
        
		/**
         Judge whether the object is in a rectangle or not.
         Used in 3-tier(web) mode for user interactivity.
         @param irRect The referenced rectangle.
         @param iorSelectedObjects, the result list of selected objects, this version is for layer object.
         @return true if object in the referenced rectangle, false if not in.
         */
		virtual bool ObjectInRectangle(const Rect2D& irRect) = 0;
		virtual void ObjectsInRectange(const Rect2D& irRect, std::vector<GraphObject::Ptr>& iorSelectedObjects);
        
		/**
         Draw the Out Line of a graph object.
         Called if users selected the object.
         */
		virtual void DrawOutLine();
        
		/**
         Judge whether a point hits the graph object or not.
         Used in 3-tier(web) mode for user interactivity.
         @param irPoint The input point.
         @return true if the point hits the graph object, false if not hit.
         */
		virtual bool PointInObject(const Point2D& irPoint);
		
		/**
		 Get the distance between current object and an input point.
		 Used in iPad for user interactivity.
		 @param irPoint The input point.
		 @return Return the distance between current object and an input point.
		 If the input point is inside current object, return 0.
		 */
		virtual Int32 DistanceToPoint(const Point2D& irPoint);
        
		/**
         Get graph object Options.
         @param iPropertyId The property ID.
         @param opValue The output value.
         */
		virtual void GetOptions(Int32 iPropertyId, CComVariant* opValue) const;
        
		/**
         Generate Image Map for this graph object.
         Used in 4-tier(web) mode for user interactivity.
         @param iorImageMap The image map instance to be filled.
         */
		virtual void GenerateImageMap(ImageMap& iorImageMap);
        
		/**
         Get the parent object of this graph object.
         @return The pointer to parent object.
         */
		GraphCollectionObject* GetParentObject() const;
        
		/**
         Set format fill to this graph object.
         @param iFormatLinePtr The pointer to the format fill instance.
         */
		void SetFormatLine(FormatLine::Ptr iFormatLinePtr);
        
		/**
         Get format fill to this graph object.
         @return The pointer to the format fill instance.
         */
		FormatLine::Ptr GetFormatLine() const;
		
		/**
         This function gives the bounding rectangle of current GraphObject.
         If this method is useful for a derived class, that class should override this method.
         @param orBoundingRect The output bounding rectangle.
         */
		virtual void GetBoundingRect(Rect2D& orBoundingRect) const;
        
        virtual Point2D GetCenterPoint() const;
        
		/**
         Get the ID of the chart area which current GraphObject belongs to.
         @return Return a chart area ID. If current GraphObject belongs to no ChartArea, return 0.
         */
		Int32 GetChartAreaId() const;
        
		/**
         @param iIsDetectable Indicate whether current object is detectable or not. For example, depth axis lines are not detectable.
         */
		void SetDetectable(bool iIsDetectable);
		
		/** Get data point for tooltip display.
		 @return Return the data point for current GraphObject.
		 */
		virtual Point2D GetDataPoint();
        
        /**
         Get the clockwise polygon of current graph object.
         For those classes which are not derived form ShapeObject, orPolygon does not have to be clockwise.
         Used by DrawOutline(), FillBevel(), PointInObject().
         @param orPolygon The output polygon.
         */
		virtual void GetClockwisePolygon(std::vector<Point2D>& orPolygon) const = 0;
        
		/**
         * prepare the next frame to draw
         */
		virtual void SetProgress(double iProgress);
		
		void SetAnimationEnable(bool iIsAnimationEnabled);
		bool IsAnimationEnabled();
		void SetNativeAnimationEnable(bool iIsNativeAnimationEnabled);
		bool IsNativeAnimationEnabled();
		
		virtual bool IsLayerObject() const;
		
		/**
		 * Get the estimation of memory usage, excluding those used by generated UIViews.
		 */
		virtual float GetMemoryUsageEstimation() const;
		
		virtual void SetHover(bool iIsHovered);
		virtual bool GetHover() const;
		virtual bool IsVirtual() const;
        void GetGraphObjectManager(GraphObjectManager* &oprManager);
        /**
         Set the Outline width to cairo environment.
         */
		void SetOutlineWidth() const;
		void SetOutlineWidth(Int32 iLineWidth) const;
#ifndef _VE_CE
		/**
		 Draw glow effect for a given rectangle.
		 This is for highlight effect when using graph as selector.
		 @param irRect Specifies a rectangle.
		 @param iGradient Specifies a gradient.
		 */
		void DrawGlowEffectForRect(const Rect2D& irRect, CGGradientRef iGradient) const;
		
		/**
		 Draw glow effect for a given polygon.
		 This is for highlight effect when using graph as selector.
		 @param irPolygon Specifies a polygon.
		 @param iGradient Specifies a gradient.
		 */
		void DrawGlowEffectForPolygon(const Polygon2D& irPolygon, CGGradientRef iGradient) const;
#endif
	protected:
		/**
         Set the line width in mFormatLinePtr to cairo environment.
         */
		void SetLineWidth() const;
        
		/**
         Draw the current path using current format line information.
         */
		void ApplyLineFormat() const;
        
		/**
         Draw the current path for Outline using Outline format information.
         */
		void ApplyOutlineFormat() const;
        
        /**
         Draw the current path for 1 px inner Outline using inner Outline format information.
         */
		void ApplyInnerOutlineFormat() const;
        
		/**
         Append a rectangle to the image map.
         @param irRect The input rectangle.
         @param iorImageMap The input and output image map.
         */
		void AppendRectangle2ImageMap(const Rect2D& irRect, ImageMap& iorImageMap) const;
        
		/**
         Append a polygon to the image map.
         @param irRect The input polygon.
         @param iorImageMap The input and output image map.
         */
		void AppendPolygon2ImageMap(const std::vector<Point2D>& irPolygon,ImageMap& iorImageMap) const;
        
		/**
         Given the depth and the angle, get the point in the angular bisector.
         In this case, for the angle ABC, we get a point in its angular bisector, which has
         an distance "irDepth" from both the edge BA and BC.
         @param irA The input point A.
         @param irB The input point B.
         @param irC The input point C.
         @param irDepth The input distance.
         @param orBisectorPoint The output point.
         */
		void GetAngularBisectorIntersectionPoint(const Point2D& irA, const Point2D& irB, const Point2D& irC, float irDepth, Point2D& orBisectorPoint) const;
		
		/**
         Make polygon clockwise.
         @param iorPolygon The input polygon.
         */
		void MakePolygonClockwise(std::vector<Point2D>& iorPolygon) const;
		//bool mIsShown;
		//mutable bool mDeviceValid;
		GraphCollectionObject* mpParentObject;
		FormatLine::Ptr mFormatLinePtr;
        
		const TripleId mTripleId;
		ChartContext::Ptr mChartContextPtr;
		GraphObjectManager* mpManager;
        
		bool mIsDetectable;					///< Indicate whether current object is detectable or not.
        
		double mProgress;
		
		bool mIsAnimationEnabled;
		
		bool mIsNativeAnimationEnabled;
		
		bool mIsHovered;					///< Hover Effect, used in Graph matrix tooltip.
	private:
		/**
         Set the line width.
         @param iLineWidth The input line width.
         */
		void hSetLineWidth(Int32 iLineWidth) const;
        
		/**
         This is a helper method for GraphCollectionObject::AddGraphObject().
         @param ipParentObject Specifies the parent object of current GraphObject.
         */
		void hSetParent(GraphCollectionObject* ipParentObject);
        
		/**
         This is a helper method for GraphCollectionObject::AddGraphObject().
         This method is used to share format object with GraphCollectionObject.
         Make sure hSetParent() is called before calling this method.
         */
		virtual void hGetFormatFromParent();
		
		friend class GraphCollectionObject;
	};
	typedef GraphObject LineObject;
    
	inline Int32 GraphObject::GetObjectId() const
	{
		return mTripleId.mObjectId;
	}
	inline Int32 GraphObject::GetSeriesId() const
	{
		return mTripleId.mSeriesId;
	}
	
	inline GraphCollectionObject* GraphObject::GetParentObject() const
	{
		return mpParentObject;
	}
    
	inline FormatLine::Ptr GraphObject::GetFormatLine() const
	{
		return mFormatLinePtr;
	}
    
	inline void GraphObject::hSetParent(GraphCollectionObject* ipParentObject)
	{
		mpParentObject = ipParentObject;
	}
    
	inline void GraphObject::SetDetectable(bool iIsDetectable)
	{
		mIsDetectable = iIsDetectable;
	}
    
	inline void GraphObject::SetAnimationEnable(bool iIsAnimationEnabled)
	{
		mIsAnimationEnabled = iIsAnimationEnabled;
		SetProgress(0.1);
	}
	inline bool GraphObject::IsAnimationEnabled()
	{
		return mIsAnimationEnabled;
	}
	inline void GraphObject::SetNativeAnimationEnable(bool iIsNativeAnimationEnabled)
	{
		mIsNativeAnimationEnabled = iIsNativeAnimationEnabled;
		SetProgress(0.1);
	}
	inline bool GraphObject::IsNativeAnimationEnabled()
	{
		return mIsNativeAnimationEnabled;	
	}
	inline void GraphObject::SetProgress(double iProgress)
	{
		mProgress = iProgress;
	}	
	inline bool GraphObject::IsLayerObject() const
	{
		return false;
	}
	inline float GraphObject::GetMemoryUsageEstimation() const
	{
		return sizeof(*this);
	}
	inline void GraphObject::SetHover(bool iIsHovered)
	{
		mIsHovered = iIsHovered;
        mChartContextPtr->SetHover(mTripleId, iIsHovered);
	}		
	inline bool GraphObject::GetHover() const
	{
		return mIsHovered;
	}
	inline bool GraphObject::IsVirtual() const
	{
		return false;
	}
    inline void GraphObject::GetGraphObjectManager(GraphObjectManager* &oprManager)
    {
        oprManager = mpManager;
    }
}

#endif
