//==============================================================================================
//	FILENAME	:	Chart.h
//	AUTHOR		:	Xin Liu
//	CREATION	:	5/20/08
//	Copyright (C) MicroStrategy, Inc. 2008
//==============================================================================================


#ifndef MSICHART_CHART_H
#define	MSICHART_CHART_H
#include "Dataset.h"
#include "PrivateSource/FormatManager.h"
#ifndef _VE_CE
#include "CoreGraphics/CoreGraphics.h"
#else
#include "VisualizationEngine/ChartEngine/JSON/JSONCanvas.h"
#include "VisualizationEngine/ChartEngine/CEOutput/CECanvas.h"
#endif
#include "PrivateSource/ImageMap.h"
#include "Common.h"
#include "PrivateSource/Point2D.h"

class DSSRWTemplateNode;
namespace MsiChart
{
	
	//Goal: We want to enahnce the generic data organization in the txt file, so...
	typedef enum
	{
		PLAIN_LABEL_SEPERATION_FORMAT = 0,  ///< old style supported by CE
		PLAIN_FULL_FORMAT = 1,              ///< AE style
		HIERACHICAL_FORAMT = 2,				///< Hierachical one, we will use it to populate row/column tree in dataset LATER.
	}GenericInputType;  //Added by dshi 
	
	struct RefreshPack
    {
        Dataset::Ptr mDatasetPtr;
        std::map<Int32, std::pair<double, double> > mValueAxisScaleSet;    //x DssGraphX1Body; y1 DssGraphY1Body; y2 DssGraphY2Body
    };
    
    typedef enum
    {
        GTS_DEFAULT = 0,
        GTS_GRADIENT,
    }EnumGraphTooltipStyle;
    
    typedef std::vector<std::pair<std::string, std::string> > GraphTooltipDataProvider;
    typedef std::pair<int, std::pair<double, double> > GraphMatrixMinMaxInfo;

	struct Tooltip
	{
        Tooltip(Int32 iDataPointX, 
				Int32 iDataPointY, 	
				Int32 iNumOfValues,
				Int32 iSeriesId,
				Int32 iGroupId,
				int iSeriesColor,
				const char* ipDataString, 
				const char* ipCategoryName,
				const char* ipSeriesName,				
				const char* ipExDataString1,
				const char* ipExDataString2,
				const char* ipConcatenatedString)
		: mDataPointX(iDataPointX),
		mDataPointY(iDataPointY),
		mNumOfValues(iNumOfValues),
		mSeriesId(iSeriesId),
		mGroupId(iGroupId),
		mSeriesColor(iSeriesColor),
		mDataString(ipDataString),
		mCategoryName(ipCategoryName),
		mSeriesName(ipSeriesName),
		mExDataString1(ipExDataString1),
		mExDataString2(ipExDataString2),
		mConcatenatedString(ipConcatenatedString)
		{
			mObjectId = -3;
            mSliceIndex = mAngleIndex = -3;
            mIsReferenceLine = false;
            mRefrenceLineValue = "";
		}
		
		Tooltip() 
		{
			mDataPointX = mDataPointY = mNumOfValues = mSeriesId = mGroupId = mSeriesColor = 0;
			mObjectId = -3;
            mSliceIndex = mAngleIndex = -3;
            mIsReferenceLine = false;
            mRefrenceLineValue = "";
 		}
		
		void Serieslise(std::vector<int>& orVect) 
		{
			orVect.clear();
			orVect.push_back(mObjectId);
			orVect.push_back(mSeriesId);
			orVect.push_back(mGroupId);
		}
		
		Int32 mDataPointX;         //< DataPoint to show tooltip for all graph types, and infowindow for all graphtypes except budgeting chart.
		Int32 mDataPointY;
		Int32 mNumOfValues;
        Int32 mAngleIndex;
        Int32 mSliceIndex;
		Int32 mSeriesId;
		Int32 mGroupId;
		Int32 mObjectId;	//used in [rotation] to keep the last selected objects.
		int mSeriesColor;	// 0xbbggrr
		std::string mDataString;
		std::string mCategoryName;
		std::string mSeriesName;
		std::string mExDataString1;		
		std::string mExDataString2;	
		std::string mConcatenatedString;	
		
        std::vector<std::string> mCategoryNames;
		std::vector<std::string> mRowLabels;
		std::vector<std::vector<std::string> > mColLabels;
		std::vector<std::string> mMetricsValues;
		std::string mAxisLabelString;
		std::string mSizeByDataString;
        std::string mPercentValueString;
        std::string mPieSlicePercentContribution;
		std::vector<Int32> mRectInfo;	//< The rect info for the selected object.set = (x,y, half width, half height).
        std::vector<Int32> mPolygonInfo;  //< The polygon info to be highlighted for sizeby line chart in selection mode.
        bool mIsReferenceLine;
        std::string mRefrenceLineValue;
        void ToDataProvider(GraphTooltipDataProvider& orDataProvider, Int32 iStyleIndex);
	};
	
    struct SelectedInfo
    {
        SelectedInfo(TripleId iId, Int32 iShape, bool iIsIsolatedDot)
        : mSelectedId(iId),
        mShapeType(iShape),
        mIsIsolatedDot(iIsIsolatedDot)
        {
            
        }
        
        SelectedInfo()
        {
        }
        
        TripleId mSelectedId;
        Int32 mShapeType;
        bool mIsIsolatedDot;
    };
	
	/** This class delegates the whole chart.
	 It provide mainly two functionalities.
	 1. Interface of the Chart Engine. All the API of the Chart Engine are exposed as the public methods of Chart class.
	 2. Manages the graph object manager tree.
	 */
	struct AxisLabelInfoMap;
	struct GraphMatrixContext;	
	class Chart
	{
	public:
		
		/**
		 This function deletes the Chart instance.
		 */
		virtual void Delete() = 0;
		
		//Setting/Getting Data
		
		/**
		 This method is to set the data source for the chart engine.
		 @param iDatasetPtr The input dataset.
		 */
		virtual void SetData(Dataset::Ptr iDatasetPtr) = 0;
		
		/**
		 This method is to get the dataset.
		 @return The dataset.
		 */
		virtual Dataset::Ptr GetData() = 0;
		
		/**
		 This function is to get the displayed data value used for tooltip.
		 @param iSeriesID Specifies the series id.
		 @param iGroupID Specifies the group id.
		 @param orValue Outputs the dislayed data value.
		 @param orPercentValue Outputs the displayed percentage data value.
		 @param iOffset Distinguishes multi-data points, such as scatter where the flags would have different values for XVALUE, YVALUE and so on for other charts
		 */
		virtual bool GetToolTipData(Int32 iSeriesID, Int32 iGroupID, double& orValue, double& orPercentValue, Int32 iOffset = 0) = 0;
		
		/**
		 This function is to get the displayed data value used for tooltip.
		 @param iSeriesID Specifies the series id.
		 @param iGroupID Specifies the group id.
		 @param orDataString Outputs the data string for display.
		 @param iOffset Distinguishes multi-data points, such as scatter where the flags would have different values for XVALUE, YVALUE and so on for other charts
		 */
		virtual bool GetToolTipDataString(Int32 iSeriesID, Int32 iGroupID, MBase::String& orDataString, Int32 iOffset = 0) = 0;
		virtual bool GetToolTipDataString(Int32 iSeriesID, Int32 iGroupID, MBase::String& orDataString, MBase::String& orPercentDataString, Int32 iOffset = 0) = 0;
		
        virtual bool GetPieSliceTooltipDataString(Int32 iSeriesID, Int32 iGroupID, Int32 iAngleID, Int32 iSliceID, Int32 iColumnOffset, MBase::String& orDataString) = 0;
        
		//Used in GraphMatrix to get the colorby or sizeby data.
		virtual bool GetPrimaryDataString(Int32 iSeriesID, Int32 iGroupID, MBase::String& orDataString, bool iIsForColor, Int32 iSizebyIndex = 0) = 0;
        virtual bool GetPrimaryDataStringForPie(Int32 iSeriesId, Int32 iGroupID, MBase::String& orDataString, bool iIsForColor, Int32 iColumnOffset = 0, bool isPercentValue = false, Int32 iAngleIndex = -3, Int32 iSlcieIndex = -3) = 0;
		/** If graph tree is ready.
         @return true means graph tree is already built for rendering.
         */
        virtual bool IsInitialized() const = 0;
        
		//Setting/Getting Attributes
		
		/**
		 This function sets the property specified by PropertyId to the value stored at ipValue.
		 @param iObjectId Identifies the object. Use NULL_OBJECTID if there is no object associated with the property or the property is object-specific.
		 @param iSeriesId Identifies the series number when changing properties that only apply to a specific series. Use NULL_SERIESID if there is no series number associated with the property.
		 @param iGroupId Identifies the group number when changing properties that only apply to a specific group. Use NULL_GROUPID if there is no group number associated with the property.
		 @param iPropertyId Identifies the property to set.
		 @param ipValue Pointer to the new property value.
		 @return indicates the result of modifying the property. True means property value successfully changed while false means failing to set the property value correctly.
		 */
		virtual bool SetOptions(Int32 iObjectId, Int32 iSeriesId, Int32 iGroupId, 
								Int32 iPropertyId, CComVariant* ipValue) = 0;
		
		/**
		 This function gets the property specified by PropertyId and store the property information at opValue
		 @param iObjectId Identifies the object. Use NULL_OBJECTID if there is no object associated with the property or the property is object-specific.
		 @param iSeriesId Identifies the series number when changing properties that only apply to a specific series. Use NULL_SERIESID if there is no series number associated with the property.
		 @param iGroupId Identifies the group number when changing properties that only apply to a specific group. Use NULL_GROUPID if there is no group number associated with the property.
		 @param iPropertyId Identifies the property to get.
		 @param opValue Pointer to location to store the property value
		 @return indicates the result of the request. True means property value successfully returned while false means failing to get the property value correctly.
		 */
		virtual bool GetOptions(Int32 iObjectId, Int32 iSeriesId, Int32 iGroupId, 
								Int32 iPropertyId, CComVariant* opValue) = 0;
		
		/**
		 This function sets the GraphStyle object.
		 @param iGraphStyleWrapperPtr The GraphStyle object.
		 */
		//virtual void SetGraphStyle(GraphStyleWrapper::Ptr iGraphStyleWrapperPtr) = 0;
		
		/**
		 This function sets the graph type.
		 @param iGraphType The graph type to set.
		 */
		virtual void SetGraphType(short iGraphType) = 0;
		
		/**
		 This function gets the graph type.
		 @param orGraphType The output graph type.
		 */
		virtual void GetGraphType(short& orGraphType) = 0;
		
		/**
		 This function specifies whether advanced graph types are supported.
		 @param iAdvancedChartTypesEnabled is a boolean.
		 */
		virtual void SetAdvancedChartTypeSupport(bool iAdvancedChartTypeSupport) = 0;
		
		/**
		 This function gets the graph width.
		 @return The graph width.
		 */
		virtual Int32 GetGraphWidth() const = 0;
		
		/**
		 This function gets the graph height.
		 @return The graph height.
		 */
		virtual Int32 GetGraphHeight() const = 0;
        
        virtual void SetCurrentGraphFrame(Int32 iX, Int32 iY, Int32 iWidth, Int32 iHeight) = 0;
		
		/**
		 This function sets which component will be generated for the chart, by defautl all will be generated.
		 Static or Dynamic components are the other two choices.
		 */
		virtual void SetChartComponent(Int32 iChartComponent) = 0;
		
		/**
		 Get the x-coordinate of the data area origin
		 */
		virtual Int32 GetDataAreaSplitX() const = 0;
		
		/**
		 Get the y-coordinate of the data area origin
		 */
		virtual Int32 GetDataAreaSplitY() const = 0;
		
		/**
		Set the x-coordinate of the data area origin.
		*/
		virtual void SetDataAreaSplitX(Int32 iXCoord) = 0;
		
		//User Interactivity
		
		/**
		 This function provides the point hit test detection service. Provided some coordinate (x, y), this function can tell the object containing that point
		 or the parent object of the selected object by the same parameter oSelectedObject. Whether return the selected object itself or the parent object is 
		 decided by the parameter iSmartSelection. For example, in a Bar graph, when I click on one riser this function could return the triple id of the 
		 selected riser or the triple id of the series containing this riser. In this case the whole series that contains the selected riser will be 
		 identified by one triple id and regarded as the "parent object" of the selected riser.
		 @param iX Specifies the x coordinate of the provided point. The point coordinate is device coordinate.
		 @param iY Specifies the y coordinate of the provided point.
		 @param iSmartSelection Indicates whether the selected object or the parent object should be returned. False means the selected object will be returned, and true means the parent object will be returned.
		 @param oSelectedObject To store the triple id of the selected object or the parent object.
		 @return True means the selected object could be resized and moved, and false means the selected object could NOT be resized and moved.
		 */
		virtual bool OnPointDetection(Int32 iX, Int32 iY, bool iSmartSelection, TripleId& orSelectedObject) = 0;
		
		/**
		 * Return the nearest virtual marker's tripleId. For Time Series.
		 * @param iX Specifies the x coordinate of the provided point.
		 * @param iY Specifies the y coordinate of the provided point.
		 * @param irObjectIds Specifies a list of candidate object IDs.
		 * @param iRadius Specifies the distance within which to detect object.
		 * @param orNearestTripleId the nearest object's tripleId.
		 * @return True if there is an object found.
		 */
		virtual bool GetNearestObject(Int32 iX, Int32 iY, const std::vector<Int32>& irObjectIds,Int32 iRadius, TripleId& orNearestTripleId, int* opNearestDistance = NULL) = 0;
		
		/**
         * Return those objects with distance between input point (iX, iY) smaller than iRadius.
         * @param iX Specifies the x coordinate of the provided point.
		 * @param iY Specifies the y coordinate of the provided point.
		 * @param irObjectIds Specifies a list of candidate object IDs.
		 * @param iRadius Specifies the distance within which to detect object.
		 * @return True if there is an object found.
         */
        
        virtual bool GetNearestObjectForGMCombo(Int32 iX, Int32 iY, const std::vector<Int32>& irObjectIds,Int32 iRadius, TripleId& orNearestTripleId, int* opNearestDistance) = 0;
        virtual bool GetAllObjectsWithInDistance(Int32 iX, Int32 iY, const std::vector<Int32>& irObjectIds, Int32 iRadius, std::vector<SelectedInfo>& orNearObjects, std::vector<Int32>& orDistance) = 0;
		
		/**
		 This function provides the rectangle hit test detection service. Provided a bounding rectangle (x, y, width, height), this function can tell all the 
		 object inside that area by the parameter oSelectedObjects.
		 @param iLeft Specifies the x coordinate of the left top point of the provided rectangle. The rectangle coordinate is device coordinate.
		 @param iTop Specifies the y coordinate of the left top point of the provided rectangle.
		 @param iSmartSelection Indicates whether the selected objects or the parent objects should be returned. False means the selected objects will be returned, and true means the parent objects will be returned
		 @param oSelectedObjects To store the triple ids of the selected object or the parent objects.
		 @return True means the selected object could be resized and moved, and false means the selected object could NOT be resized and moved.
		 */
		virtual bool OnRectangleSelection(Int32 iLeft, Int32 iTop, Int32 iWidth, Int32 iHeight, 
										  bool iSmartSelection, TripleId*& orpSelectionList, Int32 &oNum) = 0;
		
		/**
		 This function tells whether an object is in the selection list or not.
		 @param iObject Specifies the object.
		 @return True means the object is in the selection list, and false means not.
		 */
		virtual bool IsInSelectionList(const TripleId& iObject) = 0;
		
		/**
		 This function adds an object into the selection list.
		 @param iObject Specifies the object.
		 @return True means the object is added into the selection list successfully, and false means the object is already in the selection list.
		 */
		virtual bool AddToSelectionList(const TripleId& iObject) = 0;
		
		/**
		 This function removes an object from the selection list.
		 @param iObject Specifies the object.
		 @return True means the object is removed from the selection list successfully, and false means the object is not in the selection list.
		 */
		virtual bool RemoveFromSelectionList(const TripleId& iObject) = 0;
		
		/**
		 This function clear the selection list.
		 */
		virtual void ClearSelectionList() = 0;
		
		/**
		 This function allocate the selection list for the client.
		 @param orpSelectionList The pointer to the list of triple ids of those selected graph object.
		 @param oNum The number of the graph object in the list.
		 */
		virtual void AllocSelectionList(TripleId*& orpSelectionList, Int32 &oNum) = 0;
		
		/**
		 This function free the selection list allocated by AllocSelectionList().
		 */
		virtual void DisposeSelectionList(TripleId* ipSelectionList) = 0;
		
		//Export
		
		/**
		 This function saves the graph to raw memory. The supported image format type should include PNG, BMP, JPEG and PDF.
		 @param iWidth Width in pixels. 
		 @param iHeight Height in pixels. 
		 @param iFormat Specifies the export image format. 
		 @param oprMemory Reference to the pointer to the memory to store the binary image. 
		 @param oSize Size of the binary image.
		 @return True means the image is saved to memory successfully, and false means failing to save the image to memory.
		 */
		virtual bool SaveImageToMemory(Int32 iWidth, Int32 iHeight, Int32 iFormat, 
									   char*& oprMemory, unsigned Int32& oSize) = 0;
		
		/**
		 This function saves the graph to file. The supported image format type should include PNG, BMP, JPEG and PDF.
		 @param iWidth Width in pixels. 
		 @param iHeight Height in pixels. 
		 @param iFormat Export image format. It should PNG, BMP, JPEG and PDF.
		 @param iFilename File Name.
		 @return True means the image is saved to file successfully, and false means failing to save the image to file.
		 */
		virtual bool SaveImageToFile(Int32 iWidth, Int32 iHeight, 
									 Int32 iFormat, const char* ipFilename) = 0;	
		
		/**
		 This function saves the graph to a surface. 
		 */
		//virtual bool SaveImageToSurface(cairo_surface_t * ipSurface, Int32 iWidth, Int32 iHeight) = 0;
#ifdef _VE_CE
		virtual bool SaveImageToString(Int32 iWidth, Int32 iHeight, std::string& orGraphDesc, 
										bool ibRowsHaveControls, bool ibColumnsHaveControls, bool ibHasMetricSelector, 
										std::vector<std::pair<Int32, Int32> >& iSelectedSeriesAndColumns,
										bool ibSkipRendering = true) = 0;
		virtual bool SaveImageToVEObjects(Int32 iWidth, Int32 iHeight,
										std::vector<MVE::VEFormatList>& ioFormatList, std::vector<MVE::VEUnitList>& ioUnitList,
										bool ibRowsHaveControls, bool ibColumnsHaveControls, bool ibHasMetricSelector, 
										std::vector<std::pair<Int32, Int32> >& iSelectedSeriesAndColumns,
										bool ibSkipRendering = true) = 0;

		virtual void SetCanvas(ChartCanvas* ipCanvas) = 0;
		virtual ChartCanvas* GetCanvas() = 0;
#else
		/**
		 This function saves the graph to a core graphics context. 
		 @param ipCGContest Speicfies a core graphics context.
		 @param iWidth,iHeight Specifies the graph size.
		 @param iShowValueAxis Indicates whether value axis objects needs to be generated or not. The value axis area will be reserved no matter this parameter is true or false.
		 @param iImageUsage Specifies the usage of current image.
		 */
		virtual bool SaveImageToCGContext(CGContext* ipCGContext, Int32 iWidth, Int32 iHeight, bool iShowValueAxis = true, Int32 iImageUsage = IU_GENERAL) = 0;
#endif

		/**
		 This function generates image map for the web client.
		 @param iWidth Width in pixels. 
		 @param iHeight Height in pixels. 
		 @param oImageMap Output to store the generated image map.
		 @return True means the image map is generated successfully, and false means not.
		 */
		virtual bool GenerateImageMap(Int32 iWidth, Int32 iHeight, ImageMap& oImageMap) = 0;
		
		//Graph Imaging
		
		/**
		 This function calculates and constructs, but does not draw, all graph objects. This function DOES generate selection list.
		 @param iWidth Width in pixels. 
		 @param iHeight Height in pixels. 
		 @return True means all the graph objects and graph object managers are generated and constructed successfully, and false means not.
		 */
		virtual bool PlainDraw(Int32 iWidth, Int32 iHeight) = 0;
#ifndef _VE_CE		
		/**
		 This function should be called before PlainDraw so that we can get the correct text size using this context.
		 @param ipCGContext The input CGContext pointer
		 */
		virtual void SetCGContext(CGContext* ipCGContext) = 0;
        
        // Categorya X axis special handle for partial rendering
        virtual void SetIsCategoryXAxisForPartialRendering(bool isForPartialRendering) = 0;
#endif		
		/**
		 This function is a helper function for the client side. Given a triple id, the function return the id of the chart area which the graph object
		 with the triple id belongs to.
		 @param iObject Specifies the object.
		 @return The id of the chart area that the graph object belongs to.
		 */
		virtual Int32 GetChartAreaId(const TripleId& iObject) = 0;
		
		//Misc, tranformation bewteen device coordinate and 3DG virtual coordinate
		
		/**
		 This function transform a point in device coordinate to virtual coordinate.
		 @param iorX Specifies x coordinate of the point.
		 @param iorY Specifies y coordinate of the point.
		 */
		virtual void DeviceToVirtualPoint(Int32& iorX, Int32& iorY) const = 0;
		
		/**
		 This function transform a point in virtual coordinate to device coordinate.
		 @param iorX Specifies x coordinate of the point.
		 @param iorY Specifies y coordinate of the point.
		 */
		virtual void VirtualToDevicePoint(Int32& iorX, Int32& iorY) const = 0;
		
		/**
		 This function gets the data group.
		 @parm orIgnoreForNoSeriesOrGroup, decide how we should group insufficient data.
		 @return The data group.
		 */
		virtual Int32 GetDataGroup(bool& orIgnoreForNoSeriesOrGroup) const = 0;
		
		/**
		 This function gets the auto number format for the value axis.
		 @param opValue Output the number format in a string.
		 */
		virtual void GetAxisNumberFormat(CComVariant* opValue) const = 0;
		
		
		/**
		 Handles zooming event.
		 @param iOldStart, iOldEnd Specifies an old range.
		 @param iNewStart, iNewEnd Specifies a new range.
		 @param iIsVertical Indicate whether to zoom vertically or horizontally.
		 */
		virtual void OnZooming(Int32 iOldStart, Int32 iOldEnd, Int32 iNewStart, Int32 iNewEnd, bool iIsVertical) = 0;
		
		/**
		 Handles zooming by applying a new group range.
		 @param iStartGroup,iEndGroup Specifies a new group range.
		 */
		virtual void OnZoomingByGroupRange(double iStartGroup, double iEndGroup) = 0;
		
		/**
		 Handles double tap event. Zoom out and render original view.		
		 */
		virtual void OnZoomRestore() = 0;	
		
		/**
		 Handles Swiping along category axis.
		 @param iOffset Specifies the offset of swiping.
		 */
		virtual void OnSwipingOutOfZoom(Int32 iOffset) = 0;	
		/**
		 Handles Swiping in zoom state.
		 @param iX Specifies the mouse move offset along horizontal direction. The positive direction is right.
		 @param iY Specifies the mouse move offset along vertical direction. The positive direction is down.
		 */
		virtual void OnSwipingInZoom(Int32 iX, Int32 iY) = 0;	
		
		/**
		 Get the last group ID which is shown in current graph.
		 @return Return the last group ID.
		 */
		virtual Int32 GetLastGroupId() = 0;
		
		/**
		 Get position by Series ID and Group ID.
		 @param iSeriesId,iGroupId Specifies a Series ID and a Group ID.
		 @param orX,orY If a graph object is found for the given IDs, output the position for that graph object.
		 @return Return true if a graph object is found for the given IDs. Otherwise, return false.
		 */
		virtual bool GetPositionById(Int32 iSeriesId, Int32 iGroupId, Int32& orX, Int32& orY) = 0;		
		
		/**
		 Reset group range of dataset and axes state.
		 @param iBeginGroup,iEndGroup Specifes a new group range.
		 */
		virtual void ResetGroupRangeAndAxesState(Int32 iBeginGroup, Int32 iEndGroup) = 0;
		
		/**
		 Get group interval of category axis in pixel.
		 @return Return the group interval in pixel.
		 */
		virtual double GetGroupIntervalInPixel() = 0;
		
		/**
		 Get group label by group ID.
		 @param iGroupId Specifies a group ID in dataset slice context.
		 @param orGroupLabel Output a group label.
		 @param iSeparator Specifies the separator between adjacent elements. 
		 Only useful when tree structure is available in dataset.
		 @param iIsAbbreviated Indicate whether the group label should be abbreviated. 
		 Only useful when tree structure is available in dataset.
		 */
		virtual void GetGroupLabel(Int32 iGroupId, MBase::String& orGroupLabel, const wchar_t iSeparator, bool iIsAbbreviated) = 0;	
		
		
		/**
		 Get series label by group ID.
		 @param iGroupId Specifies a group ID in dataset slice context.
		 @param orGroupLabel Output a group label.
		 @param iSeparator Specifies the separator between adjacent elements. 
		 Only useful when tree structure is available in dataset.
		 @param iIsAbbreviated Indicate whether the group label should be abbreviated. 
		 Only useful when tree structure is available in dataset.
		 */
		virtual void GetSeriesLabel(Int32 iSeriesID,MBase::String& orSeriesLabel, bool iSubstitute = true, Int32 iColumnOffset = 0, bool iIsForSlice = true) = 0;		
		
        /**
		 This function is to Create a tooltip by series ID and group ID.
		 The caller is responsible to delete it.
		 @param iSeriesID Specifies the series id.
		 @param iGroupID Specifies the group id.
         @param iPersistent if set as true, then the property DssGraphSDShowTooltip will be ignored.
		 @return Return a tooltip is created. Otherwise, return NULL.
		 */
		virtual Tooltip* CreateToolTipBySeriesAndGroup(Int32 iSeriesID, Int32 iGroupID, bool iPersistent = false) = 0;
        
		/**
		 This function provides zero or more tooltips for input coordinates.
		 @param iX Specifies the x coordinate of the provided point. The point coordinate is device coordinate.
		 @param iY Specifies the y coordinate of the provided point.	
		 @param orTooltips Output tooltips.
         @param iPersisitent if set as true, the property setting "DssGraphSDShowTooltip" will be ignored.
		 @param Int32 iDetectiveRadius, objects falls inside this radius will be collected.
		 @return Return the number of tooltips in orTooltips.
		 */
		virtual unsigned Int32 GetToolTipByCoords(Int32 iX, Int32 iY, std::vector<Tooltip*>& orTooltips, bool iPersistent = false) = 0;
		virtual unsigned Int32 GetToolTipByCoords(Int32 iX, Int32 iY, std::vector<Tooltip*>& orTooltips, Int32 iDetectiveRadius, bool iPersistent = false) = 0;
	
		/**
		Get tooltip by passing the corresponding tripleId list.
		@param ipSelectionList, the list containing the IDs to search for tooltips
		@param iNum, the number of elements in ipSelectionList
		@param orTooltips, the returned list of tooltips
        @param iPersisitent if set as true, the property setting "DssGraphSDShowTooltip" will be ignored.
		@return Return the size of orTooltips
		*/	
		virtual Int32 CreateTooltipByObject(TripleId* ipSelectionList, Int32 iNum, std::vector<Tooltip*>& orTooltips, bool iPersistent = false) = 0;
		virtual Int32 CreateTooltipByObject(const std::vector<TripleId>& irSelectionList, std::vector<Tooltip*>& orTooltips, bool iPersistent = false) = 0;
		
        /**
         *If we can show tooltip for a graph object at the specified tripleId.
         *@param irId, the object tripleId
         *@return TRUE means we can show tooltip for this object.
         */
        virtual bool CanShowTooltipById(TripleId& irId) const = 0;
        
        /**
		 This function provides zero or more tooltips for input coordinates.
		 @param iX Specifies the x coordinate of the provided point. The point coordinate is device coordinate.
		 @param iY Specifies the y coordinate of the provided point.	
		 @param ioPoint Output data point for the menu controller.
		 */
		virtual int GetDataPointForUIMenu(Int32 iX, Int32 iY, Tooltip& ioTooltip, bool iLSMode = false) = 0;

        // get the number of selected objects
        virtual int GetNumOfSelectedObjects() = 0;
        
        // get selected objects
        virtual int ApplySelectedObjects(Int32 iX, Int32 iY, std::vector<Point2D>& iPoints, Point2D& ioPoint)= 0;
        
		/** To highlight graph object(s) at (ix, iy)
		 @return true means the hover is successful.
		 */
		virtual bool OnHover(Int32 iX, Int32 iY) = 0;
        virtual bool OnHover(Int32 iX, Int32 iY, Int32 iDistance) = 0;
        
        virtual void SetChartOffset(Int32 iXOffset, Int32 iYOffset) = 0;
        
        virtual void HighlightOrHideOneObject(const TripleId irTripleId, bool iIsHide) = 0;
        virtual void HighlightOrHideObjects(const std::vector<TripleId>& irTripleIds, bool iIsHide) = 0;
        virtual bool OnHover(const TripleId& irHoverObject) = 0;
        
        virtual bool ClearOneHighlightObject(MsiChart::TripleId iTripleId) = 0;
		/** To Get the triple Id of the hoved object, the 2nd parameter is left for futher enhancement.
		 @return the tripleId of the hovered object. 
		 */
		virtual bool GetHoveredObject(TripleId& orHoveredObject, Int32 iIndex = 0) = 0;
        
        virtual bool GetHoveredLineObject(TripleId& orHoveredLineObject, Int32 iIndex = 0) = 0;
#ifndef _VE_CE        
        /*
         */
        virtual void GetAllGraphObjectInfo(std::vector<CGRect>  &iAllGraphObjectsBorder, std::vector<MsiChart::TripleId > &iAllGraphObjectsTripleId) = 0;
#endif
        // Actually zoom scale info in GM
        virtual void SetGMZoomScale(float iHeightScale, float iWidthScale) = 0;
		
		/** To Get the shown value axis ticks for uniform axis
		@param orTicksSet, the set to hold the results
		@param iIsForXAxis, which value axis we are getting informatin from
		*/
		virtual	void GetShownNumericTicks(std::set<double>& orTicksSet, bool iIsForXAxis) = 0;
		
		virtual void SetShowNumbericTicks(std::set<double>& irTicksSet, bool iIsForXAxis) = 0;
		
        virtual	void GetStaggerLabelEndLevel(Int32& orLabelEndLevel) = 0;
        virtual void SetStaggerLabelStartLevel(Int32 irLabelStartLevel) = 0;
        
        virtual void SetCategoryAxisLabelInterval(Int32 irLabelInterval) = 0;
        virtual Int32 GetCategoryAxisLabelInterval() = 0;
        
		/**
		 Set logical range of X axis.
		 @param iXLogicalStart,iXLogicalEnd Specifies the range of X axis.
		 */
		virtual void SetXAxisLogicalRange(double iXLogicalStart, double iXLogicalEnd) = 0;
		
		/**
		 Enable or disable voice over.
		 @param iIsVoiceOverMode Specifies whether current chart is for voice over mode.
		 */
		virtual void EnableVoiceOver(bool iIsVoiceOverMode) = 0;
		
		/**
		 Enable or disable time series chart.
		 @param iIsTimeSeriesChart Specifies whether current chart is a time series chart or not.
		 */
		virtual void EnableTimeSeriesChart(bool iIsTimeSeriesChart) = 0;	
		/**
		 Enable or disable macro chart.
		 @param iIsMicroChart Specifies whether current chart is a micro chart or not.
		 */
		virtual void EnableMicroChart(bool iIsMicroChart) = 0;
        
        
        /**
         *To set which part of the time series chart should be rendered.
         *@param iIntegrity, enum from DssTimeSeriesItems
         */
        virtual void SetTimeSeriesIntegrity(Int32 iIntegrity) = 0;
        
		/**
		 Enable or disable graph matrix.
		 @param iIsGraphMatrix Specifies whether current chart is a graph matrix or not.
		 */
		virtual void EnableGraphMatrix(bool iIsGraphMatrix) = 0;
        virtual void EnableVIDocument(bool iIsVIDocument) = 0;
		/**
		 Set the key of corresponding template node. This key can be used for logging.
		 @param irKey Specifies a template node key.		 
		 */
		virtual void SetTemplateNodeKey(const std::string& irKey) = 0;
		
		//---------- APIs for animation. 
		/**
		 Pass GridGraphViewer pointer to chart instance. This is for animation support.
		 @param ipGridGraphViewer GridGraphViewer pointer. The actual type is UIView*. 
		 Use void* here since UIView is an Objective-C class.		 
		 */
		virtual void SetGridGraphViewer(void* ipGridGraphViewer) = 0;		
		
		/** Enable or disable animation.
		 @param iUseAnimation Indicates whether animation is needed or not.
		 */
		virtual void EnableAnimation(bool iUseAnimation) = 0;
		
		/**
		 Reset the animation state so that the chart instance could be reused for animation
		 */
		virtual void ResetAnimationState() = 0;
		
		/** Set the duration of animation.
		 @param iSeconds Specifies animation duration in seconds. 
		 */
		virtual void SetAnimationDuration(double iSeconds) = 0;		
		
        /** Specify whether axis elements should be hidden.
         @param iHide, true means to hide.
         */
        virtual void HideAxisElements(bool iHide) = 0;
        
		//---------- APIs for uniform axis.
		/**
		 Enable uniform axis for graph matrix.
		 @param i{XZ}MinValue,i{XZ}MaxValue Specifies a value range from dataset for Y, X, Z axis.
		 @param iGraphHeight Specifies a rough estimation on graph height.
		 This parameter is used to get relative font size in zoom out mode.
		 In zoom in mode, this parameter is ignored.
		 */
		virtual void EnableUniformAxis(GraphMatrixMinMaxInfo yMinMaxInfo,
                                       GraphMatrixMinMaxInfo xMinMaxInfo,
                                       GraphMatrixMinMaxInfo zMinMaxInfo, Int32 iGraphHeight,
                                       GraphMatrixMinMaxInfo y2MinMaxInfo = std::make_pair(-1, std::make_pair(0.0, 0.0)),
                                       GraphMatrixMinMaxInfo x2MinMaxInfo = std::make_pair(-1, std::make_pair(0.0, 0.0)) ) = 0;
		/**
		 This method should only be called when uniform axis is enabled.
		 This method gets Y axis width in uniform axis mode. 
		 For graph matrix, we assume that Y axis is always a value axis.	 
		 @param iYAxisLength Specifies a rough estimation on Y axis length. Y axis may have less labels for Int32er axis length.		
		 @param iMetricIndex Specifies the metric index that hosts this axis, it only works when the axis is under only one metric and
		 the axis itself is value axis. By default it is -1.
		 @return Return Y axis width.
		 */
		virtual Int32 GetUniformYAxisWidth(Int32 iYAxisLength, Int32 iMetricIndex = -1, bool iIsForSecondaryAxis = false) = 0;
		/**
		 This method should only be called when uniform axis is enabled.
		 This method gets X axis width in uniform axis mode. 
		 For graph matrix, we assume that X axis is always a category axis.		
		 @param iXAxisLength Specifies X axis length. X axis may have less labels for Int32er axis length.
		 @param iMetricIndex Specifies the metric index that hosts this axis, it only works when the axis is under only one metric and
		 the axis itself is value axis. By default it is -1.
		 It is better to call GetUniformYAxisWidth() before calling this method so that X axis length can be calculated based on Y axis width.
		 @return Return X axis width.
		 */
		virtual Int32 GetUniformXAxisHeight(Int32 iXAxisLength, Int32 iMetricIndex = -1, bool iIsForSecondaryAxis = false) = 0;
		
		/**
		 This method should only be called when uniform axis is enabled.
		 This method should be called after GetUniformYAxisWidth() and GetUniformXAxisHeight() are called.
		 This method draws Y axis labels to a give CGContext.
		 @param ipCGContest Speicfies a core graphics context.
		 @param iWidth,iHeight Specifies the image size for Y axis labels.		
		 PS: If you want to draw the entire chart, please call SaveImageToCGContext().
		 */
#ifndef _VE_CE
		virtual void DrawUniformYAxis(CGContext* ipCGContext, Int32 iWidth, Int32 iHeight, bool iIsForSecondaryAxis = false) = 0;
#else
		virtual void DrawUniformYAxis(Int32 iWidth, Int32 iHeight, bool iIsForSecondaryAxis = false) = 0;
#endif
		/**
		 This method should only be called when uniform axis is enabled.
		 This method should be called after GetUniformYAxisWidth() and GetUniformXAxisHeight() are called.
		 This method draws X axis labels to a give CGContext.		
		 @param ipCGContest Speicfies a core graphics context.
		 @param iWidth,iHeight Specifies the image size for X axis labels.		
		 PS: If you want to draw the entire chart, please call SaveImageToCGContext().
		 */
#ifndef _VE_CE
		virtual void DrawUniformXAxis(CGContext* ipCGContext, Int32 iWidth, Int32 iHeight, bool iIsForSecondaryAxis = false) = 0;
#else
		virtual void DrawUniformXAxis(Int32 iWidth, Int32 iHeight, bool iIsForSecondaryAxis = false) = 0;		
#endif
		/**
		 This method should only be called when uniform axis is enabled.
		 This method should be called after GetUniformYAxisWidth() and GetUniformXAxisHeight() are called.
		 This method draws X axis labels to a give CGContext.		
		 @param ipCGContest Speicfies a core graphics context.
		 @param iWidth,iHeight Specifies the image size for X axis labels.		
		 PS: If you want to draw the entire chart, please call SaveImageToCGContext().
		 */
#ifndef _VE_CE
		virtual void DrawDataAreaForUniformAxis(CGContext* ipCGContext, Int32 iWidth, Int32 iHeight) = 0;
#else
		virtual void DrawDataAreaForUniformAxis(Int32 iWidth, Int32 iHeight) = 0;
#endif
		/**
		 This method should only be called when uniform axis is enabled.		
		 This method gets the margins around data area.
		 Before calling this metohd, GetUniformYAxisWidth() and GetUniformXAxisHeight() should be called.
		 @param orLeftMargin,orRightMargin,orTopMargin,orBottomMargin Output margin info around data area. 
		 Use these margins to SetUniformAxisInfo() for other chart instances.	
		 */
		virtual void GetUniformAxisMargins(short& orLeftMargin, short& orRightMargin, short& orTopMargin, short& orBottomMargin, bool iIsForSecondaryAxis = false) = 0;
		/**
		 This method should only be called when uniform axis is enabled.		
		 @param iLeftMargin,iRightMargin,iTopMargin,iBottomMargin Set margin info around data area.		
		 */
		virtual void SetUniformAxisMargins(short iLeftMargin, short iRightMargin, short iTopMargin, short iBottomMargin, bool iIsForSecondaryAxis = false) = 0;
		
		/**
		 Get the label informatin for the uniform axis, used in iPad Graph Matrix for axis label tooltip
		 */	
		//virtual void GetUniformAxisLabelInfoMap(AxisLabelInfoMap* ipInfoMap, bool iIsXAxis) = 0;
		
		
		/**
		 Get row and column indices by coordinates of touch point.
		 @param iX,iY Specifies the coordinates of touch point.
		 @param orRowIndex,orColumnIndex Output row and column indices if a riser or data marker is found.
		 @return Return true if a riser or data marker is found for the input touch point.
		 */
		virtual bool GetRowAndColumnIndexByCoords(int iX, int iY, int& orRowIndex, int& orColumnIndex, bool iIsDataCellOnly, int iDistance=-1) = 0;				
		
        /**
         Decide if the coords is for a graph axis label and if this label is drillable.
		 @param iX,iY Specifies the coordinates of touch point.
         @param iorLinkURL, if it is drillable, the link will be stored here.   
         @return Return true if the (iX, iY) is for a graph axis label and this label is drillabel, and false otherwise.
         */
        virtual bool IsDrillableLabelAtCoords(int iX, int iY, std::string& orLinkURL) = 0;
        
        
		/// Set a new FormatManager to current chart.
		virtual void SetFormatManager(FormatManager::Ptr iFormatManagerPtr) = 0; 
		
		/**
		 * Get the <seriesId, groupId> pairs of objects that are highlighted.
		 * @param orHighlightObjects, the result <seriesId, groupId> pairs.
		 * @return void.
		 */
		virtual void GetHighlightObjects(std::vector<std::pair<Int32, Int32> >& orHighlightObjects) const = 0;
		virtual void GetHighlightObjects(std::vector<MsiChart::TripleId>& orHighlightObjects) const = 0;
		
		/// Highlight selected object
		virtual void HighlightSelectedObject(bool iHighlight, bool iIsForLassoSelction = false) = 0;
        
        /// Get Selected Objects by Lasso Selections
        virtual void GetSOByLassoSelection(std::vector<std::pair<int,int> >& iObjects) = 0;
		
		/// Highlight selected object when the graph document is opened
		virtual void HighlightSelectedInInit(std::vector<std::pair<Int32, Int32> >& iSelectedSeriesAndColumns) = 0;
        
        /// Enter gray out mode
        virtual void EnterGrayoutMode() = 0;
        
        /// Exit gray out mode
        virtual void ExitGrayoutMode() = 0;
        
        /// is gray out mode
        virtual bool IsGrayoutMode() const = 0;
        
        virtual void EnterDuplicateMode() = 0;
        virtual void ExitDuplicateMode() = 0;
        virtual bool IsDuplicateMode() const = 0;
		
		/**
		 This function refresh the graph matirx context. Used in Graph Matrix.
		 */
		virtual void RefreshGraphMatrixContext() = 0;
        
        /**
         This function is to pass the graph matrix context from Widget to chart instance, iGMCEContext is an object-c class, use void*
         */
        virtual void SetGMCEContext(void* iGMCEContext) = 0;
		
		/** This function refresh chart's some internal data structure (like some caches) once the dataset is reset/refreshed
         */
        virtual void RefreshOnDataChange() = 0;

		/**
		 This function get series color from a default color palette
		 */
		virtual Int32 GetSeriesColorFromPalette(Int32 iIndex) = 0;
		
		/// Check whether the data is enough for selected graph type.
		virtual bool IsDataEnough() = 0;
		
		/// Get memory usage estimation before drawing.
		virtual float GetMemoryUsageEstimation() = 0;
		
		virtual void SetFontScale(double iRatio = 1.0) = 0;
		
		// 470358 Set DSSRWTemplateNode to resolve macros such as {&ReportName} in tooltip.
		virtual void SetTemplateNode(DSSRWTemplateNode* ipTemplateNode) = 0;
		
		/**
		 GM to get the 0-based line.
		 @param iTickValue, the value of the tick.
		 @param iIsForXAxis, wether this computation is on x axis. 
		 @parma iGraphDimentionLength, when iIsForXAxis is true, this one should be graph width otherwise it should be height.
		 @return: the position of that tick.
		 */
		virtual double GetPositionAtTick(double iTickValue, bool iIsForXAxis, Int32 iGraphDimentionLength) = 0;
		
		/**
		 Speicifies whehter entire chart is needed.
		 @param iIsEntireChart Speicifies whehter entire chart is needed.
		 */
		virtual void SetEntireness(bool iIsEntireChart) = 0;
		
		/**
		 RR, to get the space for one interval along the specified numeric axis, if it is not numberic, return 0
		 @param iIsXAxis, wether this computation is on x axis. 
		 @return: the space for one interval along the denoted axis.
		 */
		virtual Int32 GetIntervalSpace(bool iIsXAxis) = 0;
	
		/**
		RR, to decide if we need zero base line, this should only be called for bar chart case.
		@param iIsXAxis, wether this computation is on x axis. 
		@return: true means we can draw zero base line.
		*/
		virtual bool IsZeroBaseLineQualified(bool iIsXAxis, bool isSecondaryAxis) = 0;
		
		/** To layout the axis in "Fit to Content" mode.
		@return the total length/height of this axis.
		@iIsXAxis, x-axis or y-axis to layout, if it is y axis, no need to consider iIsRotated, since the labels will always be horizontally placed.
		@iIsRotated, if the labels should be rotated, useful when it is x-axis.
		@orMaxLabelSpace, the max space taken by one single label, if it is rotated, this is a fixed value.
		*/
		virtual Int32 LayoutAxis(bool iIsXAxis, bool iIsRotated, Int32& orMaxLabelSpace) = 0;	
	
        /** To see how the axis labels are rotated, used in Graph matrix mode.
         @iIsXAxis, true means the uniform x axis.
         @return the rotated mode.
         */
		virtual Int32 GetAxisLabelRotaion(bool iIsXAxis) = 0;				
		
		virtual Int32 GetMaxElementRadius(Int32 iWidth, Int32 iHeight, bool iIsPerfectPadding = false) = 0;
        
        virtual Int32 GetCachedMaxElementSize() = 0;
        
        ////////////////////Data Label and Custom min/max method//////////////////////
        //judge whether should rotate data label in vertical bar chart. used in GMCEController
        virtual bool ShouldRotateDataLabel(Int32 iWidth, Int32 iHeight, bool iIsForPefectPadding = false) = 0;
        
        //judge whether should adjust value axis, used in GMCEDataManager
        virtual bool ShouldAdjustValueAxis(Int32 iWidth, Int32 iHeight, double &irAdjustedValue, bool iIsForPefectPadding = false) = 0;
		
        /** To see if one object with the input iSeriesId iGroupId is an isolated dot.
         @param irId, the input object id to test.
         @return TRUE means this is an isolated dot and false otherwise.
         */
		virtual	bool IsIsolatedDot(const TripleId& irId) = 0;
        
        /** To see if one object with the input iSeriesId iGroupId is an isolated dot in line chart.
         */
		virtual	bool IsIsolatedDotInLine(const TripleId& irId) = 0;
		
        /** To get the position of the banding into charts. used in Graph matrix
         @iIsXAxis, is X axis or not.
         @orPostions, the results for banding postions.
         @return true if we get the result with success.
         */
        virtual bool GetInterlacedBandingPosition(bool iIsXAxis, std::vector<double>& orPostions, bool iIsFromUniformAxis = true) = 0;
        
		/** Both functions retuns the size information for a specific area/object
		@param orVect, returned information in terms of <x,y, w, h>
		@param iGroupIndex, the area for the indicated group.
		@param iIsLegend, denote if we are getting the area for legend.
		@param irObjectId, the target object to search for size information.
		@return Return true if the result is valid.
		*/
		virtual bool GetGroupSpace(std::vector<Int32>& orVect, Int32 iGroupIndex, bool iIsLegned = false) = 0;
		virtual bool GetSpaceAt(std::vector<Int32>& orVect, const TripleId& irObjectId) = 0;
        
        /**
         Zoom spec, if the charts are generated right after zoom, animation should be disabled.
         @param iDisableAnimation, true means animation should be disabled.
         */
        virtual void DisableAnimationForZoom(bool iDisableAnimation) = 0;
        
        /**
         Graph Matrix, to refresh chart by changing its Dataset/Axis component.
         @param irRefreshPack, the data structure containing information about which component to refresh.
         @return true means refresh is done with success.
         */
        virtual bool RefreshChart(const RefreshPack& irRefreshPack) = 0;
        
        /**
         Graph Matrix, to get the four paddings around the data area for each mini-chart.
         @param orPaddings, the result padding set. It is put in the following sequence: <xLow, xHigh, yLow, yHigh.>
         @return true means getting padding is done with success.
         */
        virtual bool GetDataAreaPadding(std::vector<double>& orPaddings) = 0;
        
        /**
         If this chart is a vertically lay-out chart.
         @return TRUE means a vertical chart and false otherwise.
         */
        virtual bool IsVerticalChart() const = 0;
        virtual void SetTextAsShape(bool iFlag) = 0;
        virtual void cleanDataSet() = 0;
	};
	
	typedef MBase::StrongPtr<Chart, MBase::DeleteOperatorGeneric<Chart> > ChartPtr;	

#ifdef _VE_CE
    CHARTENGINE_API FormatManager::Ptr CreateFormatManager(ICDSSRWIterator* ipIterator, ICDSSGridFormatter* ipGridFormatter);
#endif

	/**
	 This function creates a Chart instance given a dataset.
	 @param iDatasetPtr The input dataset.
	 @param ipGDFFileName The input GDF file name. If it is NULL, create a chart instance without GDF.
	 @return The pointer to a Chart instance.
	 */
	CHARTENGINE_API Chart* CreateChart(Dataset::Ptr iDatasetPtr, const char* ipGDFFileName = NULL);	
	
	/**
	 This function creates a Chart instance given a Template.
	 @param ipTemplate The input Template.
	 @param iBeginRow The begin row of the template window.
	 @param iEndRow The end row of the template window.
	 @param iBeginCol The begin column of the template window.
	 @param iEndCol The end column of the template window.
	 @return The pointer to a Chart instance.
	 */
	//CHARTENGINE_API Chart* CreateChartFromTemplate(IDSSTemplate* ipTemplate, Int32 iBeginRow, Int32 iEndRow, Int32 iBeginCol, Int32 iEndCol);
	
	/**
	 This function creates a Chart instance given a file containing raw text data and a GDF as base graphstyle.
	 @param ipDataFileName The input file name.
	 @param ipGDFFileName The input GDF file name.
	 @param iType, indicate how the input TXT file is organized, we may bring in more format later. 
	 @return The pointer to a Chart instance.
	 */
	CHARTENGINE_API Chart* CreateChartFromRawDataAndGDF(const char* ipDataFileName, const char* ipGDFFileName, GenericInputType iType = PLAIN_LABEL_SEPERATION_FORMAT);
	
	
	CHARTENGINE_API Chart* CreateChartFromFormatManager(Dataset::Ptr iDatasetPtr, FormatManager::Ptr iFormatManagerPtr, const char* ipGDFFileName = NULL, bool iForceGraph = false);

#ifdef _VE_CE
	CHARTENGINE_API JSONCanvas::Ptr CreateJSONCanvas();
	CHARTENGINE_API CECanvas::Ptr CreateCECanvas();
#endif
}

#endif
