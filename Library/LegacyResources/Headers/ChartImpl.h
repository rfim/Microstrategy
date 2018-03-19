//==============================================================================================
//	FILENAME	:	ChartImpl.h
//	AUTHOR		:	Xin Liu
//	CREATION	:	5/20/08
//	Copyright (C) MicroStrategy, Inc. 2008
//==============================================================================================


#ifndef MSICHART_CHARTIMPL_H
#define	MSICHART_CHARTIMPL_H

#include "../Chart.h"

#include "GraphObject.h"
#include "ChartManager.h"
#include "ChartContext.h"
#ifdef _VE_CE
#include "ChartCanvas.h"
#else
#include "LayerObject.h"
#endif
#include "ValueAxis.h"
#include "CategoryAxis.h"
#include "NumberObject.h"

namespace MsiChart
{
	/**
	This class is the implementation class of Chart class.
	*/
	class ChartImpl: public Chart
	{
	public:
		//Initialization and Termination
		ChartImpl(Dataset::Ptr iDatasetPtr);
		//ChartImpl(GraphStyleWrapper* ipGraphStyleWrapper, Dataset::Ptr iDatasetPtr);
		ChartImpl(Dataset::Ptr iDatasetPtr, const char* ipGDFFileName);
		ChartImpl(Dataset::Ptr iDatasetPtr, FormatManager::Ptr iFormatManagerPtr, const char* ipGDFFileName, bool iForceGraph);
		virtual void Delete();
		
		static bool Initialize();
		static bool Finalize();

		//Setting/Getting Data
		void SetData(Dataset::Ptr iDatasetPtr);
		Dataset::Ptr GetData();
		bool GetToolTipData(Int32 iSeriesID, Int32 iGroupID, double& orValue, double& orPercentValue, Int32 iOffset = 0);		
		bool GetToolTipDataString(Int32 iSeriesID, Int32 iGroupID, MBase::String& orDataString, Int32 iOffset = 0);
		bool GetPrimaryDataString(Int32 iSeriesID, Int32 iGroupID, MBase::String& orDataString, bool iIsForColor, Int32 iSizebyIndex = 0);
        bool GetPrimaryDataStringForPie(Int32 iSeries, Int32 iGroupID, MBase::String& orDataString, bool iIsForColor, Int32 iColumnOffset = 0, bool isPercentValue = false, Int32 iAngleIndex = -3, Int32 iSliceIndex = -3);
        bool GetPieSliceTooltipDataString(Int32 iSeriesID, Int32 iGroupID, Int32 iAngleID, Int32 iSliceID, Int32 iColumnOffset, MBase::String& orDataString);
        bool GetToolTipDataString(Int32 iSeriesID, Int32 iGroupID, MBase::String& orDataString, MBase::String& orPercentDataString, Int32 iOffset = 0);

		//Setting/Getting Attributes
		bool SetOptions(Int32 iObjectId, Int32 iSeriesId, Int32 iGroupId, 
						Int32 iPropertyId, CComVariant* ipValue);
		bool GetOptions(Int32 iObjectId, Int32 iSeriesId, Int32 iGroupId, 
						Int32 iPropertyId, CComVariant* opValue);
		void SetGraphType(short iGraphType);
		void GetGraphType(short& orGraphType);
		Int32 GetGraphWidth() const;
		Int32 GetGraphHeight() const;
		void SetCurrentGraphFrame(Int32 iX, Int32 iY, Int32 iWidth, Int32 iHeight);
		
        void SetAdvancedChartTypeSupport(bool iAdvancedChartTypeSupport);

		//User Interactivity
		bool OnPointDetection(Int32 iX, Int32 iY, bool iSmartSelection, TripleId& orSelectedObject);
		bool GetNearestObject(Int32 iX, Int32 iY, const std::vector<Int32>& irObjectIds,Int32 iRadius, TripleId& orNearestTripleId, int* opNearestDistance = NULL);
        bool GetNearestObjectForGMCombo(Int32 iX, Int32 iY, const std::vector<Int32>& irObjectIds,Int32 iRadius, TripleId& orNearestTripleId, int* opNearestDistance = NULL);
        bool GetAllObjectsWithInDistance(Int32 iX, Int32 iY, const std::vector<Int32>& irObjectIds, Int32 iRadius, std::vector<SelectedInfo>& orNearObjects, std::vector<Int32>& orDistance);
		bool OnRectangleSelection(Int32 iLeft, Int32 iTop, Int32 iWidth, Int32 iHeight, 
								bool iSmartSelection, TripleId*& orpSelectionList, Int32 &oNum);
        
		bool IsInSelectionList(const TripleId& iObject);
		bool AddToSelectionList(const TripleId& iObject);
		bool RemoveFromSelectionList(const TripleId& iObject);
	//	void GetSelectionList(std::vector<TripleId>& orSelectionList);
		void ClearSelectionList();

		virtual void AllocSelectionList(TripleId*& orpSelectionList, Int32 &oNum);
		virtual void DisposeSelectionList(TripleId* ipSelectionList);

		//Export
		bool SaveImageToMemory(Int32 iWidth, Int32 iHeight, Int32 iFormat, 
							char*& oprMemory, unsigned Int32& orSize);
		bool SaveImageToFile(Int32 iWidth, Int32 iHeight, 
							Int32 iFormat, const char* ipFilename);
#ifdef _VE_CE		
		bool SaveImageToString(Int32 iWidth, Int32 iHeight, std::string& orGraphDesc, 
										bool ibRowsHaveControls, bool ibColumnsHaveControls, bool ibHasMetricSelector, 
										std::vector<std::pair<Int32, Int32> >& iSelectedSeriesAndColumns,
										bool ibSkipRendering = true);

		bool SaveImageToVEObjects(Int32 iWidth, Int32 iHeight,
			std::vector<MVE::VEFormatList>& ioFormatList, std::vector<MVE::VEUnitList>& ioUnitList,
			bool ibRowsHaveControls, bool ibColumnsHaveControls, bool ibHasMetricSelector, 
			std::vector<std::pair<Int32, Int32> >& iSelectedSeriesAndColumns,
			bool ibSkipRendering = true);

#else
		bool SaveImageToCGContext(CGContext* ipCGContext, Int32 iWidth, Int32 iHeight, bool iShowValueAxis = true, Int32 iImageUsage = IU_GENERAL);
#endif		
		Int32 GetMaxElementRadius(Int32 iWidth, Int32 iHeight, bool iIsPerfectPadding = false);
        
        Int32 GetCachedMaxElementSize();
        
        bool ShouldRotateDataLabel(Int32 iWidth, Int32 iHeight, bool iIsForPefectPadding = false); 
        
        bool ShouldAdjustValueAxis(Int32 iWidth, Int32 iHeight, double &irAdjustedValue, bool iIsForPefectPadding = false);

		bool GenerateImageMap(Int32 iWidth, Int32 iHeight, ImageMap& oImageMap);
		

		//Set Default Properties
		//void SetGraphStyle(GraphStyleWrapper::Ptr iGraphStyleWrapperPtr);

		//Graph Imaging
		bool PlainDraw(Int32 iWidth, Int32 iHeight);
#ifndef _VE_CE
		void SetCGContext(CGContext* ipCGContext);
        void SetIsCategoryXAxisForPartialRendering(bool isForPartialRendering);
#endif
		//Helper functions
		Int32 GetChartAreaId(const TripleId& iObject);

		//Misc, tranformation bewteen device coordinate and 3DG virtual coordinate
		void DeviceToVirtualPoint(Int32& iorX, Int32& iorY) const;
		void VirtualToDevicePoint(Int32& iorX, Int32& iorY) const;

		Int32 GetDataGroup(bool& orIgnoreForNoSeriesOrGroup) const;

		void GetAxisNumberFormat(CComVariant* opValue) const;
		
		void OnZooming(Int32 iOldStart, Int32 iOldEnd, Int32 iNewStart, Int32 iNewEnd, bool iIsVertical);
		
		void OnZoomingByGroupRange(double iStartGroup, double iEndGroup);
		
		void OnZoomRestore();
		
		void OnSwipingOutOfZoom(Int32 iOffset);
		
		void OnSwipingInZoom(Int32 iX, Int32 iY);
		
		void SetChartComponent(Int32 iChartComponent);
		
		Int32 GetDataAreaSplitX() const;
		
		Int32 GetDataAreaSplitY() const;
		
		void SetDataAreaSplitX(Int32 iXCoord);
		
		Int32 GetLastGroupId();
		
		bool GetPositionById(Int32 iSeriesId, Int32 iGroupId, Int32& orX, Int32& orY);
		
		void ResetGroupRangeAndAxesState(Int32 iBeginGroup, Int32 iEndGroup);
		
		double GetGroupIntervalInPixel();
		
		void GetGroupLabel(Int32 iGroupId, MBase::String& orGroupLabel, const wchar_t iSeparator, bool iIsAbbreviated);
		
		void GetSeriesLabel(Int32 iSeriesID, MBase::String& orSeriesLabel, bool iSubstitute, Int32 iColumnOffset, bool iIsForSlice);

#ifdef _VE_CE
		void SetCanvas(ChartCanvas* ipCanvas);

		ChartCanvas*  GetCanvas();
#endif

        int GetDataPointForUIMenu(Int32 iX, Int32 iY, Tooltip& ioTooltip, bool iLSMode = false);
        int GetNumOfSelectedObjects();
        int ApplySelectedObjects(Int32 iX, Int32 iY, std::vector<Point2D>& iPoints, Point2D& ioPoint);
        
		unsigned Int32 GetToolTipByCoords(Int32 iX, Int32 iY, std::vector<Tooltip*>& orTooltips, bool iPersistent = false);

		unsigned Int32 GetToolTipByCoords(Int32 iX, Int32 iY, std::vector<Tooltip*>& orTooltips, Int32 iDetectiveRadius, bool iPersistent = false);
        bool CanShowTooltipById(TripleId& irId) const;

        
		Int32 CreateTooltipByObject(TripleId* ipSelectionList, Int32 iNum, std::vector<Tooltip*>& orTooltips, bool iPersistent = false);
        Int32 CreateTooltipByObject(const std::vector<TripleId>& irSelectionList, std::vector<Tooltip*>& orTooltips, bool iPersistent = false);

        Tooltip* CreateToolTipBySeriesAndGroup(Int32 iSeriesID, Int32 iGroupID, bool iPersistent = false);


		bool OnHover(Int32 iX, Int32 iY);
        bool OnHover(Int32 iX, Int32 iY, Int32 iDistance);
        bool OnHover(const TripleId& irHoverObject);

		bool GetHoveredObject(TripleId& orHoveredObject, Int32 iIndex = 0);
        bool GetHoveredLineObject(TripleId& orHoveredLineObject, Int32 iIndex = 0);
		void SetTemplateNodeKey(const std::string& irKey);
		
		void SetGridGraphViewer(void* ipGridGraphViewer);				
		void EnableAnimation(bool iUseAnimation);		
		void ResetAnimationState();
		void SetAnimationDuration(double iSeconds);
        void HideAxisElements(bool iHide);

		void SetXAxisLogicalRange(double iXLogicalStart, double iXLogicalEnd);		
		void EnableTimeSeriesChart(bool iIsTimeSeriesChart);
        void SetTimeSeriesIntegrity(Int32 iIntegrity);
		void EnableVoiceOver(bool iIsVoiceOverMode);
		void EnableMicroChart(bool iIsMicroChart);
		void EnableGraphMatrix(bool iIsGraphMatrix);
        void EnableVIDocument(bool iIsVIDocument);

		void EnableUniformAxis(GraphMatrixMinMaxInfo yMinMaxInfo, GraphMatrixMinMaxInfo xMinMaxInfo, GraphMatrixMinMaxInfo zMinMaxInfo, Int32 iGraphHeight,GraphMatrixMinMaxInfo y2MinMaxInfo = std::make_pair(-1, std::make_pair(0.0, 0.0)), GraphMatrixMinMaxInfo x2MinMaxInfo = std::make_pair(-1, std::make_pair(0.0, 0.0)) );
		Int32 GetUniformYAxisWidth(Int32 iYAxisLength, Int32 iMetricIndex = -1, bool iIsForSecondaryAxis = false);
		Int32 GetUniformXAxisHeight(Int32 iXAxisLength, Int32 iMetricIndex = -1, bool iIsForSecondaryAxis = false);
        //Adjust padding for Data Label in GM
        bool AdjustingAxisPaddingForDataLabel(Rect2D& iorRect, Int32 iMetricIndex, bool iIsForXAxis,Int32 iAxisLength);
        bool hAdjustingXAxisPadding(Rect2D& iorRect, std::vector<Int32> iMetricIndexList,Int32 iXAxisLength);
        bool hAdjustingYAxisPadding(Rect2D& iorRect, std::vector<Int32> iMetricIndexList,Int32 iYAxisLength);
        TextObject::Ptr hCreateExtremeDataLabel(std::vector<Int32> iMetricIndexList, bool iIsForXAxis);
		void GetShownNumericTicks(std::set<double>& orTicksSet, bool iIsForXAxis);
		void SetShowNumbericTicks(std::set<double>& irTicksSet, bool iIsForXAxis);
#ifndef _VE_CE
		void DrawUniformYAxis(CGContext* ipCGContext, Int32 iWidth, Int32 iHeight, bool iIsForSecondaryAxis = false);
		void DrawUniformXAxis(CGContext* ipCGContext, Int32 iWidth, Int32 iHeight, bool iIsForSecondaryAxis = false);
		void DrawDataAreaForUniformAxis(CGContext* ipCGContext, Int32 iWidth, Int32 iHeight);
#else
		void DrawUniformYAxis(Int32 iWidth, Int32 iHeight, bool iIsForSecondaryAxis = false);
		void DrawUniformXAxis(Int32 iWidth, Int32 iHeight, bool iIsForSecondaryAxis = false);
		void DrawDataAreaForUniformAxis(Int32 iWidth, Int32 iHeight);
#endif
		void GetUniformAxisMargins(short& orLeftMargin, short& orRightMargin, short& orTopMargin, short& orBottomMargin, bool iIsForSecondaryAxis = false);
		void SetUniformAxisMargins(short iLeftMargin, short iRightMargin, short iTopMargin, short iBottomMargin, bool iIsForSecondaryAxis = false);
		bool GetRowAndColumnIndexByCoords(int iX, int iY, int& orRowIndex, int& orColumnIndex, bool iIsDataCellOnly, int iDistance=-1);
        
        void GetStaggerLabelEndLevel(Int32& orLabelEndLevel);
        void SetStaggerLabelStartLevel(Int32 irLabelStartLevel);
        void SetCategoryAxisLabelInterval(Int32 irLabelInterval);
        Int32 GetCategoryAxisLabelInterval();
        
        bool IsDrillableLabelAtCoords(int iX, int iY, std::string& orLinkURL);

		void SetFormatManager(FormatManager::Ptr iFormatManagerPtr);	
		
		void GetHighlightObjects(std::vector<std::pair<Int32, Int32> >& orHighlightObjects) const;
        void GetHighlightObjects(std::vector<TripleId>& orHighlightObjects) const;

		void HighlightSelectedObject(bool iHighlight, bool iIsForLassoSelction = false);
        void GetSOByLassoSelection(std::vector<std::pair<int,int> >& iObjects);
        void EnterGrayoutMode();
        void ExitGrayoutMode();
        bool IsGrayoutMode() const;
        void EnterDuplicateMode();
        void ExitDuplicateMode();
        bool IsDuplicateMode() const;
        
        void hCalculateInnerStrokeForConsectivePieSlice(TripleId iTripleId, Int32 lBeginIndex, Int32 lEndIndex);
        void hHandleHighLightObjects(const std::vector<TripleId>& irTripleIds);
		void hAddRedrawObjectsInHighLightView(const LayerObject::Ptr &iHighLightLayerObjectPtr, const TripleId& iTripleId);
        void hDeleteRedrawObjectsInHighLightView(const LayerObject::Ptr &iHighLightLayerObjectPtr, const TripleId& iTripleId);
        void SetChartOffset(Int32 iXOffset, Int32 iYOffset);
        void HighlightOrHideOneObject(const TripleId irTripleId, bool iIsHide);
        void HighlightOrHideObjects(const std::vector<TripleId>& irTripleIds, bool iIsHide);
        bool ClearOneHighlightObject(MsiChart::TripleId iTripleId);
		void HighlightSelectedInInit(std::vector<std::pair<Int32, Int32> >& iSelectedSeriesAndColumns);
#ifndef _VE_CE        
        void GetAllGraphObjectInfo(std::vector<CGRect>  &iAllGraphObjectsBorder, std::vector<MsiChart::TripleId > &iAllGraphObjectsTripleId);
#endif
        void SetGMZoomScale(float iHeightScale, float iWidthScase);
        
		void RefreshGraphMatrixContext();	
        void SetGMCEContext(void* iGMCEContext);
        void RefreshOnDataChange();
		Int32 GetSeriesColorFromPalette(Int32 iIndex);
		bool IsDataEnough();
		float GetMemoryUsageEstimation();
		void SetFontScale(double iRatio = 1.0);
		void SetTemplateNode(DSSRWTemplateNode* ipTemplateNode);
		double GetPositionAtTick(double iTickValue, bool iIsForXAxis, Int32 iGraphDimentionLength);
		void SetEntireness(bool iIsEntireChart);
		Int32 GetIntervalSpace(bool iIsXAxis);
		bool IsZeroBaseLineQualified(bool iIsXAxis, bool isSecondaryAxis);
        void DisableAnimationForZoom(bool iDisableAnimation);
		bool GetGroupSpace(std::vector<Int32>& orVect, Int32 iGroupIndex, bool iIsLegned = false);
		bool GetSpaceAt(std::vector<Int32>& orVect, const TripleId& irObjectId);
        Int32 GetAxisLabelRotaion(bool iIsXAxis);
		Int32 LayoutAxis(bool iIsXAxis, bool iIsRotated, Int32& orMaxLabelSpace);	
		bool IsIsolatedDot(const TripleId& irId);
        bool IsIsolatedDotInLine(const TripleId& irId);
        bool GetInterlacedBandingPosition(bool iIsXAxis, std::vector<double>& orPostions, bool iIsFromUniformAxis = true);
        bool RefreshChart(const RefreshPack& irRefreshPack);
        bool GetDataAreaPadding(std::vector<double>& orPaddings);
        bool IsInitialized() const;
        bool IsVerticalChart() const;
        void SetTextAsShape(bool iFlag);
        void cleanDataSet();
#ifdef _VE_CE
		bool isSeriesHaveControls() { return mSeriesHaveControls; }
		bool isGroupsHaveControls() { return mGroupsHaveControls; }
		bool isHasMetricSelector() { return mHasMetricSelector; }
#endif
		ChartContext* GetChartContext(void) { return mChartContextPtr.Get(); }

	protected:
		~ChartImpl();

	private:
		/**
		This function creates a cairo drawing environment.
		@param iFormat Specifies the image format, which could be PNG, BMP, GIF, JPEG, PDF, SVG.
		@return True means the cairo drawing environment is created successfully while false means the opposite.
		*/
		bool hCreateDrawEnv(Int32 iFormat);		

		/**
		This function creates a cairo drawing environment for Windows DC.
		@param iHdc The handler to Windows DC.
		@return True means the cairo drawing environment is created successfully while false means the opposite.
		*/
		bool hCreateWin32DrawEnv(HDC iHdc);

		/**
		This function traverses the GraphObjectManager tree and draw every GraphObject.
		*/
		void hDrawImage();

		/**
		This function returns the id of the GraphObjectManager which is responsible for the input property id.
		@param iPropertyId The property id.
		@param iObjectId The object id.
		@return The id of the GraphObjectManager which is responsible for the input property id.
		*/
		Int32 hGetGraphObjectManagerId(Int32 iPropertyId, Int32 iObjectId);		

		/**
		This function loads those properties used in ChartImpl from ChartContext.
		*/
		void hLoadProperties();

		//	bool hImageCoordinateToVirtualCoordinate(Int32 iImageX, Int32 iImageY, Int32& orVirtualX, Int32& orVirtualY);
		//Graph Imaging
		bool hPlainDraw(Int32 iWidth, Int32 iHeight);

		/**
		This function calculates the value of the data group for dataset according to the graph type as well as other related options.
		@return The data group value.
		*/
		void hGetDataGroupAndFlag(Int32& orDataGroup, bool& orFlag) const;
		
		/** Find the position of selected graph object for tooltip
		 @param iGraphObjectPtr, selected graph object
		 @param orDataPoint, the position of the selected graph object
		 @return if we find the data point for selected graph object
		 */
		bool hGetDataPointForToolTip(GraphObject::Ptr iGraphObjectPtr,Point2D&orDataPoint);
		
		/** Find the corresponding series color for selected graph object in tooltip
		 @return the color value in int.
		 */
		Int32 hGetSeriesColorForToolTip(GraphObject::Ptr iGraphObjectPtr);
		
		/**	Update Dataset Information at ctor.
		*/
		void hUpdateDataset();
		
		/**	Convert a string to ChartAdvFormat structure.
		@param lpFormatString, string to be converted.
		@param orFormat, the output 
		*/
		void hStringToFormat(const wchar_t* lpFormatString, ChartAdvFormat& orFormat);
		
		/** Clear up all the tooltips
		*/
		void hClearTooltips();
		
		/** To decide if X axis is Value axis
		@return ture for Yes and false for No.
		*/
		bool hIsXAxisValueAxis();

		/** To decide if Y axis is Category axis
		@return ture for Yes and false for No.
		*/
		bool hIsYAxisCategoryAxis();
		
		/** To highlight one graph object with Hover effect
		@param iGraphObjectPtr, the graph object to be hovered.
		@param iIsOn, whether to hover it or not, by default Yes.
		@return true means the hover is successful.
		*/
		bool hOnHover(GraphObject::Ptr iGraphObjectPtr, bool iIsOn = true);

		/** Helper function called by GetToolTipByCoords, to collect the information used by the tooltip in Graph Matrix
		@param iGraphObjectPtr, the graph object user taps on.
		@return the output tooltip structure. Null is returned if no tooltip is generated.
		 */
		Tooltip* hCreateGMTooltipByObject(GraphObject::Ptr iGraphObjectPtr);

		/** Helper function called by GetToolTipByCoords, to collect the information used by the tooltip in Graph Matrix
         @param iGraphObjectPtr, the graph object user taps on.
         @return the output tooltip structure. Null is returned if no tooltip is generated.
		 */
		Tooltip* hCreateBudgetingTooltipByObject(GraphObject::Ptr iGraphObjectPtr);

		/** Helper function called by GetToolTipByCoords, to collect the information used by the tooltip in Normal Graph
		 @param iGraphObjectPtr, the graph object user taps on.
		 @return the output tooltip structure. Null is returned if no tooltip is generated.
		 */
		Tooltip* hCreateTooltipByObject(GraphObject::Ptr iGraphObjectPtr);
        
        /** Helper function called by GetToolTipByCoords, to collect the information used by the tooltip in VI Graph
		 @param iGraphObjectPtr, the graph object user taps on.
		 @return the output tooltip structure. Null is returned if no tooltip is generated.
		 */
		Tooltip* hCreateVITooltipByObject(GraphObject::Ptr iGraphObjectPtr);
        
        /** The entry function to get tooltip, it will dispatch the logic to different helper functions.
		 @param iGraphObjectPtr, the graph object user taps on.
         @param iPersisitent if set as true, the property setting "DssGraphSDShowTooltip" will be ignored.
		 @return the output tooltip structure. Null is returned if no tooltip is generated.
         */
        Tooltip* hGenerateTooltipByObject(GraphObject::Ptr iGraphObjectPtr, bool iPersistent = false);
        
        /** If the tooltip is enabled (for all/a object)
         @param iGraphObjectPtr, the target object to show tooltip for.
         @return true means tooltip is eligible, and false otherwise.
         */
        bool hIsTooltipEnabled() const;
        bool hIsTooltipEnabled(GraphObject::Ptr iGraphObjectPtr) const;
        bool hIsTooltipEnabled(const TripleId& irId) const;
        
        /**Decide if a tooltip can be shown for graph object
         *@param iGraphObjectPtr, the graphObject for the tooltip.
         *@param iPersisitent if set as true, the property setting "DssGraphSDShowTooltip" will be ignored.
         */
        bool hCanShowTooltipByObject(GraphObject::Ptr iGraphObjectPtr, bool iPersistent = false) const;
        
		/** To set the axiscross property to the specified category axis
		 @dicsussion : We should set the axiscross property to uniform axis to be consistent with plot side.
		 @param ipAxis, the target category axis
		 */
		void hUpdateGMCategoryAxisCross(CategoryAxis* ipAxis); 
		
		/** To resove the macro string wrapped in ipString.
		 */
		void hReplaceNewLineWithSpace(char* ipString) const;
		
		/** To get the min group count according to graphtype.
		 */
		int hMinGroupCountByGraphType(Int32 iGraphType);
#ifdef _VE_CE		
		/** judge whether the current area is selected or not
		*/
		bool hIsSelected(short iSID, short iGID, std::vector<std::pair<Int32, Int32> >& iSelectedSeriesAndColumns);
#endif
		/** To get the resolved global marco text.
		 */
		std::string hGetResolvedGlobalMacro(const std::string& irGlobalMacro, bool& orIsResolved);

		/** Remove the duplicated items from lObjectlist by comparing content to lCandidateObjects
		@param orCandidateObjects: output list
		@param irObjectList: object to pick
		@param iObjectPtr, the 1st object to be put in orCandidateObjects
		*/
		void hFilterDuplicationFromList(std::vector<GraphObject::Ptr>& orCandidateObjects, const std::vector<GraphObject::Ptr>& irObjectList, GraphObject::Ptr iObjectPtr,  const Point2D& irCursor);
		
        /** This method would be used to prepare the object id collection, against which the tooltip search is carried out.
         @param orObjectIds, the object id collection.
         */
        void hPrepareTargetObjectIds(std::vector<Int32>& orObjectIds, Int32 iType);
        
        
		/**Check if the target object Id is in the list
		@return ture if iObjectId is in irObjectIdList
		*/
		bool hIsInObjectIdList(Int32 iObjectId, const std::vector<Int32>& irObjectIdList);

        /** Update ChartContext::mHighlightObjectList with mSelectObjectPtr
         */
        void hUpdateHighlightObjectList();
        
        /** Decide how many margins should be trimeed from the passed-in rect, used in Graph matrix.
         */
        Int32 hGetMarginCount();
        
        /**
         * This is a helper method called by getPositionById. This is to get position by axis.
         * @param iSeriesId seriesIndex.
         * @param iGroupId groupIndex
         * @param orX, result of x coordinate.
         * @param orY, result of y coordinate.
         * @return boolean to indicate if the computation is successful.
         */
        bool hComputePositionById(Int32 iSeriesId, Int32 iGroupId, Int32& orX, Int32& orY);
        
        /**
         * This is a helper method called by getPositionById. This is to get position by object.
         * @param iSeriesId seriesIndex.
         * @param iGroupId groupIndex
         * @param orX, result of x coordinate.
         * @param orY, result of y coordinate.
         * @return boolean to indicate if the computation is successful.
         */
        bool hComputePositionByObject(Int32 iSeriesId, Int32 iGroupId, Int32& orX, Int32& orY);
        
        
		/* helper function to GetTooltipDataString.
		@param iValue, the returned value from GetTooltipDataString
		@param iPercentValue, the returned percentage from GetTooltipDataString
		*/
		bool hFormatTooltipDataString(Int32 iSeriesID, Int32 iGroupID, MBase::String& orDataString, Int32 iOffset, double iValue, double iPercentValue, bool isForPercentValue = false, Int32 iAngleId = -3, Int32 iSliceId = -3);

        Axis* hGetAxis(bool iIsXAxis, bool iIsUniformAxis, bool iIsPrimary = true); 

		bool mFontAntiAlias;					///< Indicates whether the font anti-alias is used or not.
		std::vector<TripleId> mSelectionList;	///< The selection list.
		Dataset::Ptr mDatasetPtr;				///< The datases which is the data source for chart engine.
		FormatManager::Ptr mFormatManagerPtr;
		ChartManager::Ptr mChartManagerPtr;		///< A ChartManager instance which is the root of the GraphObjectManager tree.
		ChartManager::Ptr mOldChartManagerPtr;		///< An old ChartManager instance for animation support.
		MSynch::SmartPtr<ChartContext>	mChartContextPtr;	///< The ChartContext which holds all the propertis for chart engine.
		bool mInitialized;		
		
		std::vector<Tooltip*> mTooltips;			///< Maintains tooltip information.
        
		Axis::Ptr mUniformYAxisPtr;
		Axis::Ptr mUniformXAxisPtr;
        // Dual Axis Support
        Axis::Ptr mUniformY2AxisPtr;
		Axis::Ptr mUniformX2AxisPtr;
		
		LayerObject::Ptr mBaseLayerObjectPtr;
		GraphObject::Ptr mSelectedObjectPtr;
        GraphObject::Ptr mSelectedLineObjectPtr;
		DSSRWTemplateNode* mpRWTemplateNode;
        bool mIsShowHighLightedObj;

#ifdef _VE_CE
		bool mSeriesHaveControls;
		bool mGroupsHaveControls;
		bool mHasMetricSelector;
#endif
        
        // lasso selections
        std::vector<GraphObject::Ptr> mSelectedObjectsForLassoSelection;
	};
}

#endif
