//==============================================================================================
//	FILENAME	:	LayerObject.h
//	AUTHOR		:	Liang Liu
//	CREATION	:	2010-04-28
//	Copyright (C) MicroStrategy Incorporated 2008
//	All Rights Reserved
//==============================================================================================
#ifndef MsiChart_LayerObject_h
#define MsiChart_LayerObject_h

#include "PDCHeader/PDCvector"
#include "GraphObject.h"
#include "LayerObjectHelper.h"

namespace MsiChart
{

	/**
	 Maintains a list of GraphObjects which can be selected and highlighted together.
	 */
	class LayerObject: public GraphObject
	{
#ifdef _VE_CE
		friend class LayerObjectWINHelper;
#else
        friend class LayerObjectIOSHelper;
#endif
	public:
		typedef MSynch::SmartPtr<LayerObject> Ptr;

		/**	Constructor.
		 @param irTripleId This is used by base class.(GraphObject)
		 @param ipManager This is used by base class.
		 */
		LayerObject(const TripleId& irTripleId, GraphObjectManager* ipManager, bool iIsAnimationNeeded);
		virtual ~LayerObject();

		/** This function will add a GraphObject into current LayerObject.
		 @param iGraphObjectPtr Pointer to the GraphObject which is ready to be added into current LayerObject.
		 */
		void AddGraphObject(GraphObject::Ptr iGraphObjectPtr);
        bool DeleteGraphObject(GraphObject::Ptr iGraphObjectPtr);

        void AddRedrawGraphObject(GraphObject::Ptr iGraphObjectPtr);
        bool DeleteRedrawGraphObject(GraphObject::Ptr iGraphObjectPtr);

		/**	Clear mGraphObjectList.
		 */
		void Clear();

		//---------Override functions-------------------------------------
		virtual void Draw() const;
		virtual void GenerateView();
		virtual void DrawOutLine();
		virtual bool ObjectInRectangle(const Rect2D& irRect);
		virtual void GetClockwisePolygon(std::vector<Point2D>& orPolygon) const;
		virtual bool IsLayerObject() const;
		virtual void ObjectsInRectange(const Rect2D& irRect, std::vector<GraphObject::Ptr>& iorSelectedObjects);

#ifndef _VE_CE
		void DrawWithContext(CGContextRef ipCGContext, const CGRect& irRect);
#endif

		/** Enable or disable animation for current LayerObject.
		 @param iIsAnimationNeeded Indicates whether animation is needed for current LayerObject.
		 */
		void EnableAnimation(bool iIsAnimationNeeded);

		/** Check whether animation is needed for current LayerObject.
		 @return Return true if animation is needed for current LayerObject. Otherwise, return false;
		 */
		bool IsAnimationNeeded() const;

		/// Set frame index.
		void SetFrameIndex(Int32 iFrameIndex);
		/// Get frame index.
		Int32 GetFrameIndex() const;
		/// Check whether the current LayerObject is empty.
		bool IsEmpty() const;
		/// Get the duration of animation;
		double GetAnimationDuration() const;
		/**
		 Get the key of corresponding template node. This key can be used for logging.
		 @return Return current template node key.
		 */
		std::string GetTemplateNodeKey() const;

		/// Remove current layer view from its superview and clear reference.
		void ResetLayerView();

		void AnimationDidFinished();

		bool IsInGraphObjectsList(const GraphObject* ipGraphObject);
        bool IsInRedrawGraphObjectsList(const GraphObject* ipGraphObject);

		int getGraphObjectsCount();

		void SetProgress(double iProgress);

		GraphObject::Ptr OnPointDetectionEx(const Point2D& irCursor,
			const std::vector<Int32>& irObjectIds, Int32& iorDistance, Int32 iRadius,
			std::vector<GraphObject::Ptr>& iorGraphObjectsAround, EnumGraphObjectUsage iUsage = GOU_NORMAL_HIGHLIGHT);

#ifndef _VE_CE
		void GetAllGraphObjectInfo(std::vector<CGRect>  &iAllGraphObjectsBorder, std::vector<MsiChart::TripleId > &iAllGraphObjectsTripleId);
#endif

		virtual float GetMemoryUsageEstimation() const;

		void GetGraphObjectList(std::vector<GraphObject::Ptr>& orGraphObjectList);

	protected:
		// Draw the objects in mGraphObjectList;
		void DoDraw() const;

        // A collection of GraphObjects which will be redraw in the highlight view to bring the object to the top of the chart.
        std::vector<GraphObject::Ptr> mRedrawGraphObjectList;

		/// A collection of GraphObjects which share the same format properties.
		std::vector<GraphObject::Ptr> mGraphObjectList;
		bool mIsInSeriesRenderer;
		bool mIsAnimationNeeded;
		Int32 mFrameIndex;
		mutable void* mpLayerObjectView;
		mutable float mPreviousScale;
	private:
		void hGenerateLayerObjectView() const;

		void hMakeClearForHoveredGrid(GraphObject::Ptr iGraphObjectPtr, double iRatio) const;

		//void hClearAllSubviewsFromDrawRect() const;
		LayerObjectHelper *mpLayerObjectHelper;
	};

	inline void LayerObject::EnableAnimation(bool iIsAnimationNeeded)
	{
		mIsAnimationNeeded = iIsAnimationNeeded;
	}

	inline bool LayerObject::IsAnimationNeeded() const
	{
		return mIsAnimationNeeded;
	}
	inline void LayerObject::SetFrameIndex(Int32 iFrameIndex)
	{
		mFrameIndex = iFrameIndex;
	}
	inline Int32 LayerObject::GetFrameIndex() const
	{
		return mFrameIndex;
	}
	inline bool LayerObject::IsLayerObject() const
	{
		return true;
	}

	inline void LayerObject::GetGraphObjectList(std::vector<GraphObject::Ptr>& orGraphObjectList)
	{
		orGraphObjectList = mGraphObjectList;
	}
}
#endif
