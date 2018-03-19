//==============================================================================================
//	FILENAME	:	GraphObjectManager.h
//	AUTHOR		:	Liang Liu
//	CREATION	:	2008-05-14
//	Copyright (C) MicroStrategy Incorporated 2008
//	All Rights Reserved
//==============================================================================================
#ifndef MsiChart_GraphObjectManager_h
#define MsiChart_GraphObjectManager_h

#include "PDCHeader/PDCmap"
#include "PDCHeader/PDCvector"
#include "GraphObject.h"
#include "LayerObject.h"

namespace MsiChart
{
	enum EnumDSSGraphObjectManager
	{
		DssChartManager = 20000,
		DssPlotManager,
		DssPlot,
	//	DssPiePlot,	
		DssLegend,
		DssAreaSeriesRenderer,
		DssBarSeriesRenderer,
		DssLineSeriesRenderer,
		DssStepLineSeriesRenderer,
	};

	/**
	This is the base class of those classes which create and maintain GraphObject. 
	Each GraphObjectManager contains a GraphObject list and a GraphObject map.
	A GraphObject list contains a list of GraphObjects. 
	A GraphObject map is used for finding a GraphObject quickly according to a given triple ID. 
	*/
	class GraphObjectManager
	{
	public:
		typedef MSynch::SmartPtr<GraphObjectManager> Ptr;

		/**
		Constructor.
		@param irTripleId Reference to triple ID of current GraphObjectManager. 
		@param ipManager Pointer to the GraphObjectManager which created current GraphObjectManager. 
		Provide NULL for root node (ChartManager).
		*/
		GraphObjectManager(const TripleId& irTripleId, GraphObjectManager* ipManager); 

		virtual ~GraphObjectManager();

		/**
		This function is used to get the triple id of current GraphObjectManager.
		@param orTriple This output parameter will contain the TripleId of current GraphObjectManager. 
		*/
		void GetTripleId(TripleId& orTripleId) const;

		/**
		Provided the cursor coordinate, this function returns the GraphObject which contains that coordinate.
		If there are more then one GraphObject contains this coordinate, the one on top layer will be returned.
		@param irCursor The coordinate of cursor. 
		@return Smart pointer to the selected object. NULL if no selected object is found. 
		*/
		virtual GraphObject::Ptr OnPointDetection(const Point2D& irCursor) const; 
		
		/** This is an enhanced version of OnPointDetection() for iPad.
		 Provided the cursor coordinate, this function returns the nearest GraphObject from that coordinate.		
		 @param irCursor The coordinate of cursor. 
		 @param irObjectIds Specifies a list of candidate object IDs.
		 @param iorDistance Input an initial distance. Output a smaller distance if an object
		 with a smaller distance is found. Otherwise, this parameter will not be changed.
		 @param iRadius Specifies a radius around irCursor.
		 @param iorGraphObjectsAround Appends graph objects within iRadius. 
		 @param iUsage Denote what kind of usage will the selected objects be used for.
		 @return Smart pointer to the selected object. NULL if no selected object is found. 
		 */
		virtual GraphObject::Ptr OnPointDetectionEx(const Point2D& irCursor, 
			const std::vector<Int32>& irObjectIds, Int32& iorDistance, Int32 iRadius,
			std::vector<GraphObject::Ptr>& iorGraphObjectsAround, EnumGraphObjectUsage iUsage = GOU_NORMAL_HIGHLIGHT); 

        virtual void OnPointInPolygon(const Point2D& irCursor, 
                                      const std::vector<Int32>& irObjectIds, const std::vector<Point2D>& iPoints,
                                      std::vector<GraphObject::Ptr>& iorGraphObjectsIn) const; 
		/**
		This function provides the rectangle hit test detection service.
		Provided a bounding rectangle, this function can tell all the object inside that area by the 
		parameter ioSelectedObjects. 
		@param irRect Specifies the bounding rectangle.
		@param iorSelectedObjects To store the pointers of the selected objects. This function will append 
		the smart pointer of each selected GraphObject in current GraphObjectCreater to ioSelectedObjects. 	
		*/
		virtual void OnRectangleSelection(const Rect2D& irRect, std::vector<GraphObject::Ptr>& iorSelectedObjects) const;

		/**
		This function is the drawing function for all graph object. 
		Draw all the GraphObject in the GraphObject list which maintained by current GraphObjectManager.
		*/
		virtual void Draw();
		
		/**
		 * Generate UIViews for drawing.
		 * The UIViews generation is seperated from Draw() and put here to optimize the workflow and
		 * support Memory Governor's work. This function has the same workflow as Draw(),
		 * which means either function is changed, the other one has to update its workflow too.
		 * All the inheriting classes has to ensure this restriction too.
		 */
		virtual void GenerateView();

		/**
		This function is to generate the image map for the web. 
		@param ioImageMap Input and output parameter which contains the image map.
		*/
		virtual void GenerateImageMap(ImageMap& iorImageMap); 
		
		/**
		This function generates GraphObjectMap and GraphObjectList.
		*/
		virtual void GenerateMapAndList() = 0; 	

		/**
		This function gets the options related to current GraphObjectManager.
		@param iSeriesId Series ID.
		@param iPropertyId Specifies the property ID to be set.
		@param opValue A pointer to VARIANT which is used to store property value. 
		*/
		virtual void GetOptions(Int32 iSeriesId, Int32 iPropertyId, CComVariant* opValue) const = 0;			
			
		/**
		This function finds GraphObjects in current subtree. 
		It returns a GraphObjectPtr according to an input triple ID if it exists. 
		@param irTripleId The triple ID of the GraphObject to be queried.
		@param irRecursive If it is false, search GraphObject only in current GraphObjectMap. 
		If it is true and no GraphObject in current GraphObjectMap has irTripleId, recursively 
		search GraphObject in sub-GraphObjectManager. 
		@return Return NULL if no GraphObject has this triple ID in current GraphObjectManager. 
		Otherwise, return a pointer to the required GraphObject. 
		*/
		virtual GraphObject::Ptr GetGraphObject(const TripleId& irTripleId, bool iRecursive = true) const; 

		/**
		This function finds GraphObjectManager in current GraphObjectManager.
		@param irTripleId The triple ID of the GraphObjectManager to be queried.
		@param irRecursive If it is false, search GraphObjectMananger only in current GraphObjectManagerList. 
		If it is true and no GraphObjectManager in current GraphObjectManagerList has irTripleId, recursively 
		search GraphObjectManager in sub-GraphObjectManager. 
		@return Return NULL if no GraphObject has this triple ID in current GraphObjectManager. 
		Otherwise, return a pointer to the required GraphObject. 
		*/
		GraphObjectManager::Ptr GetGraphObjectManager(const TripleId& irTripleId, bool iRecursive = true) const;
#ifndef _VE_CE        
        void GetAllGraphObjectInfo(std::vector<CGRect>  &iAllGraphObjectsBorder, std::vector<MsiChart::TripleId > &iAllGraphObjectsTripleId);
#endif        
        /**
         This function is used to generate virtual marker in Graph Matrix for tooltip.
         currently, it's only called by line chart. we can also extend to all chart type later if necessary.
         */
        virtual void GenerateVirtualMarker();

		/**
		This function returns a pointer to ChartContext.
		@return A pointer to ChartContext.
		*/
		ChartContext::Ptr GetChartContext() const;	
		
		/**
		Get the ID of the chart area which current GraphObjectManager belongs to.
		@return Return a chart area ID. If current GraphObjectManager belongs to no ChartArea, return 0. 
		*/
		Int32 GetChartAreaId() const;

		/**
		Add the GraphObject to mGraphObjectMap and mGraphObjectList.
		@param iGraphObjectPtr Specify a GraphObject which need to be added to mGraphObjectMap and mGraphObjectList.
		*/
		void AddToMapAndList(GraphObject::Ptr iGraphObjectPtr);
        
        void AddToMapAndList(std::vector<GraphObject::Ptr> iGraphObjectPtrList);
		/**
		Add the GraphObject to mGraphObjectList.
		@param iGraphObjectPtr Specify a GraphObject which need to be added to mGraphObjectList.
		*/
		void AddToList(GraphObject::Ptr iGraphObjectPtr);

		/**
		 Add the GraphObjectManager to mGraphObjectManagerList.
		 @param iGraphObjectPtr Specify a GraphObjectManager which need to be added to mGraphObjectManagerList.
		 */
		void AddToManagerList(GraphObjectManager::Ptr iGraphObjectPtr);

		/**
		Add the GraphObject to mGraphObjectMap.
		@param iGraphObjectPtr Specify a GraphObject which need to be added to mGraphObjectMap.
		*/
		void AddToMap(GraphObject::Ptr iGraphObjectPtr);	
		
		/** Insert graph object list after a given graph object.
		 @param irGraphObjectList Specifies a graph object list.
		 @param ipGraphObject Specifies a GraphObject in the graph object list of current GraphObjectManager. 
		 */
		void InsertGraphObjectListAfterObject(const std::vector<GraphObject::Ptr>& irGraphObjectList, const GraphObject* ipGraphObject);
		
		/** This method is used to support non-native animation for Line chart and Gauge chart.		
		 @param ipLayerObject Specifies the LayerObject which needs to be refreshed.
		 */
#ifndef _VE_CE
		virtual void GenerateInterpolationFrame(double iProgress, LayerObject* ipLayerObject);
#else
		virtual void GenerateInterpolationFrame(double iProgress);
#endif
		
		/** Merge current GraphObjectList with a GraphObjectList from a input GraphObjectManager
		 */
		void MergeObjectList(GraphObjectManager* ipGraphObjectManager, bool iIsAppend);
		
		/// Get GraphObject list.
		void GetGraphObjectList(std::vector<GraphObject::Ptr>& orGraphObjectList);	
        
        /// Get GraphObjectManager list.
		void GetGraphObjectManagerList(std::vector<GraphObjectManager::Ptr>& orGraphObjectManagerList);
		
		/// Clear GraphObject list.
		void ClearGraphObjectList();
		
		static GraphObject::Ptr OnPointDetectionByList(const std::vector<GraphObject::Ptr>& irGraphObjectList,
			const Point2D& irCursor, const std::vector<Int32>& irObjectIds, Int32& iorDistance, Int32 iRadius,
			std::vector<GraphObject::Ptr>& iorGraphObjectsAround, EnumGraphObjectUsage iUsage = GOU_NORMAL_HIGHLIGHT);
			
		/// Get memory usage estimation for current GraphObjectManager.	
		float GetMemoryUsageEstimation()  const;
		
		/// Extract some graph objects out by object ID. The extracted graph object will be removed from mGraphObjectList.
		void ExtractGraphObjectsByObjectId(std::vector<GraphObject::Ptr>& orGraphObjectList, Int32 iObjectId);
	protected:
		/**
		Get a bool value from VARIANT_BOOL.
		@param ipValue Specifies a VARIANT which holds a VARIANT_BOOL value.
		@return Return true if ipValue is VARIANT_TRUE. Otherwise, return false.
		*/
		bool ToBool(CComVariant* ipValue) const;
		/**
		Get a VARIANT_BOOL value from a bool value.
		@param iValue Specifies a bool value.
		@param opValue Output a VARIANT which holds a VARIANT_BOOL value.
		*/
		void BoolToVariant(bool iValue, CComVariant* opValue) const;

		/**
		 Adjust GraphObjects for layer.	
		 */
		void AdjustObjectsForLayer();

	protected:
		/**
		Keep pointers to all the GraphObjects which are created by current GraphObjectManager, 
		including both individual GraphObject and GraphCollectionObject.
		*/
		std::map<TripleId, GraphObject::Ptr> mGraphObjectMap;
		typedef std::pair<TripleId, GraphObject::Ptr> MapPair;
	
		/**
		Generally, this list keeps pointers to all the individual GraphObjects which are 
		created by current GraphObjectManager.
		*/
		std::vector<GraphObject::Ptr> mGraphObjectList;	

		/// This list keeps all the children of current GraphObjectManager.
		std::vector<GraphObjectManager::Ptr> mGraphObjectManagerList;

		/// Identifiy current GraphObjectManager. 
		const TripleId mTripleId;

		/// The drawing context instance. Only one object in a chart.
		ChartContext::Ptr mChartContextPtr;

		/// The manager object which created the current object.
		GraphObjectManager* mpManager;
		
		double mProgress;
		LayerObject* mpCurrentLayerObject;
	};		

	inline ChartContext::Ptr GraphObjectManager::GetChartContext() const
	{
		return mChartContextPtr;
	}

	inline bool GraphObjectManager::ToBool(CComVariant* ipValue) const
	{
		_ASSERTE(ipValue->vt == VT_BOOL);
		return ipValue->boolVal != VARIANT_FALSE;
	}
	inline void GraphObjectManager::BoolToVariant(bool iValue, CComVariant* opValue) const
	{
		opValue->vt = VT_BOOL;
		opValue->boolVal = iValue ? VARIANT_TRUE : VARIANT_FALSE;
	}	
}
#endif
